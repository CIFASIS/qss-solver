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

#include <deps/sbg_graph/deps_graph.h>
#include <deps/sbg_graph/lmap_exp.h>
#include <deps/sbg_graph/pw_lmap.h>
#include <deps/sbg_graph/set.h>
#include <ir/equation.h>
#include <ir/expression.h>
#include <ir/event.h>
#include <util/symbol_table.h>

namespace SB {

typedef std::pair<Deps::Vertex, MicroModelica::IR::Equation> EquationDesc;
typedef std::list<EquationDesc> EquationDescList;
typedef std::map<std::string, int> Usage;
typedef std::map<std::string, Usage> EqUsage;
typedef std::map<std::string, Interval> DimRange;

struct EdgeMaps {
  PWLMap F;
  PWLMap U;
  Deps::LMapExp map_exp;
};

template <typename I>
struct StateSelector : public I {
  std::string name() { return I::name(); };
  bool selectVariable(MicroModelica::Util::Variable var) { return var.isState(); };
};

template <typename I>
struct DiscreteSelector : public I {
  std::string name() { return I::name(); };
  bool selectVariable(MicroModelica::Util::Variable var) { return var.isDiscrete(); };
};

Real getValue(MicroModelica::IR::Expression exp);

void addDims(size_t max_dim, size_t exp_dim, MultiInterval& intervals, int offset);

void addDims(size_t max_dim, size_t exp_dim, OrdRealCT& constants, OrdRealCT& slopes, int offset);

Set buildSet(MultiInterval intervals);

Set buildSet(MicroModelica::Util::Variable variable, int offset, size_t max_dim);

Set buildSet(MicroModelica::IR::Equation eq, string eq_id, int offset, size_t max_dim);

Deps::SetVertex createSetVertex(MicroModelica::Util::Variable variable, int& offset, size_t max_dim, SB::Deps::VERTEX::Type type);

Deps::SetVertex createSetVertex(MicroModelica::IR::Equation eq, int& offset, size_t max_dim, SB::Deps::VERTEX::Type type, EqUsage& usage);

list<Deps::SetVertex> createSetVertex(MicroModelica::IR::Event ev, int& offset, size_t max_dim, SB::Deps::VERTEX::Type type, EqUsage& usage, MicroModelica::IR::STATEMENT::AssignTerm search);

/**
 * @brief Add the offset to a given equation domain.
 *
 * @param dom
 * @param offset
 * @return SBG::Set The new set with all the offsets applied.
 */
Set generateMapDom(Set dom, Set unk_dom, int offset, size_t max_dim);

Deps::Vertex addVertex(Deps::SetVertex V, Deps::Graph& graph);

Deps::Vertex addVertex(std::string vertex_name, Set set, Deps::Graph& graph);

void addEquation(MicroModelica::IR::Equation eq, string id, Set set, Deps::Graph& graph);

MicroModelica::IR::Equation getEquation(Deps::SetVertex n,
                                        MicroModelica::IR::EquationTable eqs);

MicroModelica::IR::Equation getEquation(Deps::SetVertex n);

MicroModelica::IR::Event getEvent(Deps::SetVertex n);

PWLMap buildPWLMap(OrdRealCT constants, OrdRealCT slopes, Set dom);

EdgeMaps generatePWLMaps(MicroModelica::IR::Expression exp, Set dom, Set unk_dom, int offset, string eq_id, size_t max_dim,
                         EqUsage eq_usage);

void buildEdge(MicroModelica::IR::Expression builder, Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset,
               SB::Deps::EDGE::Type type);

template<class N>
void buildEdges(Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset,
                     N nodes, SB::Deps::EDGE::Type type)
{
  list<MicroModelica::IR::Expression> exps = nodes.getExpressions(graph[e], type);
  for(MicroModelica::IR::Expression exp : exps) {
    buildEdge(exp, e, v, graph, max_dim, eq_usage, offset, type);
  }
};

template<class N>
void computeOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                        int& offset, N nodes)
{
  for (Vertex E : e_nodes) {
    for (Vertex V : v_nodes) {
      buildEdges(E, V, graph, max_dim, eq_usage, offset, nodes, SB::Deps::EDGE::Output);
    }
  }
};

template<class N>
void computeInputOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                             int& offset, N nodes)
{
  for (Vertex E : e_nodes) {
    for (Vertex V : v_nodes) {
      buildEdges(E, V, graph, max_dim, eq_usage, offset, nodes, SB::Deps::EDGE::Output);
      buildEdges(E, V, graph, max_dim, eq_usage, offset, nodes, SB::Deps::EDGE::Input);
    }
  }
};

namespace Deps {
void updateVisited(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, bool visited);

void updateNumDeps(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, int num_deps);

VertexIt findSetVertex(SB::Deps::Graph& graph, Set matched);

VertexIt findSetVertexByName(SB::Deps::Graph& graph, string name);

std::list<Edge> inputEdges(SB::Deps::Graph& graph, string name);

Set wholeVertex(SB::Deps::Graph& graph, Set matched_subset);

}  // namespace Deps
}  // namespace SB