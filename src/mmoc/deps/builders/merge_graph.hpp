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


#include <deps/builders/eq_graph_builder.h>
#include <deps/builders/ev_graph_builder.h>
#include <deps/sb_dependencies.h>
#include <deps/sbg_graph/deps_graph.h>
#include <ir/equation.h>
#include <ir/event.h>
#include <ir/node_selector.h>

namespace MicroModelica {
namespace Deps {

struct MergeHDSelector {
  std::string name();
  IR::EquationTable getNodes();
  Deps::EQSelector getConfig();
};

struct MergeHZSelector {
  std::string name();
  IR::EquationTable getNodes();
  Deps::EQSelector getConfig();
};

struct MergeHZSTSelector {
  std::string name();
  IR::EquationTable getNodes();
  Deps::EQSelector getConfig();
};

struct MergeHHSelector {
  std::string name();
  IR::EventTable getNodes();
  Deps::EVSelector getConfig();
};

template <class SBGraph, typename S, typename C, class Builder>
class MergeGraph : public S {
  public:
  MergeGraph(IR::EventTable table, IR::EquationTable algebraics, IR::STATEMENT::AssignTerm search);
  ~MergeGraph() = default;

  SB::Deps::Graph build();

  protected:
  IR::EquationTable _algebraics;
  IR::STATEMENT::AssignTerm _search;
};

typedef MergeGraph<Deps::DDSBGraphBuilder, MergeHDSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder> HDGraphBuilder;

typedef MergeGraph<Deps::DZSBGraphBuilder, MergeHZSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder> HZGraphBuilder;

typedef MergeGraph<Deps::SZSBGraphBuilder, MergeHZSTSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder> HZSTGraphBuilder;

typedef MergeGraph<Deps::DSCRHSGraphBuilder, MergeHHSelector, Deps::EVSelector, Deps::MergeEVGraphBuilder> HHGraphBuilder;

}  // namespace Deps
}  // namespace MicroModelica
