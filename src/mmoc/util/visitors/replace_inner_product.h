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

#pragma once

#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {

class ReplaceInnerProduct : public AST_Expression_Visitor<AST_Expression> {
  public:
  ReplaceInnerProduct();
  ~ReplaceInnerProduct() = default;

  private:
  bool controlArray(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);
};
}  // namespace Util
}  // namespace MicroModelica
