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
    Dependency::variableMDI(Variable var) 
    {
      IntervalList intervals;
      
      if(var.isArray()) {
        int i, d = var.dimensions();
        for(i = 0; i < d; i++) {
          intervals.push_back(Interval::closed(1,var.size(i)));  
        }
      } else {
        intervals.push_back(Interval::closed(1,1));
      }
      return MDI(intervals); 
    }

    void
    Dependency::compute(DepsGraph g, VariableDependencyMatrix& vdm)
    {
      for (Vertex vd : boost::make_iterator_range(vertices(g))) {
        std::cout << "Vertex descriptor #" << vd << std::endl; /* iterate over the ifr vertex. */
        VertexProperty v = g[vd];
        if (v.type == VERTEX::Influencer) {
          VariableDependencies vdeps; 
          VariableInfluences algs;
          cout << "Compute for: " << v.var << endl;
          influencees(g, vd, variableMDI(v.var), vdeps, algs);
          vdm.insert(v.var.name(), vdeps);    
        }
      }
    }

    VariableDependency 
    Dependency::getVariableDependency(string name, MDI dom, MDI ran, int id) 
    {
      VariableDependency vdep;
      vdep.setVariable(name);
      vdep.setDom(dom);
      vdep.setRan(ran);
      vdep.setEquationId(id);
      return vdep;
    }

    void 
    Dependency::influencees(DepsGraph g, Vertex vd, MDI mdi, VariableDependencies& deps, VariableInfluences& algs) 
    {
      VertexProperty v = g[vd];
      boost::graph_traits<DepsGraph>::out_edge_iterator ei, ei_end;
      for (boost::tie(ei, ei_end) = out_edges(vd, g); ei != ei_end; ++ei) {
        Label l = g[*ei];
        MDI dom = l.Pair().Dom();
        Option<MDI> intersect = mdi.Intersection(dom);
        if (intersect) {
          MDI intersection = boost::get<MDI>(intersect);
          auto target = boost::target(*ei, g);
          if (v.type == VERTEX::Influencer) {
            // Store the influencer expression.
            _ifr = l.Pair().exp();
            _ifrUsg = l.Pair().GetUsage();
          }   
          // First look if the target node is terminal.
          VertexProperty tar = g[target];
          MDI ran = l.getRange(intersection);
          if (tar.type == VERTEX::Influencee) {
            assert(v.type == VERTEX::Equation);
            VariableDependency vdep = getVariableDependency(v.var.name(),intersection,
                                                            ran, v.id);
            vdep.setIfr(_ifr);
            vdep.setIfe(l.Pair().exp());
            vdep.setRange(_ifrUsg, l.Pair().GetUsage());
            
            Influences inf = {algs, vdep};
            deps.push_back(inf);
          } else if (tar.type == VERTEX::Algebraic) {
            assert(v.type == VERTEX::Equation);
            VariableDependency vdep = getVariableDependency(v.var.name(),intersection,
                                                            ran, v.id);
            algs.push_back(vdep);
            influencees(g, target, ran, deps, algs);
          } else { 
            influencees(g, target, ran, deps, algs);
          }
        }
      }
    }
  }
}