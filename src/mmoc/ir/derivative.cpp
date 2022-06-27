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
#include "../util/model_config.h"
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

ExpressionDerivator::ExpressionDerivator() {}

AST_Equation_Equality EquationDerivator::derivate(AST_Equation_Equality eq)
{
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  ConvertToGiNaC to_ginac= ConvertToGiNaC(Option<Expression>());
  ConvertToExpression to_exp;
  GiNaC::ex left = to_ginac.convert(eq->left());
  GiNaC::ex right = to_ginac.convert(eq->right());
  GiNaC::symbol time = to_ginac.getTime();
  GiNaC::ex der_left = left.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  GiNaC::ex der_right = right.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return (newAST_Equation_Equality(to_exp.convert(der_left), to_exp.convert(der_right), eq->comment())->getAsEquality());
}

AST_Expression ExpressionDerivator::derivate(AST_Expression exp, Expression e)
{
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  ConvertToGiNaC to_ginac = ConvertToGiNaC(e);
  ConvertToExpression to_exp;
  GiNaC::ex dexp = to_ginac.convert(exp, false, true);
  GiNaC::symbol time = to_ginac.getTime();
  GiNaC::ex der_exp = dexp.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return to_exp.convert(der_exp);
}

Expression ExpressionDerivator::partialDerivative(Equation eq, Index variable)
{
  Variable var_usage = variable.variable();
  assert(var_usage.isState() || var_usage.isAlgebraic());
  AST_Expression rhs_exp = eq.rhs().expression();
  string usage = variable.modelicaExp();
  ConvertToGiNaC to_ginac= ConvertToGiNaC(Option<Expression>());
  ConvertToExpression to_exp;
  ReplaceDer replace_der;
  GiNaC::ex dexp = to_ginac.convert(rhs_exp, false, true);
  map<string, GiNaC::symbol> dir = to_ginac.directory();
  GiNaC::symbol time = to_ginac.getTime();
  GiNaC::symbol ginac_usage = dir[usage];
  GiNaC::ex der_exp = dexp.subs(var(GiNaC::wild(), time) == GiNaC::wild()).diff(ginac_usage);
  AST_Expression jac_exp = replace_der.apply(to_exp.convert(der_exp));
  return Expression(jac_exp);
}

}  // namespace IR
}  // namespace MicroModelica
