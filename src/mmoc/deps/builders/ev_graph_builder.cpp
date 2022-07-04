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

#include <boost/algorithm/string.hpp>
#include <string>

#include "ev_graph_builder.h"

#include <util/model_config.h>
#include <util/logger.h>
#include <util/util_types.h>

using namespace SB;

using namespace SB::Deps;

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

struct EvNodes {
  EvNodes(EventTable evs, IR::STATEMENT::AssignTerm search) : _events(evs), _search(search) {}
  list<Expression> getExpressions(SB::Deps::SetVertex n, SB::Deps::EDGE::Type type)
  {

    ExpressionList exps;
    if (n.desc().type() == SB::Deps::VERTEX::Equation) {
      Equation eq = getEquation(n);
      exps.push_back((type == SB::Deps::EDGE::Output) ? eq.rhs() : eq.lhs());
    } else {
      Event ev = getEvent(n);
      vector<string> tokens;
      boost::split(tokens,n.name(),boost::is_any_of("_"));
      StatementTable stms = (tokens[2] == "POS") ? ev.positiveHandler() : ev.negativeHandler();
      int stm_nbr = stoi(tokens[3]) - 1;
      int asg_nbr = stoi(tokens[4]) - 1;
      Option<Statement> stm = stms[stm_nbr];
      assert(stm);
      ExpressionList assignments = stm->assignments(_search);
      int asg_count = 0;
      for (Expression exp : assignments) {
        if (asg_count == asg_nbr) {
          exps.push_back(exp);
        }
        asg_count++;
      }
    }
    return exps;    
  }

  protected:
  IR::EventTable _events;
  IR::STATEMENT::AssignTerm _search;
};

template<typename S>
EvGraphBuilder<S>::EvGraphBuilder(EventTable events, EquationTable algebraics, IR::STATEMENT::AssignTerm search) 
    : _F_nodes(),
      _G_nodes(),
      _g_nodes(),
      _u_nodes(),
      _events(events),
      _algebraics(algebraics),
      _node_names(),
      _usage(),
      _search(search)
{
}
template<typename S>
SB::Deps::Graph EvGraphBuilder<S>::build()
{
  LOG << endl << "Building " << S::name() << endl;
  SB::Deps::Graph graph;
  int edge_dom_offset = 1;
  int max_dims = ModelConfig::instance().symbols().maxDim();
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (S::selectVariable(var)) {
      SB::Deps::SetVertex ife_node = createSetVertex(var, edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencee);      
      _u_nodes.push_back(addVertex(ife_node, graph));
    } else if (var.isAlgebraic()) {
      SB::Deps::SetVertex alg_node = createSetVertex(var, edge_dom_offset, max_dims, SB::Deps::VERTEX::Algebraic);      
      _g_nodes.push_back(addVertex(alg_node, graph));
    }
  }
  EventTable::iterator ev_it;
  for (Event ev = _events.begin(ev_it); !_events.end(ev_it); ev = _events.next(ev_it)) {
    list<SB::Deps::SetVertex> ifr_nodes = createSetVertex(ev, edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencer, _usage, _search);
    for (SB::Deps::SetVertex ifr_node : ifr_nodes) {
      _F_nodes.push_back(addVertex(ifr_node, graph));
    }
  }
  EquationTable::iterator eq_it;
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    Option<SB::Deps::SetVertex> alg_node = createSetVertex(eq, edge_dom_offset, max_dims, SB::Deps::VERTEX::Equation, _usage);
    if (alg_node) {
      _G_nodes.push_back(addVertex(alg_node.get(), graph));
    }
  }

  EvNodes nodes(_events, _search);

  computeOutputEdges<EvNodes>(_F_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeOutputEdges<EvNodes>(_F_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeOutputEdges<EvNodes>(_G_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeInputOutputEdges<EvNodes>(_G_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  SB::Deps::GraphPrinter printer(graph);

  printer.printGraph(Logger::instance().getLogsPath()+ S::name() + ".dot");

  return graph;
}

template class EvGraphBuilder<StateSelector<STLG>>;

template class EvGraphBuilder<StateSelector<STRG>>;

template class EvGraphBuilder<DiscreteSelector<DHG>>;

template class EvGraphBuilder<SB::DiscreteSelector<DRHG>>;

}  // namespace Deps
}  // namespace MicroModelica
