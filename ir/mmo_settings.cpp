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

#include "mmo_settings.h"

#include "../ast/class.h"
#include "../ast/modification.h"
#include "../util/error.h"
#include "class.h"

/* MicroModelica Intermediate Representation */

MMO_Settings_::MMO_Settings_(string name) :
    _class(NULL), _classModification(false), _insertAnnotation(false), _className(), _classPrefix(), _insideFunction()
{
}

MMO_Settings_::~MMO_Settings_()
{
}

void
MMO_Settings_::visit(AST_Class x)
{
  Error::getInstance()->setClassName(*(x->name()));
  AST_TypePrefix p = x->prefix();
  if(_class != NULL)
  {
    if((p & CP_FUNCTION) || (p & CP_IMPURE) || (p & CP_PURE))
    {
      _insertAnnotation = false;
      _insideFunction = true;
    }
  }
  else
  {
    if(p & CP_MODEL)
    {
      _class = newMMO_Model(*x->name());
      _insertAnnotation = true;
    }
  }
}

void
MMO_Settings_::leave(AST_Class x)
{
  if(_insideFunction)
  {
    _insertAnnotation = true;
    _insideFunction = false;
  }
}

void
MMO_Settings_::visit(AST_Composition x)
{
}

void
MMO_Settings_::leave(AST_Composition x)
{
}

void
MMO_Settings_::visit(AST_CompositionElement x)
{
}

void
MMO_Settings_::leave(AST_CompositionElement x)
{
}

void
MMO_Settings_::visit(AST_CompositionEqsAlgs x)
{
}

void
MMO_Settings_::leave(AST_CompositionEqsAlgs x)
{
}

void
MMO_Settings_::visit(AST_External_Function_Call x)
{
}

void
MMO_Settings_::visit(AST_Element x)
{
}

void
MMO_Settings_::visit(AST_Modification x)
{
  if(x->modificationType() == MODCLASS)
  {
    _classModification = true;
  }
}

void
MMO_Settings_::leave(AST_Modification x)
{
  if(x->modificationType() == MODCLASS)
  {
    _classModification = false;
  }
}

void
MMO_Settings_::visit(AST_Comment x)
{
}

void
MMO_Settings_::visit(AST_Equation x)
{
}

void
MMO_Settings_::visit(AST_ForIndex x)
{
}

void
MMO_Settings_::visit(AST_Equation_Else x)
{
}

void
MMO_Settings_::visit(AST_Expression x)
{
}

void
MMO_Settings_::visit(AST_Argument x)
{
  if(_insertAnnotation)
  {
    if(x->argumentType() == AR_MODIFICATION)
    {
      AST_Argument_Modification am = x->getAsModification();
      if(am->hasModification() && _classModification == false)
      {
        _class->insert(am);
      }
    }
  }
}

void
MMO_Settings_::visit(AST_Statement x)
{
}

void
MMO_Settings_::leave(AST_Statement x)
{
}

void
MMO_Settings_::visit(AST_Statement_Else x)
{
}

void
MMO_Settings_::visit(AST_StoredDefinition x)
{
}

void
MMO_Settings_::leave(AST_StoredDefinition x)
{
}

int
MMO_Settings_::apply(AST_Node x)
{
  x->accept(this);
  return Error::getInstance()->errors();
}

MMO_Annotation
MMO_Settings_::annotations()
{
  return _class->getAsModel()->annotation();
}

MMO_Settings
newMMO_Settings(string name)
{
  return new MMO_Settings_(name);
}

void
deleteMMO_Settings(MMO_Settings m)
{
  delete m;
}
