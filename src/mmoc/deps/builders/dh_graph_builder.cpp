/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "dh_graph_builder.h"

#include <util/model_config.h>
#include <util/logger.h>
#include <util/util_types.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

DHGraphBuilder::DHGraphBuilder(EventTable& events, EquationTable& algebraics, STATEMENT::AssignTerm search,
                               DHGRAPHBUILDER::IfrType ifr_type)
    : _statementDescriptors(),
      _variableDescriptors(),
      _events(events),
      _algebraics(algebraics),
      _search(search),
      _ifr_type(ifr_type),
      _pushed_vars()
{
}

void DHGraphBuilder::addStatements(StatementTable stms, DepsGraph& graph, Expression exp, int id, Option<Range> range)
{
  StatementTable::iterator stm_it;
  for (Statement stm = stms.begin(stm_it); !stms.end(stm_it); stm = stms.next(stm_it)) {
    ExpressionList lhs_discretes = stm.lhsDiscretes();
    ExpressionList assignments = stm.assignments(_search);
    //    assert(assignments.size() == lhs_discretes.size());
    //ExpressionList::iterator it = lhs_discretes.begin();
    for (Expression e : assignments) {
      VertexProperty vp = VertexProperty();
      StatementVertex sv = StatementVertex();
      vp.setType(VERTEX::Statement);
      sv.setExp(e);
      sv.setEvent(exp);

      if (stm.isForStatement()) {
        assert(stm.range());
        sv.setRange(stm.range().get());
      } else if (range) {
        sv.setRange(range.get());
      }
      //      vp.stm.lhs = *it;
      vp.setId(id);
      //      it++;
      vp.setStm(sv);
      _statementDescriptors.push_back(add_vertex(vp, graph));
    }
  }
}

bool DHGraphBuilder::addNewEvent(VertexProperty source, VertexProperty sink)
{
  string var = source.var().name();
  vector<int> ids = _pushed_vars[var]; 
  if (std::find(ids.begin(), ids.end(), sink.id()) == ids.end()) {
    ids.push_back(sink.id());
    _pushed_vars[var] = ids;
    return true;
  }
  return false;
}

DepsGraph DHGraphBuilder::build()
{
  DepsGraph graph;
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    VertexProperty vp = VertexProperty();
    bool add_influencer = (_ifr_type == DHGRAPHBUILDER::State) ? var.isState() : var.isDiscrete();
    if (add_influencer) {
      vp.setType(VERTEX::Influencer);
      vp.setVar(var);
      _variableDescriptors.push_back(add_vertex(vp, graph));
    } else if (var.isAlgebraic()) {
      vp.setType(VERTEX::Algebraic);
      vp.setVar(var);
      _variableDescriptors.push_back(add_vertex(vp, graph));
    }
  }
  EventTable::iterator ev_it;
  for (Event ev = _events.begin(ev_it); !_events.end(ev_it); ev = _events.next(ev_it)) {
    Expression exp = ev.exp();
    Option<Range> range = ev.range();
    int id = ev.id();
    addStatements(ev.positiveHandler(), graph, exp, id, range);
    addStatements(ev.negativeHandler(), graph, exp, id, range);
    VertexProperty icee = VertexProperty();
    icee.setType(VERTEX::Influencee);
    icee.setId(id);
    _eventDescriptors.push_back(add_vertex(icee, graph));
  }
  EquationTable::iterator eq_it;
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    VertexProperty vp = VertexProperty();
    vp.setType(VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    _statementDescriptors.push_back(add_vertex(vp, graph));
  }

  foreach_(EqVertex sink, _statementDescriptors)
  {
    foreach_(IfrVertex source, _variableDescriptors)
    {
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink]);
      if (edge.exists()) {
        if (addNewEvent(graph[source], graph[sink])) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            LOG << "Adding edge from var: " << graph[source].var() << " to event: " << graph[sink].id() << endl;
            add_edge(source, sink, lbl, graph);
          }
        }
      }
      // Check LHS too if we are working with algebraics.
      if (graph[source].type() == VERTEX::Algebraic && graph[sink].eq().type() == EQUATION::Algebraic) {
        edge = GenerateEdge(graph[source], graph[sink], EDGE::Input);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            add_edge(sink, source, lbl, graph);
          }
        }
      }
    }
  }
  foreach_(EqVertex sink, _statementDescriptors)
  {
    foreach_(IfeVertex source, _eventDescriptors)
    {
      if (graph[sink].type() == VERTEX::Statement) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink]);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            LOG << "Adding edge from event: " << graph[source].id() << " to var: " << graph[sink].var() << endl;
            add_edge(sink, source, lbl, graph);
          }
        }
      }
    }
  }
  return graph;
}
}  // namespace Deps
}  // namespace MicroModelica