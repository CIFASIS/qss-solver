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
    : _equation_def_nodes(), _equation_lhs_nodes(), _state_nodes(), _equations(equations), _algebraics(algebraics), _symbols(symbols)
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
      vp.setType(VERTEX::Influencee);
      vp.setVar(var);
      //  Use to differentiate from EA builders need to refactor Edge builder.
      const int NOT_ASSIGNED = -1;
      vp.setId(NOT_ASSIGNED);
      _state_nodes.push_back(add_vertex(vp, graph));
    } else if (var.isAlgebraic()) {
      vp.setType(VERTEX::Algebraic);
      vp.setVar(var);
      _equation_lhs_nodes.push_back(add_vertex(vp, graph));
    }
  }
  EquationTable::iterator eqit;
  for (Equation eq = _equations.begin(eqit); !_equations.end(eqit); eq = _equations.next(eqit)) {
    VertexProperty vp = VertexProperty();
    vp.setType(VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    _equation_def_nodes.push_back(add_vertex(vp, graph));
    Option<Variable> assigned = eq.LHSVariable();
    assert(assigned);
    VertexProperty ifr = VertexProperty();
    ifr.setType(VERTEX::Influencer);
    ifr.setVar(assigned.get());
    ifr.setId(eq.id());
    _equation_lhs_nodes.push_back(add_vertex(ifr, graph));
  }
  for (Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit)) {
    VertexProperty vp = VertexProperty();
    vp.setType(VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    _equation_def_nodes.push_back(add_vertex(vp, graph));
  }

  foreach_(EqVertex sink, _equation_def_nodes)
  {
    foreach_(IfrVertex source, _equation_lhs_nodes)
    {
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Output, VERTEX::LHS);
      if (edge.exists()) {
        IndexPairSet ips = edge.indexes();
        for (auto ip : ips) {
          Label lbl(ip);
          add_edge(source, sink, lbl, graph);
          // cout << "Adding OUTPUT LHS edge from: " << graph[source].var().name() << " to equation: " << graph[sink].eq().id() << endl;
          // cout << "Equation type: " << graph[sink].eq().type() << endl;
          // cout << "With Domain: " << lbl.Pair().Dom() << " and Range: " << lbl.Pair().Ran() << endl;
        }
      }
      // Check RHS too if we are working with algebraics.
      if (graph[source].type() == VERTEX::Algebraic) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input, VERTEX::LHS);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip, EDGE::Input);
            add_edge(sink, source, lbl, graph);
            // cout << "Adding INPUT RHS edge from equation: " << graph[sink].eq().id()
            //     << " to algebraic variable: " << graph[source].var().name() << endl;
            // cout << "With Domain: " << lbl.Pair().Dom() << " and Range: " << lbl.Pair().Ran() << endl;
          }
        }
      }
    }
  }
  foreach_(EqVertex sink, _equation_def_nodes)
  {
    foreach_(IfeVertex source, _state_nodes)
    {
      GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input, VERTEX::LHS);
      if (edge.exists()) {
        IndexPairSet ips = edge.indexes();
        for (auto ip : ips) {
          Label lbl(ip, EDGE::Input);
          add_edge(sink, source, lbl, graph);
          // cout << "Adding FINAL edge from equation: " << graph[sink].eq().id() << " to variable: " << graph[source].var().name() << endl;
          // cout << "With Domain: " << lbl.Pair().Dom() << " and Range: " << lbl.Pair().Ran() << endl;
        }
      }
    }
  }
  return graph;
}
}  // namespace Deps
}  // namespace MicroModelica