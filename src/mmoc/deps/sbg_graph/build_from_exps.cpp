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

#include "build_from_exps.h"

#include <deps/sbg_graph/interval.h>
#include <util/model_config.h>
#include <util/logger.h>
#include <util/visitors/occurs.h>
#include <util/visitors/pwl_map_values.h>

namespace SB {

using namespace Deps;

using namespace MicroModelica::IR;
using namespace MicroModelica::Util;

void addDims(size_t max_dim, size_t exp_dim, MultiInterval& intervals, int offset)
{
  if (max_dim > exp_dim) {
    for (size_t i = exp_dim; i < max_dim; i++) {
      Interval interval(offset, 1, offset);
      intervals.addInter(interval);
    }
  }
}

void addDims(size_t max_dim, size_t exp_dim, OrdRealCT& constants, OrdRealCT& slopes)
{
  OrdRealCT::iterator constant_it = constants.end();
  OrdRealCT::iterator slope_it = slopes.end();
  if (max_dim > exp_dim) {
    for (size_t i = exp_dim; i < max_dim; i++) {
      constant_it = constants.insert(constant_it, 0);
      slope_it = slopes.insert(slope_it, 0);
    }
  }
}

Set buildSet(MultiInterval intervals)
{
  Set set;
  AtomSet node_set(intervals);
  set.addAtomSet(node_set);
  return set;
}

Set buildSet(Variable variable, int offset, size_t max_dim)
{
  unsigned int dims = variable.dimensions();
  MultiInterval variable_intervals;
  if (dims) {
    for (unsigned int i = 0; i < dims; i++) {
      Real end = variable.size(i) + offset - 1;
      Interval interval(offset, 1, end);
      variable_intervals.addInter(interval);
    }
    addDims(max_dim, dims, variable_intervals, offset);
  } else {
    addDims(max_dim, 0, variable_intervals, offset);
  }
  return buildSet(variable_intervals);
}

Set buildSet(Equation eq, string eq_id, int offset, size_t max_dim, EqUsage& eq_usage)
{
  Usage usage;
  DimRange dim_range;
  MultiInterval equation_intervals;
  if (eq.hasRange()) {
    RangeDefinitionTable ranges = eq.range()->definition();
    for (auto range : ranges) {
      Real lower = range.second.begin();
      Real step = range.second.step();
      Real upper = range.second.end();
      Real end = offset + upper - lower;
      Interval interval(offset, step, end);
      LOG << "Variable: " << range.first << " Lower: " << lower << endl;
      usage[range.first] = lower;
      dim_range[range.first] = interval;
    }
    Expression lhs = eq.lhs();
    ExpressionList indexes = lhs.indexes();
    for (Expression idx : indexes) {
      PWLMapValues pwl_map_values;
      pwl_map_values.apply(idx.expression());
      if (pwl_map_values.isScalar()) { // Scalar index
        Interval interval(offset, 1, offset);
        equation_intervals.addInter(interval);
      } else {
        Interval interval = dim_range[pwl_map_values.variable()];
        equation_intervals.addInter(interval);
      }
    }
    addDims(max_dim, indexes.size(), equation_intervals, offset);
    eq_usage[eq_id] = usage;
  } else {
    addDims(max_dim, 0, equation_intervals, offset);
  }
  return buildSet(equation_intervals);
}

/**
 * @brief Add the offset to a given equation domain.
 *
 * @param dom
 * @param offset
 * @return SBG::Set The new set with all the offsets applied.
 */
Set generateMapDom(Set dom, Set unk_dom, int offset, size_t max_dim)
{
  MultiInterval edge_set_intervals;
  UnordAtomSet atom_sets = dom.atomicSets();
  for (AtomSet atom_set : atom_sets) {
    MultiInterval dom_intervals = atom_set.atomicSets();
    for (Interval inter : dom_intervals.intervals()) {
      Real end = inter.size() + offset - 1;
      edge_set_intervals.addInter(Interval(offset, inter.step(), end));
    }
    addDims(max_dim, dom_intervals.intervals().size(), edge_set_intervals, offset);
  }
  return buildSet(edge_set_intervals);
}

Vertex addVertex(Deps::SetVertex V, Deps::Graph& graph)
{
  Vertex v = boost::add_vertex(graph);
  graph[v] = V;
  return v;
}

Vertex addVertex(std::string vertex_name, Set set, Deps::Graph& graph)
{
  Deps::SetVertex V(vertex_name, set);
  Vertex v = boost::add_vertex(graph);
  graph[v] = V;
  return v;
}

Deps::SetVertex createSetVertex(Variable variable, int& offset, size_t max_dim, VERTEX::Type type)
{
  Set range = buildSet(variable, offset, max_dim);
  Deps::SetVertex node = Deps::SetVertex(variable.name(), offset, range, Deps::VertexDesc(type, variable));
  offset += range.size();
  return node;
}

Deps::SetVertex createSetVertex(Equation eq, int& offset, size_t max_dim, VERTEX::Type type, EqUsage& usage)
{
  std::stringstream eq_name;
  eq_name << "EQ_" << eq.id();
  Set range = buildSet(eq, eq_name.str(), offset, max_dim, usage);
  Deps::SetVertex node = Deps::SetVertex(eq_name.str(), offset, range, eq.id(), Deps::VertexDesc(type));
  offset += range.size();
  return node;
}

void addEquation(Equation eq, string id, Set set, Deps::Graph& graph, EquationDescList& F)
{
  F.push_back(make_pair(addVertex(id, set, graph), eq));
  return;
}

PWLMap buildPWLMap(OrdRealCT constants, OrdRealCT slopes, Set dom)
{
  LMap lmap(slopes, constants);

  OrdCT<Set> doms;
  doms.insert(doms.end(), dom);

  OrdCT<LMap> maps;
  maps.insert(maps.end(), lmap);
  PWLMap map(doms, maps);
  return map;
}

EdgeMaps generatePWLMaps(Expression exp, Set dom, Set unk_dom, int offset, string eq_id, size_t max_dim, EqUsage eq_usage)
{
  assert(exp.isReference());
  EdgeMaps maps;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  OrdRealCT constant_pwl_map_u;
  OrdRealCT::iterator constant_pwl_map_u_it = constant_pwl_map_u.begin();
  OrdRealCT slope_pwl_map_u;
  OrdRealCT::iterator slope_pwl_map_u_it = slope_pwl_map_u.begin();
  OrdRealCT constant_pwl_map_f;
  OrdRealCT::iterator constant_pwl_map_f_it = constant_pwl_map_f.begin();
  OrdRealCT slope_pwl_map_f;
  OrdRealCT::iterator slope_pwl_map_f_it = slope_pwl_map_f.begin();

  ExpressionList indexes = exp.indexes();
  OrdIntegerCT init_elems = unk_dom.minElem();
  assert(init_elems.size() == indexes.size() || indexes.size() == 0);
  OrdIntegerCT::iterator min_elem = init_elems.begin();
  Constants exp_constants;
  Slopes exp_slopes;
  InitValues exp_init_values;
  Set map_dom = generateMapDom(dom, unk_dom, offset, max_dim);
  int map_offset = offset - 1;

  for (Expression idx : indexes) {
    Integer set_vertex_init = *min_elem - 1;
    PWLMapValues pwl_map_values;
    pwl_map_values.apply(idx.expression());
    Usage usage = eq_usage[eq_id];
    LOG << "Looking for: " << pwl_map_values.variable() << endl;
    int range_init_value = usage[pwl_map_values.variable()];
    int set_vertex_offset = pwl_map_values.slope() * offset;
    LOG << "Init: " << range_init_value << endl;
    int map_first_value = pwl_map_values.constant() + pwl_map_values.slope() * range_init_value + set_vertex_init;
    LOG << "Map first: " << map_first_value << endl;
    constant_pwl_map_u_it = constant_pwl_map_u.insert(constant_pwl_map_u_it, map_first_value - set_vertex_offset);
    slope_pwl_map_u_it = slope_pwl_map_u.insert(slope_pwl_map_u_it, pwl_map_values.slope());
    exp_constants.insert(pwl_map_values.constant());
    exp_slopes.insert(pwl_map_values.slope());
    if (range_init_value == 0) { // Scalar index
      exp_init_values.insert(pwl_map_values.constant());
    } else {
      exp_init_values.insert(offset);
    }
    constant_pwl_map_u_it++;
    slope_pwl_map_u_it++;
    min_elem++;
  }
  if (indexes.empty()) {  // Scalar variable.
    Integer set_vertex_init = *min_elem - 1;
    constant_pwl_map_u_it = constant_pwl_map_u.insert(constant_pwl_map_u_it, -map_offset + set_vertex_init);
    slope_pwl_map_u_it = slope_pwl_map_u.insert(slope_pwl_map_u_it, 1);
    constant_pwl_map_u_it++;
    slope_pwl_map_u_it++;
    addDims(max_dim, 1, constant_pwl_map_u, slope_pwl_map_u);
  } else {
    addDims(max_dim, indexes.size(), constant_pwl_map_u, slope_pwl_map_u);
  }
  for (Integer init : dom.minElem()) {
    Integer set_vertex_init = init - 1;
    constant_pwl_map_f_it = constant_pwl_map_f.insert(constant_pwl_map_f_it, -map_offset + set_vertex_init);
    slope_pwl_map_f_it = slope_pwl_map_f.insert(slope_pwl_map_f_it, 1);
    constant_pwl_map_f_it++;
    slope_pwl_map_f_it++;
  }
  addDims(max_dim, dom.minElem().size(), constant_pwl_map_f, slope_pwl_map_f);
  maps.F = buildPWLMap(constant_pwl_map_f, slope_pwl_map_f, map_dom);
  maps.U = buildPWLMap(constant_pwl_map_u, slope_pwl_map_u, map_dom);
  maps.map_exp = LMapExp(exp_constants, exp_slopes, exp_init_values);
  return maps;
}

Equation getEquation(Deps::SetVertex n)
{
  EquationTable equations = ModelConfig::instance().derivatives();
  ;
  if (n.desc().type() == SB::Deps::VERTEX::Equation) {
    equations = ModelConfig::instance().algebraics();
  }
  Option<Equation> eq = equations[n.index()];
  assert(eq);
  return eq.get();
}

void buildEdge(Expression builder, Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset,
               SB::Deps::EDGE::Type type = SB::Deps::EDGE::Output)
{
  Deps::SetVertex e_vertex = graph[e];
  Deps::SetVertex v_vertex = graph[v];
  Set e_dom = e_vertex.range();
  Set v_dom = v_vertex.range();
  Occurs matching_exps(v_vertex.name());
  matching_exps.apply(builder.expression());
  set<Expression> matched_exps = matching_exps.occurrences();
  LOG << "Matched exps for: " << v_vertex.name() << " in " << builder << " of " << e_vertex.name() << endl;
  LOG << "Equation dom: " << e_dom << endl;
  for (Expression exp : matched_exps) {
    LOG << "Expression: " << exp << endl;
    EdgeMaps maps = generatePWLMaps(exp, e_dom, v_dom, offset, e_vertex.name(), max_dim, eq_usage);
    offset += e_dom.size();
    string edge_name = "E_" + to_string(1);
    LOG << "MapF: " << maps.F << endl;
    LOG << "MapU: " << maps.U << endl;
    Deps::SetEdge label(edge_name, maps.F, maps.U, Deps::EdgeDesc(exp, maps.map_exp));
    Deps::Edge edge;
    bool b;
    boost::tie(edge, b) = (type == SB::Deps::EDGE::Output) ? boost::add_edge(e, v, graph) : boost::add_edge(v, e, graph);
    graph[edge] = label;
  }
}

void buildOutputEdge(Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset)
{
  Expression output = getEquation(graph[e]).rhs();
  buildEdge(output, e, v, graph, max_dim, eq_usage, offset);
}

void buildInputEdge(Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset)
{
  Expression input = getEquation(graph[e]).lhs();
  buildEdge(input, e, v, graph, max_dim, eq_usage, offset, SB::Deps::EDGE::Input);
}

void computeOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                        int& offset)
{
  for (Vertex E : e_nodes) {
    for (Vertex V : v_nodes) {
      buildOutputEdge(E, V, graph, max_dim, eq_usage, offset);
    }
  }
}

void computeInputOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                             int& offset)
{
  for (Vertex E : e_nodes) {
    for (Vertex V : v_nodes) {
      buildOutputEdge(E, V, graph, max_dim, eq_usage, offset);
      buildInputEdge(E, V, graph, max_dim, eq_usage, offset);
    }
  }
}

namespace Deps {
void updateVisited(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, bool visited)
{
  SB::Deps::VertexDesc update_desc = graph[vertex].desc();
  update_desc.setVisited(true);
  graph[vertex].updateDesc(update_desc);
}

void updateNumDeps(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, int num_deps)
{
  SB::Deps::VertexDesc update_desc = graph[vertex].desc();
  update_desc.setNumDeps(num_deps);
  graph[vertex].updateDesc(update_desc);
}

}  // namespace Deps
}  // namespace SB
