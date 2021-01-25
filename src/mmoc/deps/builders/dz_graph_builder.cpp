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

#include "dz_graph_builder.h"

#include "../../util/model_config.h"
#include "../../util/util_types.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

DZGraphBuilder::DZGraphBuilder(EventTable &events, EquationTable &algebraics)
    : _equationDescriptors(), _variableDescriptors(), _events(events), _algebraics(algebraics)
{
}

DepsGraph DZGraphBuilder::build()
{
  DepsGraph graph;
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    VertexProperty vp = VertexProperty();
    if (var.isDiscrete()) {
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
    int id = ev.id();
    VertexProperty vp = VertexProperty();
    StatementVertex sv = StatementVertex();
    vp.setType(VERTEX::Equation);
    vp.setEq(ev.zeroCrossing());
    sv.setEvent(ev.exp());
    vp.setStm(sv);
    vp.setId(id);
    _equationDescriptors.push_back(add_vertex(vp, graph));
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
    _equationDescriptors.push_back(add_vertex(vp, graph));
  }

  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfrVertex source, _variableDescriptors)
    {
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink]);
      if (edge.exists()) {
        IndexPairSet ips = edge.indexes();
        for (auto ip : ips) {
          Label lbl(ip);
          add_edge(source, sink, lbl, graph);
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
  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfeVertex source, _eventDescriptors)
    {
      if (graph[sink].eq().isZeroCrossing()) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink]);
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
  return graph;
}
}  // namespace Deps
}  // namespace MicroModelica