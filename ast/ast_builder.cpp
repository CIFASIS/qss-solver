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

#include <ast/ast_builder.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

#include "../parser/mocc_parser.h"
#include "ast_types.h"
#include "class.h"
#include "composition.h"
#include "element.h"
#include "equation.h"
#include "expression.h"
#include "modification.h"
#include "statement.h"
#include "stored_definition.h"

class MCC_Parser;
class MCC_Lexer;
int depth;

MCC_Lexer *MCC_Parser::lexer = NULL;

AST_StoredDefinition
newAST_StoredDefinition(AST_ClassList cl, AST_String within)
{
  return new AST_StoredDefinition_(cl, within);
}

AST_String
newAST_String(string s)
{
  return new string(s);
}

AST_String
newAST_String(char * s)
{
  return new string(s);
}

AST_String
newAST_String(const char * s)
{
  return new string(s);
}

AST_String
newAST_String(AST_String s)
{
  AST_String ret = new string(*s);
  delete s;
  return ret;
}

AST_String
copyAST_String(AST_String s)
{
  AST_String ret = new string(*s);
  return ret;
}

AST_String
newAST_DotString(AST_String s)
{
  AST_String ret = new string(*s);
  ret->insert(0, ".");
  delete s;
  return ret;
}

AST_String
AST_StringDotAppend(AST_String ret, AST_String a)
{
  if(ret == NULL)
  {
    ret = new string();
  }
  if(a != NULL)
  {
    ret->append(".");
    ret->append(*a);
    delete a;
  }
  return ret;
}

AST_String
newAST_StringNull()
{
  return NULL;
}

AST_ClassList
newAST_ClassList()
{
  return new list<AST_Class>();
}

AST_Composition
newAST_Composition()
{
  return (new AST_Composition_(newAST_ElementList(),
      newAST_CompositionElementList()));
}

AST_Composition
newAST_Composition(AST_ElementList el)
{
  return new AST_Composition_(el, newAST_CompositionElementList());
}

AST_Composition
newAST_Composition(AST_ElementList el, AST_CompositionElementList cl)
{
  return new AST_Composition_(el, cl);
}

AST_Composition
newAST_Composition(AST_CompositionElementList cl)
{
  return new AST_Composition_(newAST_ElementList(), cl);
}

AST_ElementList
newAST_ElementList()
{
  return new list<AST_Element>();
}

AST_Element
AST_ImportToElement(AST_Element_ImportClause i)
{
  return i;
}

AST_Element
AST_ExtendsToElement(AST_Element_ExtendsClause e)
{
  return e;
}

AST_Element_ExtendsClause
newAST_Element_ExtendsClause(AST_String name)
{
  AST_Element_ExtendsClause ret = new AST_Element_ExtendsClause_(*name);
  delete name;
  return ret;
}

AST_Element
newAST_Element_ClassWrapper(AST_Class c)
{
  return new AST_Element_ClassWrapper_(c);
}

AST_Element
AST_Element_ComponentToElement(AST_Element_Component c)
{
  return c;
}

AST_CompositionElementList
newAST_CompositionElementList()
{
  return new list<AST_CompositionElement>();
}

AST_Element_ImportClause
newAST_ImportClause(AST_String name)
{
  return new AST_Element_ImportClause_(*name);
}

AST_Expression
newAST_Expression_Integer(int i)
{
  return new AST_Expression_Integer_(i);
}

AST_ExpressionList
newAST_ExpressionList(AST_Expression e)
{
  return newAST_SimpleList(e);
}

AST_ExpressionList
newAST_ExpressionList()
{
  AST_ExpressionList l = new list<AST_Expression>();
  return l;
}

AST_Argument
AST_ArgumentSet(bool each, bool final, AST_Argument arg)
{
  if(each)
  {
    arg->setEach();
  }
  if(final)
  {
    arg->setFinal();
  }
  return arg;
}

AST_EquationList
newAST_EquationList()
{
  return new list<AST_Equation>();
}

AST_Equation
newAST_Equation_Equality(AST_Expression left, AST_Expression right)
{
  return new AST_Equation_Equality_(left, right);
}

AST_Equation
newAST_Equation_Connect(AST_Expression_ComponentReference cr1,
    AST_Expression_ComponentReference cr2)
{
  return new AST_Equation_Connect_(cr1, cr2);
}

AST_Expression
newAST_Expression_Real(AST_Real r)
{
  return new AST_Expression_Real_(r);
}

AST_Expression
newAST_Expression_String(AST_String s)
{
  AST_Expression_String ret = new AST_Expression_String_(*s);
  delete s;
  return ret;
}

AST_Expression
newAST_Expression_Boolean(AST_Boolean b)
{
  return new AST_Expression_Boolean_(b);
}

AST_Expression
newAST_Expression_Derivative(AST_ExpressionList el)
{
  return new AST_Expression_Derivative_(el);
}

AST_Class
newAST_Class(AST_String s)
{
  return newAST_Class(s, newAST_Composition());
}

AST_Class
newAST_Class(AST_String s, AST_Composition comp)
{
  return new AST_Class_(s, comp);
}

AST_Element_Component
newAST_Element_Component(AST_DeclarationList cl, AST_String type,
    AST_TypePrefix tp, AST_ExpressionList ind)
{
  AST_Element_Component c = new AST_Element_Component_(cl, *type, tp, ind);
  delete type;
  return c;
}

AST_Declaration
newAST_Declaration(AST_String s, AST_ExpressionList indexes, AST_Modification m)
{
  AST_Declaration ret = new AST_Declaration_(*s, indexes, m);
  delete s;
  return ret;
}

AST_DeclarationList
newAST_DeclarationList(AST_Declaration d)
{
  return newAST_SimpleList(d);
}

AST_CompositionElement
newAST_CompositionElement(AST_ElementList el)
{
  return new AST_CompositionElement_(el);
}

AST_Expression
newAST_Expression_ComponentReferenceExp(AST_String s)
{
  AST_Expression_ComponentReference e = newAST_Expression_ComponentReference();
  e->append(s, newAST_ExpressionList());
  return e;
}

AST_Expression
newAST_Expression_ComponentReferenceExp(AST_String s, AST_String s2)
{
  if(s2 != NULL)
  {
    s->append(*s2);
    delete s2;
  }
  return newAST_Expression_ComponentReferenceExp(s);
}

AST_Expression
newAST_Expression_ComponentReferenceExpDot(AST_String s, AST_String rest)
{
  s->insert(0, ".");
  s->append(rest->c_str());
  delete rest;
  return newAST_Expression_ComponentReferenceExp(s);
}

AST_Expression_ComponentReference
newAST_Expression_ComponentReference()
{
  // Empty cref
  return new AST_Expression_ComponentReference_();
}

AST_Expression_ComponentReference
AST_Expression_ComponentReference_AddDot(AST_Expression_ComponentReference cr,
    AST_String s, AST_ExpressionList subs)
{
  s->insert(0, ".");
  cr->prepend(s, subs);
  return cr;
}

AST_Expression_ComponentReference
AST_Expression_ComponentReference_Add(AST_Expression_ComponentReference cr,
    AST_String s, AST_ExpressionList subs)
{
  cr->append(s, subs);
  return cr;
}

ostream &
operator<<(ostream &os, const AST_CompositionElement &ce)
{
  assert(ce!=NULL);
  os << *ce;
  return os;
}

ostream &
operator<<(ostream &os, const AST_String &s)
{
  assert(s!=NULL);
  os << *s;
  return os;
}

AST_Element_ComponentList
newAST_Element_ComponentList()
{
  return new list<AST_Element_Component>();
}

AST_TypePrefix
newAST_TypePrefix()
{
  return 0;
}

AST_TypePrefix
newAST_TypePrefix(AST_TypePrefix tp)
{
  return tp;
}

AST_TypePrefix
AST_TypePrefixAdd(AST_TypePrefix tp, AST_TypePrefix tpadd)
{
  return tp | tpadd;
}

AST_Expression
newAST_Expression_BinOp(AST_Expression left, AST_Expression right,
    BinOpType type)
{
  /*if (type==BINOPMULT && right->expressionType()==EXPREAL && right->getAsReal()->val()==1.0)
   {
   return left;
   }*/
  if(type == BINOPMULT && right->expressionType() == EXPINTEGER
      && right->getAsInteger()->val() == 1)
  {
    return left;
  }
  /*if (type==BINOPMULT && left->expressionType()==EXPREAL && left->getAsReal()->val()==1.0)
   {
   return right;
   }*/
  if(type == BINOPMULT && left->expressionType() == EXPINTEGER
      && left->getAsInteger()->val() == 1)
  {
    return right;
  }
  if(type == BINOPMULT && right->expressionType() == EXPREAL
      && right->getAsReal()->val() == 0.0)
  {
    return right;
  }
  if(type == BINOPMULT && right->expressionType() == EXPINTEGER
      && right->getAsInteger()->val() == 0)
  {
    return right;
  }
  if(type == BINOPMULT && left->expressionType() == EXPREAL
      && left->getAsReal()->val() == 0.0)
  {
    return left;
  }
  if(type == BINOPMULT && left->expressionType() == EXPINTEGER
      && left->getAsInteger()->val() == 0)
  {
    return left;
  }
  return new AST_Expression_BinOp_(left, right, type);
}

AST_Expression
newAST_Expression_UnaryMinus(AST_Expression e)
{
  // Remove doble negation
  if(e->expressionType() == EXPUMINUS)
  {
    AST_Expression e2 = e->getAsUMinus()->exp();
    delete e;
    return e2;
  }
  return new AST_Expression_UMinus_(e);
}

AST_Expression
newAST_Expression_If(AST_Expression cond, AST_Expression then,
    AST_ExpressionList else_list, AST_Expression else_exp)
{
  return new AST_Expression_If_(cond, then, else_exp, else_list);
}

AST_Equation
newAST_Equation_If(AST_Expression e, AST_EquationList eql,
    AST_Equation_ElseList elseif, AST_EquationList elseeqs)
{
  return new AST_Equation_If_(e, eql, elseif, elseeqs);
}

AST_StringList
newAST_StringList()
{
  return new list<AST_String>();
}

AST_Expression
newAST_Expression_End()
{
  return new AST_Expression_End_();
}

AST_Expression
newAST_Expression_Null()
{
  return new AST_Expression_Null_();
}

AST_Expression
newAST_Expression_DotCall(AST_String name, AST_String rest,
    AST_ExpressionList args)
{
  name->insert(0, ".");
  if(rest != NULL)
  {
    name->append(rest->c_str());
    delete rest;
  }
  return new AST_Expression_Call_(name, args);
}

AST_Expression
newAST_Expression_Call(AST_String name, AST_String rest,
    AST_ExpressionList args)
{
  if(rest != NULL)
  {
    name->append(rest->c_str());
    delete rest;
  }
  return new AST_Expression_Call_(name, args);
}

AST_Expression
newAST_Expression_Colon()
{
  return new AST_Expression_Colon_();
}

AST_Expression
newAST_Expression_BooleanNot(AST_Expression e)
{
  return new AST_Expression_BooleanNot_(e);
}

AST_Equation
newAST_Equation_Call(AST_Expression e)
{
  if(e->expressionType() != EXPCALL)
  {
    cerr << "Not valid equation" << endl;
    exit(-1);
  }
  return new AST_Equation_Call_(e);
}

AST_ClassPrefix
AST_ClassPrefix_Class()
{
  return CP_CLASS;
}

AST_ClassPrefix
AST_ClassPrefix_Model()
{
  return CP_MODEL;
}

AST_ClassPrefix
AST_ClassPrefix_Record(AST_Boolean oper)
{
  if(oper)
  {
    return CP_RECORD | CP_OPERATOR;
  }
  return CP_RECORD;
}

AST_ClassPrefix
AST_ClassPrefix_Block()
{
  return CP_BLOCK;
}

AST_ClassPrefix
AST_ClassPrefix_Connector(AST_Boolean expandable)
{
  if(expandable)
  {
    return CP_CONNECTOR | CP_EXPANDABLE;
  }
  return CP_CONNECTOR;
}

AST_ClassPrefix
AST_ClassPrefix_Type()
{
  return CP_TYPE;
}

AST_ClassPrefix
AST_ClassPrefix_Package()
{
  return CP_PACKAGE;
}

AST_ClassPrefix
AST_ClassPrefix_Function(AST_ClassPrefix p)
{
  return CP_FUNCTION | p;
}

AST_ClassPrefix
AST_ClassPrefix_Operator()
{
  return CP_OPERATOR;
}

AST_ClassPrefix
AST_ClassPrefix_Impure(AST_Boolean oper)
{
  if(oper)
  {
    return CP_IMPURE | CP_OPERATOR;
  }
  return CP_IMPURE;
}

AST_ClassPrefix
AST_ClassPrefix_Pure(AST_Boolean oper)
{
  if(oper)
  {
    return CP_PURE | CP_OPERATOR;
  }
  return CP_PURE;
}

AST_ClassPrefix
AST_ClassPrefix_None()
{
  return 0;
}

AST_ClassPrefix
AST_ClassPrefix_AddPartial(AST_Boolean partial, AST_ClassPrefix p)
{
  if(partial)
  {
    return p | CP_PARTIAL;
  }
  return p;
}

AST_Equation
newAST_EquationFor(AST_ForIndexList ind, AST_EquationList eql)
{
  return new AST_Equation_For_(ind, eql);
}

AST_ForIndex
newAST_ForIndex(AST_String ident, AST_Expression exp)
{
  return new AST_ForIndex_(ident, exp);
}

AST_ForIndexList
newAST_ForIndexList()
{
  return new list<AST_ForIndex>();
}

AST_Equation
newAST_Equation_For(AST_ForIndexList ind, AST_EquationList eql)
{
  return new AST_Equation_For_(ind, eql);
}

AST_Class
AST_Class_SetFinal(AST_Class c, AST_Boolean final)
{
  if(final)
  {
    c->setFinal();
  }
  return c;
}

AST_Class
AST_Class_SetPrefixEncapsulated(AST_Class c, AST_ClassPrefix prefix,
    AST_Boolean enc)
{
  if(enc)
  {
    c->setEncapsulated();
  }
  c->setPrefixes(prefix);
  return c;
}

AST_Expression
AST_Expression_ComponentReferenceExpAddDot(AST_Expression_ComponentReference cr,
    AST_String id, AST_String rest, AST_ExpressionList subs)
{
  if(rest != NULL)
  {
    id->append(rest->c_str());
    delete rest;
  }
  return AST_Expression_ComponentReference_AddDot(cr, id, subs);
}

AST_Expression
AST_Expression_ComponentReferenceExpAdd(AST_Expression_ComponentReference cr,
    AST_String id, AST_String rest, AST_ExpressionList subs)
{
  if(rest != NULL)
  {
    id->append(rest->c_str());
    delete rest;
  }
  return AST_Expression_ComponentReference_Add(cr, id, subs);
}

AST_Expression
newAST_Expression_NamedArgument(AST_String s, AST_Expression e)
{
  AST_Expression_NamedArgument na = new AST_Expression_NamedArgument_(s, e);
  return na;
}

AST_Expression
newAST_Expression_FunctionCallArgs(AST_ExpressionList el)
{
  AST_Expression e = new AST_Expression_CallArgs_(el);
  return e;
}

AST_Expression
newAST_Expression_Function(AST_String, AST_ExpressionList)
{
  /* TODO */
  return NULL;
}

AST_Equation_ElseList
newAST_Equation_ElseIfList()
{
  return new list<AST_Equation_Else>();
}

AST_Equation_Else
newAST_Equation_Else(AST_Expression cond, AST_EquationList eqs)
{
  return new AST_Equation_Else_(cond, eqs);
}

AST_StatementList
newAST_StatementList()
{
  return new list<AST_Statement>();
}

AST_Statement
newAST_Statement_Break()
{
  return new AST_Statement_Break_();
}

AST_Statement
newAST_Statement_Return()
{
  return new AST_Statement_Return_();
}

AST_Modification
newAST_ModificationNull()
{
  return NULL;
}

AST_Statement_ElseList
newAST_Statement_ElseList()
{
  return new list<AST_Statement_Else>();
}

AST_Equation_ElseList
newAST_Equation_ElseList()
{
  return new list<AST_Equation_Else>();
}

AST_ArgumentList
newAST_ArgumentList()
{
  return new list<AST_Argument>();
}

AST_Argument
newAST_Argument_Modification(AST_String s, AST_Modification m)
{
  return new AST_Argument_Modification_(s, m);
}

AST_ExpressionListList
newAST_ExpressionListList()
{
  return new list<AST_ExpressionList>();
}

AST_Argument
AST_ArgumentSetReplaceable(AST_Argument arg)
{
  return arg;
}

AST_Modification
newAST_Modification_Equal(AST_Expression e)
{
  return new AST_Modification_Equal_(e);
}

AST_Modification
newAST_Modification_Assign(AST_Expression e)
{
  return new AST_Modification_Assign_(e);
}

AST_Expression
newAST_Expression_Range(AST_Expression e1, AST_Expression e2)
{
  AST_ExpressionList l;
  if(e1->expressionType() == EXPRANGE)
  {
    l = e1->getAsRange()->expressionList();
    if(l->size() > 2)
    {
      cerr << "Not valid range expression: line " << e1->lineNum() << endl;
      exit(-1);
    }
    l = AST_ListAppend(l, e2);
  }
  else
  {
    l = newAST_ExpressionList(e1);
    l = AST_ListAppend(l, e2);
  }
  return new AST_Expression_Range_(l);
}

AST_Expression
newAST_Expression_OutputExpressions(AST_ExpressionList exp_list)
{
  return new AST_Expression_Output_(exp_list);
}

AST_Expression
newAST_Expression_Brace(AST_ExpressionList el)
{
  return new AST_Expression_Brace_(el);
}

AST_Expression
newAST_BracketExpList(AST_ExpressionListList)
{
  return newAST_Expression_Null();
}

AST_Expression
newAST_Expression_ElseIf(AST_Expression c, AST_Expression t)
{
  return new AST_Expression_If_ElseIf_(c, t);
}

AST_Statement_Else
newAST_Statement_Else(AST_Expression cond, AST_StatementList st)
{
  return new AST_Statement_Else_(cond, st);
}

AST_Statement
newAST_Statement_If(AST_Expression cond, AST_StatementList true_st,
    AST_Statement_ElseList else_st, AST_StatementList false_st)
{
  return new AST_Statement_If_(cond, true_st, else_st, false_st);
}

AST_Statement
newAST_Statement_For(AST_ForIndexList index, AST_StatementList st)
{
  return new AST_Statement_For_(index, st);
}

AST_Statement
newAST_Statement_While(AST_Expression cond, AST_StatementList st)
{
  return new AST_Statement_While_(cond, st);
}

AST_Statement
newAST_Statement_When(AST_Expression cond, AST_StatementList sts,
    AST_Statement_ElseList else_st, AST_Comment c)
{
  return new AST_Statement_When_(cond, sts, else_st, c);
}

AST_Statement
newAST_Statement_OutputAssign(AST_ExpressionList out_vars,
    AST_Expression_ComponentReference funname, AST_ExpressionList args)
{
  return new AST_Statement_OutputAssigment_(out_vars, funname, args);
}

AST_Statement
newAST_Statement_Assign(AST_Expression_ComponentReference cr,
    AST_Expression exp)
{
  return new AST_Statement_Assign_(cr, exp);
}

AST_Equation
newAST_Equation_When(AST_Expression cond, AST_EquationList eqs,
    AST_Equation_ElseList else_list)
{
  return new AST_Equation_When_(cond, eqs, else_list);
}

AST_CompositionEqsAlgs
newAST_CompositionInitialEquations(AST_EquationList eqlist)
{
  return new AST_CompositionEqsAlgs_(eqlist, true);
}

AST_CompositionEqsAlgs
newAST_CompositionEquations(AST_EquationList eqlist)
{
  return new AST_CompositionEqsAlgs_(eqlist);
}

AST_CompositionEqsAlgs
newAST_CompositionInitialAlgorithms(AST_StatementList stlist)
{
  return new AST_CompositionEqsAlgs_(stlist, true);
}

AST_CompositionEqsAlgs
newAST_CompositionAlgorithms(AST_StatementList stlist)
{
  return new AST_CompositionEqsAlgs_(stlist);
}

AST_ShortClassExp
newAST_ShortClassExp(AST_TypePrefix, AST_String, AST_ExpressionList,
    AST_ArgumentList)
{
  /* TODO */
  return NULL;
}
AST_ShortClassExp
newAST_ShortClassExp_Enum(AST_StringList)
{
  /* TODO */
  return NULL;
}

AST_Argument
newAST_Redeclaration(AST_Boolean, AST_Boolean, AST_Argument)
{
  /* TODO */
  return NULL;
}

AST_Argument
newAST_ShortClass(AST_ClassPrefix, AST_String, AST_ShortClassExp)
{
  /* TODO */
  return NULL;
}

AST_Argument
newAST_ElementModification(AST_String n, AST_Modification m)
{
  return new AST_Argument_Modification_(n, m);
}

AST_Modification
newAST_Modification_Class(AST_ArgumentList al, AST_Expression e)
{
  return new AST_Modification_Class_(al, e);
}

AST_CompositionElement
newAST_CompositionElement(AST_CompositionEqsAlgs comp_eq_algs)
{
  return new AST_CompositionElement_(comp_eq_algs);
}

AST_Class
newAST_ClassExtends(AST_String, AST_Composition)
{
  /* TODO */
  return NULL;
}

AST_Class
newAST_ClassEnumeration(AST_String, AST_StringList)
{
  /* TODO */
  return NULL;
}

AST_Class
newAST_ClassDerivation(AST_String, AST_String, AST_StringList)
{
  /* TODO */
  return NULL;
}

AST_Class
newAST_ClassModification(AST_String, AST_TypePrefix, AST_String,
    AST_ExpressionList, AST_ArgumentList)
{
  /* TODO */
  return NULL;
}

AST_CompositionEqsAlgs
newAST_NullCompositionEquations()
{
  return NULL;
}

AST_Declaration
AST_Declaration_AddCondComment(AST_Declaration decl, AST_Expression exp,
    AST_Comment comment)
{
  decl->setComment(comment);
  return decl;
}

AST_Comment
newAST_Comment(AST_String st, AST_ArgumentList al)
{
  if(al->size() || st != NULL)
  {
    return new AST_Comment_(st, al);
  }
  return NULL;
}

AST_External_Function_Call
newAST_ExternalCall()
{
  return NULL;
}

AST_External_Function_Call
newAST_ExternalCall(AST_String lang, AST_Expression_ComponentReference cr,
    AST_Expression args, AST_ArgumentList annot)
{
  return new AST_External_Function_Call_(lang, cr, args, annot);
}

AST_Composition
AST_Composition_SetExternalAnnotation(AST_Composition c,
    AST_External_Function_Call ext, AST_ArgumentList annot)
{
  c->setExternalFunctionCall(ext);
  c->setAnnotation(annot);
  return c;
}
