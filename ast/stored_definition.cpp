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

#include "stored_definition.h"

#include <stddef.h>
#include <iostream>
#include <list>

#include "class.h"
#include "imports.h"

AST_StoredDefinition_::AST_StoredDefinition_(AST_ClassList ml,
    AST_String within) :
    _model_list(ml), _within(within)
{
  _imports = new AST_Imports();
}

CLASSP_PRINTER_IMP(AST_StoredDefinition);

AST_ClassList
AST_StoredDefinition_::models() const
{
  return _model_list;
}

AST_String
AST_StoredDefinition_::within() const
{
  return _within;
}

ostream &
operator<<(ostream &os, const AST_StoredDefinition_ &sd)
{
  AST_ClassListIterator it;
  AST_ClassList cl = sd.models();
  if(sd.within() != NULL)
    os << "within " << sd.within() << endl;
  foreach(it,cl)
  {
    os << current_element(it);
  }
  return os;
}

void
AST_StoredDefinition_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  AST_ClassListIterator it;
  foreach(it,_model_list)
  {
    AST_Class x = current_element(it);
    AST_TypePrefix p = x->prefix();
    if((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE))
    {
      x->accept(visitor);
    }
  }
  foreach(it,_model_list)
  {
    AST_Class x = current_element(it);
    AST_TypePrefix p = x->prefix();
    if(!(p & CP_FUNCTION) && !(p & CP_IMPURE) && !(p & CP_PURE))
    {
      x->accept(visitor);
    }
  }
  visitor->leave(this);
}

bool
AST_StoredDefinition_::hasWithin()
{
  return _within != NULL;
}

AST_StringList
AST_StoredDefinition_::imports()
{
  AST_Imports imp;
  _imports->apply(this);
  return _imports->imports();
}
