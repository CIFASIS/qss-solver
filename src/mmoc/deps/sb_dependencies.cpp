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

#include <deps/sbg_graph/build_from_exps.h>
#include <ir/helpers.h>
#include <ir/index.h>
#include <parser/parse.h>
#include <util/logger.h>
#include <util/util.h>

using namespace std;
using namespace SB;

namespace MicroModelica {
using namespace Util;
using namespace IR;
namespace Deps {

template <typename IDependencies, typename R>
SBDependencies<IDependencies, R>::SBDependencies() : _index_shift(){};

template <typename IDependencies, typename R>
R SBDependencies<IDependencies, R>::deps()
{
  return _gen.deps();
}

template <typename IDependencies, typename R>
void SBDependencies<IDependencies, R>::compute(SB::Deps::Graph graph, SB::Deps::IndexShift index_shift)
{
  _index_shift = index_shift;
  SB::Deps::EdgeIt ei_start, ei_end;
  boost::tie(ei_start, ei_end) = edges(graph);

  // First build the complete maps so we can get the pre-images.
  for (; ei_start != ei_end; ++ei_start) {
    PWLMap f_map = (graph[*ei_start]).mapF();
    PWLMap u_map = (graph[*ei_start]).mapU();

    _map_F = _map_F.concat(f_map);
    _map_U = _map_U.concat(u_map);
  }

  for (Deps::Vertex vertex : boost::make_iterator_range(vertices(graph))) {
    SB::Deps::SetVertex vertex_info = graph[vertex];
    if (vertex_info.desc().type() == SB::Deps::VERTEX::Influencer) {
      LOG << "Compute paths for equation: " << graph[vertex].name() << endl;
      paths(graph, vertex, Variable());
    }
  }
  for (SB::Deps::Vertex vertex : boost::make_iterator_range(vertices(graph))) {
    SB::Deps::SetVertex vertex_info = graph[vertex];
    if (vertex_info.desc().type() == SB::Deps::VERTEX::Influencer) {
      _gen.postProcess(graph[vertex]);
    }
  }
}

template <typename IDependencies, typename R>
void SBDependencies<IDependencies, R>::paths(SB::Deps::Graph graph, SB::Deps::Vertex V, Variable visiting_alg)
{
  int num_gen = 0;
  boost::graph_traits<SB::Deps::Graph>::out_edge_iterator edge, out_edge_end;
  list<SB::Deps::SetEdge> rec_alg_use_maps;
  // For all A_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    SB::Deps::SetEdge edge_label = graph[*edge];
    SB::Deps::Vertex A = boost::target(*edge, graph);
    if (graph[A].desc().var() == visiting_alg) {
      rec_alg_use_maps.push_back(edge_label);
    } else if (graph[A].desc().type() == SB::Deps::VERTEX::Algebraic) {
      if (!graph[A].desc().visited()) {
        boost::graph_traits<SB::Deps::Graph>::out_edge_iterator alg_edge, alg_out_edge_end;
        // For all G in Succ(A_i)
        for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
          SB::Deps::Vertex G = boost::target(*alg_edge, graph);
          if (graph[G].desc().type() == SB::Deps::VERTEX::Equation) {
            paths(graph, G, graph[A].desc().var());
          }
        }
        SB::Deps::updateVisited(graph, A, true);
      }
    }
  }

  _gen.init(graph[V]);
  // For A_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    SB::Deps::SetEdge edge_label = graph[*edge];
    SB::Deps::Vertex A = boost::target(*edge, graph);
    if (graph[A].desc().type() == SB::Deps::VERTEX::Algebraic) {
      // For all map_n in E_VA
      PWLMap map_m_f = edge_label.mapF();
      PWLMap map_m_u = edge_label.mapU();
      SB::Deps::LMapExp map_m = edge_label.desc().mapExp();
      _gen.initG(graph[V], edge_label);
      boost::graph_traits<SB::Deps::Graph>::out_edge_iterator alg_edge, alg_out_edge_end;
      // For all G in Succ(A_i)
      for (boost::tie(alg_edge, alg_out_edge_end) = out_edges(A, graph); alg_edge != alg_out_edge_end; ++alg_edge) {
        SB::Deps::Vertex G = boost::target(*alg_edge, graph);
        if (graph[G].desc().type() == SB::Deps::VERTEX::Equation) {
          SB::Deps::SetEdge alg_label = graph[*alg_edge];
          int dep, deps = graph[G].desc().numDeps();
          for (dep = 1; dep <= deps; dep++) {
            // Ai -> Gij labels must contain only one pair.
            // Get the subset of algebraic variables from F.
            SB::Set map_u_dom = map_m_u.wholeDom();
            SB::Set a_subset = map_m_u.image(map_u_dom);
            // Get the preimage of the algebraic variables to match against the out edges to equation definitions.
            SB::Set alg_variable_dom_edges = _map_U.preImage(a_subset);
            SB::Set alg_label_dom = alg_label.mapU().wholeDom();
            SB::Set d_intersect = alg_label_dom.cap(alg_variable_dom_edges);
            SB::Deps::LMapExp G_map = alg_label.desc().mapExp();
            if (!d_intersect.empty()) {
              // Get the map exp for the intersection
              SB::Deps::LMapExp g_map = graph[G].desc().depState(dep).nMap();
              SB::Deps::LMapExp n_map = g_map.compose(G_map.solve(map_m));
              // Get the the influencer equation egde dom.
              SB::Set inf_vars = alg_label.mapU().image(d_intersect);
              assert(!inf_vars.empty());
              SB::Set inf_vars_by_edge = a_subset.cap(inf_vars);
              SB::Set inf_eq_dom = map_m_u.preImage(inf_vars_by_edge);
              // Get the g subset that we arrived from the a variables.
              SB::Set g_subset = alg_label.mapF().image(d_intersect);
              // Get the pre-image for the F maps.
              SB::Set g_pre_image = _map_F.preImage(g_subset);
              SB::Deps::VariableDep state_dep = graph[G].desc().depState(dep);
              SB::Set state_intersection = state_dep.mapF().wholeDom().cap(g_pre_image);
              if (!state_intersection.empty()) {
                num_gen++;
                SB::Set u_dom = state_intersection;
                SB::Set f_dom = inf_eq_dom;
                // Map dep_map = n_map;
                /*if (state_dep.isRecursive()) {
                  //dep_map = g_map;
                  u_dom = g_dom;
                  f_dom = state_dep.range();
                }*/
                SB::Deps::VariableDep var_dep(state_dep.var(), map_m_f, state_dep.mapU(), alg_label.desc().exp(), state_dep.isRecursive(),
                                              f_dom, u_dom, n_map, map_m, state_dep.varOffset(), graph[V].id());
                /*if (graph[A].var() == visiting_alg) {
                  var_dep.setAlgDom(dep_dom);
                  var_dep.setAlgEq(graph[G].eq());
                } else if (state_dep.hasAlgDeps()) {
                  var_dep.setAlgDom(state_dep.algDom());
                  var_dep.setAlgEq(state_dep.algEq());
                }*/
                SB::Deps::VertexDesc update_desc = graph[V].desc();
                update_desc.setDepState(num_gen, var_dep);
                graph[V].updateDesc(update_desc);
                _gen.visitG(graph[V], graph[G], var_dep, _index_shift[graph[G].index()]);
              }
            }
          }
        }
      }
    }
  }
  // For X_i in Succ(V)
  for (boost::tie(edge, out_edge_end) = out_edges(V, graph); edge != out_edge_end; ++edge) {
    SB::Deps::SetEdge edge_label = graph[*edge];
    SB::Deps::Vertex X = boost::target(*edge, graph);
    if (graph[X].desc().type() == SB::Deps::VERTEX::Influencee) {
      // For all map_n in E_V
      num_gen++;
      SB::Deps::VertexDesc update_desc = graph[V].desc();
      SB::Deps::VariableDep var_dep(graph[X].desc().var(), edge_label.mapF(), edge_label.mapU(), edge_label.desc().exp(),
                                    edge_label.desc().mapExp(), graph[X].id(), graph[V].id());
      update_desc.setDepState(num_gen, var_dep);
      // update_desc.setMap(num_gen, edge_label.mapU());
      graph[V].updateDesc(update_desc);
      _gen.visitF(graph[V], var_dep);
      /// Add algebraic recursive uses.
      /*for (SB::Deps::SetEdge rec_alg_use: rec_alg_use_maps) {
        num_gen++;
        Map use_map = rec_alg_use.compose(map);
        MDI applied_map = use_map.apply(pair.dom(), pair.ran());
        Option<MDI> range = pair.dom().intersection(applied_map);
        assert(range);
        const bool RECURSIVE = true;
        SB::Deps::VariableDep rec_alg_dep(graph[X].var(), pair.dom(), range.get(), RECURSIVE);
        graph[V].setDepState(num_gen, rec_alg_dep);
        graph[V].setMap(num_gen, use_map);
        _gen.visitF(graph[V], VariableDep(graph[X].var(), pair.dom(), range.get()), use_map);
      }*/
    }
  }
  SB::Deps::updateNumDeps(graph, V, num_gen);
  _gen.end();
}

template class SBDependencies<JacMatrixGenerator, JacMatrixDef>;

template class SBDependencies<JacGenerator, JacDef>;

}  // namespace Deps
}  // namespace MicroModelica