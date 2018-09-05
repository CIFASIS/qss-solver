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
#include "../parser/parse.h"
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../ir/event.h"
#include "debug.h"
#include "error.h"
#include "macros.h"
#include "symbol_table.h"
#include "type.h"
#include "util.h"

using namespace MicroModelica::Util;
using namespace MicroModelica::IR;

AST_Expression
AST_Expression_Traverse::apply(AST_Expression e)
{
  AST_Expression e2 = mapTraverseElement(e);
  switch(e2->expressionType())
  {
    case EXPBINOP:
      {
      AST_Expression_BinOp b = e2->getAsBinOp();
      return (newAST_Expression_BinOp(apply(b->left()),
          apply(b->right()), b->binopType()));
    }
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot n = e2->getAsBooleanNot();
      return newAST_Expression_BooleanNot(apply(n->exp()));
    }
    case EXPUMINUS:
      {
      AST_Expression_UMinus m = e2->getAsUMinus();
      return newAST_Expression_UnaryMinus(apply(m->exp()));
    }
    case EXPOUTPUT:
      {
      AST_Expression_Output b = e2->getAsOutput();
      AST_ExpressionList ls = new list<AST_Expression>();
      AST_ListAppend(ls, apply(b->expressionList()->front()));
      return newAST_Expression_OutputExpressions(ls);
    }
    case EXPIF:
      {
      AST_Expression_If i = e2->getAsIf();
      return (newAST_Expression_If(apply(i->condition()),
          apply(i->then()), i->elseif_list(), apply(i->else_exp())));
    }
    case EXPCALL:
      {
      AST_Expression_Call c = e2->getAsCall();
      AST_ExpressionList args = newAST_ExpressionList();
      AST_ExpressionListIterator args_it;
      foreach(args_it,c->arguments())
      {
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

EqualExp::EqualExp(VarSymbolTable symbolTable)
{
  _symbolTable = symbolTable;
}

bool
EqualExp::equalTraverse(AST_Expression a, AST_Expression b)
{
  if(a->expressionType() != b->expressionType())
  {
    return false;
  }
  switch(a->expressionType())
  {
    case EXPBINOP:
      {
      AST_Expression_BinOp binOpA = a->getAsBinOp();
      AST_Expression_BinOp binOpB = b->getAsBinOp();
      return (equalTraverse(binOpA->left(), binOpB->left())
          && equalTraverse(binOpA->right(), binOpB->right()));
    }
    default:
      return equalTraverseElement(a, b);
  }
}

bool
EqualExp::_compareList(AST_ExpressionList ael, AST_ExpressionList bel)
{
  if(AST_Length(ael) != AST_Length(bel))
  {
    return false;
  }
  AST_ExpressionListIterator it;
  bool ret = true;
  int c = 0;
  foreach(it,ael)
  {
    AST_Expression eb = AST_ListAt(bel, c++);
    ret = ret && equalTraverse(current_element(it), eb);
  }
  return ret;
}

bool
EqualExp::equalTraverseElement(AST_Expression a, AST_Expression b)
{
  if(a->expressionType() != b->expressionType())
  {
    return false;
  }
  switch(a->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference compRefA = a->getAsComponentReference();
      Option<Variable> varInfoA = _symbolTable[CREF_NAME(compRefA)];
      if(varInfoA && varInfoA->type()->getType() == TYARRAY)
      {
        return compareArrays(compRefA, b->getAsComponentReference());
      }
      else
      {
        return CREF_NAME(a).compare(CREF_NAME(b)) == 0;
      }
    }
    case EXPDERIVATIVE:
      {
      AST_ExpressionList argsA = a->getAsDerivative()->arguments();
      AST_ExpressionList argsB = b->getAsDerivative()->arguments();
      if(argsA->size() != argsB->size())
      {
        return false;
      }
      ERROR_UNLESS(argsA->size() == 1,
          "EqualExp::equalTraverseElement:\n"
              "AST_Expression_Derivative with more than 1 argument are not supported yet.\n");
      AST_Expression_ComponentReference compRef1 =
          a->getAsDerivative()->arguments()->front()->getAsComponentReference();
      AST_Expression_ComponentReference compRef2 =
          b->getAsDerivative()->arguments()->front()->getAsComponentReference();
      return equalTraverse(compRef1, compRef2);
    }
    case EXPREAL:
      {
      AST_Expression_Real realA = a->getAsReal();
      AST_Expression_Real realB = b->getAsReal();
      return realA->val() == realB->val();
    }
    case EXPINTEGER:
      {
      AST_Expression_Integer intA = a->getAsInteger();
      AST_Expression_Integer intB = b->getAsInteger();
      return intA->val() == intB->val();
    }
    case EXPCALL:
      {
      string nameA = *(a->getAsCall()->name());
      string nameB = *(b->getAsCall()->name());
      if(nameA.compare(nameB))
      {
        return false;
      }
      AST_ExpressionList ael = a->getAsCall()->arguments();
      AST_ExpressionList bel = b->getAsCall()->arguments();
      return _compareList(ael, bel);
    }
    case EXPCALLARG:
      {
      AST_ExpressionList ael = a->getAsCallArgs()->arguments();
      AST_ExpressionList bel = b->getAsCallArgs()->arguments();
      return _compareList(ael, bel);
    }
    case EXPBRACE:
      {
      AST_ExpressionList ael = a->getAsBrace()->arguments();
      AST_ExpressionList bel = b->getAsBrace()->arguments();
      return _compareList(ael, bel);
    }
    case EXPOUTPUT:
      {
      AST_ExpressionList ael = a->getAsOutput()->expressionList();
      AST_ExpressionList bel = b->getAsOutput()->expressionList();
      return _compareList(ael, bel);
    }
    case EXPUMINUS:
      {
      AST_Expression_UMinus m = a->getAsUMinus();
      AST_Expression_UMinus m1 = b->getAsUMinus();
      return equalTraverseElement(m->exp(), m1->exp());
    }
    default:
      ERROR("EqualExp::equalTraverseElement:\n"
          "Incorrect AST_Expression_Type %s",
          a->expressionType());
  }
  return true;
}

Option<Variable>
EqualExp::getVarInfo(AST_Expression_ComponentReference compRef)
{
  Option<Variable> varInfo;
  AST_StringList names = compRef->names();
  if(names->size() > 0)
  {
    ERROR_UNLESS(names->size() == 1, "EqualExp::getVariable\n"
            "AST_Component_Reference with names list bigger than 1 are not supported yet.\n");
    AST_String name = names->front();
    varInfo = _symbolTable[*name];
  }
  else
  {
    varInfo = _symbolTable[compRef->name()];
  }
  return varInfo;
}

bool
EqualExp::compareArrays(AST_Expression_ComponentReference arrayA,
    AST_Expression_ComponentReference arrayB)
{
  AST_ExpressionListList indexesListA = arrayA->indexes();
  AST_ExpressionListList indexesListB = arrayB->indexes();
  ERROR_UNLESS(indexesListA->size() == indexesListB->size(),
      "EqualExp::compareArrays:\n"
          "indexes list sizes should be equal.\n");
  ERROR_UNLESS(indexesListA->size() == 1, "EqualExp::compareArrays:\n"
      "Indexes list sizes greater than 1 are not supported yet.\n");
  AST_ExpressionList indexesA = indexesListA->front();
  AST_ExpressionList indexesB = indexesListB->front();
  ERROR_UNLESS(indexesA->size() == indexesB->size(),
      "EqualExp::compareArrays:\n"
          "indexes sizes should be equal.\n");
  ERROR_UNLESS(indexesA->size() == 1, "EqualExp::compareArrays:\n"
      "Multidimensional arrays are not supported yet.\n");
  return equalTraverse(indexesA->front(), indexesB->front());
}

bool
IsConstant::foldTraverseElement(bool b1, bool b2, BinOpType)
{
  return b1 && b2;
}

bool
IsConstant::foldTraverseElementUMinus(AST_Expression e)
{
  return apply(e->getAsUMinus()->exp());
}

bool
IsConstant::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPREAL:
      case EXPINTEGER:
      case EXPSTRING:
      case EXPBOOLEAN:
      return true;
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      Option<Variable> v = _st[cr->name()];
      if(v->isParameter())
      {
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

AST_Expression
ReplaceExp::replaceExp(AST_Expression rep, AST_Expression for_exp,
    AST_Expression in, VarSymbolTable symbol_table)
{
  _rep = rep;
  _for_exp = for_exp;
  _in = in;
  _symbol_table = symbol_table;
  return apply(in);
}

AST_Expression
ReplaceExp::mapTraverseElement(AST_Expression e)
{
  EqualExp *equalExp = new EqualExp(_symbol_table);
  if(equalExp->equalTraverse(e, _rep))
  {
    return _for_exp;
  }
  return e;
}

ReplaceBoolean::ReplaceBoolean()
{
}

AST_Expression
ReplaceBoolean::foldTraverseElement(AST_Expression b1, AST_Expression b2,
    BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression
ReplaceBoolean::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression
ReplaceBoolean::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPBOOLEAN:
      {
      AST_Expression_Boolean b = e->getAsBoolean();
      if(b->value())
      {
        return newAST_Expression_Real(1.0);
      }
      else
      {
        return newAST_Expression_Real(0.0);
      }
    }
    default:
      return e;
  }
}

WhenEqualityTrasforms::WhenEqualityTrasforms()
{
}

AST_Expression
WhenEqualityTrasforms::foldTraverseElement(AST_Expression b1, AST_Expression b2,
    BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression
WhenEqualityTrasforms::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression
WhenEqualityTrasforms::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPBOOLEAN:
      {
      AST_Expression_Boolean b = e->getAsBoolean();
      if(b->value())
      {
        return newAST_Expression_Real(1.0);
      }
      else
      {
        return newAST_Expression_Real(0.0);
      }
      /*  case EXPUMINUS:
       AST_Expression_UMinus u = e->getAsUMinus();
       return newAST_Expression_UnaryMinus( foldTraverse(u->exp()) );
       */
    }
    case EXPOUTPUT:
      {
      AST_Expression_Output b = e->getAsOutput();
      return newAST_Expression_OutputExpressions(
          newAST_SimpleList(apply(b->expressionList()->front())));
    }
    case EXPCALL:
      {
      AST_Expression_Call call = e->getAsCall();
      if(toStr(call->name()) == "edge")
      {
        return GREATER(call->arguments()->front(), _R(0.5));
      }
      return call;
    }
    case EXPCOMPREF:
      return e;
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot no = e->getAsBooleanNot();
      return newAST_Expression_BooleanNot(apply(no->exp()));
    }
    case EXPIF:
      {
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

PreChange::PreChange(PreSet p)
{
  _pre = p;
}

AST_Expression
PreChange::foldTraverseElement(AST_Expression b1, AST_Expression b2,
    BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression
PreChange::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression
PreChange::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPOUTPUT:
      {
      AST_Expression_Output b = e->getAsOutput();
      return (newAST_Expression_OutputExpressions(
          newAST_SimpleList(apply(b->expressionList()->front()))));
    }
    case EXPCALL:
      {
      AST_Expression_Call call = e->getAsCall();
      AST_ExpressionListIterator it;
      foreach(it , call->arguments())
      {
        current_element(it) = apply(current_element(it));
      }
      return call;
    }
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      if(_pre->find(cr->name()) != _pre->end())
      {
        return (newAST_Expression_Call(_MMOC_S("pre"), NULL,
            newAST_SimpleList(static_cast<AST_Expression>(cr))));
      }
      return e;
    }
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot no = e->getAsBooleanNot();
      return newAST_Expression_BooleanNot(apply(no->exp()));
    }
    default:
      return e;
  }
}

FindReference::FindReference(AST_String var)
{
  _var = var;
}

bool
FindReference::foldTraverseElement(bool b1, bool b2, BinOpType t)
{
  return b1 || b2;
}

bool
FindReference::foldTraverseElementUMinus(AST_Expression u)
{
  return apply(u->getAsUMinus()->exp());
}

bool
FindReference::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPOUTPUT:
      {
      AST_Expression_Output b = e->getAsOutput();
      return apply(b->expressionList()->front());
    }
    case EXPCALL:
      {
      AST_Expression_Call call = e->getAsCall();
      AST_ExpressionListIterator it;
      bool b = false;
      foreach(it , call->arguments())
        b |= apply(current_element(it));
      return b;
    }
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      return cr->name() == toStr(_var);
    }
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot no = e->getAsBooleanNot();
      return apply(no->exp());
    }
    case EXPIF:
      {
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

AST_Expression
ReplaceReference::foldTraverseElement(AST_Expression b1, AST_Expression b2,
    BinOpType t)
{
  return newAST_Expression_BinOp(b1, b2, t);
}

AST_Expression
ReplaceReference::foldTraverseElementUMinus(AST_Expression u)
{
  return newAST_Expression_UnaryMinus(apply(u->getAsUMinus()->exp()));
}

AST_Expression
ReplaceReference::foldTraverseElement(AST_Expression e)
{
  switch(e->expressionType())
  {
    case EXPOUTPUT:
      {
      AST_Expression_Output b = e->getAsOutput();
      return (newAST_Expression_OutputExpressions(
          newAST_SimpleList(apply(b->expressionList()->front()))));
    }
    case EXPCALL:
      {
      AST_Expression_Call call = e->getAsCall();
      AST_ExpressionListIterator it;
      foreach(it , call->arguments())
      {
        current_element(it) = apply(current_element(it));
      }
      return call;
    }
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      if(cr->name() == toStr(_pre))
      {
        return newAST_Expression_ComponentReferenceExp(_post);
      }
      return e;
    }
    case EXPBOOLEANNOT:
      {
      AST_Expression_BooleanNot no = e->getAsBooleanNot();
      return newAST_Expression_BooleanNot(apply(no->exp()));
    }
    default:
      return e;
  }
}


/* ReplaceDer Class */

ReplaceDer::ReplaceDer(VarSymbolTable vt) :
    _vt(vt)
{
}

ReplaceDer::~ReplaceDer()
{
}

AST_Expression
ReplaceDer::foldTraverseElement(AST_Expression exp)
{
  stringstream ret(stringstream::out);
  switch(exp->expressionType())
  {
    case EXPCALL:
    {
      AST_Expression_Call ec = exp->getAsCall();
      string name = *(ec->name());
      if(!name.compare("der2"))
      {
        return (newAST_Expression_Derivative(
                newAST_ExpressionList(
                newAST_Expression_Derivative(ec->arguments()))));
      }
      else if(!name.compare("der3"))
      {
        return (newAST_Expression_Derivative(
                newAST_ExpressionList(
                newAST_Expression_Derivative(
                newAST_ExpressionList(
                        newAST_Expression_Derivative(ec->arguments()))))));
      }
    }
      break;
    case EXPOUTPUT:
      return (newAST_Expression_OutputExpressions(
              newAST_ExpressionList(apply(AST_ListFirst(exp->getAsOutput()->expressionList())))));
    default:
      break;
  }
  return exp;
}

AST_Expression
ReplaceDer::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression
ReplaceDer::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

/* Eval Expression class. */

EvalExp::EvalExp(VarSymbolTable symbolTable)
{
  _compRef = NULL;
  _compRefVal = NULL;
  _symbolTable = symbolTable;
}

AST_Expression
EvalExp::eval(AST_Expression exp)
{
  return apply(exp);
}

AST_Expression
EvalExp::eval(AST_Expression_ComponentReference compRef,
    AST_Expression compRefValue, AST_Expression exp)
{
  _compRef = compRef;
  _compRefVal = compRefValue;
  return eval(exp);
}

AST_Expression
EvalExp::foldTraverseElement(AST_Expression exp)
{
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference expCompRef = exp->getAsComponentReference();
      Option<Variable> varInfo = _symbolTable[CREF_NAME(expCompRef)];
      if(varInfo && varInfo->type()->getType() == TYARRAY)
      {
        return evalArray(expCompRef);
      }
      else
      {
        return evalCompRef(expCompRef);
      }
    }
    case EXPDERIVATIVE:
      {
      AST_Expression_Derivative der = exp->getAsDerivative();
      AST_ExpressionList arguments = der->arguments();
      AST_ExpressionListIterator argsIter;
      AST_ExpressionList newArgs = newAST_ExpressionList();
      foreach(argsIter, arguments)
      {
        AST_Expression arg = current_element(argsIter);
        ERROR_UNLESS(arg->expressionType() == EXPCOMPREF,
            "InstantiationTraverse::mapTraverseElement:\n"
                "Incorrect AST_ExpressionDerivative argument type or not supported yet.\n");
        AST_Expression newArg = evalArray(
            (AST_Expression_ComponentReference) arg);
        newArgs->push_back(newArg);
      }
      return newAST_Expression_Derivative(newArgs);
    }
    default:
      return exp;
  }
}

AST_Expression
EvalExp::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression_Integer zero =
      (AST_Expression_Integer) newAST_Expression_Integer(0);
  return (foldTraverseElement(zero, apply(exp->getAsUMinus()->exp()),
      BINOPSUB));
}

AST_Expression
EvalExp::foldTraverseElement(AST_Expression left, AST_Expression right,
    BinOpType binOpType)
{
  switch(binOpType)
  {
    case BINOPADD:
      if(shouldReturnInteger(left, right))
      {
        return (newAST_Expression_Integer(
            left->getAsInteger()->val() + right->getAsInteger()->val()));
      }
      else if(shouldReturnReal(left, right))
      {
        return newAST_Expression_Real(getRealVal(left) + getRealVal(right));
      }
      else
      {
        return newAST_Expression_BinOp(left, right, BINOPADD);
      }
    case BINOPSUB:
      if(shouldReturnInteger(left, right))
      {
        return (newAST_Expression_Integer(
            left->getAsInteger()->val() - right->getAsInteger()->val()));
      }
      else if(shouldReturnReal(left, right))
      {
        return newAST_Expression_Real(getRealVal(left) - getRealVal(right));
      }
      else
      {
        return newAST_Expression_BinOp(left, right, BINOPSUB);
      }
    case BINOPMULT:
      if(shouldReturnInteger(left, right))
      {
        return (newAST_Expression_Integer(
            left->getAsInteger()->val() * right->getAsInteger()->val()));
      }
      else if(shouldReturnReal(left, right))
      {
        return newAST_Expression_Real(getRealVal(left) * getRealVal(right));
      }
      else
      {
        return newAST_Expression_BinOp(left, right, BINOPMULT);
      }
    case BINOPDIV:
      ERROR_UNLESS(right != 0, "process_for_equations - evalExp:\n"
          "Division by zero.\n");
      if(shouldReturnInteger(left, right))
      {
        return (newAST_Expression_Integer(
            left->getAsInteger()->val() / right->getAsInteger()->val()));
      }
      else if(shouldReturnReal(left, right))
      {
        return newAST_Expression_Real(getRealVal(left) / getRealVal(right));
      }
      else
      {
        return newAST_Expression_BinOp(left, right, BINOPDIV);
      }
    case BINOPEXP:
      if(shouldReturnInteger(left, right))
      {
        return (newAST_Expression_Integer(
            pow(left->getAsInteger()->val(), right->getAsInteger()->val())));
      }
      else if(shouldReturnReal(left, right))
      {
        return newAST_Expression_Real(pow(getRealVal(left), getRealVal(right)));
      }
      else
      {
        return newAST_Expression_BinOp(left, right, BINOPEXP);
      }
    default:
      ERROR("process_for_equations.cpp - evalBinOp:\n"
          "Incorrect Binary operation type.\n");
  }
  ERROR("process_for_equations.cpp - evalBinOp:\n"
      "Incorrect Binary operation type.\n");
  return NULL;
}

AST_Expression
EvalExp::evalCompRef(AST_Expression_ComponentReference compRef)
{
  EqualExp eqExp(_symbolTable);
  if(_compRef && eqExp.equalTraverse(compRef, _compRef))
  {
    return _compRefVal;
  }
  Option<Variable> varInfo = _symbolTable[CREF_NAME(compRef)];
  if(varInfo && (varInfo->isParameter() || varInfo->isConstant()))
  {
    AST_Modification mod = varInfo->modification();
    switch(mod->modificationType())
    {
      case MODEQUAL:
        {
        AST_Modification_Equal equal = mod->getAsEqual();
        return apply(equal->exp());
      }
      default:
        ERROR("RangeIterator::getVal\n"
            "Incorrect AST_Modification type or not supported yet.\n");
    }
  }
  return compRef;
}

// The component reference is an array of the form
// a[i,j].b[x].c[y] (indexesList is ( (i,j) , (x), (y) ).
AST_Expression
EvalExp::evalArray(AST_Expression_ComponentReference array)
{
  AST_Expression_ComponentReference newArray =
      newAST_Expression_ComponentReference();
  AST_StringList names = array->names();
  AST_StringListIterator namesIter = names->begin();
  AST_ExpressionListList indexesList = array->indexes();
  AST_ExpressionListListIterator indexesListIter;
  foreach (indexesListIter, indexesList)
  {
    AST_ExpressionListIterator listIter;
    AST_ExpressionList indexes = current_element(indexesListIter);
    AST_ExpressionList newIndexes = newAST_ExpressionList();
    foreach (listIter, indexes)
    {
      AST_Expression arrayIndex = current_element(listIter);
      AST_Expression newArrayIndex = apply(arrayIndex);
      newIndexes->push_back(newArrayIndex);
    }
    newArray->append((AST_String) current_element(namesIter), newIndexes);
    namesIter++;
  }
  return newArray;
}

bool
EvalExp::shouldReturnInteger(AST_Expression left, AST_Expression right)
{
  return (left->expressionType() == EXPINTEGER
      && right->expressionType() == EXPINTEGER);
}

bool
EvalExp::shouldReturnReal(AST_Expression left, AST_Expression right)
{
  return ((left->expressionType() == EXPREAL
      && right->expressionType() == EXPINTEGER)
      || (left->expressionType() == EXPINTEGER
          && right->expressionType() == EXPREAL)
      || (left->expressionType() == EXPREAL
          && right->expressionType() == EXPREAL));
}

AST_Real
EvalExp::getRealVal(AST_Expression exp)
{
  if(exp->expressionType() == EXPREAL)
  {
    return exp->getAsReal()->val();
  }
  else
  {
    return exp->getAsInteger()->val();
  }
}


/* ReplaceInnerProduct Class */

ReplaceInnerProduct::ReplaceInnerProduct(VarSymbolTable& symbols) :
    _symbols(symbols)
{
}

AST_Expression
ReplaceInnerProduct::foldTraverseElement(AST_Expression exp)
{
  switch(exp->expressionType())
  {
    case EXPCALL:
      {
      string name = *(exp->getAsCall()->name());
      AST_ExpressionList nel = newAST_ExpressionList();
      AST_ExpressionList el = exp->getAsCall()->arguments();
      AST_ExpressionListIterator it;
      foreach(it,el)
      {
        AST_ListAppend(nel, apply(current_element(it)));
      }
      AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL, nel);
      return ret;
    }
    case EXPCALLARG:
      {
      AST_ExpressionList el = exp->getAsCallArgs()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, apply(current_element(it)));
      }
      return newAST_Expression_FunctionCallArgs(nel);
    }
    case EXPBRACE:
      {
      AST_ExpressionList el = exp->getAsBrace()->arguments();
      AST_ExpressionListIterator it;
      AST_ExpressionList nel = newAST_ExpressionList();
      foreach(it,el)
      {
        AST_ListAppend(nel, apply(current_element(it)));
      }
      return newAST_Expression_Brace(nel);
    }
    case EXPOUTPUT:
      {
      AST_ExpressionList el = exp->getAsOutput()->expressionList();
      AST_ExpressionListIterator eli;
      AST_ExpressionList ret = newAST_ExpressionList();
      foreach(eli,el)
      {
        AST_ListAppend(ret, apply(current_element(eli)));
      }
      return newAST_Expression_OutputExpressions(ret);
    }
    default:
      return exp;
  }
  return NULL;
}

AST_Expression
ReplaceInnerProduct::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  if(bot == BINOPMULT)
  {
    if(controlArray(l) && controlArray(r))
    {
      string name = "__INNER_PRODUCT";
      AST_ExpressionList nel = newAST_ExpressionList();
      AST_ListAppend(nel, l);
      AST_ListAppend(nel, r);
      AST_Expression ret = newAST_Expression_Call(newAST_String(name), NULL, nel);
      return ret;
    }
  }
  return newAST_Expression_BinOp(l, r, bot);
}

AST_Expression
ReplaceInnerProduct::foldTraverseElementUMinus(AST_Expression exp)
{
  return exp;
}

bool
ReplaceInnerProduct::controlArray(AST_Expression exp)
{
  if(exp->expressionType() == EXPCOMPREF)
  {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> vi = _symbols[cr->name()];
    if(!vi)
    {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "Control Array: %s", cr->name().c_str());
    }
    if(!cr->hasIndexes() && vi->isArray())
    {
      return true;
    }
  }
  return false;
}
/* EvalInitExp class */

EvalInitExp::EvalInitExp(VarSymbolTable vt) :
    _vt(vt)
{
}

EvalInitExp::~EvalInitExp()
{
}

int
EvalInitExp::foldTraverseElement(AST_Expression exp)
{
  int ret = 0;
  switch(exp->expressionType())
  {
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      Option<Variable> vi = _vt[cr->name()];
      if(!vi)
      {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
        return ret;
      }
      if(!vi->isConstant())
      {
        Error::instance().add(exp->lineNum(), EM_IR | EM_INIT_EXP, ER_Error, "Only constants allowed inside initial expressions. %s", cr->name().c_str());
        return ret;
      }
      return vi->value();
    }
    case EXPBOOLEAN:
      {
      AST_Expression_Boolean eb = exp->getAsBoolean();
      if(eb->value())
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
    case EXPBOOLEANNOT:
    {
      AST_Expression_BooleanNot ebn = exp->getAsBooleanNot();
      int res = apply(ebn->exp());
      if(res == 0)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }
    case EXPREAL:
      Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Implicit conversion from Real to Integer, in initial expression.");
      return exp->getAsReal()->val();
    case EXPINTEGER:
      return exp->getAsInteger()->val();
    default:
      Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression not recognized, returning zero as default value.");
      break;
  }
  return ret;
}

int
EvalInitExp::foldTraverseElementUMinus(AST_Expression exp)
{
  return -1 * apply(exp->getAsUMinus()->exp());
}

int
EvalInitExp::foldTraverseElement(int l, int r, BinOpType bot)
{
  switch(bot)
  {
    case BINOPOR:
      return ((l != 0 || r != 0) ? 1 : 0);
    case BINOPAND:
      return ((l != 0 && r != 0) ? 1 : 0); 
    case BINOPLOWER:
      return (l < r ? 1 : 0);
    case BINOPLOWEREQ:
      return (l <= r ? 1 : 0);
    case BINOPGREATER:
      return (l > r ? 1 : 0);
    case BINOPGREATEREQ:
      return (l >= r ? 1 : 0);
    case BINOPCOMPNE:
      return (l != r ? 1 : 0);
    case BINOPCOMPEQ:
      return (l == r ? 1 : 0);
    case BINOPADD:
      return l + r;
    case BINOPSUB:
      return l - r;
    case BINOPDIV:
      if(r != 0)
      {
        return l / r;
      }
      else
      {
        Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression zero division, returning zero as default value.");
      }
      break;
    case BINOPMULT:
      return l * r;
    default:
      break;
  }
  return 0;
}

/* VariableLookup class */

VariableLookup::VariableLookup(VarSymbolTable st, VarSymbolTable lst) :
    _st(st), 
    _lst(lst)
{
}

VariableLookup::~VariableLookup()
{
}

bool
VariableLookup::foldTraverseElement(AST_Expression e)
{
  if(e->expressionType() == EXPCOMPREF)
  {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    Option<Variable> vi = _st[cr->name()];
    if(!vi)
    {
      vi = _lst[cr->name()];
      if(!vi)
      {
        return false;
      }
    }
  }
  return true;
}

bool
VariableLookup::foldTraverseElement(bool e1, bool e2, BinOpType bot)
{
  return e1 && e2;
}

bool
VariableLookup::foldTraverseElementUMinus(AST_Expression e)
{
  return foldTraverseElement(e);
}

/* ConvertExpression Class */

ConvertExpression::ConvertExpression(AST_Expression left, AST_Expression right, VarSymbolTable& symbols) :
  _left(left),
  _right(right),
  _symbols(symbols),
  _convert()
{
  convert();
}

AST_Expression_ComponentReference 
ConvertExpression::componentReference(AST_Expression exp)
{
  if(exp->expressionType() == EXPDERIVATIVE)
  {
    AST_Expression_Derivative ed = exp->getAsDerivative();
    return AST_ListFirst(ed->arguments())->getAsComponentReference();
  }
  else if(exp->expressionType() == EXPCOMPREF)
  {
    return exp->getAsComponentReference();
  }
  return NULL;
}

Variable 
ConvertExpression::variable(AST_Expression_ComponentReference exp)
{  
  Option<Variable> var = _symbols[exp->name()];
  if(!var)
  {
    Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "%s", exp->name().c_str());
  }
  return var.get();
}

bool 
ConvertExpression::scalarExpression(AST_Expression exp)
{
  return (exp->expressionType() == EXPREAL || exp->expressionType() == EXPINTEGER);
}

double 
ConvertExpression::scalarValue(AST_Expression exp)
{
  if(exp->expressionType() == EXPINTEGER)
  {
    return exp->getAsInteger()->val();
  }
  else if(exp->expressionType() == EXPREAL)
  {
    return exp->getAsReal()->val();
  }
  return 0;
}

void 
ConvertExpression::convert()
{
  AST_Expression_ComponentReference eleft = componentReference(_left);
  if(eleft == NULL)
  {
    return;
  }
  string prefix = "";
  string postfix = "";
  if(_left->expressionType() == EXPDERIVATIVE)
  {
    prefix = "der(";
    postfix = ")";
  }
  if(_right->expressionType() == EXPBINOP)
  {
    AST_Expression_BinOp bo = _right->getAsBinOp();
    BinOpType type = bo->binopType();
    if(type == BINOPELADD || type == BINOPELSUB || type == BINOPELDIV 
        || type == BINOPELMULT || type == BINOPELEXP)
    {
      AST_Expression left = bo->left();
      AST_Expression right = bo->right();
      if(right->expressionType() == EXPCOMPREF && left->expressionType() == EXPCOMPREF)
      {
        AST_Expression_ComponentReference cleft = left->getAsComponentReference();
        AST_Expression_ComponentReference cright = right->getAsComponentReference();
        if(!cleft->hasIndexes() && !cright->hasIndexes())
        {
          Variable lvi = variable(cleft);
          Variable rvi = variable(cright);
          if(rvi.size() != lvi.size() && rvi.isArray() && lvi.isArray())
          {
            Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                ER_Fatal, "Different array size in element-wise operation");
          }
          Variable elvi = variable(eleft);
          if(lvi.isArray() && rvi.isArray())
          {
            if(rvi.size() != elvi.size())
            {
              Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                  ER_Fatal, "Different array size in element-wise operation");
            }
            Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
            _symbols["i"] = vi;
            stringstream buffer;
            buffer << "for i in 1:" << elvi.size() << " loop" << endl;
            buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                << cleft->name() << "[i] "
                << Utils::instance().opString(type);
            buffer << cright->name() << "[i];" << endl;
            buffer << "end for" << endl;
            _convert = buffer.str();
          }
          else if(lvi.size() == 1 && rvi.size() > 1)
          {
            if(rvi.size() != elvi.size())
            {
              Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                  ER_Fatal, "Different array size in element-wise operation");
            }
            Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
            _symbols["i"] = vi;
            stringstream buffer;
            buffer << "for i in 1:" << elvi.size() << " loop" << endl;
            buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                << cleft->name()
                << Utils::instance().opString(type);
            buffer << cright->name() << "[i];" << endl;
            buffer << "end for" << endl;
            _convert = buffer.str();
          }
          else if(lvi.size() > 1 && rvi.size() == 1)
          {
            if(lvi.size() != elvi.size())
            {
              Error::instance().add(cleft->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                  ER_Error, "Different array size in element-wise operation");
            }
            Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
            _symbols["i"] = vi;
            stringstream buffer;
            buffer << "for i in 1:" << elvi.size() << " loop" << endl;
            buffer << prefix << eleft->name() << "[i]" << postfix << " = "
                << cleft->name() << "[i]"
                << Utils::instance().opString(type);
            buffer << cright->name() << ";" << endl;
            buffer << "end for" << endl;
            _convert = buffer.str();
          }
        }
      }
      else if(left->expressionType() == EXPCOMPREF && scalarExpression(right))
      {
        AST_Expression_ComponentReference cleft = left->getAsComponentReference();
        if(!cleft->hasIndexes())
        {
          Variable lvi = variable(cleft);
          Variable elvi = variable(eleft);
          if(lvi.size() != elvi.size())
          {
            Error::instance().add(cleft->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                ER_Error, "Different array size in element-wise operation");
          }
          Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
          _symbols["i"] = vi;
          stringstream buffer;
          buffer << "for i in 1:" << elvi.size() << " loop" << endl;
          buffer << prefix << eleft->name() << "[i]" << postfix << " = "
              << cleft->name() << "[i]"
              << Utils::instance().opString(type);
          buffer << scalarValue(right) << ";" << endl;
          buffer << "end for" << endl;
          _convert = buffer.str();
        }
      }
      else if(right->expressionType() == EXPCOMPREF && scalarExpression(left))
      {
        AST_Expression_ComponentReference cright = right->getAsComponentReference();
        if(!cright->hasIndexes())
        {
          Variable lvi = variable(cright);
          Variable elvi = variable(eleft);
          if(lvi.size() != elvi.size())
          {
            Error::instance().add(cright->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND,
                ER_Error, "Different array size in element-wise operation");
          }
          Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
          _symbols["i"] = vi;
          stringstream buffer;
          buffer << "for i in 1:" << elvi.size() << " loop" << endl;
          buffer << prefix << eleft->name() << "[i]" << postfix << " = "
              << scalarValue(left)
              << Utils::instance().opString(type);
          buffer << cright->name() << "[i]" << ";" << endl;
          buffer << "end for" << endl;
          _convert = buffer.str();
        }
      }
    }
  }
}

/* ConvertEquation Class */

ConvertEquation::ConvertEquation(AST_Equation equation, VarSymbolTable& symbols) :
  _symbols(symbols)
{
  _equation = convert(equation);
}

AST_Equation 
ConvertEquation::convert(AST_Equation eq)
{
  if(eq->equationType() == EQEQUALITY)
  {
    ReplaceInnerProduct rip(_symbols);
    AST_Expression l = eq->getAsEquality()->left();
    AST_Expression r = rip.apply(eq->getAsEquality()->right());
    string transform = ConvertExpression(l, r,_symbols).get();
    if(transform.empty())
    {
      return newAST_Equation_Equality(l, r);
    }
    int rValue;
    return parseEquation(transform, &rValue);
  }
  else if(eq->equationType() == EQFOR)
  {
    AST_Equation_For eqf = eq->getAsFor();
    AST_ForIndexList fil = eqf->forIndexList();
    AST_EquationList eqs = eqf->equationList();
    AST_EquationListIterator it;
    AST_EquationList tel = newAST_EquationList();
    foreach(it,eqs)
    {
      AST_ListAppend(tel, convert(current_element(it)));
    }
    return newAST_Equation_For(fil, tel);
  }
  return eq;
}

/* ConvertStatement Class */

ConvertStatement::ConvertStatement(AST_Statement statement, VarSymbolTable& symbols) :
  _symbols(symbols)
{
  _statement = convert(statement);
}

AST_StatementList 
ConvertStatement::convert(AST_StatementList sts)
{
  AST_StatementList stt = newAST_StatementList();
  AST_StatementListIterator stit;
  foreach(stit,sts)
  {
    AST_ListAppend(stt, convert(current_element(stit)));
  }
  return stt;
}

AST_Statement_ElseList 
ConvertStatement::convert(AST_Statement_ElseList stel)
{
  AST_Statement_ElseList stelt = newAST_Statement_ElseList();
  AST_Statement_ElseListIterator stelit;
  foreach(stelit,stel)
  {
    AST_Statement_Else ceif = current_element(stelit);
    AST_StatementList newElseIfSts = newAST_StatementList();
    AST_StatementList elseIfSts = ceif->statements();
    AST_StatementListIterator elseIfStIt;
    foreach(elseIfStIt,elseIfSts)
    {
      AST_ListAppend(newElseIfSts, convert(current_element(elseIfStIt)));
    }
    AST_Statement_Else newStElse = newAST_Statement_Else(ceif->condition(), newElseIfSts);
    AST_ListAppend(stelt, newStElse);
  }
  return stelt;
}

AST_Statement
ConvertStatement::convert(AST_Statement st)
{
  if(st->statementType() == STASSING)
  {
    ReplaceInnerProduct rip(_symbols);
    AST_Expression l = st->getAsAssign()->lhs();
    AST_Expression r = rip.apply(st->getAsAssign()->exp());
    string transform = ConvertExpression(l, r,_symbols).get();
    if(transform.empty())
    {
      return newAST_Statement_Assign(st->getAsAssign()->lhs(), r);
    }
    int rValue;
    return parseStatement(transform, &rValue);
  }
  else if(st->statementType() == STFOR)
  {
    AST_Statement_For stf = st->getAsFor();
    AST_ForIndexList fil = stf->forIndexList();
    AST_StatementList sts = stf->statements();
    AST_StatementList stt = convert(sts);
    AST_Statement retFor = newAST_Statement_For(fil, stt);
    return retFor;
  }
  else if(st->statementType() == STIF)
  {
    AST_Statement_If sti = st->getAsIf();
    AST_StatementList stt = convert(sti->statements());
    AST_StatementList stet = convert(sti->else_statements());
    AST_Statement_ElseList stel = sti->else_if();
    AST_Statement_ElseList stelt = convert(stel);
    AST_Statement retIf = newAST_Statement_If(sti->condition(), stt, stelt,
        stet);
    return retIf;
  }
  else if(st->statementType() == STWHEN)
  {
    AST_Statement_When stWhen = st->getAsWhen();
    AST_StatementList stList = convert(stWhen->statements());
    AST_Statement_ElseList stElseList = convert(stWhen->else_when());
    AST_Statement retWhen = newAST_Statement_When(stWhen->condition(), stList, stElseList, stWhen->comment());
    return retWhen;
  }
  return st;
}


/* ConvertCondition class */

ConvertCondition::ConvertCondition() :
    _zc(EVENT::Zero), 
    _zcRelation(EVENT::GT)
{
}

AST_Expression
ConvertCondition::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  switch(bot)
  {
    case BINOPLOWER:
    {
      AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
      _zc = EVENT::Negative;
      _zcRelation = EVENT::LT;
      return bo;
    }
    case BINOPLOWEREQ:
    {
      AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
      _zc = EVENT::Negative;
      _zcRelation = EVENT::LE;
      return bo;
    }
    case BINOPGREATER:
      {
      AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
      _zc = EVENT::Positive;
      _zcRelation = EVENT::GT;
      return bo;
    }
    case BINOPGREATEREQ:
      {
      AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
      _zc = EVENT::Positive;
      _zcRelation = EVENT::GE;
      return bo;
    }
    default:
      break;
  }
  AST_Expression bo = newAST_Expression_BinOp(l, r, bot);
  return bo;
}


bool 
Autonomous::foldTraverseElement(AST_Expression e)
{
  bool autonomous = true;
  switch(e->expressionType())
  {
    case EXPCALL:
    {
      AST_Expression_Call call = e->getAsCall();
      AST_ExpressionListIterator it;
      foreach(it , call->arguments())
      {
        autonomous &= apply(current_element(it));
      }
      return autonomous;
    }
    case EXPCOMPREF:
    {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      Option<Variable> var = _symbols[cr->name()];
      if(var && var->isTime()) { autonomous = false; }
    }
    default:
      return autonomous;
  }
}

SymbolTable 
CalledFunctions::foldTraverseElement(AST_Expression exp)
{
  SymbolTable symbols;
  switch(exp->expressionType())
  {
    case EXPCALL:
    {
      AST_Expression_Call call = exp->getAsCall();
      symbols.insert(*call->name(), *call->name());
      return symbols;
    }
    default:
      return symbols;
  }
}

SymbolTable 
CalledFunctions::foldTraverseElement(SymbolTable l, SymbolTable r, BinOpType bot)
{
  SymbolTable::iterator it;
  for(string i = l.begin(it); !l.end(it); i = l.next(it))
  {
    r.insert(i,i);
  }
  return r;
}

SymbolTable  
StatementCalledFunctions::foldTraverse(SymbolTable s1, SymbolTable s2)
{
  SymbolTable::iterator it;
  for(string i = s1.begin(it); !s1.end(it); i = s1.next(it))
  {
    s2.insert(i,i);
  }
  return s2;
}

ExpressionPrinter::ExpressionPrinter(const VarSymbolTable& symbols) :
  _symbols(symbols)
{
}

string 
ExpressionPrinter::foldTraverseElement(AST_Expression exp)
{
  stringstream buffer;
  switch(exp->expressionType())
  {
    case EXPBOOLEAN:
      buffer << (exp->getAsBoolean()->value() ? 1 : 0); break;
    case EXPBOOLEANNOT:
      buffer << "!" << apply(exp->getAsBooleanNot()->exp()); break;
    case EXPBRACE: break;
    case EXPCALL:
    {
      AST_Expression_Call call = exp->getAsCall();
      buffer << call->name() << "(";
      AST_ExpressionListIterator it;
      int size = call->arguments()->size(), i = 0;
      foreach(it,call->arguments())
      {
        i++;
        buffer << apply(current_element(it));
        buffer << (i < size ? "," : "");
      }
      buffer << ")";
      break;
    }
    case EXPCALLARG: break;
    case EXPCOMPREF:
    {
      AST_Expression_ComponentReference cr = exp->getAsComponentReference();
      Option<Variable> var = _symbols[cr->name()];
      if(!var)
      {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "%s", cr->name().c_str());
        break;
      }
      buffer << var.get();
      if(cr->hasIndexes())
      {
        AST_ExpressionList indexes = cr->firstIndex();
        AST_ExpressionListIterator it;
        int size = indexes->size(), i = 0;
        foreach(it, indexes)
        {
          buffer << "(";
          buffer << apply(current_element(it)) << (++i < size ? "," : "");
          buffer << ")";
        }
      }
      break;
    }
    case EXPDERIVATIVE:
    {
      AST_Expression_Derivative der = exp->getAsDerivative();
      buffer << apply(AST_ListFirst(der->arguments()));
      break;
    }
    case EXPINTEGER:
      buffer << exp->getAsInteger()->val();
      break;
    case EXPOUTPUT:
    {
      AST_Expression_Output out = exp->getAsOutput();
      AST_ExpressionListIterator it;
      int size = out->expressionList()->size(), i = 0;
      foreach (it,out->expressionList())
      {
        buffer << apply(current_element(it));
        buffer << (++i == size ? "" : ",");
      }
      break;
    }
    case EXPREAL:
      buffer << exp->getAsReal()->val();
      break;
    case EXPSTRING:
      buffer << exp->getAsString()->str();
      break;
    default:
      return "";
  }
  return buffer.str();
}

string 
ExpressionPrinter::foldTraverseElement(string l, string r, BinOpType bot)
{
  stringstream buffer;
  switch(bot)
  {
    case BINOPOR: buffer << l << "||" << r; break;
    case BINOPAND: buffer << l << "&&" << r; break;
    case BINOPLOWER: buffer << l << "<" << r; break; 
    case BINOPLOWEREQ: buffer << l << "<=" << r; break;
    case BINOPGREATER: buffer << l << ">" << r; break; 
    case BINOPGREATEREQ: buffer << l << ">=" << r; break;
    case BINOPCOMPNE: buffer << l << "!=" << r; break;
    case BINOPCOMPEQ: buffer << l << "==" << r; break; 
    case BINOPADD: buffer << l << "+" << r; break; 
    case BINOPELADD: buffer << l << "IMPLEMENT" << r; break;
    case BINOPSUB: buffer << l << "-" << r; break;  
    case BINOPELSUB: buffer << l << "IMPLEMENT" << r; break; 
    case BINOPDIV: buffer << l << "/" << r; break;  
    case BINOPELDIV: buffer << l << "IMPLEMENT" << r; break; 
    case BINOPMULT: buffer << l << "*" << r; break;  
    case BINOPELMULT: buffer << l << "IMPLEMENT" << r; break; 
    case BINOPEXP: buffer << l << "^" << r; break;     
    case BINOPELEXP: buffer << l << "IMPLEMENT" << r; break;    
    default:
      return "NOT IMPLEMENTED";
  }
  return buffer.str();
}
 
string 
ExpressionPrinter::foldTraverseElementUMinus(AST_Expression exp)
{
  stringstream buffer;
  buffer << "-" << apply(exp->getAsUMinus()->exp());
  return buffer.str();
}


