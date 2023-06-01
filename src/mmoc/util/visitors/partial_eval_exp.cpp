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

#include <ast/ast_builder.h>
#include <util/debug.h>
#include <util/error.h>
#include <util/model_config.h>
#include <util/symbol_table.h>

namespace MicroModelica {
namespace Util {

/* Eval Expression class. */

PartialEvalExp::PartialEvalExp() {}

AST_Expression PartialEvalExp::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "partial_eval_exp.cpp:43 %s", cr->name().c_str());
      break;
    }
    if (var->isConstant()) {
      return newAST_Expression_Integer(var->value());
    }
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference new_cr = newAST_Expression_ComponentReference();
      AST_ExpressionList idxs = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_ExpressionList new_idxs = newAST_ExpressionList();
      foreach (it, idxs) {
        new_idxs = AST_ListAppend(new_idxs, apply(current_element(it)));
      }
      new_cr = AST_Expression_ComponentReference_Add(new_cr, newAST_String(cr->name()), new_idxs);
      return new_cr;
    }
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output output = exp->getAsOutput();
    AST_ExpressionList new_outputs = newAST_ExpressionList();
    AST_ExpressionList outputs = output->expressionList();
    AST_ExpressionListIterator it;
    foreach (it, outputs) {
      new_outputs = AST_ListAppend(new_outputs, apply(current_element(it)));
    }
    return newAST_Expression_OutputExpressions(new_outputs);
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
    } else if (canReduce(left)) {
      return reduce(left, right, binOpType);
    } else if (canReduce(right)) {
      return reduce(right, left, binOpType);
    } else {
      return newAST_Expression_BinOp(left, right, BINOPADD);
    }
  case BINOPSUB:
    if (shouldReturnInteger(left, right)) {
      return (newAST_Expression_Integer(getValue(left) - getValue(right)));
    } else if (canReduce(left)) {
      return reduce(left, right, binOpType);
    } else if (canReduce(right)) {
      return reduce(right, left, binOpType);
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
                       "Partial evaluation:\n"
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
        "Partial evaluation:\n"
        "Incorrect Binary operation type.\n");
  }
  Util::ERROR(
      "Partial evaluation::\n"
      "Incorrect Binary operation type.\n");
  return nullptr;
}

bool PartialEvalExp::canReduce(AST_Expression exp)
{
  if (exp->expressionType() == EXPBINOP) {
    AST_Expression_BinOp bin_op = exp->getAsBinOp();
    BinOpType type = bin_op->binopType();
    AST_Expression left = bin_op->left();
    AST_Expression right = bin_op->right();
    switch (type) {
    case BINOPADD:
    case BINOPSUB:
      return isIntegerOrConstant(left) || isIntegerOrConstant(right);
    default:
      break;
    }
  }
  return false;
}

AST_Expression PartialEvalExp::reduce(AST_Expression reduce, AST_Expression term, BinOpType type)
{
  assert(reduce->expressionType() == EXPBINOP);
  if (isIntegerOrConstant(term)) {
    AST_Expression_BinOp bin_op = reduce->getAsBinOp();
    AST_Expression left = bin_op->left();
    AST_Expression right = bin_op->right();
    int new_constant = 0;
    int sign = (bin_op->binopType() == BINOPSUB) ? -1 : 1;
    AST_Expression new_term = nullptr;
    if (isIntegerOrConstant(left)) {
      new_constant = sign * getValue(left);
      new_term = right;
    } else if (isIntegerOrConstant(right)) {
      new_constant = sign * getValue(right);
      new_term = left;
    }
    if (type == BINOPADD) {
      new_constant += getValue(term);
    } else {
      new_constant -= getValue(term);
    }
    assert(new_term != nullptr);
    BinOpType new_type = (new_constant >= 0) ? BINOPADD : BINOPSUB;
    return newAST_Expression_BinOp(new_term, newAST_Expression_Integer(abs(new_constant)), new_type);
  }
  return newAST_Expression_BinOp(reduce, term, type);
}

bool PartialEvalExp::isIntegerOrConstant(AST_Expression exp)
{
  if (exp->expressionType() == EXPINTEGER) {
    return true;
  } else if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference ref = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(ref->name());
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
    Option<Variable> var = ModelConfig::instance().lookup(ref->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "partial_eval_exp.cpp:214 %s", ref->name().c_str());
      return 0;
    }
    assert(var->isConstant());
    return var->value();
  }
  assert(false);
}
}  // namespace Util
}  // namespace MicroModelica
