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

#ifndef AST_COMPOSITION_H_
#define AST_COMPOSITION_H_

#include <string>

#include "../util/macros.h"
#include "../util/util_types.h"
#include "ast_node.h"
#include "ast_types.h"

class AST_CompositionElement_;

using namespace std;

/**
 *
 */
class AST_Composition_: public AST_Node_
{
  public:
    /**
     *
     * @param el
     * @param cl
     */
    AST_Composition_(AST_ElementList el, AST_CompositionElementList cl);
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Composition)
    ;
    /**
     *
     * @return
     */
    AST_CompositionElementList
    compositionList() const;
    /**
     *
     * @return
     */
    bool
    hasCompositionList();
    /**
     *
     * @return
     */
    AST_ElementList
    elementList() const;
    /**
     *
     * @return
     */
    bool
    hasElementList();
    /**
     *
     * @param
     */
    void
    setExternalFunctionCall(AST_External_Function_Call);
    /**
     *
     * @return
     */
    AST_External_Function_Call
    externalCall() const;
    /**
     *
     * @return
     */
    bool
    hasExternalFunctionCall();
    /**
     *
     * @param
     */
    void
    setAnnotation(AST_ArgumentList);
    /**
     *
     * @return
     */
    bool
    hasAnnotation();
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    AST_CompositionElementList _composition_list;
    AST_ElementList _element_list;
    AST_ArgumentList _annot;
    AST_External_Function_Call _ext;
};

/**
 *
 */
class AST_CompositionElement_: public AST_Node_
{
  public:
    /**
     *
     * @param eqs_algs
     */
    AST_CompositionElement_(AST_CompositionEqsAlgs eqs_algs);
    /**
     *
     * @param el
     */
    AST_CompositionElement_(AST_ElementList el);
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_CompositionElement)
    ;
    /**
     *
     * @return
     */
    AST_CompositionEqsAlgs
    getEquationsAlgs();
    /**
     *
     * @return
     */
    AST_ElementList
    getElementList();
    /**
     *
     * @return
     */
    bool
    hasEquations();
    /**
     *
     * @return
     */
    bool
    hasStatements();
    /**
     *
     * @return
     */
    bool
    hasElements();
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    AST_CompositionEqsAlgs _eqs_algs;
    AST_ElementList _el;
};

/**
 *
 */
class AST_CompositionEqsAlgs_: public AST_Node_
{
  public:
    /**
     *  @param eq
     */
    AST_CompositionEqsAlgs_(AST_EquationList eq);
    /**
     *
     * @param st
     */
    AST_CompositionEqsAlgs_(AST_StatementList st);
    /**
     *
     * @param st
     * @param initial
     */
    AST_CompositionEqsAlgs_(AST_StatementList st, bool initial);
    /**
     *
     * @param eq
     * @param initial
     */
    AST_CompositionEqsAlgs_(AST_EquationList eq, bool initial);
    /**
     *
     * @return
     */
    AST_EquationList
    getEquations();
    /**
     *
     * @return
     */
    AST_StatementList
    getAlgorithms();
    /**
     *
     * @return
     */
    bool
    isInitial();
    /**
     *
     * @return
     */
    bool
    hasEquations();
    /**
     *
     * @return
     */
    bool
    hasStatements();
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    bool _initial;
    AST_EquationList _eq;
    AST_StatementList _st;
};

/**
 *
 */
class AST_External_Function_Call_: public AST_Node_
{
  public:
    /**
     *
     * @param
     * @param
     * @param
     * @param
     */
    AST_External_Function_Call_(AST_String, AST_Expression_ComponentReference,
        AST_Expression, AST_ArgumentList);
    /**
     *
     * @return
     */
    AST_String
    language();
    /**
     *
     * @return
     */
    string
    languageString();
    /**
     *
     * @return
     */
    AST_ArgumentList
    annotation();
    /**
     *
     * @return
     */
    AST_ExpressionList
    args();
    /**
     *
     * @return
     */
    string
    name();
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
    AST_Expression_ComponentReference
    componentReference();
    /**
     *
     * @return
     */
    bool
    hasComponentReference();
    /**
     *
     * @param visitor
     */
    void
    accept(AST_Visitor visitor);
    private:
    AST_String _lang;
    AST_ArgumentList _annot;
    AST_Expression_Call _call;
    AST_Expression _exp;
    AST_Expression_ComponentReference _cr;
};

#endif  /* AST_COMPOSITION_H_ */
