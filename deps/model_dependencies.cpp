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

#include "model_dependencies.h"

#include "../util/util.h"

#include "builders/sd_graph_builder.h"
#include "builders/so_graph_builder.h"

namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Deps {

    static constexpr char* INT_CONTAINER = "modelData->";
    static constexpr char* OUT_CONTAINER = "modelOutput->";
    static constexpr char* STATES = "states";
    static constexpr char* EVENTS = "events";    
    static constexpr char* OUTPUTS = "outputs";
    static constexpr char* DISCRETES = "discretes";    

    static MatrixConfig SDCfg = { INT_CONTAINER, { "nSD", "nDS", "SD", "DS" }, { STATES, STATES } };
    static MatrixConfig SZCfg = { INT_CONTAINER, { "nSZ", "nZS", "SZ", "ZS" }, { STATES, EVENTS } };
    static MatrixConfig SOCfg = { OUT_CONTAINER, { "nSO", "nOS", "SO", "OS" }, { STATES, OUTPUTS } };
    static MatrixConfig DOCfg = { OUT_CONTAINER, { "nDO", "nOD", "DO", "OD" }, { DISCRETES, OUTPUTS } };

    ModelDependencies::ModelDependencies() :
      _SD(SDCfg),
      _SZ(SZCfg),
      _SO(SOCfg),
      _DO(DOCfg),
      _deps()
    {
    }

    void
    ModelDependencies::compute(EquationTable eqs, EquationTable outputs, EquationTable algs, EventTable events, VarSymbolTable symbols)
    {
      Utils::instance().setSymbols(symbols);
      SDGraphBuilder SD = SDGraphBuilder(eqs, algs, symbols);
      _deps.compute(SD.build(), _SD);
      OutputGraphBuilder SO = OutputGraphBuilder(outputs, algs, symbols);
      _deps.compute(SO.build(), _SO);
      OutputGraphBuilder DO = OutputGraphBuilder(outputs, algs, symbols, OUTPUT::DO);
      _deps.compute(DO.build(), _DO);
    }
  }
}
