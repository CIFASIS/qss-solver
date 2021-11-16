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

#pragma once

#include <ir/class.h>
#include <util/symbol_table.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sbg_graph/deps_graph.h>

namespace MicroModelica {
namespace Deps {

class SDSBGraphBuilder {
  public:
  SDSBGraphBuilder(IR::EquationTable &equations, IR::EquationTable &algebraics);
  ~SDSBGraphBuilder() = default;
  SB::Deps::Graph build();

  protected:
  std::string nodeName(IR::Equation eq);

  private:
  list<SB::Deps::Vertex> _F_nodes;
  list<SB::Deps::Vertex> _G_nodes;
  list<SB::Deps::Vertex> _g_nodes;
  list<SB::Deps::Vertex> _u_nodes;
  IR::EquationTable _equations;
  IR::EquationTable _algebraics;
  std::map<std::string, int> _node_names;
  SB::EqUsage _usage;
};
}  // namespace Deps
}  // namespace MicroModelica
