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

#include "eq_graph_builder.h"

#include <util/model_config.h>
#include <util/logger.h>
#include <util/util_types.h>

using namespace SB;

using namespace SB::Deps;

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

struct EqNodes {
  EqNodes(EquationTable eqs) : _equations(eqs) {}
  list<Expression> getExpressions(SB::Deps::SetVertex n, SB::Deps::EDGE::Type type)
  {
    list<Expression> exps;
    Equation eq = getEquation(n, _equations);
    exps.push_back((type == SB::Deps::EDGE::Output) ? eq.rhs() : eq.lhs());
    return exps;    
  }

  protected:
  IR::EquationTable _equations;
};

template<typename S, typename T>
EQGraphBuilder<S, T>::EQGraphBuilder(T &equations, EquationTable &algebraics, IR::STATEMENT::AssignTerm search) 
    : EQGraphBuilder(equations, algebraics)
{
}

template<typename S, typename T>
EQGraphBuilder<S, T>::EQGraphBuilder(T &equations, EquationTable &algebraics) 
    : _F_nodes(),
      _G_nodes(),
      _g_nodes(),
      _u_nodes(),
      _equations(equations),
      _algebraics(algebraics),
      _node_names(),
      _usage()
{
}

template<typename S, typename T>
SB::Deps::Graph EQGraphBuilder<S, T>::build()
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
      SB::Deps::SetVertex state_node = createSetVertex(var, edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencee);      
      _u_nodes.push_back(addVertex(state_node, graph));
    } else if (var.isAlgebraic()) {
      SB::Deps::SetVertex alg_node = createSetVertex(var, edge_dom_offset, max_dims, SB::Deps::VERTEX::Algebraic);      
      _g_nodes.push_back(addVertex(alg_node, graph));
    }
  }
  EquationTable::iterator eq_it;
  for (Equation eq = _equations.begin(eq_it); !_equations.end(eq_it); eq = _equations.next(eq_it)) {
    SB::Deps::SetVertex ifr_node = createSetVertex(eq, edge_dom_offset, max_dims, SB::Deps::VERTEX::Influencer, _usage);
    _F_nodes.push_back(addVertex(ifr_node, graph));
  }
  for (Equation eq = _algebraics.begin(eq_it); !_algebraics.end(eq_it); eq = _algebraics.next(eq_it)) {
    SB::Deps::SetVertex alg_node = createSetVertex(eq, edge_dom_offset, max_dims, SB::Deps::VERTEX::Equation, _usage);
    _G_nodes.push_back(addVertex(alg_node, graph));
  }

  EqNodes nodes(_equations);

  computeOutputEdges<EqNodes>(_F_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeOutputEdges<EqNodes>(_F_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeOutputEdges<EqNodes>(_G_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  computeInputOutputEdges<EqNodes>(_G_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset, nodes);

  SB::Deps::GraphPrinter printer(graph);

  printer.printGraph(Logger::instance().getLogsPath()+ S::name() + ".dot");

  return graph;
}

template class EQGraphBuilder<SB::StateSelector<SDG>, IR::EquationTable>;

template class EQGraphBuilder<SB::StateSelector<SZG>, IR::EquationTable>;

template class EQGraphBuilder<SB::StateSelector<SOG>, IR::EquationTable>;

template class EQGraphBuilder<SB::DiscreteSelector<DOG>, IR::EquationTable>;

}  // namespace Deps
}  // namespace MicroModelica