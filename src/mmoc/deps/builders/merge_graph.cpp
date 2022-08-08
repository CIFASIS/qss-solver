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

#include <deps/builders/index_shift_builder.h>
#include <deps/builders/merge_graph_builder.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sbg_graph/graph.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
using namespace SB;
namespace Deps {

std::string MergeHDSelector::name() { return "HD Matrix"; };

Deps::EQSelector MergeHDSelector::getConfig() { return Deps::EQSelector(getNodes(), true, false); }

IR::EquationTable MergeHDSelector::getNodes() { return ModelConfig::instance().derivatives(); };

std::string MergeHZSelector::name() { return "HZ Matrix"; };

IR::EquationTable MergeHZSelector::getNodes() { return zeroCrossingTable(ModelConfig::instance().events()); };

Deps::EQSelector MergeHZSelector::getConfig() { return Deps::EQSelector(getNodes(), false, false); }

std::string MergeHZSTSelector::name() { return "HZ ST Matrix"; };

IR::EquationTable MergeHZSTSelector::getNodes() { return zeroCrossingTable(ModelConfig::instance().events()); };

Deps::EQSelector MergeHZSTSelector::getConfig() { return Deps::EQSelector(getNodes(), false, true); }

std::string MergeHHSelector::name() { return "HH Matrix"; };

IR::EventTable MergeHHSelector::getNodes() { return ModelConfig::instance().events(); };

Deps::EVSelector MergeHHSelector::getConfig() { return Deps::EVSelector(getNodes()); }

template <class GraphBuilder, typename S, typename C, class Builder>
MergeGraph<GraphBuilder, S, C, Builder>::MergeGraph(IR::EventTable table, IR::EquationTable algebraics, IR::STATEMENT::AssignTerm search)
    : _algebraics(algebraics), _search(search)
{
}

template <class GraphBuilder, typename S, typename C, class Builder>
SB::Deps::Graph MergeGraph<GraphBuilder, S, C, Builder>::build()
{
  Builder merge_graph_builder;
  IndexShiftBuilder index_shifts(_algebraics);
  GraphBuilder SBGraph(S::getNodes(), _algebraics, _search);

  SB::Deps::Graph graph = SBGraph.build();

  SB::Deps::EdgeIt ei_start, ei_end;
  boost::tie(ei_start, ei_end) = edges(graph);
  PWLMap mapF;
  PWLMap mapU;

  // First build the complete maps so we can get the pre-images.
  for (; ei_start != ei_end; ++ei_start) {
    PWLMap f_map = (graph[*ei_start]).mapF();
    PWLMap u_map = (graph[*ei_start]).mapU();

    mapF = mapF.concat(f_map);
    mapU = mapU.concat(u_map);
  }

  C config = S::getConfig();
  config.setMapU(mapU);
  config.setMapF(mapF);

  merge_graph_builder.setup(config);
  merge_graph_builder.compute(graph, index_shifts.build());
  SB::Deps::Graph merge_graph = merge_graph_builder.def();

  SB::Deps::GraphPrinter printer(merge_graph);

  printer.printGraph(Logger::instance().getLogsPath()+S::name()+".dot");

  return merge_graph;
}

template class MergeGraph<Deps::DDSBGraphBuilder, MergeHDSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder>;

template class MergeGraph<Deps::DZSBGraphBuilder, MergeHZSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder>;

template class MergeGraph<Deps::SZSBGraphBuilder, MergeHZSTSelector, Deps::EQSelector, Deps::MergeEQGraphBuilder>;

template class MergeGraph<Deps::DSCRHSGraphBuilder, MergeHHSelector, Deps::EVSelector, Deps::MergeEVGraphBuilder>;

}  // namespace Deps
}  // namespace MicroModelica
