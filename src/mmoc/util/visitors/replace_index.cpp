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

#include "replace_index.h"

#include <sstream>

#include <util/error.h>
#include <util/model_config.h>
#include <util/util.h>
#include <util/visitors/get_index_usage.h>
#include <util/visitors/get_index_variables.h>

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

ReplaceIndex::ReplaceIndex(Range range, AST_Expression exp, bool range_idxs) : _range(range), _usage(), _range_idxs(range_idxs)
{
  GetIndexUsage usage;
  _usage = usage.apply(exp);
}

AST_Expression ReplaceIndex::foldTraverseElement(AST_Expression exp)
{
  static constexpr int USED = 1;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      map<int, string> used_variables = getUsedVariables(exp);
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      int i = 0;
      AST_ExpressionList l = newAST_ExpressionList();
      assert(indexes->size() == (size_t)_usage.size());
      foreach (it, indexes) {
        bool is_parameter = false;
        if (current_element(it)->expressionType() == EXPCOMPREF) {
          AST_Expression_ComponentReference comp_ref = current_element(it)->getAsComponentReference(); 
          Option<Variable> var = ModelConfig::instance().lookup(comp_ref->name());
          if (!var) {
              Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "replace_index.cpp:63 %s", comp_ref->name().c_str());
              break;
          }
          is_parameter = var->isParameter();
        }
        if (is_parameter) {
          ReplaceIndex replace_param = ReplaceIndex(_range, current_element(it), _range_idxs);
          l = AST_ListAppend(l, replace_param.apply(current_element(it)));    
        } else { 
          if (_usage[i] == USED) {
            Option<Variable> used_var = ModelConfig::instance().lookup(used_variables[i+1]);
            string used_var_name = "";
            if (used_var) {
              used_var_name = used_var->name();
            }
            string var = _range.iterator(used_var_name, i, _range_idxs);
            ReplaceVar rv(var);
            l = AST_ListAppend(l, rv.apply(current_element(it)));
          } else {
            l = AST_ListAppend(l, current_element(it));
          }
        }
        i++;
      }
      ret = AST_Expression_ComponentReference_Add(ret, newAST_String(cr->name()), l);
      return ret;
    }
    break;
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceIndex::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceIndex::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

map<int, std::string> ReplaceIndex::getUsedVariables(AST_Expression exp)
{
  GetIndexVariables index_usage;
  multimap<string, int> used_variables = index_usage.apply(exp);
  map<int, string> d;
  for(auto used : used_variables) {
    d[used.second] = used.first;
  }
  return d;
}

ReplaceVar::ReplaceVar(string var) : _var(var) {}

AST_Expression ReplaceVar::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    VarSymbolTable symbols = ModelConfig::instance().symbols();
    Option<Variable> var = symbols[exp->getAsComponentReference()->name()];
    assert(var);
    if (!var->isConstant()) {
      Variable vi(newType_Integer(), TP_FOR, nullptr, nullptr, vector<int>(1, 1), false);
      ModelConfig::instance().addVariable(_var, vi);
      return newAST_Expression_ComponentReferenceExp(newAST_String(_var));
    }
    return exp;
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ReplaceVar::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ReplaceVar::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
