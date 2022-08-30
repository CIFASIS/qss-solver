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

#include "ast_util.h"

#include <math.h>
#include <list>
#include <map>
#include <set>
#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/modification.h"
#include "../ast/equation.h"
#include "../ast/statement.h"
#include <ast/parser/parse.h>
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../ir/event.h"
#include "debug.h"
#include "error.h"
#include "macros.h"
#include "model_config.h"
#include "symbol_table.h"
#include "type.h"
#include "util.h"

using namespace MicroModelica::Util;
using namespace MicroModelica::IR;

AST_Expression AST_Expression_Traverse::apply(AST_Expression e)
{
  AST_Expression e2 = mapTraverseElement(e);
  switch (e2->expressionType()) {
  case EXPBINOP: {
    AST_Expression_BinOp b = e2->getAsBinOp();
    return (newAST_Expression_BinOp(apply(b->left()), apply(b->right()), b->binopType()));
  }
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot n = e2->getAsBooleanNot();
    return newAST_Expression_BooleanNot(apply(n->exp()));
  }
  case EXPUMINUS: {
    AST_Expression_UMinus m = e2->getAsUMinus();
    return newAST_Expression_UnaryMinus(apply(m->exp()));
  }
  case EXPOUTPUT: {
    AST_Expression_Output b = e2->getAsOutput();
    AST_ExpressionList ls = new list<AST_Expression>();
    AST_ListAppend(ls, apply(b->expressionList()->front()));
    return newAST_Expression_OutputExpressions(ls);
  }
  case EXPIF: {
    AST_Expression_If i = e2->getAsIf();
    return (newAST_Expression_If(apply(i->condition()), apply(i->then()), i->elseif_list(), apply(i->else_exp())));
  }
  case EXPCALL: {
    AST_Expression_Call c = e2->getAsCall();
    AST_ExpressionList args = newAST_ExpressionList();
    AST_ExpressionListIterator args_it;
    foreach (args_it, c->arguments()) {
      AST_ListAppend(args, apply(current_element(args_it)));
    }
    return newAST_Expression_Call(c->name(), newAST_StringNull(), args);
  }
  default:
    /* TODO */
    break;
  }
  return e2;
}

EqualExp::EqualExp() {}

bool EqualExp::equalTraverse(AST_Expression a, AST_Expression b)
{
  if (a->expressionType() != b->expressionType()) {
    return false;
  }
  switch (a->expressionType()) {
  case EXPBINOP: {
    AST_Expression_BinOp binOpA = a->getAsBinOp();
    AST_Expression_BinOp binOpB = b->getAsBinOp();
    return (equalTraverse(binOpA->left(), binOpB->left()) && equalTraverse(binOpA->right(), binOpB->right()));
  }
  default:
    return equalTraverseElement(a, b);
  }
}

bool EqualExp::_compareList(AST_ExpressionList ael, AST_ExpressionList bel)
{
  if (AST_Length(ael) != AST_Length(bel)) {
    return false;
  }
  AST_ExpressionListIterator it;
  bool ret = true;
  int c = 0;
  foreach (it, ael) {
    AST_Expression eb = AST_ListAt(bel, c++);
    ret = ret && equalTraverse(current_element(it), eb);
  }
  return ret;
}

bool EqualExp::equalTraverseElement(AST_Expression a, AST_Expression b)
{
  if (a->expressionType() != b->expressionType()) {
    return false;
  }
  switch (a->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference compRefA = a->getAsComponentReference();
    Option<Variable> varInfoA = ModelConfig::instance().lookup(CREF_NAME(compRefA));
    if (varInfoA && varInfoA->type()->getType() == TYARRAY) {
      return compareArrays(compRefA, b->getAsComponentReference());
    } else {
      return CREF_NAME(a).compare(CREF_NAME(b)) == 0;
    }
  }
  case EXPDERIVATIVE: {
    AST_ExpressionList argsA = a->getAsDerivative()->arguments();
    AST_ExpressionList argsB = b->getAsDerivative()->arguments();
    if (argsA->size() != argsB->size()) {
      return false;
    }
    ERROR_UNLESS(argsA->size() == 1,
                 "EqualExp::equalTraverseElement:\n"
                 "AST_Expression_Derivative with more than 1 argument are not supported yet.\n");
    AST_Expression_ComponentReference compRef1 = a->getAsDerivative()->arguments()->front()->getAsComponentReference();
    AST_Expression_ComponentReference compRef2 = b->getAsDerivative()->arguments()->front()->getAsComponentReference();
    return equalTraverse(compRef1, compRef2);
  }
  case EXPREAL: {
    AST_Expression_Real realA = a->getAsReal();
    AST_Expression_Real realB = b->getAsReal();
    return realA->val() == realB->val();
  }
  case EXPINTEGER: {
    AST_Expression_Integer intA = a->getAsInteger();
    AST_Expression_Integer intB = b->getAsInteger();
    return intA->val() == intB->val();
  }
  case EXPCALL: {
    string nameA = *(a->getAsCall()->name());
    string nameB = *(b->getAsCall()->name());
    if (nameA.compare(nameB)) {
      return false;
    }
    AST_ExpressionList ael = a->getAsCall()->arguments();
    AST_ExpressionList bel = b->getAsCall()->arguments();
    return _compareList(ael, bel);
  }
  case EXPCALLARG: {
    AST_ExpressionList ael = a->getAsCallArgs()->arguments();
    AST_ExpressionList bel = b->getAsCallArgs()->arguments();
    return _compareList(ael, bel);
  }
  case EXPBRACE: {
    AST_ExpressionList ael = a->getAsBrace()->arguments();
    AST_ExpressionList bel = b->getAsBrace()->arguments();
    return _compareList(ael, bel);
  }
  case EXPOUTPUT: {
    AST_ExpressionList ael = a->getAsOutput()->expressionList();
    AST_ExpressionList bel = b->getAsOutput()->expressionList();
    return _compareList(ael, bel);
  }
  case EXPUMINUS: {
    AST_Expression_UMinus m = a->getAsUMinus();
    AST_Expression_UMinus m1 = b->getAsUMinus();
    return equalTraverseElement(m->exp(), m1->exp());
  }
  default:
    ERROR(
        "EqualExp::equalTraverseElement:\n"
        "Incorrect AST_Expression_Type %s",
        a->expressionType());
  }
  return true;
}

Option<Variable> EqualExp::getVarInfo(AST_Expression_ComponentReference compRef)
{
  Option<Variable> varInfo;
  AST_StringList names = compRef->names();
  if (names->size() > 0) {
    ERROR_UNLESS(names->size() == 1,
                 "EqualExp::getVariable\n"
                 "AST_Component_Reference with names list bigger than 1 are not supported yet.\n");
    AST_String name = names->front();
    varInfo = ModelConfig::instance().lookup(*name);
  } else {
    varInfo = ModelConfig::instance().lookup(compRef->name());
  }
  return varInfo;
}

bool EqualExp::compareArrays(AST_Expression_ComponentReference arrayA, AST_Expression_ComponentReference arrayB)
{
  AST_ExpressionListList indexesListA = arrayA->indexes();
  AST_ExpressionListList indexesListB = arrayB->indexes();
  ERROR_UNLESS(indexesListA->size() == indexesListB->size(),
               "EqualExp::compareArrays:\n"
               "indexes list sizes should be equal.\n");
  ERROR_UNLESS(indexesListA->size() == 1,
               "EqualExp::compareArrays:\n"
               "Indexes list sizes greater than 1 are not supported yet.\n");
  AST_ExpressionList indexesA = indexesListA->front();
  AST_ExpressionList indexesB = indexesListB->front();
  ERROR_UNLESS(indexesA->size() == indexesB->size(),
               "EqualExp::compareArrays:\n"
               "indexes sizes should be equal.\n");
  ERROR_UNLESS(indexesA->size() == 1,
               "EqualExp::compareArrays:\n"
               "Multidimensional arrays are not supported yet.\n");
  return equalTraverse(indexesA->front(), indexesB->front());
}

bool IsConstant::foldTraverseElement(bool b1, bool b2, BinOpType) { return b1 && b2; }

bool IsConstant::foldTraverseElementUMinus(AST_Expression e) { return apply(e->getAsUMinus()->exp()); }

bool IsConstant::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPREAL:
  case EXPINTEGER:
  case EXPSTRING:
  case EXPBOOLEAN:
    return true;
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    Option<Variable> v = _st[cr->name()];
    if (v->isParameter()) {
      return true;
    }
    // Check symbol table!!!
    return false;
  }
  default:
    break;
  }
  return false;
}

AST_Expression ReplaceExp::replaceExp(AST_Expression rep, AST_Expression for_exp, AST_Expression in, VarSymbolTable symbol_table)
{
  _rep = rep;
  _for_exp = for_exp;
  _in = in;
  _symbol_table = symbol_table;
  return apply(in);
}

AST_Expression ReplaceExp::mapTraverseElement(AST_Expression e)
{
  EqualExp *equalExp = new EqualExp();
  if (equalExp->equalTraverse(e, _rep)) {
    return _for_exp;
  }
  return e;
}

ReplaceBoolean::ReplaceBoolean() {}

AST_Expression ReplaceBoolean::foldTraverseElement(AST_Expression b1, AST_Expression b2, BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression ReplaceBoolean::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression ReplaceBoolean::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPBOOLEAN: {
    AST_Expression_Boolean b = e->getAsBoolean();
    if (b->value()) {
      return newAST_Expression_Real(1.0);
    } else {
      return newAST_Expression_Real(0.0);
    }
  }
  default:
    return e;
  }
}

WhenEqualityTrasforms::WhenEqualityTrasforms() {}

AST_Expression WhenEqualityTrasforms::foldTraverseElement(AST_Expression b1, AST_Expression b2, BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression WhenEqualityTrasforms::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression WhenEqualityTrasforms::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPBOOLEAN: {
    AST_Expression_Boolean b = e->getAsBoolean();
    if (b->value()) {
      return newAST_Expression_Real(1.0);
    } else {
      return newAST_Expression_Real(0.0);
    }
    /*  case EXPUMINUS:
     AST_Expression_UMinus u = e->getAsUMinus();
     return newAST_Expression_UnaryMinus( foldTraverse(u->exp()) );
     */
  }
  case EXPOUTPUT: {
    AST_Expression_Output b = e->getAsOutput();
    return newAST_Expression_OutputExpressions(newAST_SimpleList(apply(b->expressionList()->front())));
  }
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    if (toStr(call->name()) == "edge") {
      return GREATER(call->arguments()->front(), _R(0.5));
    }
    return call;
  }
  case EXPCOMPREF:
    return e;
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot no = e->getAsBooleanNot();
    return newAST_Expression_BooleanNot(apply(no->exp()));
  }
  case EXPIF: {
    AST_Expression_If i = e->getAsIf();
    AST_Expression eq1 = apply(i->then());
    AST_Expression eq2 = apply(i->else_exp());
    AST_Expression cond = apply(i->condition());
    return newAST_Expression_If(cond, eq1, newAST_ExpressionList(), eq2);
  }
  default:
    return e;
  }
}

PreChange::PreChange(PreSet p) { _pre = p; }

AST_Expression PreChange::foldTraverseElement(AST_Expression b1, AST_Expression b2, BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression PreChange::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression PreChange::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPOUTPUT: {
    AST_Expression_Output b = e->getAsOutput();
    return (newAST_Expression_OutputExpressions(newAST_SimpleList(apply(b->expressionList()->front()))));
  }
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      current_element(it) = apply(current_element(it));
    }
    return call;
  }
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    if (_pre->find(cr->name()) != _pre->end()) {
      return (newAST_Expression_Call(_MMOC_S("pre"), nullptr, newAST_SimpleList(static_cast<AST_Expression>(cr))));
    }
    return e;
  }
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot no = e->getAsBooleanNot();
    return newAST_Expression_BooleanNot(apply(no->exp()));
  }
  default:
    return e;
  }
}

FindReference::FindReference(AST_String var) { _var = var; }

bool FindReference::foldTraverseElement(bool b1, bool b2, BinOpType t) { return b1 || b2; }

bool FindReference::foldTraverseElementUMinus(AST_Expression u) { return apply(u->getAsUMinus()->exp()); }

bool FindReference::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPOUTPUT: {
    AST_Expression_Output b = e->getAsOutput();
    return apply(b->expressionList()->front());
  }
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    AST_ExpressionListIterator it;
    bool b = false;
    foreach (it, call->arguments())
      b |= apply(current_element(it));
    return b;
  }
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    return cr->name() == toStr(_var);
  }
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot no = e->getAsBooleanNot();
    return apply(no->exp());
  }
  case EXPIF: {
    AST_Expression_If i = e->getAsIf();
    bool eq1 = apply(i->then());
    bool eq2 = apply(i->else_exp());
    bool cond = apply(i->condition());
    return eq1 || eq2 || cond;
  }
  default:
    return false;
  }
}

ReplaceReference::ReplaceReference(AST_String pre, AST_String post)
{
  _pre = pre;
  _post = post;
}

AST_Expression ReplaceReference::foldTraverseElement(AST_Expression b1, AST_Expression b2, BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression ReplaceReference::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression ReplaceReference::foldTraverseElement(AST_Expression e)
{
  switch (e->expressionType()) {
  case EXPOUTPUT: {
    AST_Expression_Output b = e->getAsOutput();
    return (newAST_Expression_OutputExpressions(newAST_SimpleList(apply(b->expressionList()->front()))));
  }
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      current_element(it) = apply(current_element(it));
    }
    return call;
  }
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    if (cr->name() == toStr(_pre)) {
      return newAST_Expression_ComponentReferenceExp(_post);
    }
    return e;
  }
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot no = e->getAsBooleanNot();
    return newAST_Expression_BooleanNot(apply(no->exp()));
  }
  default:
    return e;
  }
}
