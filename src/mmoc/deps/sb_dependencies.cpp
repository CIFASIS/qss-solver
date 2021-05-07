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

#include "sb_dependencies.h"

#include <cassert>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

#include <ir/helpers.h>
#include <ir/index.h>
#include <parser/parse.h>
#include <util/logger.h>
#include <util/util.h>

using namespace std;

namespace MicroModelica {
using namespace Util;
using namespace IR;
namespace Deps {
namespace SBG {

template <typename IDependencies, typename R>
SBDependencies<IDependencies, R>::SBDependencies() : _index_shift(){};

template <typename IDependencies, typename R>
R SBDependencies<IDependencies, R>::deps()
{
  return _gen.deps();
}

template <typename IDependencies, typename R>
void SBDependencies<IDependencies, R>::compute(SBGraph graph, IndexShift index_shift)
{
  _index_shift = index_shift;
  for (SBVertex vertex : boost::make_iterator_range(vertices(graph))) {
    VertexProperty vertex_info = graph[vertex];
    if (vertex_info.type() == VERTEX::Influencer) {
      auto edge = *out_edges(vertex, graph).first;
      SBVertex f_vertex = boost::target(edge, graph);
      LOG << "Compute paths for equation: " << graph[f_vertex].name() << endl;
      paths(graph, f_vertex, Variable());
    }
  }
  for (SBVertex vertex : boost::make_iterator_range(vertices(graph))) {
    VertexProperty vertex_info = graph[vertex];
    if (vertex_info.type() == VERTEX::Influencer) {
      auto edge = *out_edges(vertex, graph).first;
      SBVertex f_vertex = boost::target(edge, graph);
      _gen.postProcess(graph[f_vertex]);
    }
  }
}

template <typename IDependencies, typename R>
void SBDependencies<IDependencies, R>::paths(SBGraph graph, SBVertex V, Variable visiting_alg)
{
  int num_gen = 0;
  boost::graph_traits<SBGraph>::out_edge_iterator edge, out_edge_end;
  list<Map> rec_alg_use_maps;
  // For all A_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    Label edge_label = graph[*edge];
    SBVertex A = boost::target(*edge, graph);
    if (graph[A].var() == visiting_alg) {
      for (Pair pair : edge_label.pairs()) {
        rec_alg_use_maps.push_back(pair.map());
      }
    } else if (graph[A].type() == VERTEX::Algebraic) {
      if (!graph[A].visited()) {
        boost::graph_traits<SBGraph>::out_edge_iterator alg_edge, alg_out_edge_end;
        // For all G in Succ(A_i)
        for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
          SBVertex G = boost::target(*alg_edge, graph);
          if (graph[G].eq().type() == IR::EQUATION::Algebraic) {
            paths(graph, G, graph[A].var());
          }
        }
        graph[A].setVisited(true);
      }
    }
  }
  _gen.init(graph[V]);
  // For A_i in Succ(V) 
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    Label edge_label = graph[*edge];
    SBVertex A = boost::target(*edge, graph);
    if (graph[A].type() == VERTEX::Algebraic) { 
      // For all map_n in E_VA
      for (Pair pair : edge_label.pairs()) {
        Map map_m = pair.map();
        _gen.initG(graph[V].eq(), map_m);
        boost::graph_traits<SBGraph>::out_edge_iterator alg_edge, alg_out_edge_end;
        // For all G in Succ(A_i)
        for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
          SBVertex G = boost::target(*alg_edge, graph);
          if (graph[G].eq().type() == IR::EQUATION::Algebraic) {
            Label alg_label = graph[*alg_edge];
            int dep, deps = graph[G].numDeps();
            for (dep = 1; dep <= deps; dep++) {
              // Ai -> Gij labels must contain only one pair.
              assert(alg_label.pairs().size() == 1);
              // First get the dom of G for the algebraic equation
              MDI g_dom = alg_label.pairs().begin()->dom();
              Map G_map = alg_label.pairs().begin()->map();
              MDI v_range = pair.map().apply(pair.dom(), pair.ran());
              Option<MDI> d_intersect = g_dom.intersection(v_range);
              if (d_intersect) {
                Map g_map = graph[G].map(dep);
                Map n_map = g_map.compose(G_map.solve(map_m));
                MDI range = n_map.apply(pair.dom(), pair.ran());
                VariableDep state_dep = graph[G].depStates(dep);
                MDI state_range = state_dep.range();
                Option<MDI> r_intersect = range.intersection(state_range);
                if (r_intersect) {
                  num_gen++;
                  MDI dep_dom = d_intersect.get();
                  MDI dep_ran = r_intersect.get();
                  Map dep_map = n_map;
                  if (state_dep.isRecursive()) {
                    dep_map = g_map;
                    dep_dom = g_dom;
                    dep_ran = state_dep.range();  
                  }
                  VariableDep var_dep(state_dep.var(), dep_dom, dep_ran, state_dep.isRecursive());
                  if (graph[A].var() == visiting_alg) {
                    var_dep.setAlgDom(dep_dom);
                    var_dep.setAlgEq(graph[G].eq());
                  } else if (state_dep.hasAlgDeps()) {
                    var_dep.setAlgDom(state_dep.algDom());
                    var_dep.setAlgEq(state_dep.algEq());
                  }
                  graph[V].setDepState(num_gen, var_dep);
                  graph[V].setMap(num_gen, dep_map);
                  _gen.visitG(graph[V].eq(), graph[G].eq(), var_dep, dep_map, map_m, _index_shift[graph[G].eq().id()]);
                }
              }
            }
          }
        }
      }
    }
  }
  // For X_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    Label edge_label = graph[*edge];
    SBVertex X = boost::target(*edge, graph);
    if (graph[X].type() == VERTEX::Influencee) {
      // For all map_n in E_V
      for (Pair pair : edge_label.pairs()) {
        Map map = pair.map();
        MDI range = pair.map().apply(pair.dom(), pair.ran());
        num_gen++;
        VariableDep var_dep(graph[X].var(), pair.dom(), range);
        graph[V].setDepState(num_gen, var_dep);
        graph[V].setMap(num_gen, map);
        _gen.visitF(graph[V].eq(), var_dep, map);
        /// Add algebraic recursive uses.
        for (Map rec_alg_use: rec_alg_use_maps) {
          num_gen++;
          Map use_map = rec_alg_use.compose(map);
          MDI applied_map = use_map.apply(pair.dom(), pair.ran());
          Option<MDI> range = pair.dom().intersection(applied_map);
          assert(range);
          const bool RECURSIVE = true;
          VariableDep rec_alg_dep(graph[X].var(), pair.dom(), range.get(), RECURSIVE);
          graph[V].setDepState(num_gen, rec_alg_dep);
          graph[V].setMap(num_gen, use_map);
          _gen.visitF(graph[V].eq(), VariableDep(graph[X].var(), pair.dom(), range.get()), use_map);
        }
      }
    }
  }
  graph[V].setNumDeps(num_gen);
  _gen.end();
}

template class SBDependencies<JacMatrixGenerator, JacMatrixDef>;

template class SBDependencies<JacGenerator, JacDef>;

}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica