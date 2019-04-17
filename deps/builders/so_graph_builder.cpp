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
        VertexProperty vp;
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
        VertexProperty vp;
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _equationDescriptors.push_back(add_vertex(vp,graph));
        VertexProperty icee;
        icee.type = VERTEX::Influencee;
        icee.id = eq.id();
        _outputDescriptors[eq.id()] = add_vertex(icee, graph);
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
      cout << "Salidas: " << _outputDescriptors.size() << endl;

      foreach_(EqVertex eq, _equationDescriptors){
        foreach_(IfrVertex inf, _variableDescriptors){
          GenerateEdge ge = GenerateEdge(graph[eq], graph[inf], _symbols);
          if(ge.exists()) {
            IndexPairSet ips = ge.indexes();
            for (auto ip : ips) {
              Label ep(ip);
              cout << ip << " empty: " << ep.IsEmpty() << endl;
              add_edge(inf, eq, ep, graph);
              IndexPair op(ip.Ran(), ip.Ran(), Offset(), Usage()); 
              Label oep(op);
              add_edge(eq, _outputDescriptors[graph[eq].eq.id()], graph); 
            }
          }
          // Check LHS too if we are working with algebraics.
          if (graph[inf].type == VERTEX::Algebraic) { 
            GenerateEdge gea = GenerateEdge(graph[eq], graph[inf], _symbols, VERTEX::Input);
            if(gea.exists()) {
              IndexPairSet ips = gea.indexes();
              for (auto ip : ips) {
                Label ep(ip);
                cout << ip << " empty: " << ep.IsEmpty() << endl;
                add_edge(eq, inf, ep, graph);  
                IndexPair op(ip.Ran(), ip.Ran(), Offset(), Usage()); 
                Label oep(op);
                add_edge(eq, _outputDescriptors[graph[eq].eq.id()], graph); 
              }
            }            
          }
        }
      }
      return graph;
    }    
  }
}
