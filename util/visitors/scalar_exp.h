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

#ifndef EVAL_EXP_H_
#define EVAL_EXP_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
/**
 * Evaluates an arithmetic expression.
 */
class EvalExp : public AST_Expression_Fold<AST_Expression> {
  public:
  /**
   *
   * @param symbolTable
   */
  EvalExp(VarSymbolTable symbolTable);
  /**
   * Evaluates an arithmetic expression.
   */
  /**
   *
   * @param exp
   * @return
   */
  AST_Expression eval(AST_Expression exp);
  /**
   * Evaluates an arithmetic expression replacing the occurrences of compRef with the value provided (compRefValue).
   */
  /**
   *
   * @param compRef
   * @param compRefValue
   * @param exp
   * @return
   */
  AST_Expression eval(AST_Expression_ComponentReference compRef, AST_Expression compRefValue, AST_Expression exp);

  private:
  AST_Expression foldTraverseElement(AST_Expression);
  AST_Expression foldTraverseElementUMinus(AST_Expression);
  AST_Expression foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
  AST_Expression evalCompRef(AST_Expression_ComponentReference compRef);
  AST_Expression evalArray(AST_Expression_ComponentReference exp);
  bool shouldReturnInteger(AST_Expression left, AST_Expression right);
  bool shouldReturnReal(AST_Expression left, AST_Expression right);
  AST_Real getRealVal(AST_Expression exp);
  AST_Expression_ComponentReference _compRef;
  AST_Expression _compRefVal;
  VarSymbolTable _symbolTable;
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* EVAL_EXP_H_ */
