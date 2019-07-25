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

#include "array_use.h"
#include "../symbol_table.h"
#include "../error.h"

namespace MicroModelica {
namespace Util {
bool ArrayUse::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = _symbols[cr->name()];
    if (cr->hasIndexes() && var) {
      if (AST_ListFirst(cr->indexes())->size() != var->dimensions()) {
        Error::instance().add(exp->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong array dimension, expected %d got %d.",
                              var->dimensions(), cr->indexes()->size());
      }
    }
    break;
  }
  case EXPDERIVATIVE: {
    AST_Expression e = AST_ListFirst(exp->getAsDerivative()->arguments());
    if (e->expressionType() != EXPCOMPREF) {
      Error::instance().add(exp->lineNum(), EM_AST | EM_CLASS_DEFINITION, ER_Error, "Wrong assign expression in equation.");
    }
    foldTraverseElement(e);
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output eout = exp->getAsOutput();
    AST_ExpressionList el = eout->expressionList();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      foldTraverseElement(current_element(it));
    }
    break;
  }
  default:
    return true;
  }
  return true;
}
}  // namespace Util
}  // namespace MicroModelica
