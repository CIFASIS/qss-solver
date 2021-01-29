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

#include "apply_var_usage.h"

#include "../../ast/ast_builder.h"
#include "../error.h"
#include "../util.h"
#include "partial_eval_exp.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ApplyVariableUsage::ApplyVariableUsage(std::map<std::string, AST_Expression> usage_map) : _usage_map(usage_map) {}

AST_Expression ApplyVariableUsage::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    string var_name = cr->name();
    if (_usage_map.find(var_name) != _usage_map.end()) {
      return _usage_map[var_name];
    }
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_ExpressionList l = newAST_ExpressionList();
      foreach (it, indexes) {
        l = AST_ListAppend(l, apply(current_element(it)));
      }
      ret = AST_Expression_ComponentReference_Add(ret, newAST_String(cr->name()), l);
      return ret;
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
    AST_Expression_Call call = exp->getAsCall();
    AST_ExpressionList args = call->arguments();
    AST_ExpressionList new_args = newAST_ExpressionList();
    AST_ExpressionListIterator it;
    foreach (it, args) {
      new_args = AST_ListAppend(new_args, apply(current_element(it)));
    }
    args = call->outputArguments();
    AST_ExpressionList new_output_args = newAST_ExpressionList();
    foreach (it, args) {
      new_output_args = AST_ListAppend(new_output_args, apply(current_element(it)));
    }
    return newAST_Expression_Call(call->name(), nullptr, new_args, new_output_args);
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionList args = call->arguments();
    AST_ExpressionList new_args = newAST_ExpressionList();
    AST_ExpressionListIterator it;
    foreach (it, args) {
      new_args = AST_ListAppend(new_args, apply(current_element(it)));
    }
    return newAST_Expression_FunctionCallArgs(new_args);
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ApplyVariableUsage::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ApplyVariableUsage::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
