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
#include "../../util/visitors/eval_init_exp.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
using namespace SBG;

IndexShiftBuilder::IndexShiftBuilder(EquationTable &algebraics, VarSymbolTable &symbols)
    : _algebraics(algebraics), _symbols(symbols), _index_shift()
{
}

IndexShift IndexShiftBuilder::build()
{
  EquationTable::iterator eq_it;
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    Expression lhs = eq.lhs();
    SBG::Offset index_shift;
    vector<int> vals;
    Option<Variable> var = eq.LHSVariable();
    if (eq.hasRange()) {
      EvalOccur eval_occur(eq.lhs(), _symbols, eq.range());
      SBG::LinearFunction lf = eval_occur.linearFunctions();
      RangeDefinitionTable ranges = eq.range()->definition();
      RangeDefinitionTable::iterator it;
      for (RangeDefinition range = ranges.begin(it); !ranges.end(it); range = ranges.next(it)) {
        // Use cBegin here to get minus 1 that we need for the shift.
        vals.push_back(range.cBegin());
      }
      int val = Variable::eval(var.get(), lf.apply(vals)) - 1;
      index_shift.add(val);
    } else {
      if (lhs.isScalar()) {
        EvalInitExp eval_exp(_symbols);
        ExpressionList indexes = lhs.indexes();
        for (Expression index : indexes) {
          vals.push_back(eval_exp.apply(index.expression()));
        }
        int val = Variable::eval(var.get(), vals) - 1;
        index_shift.add(val);
      } else {
        index_shift.add(0);
      }
    }
    _index_shift[eq.id()] = index_shift;
  }
  return _index_shift;
}

}  // namespace Deps
}  // namespace MicroModelica