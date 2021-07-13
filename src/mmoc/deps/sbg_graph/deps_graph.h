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

#include <deps/sbg_graph/graph.h>
#include <deps/sbg_graph/graph_printer.h>
#include <deps/sbg_graph/lmap_exp.h>
#include <deps/sbg_graph/pw_lmap.h>
#include <ir/expression.h>
#include <util/symbol_table.h>

namespace SB {
namespace Deps {

namespace VERTEX {
typedef enum { Equation, Influencer, Algebraic, Influencee, Statement, Derivative } Type;
}  // namespace VERTEX

namespace EDGE {
typedef enum { Input, Output } Type;
}  // namespace EDGE

struct VariableDep {
  VariableDep()
      : _var(),
        _map_f(),
        _map_u(),
        _recursive(false),
        _alg_dom(),
        _alg_eq(),
        _alg_dep(false),
        _exp(),
        _u_dom(),
        _f_dom(),
        _n_map(),
        _m_map(),
        _var_offset(0),
        _eq_offset(0){};
  VariableDep(MicroModelica::Util::Variable var, SB::PWLMap map_f, SB::PWLMap map_u, MicroModelica::IR::Expression exp, LMapExp n_map,
              int var_offset, int eq_offset)
      : _var(var),
        _map_f(map_f),
        _map_u(map_u),
        _recursive(false),
        _alg_dom(),
        _alg_eq(),
        _alg_dep(false),
        _exp(exp),
        _u_dom(),
        _f_dom(),
        _n_map(n_map),
        _m_map(),
        _var_offset(var_offset),
        _eq_offset(eq_offset)
  {
    _u_dom = _map_u.wholeDom();
    _f_dom = _map_f.wholeDom();
  };
  VariableDep(MicroModelica::Util::Variable var, SB::PWLMap map_f, SB::PWLMap map_u, MicroModelica::IR::Expression exp, bool recursive,
              SB::Set f_dom, SB::Set u_dom, LMapExp n_map, LMapExp m_map, int var_offset, int eq_offset)
      : _var(var),
        _map_f(map_f),
        _map_u(map_u),
        _recursive(recursive),
        _alg_dom(),
        _alg_eq(),
        _alg_dep(false),
        _exp(exp),
        _u_dom(u_dom),
        _f_dom(f_dom),
        _n_map(n_map),
        _m_map(m_map),
        _var_offset(var_offset),
        _eq_offset(eq_offset){};

  PWLMap mapF() const { return _map_f; };
  PWLMap mapU() const { return _map_u; };
  MicroModelica::Util::Variable var() const { return _var; };
  bool isRecursive() const { return _recursive; };
  bool hasAlgDeps() const { return _alg_dep; };
  void setAlgDom(SB::Set alg_dom) { _alg_dom = alg_dom; };
  SB::Set algDom() const { return _alg_dom; };
  int algEq() const { return _alg_eq; };
  void setAlgEq(int id)
  {
    _alg_eq = id;
    _alg_dep = true;
  };
  MicroModelica::IR::Expression exp() const { return _exp; };
  SB::Set range()
  {
    SB::Set whole_dom = _map_u.wholeDom();
    return _map_u.image(whole_dom);
  }

  SB::Set uDom() const { return _u_dom; };
  SB::Set fDom() const { return _f_dom; };

  SB::Set variables() { return _map_u.image(_u_dom); };
  SB::Set equations() { return _map_f.image(_f_dom); };

  void setNMap(LMapExp n_map) { _n_map = n_map; }
  void setMMap(LMapExp m_map) { _m_map = m_map; }
  LMapExp nMap() const { return _n_map; }
  LMapExp mMap() const { return _m_map; }
  int varOffset() const { return _var_offset; }
  int eqOffset() const { return _eq_offset; }

  protected:
  MicroModelica::Util::Variable _var;
  PWLMap _map_f;
  PWLMap _map_u;
  bool _recursive;
  SB::Set _alg_dom;
  int _alg_eq;
  bool _alg_dep;
  MicroModelica::IR::Expression _exp;
  SB::Set _u_dom;
  SB::Set _f_dom;
  LMapExp _n_map;
  LMapExp _m_map;
  int _var_offset;
  int _eq_offset;
};

struct VertexDesc {
  VertexDesc() : _type(VERTEX::Influencer), _visited(false), _var(), _deps(), _num_deps(0) {}
  VertexDesc(VERTEX::Type type) : _type(type), _visited(false), _var(), _deps(), _num_deps(0) {}
  VertexDesc(VERTEX::Type type, MicroModelica::Util::Variable var) : _type(type), _visited(false), _var(var), _deps(), _num_deps(0) {}
  VertexDesc(const VertexDesc& other)
  {
    _type = other._type;
    _visited = other._visited;
    _var = other._var;
    _deps = other._deps;
    _num_deps = other._num_deps;
  }
  VERTEX::Type type() { return _type; };
  bool visited() const { return _visited; }
  void setVisited(bool visited) { _visited = visited; }
  MicroModelica::Util::Variable var() const { return _var; }
  void setVar(MicroModelica::Util::Variable var) { _var = var; }
  void setDepState(int num_gen, VariableDep var_dep) { _deps[num_gen] = var_dep; };
  void setNumDeps(int num_deps) { _num_deps = num_deps; };
  int numDeps() const { return _num_deps; };
  VariableDep depState(int id) { return _deps[id]; }

  protected:
  VERTEX::Type _type;
  bool _visited;
  MicroModelica::Util::Variable _var;
  std::map<int, VariableDep> _deps;
  int _num_deps;
};

struct EdgeDesc {
  EdgeDesc() : _exp(), _lmap_exp() {}
  EdgeDesc(MicroModelica::IR::Expression exp, SB::Deps::LMapExp lmap_exp) : _exp(exp), _lmap_exp(lmap_exp) {}
  EdgeDesc(const EdgeDesc& other)
  {
    _exp = other._exp;
    _lmap_exp = other._lmap_exp;
  }

  MicroModelica::IR::Expression exp() const { return _exp; }
  SB::Deps::LMapExp mapExp() const { return _lmap_exp; }

  protected:
  MicroModelica::IR::Expression _exp;
  SB::Deps::LMapExp _lmap_exp;
};

using SetVertex = SB::SetVertex<VertexDesc>;

using SetEdge = SB::SetEdge<EdgeDesc>;

// Directed Dependency Graph def
typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, SetVertex, SetEdge> Graph;
typedef Graph::vertex_descriptor Vertex;
typedef std::list<Vertex> VertexList;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIt;
typedef Graph::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIt;
typedef boost::graph_traits<Graph>::out_edge_iterator OutEdgeIt;

typedef std::map<int, int> IndexShift;

using GraphPrinter = SB::GraphPrinter<Graph, VertexDesc, EdgeDesc, Vertex, VertexIt, EdgeIt>;

}  // namespace Deps

template class GraphPrinter<SB::Deps::Graph, SB::Deps::VertexDesc, SB::Deps::EdgeDesc, SB::Deps::Vertex, SB::Deps::VertexIt,
                            SB::Deps::EdgeIt>;

}  // namespace SB