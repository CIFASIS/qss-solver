/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "derivative.h"

#include <ginac/function.h>
#include <ginac/operators.h>
#include <ginac/relational.h>
#include <ginac/symbol.h>
#include <ginac/wildcard.h>

#include "../ast/ast_builder.h"
#include "../ast/equation.h"
#include "../ir/expression.h"
#include "../parser/parse.h"
#include "../util/error.h"
#include "../util/ginac_interface.h"
#include "../util/util.h"
#include "../util/symbol_table.h"
#include "../util/visitors/replace_der.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

ExpressionDerivator::ExpressionDerivator() : _alg_terms(), _der_terms(), _term_num(0) {}

AST_Equation_Equality EquationDerivator::derivate(AST_Equation_Equality eq)
{
  VarSymbolTable symbols = Utils::instance().symbols();
  ConvertToGiNaC to_ginac(symbols, Option<Expression>());
  ConvertToExpression to_exp;
  GiNaC::ex left = to_ginac.convert(eq->left());
  GiNaC::ex right = to_ginac.convert(eq->right());
  GiNaC::symbol time = to_ginac.getTime();
  GiNaC::ex der_left = left.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  GiNaC::ex der_right = right.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return (newAST_Equation_Equality(to_exp.convert(der_left), to_exp.convert(der_right))->getAsEquality());
}

AST_Expression ExpressionDerivator::derivate(AST_Expression exp, Expression e)
{
  VarSymbolTable symbols = Utils::instance().symbols();
  ConvertToGiNaC to_ginac(symbols, e);
  ConvertToExpression to_exp;
  GiNaC::ex dexp = to_ginac.convert(exp, false, true);
  GiNaC::symbol time = to_ginac.getTime();
  GiNaC::ex der_exp = dexp.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return to_exp.convert(der_exp);
}

string ExpressionDerivator::lookup(Index index, Option<Range> range)
{
  for (pair<Equation, string> terms : _alg_terms) {
    Equation eq = terms.first;
    Option<Variable> var = eq.LHSVariable();
    assert(var);
    Variable usage = index.variable();
    if (var->name() == usage.name()) {
      // Compare ranges to see if the replace is a match.
      Option<Range> eq_range = eq.range();
      if (!eq_range && !range) {
        return terms.second;
      } else if (eq_range && range) {
        if (range->intersect(eq.range().get())) {
          return terms.second;
        }
      }
    }
  }
  assert(false);
  return "";
}

AST_ExpressionList ExpressionDerivator::generateChainRule(list<JacAlgTerm> alg_terms)
{
  VarSymbolTable symbols = Utils::instance().symbols();
  ConvertToGiNaC to_ginac(symbols, Option<Expression>());
  ConvertToExpression to_exp;
  ReplaceDer replace_der(symbols);
  AST_ExpressionList crs = newAST_ExpressionList();
  for (JacAlgTerm jac_alg_term : alg_terms) {
    AST_ExpressionListIterator it;
    AST_ExpressionList alg_vars = jac_alg_term.algExps();
    foreach (it, alg_vars) {
      AST_Expression e = current_element(it);
      Index index = Index(Expression(e, symbols));
      string usage = index.modelicaExp();
      Variable variable = index.variable();
      GiNaC::ex dexp = to_ginac.convert(jac_alg_term.algTerm(), false, true);
      map<string, GiNaC::symbol> dir = to_ginac.directory();
      GiNaC::symbol time = to_ginac.getTime();
      assert(variable.isState() || variable.isAlgebraic());
      GiNaC::symbol ginac_usage = dir[usage];
      GiNaC::ex der_exp = dexp.subs(var(GiNaC::wild(), time) == GiNaC::wild()).diff(ginac_usage);
      AST_Expression jac_exp = replace_der.apply(to_exp.convert(der_exp));
      string rep_var = lookup(index, jac_alg_term.range());
      AST_Expression rep = newAST_Expression_ComponentReferenceExp(newAST_String(rep_var));
      AST_Expression term = newAST_Expression_BinOp(jac_exp, rep, BINOPMULT);
      AST_ListAppend(crs, term);
    }
  }
  return crs;
}

AST_Expression ExpressionDerivator::jacobianExp(AST_Expression exp, AST_ExpressionList algs)
{
  AST_ExpressionListIterator it;
  AST_Expression sum = exp;
  foreach (it, algs) {
    AST_Expression e = current_element(it);
    sum = newAST_Expression_BinOp(sum, e, BINOPSUB);
  }
  return sum;
}

void ExpressionDerivator::generateJacobianTerm(Index index, Deps::VariableDependency var)
{
  EquationTable algebraic = ModelConfig::instance().algebraics();
  Option<Equation> alg = algebraic[var.equationId()];
  assert(alg);
  _der_terms.push_back(generateJacobianExp(index, alg.get()));
}

string ExpressionDerivator::termVariable()
{
  stringstream buffer;
  buffer << "__ch" << _term_num++;
  return buffer.str();
}

Equation ExpressionDerivator::generateEquation(std::string usage, Expression exp, Equation orig)
{
  VarSymbolTable symbols = Utils::instance().symbols();
  int res = 0;
  string lhs_usage = usage;
  if (orig.isAlgebraic()) {
    lhs_usage = termVariable();
    _alg_terms.push_back(make_pair(orig, lhs_usage));
  }
  AST_Expression lhs = parseExpression(lhs_usage, &res);
  if (res) {
    Error::instance().add(0, EM_IR | EM_PARSE_FILE, ER_Fatal, "Can't generate Jacobian equation left hand side for: %s", usage.c_str());
  }
  return Equation(lhs, exp.expression(), symbols, orig.range(), EQUATION::Jacobian, orig.id());
}

Equation ExpressionDerivator::generateJacobianExp(Index index, Equation eq)
{
  AST_Expression exp = eq.rhs().expression();
  Variable variable = index.variable();
  string usage = index.modelicaExp();
  VarSymbolTable symbols = Utils::instance().symbols();
  ConvertToGiNaC to_ginac(symbols, Option<Expression>());
  ConvertToExpression to_exp;
  ReplaceDer replace_der(symbols);
  // Get the expression terms that doesn't have algebraic variables.
  Option<Variable> lhs = eq.LHSVariable();
  assert(lhs);
  JacAlgExps jac_alg_exps(lhs.get(), eq.range());
  AST_Expression state_terms = jac_alg_exps.apply(exp);
  AST_ExpressionList alg_terms = generateChainRule(jac_alg_exps.algebraicTerms());
  GiNaC::ex dexp = to_ginac.convert(state_terms, false, true);
  map<string, GiNaC::symbol> dir = to_ginac.directory();
  GiNaC::symbol time = to_ginac.getTime();
  assert(variable.isState() || variable.isAlgebraic());
  GiNaC::symbol ginac_usage = dir[usage];
  GiNaC::ex der_exp = dexp.subs(var(GiNaC::wild(), time) == GiNaC::wild()).diff(ginac_usage);
  AST_Expression jac_exp = jacobianExp(replace_der.apply(to_exp.convert(der_exp)), alg_terms);
  Equation jac = generateEquation(usage, Expression(jac_exp, symbols), eq);
  jac.setUsage(index);
  return jac;
}

}  // namespace IR
}  // namespace MicroModelica
