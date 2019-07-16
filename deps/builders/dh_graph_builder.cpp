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

    DHGraphBuilder::DHGraphBuilder(EventTable &events, EquationTable &algebraics, 
                                   VarSymbolTable& symbols, STATEMENT::AssignTerm search,
                                   DHGRAPHBUILDER::IfrType ifr_type) :
      _statementDescriptors(),
      _variableDescriptors(),
      _events(events),
      _algebraics(algebraics),
      _symbols(symbols),
      _search(search),
      _ifr_type(ifr_type)
    {
    }
    
    void DHGraphBuilder::addStatements(StatementTable stms, DepsGraph& graph, Expression exp, int id)
    {
        StatementTable::iterator stm_it;
        for (Statement stm = stms.begin(stm_it); !stms.end(stm_it); stm = stms.next(stm_it)) {
          for (Expression e : stm.assignments(_search)) {
            VertexProperty vp = VertexProperty();
            vp.type = VERTEX::Statement;
            vp.stm.exp = e;
            vp.stm.event = exp;
            vp.id = id;
            _statementDescriptors.push_back(add_vertex(vp,graph));
          }
        }
    }

    DepsGraph
    DHGraphBuilder::build()
    {
      DepsGraph graph;
      // First, add the symbols as vertex.
      VarSymbolTable::iterator it;
      for(Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it))
      {
        VertexProperty vp = VertexProperty();
        bool add_influencer = (_ifr_type == DHGRAPHBUILDER::State) ? var.isState() : var.isDiscrete();
        if(add_influencer) {
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
      EventTable::iterator ev_it;
      for (Event ev = _events.begin(ev_it); !_events.end(ev_it); ev = _events.next(ev_it)) {
        Expression exp = ev.exp();
        int id = ev.id();
        addStatements(ev.positiveHandler(), graph, exp, id); 
        addStatements(ev.negativeHandler(), graph, exp, id);
        VertexProperty icee = VertexProperty();
        icee.type = VERTEX::Influencee;
        icee.id = id; 
        _eventDescriptors.push_back(add_vertex(icee, graph)); 
      }
      EquationTable::iterator eq_it;
      for(Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it))
      {
        VertexProperty vp = VertexProperty();
        vp.type = VERTEX::Equation;
        vp.eq = eq;
        _statementDescriptors.push_back(add_vertex(vp,graph));
      }

      foreach_(EqVertex sink, _statementDescriptors){
        foreach_(IfrVertex source, _variableDescriptors){
          GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
          if(edge.exists()) {
            IndexPairSet ips = edge.indexes();
            for (auto ip : ips) {
              Label lbl(ip);
              add_edge(source, sink, lbl, graph);  
            }
          }
          // Check LHS too if we are working with algebraics.
          if (graph[source].type == VERTEX::Algebraic && graph[sink].eq.type() == EQUATION::Algebraic) { 
            edge  = GenerateEdge(graph[source], graph[sink], _symbols, EDGE::Input);
            if(edge.exists()) {
              IndexPairSet ips = edge.indexes();
              for (auto ip : ips) {
                Label lbl(ip);
                add_edge(sink, source, lbl, graph);  
              }
            }            
          }
        }
      }
      foreach_(EqVertex sink, _statementDescriptors){
        foreach_(IfeVertex source, _eventDescriptors){
          if (graph[sink].type == VERTEX::Statement) {
            GenerateEdge edge = GenerateEdge(graph[source], graph[sink], _symbols);
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
  }
}