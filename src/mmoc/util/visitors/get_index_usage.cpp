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

#include "../error.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

GetIndexUsage::GetIndexUsage() : _in_index_list(false) {}

Usage GetIndexUsage::foldTraverseElement(AST_Expression exp)
{
  Usage ret;
  static constexpr int USED = 1;
  static constexpr int UNUSED = -1;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (_in_index_list) {
      ret.push_back(USED);
    }
    if (cr->hasIndexes()) {
      assert(!_in_index_list);
      _in_index_list = true;
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      foreach (it, indexes) {
        Usage dim_usage = apply(current_element(it));
        bool used_dim = false;
        for (int u : dim_usage) {
          if (u == USED) {
            used_dim = true;
            break;
          }
        }
        if (used_dim) {
          ret.push_back(USED);
        } else {
          ret.push_back(UNUSED);
        }
      }
      _in_index_list = false;
    }
    break;
  }
  default:
    if (_in_index_list) {
      ret.push_back(UNUSED);
    }
    break;
  }
  return ret;
}

Usage GetIndexUsage::foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); }

Usage GetIndexUsage::foldTraverseElement(Usage l, Usage r, BinOpType bot)
{
  l.join(r);
  return l;
}

}  // namespace Util
}  // namespace MicroModelica