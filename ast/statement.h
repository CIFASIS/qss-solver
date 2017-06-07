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

#ifndef AST_STATEMENT_H_
#define AST_STATEMENT_H_

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

/**
 *
 */
class AST_Statement_ : public AST_Node_
{
public:
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Statement)
    ;
    /**
     *
     * @return
     */
    virtual string
    print () const = 0;
    /**
     *
     * @return
     */
    virtual StatementType
    statementType () = 0;
    /**
     *
     * @param visitor
     */
    void
    accept (AST_Visitor visitor);
    /**
     *
     */
    GET_AS(Statement,While)
    ;
    /**
     *
     */
    GET_AS(Statement,If)
    ;
    /**
     *
     */
    GET_AS(Statement,For)
    ;
    /**
     *
     */
    GET_AS(Statement,When)
    ;
    /**
     *
     */
    GET_AS(Statement,Assign)
    ;
};

/**
 *
 */
class AST_Statement_Return_ : public AST_Statement_
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
    virtual StatementType
    statementType ();
};

/**
 *
 */
class AST_Statement_Break_ : public AST_Statement_
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
    virtual StatementType
    statementType ();
};

/**
 *
 */
class AST_Statement_When_ : public AST_Statement_
{
public:
    /**
     *
     * @param cond
     * @param
     * @param
     * @param
     */
    AST_Statement_When_ (AST_Expression cond, AST_StatementList, AST_Statement_ElseList, AST_Comment);
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
    condition () const;
    /**
     *
     * @return
     */
    AST_StatementList
    statements () const;
    /**
     *
     * @return
     */
    AST_Statement_ElseList
    else_when () const;
    /**
     *
     * @return
     */
    AST_Comment
    comment () const;
    /**
     *
     * @return
     */
    bool
    hasComment ();
    /**
     *
     * @return
     */
    bool
    hasElsewhen ();
    /**
     *
     * @return
     */
    virtual StatementType
    statementType ();
private:
    AST_Expression _cond;
    AST_StatementList _sts;
    AST_Statement_ElseList _else_list;
    AST_Comment _comm;
};

/**
 *
 */
class AST_Statement_Assign_ : public AST_Statement_
{
public:
    /**
     *
     * @param cr
     * @param exp
     */
    AST_Statement_Assign_ (AST_Expression_ComponentReference cr, AST_Expression exp);
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
    AST_Expression_ComponentReference
    lhs () const;
    /**
     *
     * @return
     */
    virtual StatementType
    statementType ();
private:
    AST_Expression_ComponentReference _lhs;
    AST_Expression _exp;
};

/**
 *
 */
class AST_Statement_If_ : public AST_Statement_
{
public:
    /**
     *
     * @param cond
     * @param true_st
     * @param
     * @param false_st
     */
    AST_Statement_If_ (AST_Expression cond, AST_StatementList true_st, AST_Statement_ElseList, AST_StatementList false_st);
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
    condition () const;
    /**
     *
     * @return
     */
    AST_StatementList
    statements () const;
    /**
     *
     * @return
     */
    AST_StatementList
    else_statements () const;
    /**
     *
     * @return
     */
    AST_Statement_ElseList
    else_if () const;
    /**
     *
     * @return
     */
    virtual StatementType
    statementType ();
private:
    AST_Expression _cond;
    AST_StatementList _true_st, _false_st;
    AST_Statement_ElseList _else_list;
};

/**
 *
 */
class AST_Statement_While_ : public AST_Statement_
{
public:
    /**
     *
     * @param cond
     * @param
     */
    AST_Statement_While_ (AST_Expression cond, AST_StatementList);
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
    condition () const;
    /**
     *
     * @return
     */
    AST_StatementList
    statements () const;
    /**
     *
     * @return
     */
    virtual StatementType
    statementType ();
private:
    AST_Expression _cond;
    AST_StatementList _sts;
};

/**
 *
 */
class AST_Statement_For_ : public AST_Statement_
{
public:
    /**
     *
     * @param index
     * @param
     */
    AST_Statement_For_ (AST_ForIndexList index, AST_StatementList);
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
    AST_StatementList
    statements () const;
    /**
     *
     * @return
     */
    AST_ForIndexList
    forIndexList () const;
    /**
     *
     * @return
     */
    virtual StatementType
    statementType ();
private:
    AST_StatementList _sts;
    AST_ForIndexList _ind;
};

/**
 *
 */
class AST_Statement_OutputAssigment_ : public AST_Statement_
{
public:
    /**
     *
     * @param
     * @param
     * @param
     */
    AST_Statement_OutputAssigment_ (AST_ExpressionList, AST_Expression_ComponentReference, AST_ExpressionList);
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
    out_expressions () const;
    /**
     *
     * @return
     */
    AST_Expression_ComponentReference
    function () const;
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
    virtual StatementType
    statementType ();
private:
    AST_ExpressionList _out_exps;
    AST_Expression_ComponentReference _func;
    AST_ExpressionList _args;
};

/**
 *
 */
class AST_Statement_Else_ : public AST_Node_
{
public:
    /**
     *
     * @param cond
     * @param sts
     */
    AST_Statement_Else_ (AST_Expression cond, AST_StatementList sts);
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
    AST_StatementList
    statements () const;
    /**
     *
     * @param visitor
     */
    void
    accept (AST_Visitor visitor);
private:
    AST_Expression _cond;
    AST_StatementList _sts;
};

#endif  /* AST_STATEMENT_H_ */
