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

#include "get_index_usage.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../error.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

GetIndexUsage::GetIndexUsage() : _in_index_list(false) {}

list<string> GetIndexUsage::foldTraverseElement(AST_Expression exp)
{
  list<string> ret;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (_in_index_list) {
      ret.push_back(cr->name());
    }
    if (cr->hasIndexes()) {
      assert(!_in_index_list);
      _in_index_list = true;
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      foreach (it, indexes) {
        ret.splice(ret.end(), foldTraverseElement(current_element(it)));
      }
      _in_index_list = false;
    }
    break;
  }
  default:
    break;
  }
  return ret;
}

}  // namespace Util
}  // namespace MicroModelica
