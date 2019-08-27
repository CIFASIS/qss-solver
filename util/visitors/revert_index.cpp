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

#include "revert_index.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../error.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

RevertIndex::RevertIndex() {}

AST_Expression RevertIndex::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_ExpressionList l = newAST_ExpressionList();
      foreach (it, indexes) {
        l = AST_ListAppend(l, apply(current_element(it)));
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

AST_Expression RevertIndex::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  switch (bot) {
  case BINOPADD:
    return newAST_Expression_BinOp(l, r, BINOPSUB);
  case BINOPSUB:
    return newAST_Expression_BinOp(l, r, BINOPADD);
  case BINOPDIV:
    return newAST_Expression_BinOp(l, r, BINOPMULT);
  case BINOPMULT:
    return newAST_Expression_BinOp(l, r, BINOPDIV);
  default:
    break;
  }
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
