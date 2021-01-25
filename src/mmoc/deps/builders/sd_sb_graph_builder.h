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

#ifndef SD_SB_GRAPH_BUILDER_H
#define SD_SB_GRAPH_BUILDER_H

#include "../../ir/class.h"
#include "../../util/symbol_table.h"
#include "../graph/sb_graph.h"

namespace MicroModelica {
namespace Deps {

class SDSBGraphBuilder {
  public:
  SDSBGraphBuilder(IR::EquationTable &equations, IR::EquationTable &algebraics);
  ~SDSBGraphBuilder() = default;
  SBG::SBGraph build();

  protected:
  std::string nodeName(IR::Equation eq);

  private:
  list<SBG::F_Vertex> _equation_def_nodes;
  list<SBG::S_Vertex> _equation_lhs_nodes;
  list<SBG::S_Vertex> _state_nodes;
  IR::EquationTable _equations;
  IR::EquationTable _algebraics;
  std::map<std::string, int> _node_names;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif /* SD_SB_GRAPH_BUILDER_H */
