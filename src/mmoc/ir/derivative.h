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

#ifndef DERIVATIVE_H_
#define DERIVATIVE_H_

#include <map>

#include "../ast/ast_types.h"
#include "../deps/dependency_matrix.h"
#include "../util/util_types.h"
#include "../util/visitors/jac_alg_exps.h"
#include "index.h"
#include "equation.h"
#include "expression.h"

namespace MicroModelica {
namespace IR {

class EquationDerivator {
  public:
  static AST_Equation_Equality derivate(AST_Equation_Equality eq);
};

class ExpressionDerivator {
  public:
  ExpressionDerivator();
  ~ExpressionDerivator() = default;

  static AST_Expression derivate(AST_Expression exp, Expression e);
  void generateJacobian(Index index, Equation eq, Deps::AlgebraicPath path);
  AST_Expression jacobianVariable(std::string name);
  list<Equation> terms() const;

  protected:
  AST_Expression generateJacobianExp(Index index, Equation exp);
  Equation generateEquation(AST_Expression lhs, AST_Expression rhs, Option<Range> range, int id);

  bool checkExpression(AST_Expression exp);

  list<Equation> _chain_rule_terms;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* DERIVATIVE_H_ */
