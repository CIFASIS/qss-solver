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

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
using namespace SBG;

IndexShiftBuilder::IndexShiftBuilder(EquationTable &algebraics)
    : _algebraics(algebraics), _index_shift()
{
}

IndexShift IndexShiftBuilder::build()
{
  EquationTable::iterator eq_it;
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    EvalOccur eval_occur(eq.lhs(), eq.range());
    if (eval_occur.hasIndex()) {
      _index_shift[eq.id()] = eval_occur.offsets();
    }
  }
  return _index_shift;
}

}  // namespace Deps
}  // namespace MicroModelica