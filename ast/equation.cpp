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

#include "equation.h"

#include <list>
#include <sstream>

#include "../util/ast_util.h"
#include "expression.h"

/* Equation Class */

CLASS_PRINTER_IMP(AST_Equation);

void
AST_Equation_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
}

/* Equality Class */

string
AST_Equation_Equality_::print() const
{
  stringstream ret(stringstream::out);
  MAKE_SPACE
  ;
  ret << left() << " = " << right() << ";" << endl;
  return ret.str();
}

void
AST_Equation_Equality_::setLeft(AST_Expression e)
{
  _left = e;
}

void
AST_Equation_Equality_::setRight(AST_Expression e)
{
  _right = e;
}

AST_Equation_Equality_::AST_Equation_Equality_(AST_Expression left,
    AST_Expression right) :
    _left(left), _right(right)
{
}

AST_Expression
AST_Equation_Equality_::left() const
{
  return _left;
}

AST_Expression
AST_Equation_Equality_::right() const
{
  return _right;
}

EquationType
AST_Equation_Equality_::equationType()
{
  return EQEQUALITY;
}

GET_AS_IMP(Equation, Equality);

/* Connect Class */

AST_Equation_Connect_::AST_Equation_Connect_(
    AST_Expression_ComponentReference cr1,
    AST_Expression_ComponentReference cr2) :
    _cr1(cr1), _cr2(cr2)
{
}

string
AST_Equation_Connect_::print() const
{
  stringstream ret(stringstream::out);
  MAKE_SPACE
  ;
  ret << "connect(" << _cr1 << "," << _cr2 << ");" << endl;
  return ret.str();
}

AST_Expression_ComponentReference
AST_Equation_Connect_::left()
{
  return _cr1;
}

AST_Expression_ComponentReference
AST_Equation_Connect_::right()
{
  return _cr2;
}

EquationType
AST_Equation_Connect_::equationType()
{
  return EQCONNECT;
}

/* Call Class */

AST_Equation_Call_::AST_Equation_Call_(AST_Expression e) :
    _call(e)
{
}

AST_Expression
AST_Equation_Call_::call() const
{
  return _call;
}

string
AST_Equation_Call_::print() const
{
  stringstream ret(stringstream::out);
  MAKE_SPACE
  ;
  ret << call() << ";" << endl;
  return ret.str();
}

EquationType
AST_Equation_Call_::equationType()
{
  return EQCALL;
}

GET_AS_IMP(Equation, Call);

/* If equation Class */

AST_Equation_If_::AST_Equation_If_(AST_Expression cond, AST_EquationList eql,
    AST_Equation_ElseList elseif, AST_EquationList else_eq) :
    _cond(cond), _eql(eql), _else(else_eq), _else_if(elseif)
{
}

AST_Expression
AST_Equation_If_::condition() const
{
  return _cond;
}

void
AST_Equation_If_::setCondition(AST_Expression c)
{
  _cond = c;
}

AST_EquationList
AST_Equation_If_::equationList() const
{
  return _eql;
}

AST_EquationList
AST_Equation_If_::equationElseList() const
{
  return _else;
}

AST_Equation_ElseList
AST_Equation_If_::equationElseIf() const
{
  return _else_if;
}

EquationType
AST_Equation_If_::equationType()
{
  return EQIF;
}

string
AST_Equation_If_::print() const
{
  stringstream ret(stringstream::out);
  AST_EquationListIterator it;
  AST_Equation_ElseListIterator else_it;
  MAKE_SPACE
  ;
  ret << "if (" << condition() << ") then " << endl;
  BEGIN_BLOCK
  ;
  foreach(it,equationList())
    ret << current_element(it);
  END_BLOCK
  ;
  foreach(else_it,equationElseIf())
  {
    MAKE_SPACE
    ;
    ret << "elseif " << current_element(else_it)->condition() << " then"
        << endl;
    BEGIN_BLOCK
    ;
    foreach(it, current_element(else_it)->equations())
      ret << current_element(it);
    END_BLOCK
    ;

  }
  if(equationElseList()->size())
  {
    MAKE_SPACE
    ;
    ret << "else" << endl;
    BEGIN_BLOCK
    ;
    foreach(it,equationElseList())
      ret << current_element(it);
    END_BLOCK
    ;
  }
  MAKE_SPACE
  ;
  ret << "end if;" << endl;
  return ret.str();
}

GET_AS_IMP(Equation, If);

/* For equation Class */

AST_Equation_For_::AST_Equation_For_(AST_ForIndexList ind, AST_EquationList eql) :
    _eql(eql), _ind(ind)
{
}

string
AST_Equation_For_::print() const
{
  stringstream ret(stringstream::out);
  AST_EquationList eql = equationList();
  AST_EquationListIterator it;
  AST_ForIndexListIterator ind_it;
  MAKE_SPACE
  ;
  AST_ListPrint(forIndexList(), ret, "for ", ",", "", " loop\n", false);
  BEGIN_BLOCK
  ;
  foreach(it,eql)
    ret << current_element(it);
  END_BLOCK
  ;
  MAKE_SPACE
  ;
  ret << "end for;" << endl;
  return ret.str();
}

AST_EquationList
AST_Equation_For_::equationList() const
{
  return _eql;
}

AST_ForIndexList
AST_Equation_For_::forIndexList() const
{
  return _ind;
}

EquationType
AST_Equation_For_::equationType()
{
  return EQFOR;
}

/* For Index Class */

AST_ForIndex_::AST_ForIndex_(AST_String var, AST_Expression exp) :
    _var(var), _in(exp)
{
}

AST_String
AST_ForIndex_::variable() const
{
  return _var;
}

AST_Expression
AST_ForIndex_::in_exp() const
{
  return _in;
}

CLASSP_PRINTER_IMP(AST_ForIndex);

ostream &
operator<<(ostream &os, const AST_ForIndex_ &f)
{
  os << f.variable();
  if(f.in_exp() != NULL)
  {
    os << " in " << f.in_exp();
  }
  return os;
}

GET_AS_IMP(Equation, For);

void
AST_ForIndex_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  _in->accept(visitor);
}

/* When Equation Class */

AST_Equation_When_::AST_Equation_When_(AST_Expression cond,
    AST_EquationList eql, AST_Equation_ElseList elsewhen) :
    _cond(cond), _eql(eql), _else_when(elsewhen)
{
}

AST_Expression
AST_Equation_When_::condition() const
{
  return _cond;
}

AST_EquationList
AST_Equation_When_::equationList() const
{
  return _eql;
}

EquationType
AST_Equation_When_::equationType()
{
  return EQWHEN;
}

AST_Equation_ElseList
AST_Equation_When_::equationElseWhen() const
{
  return _else_when;
}

void
AST_Equation_When_::setCondition(AST_Expression e)
{
  _cond = e;
}

string
AST_Equation_When_::print() const
{
  stringstream ret(stringstream::out);
  AST_EquationList eql = equationList();
  AST_EquationListIterator it;
  AST_Equation_ElseListIterator else_it;
  MAKE_SPACE
  ;
  ret << "when " << condition() << " then" << endl;
  BEGIN_BLOCK
  ;
  foreach(it,eql)
    ret << current_element(it);
  END_BLOCK
  ;
  foreach(else_it,equationElseWhen())
  {
    MAKE_SPACE
    ;
    ret << "elsewhen " << current_element(else_it)->condition() << " then"
        << endl;
    BEGIN_BLOCK
    ;
    foreach(it, current_element(else_it)->equations())
      ret << current_element(it);
    END_BLOCK
    ;
  }

  MAKE_SPACE
  ;
  ret << "end when;" << endl;
  return ret.str();
}

GET_AS_IMP(Equation, When);

/* Else equation Class */

AST_Equation_Else_::AST_Equation_Else_(AST_Expression cond,
    AST_EquationList eqs) :
    _cond(cond), _eqs(eqs)
{
}

AST_Expression
AST_Equation_Else_::condition() const
{
  return _cond;
}

void
AST_Equation_Else_::setCondition(AST_Expression e)
{
  _cond = e;
}

AST_EquationList
AST_Equation_Else_::equations() const
{
  return _eqs;
}

void
AST_Equation_Else_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
  _cond->accept(visitor);
  AST_EquationListIterator _eqs_it;
  foreach(_eqs_it,_eqs)
  {
    current_element(_eqs_it)->accept(visitor);
  }
}
