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

#include "replace_der.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

ReplaceDer::ReplaceDer() : {}

AST_Expression ReplaceDer::foldTraverseElement(AST_Expression exp)
{
  stringstream ret(stringstream::out);
  switch (exp->expressionType()) {
  case EXPCALL: {
    AST_Expression_Call ec = exp->getAsCall();
    string name = *(ec->name());
    if (!name.compare("der2")) {
      return (newAST_Expression_Derivative(newAST_ExpressionList(newAST_Expression_Derivative(ec->arguments()))));
    } else if (!name.compare("der3")) {
      return (newAST_Expression_Derivative(
          newAST_ExpressionList(newAST_Expression_Derivative(newAST_ExpressionList(newAST_Expression_Derivative(ec->arguments()))))));
    }
  } break;
  case EXPOUTPUT:
    return (newAST_Expression_OutputExpressions(newAST_ExpressionList(apply(AST_ListFirst(exp->getAsOutput()->expressionList())))));
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceDer::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceDer::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}
}  // namespace Util
}  // namespace MicroModelica
