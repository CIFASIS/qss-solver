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

#include "ea_graph_builder.h"

#include "../../util/util_types.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

EAGraphBuilder::EAGraphBuilder(EventTable &events, EquationTable &algebraics, VarSymbolTable &symbols)
    : _equationDescriptors(),
      _sourceDescriptors(),
      _algebraicDescriptors(),
      _events(events),
      _algebraics(algebraics),
      _equations(),
      _symbols(symbols)
{
}

EAGraphBuilder::EAGraphBuilder(EquationTable &equations, EquationTable &algebraics, VarSymbolTable &symbols)
    : _equationDescriptors(),
      _sourceDescriptors(),
      _algebraicDescriptors(),
      _events(),
      _algebraics(algebraics),
      _equations(equations),
      _symbols(symbols)
{
}

DepsGraph EAGraphBuilder::build()
{
  DepsGraph graph;
  VarSymbolTable::iterator it;
  for (Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it)) {
    if (var.isAlgebraic()) {
      VertexProperty vp = VertexProperty();
      vp.setType(VERTEX::Algebraic);
      vp.setVar(var);
      _sourceDescriptors.push_back(add_vertex(vp, graph));
    }
  }

  EventTable::iterator ev_it;
  for (Event ev = _events.begin(ev_it); !_events.end(ev_it); ev = _events.next(ev_it)) {
    Equation zc = ev.zeroCrossing();
    if (zc.hasAlgebraics()) {
      Expression exp = ev.exp();
      int id = ev.id();
      VertexProperty vp = VertexProperty();
      vp.setType(VERTEX::Equation);
      vp.setId(id);
      vp.setEq(zc);
      _equationDescriptors.push_back(add_vertex(vp, graph));

      VertexProperty ifr = VertexProperty();
      ifr.setType(VERTEX::Influencer);
      Option<Variable> assigned = zc.LHSVariable();
      assert(assigned);
      ifr.setVar(assigned.get());
      ifr.setId(id);
      _sourceDescriptors.push_back(add_vertex(ifr, graph));
    }
  }

  EquationTable::iterator eq_it;
  for (Equation eq = _equations.begin(eq_it); !_equations.end(eq_it); eq = _equations.next(eq_it)) {
    if (eq.hasAlgebraics()) {
      VertexProperty vp = VertexProperty();
      vp.setType(VERTEX::Equation);
      vp.setEq(eq);
      vp.setId(eq.id());
      _equationDescriptors.push_back(add_vertex(vp, graph));
      Option<Variable> assigned = eq.LHSVariable();
      assert(assigned);
      _ifrs[assigned->name()] = pair<Variable, int>(assigned.get(), eq.id());
    }
  }

  for (auto &inf : _ifrs) {
    VertexProperty ifr = VertexProperty();
    ifr.setType(VERTEX::Influencer);
    ifr.setVar(inf.second.first);
    ifr.setId(inf.second.second);
    _sourceDescriptors.push_back(add_vertex(ifr, graph));
  }

  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    VertexProperty vp = VertexProperty();
    vp.setType(VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    _equationDescriptors.push_back(add_vertex(vp, graph));
    if (!eq.hasAlgebraics()) {
      VertexProperty ife = VertexProperty();
      ife.setType(VERTEX::Influencee);
      Option<Variable> assigned = eq.LHSVariable();
      assert(assigned);
      ife.setVar(assigned.get());
      ife.setId(eq.id());
      _algebraicDescriptors.push_back(add_vertex(ife, graph));
    }
  }

  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfrVertex source, _sourceDescriptors)
    {
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Output, VERTEX::LHS);
      if (edge.exists()) {
        IndexPairSet ips = edge.indexes();
        for (auto ip : ips) {
          Label lbl(ip);
          add_edge(source, sink, lbl, graph);
          cout << "Adding edge from: " << graph[source].var().name() << " to equation: " << graph[sink].eq().id() << endl;
          cout << "Equation type: " << graph[sink].eq().type() << endl;
        }
      }
      // Check RHS too if we are working with algebraics.
      if (graph[source].type() == VERTEX::Algebraic) {
        edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input, VERTEX::LHS);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            add_edge(sink, source, lbl, graph);
            cout << "Adding edge from equation: " << graph[sink].eq().id() << " to algebraic: " << graph[source].var().name() << endl;
            cout << "Equation type: " << graph[sink].eq().type() << endl;
          }
        }
      }
    }
  }
  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfeVertex source, _algebraicDescriptors)
    {
      if (graph[sink].type() == VERTEX::Equation && graph[sink].eq().type() == EQUATION::Algebraic) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input, VERTEX::LHS);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            add_edge(sink, source, lbl, graph);
            cout << "Adding edge from equation: " << graph[sink].eq().id() << " to algebraic: " << graph[source].var().name() << endl;
          }
        }
      }
    }
  }
  return graph;
}
}  // namespace Deps
}  // namespace MicroModelica