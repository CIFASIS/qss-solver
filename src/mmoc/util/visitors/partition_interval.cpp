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

#include "partition_interval.h"

#include <util/error.h>
#include <util/model_config.h>
#include <util/visitors/eval_init_exp.h>

namespace MicroModelica {
using namespace Deps;
using namespace IR;
namespace Util {

PartitionInterval::PartitionInterval() {}

list<int> PartitionInterval::foldTraverseElement(AST_Expression exp)
{
  list<int> part;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "partition_interval.cpp:37 %s", cr->name().c_str());
      break;
    }
    if (cr->hasIndexes()) {
      AST_ExpressionList indexes = AST_ListFirst(cr->indexes());
      AST_Expression index = AST_ListFirst(indexes);
      ExpressionType index_type = index->expressionType();
      if (index_type == EXPRANGE) {
        AST_ExpressionList range_exps = index->getAsRange()->expressionList();
        AST_ExpressionListIterator it;
        EvalInitExp eval_init_exp;
        int count = 0;
        int range[3];
        foreach (it, range_exps) {
          range[count++] = eval_init_exp.apply(current_element(it));
        }
        if (count == 2) {
          range[2] = range[1];
          range[1] = 1;
        }
        for (int p = range[0]; p < range[2]; p += range[1]) {
          part.push_back(var->offset() + p);
        }
        return part;
      } else if (index_type == EXPCOLON) {
        for (int p = 0; p < var->size(); p++) {
          part.push_back(var->offset() + p);
        }
        return part;
      }
    } else if (var->isArray()) {
      for (int p = 0; p < var->size(); p++) {
        part.push_back(var->offset() + p);
      }
      return part;
    } else {
      part.push_back(var->offset());
      return part;
    }
  }
  default:
    return part;
  }
  return part;
}

list<int> PartitionInterval::foldTraverseElementUMinus(AST_Expression exp) { return list<int>(); }

list<int> PartitionInterval::foldTraverseElement(list<int> l, list<int> r, BinOpType bot) { return list<int>(); }

}  // namespace Util
}  // namespace MicroModelica
