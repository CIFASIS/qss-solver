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

#ifndef AST_BUILDER_H_
#define AST_BUILDER_H_

#include <string>

#include "ast_types.h"

/* Constructors */
/**
 *
 * @return
 */
AST_ArgumentList
newAST_ArgumentList();
/**
 *
 * @param
 * @param
 * @return
 */
AST_Argument
newAST_Argument_Modification(AST_String, AST_Modification);
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
AST_Argument
newAST_Redeclaration(AST_Boolean, AST_Boolean, AST_Argument);
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
AST_Argument
newAST_ShortClass(AST_ClassPrefix, AST_String, AST_ShortClassExp);
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @return
 */
AST_ShortClassExp
newAST_ShortClassExp(AST_TypePrefix, AST_String, AST_ExpressionList,
    AST_ArgumentList);
/**
 *
 * @param
 * @return
 */
AST_ShortClassExp
newAST_ShortClassExp_Enum(AST_StringList);
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
AST_Argument
AST_ArgumentSet(AST_Boolean, AST_Boolean, AST_Argument);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Argument
newAST_ElementModification(AST_String, AST_Modification);
/**
 *
 * @param
 * @return
 */
AST_Argument
AST_ArgumentSetReplaceable(AST_Argument);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Comment
newAST_Comment(AST_String, AST_ArgumentList);

/* Strings */
/**
 *
 * @param
 * @return
 */
AST_String
newAST_String(AST_String);
/**
 *
 * @return
 */
AST_String
newAST_StringNull();
/**
 *
 * @param s
 * @return
 */
AST_String
newAST_String(string s);
/**
 *
 * @param s
 * @return
 */
AST_String
newAST_String(char * s);
/**
 *
 * @param s
 * @return
 */
AST_String
newAST_String(const char * s);
/**
 *
 * @param
 * @return
 */
AST_String
newAST_String(AST_String);
/**
 *
 * @param
 * @return
 */
AST_String
newAST_DotString(AST_String);
/**
 *
 * @param
 * @param
 * @return
 */
AST_String
AST_StringDotAppend(AST_String, AST_String);
/**
 *
 * @return
 */
AST_StringList
newAST_StringList();
/**
 *
 * @param
 * @return
 */
AST_String
copyAST_String(AST_String);
/**
 *
 * @param
 * @param
 * @return
 */
#define toStr(it) (*it)

/* Classes */
/**
 *
 * @param
 * @param
 * @return
 */
AST_Class
newAST_Class(AST_String, AST_Composition);
/**
 *
 * @param
 * @return
 */
AST_Class
newAST_Class(AST_String);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Class
newAST_ClassEnumeration(AST_String, AST_StringList);
/**
 *
 * @param
 * @param
 * @param
 * @return
 */
AST_Class
newAST_ClassDerivation(AST_String, AST_String, AST_StringList);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Class
newAST_ClassExtends(AST_String, AST_Composition);
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @return
 */
AST_Class
newAST_ClassModification(AST_String, AST_TypePrefix, AST_String,
    AST_ExpressionList, AST_ArgumentList);
AST_ClassList
newAST_ClassList();
AST_Class
newAST_Class(AST_String s);
AST_Class
AST_Class_SetFinal(AST_Class, AST_Boolean);
AST_Class
AST_Class_SetPrefixEncapsulated(AST_Class, AST_ClassPrefix, AST_Boolean);

/* Classes Prefixes */
AST_ClassPrefix
AST_ClassPrefix_AddPartial(AST_Boolean, AST_ClassPrefix);
AST_ClassPrefix
AST_ClassPrefix_Class();
AST_ClassPrefix
AST_ClassPrefix_Model();
AST_ClassPrefix
AST_ClassPrefix_Record(AST_Boolean);
AST_ClassPrefix
AST_ClassPrefix_Block();
AST_ClassPrefix
AST_ClassPrefix_Connector(AST_Boolean);
AST_ClassPrefix
AST_ClassPrefix_Type();
AST_ClassPrefix
AST_ClassPrefix_Package();
AST_ClassPrefix
AST_ClassPrefix_Function(AST_ClassPrefix);
AST_ClassPrefix
AST_ClassPrefix_Operator();
AST_ClassPrefix
AST_ClassPrefix_None();
AST_ClassPrefix
AST_ClassPrefix_Pure(AST_Boolean);
AST_ClassPrefix
AST_ClassPrefix_Impure(AST_Boolean);

/* Compositions */
AST_Composition
newAST_Composition();
AST_Composition
newAST_Composition(AST_ElementList);
AST_Composition
newAST_Composition(AST_ElementList el, AST_CompositionElementList cl);
AST_Composition
newAST_Composition(AST_CompositionElementList cl);
AST_Composition
AST_Composition_SetExternalAnnotation(AST_Composition c,
    AST_External_Function_Call ext, AST_ArgumentList annot);
AST_CompositionEqsAlgs
newAST_NullCompositionEquations();
AST_CompositionEqsAlgs
newAST_CompositionInitialEquations(AST_EquationList);
AST_CompositionEqsAlgs
newAST_CompositionEquations(AST_EquationList);
AST_CompositionEqsAlgs
newAST_CompositionInitialAlgorithms(AST_StatementList);
AST_CompositionEqsAlgs
newAST_CompositionAlgorithms(AST_StatementList);

/* Components and elements */
AST_Element_Component
newAST_Element_Component(AST_DeclarationList cl, AST_String type,
    AST_TypePrefix, AST_ExpressionList);
AST_Element_ComponentList
newAST_Element_ComponentList(AST_DeclarationList);
AST_Element_ComponentList
newAST_Element_ComponentList();
AST_ElementList
newAST_ElementList();
AST_Element_ImportClause
newAST_ImportClause(AST_String);
AST_Element
AST_ImportToElement(AST_Element_ImportClause i);
AST_Element
AST_Element_ComponentToElement(AST_Element_Component c);
AST_Element
AST_ExtendsToElement(AST_Element_ExtendsClause e);
AST_Element_ExtendsClause
newAST_Element_ExtendsClause(AST_String);
AST_Element
newAST_Element_ClassWrapper(AST_Class);
AST_CompositionElementList
newAST_CompositionElementList();
AST_Declaration
newAST_Declaration(AST_String, AST_ExpressionList, AST_Modification);
AST_Declaration
AST_Declaration_AddCondComment(AST_Declaration, AST_Expression, AST_Comment);
AST_DeclarationList
newAST_DeclarationList(AST_Declaration);
AST_CompositionElement
newAST_CompositionElement(AST_CompositionEqsAlgs);
AST_CompositionElement
newAST_CompositionElement(AST_ElementList);

/* Equations */
AST_EquationList
newAST_EquationList();
AST_Equation
newAST_Equation_Equality(AST_Expression, AST_Expression);
AST_Equation
newAST_Equation_Connect(AST_Expression_ComponentReference,
    AST_Expression_ComponentReference);
AST_Equation
newAST_Equation_If(AST_Expression e, AST_EquationList eql,
    AST_Equation_ElseList, AST_EquationList);
AST_Equation
newAST_Equation_For(AST_ForIndexList, AST_EquationList eql);
AST_Equation
newAST_Equation_When(AST_Expression, AST_EquationList, AST_Equation_ElseList);
AST_Equation
newAST_Equation_Call(AST_Expression);
AST_ForIndex
newAST_ForIndex(AST_String, AST_Expression);
AST_ForIndexList
newAST_ForIndexList();
AST_Equation_ElseList
newAST_Equation_ElseList();
AST_Equation_Else
newAST_Equation_Else(AST_Expression, AST_EquationList);

/* Expressions */
AST_ExpressionList
newAST_ExpressionList(AST_Expression);
AST_ExpressionList
newAST_ExpressionList();
AST_ExpressionListList
newAST_ExpressionListList();
AST_Expression
newAST_Expression_Integer(AST_Integer i);
AST_Expression
newAST_Expression_Real(AST_Real r);
AST_Expression
newAST_Expression_String(AST_String s);
AST_Expression
newAST_Expression_Boolean(AST_Boolean b);
AST_Expression
newAST_Expression_Derivative(AST_ExpressionList el);
AST_Expression_ComponentReference
newAST_Expression_ComponentReference();
AST_Expression_ComponentReference
AST_Expression_ComponentReference_Add(AST_Expression_ComponentReference,
    AST_String, AST_ExpressionList);
AST_Expression_ComponentReference
AST_Expression_ComponentReference_AddDot(AST_Expression_ComponentReference,
    AST_String, AST_ExpressionList);
AST_Expression
newAST_Expression_ComponentReferenceExp(AST_String);
AST_Expression
newAST_Expression_ComponentReferenceExp(AST_String s, AST_String s2);
AST_Expression
newAST_Expression_ComponentReferenceExpDot(AST_String s, AST_String s2);
AST_Expression
AST_Expression_ComponentReferenceExpAddDot(AST_Expression_ComponentReference,
    AST_String, AST_String, AST_ExpressionList);
AST_Expression
AST_Expression_ComponentReferenceExpAdd(AST_Expression_ComponentReference,
    AST_String, AST_String, AST_ExpressionList);
AST_Expression
newAST_Expression_BinOp(AST_Expression, AST_Expression, BinOpType);
AST_Expression
newAST_Expression_UnaryMinus(AST_Expression);
AST_Expression
newAST_Expression_If(AST_Expression, AST_Expression, AST_ExpressionList,
    AST_Expression);
AST_Expression
newAST_Expression_Null();
AST_Expression
newAST_Expression_Colon();
AST_Expression
newAST_Expression_End();
AST_Expression
newAST_Expression_Call(AST_String, AST_String, AST_ExpressionList);
AST_Expression
newAST_Expression_DotCall(AST_String, AST_String, AST_ExpressionList);
AST_Expression
newAST_Expression_BooleanNot(AST_Expression);
AST_Expression
newAST_Expression_ElseIf(AST_Expression, AST_Expression);
AST_Expression
newAST_Expression_NamedArgument(AST_String, AST_Expression);
AST_Expression
newAST_Expression_FunctionCallArgs(AST_ExpressionList);
AST_Expression
newAST_Expression_Function(AST_String, AST_ExpressionList);
AST_Expression
newAST_Expression_OutputExpressions(AST_ExpressionList);
AST_Expression
newAST_Expression_Range(AST_Expression, AST_Expression);
AST_Expression
newAST_Expression_Brace(AST_ExpressionList);
AST_Expression
newAST_BracketExpList(AST_ExpressionListList);

/* External funciton call */
AST_External_Function_Call
newAST_ExternalCall();
AST_External_Function_Call
newAST_ExternalCall(AST_String lang, AST_Expression_ComponentReference cr,
    AST_Expression args, AST_ArgumentList annot);

/* Modification */
AST_Modification
newAST_Modification_Class(AST_ArgumentList, AST_Expression);
AST_Modification
newAST_Modification_Equal(AST_Expression);
AST_Modification
newAST_Modification_Assign(AST_Expression);
AST_Modification
newAST_ModificationNull();

/* Statements */
AST_Statement
newAST_Statement_Break();
AST_Statement
newAST_Statement_Return();
AST_Statement
newAST_Statement_While(AST_Expression, AST_StatementList);
AST_Statement
newAST_Statement_When(AST_Expression, AST_StatementList, AST_Statement_ElseList,
    AST_Comment);
AST_Statement
newAST_Statement_For(AST_ForIndexList, AST_StatementList);
AST_Statement
newAST_Statement_If(AST_Expression, AST_StatementList, AST_Statement_ElseList,
    AST_StatementList);
AST_Statement
newAST_Statement_Assign(AST_Expression_ComponentReference, AST_Expression);
AST_Statement
newAST_Statement_OutputAssign(AST_ExpressionList,
    AST_Expression_ComponentReference,
    /**
     *
     */AST_ExpressionList);
AST_StatementList
newAST_StatementList();
/**
 *
 * @param
 * @param
 * @return
 */
AST_Statement_Else
newAST_Statement_Else(AST_Expression, AST_StatementList);
/**
 *
 * @return
 */
AST_Statement_ElseList
newAST_Statement_ElseList();

/**
 *
 * @param
 * @param
 * @return
 */
AST_StoredDefinition
newAST_StoredDefinition(AST_ClassList, AST_String);
/**
 *
 * @return
 */
AST_TypePrefix
newAST_TypePrefix();
/**
 *
 * @param
 * @return
 */
AST_TypePrefix
newAST_TypePrefix(AST_TypePrefix);
/**
 *
 * @param
 * @param
 * @return
 */
AST_TypePrefix
AST_TypePrefixAdd(AST_TypePrefix, AST_TypePrefix);

#endif  /* AST_BUILDER_H_ */
