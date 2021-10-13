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

#include "algebraics.h"

#include "../../ast/ast_builder.h"
#include "../model_config.h"
#include "../util.h"

namespace MicroModelica {
namespace Util {

Algebraics::Algebraics(bool state) : _exps(newAST_ExpressionList()), _var(), _state(state) {}

void Algebraics::exclude(Variable var) { _var = var; }

bool Algebraics::foldTraverseElement(AST_Expression e)
{
  bool has_algebraics = false;
  switch (e->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (var) {
      bool search_var = (_state) ? var->isState() : var->isAlgebraic(); 
      if (search_var && (var->name() != _var.name())) {
        has_algebraics = true;
        AST_ListAppend(_exps, e);
      }
    }
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output out = e->getAsOutput();
    AST_ExpressionListIterator it;
    foreach (it, out->expressionList()) {
      has_algebraics = has_algebraics || apply(current_element(it));
    }
    break;
  }
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      has_algebraics = has_algebraics || apply(current_element(it));
    }
    break;
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = e->getAsCallArgs();
    AST_ExpressionList el = call->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      has_algebraics = has_algebraics || apply(current_element(it));
    }
    break;
  }

  default:
    return has_algebraics;
  }
  return has_algebraics;
}
}  // namespace Util
}  // namespace MicroModelica
