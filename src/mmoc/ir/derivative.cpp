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
#include "../util/visitors/is_constant_expression.h"
#include "../util/visitors/replace_der.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

ExpressionDerivator::ExpressionDerivator() : _chain_rule_terms() {}

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

list<Equation> ExpressionDerivator::terms() const { return _chain_rule_terms; };

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

Equation ExpressionDerivator::generateEquation(AST_Expression lhs, AST_Expression rhs, Option<Range> range, int id)
{
  VarSymbolTable symbols = Utils::instance().symbols();
  IsConstantExpression constant_exp;
  Option<Range> eq_range;
  if (!constant_exp.apply(rhs)) {
    eq_range = range;
  }
  Equation eq(lhs, rhs, symbols, eq_range, EQUATION::JacobianTerm, id);
  if (!checkExpression(rhs)) {
    return Equation();
  }
  return eq;
}

AST_Expression ExpressionDerivator::jacobianVariable(string name) { return newAST_Expression_ComponentReferenceExp(newAST_String(name)); }

bool ExpressionDerivator::checkExpression(AST_Expression exp)
{
  if (exp->expressionType() == EXPINTEGER) {
    if (exp->getAsInteger()->val() == 0) {
      return false;
    }
  }
  return true;
}

void ExpressionDerivator::generateJacobian(Index index, Equation eq, Deps::AlgebraicPath path)
{
  bool chain_rule = false;
  AST_Expression jac_exp_lhs = jacobianVariable("_jac_exp");
  AST_Expression lhs = jacobianVariable("_chain_rule");
  Index alg_index;
  Index ch_alg_index;
  AlgebraicPath::reverse_iterator it;
  VariableDependency var;
  for (it = path.rbegin(); it != path.rend(); it++) {
    var = *it;
    EquationTable algebraic_eqs = ModelConfig::instance().algebraics();
    Option<Equation> alg = algebraic_eqs[var.equationId()];
    if (alg) {
      alg_index = var.ife();
      Equation alg_usage = alg.get();
      alg_usage.applyUsage(alg_index);
      Equation chain_rule_eq;
      if (chain_rule) {
        AST_Expression exp = generateJacobianExp(ch_alg_index, alg_usage);
        chain_rule_eq = generateEquation(lhs, newAST_Expression_BinOp(exp, lhs, BINOPMULT), alg_usage.range(), alg->id());
      } else {
        AST_Expression exp = generateJacobianExp(index, alg_usage);
        chain_rule_eq = generateEquation(lhs, exp, alg_usage.range(), alg->id());
        chain_rule = !chain_rule_eq.isEmpty();
      }
      if (!chain_rule_eq.isEmpty()) {
        ch_alg_index = alg_index;
        chain_rule_eq.setUsage(alg_index);
        _chain_rule_terms.push_back(chain_rule_eq);
      }
    }
  }
  AST_Expression exp = generateJacobianExp(index, eq);
  Equation jac_eq;
  if (chain_rule) {
    Equation usage_eq = eq;
    usage_eq.dependencyUsage(var, alg_index);
    AST_Expression mult = newAST_Expression_BinOp(generateJacobianExp(alg_index, usage_eq), lhs, BINOPMULT);
    AST_Expression sum = newAST_Expression_BinOp(mult, exp, BINOPADD);
    jac_eq = generateEquation(jac_exp_lhs, sum, usage_eq.range(), usage_eq.id());
  } else {
    jac_eq = generateEquation(jac_exp_lhs, exp, eq.range(), eq.id());
  }
  jac_eq.setUsage(index);
  _chain_rule_terms.push_back(jac_eq);
}

AST_Expression ExpressionDerivator::generateJacobianExp(Index index, Equation eq)
{
  Equation usage_eq = eq;
  AST_Expression rhs_exp = usage_eq.rhs().expression();
  Variable variable = index.variable();
  string usage = index.modelicaExp();
  VarSymbolTable symbols = Utils::instance().symbols();
  ConvertToGiNaC to_ginac(symbols, Option<Expression>());
  ConvertToExpression to_exp;
  ReplaceDer replace_der(symbols);
  Option<Variable> lhs = eq.LHSVariable();
  assert(lhs);
  GiNaC::ex dexp = to_ginac.convert(rhs_exp, false, true);
  map<string, GiNaC::symbol> dir = to_ginac.directory();
  GiNaC::symbol time = to_ginac.getTime();
  assert(variable.isState() || variable.isAlgebraic());
  GiNaC::symbol ginac_usage = dir[usage];
  GiNaC::ex der_exp = dexp.subs(var(GiNaC::wild(), time) == GiNaC::wild()).diff(ginac_usage);
  AST_Expression jac_exp = replace_der.apply(to_exp.convert(der_exp));
  return jac_exp;
}

}  // namespace IR
}  // namespace MicroModelica
