/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "sd_sb_graph_builder.h"

#include "../graph/sb_graph_helpers.h"
#include "../../util/util_types.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {
using namespace SBG;

SDSBGraphBuilder::SDSBGraphBuilder(EquationTable &equations, EquationTable &algebraics, VarSymbolTable &symbols)
    : _equation_def_nodes(),
      _equation_lhs_nodes(),
      _state_nodes(),
      _equations(equations),
      _algebraics(algebraics),
      _symbols(symbols),
      _node_names()
{
}

SBGraph SDSBGraphBuilder::build()
{
  SBGraph graph;
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  for (Variable var = _symbols.begin(it); !_symbols.end(it); var = _symbols.next(it)) {
    SBG::VertexProperty vp = SBG::VertexProperty();
    if (var.isState()) {
      vp.setType(SBG::VERTEX::Influencee);
      vp.setVar(var);
      //  Use to differentiate from EA builders need to refactor Edge builder.
      const int NOT_ASSIGNED = -1;
      vp.setId(NOT_ASSIGNED);
      vp.setName(var.name());
      _state_nodes.push_back(add_vertex(vp, graph));
    } else if (var.isAlgebraic()) {
      vp.setType(SBG::VERTEX::Algebraic);
      vp.setVar(var);
      vp.setName(var.name());
      _equation_lhs_nodes.push_back(add_vertex(vp, graph));
    }
  }
  EquationTable::iterator eqit;
  for (Equation eq = _equations.begin(eqit); !_equations.end(eqit); eq = _equations.next(eqit)) {
    SBG::VertexProperty vp = SBG::VertexProperty();
    vp.setType(SBG::VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    vp.setName(nodeName(eq));
    _equation_def_nodes.push_back(add_vertex(vp, graph));
    Option<Variable> assigned = eq.LHSVariable();
    assert(assigned);
    SBG::VertexProperty ifr = SBG::VertexProperty();
    ifr.setType(SBG::VERTEX::Influencer);
    ifr.setVar(assigned.get());
    ifr.setId(eq.id());
    _equation_lhs_nodes.push_back(add_vertex(ifr, graph));
  }
  for (Equation eq = _algebraics.begin(eqit); !_algebraics.end(eqit); eq = _algebraics.next(eqit)) {
    SBG::VertexProperty vp = SBG::VertexProperty();
    vp.setType(SBG::VERTEX::Equation);
    vp.setEq(eq);
    vp.setId(eq.id());
    vp.setName(nodeName(eq));
    _equation_def_nodes.push_back(add_vertex(vp, graph));
  }

  foreach_(F_Vertex sink, _equation_def_nodes)
  {
    foreach_(S_Vertex source, _equation_lhs_nodes)
    {
      BuildEdge edge = BuildEdge(graph[source], graph[sink], _symbols, SBG::EDGE::Output, SBG::VERTEX::LHS);
      if (edge.exists()) {
        PairSet pairs = edge.indexes();
        SBG::Label edge_label(pairs);
        add_edge(source, sink, edge_label, graph);
      }
      // Check RHS too if we are working with algebraics.
      if (graph[source].type() == SBG::VERTEX::Algebraic) {
        BuildEdge edge = BuildEdge(graph[source], graph[sink], _symbols, SBG::EDGE::Input, SBG::VERTEX::LHS);
        if (edge.exists()) {
          PairSet pairs = edge.indexes();
          SBG::Label edge_label(pairs, SBG::EDGE::Input);
          add_edge(sink, source, edge_label, graph);
        }
      }
    }
  }
  foreach_(F_Vertex sink, _equation_def_nodes)
  {
    foreach_(S_Vertex source, _state_nodes)
    {
      BuildEdge edge = BuildEdge(graph[source], graph[sink], _symbols, SBG::EDGE::Input, SBG::VERTEX::LHS);
      if (edge.exists()) {
        PairSet pairs = edge.indexes();
        SBG::Label edge_label(pairs, SBG::EDGE::Input);
        add_edge(sink, source, edge_label, graph);
      }
    }
  }
  return graph;
}

string SDSBGraphBuilder::nodeName(Equation eq)
{
  Option<Variable> var = eq.LHSVariable();
  assert(var);
  string name = var->name();
  stringstream node_name;
  int eq_num = _node_names[name];
  node_name << name << "_" << ++eq_num;
  _node_names[name] = eq_num;
  return node_name.str();
}
}  // namespace Deps
}  // namespace MicroModelica