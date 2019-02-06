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
#include "../symbol_table.h"
#include "../error.h"

namespace MicroModelica {
  using namespace IR;
  namespace Util {
    bool 
    Occurs::foldTraverseElement(AST_Expression exp)
    {
      switch(exp->expressionType())
      {
        case EXPCOMPREF:
        {
          AST_Expression_ComponentReference cr = exp->getAsComponentReference();
          Option<Variable> var = _symbols[cr->name()];
          if(var && (var->name() == _var)) {
            _occs.push_back(Expression(exp, _symbols));
            return true;  
          }
          break;
        }
        case EXPCALL:
        {
          AST_Expression_Call call = exp->getAsCall();
          AST_ExpressionList el = call->arguments();
          AST_ExpressionListIterator it;
          bool ret = false;
          foreach(it, el)
          {
            ret = ret || apply(current_element(it));
          }
          return ret;
        }
        default:
          return false;
      }
      return false;
    }
  }
}
