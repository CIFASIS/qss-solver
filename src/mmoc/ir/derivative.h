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
  Equation generateJacobianExp(Index index, Equation exp);
  void generateJacobianTerm(Index index, Deps::VariableDependency var);
  list<Equation> terms() const;

  protected:
  Equation generateEquation(std::string usage, Expression exp, Equation orig);
  AST_ExpressionList generateChainRule(list<Util::JacAlgTerm> alg_terms);
  AST_Expression jacobianExp(AST_Expression exp, AST_ExpressionList algs);
  std::string termVariable();
  std::string lookup(Index index, Option<Range> range);

  list<pair<Equation, string>> _alg_terms;
  list<Equation> _der_terms;
  int _term_num;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* DERIVATIVE_H_ */
