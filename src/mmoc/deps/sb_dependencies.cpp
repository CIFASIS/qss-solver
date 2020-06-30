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

#include "../ir/helpers.h"
#include "../ir/index.h"
#include "../parser/parse.h"
#include "../util/util.h"
#include "../util/symbol_table.h"

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
      cout << "Compute paths for equation: " << graph[f_vertex].name() << endl;
      paths(graph, f_vertex);
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
void SBDependencies<IDependencies, R>::paths(SBGraph graph, SBVertex V)
{
  int num_gen = 0;
  boost::graph_traits<SBGraph>::out_edge_iterator edge, out_edge_end;
  // For all A_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    Label edge_label = graph[*edge];
    SBVertex A = boost::target(*edge, graph);
    if (graph[A].type() == VERTEX::Algebraic) {
      if (!graph[A].visited()) {
        boost::graph_traits<SBGraph>::out_edge_iterator alg_edge, alg_out_edge_end;
        // For all G in Succ(A_i)
        for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
          SBVertex G = boost::target(*alg_edge, graph);
          if (graph[G].eq().type() == IR::EQUATION::Algebraic) {
            paths(graph, G);
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
      // For all map_n in E_V
      for (Pair pair : edge_label.pairs()) {
        Map map_m = pair.map();
        _gen.initG(graph[V].eq(), map_m);
        boost::graph_traits<SBGraph>::out_edge_iterator alg_edge, alg_out_edge_end;
        // For all G in Succ(A_i)
        for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
          SBVertex G = boost::target(*alg_edge, graph);
          if (graph[G].eq().type() == IR::EQUATION::Algebraic) {
            int dep, deps = graph[G].numDeps();
            for (dep = 1; dep <= deps; dep++) {
              Map g_map = graph[G].map(dep);
              Map n_map = map_m.compose(g_map.applyIndexShift(_index_shift[graph[G].eq().id()]));
              MDI range = n_map.apply(pair.dom(), pair.ran());
              VariableDep state_dep = graph[G].depStates(dep);
              MDI state_range = state_dep.mdi();
              Option<MDI> intersect = range.intersection(state_range);
              if (intersect) {
                num_gen++;
                VariableDep var_dep(state_dep.var(), intersect.get());
                graph[V].setDepState(num_gen, var_dep);
                graph[V].setMap(num_gen, n_map);
                _gen.visitG(graph[V].eq(), graph[G].eq(), var_dep, n_map, map_m, _index_shift[graph[G].eq().id()]);
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
        VariableDep var_dep(graph[X].var(), range);
        graph[V].setDepState(num_gen, var_dep);
        graph[V].setMap(num_gen, map);
        _gen.visitF(graph[V].eq(), var_dep, map);
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