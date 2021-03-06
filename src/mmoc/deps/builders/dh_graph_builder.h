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
#ifndef DH_GRAPH_BUILDER_H
#define DH_GRAPH_BUILDER_H

#include "../../ir/class.h"
#include "../../ir/index.h"
#include "../../util/symbol_table.h"
#include "../graph/graph.h"

namespace MicroModelica {
namespace Deps {

namespace DHGRAPHBUILDER {
typedef enum { State, Discrete } IfrType;
}

class DHGraphBuilder {
  public:
  DHGraphBuilder(IR::EventTable& events, IR::EquationTable& algebraics,
                 IR::STATEMENT::AssignTerm search = IR::STATEMENT::LHS, DHGRAPHBUILDER::IfrType ifr_type = DHGRAPHBUILDER::Discrete);
  ~DHGraphBuilder() = default;
  DepsGraph build();

  protected:
  void addStatements(IR::StatementTable stms, DepsGraph& graph, IR::Expression exp, int id, Option<IR::Range> range);
  bool addNewEvent(VertexProperty source, VertexProperty sink);

  private:
  list<EqVertex> _statementDescriptors;
  list<IfrVertex> _variableDescriptors;
  list<IfeVertex> _eventDescriptors;
  IR::EventTable _events;
  IR::EquationTable _algebraics;
  IR::STATEMENT::AssignTerm _search;
  DHGRAPHBUILDER::IfrType _ifr_type;
  std::map<string, std::vector<int>> _pushed_vars;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif /* DH_GRAPH_BUILDER_H */
