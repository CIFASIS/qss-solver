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

#ifndef AST_EXPRESSION_H_
#define AST_EXPRESSION_H_

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

/**
 *
 */
class AST_Expression_ : public AST_Node_
{
public:
  /**
   *
   */
  virtual
  ~AST_Expression_ ();
  /**
   *
   */
  DEFINE_CLASS_PRINTER(AST_Expression)
  ;
  /**
   *
   * @return
   */
  virtual string
  print () const =0;
  /**
   *
   * @return
   */
  virtual ExpressionType
  expressionType ();
  /**
   *
   * @param visitor
   */
  void
  accept (AST_Visitor visitor);
  /**
   *
   */
  GET_AS(Expression,BinOp)
  ;
  /**
   *
   */
  GET_AS(Expression,Boolean)
  ;
  /**
   *
   */
  GET_AS(Expression,BooleanNot)
  ;
  /**
   *
   */
  GET_AS(Expression,Brace)
  ;
  /**
   *
   */
  GET_AS(Expression,Call)
  ;
  /**
   *
   */
  GET_AS(Expression,CallArgs)
  ;
  /**
   *
   */
  GET_AS(Expression,ComponentReference)
  ;
  /**
   *
   */
  GET_AS(Expression,Derivative)
  ;
  /**
   *
   */
  GET_AS(Expression,If)
  ;
  /**
   *
   */
  GET_AS(Expression,Integer)
  ;
  /**
   *
   */
  GET_AS(Expression,Output)
  ;
  /**
   *
   */
  GET_AS(Expression,Range)
  ;
  /**
   *
   */
  GET_AS(Expression,Real)
  ;
  /**
   *
   */
  GET_AS(Expression,UMinus)
  ;
  /**
   *
   */
  GET_AS(Expression,String)
  ;
  /**
   *
   */
  GET_AS(Expression,NamedArgument)
  ;
  /**
   *
   */
  GET_AS(Expression,ArrayIndex)
  ;
  /**
   *
   * @return
   */
  AST_Expression_If_ElseIf
  getAsElseIf ();
};

/**
 *
 */
class AST_Expression_Integer_ : public AST_Expression_
{
public:
  /**
   *
   * @param i
   */
  AST_Expression_Integer_ (AST_Integer i);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Integer
  val ();
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
private:
  AST_Integer _i;
};

/**
 *
 */
class AST_Expression_Real_ : public AST_Expression_
{
public:
  /**
   *
   * @param d
   */
  AST_Expression_Real_ (double d);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  double
  val ()
  {
    return (_d);
  }
  ;
private:
  double _d;
};

/**
 *
 */
class AST_Expression_String_ : public AST_Expression_
{
public:
  /**
   *
   * @param s
   */
  AST_Expression_String_ (string s);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  string
  str () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
private:
  string _s;
};

/**
 *
 */
class AST_Expression_Boolean_ : public AST_Expression_
{
public:
  /**
   *
   * @param b
   */
  AST_Expression_Boolean_ (bool b);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  bool
  value ();
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
private:
  bool _b;
};

/**
 *
 */
class AST_Expression_Derivative_ : public AST_Expression_
{
public:
  /**
   *
   * @param el
   */
  AST_Expression_Derivative_ (AST_ExpressionList el);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  AST_ExpressionList
  arguments ();
  /**
   *
   * @return
   */
  string
  print () const;
private:
  AST_ExpressionList _el;
};

/**
 *
 */
class AST_Expression_UMinus_ : public AST_Expression_
{
public:
  /**
   *
   * @param e
   */
  AST_Expression_UMinus_ (AST_Expression e);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Expression
  exp () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
private:
  AST_Expression _e;
};

/**
 *
 */
class AST_Expression_BooleanNot_ : public AST_Expression_
{
public:
  /**
   *
   * @param e
   */
  AST_Expression_BooleanNot_ (AST_Expression e);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Expression
  exp () const;
private:
  AST_Expression _e;
};

/**
 *
 */
class AST_Expression_Call_ : public AST_Expression_
{
public:
  /**
   *
   * @param name
   * @param args
   */
  AST_Expression_Call_ (AST_String name, AST_ExpressionList args);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_String
  name () const;
  /**
   *
   * @return
   */
  AST_ExpressionList
  arguments () const;
  /**
   *
   * @return
   */
  virtual ExpressionType
  expressionType ();
private:
  AST_String _name;
  AST_ExpressionList _args;
};

/**
 *
 */
class AST_Expression_CallArgs_ : public AST_Expression_
{
public:
  /**
   *
   * @param args
   */
  AST_Expression_CallArgs_ (AST_ExpressionList args);
  /**
   *
   * @return
   */
  AST_ExpressionList
  arguments () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
private:
  AST_ExpressionList _args;
};

/**
 *
 */
class AST_Expression_Brace_ : public AST_Expression_
{
public:
  /**
   *
   * @param args
   */
  AST_Expression_Brace_ (AST_ExpressionList args);
  /**
   *
   * @return
   */
  AST_ExpressionList
  arguments () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
private:
  AST_ExpressionList _args;
};

/**
 *
 */
class AST_Expression_ComponentReference_ : public AST_Expression_
{
public:
  /**
   *
   * @param name
   * @param
   */
  AST_Expression_ComponentReference_ (AST_String name, AST_ExpressionList);
  /**
   *
   */
  AST_Expression_ComponentReference_ ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @param
   * @param
   */
  void
  append (AST_String, AST_ExpressionList);
  /**
   *
   * @param
   * @param
   */
  void
  prepend (AST_String, AST_ExpressionList);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  AST_StringList
  names () const;
  /**
   *
   * @return
   */
  string
  name ();
  /**
   *
   * @return
   */
  const char *
  cname ();
  /**
   *
   * @return
   */
  bool
  hasIndexes () const;
  /**
   *
   * @return
   */
  AST_ExpressionListList
  indexes () const;
  AST_ExpressionList
  firstIndex();
private:
  AST_StringList _name;
  AST_ExpressionListList _indexes;
};

/**
 *
 */
class AST_Expression_BinOp_ : public AST_Expression_
{
public:
  /**
   *
   * @param e1
   * @param e2
   * @param t
   */
  AST_Expression_BinOp_ (AST_Expression e1, AST_Expression e2, BinOpType t);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  AST_Expression
  left () const;
  /**
   *
   * @return
   */
  AST_Expression
  right () const;
  /**
   *
   * @return
   */
  BinOpType
  binopType () const;
private:
  AST_Expression _e1, _e2;
  BinOpType _t;
};

/**
 *
 */
class AST_Expression_If_ : public AST_Expression_
{
public:
  /**
   *
   * @param cond
   * @param then
   * @param else_exp
   * @param elseif_list
   */
  AST_Expression_If_ (AST_Expression cond, AST_Expression then,
		      AST_Expression else_exp, AST_ExpressionList elseif_list);
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Expression
  condition () const;
  /**
   *
   * @return
   */
  AST_Expression
  then () const;
  /**
   *
   * @return
   */
  AST_Expression
  else_exp () const;
  /**
   *
   * @return
   */
  AST_ExpressionList
  elseif_list () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
private:
  AST_Expression _cond, _then, _else_exp;
  AST_ExpressionList _elseif_list;
};

/**
 *
 */
class AST_Expression_End_ : public AST_Expression_
{
public:
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
};

/**
 *
 */
class AST_Expression_Null_ : public AST_Expression_
{
public:
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
};

/**
 *
 */
class AST_Expression_If_ElseIf_ : public AST_Expression_
{
public:
  /**
   *
   * @param
   * @param
   */
  AST_Expression_If_ElseIf_ (AST_Expression, AST_Expression);
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  AST_Expression
  condition ();
  /**
   *
   * @return
   */
  AST_Expression
  then ();
private:
  AST_Expression _cond;
  AST_Expression _then;
};

/**
 *
 */
class AST_Expression_Colon_ : public AST_Expression_
{
public:
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
};

/**
 *
 */
class AST_Expression_Output_ : public AST_Expression_
{
public:
  /**
   *
   */
  AST_Expression_Output_ (AST_ExpressionList);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_ExpressionList
  expressionList ();
private:
  AST_ExpressionList _list;
};

/**
 *
 */
class AST_Expression_Range_ : public AST_Expression_
{
public:
  /**
   *
   */
  AST_Expression_Range_ (AST_ExpressionList);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_ExpressionList
  expressionList ();
private:
  AST_ExpressionList _list;
};

/**
 *
 */
class AST_Expression_NamedArgument_ : public AST_Expression_
{
public:
  /**
   *
   * @param name
   * @param exp
   */
  AST_Expression_NamedArgument_ (AST_String name, AST_Expression exp);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Expression
  expression ();
  /**
   *
   * @return
   */
  AST_String
  name ();
private:
  AST_Expression _exp;
  AST_String _name;
};

/**
 *
 */
class AST_Expression_ArrayIndex_ : public AST_Expression_
{
public:
  /**
   *
   * @param name
   * @param exp
   */
  AST_Expression_ArrayIndex_ (AST_Expression exp, int size);
  /**
   *
   * @return
   */
  ExpressionType
  expressionType ();
  /**
   *
   * @return
   */
  string
  print () const;
  /**
   *
   * @return
   */
  AST_Expression
  expression ();
  /**
   *
   * @return
   */
  int
  size ();
private:
  AST_Expression _exp;
  int _size;
};

#endif  /* AST_EXPRESSION_H_ */
