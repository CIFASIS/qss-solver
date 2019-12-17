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

#include "sd_graph_builder.h"

#include "../../util/util_types.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

SDGraphBuilder::SDGraphBuilder(EquationTable &equations, EquationTable &algebraics, VarSymbolTable &symbols)
    : _equationDescriptors(), _variableDescriptors(), _equations(equations), _algebraics(algebraics), _symbols(symbols)
{
}

DepsGraph SDGraphBuilder::build()
{
  DepsGraph graph;
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  for (Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it)) {
    VertexProperty vp = VertexProperty();
    if (var.isState()) {
      vp.setType(VERTEX::Influencer);
      vp.setVar(var);
      _variableDescriptors.push_back(add_vertex(vp, graph));
      VertexProperty icee = VertexProperty();
      icee.setType(VERTEX::Influencee);
      icee.setVar(var);
      cout << "Adding influencee: " << var.name() << endl;
      _derivativeDescriptors.push_back(add_vertex(icee, graph));
    } else if (var.isAlgebraic()) {
      vp.setType(VERTEX::Algebraic);
      vp.setVar(var);
      _variableDescriptors.push_back(add_vertex(vp, graph));
    }
  }
  EquationTable::iterator eqit;
  for (Equation eq = _equations.begin(eqit); !_equations.end(eqit); eq = _equations.next(eqit)) {
    VertexProperty vp = VertexProperty();
    vp.setType(VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    _equationDescriptors.push_back(add_vertex(vp, graph));
  }
  for (Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit)) {
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
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
      if (edge.exists()) {
        IndexPairSet ips = edge.indexes();
        for (auto ip : ips) {
          Label lbl(ip);
          add_edge(source, sink, lbl, graph);
          cout << "Adding edge from: " << graph[source].var().name() << " to equation: " << graph[sink].eq().id() << endl;
          cout << "Equation type: " << graph[sink].eq().type() << endl;
        }
      }
      // Check LHS too if we are working with algebraics.
      if (graph[source].type() == VERTEX::Algebraic && graph[sink].eq().type() == EQUATION::Algebraic) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            add_edge(sink, source, lbl, graph);
            cout << "Adding edge from equation: " << graph[sink].eq().id() << " to equation: " << graph[source].var().name() << endl;
          }
        }
      }
    }
  }
  // Generate der(var) --> var edges.
  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfeVertex source, _derivativeDescriptors)
    {
      if (graph[sink].eq().isDerivative()) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            add_edge(sink, source, lbl, graph);
            cout << "Adding edge from equation: " << graph[sink].eq().id() << " to equation: " << graph[source].var().name() << endl;
          }
        }
      }
    }
  }
  return graph;
}
}  // namespace Deps
}  // namespace MicroModelica