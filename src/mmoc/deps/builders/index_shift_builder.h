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

#ifndef INDEX_SHIFT_BUILDER_H
#define INDEX_SHIFT_BUILDER_H

#include "../../ir/equation.h"
#include "../../util/symbol_table.h"
#include "../graph/sb_graph_helpers.h"

namespace MicroModelica {
namespace Deps {

class IndexShiftBuilder {
  public:
  IndexShiftBuilder(IR::EquationTable &algebraics, Util::VarSymbolTable &symbols);
  ~IndexShiftBuilder() = default;
  SBG::IndexShift build();

  private:
  IR::EquationTable _algebraics;
  Util::VarSymbolTable _symbols;
  SBG::IndexShift _index_shift;
};

}  // namespace Deps
}  // namespace MicroModelica

#endif /* INDEX_SHIFT_BUILDER_H */
