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

#ifndef DEPS_GRAPH_H_
#define DEPS_GRAPH_H_

#include <iostream>
#include <utility>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "../../ir/expression.h"
#include "../../ir/equation.h"
#include "../../ir/index.h"
#include "../../ir/statement.h"
#include "../../util/symbol_table.h"
#include "graph_helpers.h"

namespace MicroModelica {
namespace Deps {

namespace VERTEX {
typedef enum { Equation, Influencer, Algebraic, Influencee, Statement, Derivative } Type;

typedef enum { RHS, LHS } Eval;
}  // namespace VERTEX

namespace EDGE {
typedef enum { Input, Output } Direction;
}

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
  VertexProperty() : _type(VERTEX::Equation), _exp(), _eq(), _stm(), _id(0), _var() {}
  VertexProperty(const VertexProperty& other)
  {
    _type = other._type;
    _exp = other._exp;
    _eq = other._eq;
    _var = other._var;
    _stm = other._stm;
    _id = other._id;
  }
  Util::Variable var() { return _var; }
  void setVar(Util::Variable var) { _var = var; }
  VERTEX::Type type() { return _type; }
  void setType(VERTEX::Type type) { _type = type; }
  IR::Expression exp() { return _exp; }
  void setExp(IR::Expression exp) { _exp = exp; }
  IR::Equation eq() { return _eq; }
  void setEq(IR::Equation eq) { _eq = eq; }
  StatementVertex stm() { return _stm; }
  void setStm(StatementVertex stm) { _stm = stm; }
  int id() { return _id; }
  void setId(int id) { _id = id; }

  private:
  VERTEX::Type _type;
  IR::Expression _exp;
  IR::Equation _eq;
  StatementVertex _stm;
  int _id;
  Util::Variable _var;
};

class Label {
  public:
  inline Label(){};
  Label(IndexPairSet ips, EDGE::Direction dir = EDGE::Output);
  Label(IndexPair ip, EDGE::Direction dir = EDGE::Output);
  void RemovePairs(IndexPairSet ips);
  void RemoveUnknowns(MDI const unk2remove);
  void RemoveEquations(MDI const mdi);
  unsigned long int EdgeCount();
  inline bool IsEmpty() { return ips.size() == 0; }
  inline const IndexPairSet& Pairs() const { return ips; }
  inline const IndexPair& Pair() const { return _ip; }
  MDI getImage(MDI intersect) const;

  friend std::ostream& operator<<(std::ostream& os, const Label& label);

  private:
  void RemoveDuplicates();

  IndexPairSet ips;
  IndexPair _ip;
  EDGE::Direction _dir;
};

typedef boost::adjacency_list<boost::listS, boost::listS, boost::directedS, VertexProperty, Label> DepsGraph;
/// @brief A vertex of the Incidence graph
typedef boost::graph_traits<DepsGraph>::vertex_descriptor Vertex;
/// @brief An equation vertex is the same as a regular vertex
typedef Vertex EqVertex;
/// @brief An unknown vertex is the same as a regular vertex
typedef Vertex IfrVertex;
/// @brief This is an edge of the scalar causalization graph
typedef Vertex IfeVertex;
/// @brief This is an edge of the scalar causalization graph
typedef Vertex AlgVertex;
/// @brief This is an edge of the scalar causalization graph
typedef DepsGraph::edge_descriptor Edge;

class EvalOccur {
  public:
  EvalOccur(IR::Expression exp, Util::VarSymbolTable symbols, Option<IR::Range> range);
  ~EvalOccur(){};
  bool hasIndex();
  IntervalList intervals();
  Usage usages();
  Offset offsets();

  private:
  void initialize();
  std::string reference(AST_Expression exp);
  int constant(AST_Expression exp);
  int step(AST_Expression exp);
  int getBinopInteger(AST_Expression exp);

  IR::Expression _exp;
  AST_Expression_ComponentReference _cr;
  Util::VarSymbolTable _symbols;
  Option<IR::Range> _range;
  std::vector<int> _offsets;
  Usage _usages;
  IntervalList _intervals;
};

class GenerateEdge {
  public:
  GenerateEdge(struct VertexProperty source, struct VertexProperty sink, Util::VarSymbolTable symbols, EDGE::Direction dir = EDGE::Output,
               VERTEX::Eval eval = VERTEX::RHS);
  ~GenerateEdge() = default;
  inline bool exists() { return _exist; };
  inline IndexPairSet indexes() { return _ips; };

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
  Util::VarSymbolTable _symbols;
  IndexPairSet _ips;
  EDGE::Direction _dir;
  VERTEX::Eval _eval;
};

}  // namespace Deps
}  // namespace MicroModelica
#endif