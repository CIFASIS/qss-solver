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

#ifndef PARTIAL_EVAL_EXP_H_
#define PARTIAL_EVAL_EXP_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
/**
 * Evaluates an arithmetic expression.
 */
class PartialEvalExp : public AST_Expression_Fold<AST_Expression> {
  public:
  /**
   *
   * @param symbolTable
   */
  PartialEvalExp(VarSymbolTable symbolTable);

  private:
  AST_Expression foldTraverseElement(AST_Expression);
  AST_Expression foldTraverseElementUMinus(AST_Expression);
  AST_Expression foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
  bool shouldReturnInteger(AST_Expression left, AST_Expression right);
  VarSymbolTable _symbols;
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* PARTIAL_EVAL_EXP_H_ */
