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
#include <util/logger.h>
#include <util/model_config.h>
#include <util/visitors/is_constant_index.h>
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

void addDims(size_t max_dim, size_t exp_dim, OrdRealCT& constants, OrdRealCT& slopes, int offset)
{
  OrdRealCT::iterator constant_it = constants.end();
  OrdRealCT::iterator slope_it = slopes.end();
  if (max_dim > exp_dim) {
    for (size_t i = exp_dim; i < max_dim; i++) {
      constant_it = constants.insert(constant_it, offset);
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

void setupUsage(Range range, Usage& usage, DimRange& dim_range, int offset)
{
  RangeDefinitionTable ranges = range.definition();
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
}

void setupIntervals(Expression exp, MultiInterval& intervals, DimRange& dim_range, int offset, size_t max_dim,
                    Option<Range> range = Option<Range>())
{
  ExpressionList indexes = exp.indexes();
  size_t index_size = indexes.size();
  if (range && range->hasMergedDims() && !indexes.empty()) {
    vector<string> used_vars = Index(exp).variables();
    RangeDefinitionTable::iterator it;
    RangeDefinitionTable ranges = range->definition();
    for (RangeDefinition r = ranges.begin(it); !ranges.end(it); r = ranges.next(it)) {
      string key = ranges.key(it);
      std::vector<string>::iterator find_var = std::find(used_vars.begin(), used_vars.end(), key);
      if (find_var == used_vars.end()) {
        Interval interval = dim_range[key];
        intervals.addInter(interval);
        index_size++;
      }
    }
  }
  vector<string> used_vars;
  for (Expression idx : indexes) {
    PWLMapValues pwl_map_values;
    pwl_map_values.apply(idx.expression());
    if (pwl_map_values.isScalar() && !range) {  // Scalar index
      Interval interval(offset, 1, offset);
      intervals.addInter(interval);
    } else {
      std::vector<string>::iterator find_var = std::find(used_vars.begin(), used_vars.end(), pwl_map_values.variable());
      if (find_var == used_vars.end()) {
        Interval interval = dim_range[pwl_map_values.variable()];
        intervals.addInter(interval);
        used_vars.push_back(pwl_map_values.variable());
      }
    }
  }
  if (indexes.empty() && range) {
    RangeDefinitionTable ranges = range->definition();
    for (auto range : ranges) {
      Real lower = range.second.begin();
      Real step = range.second.step();
      Real upper = range.second.end();
      Real end = offset + upper - lower;
      Interval interval(offset, step, end);
      intervals.addInter(interval);
    }
    index_size = ranges.size();
  }
  addDims(max_dim, index_size, intervals, offset);
}

bool checkFixedRange(Option<Range> range)
{
  if (range) {
    return range->fixed();
  }
  return true;
}

bool checkExpressionIndex(Expression exp)
{
  CheckIndexExpression check_parameters;
  check_parameters.apply(exp.expression());
  return check_parameters.hasParameters();
}

Set buildSet(Equation eq, string eq_id, int offset, size_t max_dim, EqUsage& eq_usage)
{
  if (checkExpressionIndex(eq.lhs())) {
    return SB::Set();
  }
  Usage usage;
  DimRange dim_range;
  MultiInterval equation_intervals;
  if (eq.hasRange() && checkFixedRange(eq.range())) {
    setupUsage(eq.range().get(), usage, dim_range, offset);
    Expression lhs = eq.lhs();
    setupIntervals(lhs, equation_intervals, dim_range, offset, max_dim);
    eq_usage[eq_id] = usage;
  } else {
    addDims(max_dim, 0, equation_intervals, offset);
  }
  return buildSet(equation_intervals);
}

Set buildSet(Expression exp, string stm_id, int offset, size_t max_dim, EqUsage& eq_usage, Option<Range> range)
{
  if (checkExpressionIndex(exp)) {
    return SB::Set();
  }
  Usage usage;
  DimRange dim_range;
  MultiInterval ev_intervals;
  if (range) {
    setupUsage(range.get(), usage, dim_range, offset);
    setupIntervals(exp, ev_intervals, dim_range, offset, max_dim, range);
    eq_usage[stm_id] = usage;
  } else {
    addDims(max_dim, 0, ev_intervals, offset);
  }
  return buildSet(ev_intervals);
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
      Integer end = inter.size() + offset - 1;
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

Option<Deps::SetVertex> createSetVertex(Equation eq, int& offset, size_t max_dim, VERTEX::Type type, EqUsage& usage)
{
  std::stringstream eq_name;
  eq_name << "EQ_" << eq.type() << "_" << eq.id();
  Set range = buildSet(eq, eq_name.str(), offset, max_dim, usage);
  if (range.empty()) {
    return Option<Deps::SetVertex>();
  }
  Deps::SetVertex node = Deps::SetVertex(eq_name.str(), offset, range, eq.id(), Deps::VertexDesc(type));
  offset += range.size();
  return node;
}

int handlerDim(StatementTable stms)
{
  StatementTable::iterator stm_it;
  int stm_dims = 0;
  for (Statement stm = stms.begin(stm_it); !stms.end(stm_it); stm = stms.next(stm_it)) {
    ExpressionList assignments = stm.assignments(STATEMENT::AssignTerm::LHS);
    for (Expression asg : assignments) {
      if (stm.isForStatement()) {
        int for_dims = stm.range()->dim();
        if (stm_dims < for_dims) {
          stm_dims = for_dims;
        }
      }
    }
  }
  return stm_dims;
}

int eventDim()
{
  int max_event_dim = 0;
  EventTable::iterator ev_it;
  EventTable events = ModelConfig::instance().events();
  for (Event ev = events.begin(ev_it); !events.end(ev_it); ev = events.next(ev_it)) {
    if (ev.hasRange()) {
      int handler_dim = ev.range()->dim() + handlerDim(ev.positiveHandler());
      if (max_event_dim < handler_dim) {
        max_event_dim = handler_dim;
      }
      handler_dim = ev.range()->dim() + handlerDim(ev.negativeHandler());
      if (max_event_dim < handler_dim) {
        max_event_dim = handler_dim;
      }
    }
  }
  return max_event_dim;
}

int maxDim()
{
  int max_dims = ModelConfig::instance().symbols().maxDim();
  int event_dims = eventDim();
  if (max_dims < event_dims) {
    max_dims = event_dims;
  }
  return max_dims;
}

void addStatements(StatementTable stms, list<Deps::SetVertex>& nodes, Option<Range> ev_range, int& offset, size_t max_dim,
                   VERTEX::Type type, EqUsage& usage, MicroModelica::IR::STATEMENT::AssignTerm search, int id, string token)
{
  StatementTable::iterator stm_it;
  int stm_count = 1;
  const bool FIXED_RANGE = checkFixedRange(ev_range);
  for (Statement stm = stms.begin(stm_it); !stms.end(stm_it); stm = stms.next(stm_it)) {
    int asg_nbr = 1;
    ExpressionList assignments = stm.assignments(search);
    for (Expression asg : assignments) {
      std::stringstream ev_name;
      Option<Range> stm_range = ev_range;
      if (stm.isForStatement()) {
        if (!stm_range) {
          stm_range = Range();
        }
        stm_range->merge(stm.range().get());
      }
      const bool STM_FIXED_RANGE = (stm.isForStatement()) ? checkFixedRange(stm.range()) : true;
      if (FIXED_RANGE && STM_FIXED_RANGE) {
        ev_name << "EV_" << id << "_" << token << "_" << stm_count << "_" << asg_nbr++;
        Set range = buildSet(asg, ev_name.str(), offset, max_dim, usage, stm_range);
        Deps::SetVertex node = Deps::SetVertex(ev_name.str(), offset, range, id, Deps::VertexDesc(type));
        offset += range.size();
        nodes.push_back(node);
      }
    }
    stm_count++;
  }
}

list<Deps::SetVertex> createSetVertex(Event ev, int& offset, size_t max_dim, VERTEX::Type type, EqUsage& usage,
                                      MicroModelica::IR::STATEMENT::AssignTerm search)
{
  list<Deps::SetVertex> nodes;
  addStatements(ev.positiveHandler(), nodes, ev.range(), offset, max_dim, type, usage, search, ev.id(), "POS");
  addStatements(ev.negativeHandler(), nodes, ev.range(), offset, max_dim, type, usage, search, ev.id(), "NEG");
  return nodes;
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
  assert(init_elems.size() == indexes.size() || indexes.size() == 0 || indexes.size() < max_dim);
  OrdIntegerCT::iterator min_elem = init_elems.begin();
  Integer unk_offset = *min_elem;
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
    if (range_init_value == 0) {  // Scalar index
      exp_init_values.insert(pwl_map_values.constant());
    } else {
      exp_init_values.insert(range_init_value);
    }
    constant_pwl_map_u_it++;
    slope_pwl_map_u_it++;
    min_elem++;
  }
  if (indexes.empty()) {  // Scalar variable.
    Integer set_vertex_init = *min_elem;
    constant_pwl_map_u_it = constant_pwl_map_u.insert(constant_pwl_map_u_it, set_vertex_init);
    slope_pwl_map_u_it = slope_pwl_map_u.insert(slope_pwl_map_u_it, 0);
    constant_pwl_map_u_it++;
    slope_pwl_map_u_it++;
    addDims(max_dim, 1, constant_pwl_map_u, slope_pwl_map_u, unk_offset);
  } else {
    addDims(max_dim, indexes.size(), constant_pwl_map_u, slope_pwl_map_u, unk_offset);
  }
  for (Integer init : dom.minElem()) {
    Integer set_vertex_init = init - 1;
    constant_pwl_map_f_it = constant_pwl_map_f.insert(constant_pwl_map_f_it, -map_offset + set_vertex_init);
    slope_pwl_map_f_it = slope_pwl_map_f.insert(slope_pwl_map_f_it, 1);
    constant_pwl_map_f_it++;
    slope_pwl_map_f_it++;
  }
  addDims(max_dim, dom.minElem().size(), constant_pwl_map_f, slope_pwl_map_f, offset);
  maps.F = buildPWLMap(constant_pwl_map_f, slope_pwl_map_f, map_dom);
  maps.U = buildPWLMap(constant_pwl_map_u, slope_pwl_map_u, map_dom);
  maps.map_exp = LMapExp(exp_constants, exp_slopes, exp_init_values);
  return maps;
}

Equation getEquation(Deps::SetVertex n, EquationTable eqs)
{
  EquationTable equations = eqs;
  if (n.desc().type() == SB::Deps::VERTEX::Equation) {
    equations = ModelConfig::instance().algebraics();
  }
  Option<Equation> eq = equations[n.index()];
  assert(eq);
  return eq.get();
}

Event getEvent(Deps::SetVertex n)
{
  EventTable events = ModelConfig::instance().events();
  Option<Event> ev = events[n.index()];
  assert(ev);
  return ev.get();
}

Equation getEquation(Deps::SetVertex n) { return getEquation(n, MicroModelica::Util::ModelConfig::instance().derivatives()); }

void buildEdge(Expression builder, Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset,
               SB::Deps::EDGE::Type type)
{
  Deps::SetVertex e_vertex = graph[e];
  Deps::SetVertex v_vertex = graph[v];
  Set e_dom = e_vertex.range();
  Set v_dom = v_vertex.range();
  Occurs matching_exps(v_vertex.name());
  matching_exps.apply(builder.expression());
  set<Expression> matched_exps = matching_exps.occurrences();
  LOG << "Matched exps for: " << v_vertex.name() << " in " << builder << " of " << e_vertex.name() << endl;
  for (Expression exp : matched_exps) {
    LOG << "Expression: " << exp << endl;
    LOG << "Equation dom: " << e_dom << endl;
    LOG << "Variable dom: " << v_dom << endl;
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

VertexIt findSetVertex(SB::Deps::Graph& graph, Set matched)
{
  // Find the set-vertex where matched subset is included
  VertexIt vi_start, vi_end;
  boost::tie(vi_start, vi_end) = vertices(graph);

  for (; vi_start != vi_end; ++vi_start) {
    SetVertex v = graph[*vi_start];
    Set vs = v.range();
    Set inter = vs.cap(matched);
    if (!inter.empty()) {
      return vi_start;
    }
  }
  // A given subset should be included in one of the graph vertex, this should never happen.
  assert(false);
  return vi_start;
}

VertexIt findSetVertexByName(SB::Deps::Graph& graph, string name)
{
  // Find the set-vertex where matched subset is included
  VertexIt vi_start, vi_end;
  boost::tie(vi_start, vi_end) = vertices(graph);

  for (; vi_start != vi_end; ++vi_start) {
    SetVertex v = graph[*vi_start];
    if (v.name() == name) {
      return vi_start;
    }
  }
  // A given subset should be included in one of the graph vertex, this should never happen.
  assert(false);
  return vi_start;
}

std::list<Edge> inputEdges(SB::Deps::Graph& graph, string name)
{
  // Find the set-vertex where matched subset is included
  VertexIt vi_start, vi_end;
  boost::tie(vi_start, vi_end) = vertices(graph);
  std::list<Edge> edges;

  for (; vi_start != vi_end; ++vi_start) {
    SetVertex v = graph[*vi_start];
    boost::graph_traits<SB::Deps::Graph>::out_edge_iterator edge, out_edge_end;
    // As a second step, look for all the input edges that arrives to the discrete variable.
    for (boost::tie(edge, out_edge_end) = out_edges(*vi_start, graph); edge != out_edge_end; ++edge) {
      SB::Deps::SetEdge edge_label = graph[*edge];
      SB::Deps::Vertex target = boost::target(*edge, graph);
      SB::Deps::SetVertex target_set_vertex = graph[target];
      if (target_set_vertex.name() == name) {
        edges.push_back(*edge);
      }
    }
  }
  return edges;
}

Set wholeVertex(SB::Deps::Graph& graph, Set matched_subset)
{
  Set whole_vertex;
  // Find the set-vertex where the matched subset is included
  VertexIt matched_vertex = findSetVertex(graph, matched_subset);
  SetVertex v = graph[*matched_vertex];
  SB::Set r = v.range();
  return v.range();
}

}  // namespace Deps
}  // namespace SB
