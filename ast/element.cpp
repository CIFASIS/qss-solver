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

#include "element.h"

#include <list>
#include <sstream>

#include "../util/ast_util.h"
#include "class.h"
#include "modification.h"

using namespace std;

/* Element Class */

GET_AS_IMP(Element, ClassWrapper);
GET_AS_IMP(Element, Component);
GET_AS_IMP(Element, ExtendsClause);
GET_AS_IMP(Element, ImportClause);

ostream &
operator<<(ostream &os, const AST_Element &e)
{
  os << *e;
  return os;
}

ostream &
operator<<(ostream &os, const AST_Element_ &e)
{
  os << e.print();
  return os;
}

ElementType
AST_Element_::elementType()
{
  return ELNONE;
}

void
AST_Element_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
}

/* Element Class Wrapper Class */

AST_Element_ClassWrapper_::AST_Element_ClassWrapper_(AST_Class_ *c) :
    _c(c)
{
}

string
AST_Element_ClassWrapper_::print() const
{
  stringstream ret(stringstream::out);
  if(_c == NULL)
    ret << "CLASS[NULL]";
  else
    ret << (*_c);
  return ret.str();
}

ElementType
AST_Element_ClassWrapper_::elementType()
{
  return ELCLASS;
}

AST_Class
AST_Element_ClassWrapper_::getClass()
{
  return _c;
}

/* Element Component Class */

AST_Element_Component_::AST_Element_Component_(AST_DeclarationList decl_list,
    string type, AST_TypePrefix tp, AST_ExpressionList indexes) :
    _decl_list(decl_list), _type(type), _indexes(indexes), _origin(NULL), _tp(
        tp)
{
}

string
AST_Element_Component_::print() const
{
  stringstream ret(stringstream::out);
  AST_DeclarationListIterator it, itt;
  MAKE_SPACE
  ;
  if(isParameter())
  {
    ret << "parameter ";
  }
  if(isInput())
  {
    ret << "input ";
  }
  if(isOutput())
  {
    ret << "output ";
  }
  if(isDiscrete())
  {
    ret << "discrete ";
  }
  if(isFlow())
  {
    ret << "flow ";
  }
  if(isConstant())
  {
    ret << "constant ";
  }
  if(isStream())
  {
    ret << "stream ";
  }
  ret << _type;
  AST_ExpressionListIterator exp_it;
  if(indexes()->size())
  {
    ret << "[";
    int size = indexes()->size(), i = 0;
    foreach (exp_it,indexes())
    {
      i++;
      ret << current_element(it);
      if(i < size)
        ret << ",";
    }
    ret << "]";
  }
  ret << " ";
  for(it = _decl_list->begin(); it != _decl_list->end(); it++)
  {
    itt = it;
    itt++;
    ret << current_element(it) << (itt == _decl_list->end() ? "" : ",");
  }
  ret << ";";
  return ret.str();
}

ostream &
operator<<(ostream &os, const AST_Element_Component &c)
{
  os << *c;
  return os;
}

bool
AST_Element_Component_::hasIndexes()
{
  return !_indexes->empty();
}

/* Extends Clause Class */

AST_Element_ExtendsClause_::AST_Element_ExtendsClause_(string name) :
    _name(name)
{
}

string
AST_Element_ExtendsClause_::print() const
{
  stringstream ret(stringstream::out);
  ret << "EXTENDS " << _name;
  return ret.str();
}

ElementType
AST_Element_ExtendsClause_::elementType()
{
  return EXTENDS;
}

AST_String
AST_Element_ExtendsClause_::name()
{
  return &_name;
}

/* Import Clause Class */

AST_Element_ImportClause_::AST_Element_ImportClause_(string name) :
    _name(name)
{
}

string
AST_Element_ImportClause_::print() const
{
  stringstream ret(stringstream::out);
  ret << "IMPORT[" << _name << "]";
  return ret.str();
}

string
AST_Element_ImportClause_::name() const
{
  return _name;
}

ElementType
AST_Element_ImportClause_::elementType()
{
  return IMPORT;
}

/* Comment Class */

AST_Comment_::AST_Comment_(AST_String st, AST_ArgumentList al) :
    _st(st), _al(al)
{
}

AST_ArgumentList
AST_Comment_::arguments() const
{
  return _al;
}

AST_String
AST_Comment_::string() const
{
  return _st;
}

void
AST_Comment_::setAnnotation(AST_ArgumentList al)
{
  _al = al;
}

ostream &
operator<<(ostream &os, const AST_Comment_ &c)
{
  if(c.string())
  {
    os << " \"" << c.string() << "\" ";
  }
  if(c.arguments()->size())
  {
    AST_ListPrint(c.arguments(), os, " annotation", ",", "(", ")");
  }
  return os;
}

ostream &
operator<<(ostream &os, const AST_Comment &c)
{
  if(c != NULL)
  {
    os << *c;
  }
  return os;
}

void
AST_Comment_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
}

/* Declaration Class */

AST_Declaration_::AST_Declaration_(string name, AST_ExpressionList indexes,
    AST_Modification m) :
    _name(name), _indexes(indexes), _mod(m), _comm(NULL)
{
}

string
AST_Declaration_::print() const
{
  stringstream ret(stringstream::out);
  AST_ExpressionListIterator it;
  ret << name();
  if(indexes()->size())
  {
    ret << "[";
    int size = indexes()->size(), i = 0;
    foreach (it,indexes())
    {
      i++;
      ret << current_element(it);
      if(i < size)
        ret << ",";
    }
    ret << "]";
  }
  if(modification() != NULL)
  {
    /*
     if (modification()->modificationType()==MODEQUAL) {
     ret << " = " << modification()->getAsEqual()->exp();
     } else if (modification()->modificationType()==MODCLASS) {
     AST_Expression e  = modification()->getAsClass()->exp();
     if (e!=NULL && e->expressionType()!=EXPNULL)
     ret << " = " << e;
     }
     */
    ret << modification();
  }
  if(comment() != NULL)
    ret << comment();
  return ret.str();
}

string
AST_Declaration_::name() const
{
  return _name;
}

AST_ExpressionList
AST_Declaration_::indexes() const
{
  return _indexes;
}

bool
AST_Declaration_::hasIndexes()
{
  if(_indexes != NULL)
  {
    if(AST_Length(_indexes) > 0)
    {
      return true;
    }
  }
  return false;
}

AST_Modification
AST_Declaration_::modification() const
{
  return _mod;
}

bool
AST_Declaration_::hasModification()
{
  return _mod != NULL;
}

void
AST_Declaration_::setComment(AST_Comment c)
{
  _comm = c;
}

AST_Comment
AST_Declaration_::comment() const
{
  return _comm;
}

/* Component */

bool
AST_Element_Component_::isParameter() const
{
  return _tp & TP_PARAMETER;
}

bool
AST_Element_Component_::isInput() const
{
  return _tp & TP_INPUT;
}

bool
AST_Element_Component_::isOutput() const
{
  return _tp & TP_OUTPUT;
}

bool
AST_Element_Component_::isDiscrete() const
{
  return _tp & TP_DISCRETE;
}

bool
AST_Element_Component_::isFlow() const
{
  return _tp & TP_FLOW;
}

bool
AST_Element_Component_::isStream() const
{
  return _tp & TP_STREAM;
}

bool
AST_Element_Component_::isConstant() const
{
  return _tp & TP_CONSTANT;
}

string
AST_Element_Component_::type()
{
  return _type;
}
;

ElementType
AST_Element_Component_::elementType()
{
  return COMPONENT;
}

string
AST_Element_Component_::name()
{
  return _decl_list->front()->name();
}

AST_DeclarationList
AST_Element_Component_::nameList()
{
  return _decl_list;
}

AST_Class
AST_Element_Component_::origin()
{
  return _origin;
}

void
AST_Element_Component_::setOrigin(AST_Class c)
{
  if(_origin == NULL)
    _origin = c;
}

AST_TypePrefix
AST_Element_Component_::typePrefix()
{
  return _tp;
}

AST_ExpressionList
AST_Element_Component_::indexes() const
{
  return _indexes;
}

bool
AST_Element_Component_::isReal()
{
  return _type == "Real";
}

bool
AST_Element_Component_::isInteger()
{
  return _type == "Integer";
}

bool
AST_Element_Component_::isString()
{
  return _type == "String";
}