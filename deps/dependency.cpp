/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "dependency.h"

#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include "../util/symbol_table.h"

using namespace std;

namespace MicroModelica {
  using namespace Util;
  namespace Deps {

    MDI 
    Dependency::variableRange(Variable var) 
    {
      IntervalList intervals;
      
      if(var.isArray()) {
        int i, d = var.dimensions();
        for(i = 0; i < d; i++) {
          intervals.push_back(Interval::closed(1,var.size(i)));  
        }
      } 
      return MDI(intervals); 
    }

    void
    Dependency::compute(DepsGraph graph, EquationDependencyMatrix& edm)
    {
      for (Vertex vertex : boost::make_iterator_range(vertices(graph))) {
        VertexProperty vertex_info = graph[vertex];
        if (vertex_info.type == VERTEX::Influencer) {
          VariableDependencies var_deps; 
          AlgebraicDependencies algs;
          cout << "Compute dependecies for: " << vertex_info.var << endl;
          influencees(graph, vertex, variableRange(vertex_info.var), var_deps, algs);
          if (!var_deps.empty()) {
            edm.insert(vertex_info.id, var_deps);    
          }
        }
      }
    }

    void
    Dependency::compute(DepsGraph graph, VariableDependencyMatrix& vdm)
    {
      for (Vertex vertex : boost::make_iterator_range(vertices(graph))) {
        VertexProperty vertex_info = graph[vertex];
        if (vertex_info.type == VERTEX::Influencer) {
          VariableDependencies var_deps; 
          AlgebraicDependencies algs;
          cout << "Compute dependecies for: " << vertex_info.var << endl;
          influencees(graph, vertex, variableRange(vertex_info.var), var_deps, algs);
          if (!var_deps.empty()) {
            vdm.insert(vertex_info.var.name(), var_deps);
          }    
        }
      }
    }

    VariableDependency 
    Dependency::getVariableDependency(string name, MDI dom, MDI ran, int id) 
    {
      VariableDependency var_dep;
      var_dep.setVariable(name);
      var_dep.setDom(dom);
      var_dep.setRan(ran);
      var_dep.setEquationId(id);
      return var_dep;
    }

    void 
    Dependency::influencees(DepsGraph graph, Vertex source_vertex, MDI source_range, VariableDependencies& var_deps, 
                            AlgebraicDependencies& algs) 
    {
      VertexProperty source_vertex_info = graph[source_vertex];
      boost::graph_traits<DepsGraph>::out_edge_iterator edge, out_edge_end;
      for (boost::tie(edge, out_edge_end) = out_edges(source_vertex, graph); edge != out_edge_end; ++edge) {
        Label lbl = graph[*edge];
        MDI dom = lbl.Pair().Dom();
        Option<MDI> intersect = source_range.Intersection(dom);
        if (intersect) {
          MDI intersection = boost::get<MDI>(intersect);
          auto target_vertex = boost::target(*edge, graph);
          if (source_vertex_info.type == VERTEX::Influencer) {
            // Store the influencer index pair.
            _ifr = lbl.Pair();
          }   
          // First look if the target node is terminal.
          VertexProperty target_vertex_info = graph[target_vertex];
          MDI ran = lbl.getImage(intersection);
          if (target_vertex_info.type == VERTEX::Influencee) {
            assert(source_vertex_info.type == VERTEX::Equation ||
                   source_vertex_info.type == VERTEX::Statement);
            VariableDependency var_dep = getVariableDependency(target_vertex_info.var.name(), intersection,
                                                               ran, target_vertex_info.id);
            var_dep.setIfr(_ifr);
            var_dep.setIfe(lbl.Pair());
            var_dep.setRange();
            
            Influences inf = {algs, var_dep};
            var_deps.push_back(inf);
          } else if (target_vertex_info.type == VERTEX::Algebraic) {
            assert(source_vertex_info.type == VERTEX::Equation);
            VariableDependency var_dep = getVariableDependency(target_vertex_info.var.name(), intersection,
                                                               ran, target_vertex_info.id);
            algs.push_back(var_dep);
            influencees(graph, target_vertex, ran, var_deps, algs);
          } else { 
            influencees(graph, target_vertex, ran, var_deps, algs);
          }
        }
      }
    }

    void
    Dependency::merge(VariableDependencyMatrix& source, VariableDependencyMatrix& target, VariableDependencyMatrix& merge) 
    {
      VariableDependencyMatrix::const_iterator source_it;
      for(source_it = source.begin(); source_it != source.end(); source_it++)
      { 
        // First look if the source variable affects some var in the target matrix. 
        Option<VariableDependencies> has_target_deps = target[source_it->first]; 
        if (!has_target_deps) {
          continue;
        }
        VariableDependencies target_var_deps = boost::get<VariableDependencies>(has_target_deps);
        VariableDependencies source_var_deps = source_it->second;
        for (auto source_var_dep : source_var_deps) {
          for (auto target_var_dep : target_var_deps) {
             MDI target_dom = target_var_dep.ifce.dom();
             Option<MDI> intersect = source_var_dep.ifce.dom().Intersection(target_dom);    
             if (intersect) {
              // We found a match and must adjust the ranges and generate the new var_dep.
              MDI intersection = boost::get<MDI>(intersect);
              VariableDependency target = target_var_dep.ifce;
              VariableDependency source = source_var_dep.ifce;
              VariableDependency var_dep = getVariableDependency(target.variable(), 
                                                                 source.getImage(intersection),
                                                                 target.getImage(intersection), 
                                                                 target.equationId());
              var_dep.setIfr(source.ifePair());
              var_dep.setIfe(target.ifePair());
              var_dep.setRange();
              
              // Finally insert the new variable in the merge matrix.
              Option<VariableDependencies> has_merge_deps = merge[source.variable()]; 
              VariableDependencies new_merge_deps;
              if (has_merge_deps) {
                new_merge_deps = boost::get<VariableDependencies>(has_merge_deps);
              }
              Influences inf = {AlgebraicDependencies(), var_dep};
              new_merge_deps.push_back(inf);
              merge.insert(source.variable(), new_merge_deps);
            }
          }
        }      
      }
    }
  }
}