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

#include <map>

#include <deps/builders/eq_graph_builder.h>
#include <deps/builders/ev_graph_builder.h>
#include <deps/sbg_graph/deps_graph.h>
#include <ir/equation.h>
#include <ir/event.h>
#include <ir/node_selector.h>
#include <ir/statement.h>

namespace MicroModelica {
namespace Deps {

struct IntersectInfo {
  SB::Deps::VertexIt node;
  SB::Deps::SetVertex orig_node;
  SB::Deps::SetEdge edge;
  SB::Set intersection;
};

typedef list<IntersectInfo> Intersections;

template <typename S>
class MergeGraphGenerator {
  public:
  MergeGraphGenerator();
  ~MergeGraphGenerator();

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp,
              IR::Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp, SB::Set intersection);

  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  SB::Deps::Graph def();
  void setup(S config);
  S config();

  protected:
  Intersections computeIntersections(SB::Set variables);
  SB::PWLMap buildMap(SB::Set dom, int convert_offset, SB::Set dom_map, int graph_offset);
  SB::Deps::LMapExp buildLMapExp(IR::Expression dom, IR::Expression use_exp);
  SB::Deps::LMapExp buildLMapFromExp(IR::Expression exp);
  SB::EdgeMaps generatePWLMaps(IntersectInfo inter_info, SB::Deps::SetVertex orig_ife_vertex, SB::Deps::SetVertex ife_vertex,
                               SB::Deps::VariableDep var_dep);
  void addEdges(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  SB::Deps::SetVertex findAlgVertex(SB::Set variables);

  Deps::DSCGraphBuilder _lhs_dsc_graph_builder;
  Deps::LHSStGraphBuilder _lhs_state_graph_builder;
  SB::Deps::Graph _lhs_dsc_graph;
  SB::Deps::Graph _graph;
  int _edge_dom_offset;
  S _config;
  list<SB::Deps::SetVertex> _nodes;
};

}  // namespace Deps
}  // namespace MicroModelica
