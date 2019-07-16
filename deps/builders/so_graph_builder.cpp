/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free Outputftware: you can redistribute it and/or modify
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

#include "so_graph_builder.h"

#include "../../util/util_types.h"


namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Deps {

    OutputGraphBuilder::OutputGraphBuilder(EquationTable &equations, EquationTable &algebraics,
                                           VarSymbolTable& symbols, OUTPUT::Type type) :
      _equationDescriptors(),
      _variableDescriptors(),
      _outputDescriptors(),
      _equations(equations),
      _algebraics(algebraics),
      _symbols(symbols),
      _type(type)
    {

    }
    
    DepsGraph
    OutputGraphBuilder::build()
    {
      DepsGraph graph;
      // First, add the symbols as vertex.
      VarSymbolTable::iterator it;
      for(Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it))
      {
        VertexProperty vp = VertexProperty();
        bool varType = _type == OUTPUT::SO ? var.isState() : var.isDiscrete(); 
        if(varType) {
          vp.type = VERTEX::Influencer;
          vp.var = var;
          _variableDescriptors.push_back(add_vertex(vp, graph));
        }
        else if (var.isAlgebraic()) {
          vp.type = VERTEX::Algebraic;
          vp.var = var;
          _variableDescriptors.push_back(add_vertex(vp, graph));
        }

      }
      
      EquationTable::iterator eqit;
      for(Equation eq = _equations.begin(eqit); !_equations.end(eqit); eq = _equations.next(eqit))
      {
        VertexProperty vp = VertexProperty();
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _equationDescriptors.push_back(add_vertex(vp,graph));
        VertexProperty icee = VertexProperty();
        icee.type = VERTEX::Influencee;
        icee.id = eq.id();
        _outputDescriptors[eq.id()] = add_vertex(icee, graph);
      }
      for(Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit))
      {
        VertexProperty vp = VertexProperty();
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _equationDescriptors.push_back(add_vertex(vp,graph));
      }

      foreach_(EqVertex sink, _equationDescriptors){
        foreach_(IfrVertex source, _variableDescriptors){
          GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
          if(edge.exists()) {
            IndexPairSet ips = edge.indexes();
            for (auto ip : ips) {
              Label lbl(ip);
              cout << "Agrega arista desde la var: " << graph[source].var << " a la ecuacion: " << graph[sink].eq.id() << endl;
              cout << "Ecuacion: " << graph[sink].eq.type() << endl; 
              add_edge(source, sink, lbl, graph);
              cout << ip.Ran() << " " << ip.Dom() << endl;
              if (graph[sink].eq.type() == EQUATION::Output) {
                IndexPair out_pair(ip.Ran(), ip.Ran(), Offset(), Usage(), graph[sink].eq.lhs());
                Label out_lbl(out_pair, EDGE::Input);
                cout << "Agrega arista para la ecuacion: " << graph[sink].eq.id()
                     << " a la ecuacion: "
                     << graph[_outputDescriptors[graph[sink].eq.id()]].id << endl;
                cout << "Ecuacion: " << graph[sink].eq.type() << endl;
                add_edge(sink, _outputDescriptors[graph[sink].eq.id()], out_lbl, graph);
              }
            }
          }
          // Check LHS too if we are working with algebraics.
          if (graph[source].type == VERTEX::Algebraic && graph[sink].eq.type() == EQUATION::Algebraic) { 
            GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input);
            if(edge.exists()) {
              IndexPairSet ips = edge.indexes();
              for (auto ip : ips) {
                Label lbl(ip, EDGE::Input);
                cout << "Agrega arista desde la ecuacion algebraica: " << graph[sink].eq.id() << " a la variable: " << graph[source].var  << endl;
                cout << "Ecuacion algebraica: " << graph[sink].eq.type() << endl;
                add_edge(sink, source, lbl, graph);  
              }
            }            
          }
        }
      }
      return graph;
    }    
  }
}