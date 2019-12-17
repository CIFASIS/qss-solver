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

#ifndef CONVERT_SUM_H_
#define CONVERT_SUM_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ConvertSum : public AST_Expression_Visitor<AST_Expression> {
  public:
  ConvertSum(VarSymbolTable &symbols);
  ~ConvertSum() = default;

  bool hasSum();

  list<AST_Equation> code();

  list<Variable> variables();

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);

  VarSymbolTable &_symbols;
  bool _has_sum;
  list<AST_Equation> _code;
  list<Variable> _variables;
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* CONVERT_SUM_H_ */
