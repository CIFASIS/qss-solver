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

#ifndef AST_MODIFICATION_H_
#define AST_MODIFICATION_H_

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

/**
 *
 */
class AST_Argument_: public AST_Node_
{
  public:
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Argument)
    ;
    /**
     *
     */
    AST_Argument_();
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
    virtual ArgumentType
    argumentType() const =0;
    /**
     *
     */
    void
    setFinal();
    /**
     *
     */
    void
    setEach();
    /**
     *
     * @return
     */
    bool
    isFinal() const;
    /**
     *
     * @return
     */
    bool
    hasEach() const;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    /**
     *
     */
    GET_AS(Argument,Modification)
    ;
    /**
     *
     */
    GET_AS(Argument,Redeclaration)
    ;
    private:
    bool _final;
    bool _each;
};

/**
 *
 */
class AST_Argument_Modification_: public AST_Argument_
{
  public:
    /**
     *
     * @param name
     * @param m
     */
    AST_Argument_Modification_(AST_String name, AST_Modification m);
    /**
     *
     * @return
     */
    AST_String
    name() const;
    /**
     *
     * @return
     */
    AST_Modification
    modification() const;
    /**
     *
     * @return
     */
    bool
    hasModification();
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
    ArgumentType
    argumentType() const;
    private:
    AST_String _name;
    AST_Modification _mod;
};

/**
 *
 */
class AST_Argument_Redeclaration_: public AST_Argument_
{
    /**
     *
     */
    AST_Argument_Redeclaration_();
    /**
     *
     * @return
     */
    string
    print() const;
    ArgumentType
    argumentType() const;
};

/**
 *
 */
class AST_Modification_: public AST_Node_
{
  public:
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Modification)
    ;
    /**
     *
     * @return
     */
    virtual ModificationType
    modificationType();
    /**
     *
     * @return
     */
    virtual string
    print() const =0;
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    /**
     *
     */
    GET_AS(Modification,Equal)
    ;
    /**
     *
     */
    GET_AS(Modification,Class)
    ;
    /**
     *
     */
    GET_AS(Modification,Assign)
    ;
};

/**
 *
 */
class AST_Modification_Equal_: public AST_Modification_
{
  public:
    /**
     *
     * @param e
     */
    AST_Modification_Equal_(AST_Expression e);
    /**
     *
     * @return
     */
    AST_Expression
    exp();
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
    virtual ModificationType
    modificationType();
    private:
    AST_Expression _e;
};

/**
 *
 */
class AST_Modification_Assign_: public AST_Modification_
{
  public:
    /**
     *
     * @param e
     */
    AST_Modification_Assign_(AST_Expression e);
    /**
     *
     * @return
     */
    AST_Expression
    exp();
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
    virtual ModificationType
    modificationType();
    private:
    AST_Expression _e;
};

/**
 *
 */
class AST_Modification_Class_: public AST_Modification_
{
  public:
    /**
     *
     * @param al
     * @param e
     */
    AST_Modification_Class_(AST_ArgumentList al, AST_Expression e);
    /**
     *
     * @return
     */
    AST_Expression
    exp() const;
    /**
     *
     * @return
     */
    bool
    hasExpression();
    /**
     *
     * @return
     */
    AST_ArgumentList
    arguments() const;
    /**
     *
     * @return
     */
    string
    print() const;
    /**
     *
     * @param al
     */
    void
    setArguments(AST_ArgumentList al);
    /**
     *
     * @return
     */
    virtual ModificationType
    modificationType();
    private:
    AST_Expression _e;
    AST_ArgumentList _al;
};

#endif  /* AST_MODIFICATION_H_ */
