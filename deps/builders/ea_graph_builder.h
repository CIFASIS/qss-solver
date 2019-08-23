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
#ifndef EA_GRAPH_BUILDER_H
#define EA_GRAPH_BUILDER_H

#include "../../ir/class.h"
#include "../../util/symbol_table.h"
#include "../graph/graph.h"

namespace MicroModelica {
namespace Deps {
class EAGraphBuilder {
  public:
  EAGraphBuilder(IR::EquationTable &equations, IR::EquationTable &algebraics, Util::VarSymbolTable &symbols);
  EAGraphBuilder(IR::EventTable &events, IR::EquationTable &algebraics, Util::VarSymbolTable &symbols);
  ~EAGraphBuilder() = default;
  DepsGraph build();

  private:
  list<EqVertex> _equationDescriptors;
  list<IfrVertex> _sourceDescriptors;
  list<IfeVertex> _algebraicDescriptors;
  std::map<std::string, Util::Variable> _ifrs;
  IR::EventTable _events;
  IR::EquationTable _algebraics;
  IR::EquationTable _equations;
  Util::VarSymbolTable _symbols;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif /* EA_GRAPH_BUILDER_H */
