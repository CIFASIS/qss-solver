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
#include <deps/sb_dependencies.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sbg_graph/graph.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
using namespace SB;
namespace Deps {

MergeGraphGenerator::MergeGraphGenerator() : _lhs_dsc_graph_builder(ModelConfig::instance().events(), ModelConfig::instance().algebraics(), STATEMENT::LHS), _lhs_dsc_graph(), _graph(), _edge_dom_offset(1)
{
  _lhs_dsc_graph = _lhs_dsc_graph_builder.build();
  SB::EqUsage usage;
  int max_dims = ModelConfig::instance().symbols().maxDim();
  EquationTable eqs = ModelConfig::instance().derivatives();
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EventTable events = ModelConfig::instance().events();
  EquationTable::iterator eq_it;
  for (Equation eq = eqs.begin(eq_it); !eqs.end(eq_it); eq = eqs.next(eq_it)) {
    SB::Deps::SetVertex ife_node = createSetVertex(eq, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencee, usage);
    addVertex(ife_node, _graph);
  }
  EventTable::iterator ev_it;
  for (Event ev = events.begin(ev_it); !events.end(ev_it); ev = events.next(ev_it)) {
    list<SB::Deps::SetVertex> ifr_nodes = createSetVertex(ev, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencer, usage, STATEMENT::LHS);
    for (SB::Deps::SetVertex ifr_node : ifr_nodes) {
      addVertex(ifr_node, _graph);
    }
  }
  for (Equation eq = algebraics.begin(eq_it); !algebraics.end(eq_it); eq = algebraics.next(eq_it)) {
    SB::Deps::SetVertex alg_node = createSetVertex(eq, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Equation, usage);
    addVertex(alg_node, _graph);
  }
}

MergeGraphGenerator::~MergeGraphGenerator() {}

Intersections MergeGraphGenerator::computeIntersections(SB::Set variables)
{
  Intersections inters;
  // This implementation relies on the fact that both discrete variables vertices 
  // are added in the same order, so the domain in both cases should match.
  VertexIt dsc_vertex_it = SB::Deps::findSetVertex(_lhs_dsc_graph, variables);
  SB::Deps::SetVertex dsc_vertex = _lhs_dsc_graph[*dsc_vertex_it];
  SB::Set intersection = dsc_vertex.range().cap(variables);
  list<Edge> edges  = SB::Deps::inputEdges(_lhs_dsc_graph, dsc_vertex.name());

  // As a second step, look for all the input edges that arrives to the discrete variable.
  for (Edge edge : edges) {
    SB::Deps::SetEdge edge_label = _lhs_dsc_graph[edge];
    SB::Deps::Vertex E = boost::source(edge, _lhs_dsc_graph);
    SB::Deps::SetVertex EV = _lhs_dsc_graph[E]; 
    SB::Set used_dscs = edge_label.mapU().image(intersection);
    if (!used_dscs.empty()) {
      IntersectInfo inter_info;
      inter_info.node = SB::Deps::findSetVertexByName(_graph, EV.name());
      inter_info.edge = edge_label;
      inter_info.intersection = intersection;
      inters.push_back(inter_info);
    }
  }
  return inters;
} 

void MergeGraphGenerator::init(SB::Deps::SetVertex vertex) {}

void MergeGraphGenerator::end() {}

void MergeGraphGenerator::postProcess(SB::Deps::SetVertex vertex) {}

void MergeGraphGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  Intersections inters = computeIntersections(var_dep.variables());
  if (!vertex.desc().type() == SB::Deps::VERTEX::Equation && !inters.empty()) {
    VertexIt eq_vertex_it = SB::Deps::findSetVertexByName(_graph, vertex.name());
    SB::Deps::SetVertex eq_vertex = _graph[*eq_vertex_it];
  }
  /*if (!_config.selector.isAlgebraic(vertex)) {
    string var_name = var_dep.var().name();
    DepData dep_data(_config.selector.id(vertex), var_dep);
    if (!findDep(_deps, dep_data, _config.selector.multipleNodes())) {
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    }
  }*/
}

void MergeGraphGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

void MergeGraphGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift)
{
}

void MergeGraphGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map,
                                 SB::Deps::LMapExp use_map_exp, IR::Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp,
                                 SB::Set intersection)
{
}

void MergeGraphGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

SB::Deps::Graph MergeGraphGenerator::def() { return _graph; }

template <class GraphBuilder, class Table>
MergeGraph<GraphBuilder, Table>::MergeGraph(Table table, IR::EquationTable algebraics, IR::STATEMENT::AssignTerm search)
    : _table(table), _algebraics(algebraics), _search(search)
{
}

template <class GraphBuilder, class Table>
SB::Deps::Graph MergeGraph<GraphBuilder, Table>::build()
{
  MergeGraphBuilder merge_graph_builder;
  IndexShiftBuilder index_shifts(_algebraics);
  GraphBuilder SBGraph(_table, _algebraics, _search);
  merge_graph_builder.compute(SBGraph.build(), index_shifts.build());
  SB::Deps::Graph merge_graph = merge_graph_builder.def();

  SB::Deps::GraphPrinter printer(merge_graph);

  printer.printGraph(Logger::instance().getLogsPath()+ "test_merge.dot");

  return merge_graph;
}

template class MergeGraph<Deps::DDSBGraphBuilder, IR::EquationTable>;

template class MergeGraph<Deps::DZSBGraphBuilder, IR::EquationTable>;

template class MergeGraph<Deps::DSCRHSGraphBuilder, IR::EventTable>;

}  // namespace Deps
}  // namespace MicroModelica
