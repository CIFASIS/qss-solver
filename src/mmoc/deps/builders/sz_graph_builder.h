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
#ifndef SZ_GRAPH_BUILDER_H
#define SZ_GRAPH_BUILDER_H

#include "../../ir/class.h"
#include "../../util/symbol_table.h"
#include "../graph/graph.h"

namespace MicroModelica {
namespace Deps {
class SZGraphBuilder {
  public:
  SZGraphBuilder(IR::EventTable &events, IR::EquationTable &algebraics);
  ~SZGraphBuilder() = default;
  DepsGraph build();

  private:
  list<EqVertex> _equationDescriptors;
  list<IfrVertex> _variableDescriptors;
  list<IfeVertex> _eventDescriptors;
  IR::EventTable _events;
  IR::EquationTable _algebraics;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif /* SZ_GRAPH_BUILDER_H */
