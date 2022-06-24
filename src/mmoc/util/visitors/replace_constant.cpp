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

#include "replace_constant.h"

#include "../error.h"
#include "../model_config.h"
#include "../util.h"
#include "partial_eval_exp.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ReplaceConstant::ReplaceConstant() {}

AST_Expression ReplaceConstant::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "replace_constant.cpp:40 %s", cr->name().c_str());
      break;
    }
    if (var->isConstant()) {
      return newAST_Expression_Integer(var->value());
    }
    if (cr->hasIndexes()) {
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_ExpressionList l = newAST_ExpressionList();
      foreach (it, indexes) {
        PartialEvalExp partial_eval;
        l = AST_ListAppend(l, partial_eval.apply(current_element(it)));
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
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceConstant::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceConstant::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
