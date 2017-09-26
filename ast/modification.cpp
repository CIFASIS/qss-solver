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

#include "modification.h"

#include <list>
#include <sstream>

#include "../util/ast_util.h"
#include "expression.h"

/* Argument Class */

AST_Argument_::AST_Argument_() :
    _final(false), _each(false)
{
}

void
AST_Argument_::setFinal()
{
  _final = true;
}

void
AST_Argument_::setEach()
{
  _each = true;
}

bool
AST_Argument_::hasEach() const
{
  return _each;
}

bool
AST_Argument_::isFinal() const
{
  return _final;
}

void
AST_Argument_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
}

CLASS_PRINTER_IMP(AST_Argument);
GET_AS_IMP(Argument, Modification);

/*Modification Class */

ModificationType
AST_Modification_::modificationType()
{
  return MODNONE;
}

void
AST_Modification_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  visitor->leave(this);
}

GET_AS_IMP(Modification, Equal);
GET_AS_IMP(Modification, Class);
GET_AS_IMP(Modification, Assign);
CLASS_PRINTER_IMP(AST_Modification);

/* Modification Equal Class */

AST_Modification_Equal_::AST_Modification_Equal_(AST_Expression e) :
    _e(e)
{
}

string
AST_Modification_Equal_::print() const
{
  stringstream ret(stringstream::out);
  ret << " = " << _e;
  return ret.str();
}

AST_Expression
AST_Modification_Equal_::exp()
{
  return _e;
}

ModificationType
AST_Modification_Equal_::modificationType()
{
  return MODEQUAL;
}

/* Modification assign Class*/

AST_Modification_Assign_::AST_Modification_Assign_(AST_Expression e) :
    _e(e)
{
}

AST_Expression
AST_Modification_Assign_::exp()
{
  return _e;
}

ModificationType
AST_Modification_Assign_::modificationType()
{
  return MODASSIGN;
}

string
AST_Modification_Assign_::print() const
{
  stringstream ret(stringstream::out);
  ret << " := " << _e;
  return ret.str();
}

/* Modification Class Class */

AST_Modification_Class_::AST_Modification_Class_(AST_ArgumentList al,
    AST_Expression e) :
    _e(e), _al(al)
{
}

ModificationType
AST_Modification_Class_::modificationType()
{
  return MODCLASS;
}

AST_Expression
AST_Modification_Class_::exp() const
{
  return _e;
}

AST_ArgumentList
AST_Modification_Class_::arguments() const
{
  return _al;
}

void
AST_Modification_Class_::setArguments(AST_ArgumentList al)
{
  _al = al;
}

string
AST_Modification_Class_::print() const
{
  stringstream ret(stringstream::out);
  AST_ListPrint(arguments(), ret, "", ",", "(", ")");
  if(exp()->expressionType() != EXPNULL)
  {
    ret << " = " << exp();
  }
  return ret.str();
}

bool
AST_Modification_Class_::hasExpression()
{
  return _e != NULL;
}

/* Argument modification Class */

AST_Argument_Modification_::AST_Argument_Modification_(AST_String name,
    AST_Modification m) :
    _name(name), _mod(m)
{
}

string
AST_Argument_Modification_::print() const
{
  stringstream ret(stringstream::out);
  if(hasEach())
  {
    ret << "each ";
  }
  if(modification() != NULL)
  {
    ret << name() << modification();
  }
  else
  {
    ret << name();
  }
  return ret.str();
}

AST_String
AST_Argument_Modification_::name() const
{
  return _name;
}

AST_Modification
AST_Argument_Modification_::modification() const
{
  return _mod;
}

ArgumentType
AST_Argument_Modification_::argumentType() const
{
  return AR_MODIFICATION;
}

bool
AST_Argument_Modification_::hasModification()
{
  return _mod != NULL;
}

/* Argument redeclaration Class */

AST_Argument_Redeclaration_::AST_Argument_Redeclaration_()
{
}

string
AST_Argument_Redeclaration_::print() const
{
  return "";
}

ArgumentType
AST_Argument_Redeclaration_::argumentType() const
{
  return AR_REDECLARATION;
}

