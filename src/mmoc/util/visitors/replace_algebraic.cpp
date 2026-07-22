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

#include "replace_algebraic.hpp"

#include <ast/ast_builder.hpp>
#include <util/error.hpp>
#include <util/model_config.hpp>
#include <util/util.hpp>
#include "partial_eval_exp.hpp"

namespace MicroModelica {
using namespace IR;
namespace Util {

ReplaceAlgebraic::ReplaceAlgebraic(IR::EquationTable algebraics) : _algebraics(algebraics) {}

Expression ReplaceAlgebraic::searchAlgebraic(Variable variable)
{
  EquationTable::iterator it;
  for (Equation alg = _algebraics.begin(it); !_algebraics.end(it); alg = _algebraics.next(it)) {
    Option<Variable> lhs_var = alg.LHSVariable();
    assert(lhs_var);
    if (variable == lhs_var.get()) {
      return alg.rhs();
    }
  }

  assert(false);
  return Expression();
}

AST_Expression ReplaceAlgebraic::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "replace_Algebraic.cpp:40 %s", cr->name().c_str());
      break;
    }
    if (var->isAlgebraic()) {
      Expression replace_exp = searchAlgebraic(var.get());
      AST_ExpressionList new_outputs = newAST_ExpressionList();
      AST_Expression rec_exp = apply(replace_exp.expression());
      new_outputs = AST_ListAppend(new_outputs, rec_exp);
      return newAST_Expression_OutputExpressions(new_outputs);
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
  case EXPCALL: {
    const AST_Expression_Call call = exp->getAsCall();
    AST_ExpressionList new_arguments = newAST_ExpressionList();
    AST_ExpressionList arguments = call->arguments();
    AST_ExpressionListIterator it;
    foreach (it, arguments) {
      new_arguments = AST_ListAppend(new_arguments, apply(current_element(it)));
    }
    return newAST_Expression_Call(call->name(), nullptr, new_arguments, call->outputArguments());
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionList arguments = call->arguments();
    AST_ExpressionList new_arguments = newAST_ExpressionList();
    AST_ExpressionListIterator it;
    foreach (it, arguments) {
      new_arguments = AST_ListAppend(new_arguments, apply(current_element(it)));
    }
    return newAST_Expression_FunctionCallArgs(new_arguments);
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceAlgebraic::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceAlgebraic::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
