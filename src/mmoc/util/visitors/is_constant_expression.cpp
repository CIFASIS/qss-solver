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

#include "is_constant_expression.h"

#include "../error.h"
#include "../util.h"
#include "is_constant_index.h"

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

IsConstantExpression::IsConstantExpression() : _in_index_list(false) {}

bool IsConstantExpression::foldTraverseElement(AST_Expression exp)
{
  bool ret = true;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    VarSymbolTable symbols = Utils::instance().symbols();
    Option<Variable> var = symbols[cr->name()];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "is_constant_expression.cpp:41 %s",
                            cr->name().c_str());
      break;
    }
    if (var->isParameter() && cr->hasIndexes()) {
      IsConstantIndex constant_index;
      ret = constant_index.apply(exp);
    } else if (!var->isConstant() && !var->isParameter() && (var->name() != "_chain_rule")) {
      ret = false;
    }
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output out = exp->getAsOutput();
    AST_ExpressionListIterator it;
    foreach (it, out->expressionList()) {
      ret = ret && apply(current_element(it));
    }
    break;
  }
  case EXPCALL: {
    AST_Expression_Call call = exp->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      ret = ret && apply(current_element(it));
    }
    break;
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionList el = call->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      ret = ret && apply(current_element(it));
    }
    break;
  }
  default:
    break;
  }
  return ret;
}

}  // namespace Util
}  // namespace MicroModelica
