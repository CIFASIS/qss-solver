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

#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sbg_graph/deps_graph.h>

namespace MicroModelica {
namespace Deps {

template<typename S>
class EvGraphBuilder : public S {
  public:
  EvGraphBuilder(IR::EventTable &events, IR::EquationTable &algebraics, IR::STATEMENT::AssignTerm search);
  ~EvGraphBuilder() = default;
  SB::Deps::Graph build();

  private:
  list<SB::Deps::Vertex> _F_nodes;
  list<SB::Deps::Vertex> _G_nodes;
  list<SB::Deps::Vertex> _g_nodes;
  list<SB::Deps::Vertex> _u_nodes;
  IR::EventTable _events;
  IR::EquationTable _algebraics;
  std::map<std::string, int> _node_names;
  SB::EqUsage _usage;
  IR::STATEMENT::AssignTerm _search;
};

// Typedef for concrete clases.

struct STLG {
  std::string name() { return "Handler LHS State Graph"; }
};

struct STRG {
  std::string name() { return "Handler RHS State Graph"; }
};

struct DHG {
  std::string name() { return "Handler LHS Discrete Graph"; }
};

typedef EvGraphBuilder<SB::StateSelector<STLG>> LHSStGraphBuilder;

typedef EvGraphBuilder<SB::StateSelector<STRG>> RHSStGraphBuilder;

typedef EvGraphBuilder<SB::DiscreteSelector<DHG>> DSCGraphBuilder;

}  // namespace Deps
}  // namespace MicroModelica
