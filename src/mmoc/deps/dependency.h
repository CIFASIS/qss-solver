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

#ifndef DEP_DEPENDENCY_H_
#define DEP_DEPENDENCY_H_

#include <string>

#include "dependency_matrix.h"
#include "graph/graph_helpers.h"
#include "graph/graph.h"

namespace MicroModelica {
namespace Deps {
class Dependency {
  public:
  Dependency() : _ifr(), _ifr_dom(){};
  ~Dependency() = default;
  template <class DM>
  void compute(DepsGraph graph, DM& dm)
  {
    for (Vertex vertex : boost::make_iterator_range(vertices(graph))) {
      VertexProperty vertex_info = graph[vertex];
      if (vertex_info.type() == VERTEX::Influencer) {
        VariableDependencies var_deps;
        AlgebraicDependencies algs;
        AlgebraicDependencies alg_paths;
        cout << "Compute dependecies for: " << vertex_info.var() << endl;
        paths(graph, vertex, variableRange(vertex_info.var()), var_deps, algs, alg_paths);
        insert(dm, vertex_info, var_deps);
      }
    }
  }

  void merge(VariableDependencyMatrix& source, VariableDependencyMatrix& target, VariableDependencyMatrix& merge);

  void append(VariableDependencyMatrix& a, VariableDependencyMatrix& b);

  protected:
  void paths(DepsGraph graph, Vertex source_vertex, MDI source_range, VariableDependencies& var_deps, AlgebraicDependencies& algs,
             AlgebraicDependencies& alg_paths, VariableDependency recursive_alg = VariableDependency());
  void recursivePaths(DepsGraph graph, Vertex source_vertex, MDI source_range, AlgebraicDependencies& algs,
                      AlgebraicDependencies& alg_paths);
  VariableDependency getVariableDependency(string name, MDI dom, MDI ran, int id);
  MDI variableRange(Util::Variable var);
  template <class DM>
  void insert(DM& dm, VertexProperty vp, VariableDependencies var_deps)
  {
    if (!var_deps.empty()) {
      if (dm.keyType() == VDM::String_Key) {
        dm.insert(vp.var().name(), var_deps);
      } else {
        dm.insert(vp.id(), var_deps);
      }
    }
  }
  void print(DepsGraph graph);
  void printEdges(DepsGraph graph, Vertex source_vertex, MDI source_range);
  bool isRecursive(VertexProperty source, VertexProperty target);
  bool isReduction(MDI source_dom, MDI sink_dom);

  private:
  IndexPair _ifr;
  MDI _ifr_dom;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif  // DEP_DEPENDENCY_H
