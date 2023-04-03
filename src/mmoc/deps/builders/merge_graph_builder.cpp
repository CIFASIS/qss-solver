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
#include <util/visitors/pwl_map_values.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
using namespace SB;
namespace Deps {

template <typename S>
MergeGraphGenerator<S>::MergeGraphGenerator()
    : _lhs_dsc_graph_builder(ModelConfig::instance().events(), ModelConfig::instance().algebraics(), STATEMENT::LHS),
      _lhs_state_graph_builder(ModelConfig::instance().events(), ModelConfig::instance().algebraics(), STATEMENT::LHS),
      _lhs_dsc_graph(),
      _graph(),
      _edge_dom_offset(1),
      _config(),
      _nodes()
{
}

template <typename S>
MergeGraphGenerator<S>::~MergeGraphGenerator()
{
}

template <typename S>
void MergeGraphGenerator<S>::setup(S config)
{
  _config = config;
  if (_config.lhsStates()) {
    _lhs_dsc_graph = _lhs_state_graph_builder.build();
  } else {
    _lhs_dsc_graph = _lhs_dsc_graph_builder.build();
  }
  SB::EqUsage usage;
  int max_dims = ModelConfig::instance().symbols().maxDim();
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EventTable events = ModelConfig::instance().events();
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (_config.validVariable(var)) {
      SB::Deps::SetVertex state_node = createSetVertex(var, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencee);
      addVertex(state_node, _graph);
    }
  }

  EventTable::iterator ev_it;
  for (Event ev = events.begin(ev_it); !events.end(ev_it); ev = events.next(ev_it)) {
    list<SB::Deps::SetVertex> ifr_nodes =
        createSetVertex(ev, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencer, usage, STATEMENT::LHS);
    for (SB::Deps::SetVertex ifr_node : ifr_nodes) {
      addVertex(ifr_node, _graph);
    }
  }

  EquationTable::iterator eq_it;
  for (Equation eq = algebraics.begin(eq_it); !algebraics.end(eq_it); eq = algebraics.next(eq_it)) {
    Option<SB::Deps::SetVertex> alg_node = createSetVertex(eq, _edge_dom_offset, max_dims, SB::Deps::VERTEX::Equation, usage);
    if (alg_node) {
      addVertex(alg_node.get(), _graph);
    }
  }
}

template <typename S>
S MergeGraphGenerator<S>::config()
{
  return _config;
}

template <typename S>
Intersections MergeGraphGenerator<S>::computeIntersections(SB::Set variables)
{
  Intersections inters;
  // This implementation relies on the fact that both discrete variables vertices
  // are added in the same order, so the domain in both cases should match.
  VertexIt dsc_vertex_it = SB::Deps::findSetVertex(_lhs_dsc_graph, variables);
  SB::Deps::SetVertex dsc_vertex = _lhs_dsc_graph[*dsc_vertex_it];
  SB::Set intersection = dsc_vertex.range().cap(variables);
  list<SB::Deps::Edge> edges = SB::Deps::inputEdges(_lhs_dsc_graph, dsc_vertex.name());

  // As a second step, look for all the input edges that arrives to the discrete variable.
  for (SB::Deps::Edge edge : edges) {
    SB::Deps::SetEdge edge_label = _lhs_dsc_graph[edge];
    SB::Deps::Vertex E = boost::source(edge, _lhs_dsc_graph);
    SB::Deps::SetVertex EV = _lhs_dsc_graph[E];
    if (EV.desc().type() == SB::Deps::VERTEX::Influencer) {
      SB::Set whole_dom = edge_label.mapU().wholeDom();
      SB::Set input_edge_image = edge_label.mapU().image(whole_dom);
      SB::Set used_dscs = intersection.cap(input_edge_image);
      if (!used_dscs.empty()) {
        IntersectInfo inter_info;
        inter_info.node = SB::Deps::findSetVertexByName(_graph, EV.name());
        inter_info.orig_node = EV;
        inter_info.edge = edge_label;
        inter_info.intersection = used_dscs;
        inters.push_back(inter_info);
      }
    }
  }
  return inters;
}

template <typename S>
void MergeGraphGenerator<S>::init(SB::Deps::SetVertex vertex)
{
  _nodes.push_back(vertex);
}

template <typename S>
void MergeGraphGenerator<S>::end()
{
}

template <typename S>
void MergeGraphGenerator<S>::postProcess(SB::Deps::SetVertex vertex)
{
}

template <typename S>
SB::PWLMap MergeGraphGenerator<S>::buildMap(SB::Set dom, int convert_offset, SB::Set dom_map, int graph_offset)
{
  int max_dims = ModelConfig::instance().symbols().maxDim();
  OrdRealCT constant_pwl_map;
  OrdRealCT::iterator constant_pwl_map_it = constant_pwl_map.begin();
  OrdRealCT slope_pwl_map;
  OrdRealCT::iterator slope_pwl_map_it = slope_pwl_map.begin();
  SB::Set edge_dom = generateMapDom(dom_map, dom_map, _edge_dom_offset, max_dims);
  int map_offset = _edge_dom_offset - 1;
  const bool N_1_RELATION = (dom.size() == 1) && (dom_map.size() > 1);
  assert(!dom.empty());
  for (Integer init : dom.minElem()) {
    Integer set_vertex_init = init + convert_offset - 1;
    Integer slope = 1;
    if (N_1_RELATION) {
      set_vertex_init = graph_offset + map_offset;
      slope = 0;
    }
    constant_pwl_map_it = constant_pwl_map.insert(constant_pwl_map_it, set_vertex_init - map_offset);
    slope_pwl_map_it = slope_pwl_map.insert(slope_pwl_map_it, slope);
    constant_pwl_map_it++;
    slope_pwl_map_it++;
  }
  addDims(max_dims, edge_dom.minElem().size(), constant_pwl_map, slope_pwl_map, 1);
  return buildPWLMap(constant_pwl_map, slope_pwl_map, edge_dom);
}
template <typename S>
SB::Deps::LMapExp MergeGraphGenerator<S>::buildLMapFromExp(IR::Expression exp)
{
  SB::Deps::Constants exp_constants;
  SB::Deps::Slopes exp_slopes;
  SB::Deps::InitValues exp_init_values;
  ExpressionList indexes = exp.indexes();
  for (Expression idx : indexes) {
    PWLMapValues pwl_map_values;
    pwl_map_values.apply(idx.expression());
    exp_constants.insert(pwl_map_values.constant());
    exp_slopes.insert(pwl_map_values.slope());
    exp_init_values.insert(pwl_map_values.constant());
  }
  return SB::Deps::LMapExp(exp_constants, exp_slopes, exp_init_values);
}

template <typename S>
SB::Deps::LMapExp MergeGraphGenerator<S>::buildLMapExp(Expression exp, IR::Expression use_exp)
{
  SB::Deps::LMapExp use_exp_map = buildLMapFromExp(use_exp);
  SB::Deps::LMapExp exp_map = buildLMapFromExp(exp);

  // We are dealing with expressions of different dimension, it could happen in merges.
  if (use_exp_map.slopes().size() > exp_map.slopes().size()) {
    exp_map.padDims(use_exp_map.slopes().size());
  }
  if (!exp_map.isEmpty() && !use_exp_map.isEmpty()) {
    exp_map = exp_map.compose(use_exp_map);
  }
  return exp_map;
}

template <typename S>
SB::Deps::SetVertex MergeGraphGenerator<S>::findAlgVertex(SB::Set variables)
{
  for (SB::Deps::SetVertex v : _nodes) {
    SB::Set range = v.range();
    if (!range.cap(variables).empty()) {
      return v;
    }
  }
  // A given subset should be included in one of the graph vertex, this should never happen.
  assert(false);
  return SB::Deps::SetVertex();
}

template <typename S>
SB::EdgeMaps MergeGraphGenerator<S>::generatePWLMaps(IntersectInfo inter_info, SB::Deps::SetVertex orig_ife_vertex,
                                                     SB::Deps::SetVertex ife_vertex, SB::Deps::VariableDep var_dep)
{
  SB::Set edge_dom = inter_info.edge.mapU().preImage(inter_info.intersection);
  SB::Set ifr_nodes = inter_info.edge.mapF().image(edge_dom);
  SB::Set ife_dom = var_dep.mapU().preImage(inter_info.intersection);
  SB::Set ife_nodes = var_dep.mapF().image(ife_dom);
  SB::Set ife_f_nodes = var_dep.mapF().wholeDom();
  SB::PWLMap mu = var_dep.mapU();
  SB::PWLMap mf = var_dep.mapF();
  if (ife_nodes.empty()) {
    SB::PWLMap mapF = _config.mapF();
    SB::PWLMap mapU = _config.mapU();
    SB::Set alg_ife_nodes = mapF.image(ife_dom);

    SB::Deps::SetVertex alg_eq_vertex = findAlgVertex(alg_ife_nodes);
    EquationTable algebraics = ModelConfig::instance().algebraics();
    Option<Equation> alg_eq = algebraics[alg_eq_vertex.index()];
    assert(alg_eq);
    VertexIt var_it = SB::Deps::findSetVertexByName(_lhs_dsc_graph, alg_eq->LHSVariable()->name());
    SB::Deps::SetVertex var_vertex = _lhs_dsc_graph[*var_it];
    SB::Set var_dom = var_vertex.range();
    SB::Set var_pre_image = mapU.preImage(var_dom);
    SB::Set all_alg_ife_nodes_pre = mapF.preImage(alg_ife_nodes);
    SB::Set images = mapU.image(all_alg_ife_nodes_pre).cap(var_dom);
    SB::Set alg_ife_nodes_pre = mapF.preImage(alg_ife_nodes);
    SB::Set pre_images = mapU.preImage(images).diff(alg_ife_nodes_pre);
    SB::Set orig_alg_images = mapF.image(pre_images);
    SB::Set f_dom = var_dep.fDom();
    SB::Set inf_eq_vars = mapU.image(f_dom);
    SB::Set alg_eq_set = mapF.preImage(orig_alg_images);
    SB::Set alg_eq_images = mapU.image(alg_eq_set).cap(inf_eq_vars);
    SB::Set inf_node_pre_image = mapU.preImage(alg_eq_images);
    SB::Set intersected_eqs = var_dep.fDom().cap(inf_node_pre_image);
    ife_nodes = var_dep.mapF().image(intersected_eqs);
  }
  SB::Set dom_nodes = (ifr_nodes.size() > ife_nodes.size()) ? ifr_nodes : ife_nodes;
  EdgeMaps maps;
  maps.F = buildMap(ifr_nodes, _graph[*inter_info.node].id() - inter_info.orig_node.id(), dom_nodes, _graph[*inter_info.node].id());
  maps.U = buildMap(ife_nodes, ife_vertex.id() - orig_ife_vertex.id(), dom_nodes, ife_vertex.id());
  Expression node = _config.exp(_config.getNode(orig_ife_vertex.index()));
  maps.map_exp = buildLMapExp(node, inter_info.edge.desc().exp());
  _edge_dom_offset += dom_nodes.size();
  return maps;
}

template <typename S>
void MergeGraphGenerator<S>::addEdges(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  Intersections inters = computeIntersections(var_dep.variables());
  if (!inters.empty()) {
    VertexIt ife_vertex_it = SB::Deps::findSetVertexByName(_graph, _config.nodeName(vertex.index()));
    SB::Deps::SetVertex ife_vertex = _graph[*ife_vertex_it];

    for (IntersectInfo inter_info : inters) {
      EdgeMaps maps = generatePWLMaps(inter_info, vertex, ife_vertex, var_dep);
      string edge_name = "E_" + to_string(_edge_dom_offset);
      LOG << "MapF: " << maps.F << endl;
      LOG << "MapU: " << maps.U << endl;
      Expression node = _config.exp(_config.getNode(vertex.index()));
      SB::Deps::SetEdge label(edge_name, maps.F, maps.U, SB::Deps::EdgeDesc(node, maps.map_exp));
      SB::Deps::Edge edge;
      bool b;
      SB::Deps::Vertex g_ife_vertex = *ife_vertex_it;
      SB::Deps::Vertex g_ifr_vertex = *inter_info.node;
      boost::tie(edge, b) = boost::add_edge(g_ifr_vertex, g_ife_vertex, _graph);
      _graph[edge] = label;
    }
  }
}

template <typename S>
void MergeGraphGenerator<S>::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  if (vertex.desc().type() == SB::Deps::VERTEX::Influencer) {
    addEdges(vertex, var_dep);
  }
}

template <typename S>
void MergeGraphGenerator<S>::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex)
{
}

template <typename S>
void MergeGraphGenerator<S>::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep,
                                    int index_shift)
{
  if (v_vertex.desc().type() != SB::Deps::VERTEX::Equation) {
    addEdges(v_vertex, var_dep);
  }
}

template <typename S>
void MergeGraphGenerator<S>::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map,
                                    SB::Deps::LMapExp use_map_exp, IR::Expression use_exp, SB::PWLMap def_map,
                                    SB::Deps::LMapExp def_map_exp, SB::Set intersection)
{
}

template <typename S>
void MergeGraphGenerator<S>::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge)
{
}

template <typename S>
SB::Deps::Graph MergeGraphGenerator<S>::def()
{
  return _graph;
}

template class MergeGraphGenerator<Deps::EQSelector>;

template class MergeGraphGenerator<Deps::EVSelector>;

}  // namespace Deps
}  // namespace MicroModelica
