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

#include "partial_eval_exp.h"

#include <math.h>

#include "../../ast/ast_builder.h"
#include "../debug.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

/* Eval Expression class. */

PartialEvalExp::PartialEvalExp(VarSymbolTable symbolTable) { _symbols = symbolTable; }

AST_Expression PartialEvalExp::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = _symbols[cr->name()];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "partial_eval_exp.cpp:43 %s", cr->name().c_str());
      break;
    }
    if (var->isConstant()) {
      return newAST_Expression_Integer(var->value());
    }
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference newCR = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_ExpressionList newSubs = newAST_ExpressionList();
      foreach (it, indexes) {
        newSubs = AST_ListAppend(newSubs, apply(current_element(it)));
      }
      newCR = AST_Expression_ComponentReference_Add(newCR, newAST_String(cr->name()), newSubs);
      return newCR;
    }
    break;
  }
  default:
    return exp;
  }
  return exp;
}

AST_Expression PartialEvalExp::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression_Integer zero = (AST_Expression_Integer)newAST_Expression_Integer(0);
  return (foldTraverseElement(zero, apply(exp->getAsUMinus()->exp()), BINOPSUB));
}

AST_Expression PartialEvalExp::foldTraverseElement(AST_Expression left, AST_Expression right, BinOpType binOpType)
{
  switch (binOpType) {
  case BINOPADD:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(getValue(left) + getValue(right)));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPADD);
    }
  case BINOPSUB:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(getValue(left) - getValue(right)));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPSUB);
    }
  case BINOPMULT:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(getValue(left) * getValue(right)));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPMULT);
    }
  case BINOPDIV:
    Util::ERROR_UNLESS(right != 0,
                       "process_for_equations - evalExp:\n"
                       "Division by zero.\n");
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(getValue(left) / getValue(right)));
    } else {
      return newAST_Expression_BinOp(left, right, BINOPDIV);
    }
  case BINOPEXP:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(pow(getValue(left), getValue(right))));
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
  return NULL;
}

bool PartialEvalExp::isIntegerOrConstant(AST_Expression exp)
{
  if (exp->expressionType() == EXPINTEGER) {
    return true;
  } else if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference ref = exp->getAsComponentReference();
    Option<Variable> var = _symbols[ref->name()];
    if (var) {
      return var->isConstant();
    }
  }
  return false;
}

bool PartialEvalExp::shouldReturnInteger(AST_Expression left, AST_Expression right)
{
  return (isIntegerOrConstant(left) && isIntegerOrConstant(right));
}

int PartialEvalExp::getValue(AST_Expression exp)
{
  if (exp->expressionType() == EXPINTEGER) {
    return exp->getAsInteger()->val();
  } else if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference ref = exp->getAsComponentReference();
    Option<Variable> var = _symbols[ref->name()];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "partial_eval_exp.cpp:148 %s", ref->name().c_str());
      return 0;
    }
    assert(var->isConstant());
    return var->value();
  }
  assert(false);
}
}  // namespace Util
}  // namespace MicroModelica
