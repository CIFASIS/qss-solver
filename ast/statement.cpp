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

#include "statement.h"

#include <list>
#include <sstream>

#include "../util/ast_util.h"
#include "element.h"
#include "expression.h"

/* Statement Class */

GET_AS_IMP(Statement, While);
GET_AS_IMP(Statement, If);
GET_AS_IMP(Statement, For);
GET_AS_IMP(Statement, When);
GET_AS_IMP(Statement, Assign);

CLASS_PRINTER_IMP(AST_Statement);

void
AST_Statement_::accept (AST_Visitor visitor)
{
  visitor->visit (this);
  visitor->leave (this);
}

/* Return statement Class */

StatementType
AST_Statement_Return_::statementType ()
{
  return (STRETURN);
}

string
AST_Statement_Return_::print () const
{
  stringstream ret (stringstream::out);
  MAKE_SPACE
  ;
  ret << "return" << endl;
  return (ret.str ());
}

/* Break statement Class */

StatementType
AST_Statement_Break_::statementType ()
{
  return (STBREAK);
}

string
AST_Statement_Break_::print () const
{
  stringstream ret (stringstream::out);
  MAKE_SPACE
  ;
  ret << "break" << endl;
  return (ret.str ());
}

/* Assign Class */

AST_Statement_Assign_::AST_Statement_Assign_ (
    AST_Expression_ComponentReference lhs, AST_Expression exp) :
    _lhs (lhs), _exp (exp)
{
}

AST_Expression
AST_Statement_Assign_::exp () const
{
  return (_exp);
}

AST_Expression_ComponentReference
AST_Statement_Assign_::lhs () const
{
  return (_lhs);
}

StatementType
AST_Statement_Assign_::statementType ()
{
  return (STASSING);
}

string
AST_Statement_Assign_::print () const
{
  stringstream ret (stringstream::out);
  AST_ExpressionListIterator it;
  MAKE_SPACE
  ;
  if (exp ()->expressionType () != EXPCALLARG)
    ret << lhs () << ":=" << exp () << ";" << endl;
  else if (exp ()->expressionType () == EXPCALLARG)
    {
      ret << lhs () << "(";
      int size = exp ()->getAsCallArgs ()->arguments ()->size (), i = 0;
      foreach(it,exp()->getAsCallArgs()->arguments())
	{
	  ret << current_element(it) << (++i < size ? "," : "");
	}
      ret << ");" << endl;
    }

  return (ret.str ());
}

/* For statement Class */

AST_Statement_For_::AST_Statement_For_ (AST_ForIndexList index,
					AST_StatementList sts) :
    _sts (sts), _ind (index)
{
}

string
AST_Statement_For_::print () const
{
  stringstream ret (stringstream::out);
  AST_StatementListIterator it;
  MAKE_SPACE
  ;
  AST_ListPrint (forIndexList (), ret, "for ", ",", "", " loop\n", false);
  BEGIN_BLOCK
  ;
  foreach(it,statements())
    ret << current_element(it);
  END_BLOCK
  ;
  MAKE_SPACE
  ;
  ret << "end for;" << endl;
  return (ret.str ());
}

AST_StatementList
AST_Statement_For_::statements () const
{
  return (_sts);
}

AST_ForIndexList
AST_Statement_For_::forIndexList () const
{
  return (_ind);
}

StatementType
AST_Statement_For_::statementType ()
{
  return (STFOR);
}

/* When Class */

AST_Statement_When_::AST_Statement_When_ (AST_Expression cond,
					  AST_StatementList sts,
					  AST_Statement_ElseList else_st,
					  AST_Comment c) :
    _cond (cond), _sts (sts), _else_list (else_st), _comm (c)
{
}

StatementType
AST_Statement_When_::statementType ()
{
  return (STWHEN);
}

AST_Expression
AST_Statement_When_::condition () const
{
  return (_cond);
}

AST_StatementList
AST_Statement_When_::statements () const
{
  return (_sts);
}

AST_Statement_ElseList
AST_Statement_When_::else_when () const
{
  return (_else_list);
}

AST_Comment
AST_Statement_When_::comment () const
{
  return (_comm);
}

bool
AST_Statement_When_::hasComment ()
{
  return (_comm != NULL);
}

bool
AST_Statement_When_::hasElsewhen ()
{
  return (_else_list->size () > 0);
}

string
AST_Statement_When_::print () const
{
  stringstream ret (stringstream::out);
  AST_StatementListIterator it;
  AST_Statement_ElseListIterator else_it;

  MAKE_SPACE
  ;
  ret << "when " << condition () << " then" << endl;

  BEGIN_BLOCK
  ;
  foreach(it,statements())
    ret << current_element(it);
  END_BLOCK
  ;
  foreach(else_it,else_when())
    {
      MAKE_SPACE
      ;
      ret << "elsewhen " << current_element(else_it)->condition () << " then"
	  << endl;
      BEGIN_BLOCK
      ;
      foreach(it, current_element(else_it)->statements())
	ret << current_element(it);
      END_BLOCK
      ;
    }

  MAKE_SPACE
  ;
  ret << "end when;" << endl;
  return (ret.str ());
}

/* While Class */

AST_Statement_While_::AST_Statement_While_ (AST_Expression cond,
					    AST_StatementList sts) :
    _cond (cond), _sts (sts)
{
}

AST_Expression
AST_Statement_While_::condition () const
{
  return (_cond);
}

StatementType
AST_Statement_While_::statementType ()
{
  return (STWHILE);
}

AST_StatementList
AST_Statement_While_::statements () const
{
  return (_sts);
}

string
AST_Statement_While_::print () const
{
  stringstream ret (stringstream::out);
  AST_StatementListIterator it;
  MAKE_SPACE
  ;
  ret << "while " << condition () << " loop" << endl;
  BEGIN_BLOCK
  ;
  foreach(it,statements())
    ret << current_element(it);
  END_BLOCK
  ;
  MAKE_SPACE
  ;
  ret << "end while;" << endl;
  return (ret.str ());
}

/* If Class */

AST_Statement_If_::AST_Statement_If_ (AST_Expression cond,
				      AST_StatementList true_st,
				      AST_Statement_ElseList else_list,
				      AST_StatementList false_st) :
    _cond (cond), _true_st (true_st), _false_st (false_st), _else_list (
	else_list)
{
}

AST_Expression
AST_Statement_If_::condition () const
{
  return (_cond);
}

AST_StatementList
AST_Statement_If_::statements () const
{
  return (_true_st);
}

AST_StatementList
AST_Statement_If_::else_statements () const
{
  return (_false_st);
}

AST_Statement_ElseList
AST_Statement_If_::else_if () const
{
  return (_else_list);
}

StatementType
AST_Statement_If_::statementType ()
{
  return (STIF);
}

string
AST_Statement_If_::print () const
{
  stringstream ret (stringstream::out);
  AST_StatementListIterator it;
  AST_Statement_ElseListIterator else_it;
  MAKE_SPACE
  ;
  ret << "if " << condition () << " then" << endl;
  BEGIN_BLOCK
  ;
  foreach(it,statements())
    ret << current_element(it);
  END_BLOCK
  ;
  foreach(else_it,else_if())
    {
      MAKE_SPACE
      ;
      ret << "elseif " << current_element(else_it)->condition () << " then"
	  << endl;
      BEGIN_BLOCK
      ;
      foreach(it, current_element(else_it)->statements())
	ret << current_element(it);
      END_BLOCK
      ;
    }

  if (else_statements ()->size ())
    {
      MAKE_SPACE
      ;
      ret << "else" << endl;
    }
  BEGIN_BLOCK
  ;
  foreach(it,else_statements())
    ret << current_element(it);
  END_BLOCK
  ;
  MAKE_SPACE
  ;
  ret << "end if;" << endl;
  return (ret.str ());

}

/* Output assigment Classes */

AST_Statement_OutputAssigment_::AST_Statement_OutputAssigment_ (
    AST_ExpressionList out_exps, AST_Expression_ComponentReference func,
    AST_ExpressionList args) :
    _out_exps (out_exps), _func (func), _args (args)
{
}

string
AST_Statement_OutputAssigment_::print () const
{
  stringstream ret (stringstream::out);
  AST_ExpressionListIterator it;
  MAKE_SPACE
  ;
  ret << "(";
  int size = out_expressions ()->size (), i = 0;
  foreach(it,out_expressions())
    {
      i++;
      ret << current_element(it);
      ret << (i < size ? "," : "");
    }
  ret << "):=";
  ret << function ();
  ret << "(";
  size = arguments ()->size ();
  i = 0;
  foreach(it,arguments())
    {
      i++;
      ret << current_element(it);
      ret << (i < size ? "," : "");
    }
  ret << ");" << endl;
  return (ret.str ());
}

AST_ExpressionList
AST_Statement_OutputAssigment_::out_expressions () const
{
  return (_out_exps);
}

AST_Expression_ComponentReference
AST_Statement_OutputAssigment_::function () const
{
  return (_func);
}

AST_ExpressionList
AST_Statement_OutputAssigment_::arguments () const
{
  return (_args);
}

StatementType
AST_Statement_OutputAssigment_::statementType ()
{
  return (STOUTASSING);
}

/* Else statement Class */

AST_Statement_Else_::AST_Statement_Else_ (AST_Expression cond,
					  AST_StatementList sts) :
    _cond (cond), _sts (sts)
{
}

AST_Expression
AST_Statement_Else_::condition () const
{
  return (_cond);
}

AST_StatementList
AST_Statement_Else_::statements () const
{
  return (_sts);
}

void
AST_Statement_Else_::accept (AST_Visitor visitor)
{
  visitor->visit (this);
  _cond->accept (visitor);
  AST_StatementListIterator it;
  foreach(it,_sts)
    {
      current_element(it)->accept (visitor);
    }
}
