/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "index_shift_builder.h"

#include "../graph/sb_graph.h"
#include "../../util/util_types.h"
#include "../../util/visitors/apply_var_usage.h"
#include "../../util/visitors/eval_init_exp.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
using namespace SBG;

IndexShiftBuilder::IndexShiftBuilder(EquationTable &algebraics)
    : _algebraics(algebraics), _index_shift()
{
}

int IndexShiftBuilder::flatter(Variable variable, vector<int> usage)
{
  assert(variable.dimensions() == usage.size());
  int flatt = 0;
  for (unsigned int i = 0; i < usage.size(); i++) {
    flatt += variable.rowSize(i) * usage.at(i);    
  }
  return flatt;
}

IndexShift IndexShiftBuilder::build()
{
  EquationTable::iterator eq_it;
  map<string, map<int, int> > variable_shifts;
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    Option<Variable> lhs_var = eq.LHSVariable();
    assert(lhs_var);
    if (lhs_var->isScalar()) {
      map<int, int> var_values;
      var_values[eq.id()] = 1;
      variable_shifts[lhs_var->name()] = var_values;
    } else {
      list<Expression> indexes = eq.lhs().indexes();
      assert(indexes.size() > 0);
      vector<int> usage;
      map<int, int> var_values;
      map<string, AST_Expression> usage_exps;
      if (eq.hasRange()) {
        usage_exps = eq.range()->initExps();    
      }  
      for (Expression idx : indexes){
        ApplyVariableUsage apply_var_usage = ApplyVariableUsage(usage_exps);
        EvalInitExp eval;
        usage.push_back(eval.apply(apply_var_usage.apply(idx.expression())));     
      }
      var_values[eq.id()] = flatter(lhs_var.get(), usage); 
      variable_shifts[lhs_var->name()] = var_values;
    }
  }
  for (auto var_shift : variable_shifts) {
    for (auto shifts : var_shift.second) {
      _index_shift[shifts.first] = shifts.second - 1;
    }  
  }
  return _index_shift;
}

}  // namespace Deps
}  // namespace MicroModelica