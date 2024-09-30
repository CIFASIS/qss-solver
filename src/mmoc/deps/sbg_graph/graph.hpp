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

#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <variant>

#include <deps/sbg_graph/set.h>
#include <deps/sbg_graph/pw_lmap.h>

namespace SB {

template <typename D>
struct SetVertex {
  SetVertex() : _name(), _id(-1), _index(0), _desc()
  {
    Set empty;
    _range = empty;
  };

  SetVertex(string name, Set range) : _name(), _id(-1), _range(range), _index(0), _desc(){};

  SetVertex(string name, int id, Set range, D desc) : _name(name), _id(id), _range(range), _index(0), _desc(desc){};

  SetVertex(string name, int id, Set range, int index, D desc) : _name(name), _id(id), _range(range), _index(index), _desc(desc){};

  Set range() { return _range; }

  bool operator==(const SetVertex &other) const { return _id == other._id; }

  size_t hash() { return _id; }

  string name() const { return _name; }

  int id() const { return _id; }

  int index() const { return _index; }

  D desc() const { return _desc; }

  void updateDesc(D desc) { _desc = desc; }

  protected:
  string _name;
  int _id;
  Set _range;
  int _index;
  D _desc;
};

template <typename T>
size_t hash_value(SetVertex<T> v);

template <typename D>
struct SetEdge {
  SetEdge() : _name(), _id(-1), _index(0), _desc()
  {
    PWLMap empty;
    _map_f = empty;
    _map_u = empty;
  };

  SetEdge(string name, PWLMap map_f, PWLMap map_u) : _name(name), _id(-1), _map_f(map_f), _map_u(map_u), _index(0), _desc(){};

  SetEdge(string name, PWLMap map_f, PWLMap map_u, D desc) : _name(name), _id(-1), _map_f(map_f), _map_u(map_u), _index(0), _desc(desc){};

  SetEdge(string name, int id, PWLMap map_f, PWLMap map_u, int index)
      : _name(name), _id(-1), _map_f(map_f), _map_u(map_u), _index(index), _desc(){};

  PWLMap mapF() const { return _map_f; }

  PWLMap mapU() const { return _map_u; }

  int id() const { return _id; }

  string name() const { return _name; }

  D desc() const { return _desc; }

  void updateDesc(D desc) { _desc = desc; }

  protected:
  string _name;
  int _id;
  PWLMap _map_f;
  PWLMap _map_u;
  int _index;
  D _desc;
};

// Default undirected graph definition.
typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, SetVertex<std::monostate>, SetEdge<std::monostate>> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef std::list<Vertex> VertexList;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIt;
typedef Graph::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

}  // namespace SB