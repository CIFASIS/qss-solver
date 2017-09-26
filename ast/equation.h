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

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

#ifndef AST_EQUATION_H_
#define AST_EQUATION_H_

/**
 *
 */
class AST_Equation_: public AST_Node_
{
  public:
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Equation)
    ;
    /**
     *
     * @return
     */
    virtual string
    print() const =0;
    /**
     *
     * @return
     */
    virtual EquationType
    equationType() = 0;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    /**
     *
     */
    GET_AS(Equation,Call)
    ;
    /**
     *
     */
    GET_AS(Equation,Connect)
    ;
    /**
     *
     */
    GET_AS(Equation,Equality)
    ;
    /**
     *
     */
    GET_AS(Equation,For)
    ;
    /**
     *
     */
    GET_AS(Equation,If)
    ;
    /**
     *
     */
    GET_AS(Equation,When)
    ;
};

/**
 *
 */
class AST_Equation_Equality_: public AST_Equation_
{
  public:
    /**
     *
     * @param left
     * @param right
     */
    AST_Equation_Equality_(AST_Expression left, AST_Expression right);
    /**
     *
     * @return
     */
    EquationType
    equationType();
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_Expression
    left() const;
    /**
     *
     * @return
     */
    AST_Expression
    right() const;
    /**
     *
     * @param
     */
    void
    setLeft(AST_Expression);
    /**
     *
     * @param
     */
    void
    setRight(AST_Expression);
    private:
    AST_Expression _left, _right;
};

/**
 *
 */
class AST_Equation_Connect_: public AST_Equation_
{
  public:
    /**
     *
     * @param cr1
     * @param cr2
     */
    AST_Equation_Connect_(AST_Expression_ComponentReference cr1,
        AST_Expression_ComponentReference cr2);
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_Expression_ComponentReference
    left();
    /**
     *
     * @return
     */
    AST_Expression_ComponentReference
    right();
    /**
     *
     * @return
     */
    virtual EquationType
    equationType();
    private:
    AST_Expression_ComponentReference _cr1, _cr2;
};

/**
 *
 */
class AST_Equation_Call_: public AST_Equation_
{
  public:
    /**
     *
     * @param e
     */
    AST_Equation_Call_(AST_Expression e);
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    virtual EquationType
    equationType();
    /**
     *
     * @return
     */
    AST_Expression
    call() const;
    private:
    AST_Expression _call;
};

/**
 *
 */
class AST_Equation_If_: public AST_Equation_
{
  public:
    /**
     *
     * @param cond
     * @param eql
     * @param elseif
     * @param else_eq
     */
    AST_Equation_If_(AST_Expression cond, AST_EquationList eql,
        AST_Equation_ElseList elseif, AST_EquationList else_eq);
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_Expression
    condition() const;
    /**
     *
     * @param c
     */
    void
    setCondition(AST_Expression c);
    /**
     *
     * @return
     */
    AST_EquationList
    equationList() const;
    /**
     *
     * @return
     */
    AST_EquationList
    equationElseList() const;
    /**
     *
     * @return
     */
    AST_Equation_ElseList
    equationElseIf() const;
    /**
     *
     * @return
     */
    EquationType
    equationType();
    private:
    AST_Expression _cond;
    AST_EquationList _eql;
    AST_EquationList _else;
    AST_Equation_ElseList _else_if;
};

/**
 *
 */
class AST_Equation_For_: public AST_Equation_
{
  public:
    /**
     *
     * @param ind
     * @param eql
     */
    AST_Equation_For_(AST_ForIndexList ind, AST_EquationList eql);
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_EquationList
    equationList() const;
    /**
     *
     * @return
     */
    AST_ForIndexList
    forIndexList() const;
    /**
     *
     * @return
     */
    EquationType
    equationType();
    private:
    AST_EquationList _eql;
    AST_ForIndexList _ind;
};

/**
 *
 */
class AST_ForIndex_: public AST_Node_
{
  public:
    /**
     *
     * @param var
     * @param exp
     */
    AST_ForIndex_(AST_String var, AST_Expression exp);
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_ForIndex)
    ;
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_String
    variable() const;
    /**
     *
     * @return
     */
    AST_Expression
    in_exp() const;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    AST_String _var;
    AST_Expression _in;
};

/**
 *
 */
class AST_Equation_Else_: public AST_Node_
{
  public:
    /**
     *
     * @param cond
     * @param eqs
     */
    AST_Equation_Else_(AST_Expression cond, AST_EquationList eqs);
    /**
     *
     * @return
     */
    AST_Expression
    condition() const;
    /**
     *
     * @param e
     */
    void
    setCondition(AST_Expression e);
    /**
     *
     * @return
     */
    AST_EquationList
    equations() const;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    AST_Expression _cond;
    AST_EquationList _eqs;
};

/**
 *
 */
class AST_Equation_When_: public AST_Equation_
{
  public:
    /**
     *
     * @param cond
     * @param eql
     * @param elsewhen
     */
    AST_Equation_When_(AST_Expression cond, AST_EquationList eql,
        AST_Equation_ElseList elsewhen);
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @return
     */
    AST_Expression
    condition() const;
    /**
     *
     * @return
     */
    AST_EquationList
    equationList() const;
    /**
     *
     * @return
     */
    EquationType
    equationType();
    /**
     *
     * @return
     */
    AST_Equation_ElseList
    equationElseWhen() const;
    /**
     *
     * @param e
     */
    void
    setCondition(AST_Expression e);
    private:
    AST_Expression _cond;
    AST_EquationList _eql;
    AST_Equation_ElseList _else_when;
};

#endif  /* AST_EQUATION_H_ */
