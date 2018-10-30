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

#include "variable_lookup.h"

#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
  namespace Util {

    /* VariableLookup class */

    VariableLookup::VariableLookup(VarSymbolTable st, VarSymbolTable lst) :
        _st(st), 
        _lst(lst)
    {
    }

    bool
    VariableLookup::foldTraverseElement(AST_Expression e)
    {
      if(e->expressionType() == EXPCOMPREF)
      {
        AST_Expression_ComponentReference cr = e->getAsComponentReference();
        Option<Variable> vi = _st[cr->name()];
        if(!vi)
        {
          vi = _lst[cr->name()];
          if(!vi)
          {
            return false;
          }
        }
      }
      return true;
    }

    bool
    VariableLookup::foldTraverseElement(bool e1, bool e2, BinOpType bot)
    {
      return e1 && e2;
    }

    bool
    VariableLookup::foldTraverseElementUMinus(AST_Expression e)
    {
      return foldTraverseElement(e);
    }
  }
}
