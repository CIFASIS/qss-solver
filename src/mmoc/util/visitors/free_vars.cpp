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

#include "eval_exp.h"

#include <math.h>

#include "../../ast/ast_builder.h"
#include "../debug.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

/* Eval Expression class. */

EvalExp::EvalExp(VarSymbolTable symbolTable)
{
  _compRef = nullptr;
  _compRefVal = nullptr;
  _symbolTable = symbolTable;
}

AST_Expression EvalExp::eval(AST_Expression exp) { return apply(exp); }

AST_Expression EvalExp::eval(AST_Expression_ComponentReference compRef, AST_Expression compRefValue, AST_Expression exp)
{
  _compRef = compRef;
  _compRefVal = compRefValue;
  return eval(exp);
}

AST_Expression EvalExp::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference expCompRef = exp->getAsComponentReference();
    Option<Variable> varInfo = _symbolTable[CREF_NAME(expCompRef)];
    if (varInfo && varInfo->type()->getType() == TYARRAY) {
      return evalArray(expCompRef);
    } else {
      return evalCompRef(expCompRef);
    }
  }
  case EXPDERIVATIVE: {
    AST_Expression_Derivative der = exp->getAsDerivative();
    AST_ExpressionList arguments = der->arguments();
    AST_ExpressionListIterator argsIter;
    AST_ExpressionList newArgs = newAST_ExpressionList();
    foreach (argsIter, arguments) {
      AST_Expression arg = current_element(argsIter);
      Util::ERROR_UNLESS(arg->expressionType() == EXPCOMPREF,
                         "InstantiationTraverse::mapTraverseElement:\n"
                         "Incorrect AST_ExpressionDerivative argument type or not supported yet.\n");
      AST_Expression newArg = evalArray((AST_Expression_ComponentReference)arg);
      newArgs->push_back(newArg);
    }
    return newAST_Expression_Derivative(newArgs);
  }
  default:
    return exp;
  }
}

AST_Expression EvalExp::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression_Integer zero = (AST_Expression_Integer)newAST_Expression_Integer(0);
  return (foldTraverseElement(zero, apply(exp->getAsUMinus()->exp()), BINOPSUB));
}

AST_Expression EvalExp::foldTraverseElement(AST_Expression left, AST_Expression right, BinOpType binOpType)
{
  switch (binOpType) {
  case BINOPADD:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(left->getAsInteger()->val() + right->getAsInteger()->val()));
    } else if (shouldReturnReal(left, right)) {
      return newAST_Expression_Real(getRealVal(left) + getRealVal(right));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPADD);
    }
  case BINOPSUB:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(left->getAsInteger()->val() - right->getAsInteger()->val()));
    } else if (shouldReturnReal(left, right)) {
      return newAST_Expression_Real(getRealVal(left) - getRealVal(right));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPSUB);
    }
  case BINOPMULT:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(left->getAsInteger()->val() * right->getAsInteger()->val()));
    } else if (shouldReturnReal(left, right)) {
      return newAST_Expression_Real(getRealVal(left) * getRealVal(right));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPMULT);
    }
  case BINOPDIV:
    Util::ERROR_UNLESS(right != 0,
                       "process_for_equations - evalExp:\n"
                       "Division by zero.\n");
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(left->getAsInteger()->val() / right->getAsInteger()->val()));
    } else if (shouldReturnReal(left, right)) {
      return newAST_Expression_Real(getRealVal(left) / getRealVal(right));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPDIV);
    }
  case BINOPEXP:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(pow(left->getAsInteger()->val(), right->getAsInteger()->val())));
    } else if (shouldReturnReal(left, right)) {
      return newAST_Expression_Real(pow(getRealVal(left), getRealVal(right)));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPEXP);
    }
  default:
    Util::ERROR(
        "process_for_equations.cpp - evalBinOp:\n"
        "Incorrect Binary operation type.\n");
  }
  Util::ERROR(
      "process_for_equations.cpp - evalBinOp:\n"
      "Incorrect Binary operation type.\n");
  return nullptr;
}

AST_Expression EvalExp::evalCompRef(AST_Expression_ComponentReference compRef)
{
  EqualExp eqExp(_symbolTable);
  if (_compRef && eqExp.equalTraverse(compRef, _compRef)) {
    return _compRefVal;
  }
  Option<Variable> varInfo = _symbolTable[CREF_NAME(compRef)];
  if (varInfo && (varInfo->isParameter() || varInfo->isConstant())) {
    AST_Modification mod = varInfo->modification();
    switch (mod->modificationType()) {
    case MODEQUAL: {
      AST_Modification_Equal equal = mod->getAsEqual();
      return apply(equal->exp());
    }
    default:
      Util::ERROR(
          "RangeIterator::getVal\n"
          "Incorrect AST_Modification type or not supported yet.\n");
    }
  }
  return compRef;
}

// The component reference is an array of the form
// a[i,j].b[x].c[y] (indexesList is ( (i,j) , (x), (y) ).
AST_Expression EvalExp::evalArray(AST_Expression_ComponentReference array)
{
  AST_Expression_ComponentReference newArray = newAST_Expression_ComponentReference();
  AST_StringList names = array->names();
  AST_StringListIterator namesIter = names->begin();
  AST_ExpressionListList indexesList = array->indexes();
  AST_ExpressionListListIterator indexesListIter;
  foreach (indexesListIter, indexesList) {
    AST_ExpressionListIterator listIter;
    AST_ExpressionList indexes = current_element(indexesListIter);
    AST_ExpressionList newIndexes = newAST_ExpressionList();
    foreach (listIter, indexes) {
      AST_Expression arrayIndex = current_element(listIter);
      AST_Expression newArrayIndex = apply(arrayIndex);
      newIndexes->push_back(newArrayIndex);
    }
    newArray->append((AST_String)current_element(namesIter), newIndexes);
    namesIter++;
  }
  return newArray;
}

bool EvalExp::shouldReturnInteger(AST_Expression left, AST_Expression right)
{
  return (left->expressionType() == EXPINTEGER && right->expressionType() == EXPINTEGER);
}

bool EvalExp::shouldReturnReal(AST_Expression left, AST_Expression right)
{
  return ((left->expressionType() == EXPREAL && right->expressionType() == EXPINTEGER) ||
          (left->expressionType() == EXPINTEGER && right->expressionType() == EXPREAL) ||
          (left->expressionType() == EXPREAL && right->expressionType() == EXPREAL));
}

AST_Real EvalExp::getRealVal(AST_Expression exp)
{
  if (exp->expressionType() == EXPREAL) {
    return exp->getAsReal()->val();
  } else {
    return exp->getAsInteger()->val();
  }
}
}  // namespace Util
}  // namespace MicroModelica
