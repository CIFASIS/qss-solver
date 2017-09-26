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

#include "expression.h"

#include <list>
#include <sstream>

#include "../util/ast_util.h"
#include "ast_builder.h"

const char *BinOpTypeName[] =
    { " or ", " and ", "<", "<=", ">", ">=", "<>", "==", "+", ".+", "-", ".-",
        "/", "./", "*", ".*", "^", ".^" };

/* Expression Class */

AST_Expression_::~AST_Expression_()
{
}

GET_AS_IMP(Expression, BinOp);
GET_AS_IMP(Expression, Boolean);
GET_AS_IMP(Expression, BooleanNot);
GET_AS_IMP(Expression, Brace);
GET_AS_IMP(Expression, Call);
GET_AS_IMP(Expression, CallArgs);
GET_AS_IMP(Expression, ComponentReference);
GET_AS_IMP(Expression, NamedArgument);
GET_AS_IMP(Expression, Derivative);
GET_AS_IMP(Expression, If);
GET_AS_IMP(Expression, Integer);
GET_AS_IMP(Expression, Output);
GET_AS_IMP(Expression, Range);
GET_AS_IMP(Expression, Real);
GET_AS_IMP(Expression, UMinus);
GET_AS_IMP(Expression, String);

ExpressionType
AST_Expression_::expressionType()
{
  return EXPNONE;
}

CLASS_PRINTER_IMP(AST_Expression);

void
AST_Expression_::accept(AST_Visitor visitor)
{
  visitor->visit(this);
}

/* Binary operation Class */

AST_Expression_BinOp_::AST_Expression_BinOp_(AST_Expression e1,
    AST_Expression e2, BinOpType t) :
    _e1(e1), _e2(e2), _t(t)
{
}

ExpressionType
AST_Expression_BinOp_::expressionType()
{
  return EXPBINOP;
}

AST_Expression
AST_Expression_BinOp_::left() const
{
  return _e1;
}

AST_Expression
AST_Expression_BinOp_::right() const
{
  return _e2;
}

BinOpType
AST_Expression_BinOp_::binopType() const
{
  return _t;
}

string
AST_Expression_BinOp_::print() const
{
  stringstream ret(stringstream::out);
  /* Print parenthesis */
  if(left()->expressionType() == EXPBINOP)
  {
    AST_Expression_BinOp b = left()->getAsBinOp();
    if(b->binopType() < binopType())
    { // Parenthesis needed for left op
      ret << "(" << left() << ")";
    }
    else
    {
      ret << left();
    }
  }
  else
  {
    ret << left();
  }
  ret << BinOpTypeName[binopType()];
  if(right()->expressionType() == EXPBINOP)
  {
    AST_Expression_BinOp b = right()->getAsBinOp();
    if(b->binopType() < binopType())
    { // Parenthesis needed for left op
      ret << "(" << right() << ")";
    }
    else
    {
      ret << right();
    }
  }
  else
  {
    ret << right();
  }
  return ret.str();
}

/* Integer Class */

AST_Expression_Integer_::AST_Expression_Integer_(int i) :
    _i(i)
{
}

AST_Integer
AST_Expression_Integer_::val()
{
  return _i;
}

ExpressionType
AST_Expression_Integer_::expressionType()
{
  return EXPINTEGER;
}

string
AST_Expression_Integer_::print() const
{
  stringstream ret(stringstream::out);
  ret << _i;
  return ret.str();
}

/* Real Class */

AST_Expression_Real_::AST_Expression_Real_(double d) :
    _d(d)
{
}

ExpressionType
AST_Expression_Real_::expressionType()
{
  return EXPREAL;
}

string
AST_Expression_Real_::print() const
{
  stringstream ret(stringstream::out);
  ret << _d;
  return ret.str();
}

/* String Class */
AST_Expression_String_::AST_Expression_String_(string s) :
    _s(s)
{
}

ExpressionType
AST_Expression_String_::expressionType()
{
  return EXPSTRING;
}

string
AST_Expression_String_::print() const
{
  stringstream ret(stringstream::out);
  ret << "\"" << _s << "\"";
  return ret.str();
}

string
AST_Expression_String_::str() const
{
  return _s;
}

/* Boolean Class*/

AST_Expression_Boolean_::AST_Expression_Boolean_(bool b) :
    _b(b)
{
}

ExpressionType
AST_Expression_Boolean_::expressionType()
{
  return EXPBOOLEAN;
}

bool
AST_Expression_Boolean_::value()
{
  return _b;
}

string
AST_Expression_Boolean_::print() const
{
  stringstream ret(stringstream::out);
  ret << (_b ? "true" : "false");
  return ret.str();
}

/* Derivative Class */

AST_Expression_Derivative_::AST_Expression_Derivative_(AST_ExpressionList el) :
    _el(el)
{
}

ExpressionType
AST_Expression_Derivative_::expressionType()
{
  return EXPDERIVATIVE;
}

AST_ExpressionList
AST_Expression_Derivative_::arguments()
{
  return _el;
}

string
AST_Expression_Derivative_::print() const
{
  stringstream ret(stringstream::out);
  ret << "der(" << *_el->front() << ")";
  return ret.str();
}

/* Unary minus Class */

AST_Expression_UMinus_::AST_Expression_UMinus_(AST_Expression e) :
    _e(e)
{
}

ExpressionType
AST_Expression_UMinus_::expressionType()
{
  return EXPUMINUS;
}

AST_Expression
AST_Expression_UMinus_::exp() const
{
  return _e;
}

string
AST_Expression_UMinus_::print() const
{
  stringstream ret(stringstream::out);
  ret << "(-" << _e << ")";
  return ret.str();
}

/* Boolean not Class */

AST_Expression_BooleanNot_::AST_Expression_BooleanNot_(AST_Expression e) :
    _e(e)
{
}

ExpressionType
AST_Expression_BooleanNot_::expressionType()
{
  return EXPBOOLEANNOT;
}

AST_Expression
AST_Expression_BooleanNot_::exp() const
{
  return _e;
}

string
AST_Expression_BooleanNot_::print() const
{
  stringstream ret(stringstream::out);
  ret << "not " << exp();
  return ret.str();
}

/* Function call Class */

AST_Expression_Call_::AST_Expression_Call_(AST_String name,
    AST_ExpressionList args) :
    _name(name), _args(args)
{
}

ExpressionType
AST_Expression_Call_::expressionType()
{
  return EXPCALL;
}

AST_String
AST_Expression_Call_::name() const
{
  return _name;
}

AST_ExpressionList
AST_Expression_Call_::arguments() const
{
  return _args;
}

string
AST_Expression_Call_::print() const
{
  stringstream ret;
  AST_ExpressionListIterator it;
  ret << name() << "(";
  int size = arguments()->size(), i = 0;
  foreach(it,arguments())
  {
    i++;
    ret << current_element(it);
    ret << (i < size ? "," : "");
  }
  ret << ")";
  return ret.str();
}

/* Call arguments Class */
AST_Expression_CallArgs_::AST_Expression_CallArgs_(AST_ExpressionList args) :
    _args(args)
{
}

ExpressionType
AST_Expression_CallArgs_::expressionType()
{
  return EXPCALLARG;
}

AST_ExpressionList
AST_Expression_CallArgs_::arguments() const
{
  return _args;
}

string
AST_Expression_CallArgs_::print() const
{
  stringstream ret(stringstream::out);
  ret << "";
  return ret.str();
}

/* Brace expression Class */
AST_Expression_Brace_::AST_Expression_Brace_(AST_ExpressionList args) :
    _args(args)
{
}

ExpressionType
AST_Expression_Brace_::expressionType()
{
  return EXPBRACE;
}

AST_ExpressionList
AST_Expression_Brace_::arguments() const
{
  return _args;
}

string
AST_Expression_Brace_::print() const
{
  stringstream ret(stringstream::out);
  AST_ListPrint(arguments(), ret, "", ",", "{", "}");
  return ret.str();
}

/* Component Reference Class */

AST_Expression_ComponentReference_::AST_Expression_ComponentReference_() :
    _name(newAST_StringList()), _indexes(newAST_ExpressionListList())
{
}

ExpressionType
AST_Expression_ComponentReference_::expressionType()
{
  return EXPCOMPREF;
}

string
AST_Expression_ComponentReference_::print() const
{
  stringstream ret;
  AST_StringListIterator it;
  AST_ExpressionListListIterator exp_it = indexes()->begin();
  AST_ExpressionListIterator exp_it2;
  int size = names()->size(), i = 0;
  foreach (it, names())
  {
    i++;
    ret << current_element(it);
    if(current_element(exp_it)->size())
    {
      ret << "[";
      int size2 = current_element(exp_it)->size(), i2 = 0;
      foreach (exp_it2,current_element(exp_it))
        ret << current_element(exp_it2) << (++i2 < size2 ? "," : "");
      ret << "]";
    }
    ret << (i < size ? "." : "");
    exp_it++;
  }
  return ret.str();
}

CLASSP_PRINTER_IMP(AST_Expression_ComponentReference);

void
AST_Expression_ComponentReference_::append(AST_String s,
    AST_ExpressionList subs)
{
  AST_ListAppend(names(), s);
  AST_ListAppend(indexes(), subs);
}

void
AST_Expression_ComponentReference_::prepend(AST_String s,
    AST_ExpressionList subs)
{
  AST_ListPrepend(names(), s);
  AST_ListAppend(indexes(), subs);
}

AST_StringList
AST_Expression_ComponentReference_::names() const
{
  return _name;
}

const char*
AST_Expression_ComponentReference_::cname()
{
  return name().c_str();
}
string
AST_Expression_ComponentReference_::name()
{
  stringstream ret;
  AST_StringListIterator it;
  foreach (it, names())
  {
    ret << current_element(it)->c_str();
  }
  return ret.str();
}

AST_ExpressionListList
AST_Expression_ComponentReference_::indexes() const
{
  return _indexes;
}

bool
AST_Expression_ComponentReference_::hasIndexes() const
{
  if(_indexes->empty())
  {
    return false;
  }
  AST_ExpressionListListIterator it;
  foreach(it,_indexes)
  {
    AST_ExpressionList el = current_element(it);
    AST_ExpressionListIterator elit;
    foreach(elit,el)
    {
      if(current_element(elit) != NULL)
      {
        return true;
      }
    }
  }
  return false;
}

/* If expression Class */

AST_Expression_If_::AST_Expression_If_(AST_Expression cond, AST_Expression then,
    AST_Expression else_exp, AST_ExpressionList elseif_list) :
    _cond(cond), _then(then), _else_exp(else_exp), _elseif_list(elseif_list)
{
}

ExpressionType
AST_Expression_If_::expressionType()
{
  return EXPIF;
}

AST_Expression
AST_Expression_If_::else_exp() const
{
  return _else_exp;
}

AST_ExpressionList
AST_Expression_If_::elseif_list() const
{
  return _elseif_list;
}

AST_Expression
AST_Expression_If_::condition() const
{
  return _cond;
}

AST_Expression
AST_Expression_If_::then() const
{
  return _then;
}

/* Else if expression Class  */

AST_Expression_If_ElseIf_::AST_Expression_If_ElseIf_(AST_Expression c,
    AST_Expression t) :
    _cond(c), _then(t)
{
}

AST_Expression_If_ElseIf
AST_Expression_::getAsElseIf()
{
  return dynamic_cast<AST_Expression_If_ElseIf>(this);
}

ExpressionType
AST_Expression_If_ElseIf_::expressionType()
{
  return EXPELSEIF;
}

AST_Expression
AST_Expression_If_ElseIf_::condition()
{
  return _cond;
}

AST_Expression
AST_Expression_If_ElseIf_::then()
{
  return _then;
}

string
AST_Expression_If_ElseIf_::print() const
{
  stringstream ret;
  ret << " elseif " << _cond << " then " << _then;
  return ret.str();
}

string
AST_Expression_If_::print() const
{
  stringstream ret;
  AST_ExpressionListIterator it;
  ret << "if " << condition() << " then " << then();
  if(!elseif_list()->empty())
    foreach(it , elseif_list() )
      ret << current_element(it)->getAsElseIf();
  ret << " else " << else_exp();
  return ret.str();
}

/* End for subscripts Class */

ExpressionType
AST_Expression_End_::expressionType()
{
  return EXPEND;
}

string
AST_Expression_End_::print() const
{
  return "end";
}

/* Output expression Class */

AST_Expression_Output_::AST_Expression_Output_(AST_ExpressionList l) :
    _list(l)
{
}

ExpressionType
AST_Expression_Output_::expressionType()
{
  return EXPOUTPUT;
}

string
AST_Expression_Output_::print() const
{
  AST_ExpressionListIterator it;
  stringstream ret;
  ret << "(";
  int size = _list->size(), i = 0;
  foreach (it,_list)
  {
    i++;
    ret << current_element(it);
    ret << (i == size ? "" : ",");
  }
  ret << ")";
  return ret.str();

}

AST_ExpressionList
AST_Expression_Output_::expressionList()
{
  return _list;
}

/* Range expression Class */

AST_Expression_Range_::AST_Expression_Range_(AST_ExpressionList l) :
    _list(l)
{
}

ExpressionType
AST_Expression_Range_::expressionType()
{
  return EXPRANGE;
}

string
AST_Expression_Range_::print() const
{
  AST_ExpressionListIterator it;
  stringstream ret;
  int size = _list->size(), i = 0;
  foreach (it,_list)
  {
    i++;
    ret << current_element(it);
    ret << (i == size ? "" : ":");
  }
  return ret.str();
}

AST_ExpressionList
AST_Expression_Range_::expressionList()
{
  return _list;
}

/* Null Expression Class */

ExpressionType
AST_Expression_Null_::expressionType()
{
  return EXPNULL;
}

string
AST_Expression_Null_::print() const
{
  return "NULLEXP!!!";
}

/* Colon expression Class */

ExpressionType
AST_Expression_Colon_::expressionType()
{
  return EXPCOLON;
}

string
AST_Expression_Colon_::print() const
{
  return ":";
}

/* Named Argument expression Class */

AST_Expression_NamedArgument_::AST_Expression_NamedArgument_(AST_String name,
    AST_Expression exp) :
    _exp(exp), _name(name)
{
}

ExpressionType
AST_Expression_NamedArgument_::expressionType()
{
  return EXPNAMEDARGUMENT;
}

string
AST_Expression_NamedArgument_::print() const
{
  stringstream ret;
  return ret.str();
}

AST_Expression
AST_Expression_NamedArgument_::expression()
{
  return _exp;
}

AST_String
AST_Expression_NamedArgument_::name()
{
  return _name;
}

AST_Expression_ArrayIndex_::AST_Expression_ArrayIndex_(AST_Expression exp,
    int size) :
    _exp(exp), _size(size)
{
}

ExpressionType
AST_Expression_ArrayIndex_::expressionType()
{
  return EXPARRAYINDEX;
}

string
AST_Expression_ArrayIndex_::print() const
{
  return "";
}

AST_Expression
AST_Expression_ArrayIndex_::expression()
{
  return _exp;
}

int
AST_Expression_ArrayIndex_::size()
{
  return _size;
}

AST_ExpressionList
AST_Expression_ComponentReference_::firstIndex()
{
  if(_indexes->empty())
  {
    return newAST_ExpressionList();
  }
  return AST_ListFirst(_indexes);
}
