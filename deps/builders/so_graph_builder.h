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
#ifndef SO_GRAPH_BUILDER_H
#define SO_GRAPH_BUILDER_H

#include "../../ir/class.h"
#include "../../util/symbol_table.h"
#include "../graph/graph.h"

namespace MicroModelica {
namespace Deps {

namespace OUTPUT {
typedef enum { SO, DO } Type;
}

class OutputGraphBuilder {
  public:
  OutputGraphBuilder(IR::EquationTable &equations, IR::EquationTable &algebraics, Util::VarSymbolTable &symbols, OUTPUT::Type = OUTPUT::SO);
  ~OutputGraphBuilder(){};
  DepsGraph build();

  private:
  list<EqVertex> _equationDescriptors;
  list<IfrVertex> _variableDescriptors;
  map<int, IfeVertex> _outputDescriptors;
  IR::EquationTable _equations;
  IR::EquationTable _algebraics;
  Util::VarSymbolTable _symbols;
  OUTPUT::Type _type;
};
}  // namespace Deps
}  // namespace MicroModelica

#endif /* SO_GRAPH_BUILDER_H */
