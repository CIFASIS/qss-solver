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

#include "get_index_variables.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../error.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

GetIndexVariables::GetIndexVariables() : _in_index_list(false), _pos(0) {}

map<std::string, int> GetIndexVariables::foldTraverseElement(AST_Expression exp)
{
  map<std::string, int> ret;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (_in_index_list) {
      ret.insert(std::make_pair(cr->name(), _pos));
    }
    if (cr->hasIndexes()) {
      assert(!_in_index_list);
      _in_index_list = true;
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      _pos = 1;
      foreach (it, indexes) {
        map<std::string, int> args = apply(current_element(it));
        ret.insert(args.begin(), args.end());
        _pos++;
      }
      _in_index_list = false;
    }
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output out = exp->getAsOutput();
    AST_ExpressionListIterator it;
    foreach (it, out->expressionList()) {
      map<std::string, int> args = apply(current_element(it));
      ret.insert(args.begin(), args.end());
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