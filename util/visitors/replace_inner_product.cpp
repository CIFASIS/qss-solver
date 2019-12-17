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

#include "replace_inner_product.h"

#include "../../ast/ast_builder.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

ReplaceInnerProduct::ReplaceInnerProduct(VarSymbolTable& symbols) : _symbols(symbols) {}

AST_Expression ReplaceInnerProduct::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCALL: {
    string name = *(exp->getAsCall()->name());
    AST_ExpressionList nel = newAST_ExpressionList();
    AST_ExpressionList el = exp->getAsCall()->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      AST_ListAppend(nel, apply(current_element(it)));
    }
    AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL, nel);
    return ret;
  }
  case EXPCALLARG: {
    AST_ExpressionList el = exp->getAsCallArgs()->arguments();
    AST_ExpressionListIterator it;
    AST_ExpressionList nel = newAST_ExpressionList();
    foreach (it, el) {
      AST_ListAppend(nel, apply(current_element(it)));
    }
    return newAST_Expression_FunctionCallArgs(nel);
  }
  case EXPBRACE: {
    AST_ExpressionList el = exp->getAsBrace()->arguments();
    AST_ExpressionListIterator it;
    AST_ExpressionList nel = newAST_ExpressionList();
    foreach (it, el) {
      AST_ListAppend(nel, apply(current_element(it)));
    }
    return newAST_Expression_Brace(nel);
  }
  case EXPOUTPUT: {
    AST_ExpressionList el = exp->getAsOutput()->expressionList();
    AST_ExpressionListIterator eli;
    AST_ExpressionList ret = newAST_ExpressionList();
    foreach (eli, el) {
      AST_ListAppend(ret, apply(current_element(eli)));
    }
    return newAST_Expression_OutputExpressions(ret);
  }
  default:
    return exp;
  }
  return NULL;
}

AST_Expression ReplaceInnerProduct::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  if (bot == BINOPMULT) {
    if (controlArray(l) && controlArray(r)) {
      string name = "__INNER_PRODUCT";
      AST_ExpressionList nel = newAST_ExpressionList();
      AST_ListAppend(nel, l);
      AST_ListAppend(nel, r);
      AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL, nel);
      return ret;
    }
  }
  return newAST_Expression_BinOp(l, r, bot);
}

AST_Expression ReplaceInnerProduct::foldTraverseElementUMinus(AST_Expression exp) { return exp; }

bool ReplaceInnerProduct::controlArray(AST_Expression exp)
{
  if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> vi = _symbols[cr->name()];
    if (!vi) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "Control Array: %s", cr->name().c_str());
    }
    if (!cr->hasIndexes() && vi->isArray()) {
      return true;
    }
  }
  return false;
}
}  // namespace Util
}  // namespace MicroModelica
