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

#include <deps/dependency_matrix.h>
#include <deps/graph/graph_helpers.h>
#include <deps/graph/graph.h>
#include <util/logger.h>

namespace MicroModelica {
namespace Deps {

namespace TRAVERSE {
typedef enum { Variable, Equation } Init;
}

typedef std::list<std::pair<Vertex, MDI>> VertexInfo;

class Dependency {
  public:
  Dependency() : _ifr(), _ifr_dom(), _ifr_range(), _ifr_id(0), _equation_range(), _visited(), _remove_visited(true){};
  ~Dependency() = default;
  template <class DM>
  void compute(DepsGraph graph, DM& dm, TRAVERSE::Init traverse = TRAVERSE::Variable, bool remove_visited = true)
  {
    _remove_visited = remove_visited;
    for (Vertex vertex : boost::make_iterator_range(vertices(graph))) {
      VertexProperty vertex_info = graph[vertex];
      if (vertex_info.type() == VERTEX::Influencer) {
        Paths var_deps;
        AlgebraicPath algs;
        Util::LOG << "Compute paths for: " << vertex_info.var() << endl;
        paths(graph, vertex, variableRange(vertex_info.var()), var_deps, algs, traverse);
        insert(dm, vertex_info, var_deps);
      }
    }
  }

  void merge(VariableDependencyMatrix& source, VariableDependencyMatrix& target, VariableDependencyMatrix& merge);

  void append(VariableDependencyMatrix& a, VariableDependencyMatrix& b);

  protected:
  void paths(DepsGraph graph, Vertex source_vertex, MDI source_range, Paths& var_deps, AlgebraicPath algs,
             TRAVERSE::Init init, VariableDependency alg_dep = VariableDependency());
  bool recursivePaths(DepsGraph graph, Vertex source_vertex, MDI source_range, VertexInfo& node_info);
  VariableDependency getVariableDependency(string name, MDI dom, MDI ran, int id);
  MDI variableRange(Util::Variable var);
  template <class DM>
  void insert(DM& dm, VertexProperty vp, Paths var_deps)
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
  Option<MDI> intersection(VertexProperty source_vertex_info, MDI source, MDI sink, TRAVERSE::Init traverse);

  private:
  IndexPair _ifr;
  MDI _ifr_dom;
  MDI _ifr_range;
  int _ifr_id;
  IR::Range _equation_range;
  struct Visited {
    bool visited = false;
  };
  map<VariableDependency, Visited> _visited;
  bool _remove_visited;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif  // DEP_DEPENDENCY_H
