

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

#include <ir/index.h>
#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {

class GetIndexUsage : public AST_Expression_Visitor<IR::Usage> {
  public:
  GetIndexUsage();
  ~GetIndexUsage() = default;

  private:
  IR::Usage foldTraverseElement(AST_Expression exp);
  IR::Usage foldTraverseElementUMinus(AST_Expression exp);
  IR::Usage foldTraverseElement(IR::Usage l, IR::Usage r, BinOpType bot);
  bool _in_index_list;
};

}  // namespace Util
}  // namespace MicroModelica
