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

#include "get_index_variables.h"

#include <sstream>

#include <ast/ast_builder.h>
#include <util/error.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace IR;
namespace Util {

GetIndexVariables::GetIndexVariables() : _in_index_list(false), _pos(0) {}

multimap<std::string, int> GetIndexVariables::foldTraverseElement(AST_Expression exp)
{
  multimap<std::string, int> ret;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "get_index_variables.cpp:41 %s", cr->name().c_str());
      break;
    }
    if (_in_index_list) {
      ret.insert(std::make_pair(cr->name(), _pos));
    }
    bool parsing_parameter = _in_index_list && var->isParameter();
    if (cr->hasIndexes()) {
      assert(!_in_index_list || parsing_parameter);
      if (!parsing_parameter) {
        _in_index_list = true;
        _pos = 1;
      }
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      foreach (it, indexes) {
        multimap<std::string, int> args = apply(current_element(it));
        ret.insert(args.begin(), args.end());
        if (!parsing_parameter) {
          _pos++;
        }
      }
      if (!parsing_parameter) {
        _in_index_list = false;
      }
    }
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output out = exp->getAsOutput();
    AST_ExpressionListIterator it;
    foreach (it, out->expressionList()) {
      multimap<std::string, int> args = apply(current_element(it));
      ret.insert(args.begin(), args.end());
    }
    break;
  }
  case EXPCALL: {
    AST_Expression_Call call = exp->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      multimap<std::string, int> args = apply(current_element(it));
      ret.insert(args.begin(), args.end());
    }
    break;
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      multimap<std::string, int> args = apply(current_element(it));
      ret.insert(args.begin(), args.end());
    }
  }
  default:
    break;
  }
  return ret;
}

}  // namespace Util
}  // namespace MicroModelica