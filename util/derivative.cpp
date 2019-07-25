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
#include "../util/visitors/replace_der.h"
#include "ginac_interface.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

AST_Equation_Equality EquationDerivator::derivate(AST_Equation_Equality eq, const VarSymbolTable& varEnv)
{
  ConvertToGiNaC tog(varEnv, Option<Expression>());
  ConvertToExpression toe;
  GiNaC::ex left = tog.convert(eq->left());
  GiNaC::ex right = tog.convert(eq->right());
  GiNaC::symbol time = tog.getTime();
  GiNaC::ex der_left = left.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  GiNaC::ex der_right = right.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return (newAST_Equation_Equality(toe.convert(der_left), toe.convert(der_right))->getAsEquality());
}

AST_Expression ExpressionDerivator::derivate(AST_Expression exp, const VarSymbolTable& varEnv, Expression e)
{
  ConvertToGiNaC tog(varEnv, e);
  ConvertToExpression toe;
  GiNaC::ex dexp = tog.convert(exp, false, true);
  GiNaC::symbol time = tog.getTime();
  GiNaC::ex der_exp = dexp.diff(time).subs(var(GiNaC::wild(), time) == GiNaC::wild());
  return toe.convert(der_exp);
}

map<string, Expression> ExpressionDerivator::generateJacobianExps(AST_Expression exp, const VarSymbolTable& vt)
{
  ConvertToGiNaC tog(vt, Option<Expression>());
  ConvertToExpression toe;
  ReplaceDer rd(vt);
  GiNaC::ex dexp = tog.convert(exp, false, true);
  map<string, GiNaC::symbol> dir = tog.directory();
  map<string, GiNaC::symbol>::iterator it;
  map<string, Expression> jacobianExps;
  GiNaC::symbol time = tog.getTime();
  for (it = dir.begin(); it != dir.end(); it++) {
    Option<Variable> v = vt[tog.identifier(it->first)];
    if (v) {
      if (v->isState() || v->isAlgebraic()) {
        GiNaC::ex der_exp = dexp.subs(var(GiNaC::wild(), time) == GiNaC::wild()).diff(it->second);
        jacobianExps[it->first] = Expression(rd.apply(toe.convert(der_exp)), vt);
      }
    }
  }
  return jacobianExps;
}
}  // namespace Util
}  // namespace MicroModelica
