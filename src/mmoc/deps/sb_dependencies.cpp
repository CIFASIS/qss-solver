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

template <typename IDependencies, typename R, typename S>
SBDependencies<IDependencies, R, S>::SBDependencies() : _index_shift(){};

template <typename IDependencies, typename R, typename S>
R SBDependencies<IDependencies, R, S>::def()
{
  return _gen.def();
}

template <typename IDependencies, typename R, typename S>
void SBDependencies<IDependencies, R, S>::setup(S config)
{
  return _gen.setup(config);
}

template <typename IDependencies, typename R, typename S>
S SBDependencies<IDependencies, R, S>::config()
{
  return _gen.config();
}

template <typename IDependencies, typename R, typename S>
void SBDependencies<IDependencies, R, S>::compute(SB::Deps::Graph graph, SB::Deps::IndexShift index_shift)
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

template <typename IDependencies, typename R, typename S>
void SBDependencies<IDependencies, R, S>::paths(SB::Deps::Graph graph, SB::Deps::Vertex V, Variable visiting_alg)
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
      // Ignore recursive edges.
      if (graph[A].desc().var() == visiting_alg) {
        continue;
      }
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
            int dep, deps = graph[G].desc().numDeps();
            for (dep = 1; dep <= deps; dep++) {
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
                SB::Deps::VariableDep var_dep(state_dep.var(), map_m_f, state_dep.mapU(), alg_label.desc().exp(), false, f_dom, u_dom,
                                              n_map, map_m, state_dep.varOffset(), graph[V].id());
                list<SB::Deps::VariableDep> recursive_deps = state_dep.recursiveDeps();
                // Visit recursive deps.
                for (SB::Deps::VariableDep var_d : recursive_deps) {
                  SB::Set rec_dom = var_d.fDom();
                  SB::Set rec_matched_eq = _map_F.image(rec_dom);
                  VertexIt G_vertex_it = findSetVertex(graph, rec_matched_eq);
                  _gen.visitG(graph[V], graph[*G_vertex_it], var_d, _index_shift[graph[*G_vertex_it].index()]);
                }
                var_dep.setRecursiveDeps(recursive_deps);
                SB::Deps::VertexDesc update_desc = graph[V].desc();
                update_desc.setDepState(num_gen, var_dep);
                graph[V].updateDesc(update_desc);
                _gen.visitG(graph[V], graph[G], var_dep, _index_shift[graph[G].index()]);
              }
            }
            _gen.visitG(graph[V], graph[G], map_m_f, map_m, edge_label.desc().exp(), alg_label.mapF(), G_map, d_intersect);
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
      SB::PWLMap map_u = edge_label.mapU();
      SB::Deps::VertexDesc update_desc = graph[V].desc();
      SB::Deps::VariableDep var_dep(graph[X].desc().var(), edge_label.mapF(), edge_label.mapU(), edge_label.desc().exp(),
                                    edge_label.desc().mapExp(), graph[X].id(), graph[V].id());
      update_desc.setDepState(num_gen, var_dep);
      graph[V].updateDesc(update_desc);
      _gen.visitF(graph[V], var_dep);
      /// Add algebraic recursive uses.
      recursiveDeps(graph, map_u, V, X, num_gen, rec_alg_use_maps);
    }
  }
  SB::Deps::updateNumDeps(graph, V, num_gen);
  _gen.end();
}

template <typename IDependencies, typename R, typename S>
void SBDependencies<IDependencies, R, S>::recursiveDeps(SB::Deps::Graph graph, SB::PWLMap map_u, SB::Deps::Vertex V, SB::Deps::Vertex X,
                                                        int num_gen, list<SB::Deps::SetEdge> rec_alg_use_maps)
{
  for (SB::Deps::SetEdge rec_alg_use : rec_alg_use_maps) {
    // Get the whole vertex of the influenced variable.
    SB::Set map_u_dom = map_u.wholeDom();
    SB::Set map_u_im = map_u.image(map_u_dom);
    SB::Set whole_inf_var = wholeVertex(graph, map_u_im);
    // Get the algebraic variable image of the recursive use.
    SB::PWLMap rec_use = rec_alg_use.mapU();
    SB::Set rec_use_dom = rec_use.wholeDom();
    SB::Set rec_use_im = rec_use.image(rec_use_dom);
    // Get the preImage of the algebraic recursive variables.
    SB::Set alg_var_pre_im = _map_U.preImage(rec_use_im);
    SB::Set rec_map_f_dom = rec_alg_use.mapF().wholeDom();
    alg_var_pre_im = alg_var_pre_im.diff(rec_map_f_dom);
    // Iterate over the preImage to get the set vertex that defines them.
    for (SB::Set atom_set : alg_var_pre_im.atomize()) {
      // Get the F set vertex sub-set for the dom.
      SB::Set matched_f = _map_F.image(atom_set);
      VertexIt F_vertex_it = findSetVertex(graph, matched_f);
      boost::graph_traits<SB::Deps::Graph>::out_edge_iterator rec_edge, rec_out_edge_end;
      // For all A_i in Succ(V)
      for (boost::tie(rec_edge, rec_out_edge_end) = out_edges(*F_vertex_it, graph); rec_edge != rec_out_edge_end; ++rec_edge) {
        SB::Deps::SetEdge rec_edge_label = graph[*rec_edge];
        SB::Set rec_label_dom = rec_edge_label.mapF().preImage(matched_f);
        SB::Set rec_label_im = rec_edge_label.mapU().image(rec_label_dom);
        SB::Set inter = whole_inf_var.cap(rec_label_im);
        if (!inter.empty()) {
          const bool RECURSIVE = true;
          SB::Deps::VariableDep rec_var_dep(graph[X].desc().var(), rec_edge_label.mapF(), rec_edge_label.mapU(), rec_alg_use.desc().exp(),
                                            RECURSIVE, atom_set, rec_label_dom, rec_edge_label.desc().mapExp(), graph[X].id(),
                                            graph[*F_vertex_it].id());
          SB::Deps::VertexDesc update_desc = graph[V].desc();
          SB::Deps::VariableDep orig_var_dep = update_desc.depState(num_gen);
          orig_var_dep.addRecursiveDep(rec_var_dep);
          update_desc.setDepState(num_gen, orig_var_dep);
          graph[V].updateDesc(update_desc);
          _gen.visitF(graph[*F_vertex_it], rec_var_dep, graph[V]);
        }
      }
    }
  }
}

template class SBDependencies<JacMatrixGenerator, JacMatrixDef, EquationTable>;

template class SBDependencies<JacGenerator, JacDef, EquationTable>;

template class SBDependencies<QSSModelGenerator, QSSModelDef, EquationTable>;

template class SBDependencies<QSSModelDepsGenerator, QSSModelDepsDef, EquationTable>;

template class SBDependencies<ModelMatrixGenerator<EquationTable, Equation, MATRIX::EQMatrixConfig>, ModelMatrixDef,
                              MATRIX::EQMatrixConfig>;
template class SBDependencies<ModelMatrixGenerator<EventTable, Event, MATRIX::EVMatrixConfig>, ModelMatrixDef, MATRIX::EVMatrixConfig>;

}  // namespace Deps
}  // namespace MicroModelica