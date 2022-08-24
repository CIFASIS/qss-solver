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

#include <util/symbol_table.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sbg_graph/deps_graph.h>

namespace MicroModelica {
namespace Deps {

template<typename S, typename T>
class EQGraphBuilder : public S {
  public:
  EQGraphBuilder(T equations, IR::EquationTable algebraics, IR::STATEMENT::AssignTerm search);
  EQGraphBuilder(T equations, IR::EquationTable algebraics);
  ~EQGraphBuilder() = default;
  void setOrigEquations(T orig_equations);
  SB::Deps::Graph build();

  private:
  list<SB::Deps::Vertex> _F_nodes;
  list<SB::Deps::Vertex> _G_nodes;
  list<SB::Deps::Vertex> _g_nodes;
  list<SB::Deps::Vertex> _u_nodes;
  T _equations;
  T _orig_equations;
  IR::EquationTable _algebraics;
  std::map<std::string, int> _node_names;
  SB::EqUsage _usage;
};

// Typedef for concrete clases.

struct SDG {
  std::string name() { return "SD Graph";}
};

struct DDG {
  std::string name() { return "DD Graph";}
};

struct SZG {
  std::string name() { return "SZ Graph";}
};

struct DZG {
  std::string name() { return "DZ Graph";}
};

struct SOG {
  std::string name() { return "SO Graph";}
};

struct DOG {
  std::string name() { return "DO Graph";}
};

typedef EQGraphBuilder<SB::StateSelector<SDG>, IR::EquationTable> SDSBGraphBuilder;

typedef EQGraphBuilder<SB::StateSelector<SZG>, IR::EquationTable> SZSBGraphBuilder;

typedef EQGraphBuilder<SB::StateSelector<SOG>, IR::EquationTable> SOSBGraphBuilder;

typedef EQGraphBuilder<SB::DiscreteSelector<DOG>, IR::EquationTable> DOSBGraphBuilder;

typedef EQGraphBuilder<SB::DiscreteSelector<DDG>, IR::EquationTable> DDSBGraphBuilder;

typedef EQGraphBuilder<SB::DiscreteSelector<DZG>, IR::EquationTable> DZSBGraphBuilder;

}  // namespace Deps
}  // namespace MicroModelica
