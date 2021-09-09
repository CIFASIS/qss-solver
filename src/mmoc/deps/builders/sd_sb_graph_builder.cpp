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

#include <util/model_config.h>
#include <util/logger.h>
#include <util/util_types.h>

using namespace SB;

using namespace SB::Deps;

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

SDSBGraphBuilder::SDSBGraphBuilder(EquationTable &equations, EquationTable &algebraics)
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

SB::Deps::Graph SDSBGraphBuilder::build()
{
  LOG << endl << "Creating SD Graph: " << endl;
  SB::Deps::Graph graph;
  int edge_dom_offset = 1;
  int max_dims = ModelConfig::instance().symbols().maxDim();
  // First, add the symbols as vertex.
  VarSymbolTable::iterator it;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isState()) {
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

  computeOutputEdges(_F_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset);

  computeOutputEdges(_F_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset);

  computeOutputEdges(_G_nodes, _u_nodes, graph, max_dims, _usage, edge_dom_offset);

  computeInputOutputEdges(_G_nodes, _g_nodes, graph, max_dims, _usage, edge_dom_offset);

  SB::Deps::GraphPrinter printer(graph);

  printer.printGraph(Logger::instance().getLogsPath()+ "SDGraph.dot");

  return graph;
}

}  // namespace Deps
}  // namespace MicroModelica