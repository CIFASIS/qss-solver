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

#include "occurs.h"
#include <util/error.h>
#include <util/model_config.h>
#include <util/symbol_table.h>

namespace MicroModelica {
using namespace IR;
namespace Util {

bool Occurs::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (var && (var->name() == _var)) {
      bool add_occur = true;
      if (cr->hasIndexes()) { 
        AST_ExpressionList indexes = cr->firstIndex();
        AST_ExpressionListIterator it;
        foreach (it, indexes) {
          if (current_element(it)->expressionType() == EXPCOMPREF) {
            AST_Expression_ComponentReference idx_cr = current_element(it)->getAsComponentReference();
            Option<Variable> idx_var = ModelConfig::instance().lookup(idx_cr->name());
            if (idx_var && idx_var->isParameter()) {
              add_occur = false;
              break;
            }
          }
        }
      }
      if (add_occur) {
        Expression ep(exp);
        _occs.insert(ep);
        return true;
      }
    }
    break;
  }
  case EXPCALL: {
    AST_Expression_Call call = exp->getAsCall();
    AST_ExpressionList el = call->arguments();
    AST_ExpressionListIterator it;
    bool ret = false;
    foreach (it, el) {
      ret = ret || apply(current_element(it));
    }
    return ret;
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionList el = call->arguments();
    AST_ExpressionListIterator it;
    bool ret = false;
    foreach (it, el) {
      ret = ret || apply(current_element(it));
    }
    return ret;
  }
  case EXPOUTPUT: {
    bool ret = false;
    AST_Expression_Output out = exp->getAsOutput();
    AST_ExpressionListIterator it;
    foreach (it, out->expressionList()) {
      ret = ret || apply(current_element(it));
    }
    return ret;
  }
  default:
    return false;
  }
  return false;
}

set<Expression> Occurs::occurrences() { return _occs; }

}  // namespace Util
}  // namespace MicroModelica
