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

#include "builders/dh_graph_builder.h"
#include "builders/ds_graph_builder.h"
#include "builders/dz_graph_builder.h"
#include "builders/ea_graph_builder.h"
#include "builders/sd_graph_builder.h"
#include "builders/so_graph_builder.h"
#include "builders/sz_graph_builder.h"

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

    static MatrixConfig EmptyCfg = { "", {}, {} };
    static MatrixConfig SDCfg = { INT_CONTAINER, { "nSD", "nDS", "SD", "DS" }, { STATES, STATES } };
    static MatrixConfig SZCfg = { INT_CONTAINER, { "nSZ", "nZS", "SZ", "ZS" }, { STATES, EVENTS } };
    static MatrixConfig SOCfg = { OUT_CONTAINER, { "nSO", "nOS", "SO", "OS" }, { STATES, OUTPUTS } };
    static MatrixConfig DOCfg = { OUT_CONTAINER, { "nDO", "nOD", "DO", "OD" }, { DISCRETES, OUTPUTS } };
    static MatrixConfig HHCfg = { INT_CONTAINER, { "nHE", "nEH", "HE", "EH" }, { EVENTS, EVENTS } };
    static MatrixConfig HDCfg = { INT_CONTAINER, { "nHD", "nDH", "HD", "DH" }, { EVENTS, STATES } };
    static MatrixConfig HZCfg = { INT_CONTAINER, { "nHZ", "nZH", "HZ", "ZH" }, { EVENTS, EVENTS } };
    static MatrixConfig LHSDSCCfg = { INT_CONTAINER, { "nLHSDsc", "nLHSDsc", "LHSDsc", "LHSDsc" }, { EVENTS, EVENTS } };
    static MatrixConfig LHSSTCfg = { INT_CONTAINER, { "nLHSSt", "nLHSSt", "LHSSt", "LHSSt" }, { EVENTS, EVENTS } };
    static MatrixConfig RHSSTCfg = { INT_CONTAINER, { "nRHSSt", "nRHSSt", "RHSSt", "RHSSt" }, { EVENTS, EVENTS } };

    ModelDependencies::ModelDependencies() :
      _SD(SDCfg),
      _SZ(SZCfg),
      _SO(SOCfg),
      _DO(DOCfg),
      _HD(HDCfg),
      _HZ(HZCfg),
      _LHSDsc(LHSDSCCfg),
      _LHSSt(LHSSTCfg),
      _RHSSt(RHSSTCfg),
      _HH(HHCfg),
      _deps()
    {
    }

    void
    ModelDependencies::compute(EquationTable eqs, EquationTable outputs, EquationTable algs, EventTable events, VarSymbolTable symbols)
    {
      Utils::instance().setSymbols(symbols);
      SDGraphBuilder SD = SDGraphBuilder(eqs, algs, symbols);
      _deps.compute(SD.build(), _SD);
      
      VariableDependencyMatrix DS_int(EmptyCfg);
      DSGraphBuilder DS = DSGraphBuilder(eqs, algs, symbols);
      _deps.compute(DS.build(), DS_int);
      DHGraphBuilder LHSDsc = DHGraphBuilder(events, algs, symbols);
      _deps.compute(LHSDsc.build(), _LHSDsc);      
      _deps.merge(_LHSDsc, DS_int, _HD);

      VariableDependencyMatrix DZ_int(HHCfg);
      DZGraphBuilder DZ = DZGraphBuilder(events, algs, symbols);
      _deps.compute(DZ.build(),DZ_int);
      _deps.merge(_LHSDsc, DZ_int, _HZ);

      DHGraphBuilder LHSSt = DHGraphBuilder(events, algs, symbols, STATEMENT::LHS, DHGRAPHBUILDER::State);
      _deps.compute(LHSSt.build(), _LHSSt);

      DHGraphBuilder RHSSt = DHGraphBuilder(events, algs, symbols, STATEMENT::RHS, DHGRAPHBUILDER::State);
      _deps.compute(RHSSt.build(), _RHSSt);      

      VariableDependencyMatrix DD_int(EmptyCfg);
      DHGraphBuilder RHSDsc = DHGraphBuilder(events, algs, symbols, STATEMENT::RHS);
      _deps.compute(RHSDsc.build(), DD_int);
      _deps.merge(_LHSDsc, DD_int, _HH);
  
      SZGraphBuilder SZ = SZGraphBuilder(events, algs, symbols);
      _deps.compute(SZ.build(), _SZ);
      
      OutputGraphBuilder SO = OutputGraphBuilder(outputs, algs, symbols);
      _deps.compute(SO.build(), _SO);
      OutputGraphBuilder DO = OutputGraphBuilder(outputs, algs, symbols, OUTPUT::DO);
      _deps.compute(DO.build(), _DO);

      EAGraphBuilder DA(eqs, algs, symbols);
      _deps.compute(DA.build(), _DA);

      EAGraphBuilder ZCA(events, algs, symbols);
      _deps.compute(ZCA.build(), _ZCA);

      EAGraphBuilder OA(outputs, algs, symbols);
      _deps.compute(OA.build(), _OA);
    }
  }
}