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

#ifndef DEPS_SB_GRAPH_H_
#define DEPS_SB_GRAPH_H_

#include <iostream>
#include <utility>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../../ir/expression.h"
#include "../../ir/equation.h"
#include "../../ir/index.h"
#include "../../ir/statement.h"
#include "../../util/symbol_table.h"
#include "sb_graph_helpers.h"

namespace MicroModelica {
namespace Deps {
namespace SBG {

namespace VERTEX {
typedef enum { Equation, Influencer, Algebraic, Influencee, Statement, Derivative } Type;

typedef enum { RHS, LHS } Eval;
}  // namespace VERTEX

namespace EDGE {
typedef enum { Input, Output } Direction;
} 

class VariableDep {
  public:
  VariableDep();
  VariableDep(Util::Variable var, MDI dom, MDI range);
  VariableDep(Util::Variable var, MDI dom, MDI range, bool recursive);
  ~VariableDep() = default;
  
  MDI range();
  MDI dom();
  Util::Variable var();
  bool isRecursive() const;
  
  protected:
  Util::Variable _var;
  MDI _dom;
  MDI _range;
  bool _recursive;
};

typedef std::map<int, VariableDep> VariableDeps;

class StatementVertex {
  public:
  StatementVertex() : _event(), _exp(), _range() {}
  StatementVertex(const StatementVertex& other)
  {
    _event = other._event;
    _exp = other._exp;
    _range = other._range;
  }
  IR::Expression event() { return _event; }
  void setEvent(IR::Expression event) { _event = event; }
  IR::Expression exp() { return _exp; }
  void setExp(IR::Expression exp) { _exp = exp; }
  Option<IR::Range> range() { return _range; }
  void setRange(Option<IR::Range> range) { _range = range; }

  private:
  IR::Expression _event;
  IR::Expression _exp;
  Option<IR::Range> _range;
};

class VertexProperty {
  public:
  VertexProperty();
  VertexProperty(const VertexProperty& other);
  Util::Variable var();
  void setVar(Util::Variable var);
  VERTEX::Type type();
  void setType(VERTEX::Type type);
  IR::Expression exp();
  void setExp(IR::Expression exp);
  IR::Equation eq();
  void setEq(IR::Equation eq);
  StatementVertex stm();
  void setStm(StatementVertex stm);
  int id();
  void setId(int id);
  bool visited() const;
  void setVisited(bool visited);
  int numDeps() const;
  void setNumDeps(int num_deps);
  void setDepState(int id, VariableDep var_dep);
  VariableDep depStates(int id);
  void setMap(int id, Map map);
  Maps maps() const;
  Map map(int id);
  int size();
  std::string name() const;
  void setName(std::string name);

  private:
  VERTEX::Type _type;
  IR::Expression _exp;
  IR::Equation _eq;
  StatementVertex _stm;
  int _id;
  Util::Variable _var;
  bool _visited;
  int _num_deps;
  VariableDeps _var_deps;
  Maps _maps;
  std::string _name;
};

class Label {
  public:
  Label();
  Label(PairSet pairs, EDGE::Direction dir = EDGE::Output);
  const PairSet& pairs() const;
  friend std::ostream& operator<<(std::ostream& os, const Label& label);

  private:
  PairSet _pairs;
  EDGE::Direction _dir;
};

typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, VertexProperty, Label> SBGraph;
/// @brief A vertex of the Incidence graph
typedef boost::graph_traits<SBGraph>::vertex_descriptor SBVertex;
typedef SBVertex F_Vertex;
typedef SBVertex S_Vertex;

typedef SBGraph::edge_descriptor SBEdge;

class EvalOccur {
  public:
  EvalOccur(IR::Expression exp, Option<IR::Range> range);
  ~EvalOccur() = default;
  bool hasIndex();
  IntervalList intervals();
  Usage usages();
  Offset offsets();
  LinearFunction linearFunctions();

  private:
  void initialize();
  std::string reference(AST_Expression exp);
  int constant(AST_Expression exp);
  int step(AST_Expression exp);
  int getBinopInteger(AST_Expression exp);

  IR::Expression _exp;
  AST_Expression_ComponentReference _cr;
  Option<IR::Range> _range;
  std::vector<int> _offsets;
  std::vector<int> _factors;
  std::vector<int> _constants;
  Usage _usages;
  IntervalList _intervals;
};

class BuildEdge {
  public:
  BuildEdge(struct VertexProperty source, struct VertexProperty sink, EDGE::Direction dir = EDGE::Output,
            VERTEX::Eval eval = VERTEX::RHS);
  ~BuildEdge() = default;

  bool exists();
  PairSet indexes();

  protected:
  void initialize();
  void build(list<IR::Expression> exps);
  Option<IR::Range> range(struct VertexProperty sink);
  /**
   * @brief      Checks wheter the node belong to an event handler of zc.
   *
   * @param[in]  sink  The node to evaluate.
   *
   * @return     True if the node is builded from a zc function or handler
   *             statement, False otherwise.
   */
  bool sinkIsEvent();
  bool sinkIsOutput();
  MDI getScalarMDI();
  MDI getScalarMDI(IR::Expression exp);

  private:
  VertexProperty _source;
  VertexProperty _sink;
  bool _exist;
  PairSet _pairs;
  EDGE::Direction _dir;
  VERTEX::Eval _eval;
};
}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica
#endif