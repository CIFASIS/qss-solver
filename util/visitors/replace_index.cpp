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

#include "replace_index.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../error.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

ReplaceIndex::ReplaceIndex(Range range, Usage usage) : _range(range), _usage(usage) {}

AST_Expression ReplaceIndex::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      int i = 0;
      AST_ExpressionList l = newAST_ExpressionList();
      foreach (it, indexes) {
        if (_usage.isUsed(i)) {
          ReplaceVar rv(_range.iterator(i));
          l = AST_ListAppend(l, rv.apply(current_element(it)));
        } else {
          l = AST_ListAppend(l, current_element(it));
        }
        i++;
      }
      ret = AST_Expression_ComponentReference_Add(ret, newAST_String(cr->name()), l);
      return ret;
    }
    break;
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceVar::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    return newAST_Expression_ComponentReferenceExp(newAST_String(_var));
  }
  default:
    break;
  }
  return exp;
}
}  // namespace Util
}  // namespace MicroModelica
