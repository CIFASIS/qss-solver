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

#include "../../util/util_types.h"


namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Deps {

    DHGraphBuilder::DHGraphBuilder(EquationTable &equations, EquationTable &algebraics, VarSymbolTable& symbols) :
      _equationDescriptors(),
      _variableDescriptors(),
      _equations(equations),
      _algebraics(algebraics),
      _symbols(symbols)
    {

    }
    
    DepsGraph
    DHGraphBuilder::build()
    {
      DepsGraph graph;
      // First, add the symbols as vertex.
      VarSymbolTable::iterator it;
      for(Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it))
      {
        VertexProperty vp;
        if(var.isState()) {
          vp.type = VERTEX::Influencer;
          vp.var = var;
          _variableDescriptors.push_back(add_vertex(vp, graph));
          VertexProperty icee;
          icee.type = VERTEX::Influencee;
          icee.var = var; 
          _derivativeDescriptors.push_back(add_vertex(icee, graph));
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
        VertexProperty vp;
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _equationDescriptors.push_back(add_vertex(vp,graph));
      }
      for(Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit))
      {
        VertexProperty vp;
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _equationDescriptors.push_back(add_vertex(vp,graph));
      }

      cout << "Ecuaciones: " << _equationDescriptors.size() << endl;
      cout << "Variables: " << _variableDescriptors.size() << endl;
      cout << "Derivadas: " << _derivativeDescriptors.size() << endl;

      foreach_(EqVertex eq, _equationDescriptors){
        foreach_(IfrVertex inf, _variableDescriptors){
          GenerateEdge ge = GenerateEdge(graph[eq], graph[inf], _symbols);
          if(ge.exists()) {
            IndexPairSet ips = ge.indexes();
            for (auto ip : ips) {
              Label ep(ip);
              cout << "Agrega arista desde la var: " << graph[inf].var << " a la ecuacion: " << graph[eq].eq.id() << endl;
              cout << "Ecuacion: " << graph[eq].eq.type() << endl; 
              add_edge(inf, eq, ep, graph);  
            }
          }
          // Check LHS too if we are working with algebraics.
          if (graph[inf].type == VERTEX::Algebraic && graph[eq].eq.type() == EQUATION::Algebraic) { 
            GenerateEdge gea = GenerateEdge(graph[eq], graph[inf], _symbols, VERTEX::Input);
            if(gea.exists()) {
              IndexPairSet ips = gea.indexes();
              for (auto ip : ips) {
                Label ep(ip);
                cout << "Agrega arista desde la ecuacion algebraica: " << graph[eq].eq.id() << " a la variable: " << graph[inf].var  << endl;
                cout << "Ecuacion algebraica: " << graph[eq].eq.type() << endl;
                add_edge(eq, inf, ep, graph);  
              }
            }            
          }
        }
      }
      foreach_(EqVertex eq, _equationDescriptors){
        foreach_(IfeVertex inf, _derivativeDescriptors){
          if (graph[eq].eq.isDerivative()) {
            GenerateEdge ge = GenerateEdge(graph[eq], graph[inf], _symbols);
            if (ge.exists()) {
              IndexPairSet ips = ge.indexes();
              for (auto ip : ips) {
                Label ep(ip);
                cout << "Agrega arista desde la ecuacion derivada: " << graph[eq].eq.id() << " a la derivada: " << graph[inf].var  << endl;
                cout << "Ecuacion algebraica: " << graph[eq].eq.type() << endl;

                add_edge(eq, inf, ep, graph);
              }
            }
          }
        }
      }
      return graph;
    }    
  }
}