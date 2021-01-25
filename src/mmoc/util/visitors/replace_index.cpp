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
#include "../model_config.h"
#include "../util.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

ReplaceIndex::ReplaceIndex(Range range, Usage usage, bool range_idxs) : _range(range), _usage(usage), _range_idxs(range_idxs) {}

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
      assert(indexes->size() == _usage.Size());
      foreach (it, indexes) {
        if (_usage.isUsed(i)) {
          string var = _range.iterator(i, _range_idxs);
          ReplaceVar rv(var);
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

AST_Expression ReplaceIndex::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceIndex::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

ReplaceVar::ReplaceVar(string var) : _var(var) {}

AST_Expression ReplaceVar::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    VarSymbolTable symbols = ModelConfig::instance().symbols();
    Option<Variable> var = symbols[exp->getAsComponentReference()->name()];
    assert(var);
    if (!var->isConstant()) {
      return newAST_Expression_ComponentReferenceExp(newAST_String(_var));
    }
    return exp;
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceVar::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceVar::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
