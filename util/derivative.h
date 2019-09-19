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
#include "util_types.h"
#include "symbol_table.h"
#include "../ir/expression.h"

namespace MicroModelica {
namespace Util {

class MMO_Expression;

/**
 *
 */
class EquationDerivator {
  public:
  /**
   *
   * @param eq
   * @param varEnv
   * @return
   */
  static AST_Equation_Equality derivate(AST_Equation_Equality eq, const VarSymbolTable& symbols);
};

/**
 *
 */
class ExpressionDerivator {
  public:
  /**
   *
   * @param exp
   * @param varEnv
   * @param e
   * @return
   */
  static AST_Expression derivate(AST_Expression exp, const VarSymbolTable& symbols, IR::Expression e);
  map<string, MicroModelica::IR::Expression> generateJacobianExps(Variable variable, std::string usage, AST_Expression exp,
                                                                  const VarSymbolTable& symbols);
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* DERIVATIVE_H_ */
