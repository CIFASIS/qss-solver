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

#include "is_constant_index.h"

#include <util/error.h>
#include <util/model_config.h>
#include <util/util.h>

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

CheckIndexExpression::CheckIndexExpression() : _in_index_list(false), _is_constant(false), _has_parameters(false) {}

bool CheckIndexExpression::foldTraverseElement(AST_Expression exp)
{
  bool ret = true;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (_in_index_list) {
      VarSymbolTable symbols = ModelConfig::instance().symbols();
      Option<Variable> var = symbols[cr->name()];
      if (!var) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "is_constant_index.cpp:43 %s", cr->name().c_str());
        break;
      }
      if (var->isParameter()) {
        _has_parameters = true;
        return false;
      }
      if (var->isConstant()) {
        _is_constant = true;
        return true;
      }
      ret = false;
    }
    if (cr->hasIndexes()) {
      assert(!_in_index_list);
      _in_index_list = true;
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      foreach (it, indexes) {
        ret = ret && apply(current_element(it));
        _is_constant = ret;
      }
      _in_index_list = false;
    }
    break;
  }
  default:
    break;
  }
  return ret;
}

bool CheckIndexExpression::isConstant() const { return _is_constant; }

bool CheckIndexExpression::hasParameters() const { return _has_parameters; }

}  // namespace Util
}  // namespace MicroModelica
