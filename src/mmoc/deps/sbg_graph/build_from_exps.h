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

Real getValue(MicroModelica::IR::Expression exp);

void addDims(size_t max_dim, size_t exp_dim, MultiInterval& intervals, int offset);

void addDims(size_t max_dim, size_t exp_dim, OrdRealCT& constants, OrdRealCT& slopes);

Set buildSet(MultiInterval intervals);

Set buildSet(MicroModelica::Util::Variable variable, int offset, size_t max_dim);

Set buildSet(MicroModelica::IR::Equation eq, string eq_id, int offset, size_t max_dim);

Deps::SetVertex createSetVertex(MicroModelica::Util::Variable variable, int& offset, size_t max_dim, SB::Deps::VERTEX::Type type);

Deps::SetVertex createSetVertex(MicroModelica::IR::Equation eq, int& offset, size_t max_dim, SB::Deps::VERTEX::Type type, EqUsage& usage);

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

MicroModelica::IR::Equation getEquation(Deps::SetVertex n);

PWLMap buildPWLMap(OrdRealCT constants, OrdRealCT slopes, Set dom);

EdgeMaps generatePWLMaps(MicroModelica::IR::Expression exp, Set dom, Set unk_dom, int offset, string eq_id, size_t max_dim,
                         EqUsage eq_usage);

void computeOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                        int& offset);

void computeInputOutputEdges(list<Deps::Vertex> e_nodes, list<Deps::Vertex> v_nodes, Deps::Graph graph, size_t max_dim, EqUsage eq_usage,
                             int& offset);

void buildOutputEdge(Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset);

void buildOutputEdge(Deps::Vertex e, Deps::Vertex v, Deps::Graph graph, size_t max_dim, EqUsage eq_usage, int& offset);

namespace Deps {
void updateVisited(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, bool visited);

void updateNumDeps(SB::Deps::Graph& graph, SB::Deps::Vertex vertex, int num_deps);

}  // namespace Deps
}  // namespace SB