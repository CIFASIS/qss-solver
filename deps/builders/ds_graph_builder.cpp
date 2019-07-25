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

#include "ds_graph_builder.h"

#include "../../util/util_types.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

DSGraphBuilder::DSGraphBuilder(EquationTable &equations, EquationTable &algebraics, VarSymbolTable &symbols)
    : _equationDescriptors(), _variableDescriptors(), _equations(equations), _algebraics(algebraics), _symbols(symbols)
{
}

DepsGraph DSGraphBuilder::build()
{
  DepsGraph graph;
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  for (Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it)) {
    VertexProperty vp = VertexProperty();
    if (var.isDiscrete()) {
      vp.type = VERTEX::Influencer;
      vp.var = var;
      _variableDescriptors.push_back(add_vertex(vp, graph));
    } else if (var.isAlgebraic()) {
      vp.type = VERTEX::Algebraic;
      vp.var = var;
      _variableDescriptors.push_back(add_vertex(vp, graph));
    }
  }

  EquationTable::iterator eqit;
  for (Equation eq = _equations.begin(eqit); !_equations.end(eqit); eq = _equations.next(eqit)) {
    VertexProperty vp = VertexProperty();
    vp.type = VERTEX::Equation;
    vp.eq = eq;
    _equationDescriptors.push_back(add_vertex(vp, graph));
    VertexProperty ife = VertexProperty();
    ife.type = VERTEX::Influencee;
    Option<Variable> var = eq.LHSVariable();
    assert(var);
    ife.var = var.get();
    _derivativeDescriptors.push_back(add_vertex(ife, graph));
  }
  for (Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit)) {
    VertexProperty vp = VertexProperty();
    vp.type = VERTEX::Equation;
    vp.eq = eq;
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
          cout << "Agrega arista desde la var: " << graph[source].var << " a la ecuacion: " << graph[sink].eq.id() << endl;
          cout << "Ecuacion: " << graph[sink].eq.type() << endl;
          add_edge(source, sink, lbl, graph);
        }
      }
      // Check LHS too if we are working with algebraics.
      if (graph[source].type == VERTEX::Algebraic && graph[sink].eq.type() == EQUATION::Algebraic) {
        edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            cout << "Agrega arista desde la ecuacion algebraica: " << graph[sink].eq.id() << " a la variable: " << graph[source].var
                 << endl;
            cout << "Ecuacion algebraica: " << graph[sink].eq.type() << endl;
            add_edge(sink, source, lbl, graph);
          }
        }
      }
    }
  }

  foreach_(EqVertex sink, _equationDescriptors)
  {
    foreach_(IfeVertex source, _derivativeDescriptors)
    {
      if (graph[sink].eq.isDerivative()) {
        GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
        if (edge.exists()) {
          IndexPairSet ips = edge.indexes();
          for (auto ip : ips) {
            Label lbl(ip);
            cout << "Agrega arista desde la ecuacion derivada: " << graph[sink].eq.id() << " a la derivada: " << graph[source].var << endl;
            cout << "Ecuacion algebraica: " << graph[sink].eq.type() << endl;
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