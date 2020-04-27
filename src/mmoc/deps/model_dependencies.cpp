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

static constexpr char *INT_CONTAINER = "modelData->";
static constexpr char *OUT_CONTAINER = "modelOutput->";
static constexpr char *STATES = "states";
static constexpr char *EVENTS = "events";
static constexpr char *OUTPUTS = "outputs";
static constexpr char *DISCRETES = "discretes";
static string EMPTY_COMPONENT = {"", ""};

static MatrixConfig DSCfg = {INT_CONTAINER, {"nDS", "nSD", "DS", "SD"}, {STATES, STATES}, EMPTY_COMPONENT};
static MatrixConfig SZCfg = {INT_CONTAINER, {"nSZ", "nZS", "SZ", "ZS"}, {STATES, EVENTS}, EMPTY_COMPONENT};
static MatrixConfig SOCfg = {OUT_CONTAINER, {"nSO", "nOS", "SO", "OS"}, {STATES, OUTPUTS}, EMPTY_COMPONENT};
static MatrixConfig DOCfg = {OUT_CONTAINER, {"nDO", "nOD", "DO", "OD"}, {DISCRETES, OUTPUTS}, EMPTY_COMPONENT};
static MatrixConfig HHCfg = {INT_CONTAINER, {"nHE", "nEH", "HE", "EH"}, {EVENTS, EVENTS}, EMPTY_COMPONENT};
static MatrixConfig HDCfg = {INT_CONTAINER, {"nHD", "nDH", "HD", "DH"}, {EVENTS, STATES}, EMPTY_COMPONENT};
static MatrixConfig HZCfg = {INT_CONTAINER, {"nHZ", "nZH", "HZ", "ZH"}, {EVENTS, EVENTS}, EMPTY_COMPONENT};
static MatrixConfig LHSDSCCfg = {INT_CONTAINER, {"event", "event", "event", "event"}, {EVENTS, EVENTS}, {"nLHSDsc", "LHSDsc"}};
static MatrixConfig LHSSTCfg = {INT_CONTAINER, {"event", "event", "event", "event"}, {EVENTS, EVENTS}, {"nLHSSt", "LHSSt"}};
static MatrixConfig RHSSTCfg = {INT_CONTAINER, {"event", "event", "event", "event"}, {EVENTS, EVENTS}, {"nRHSSt", "RHSSt"}};

ModelDependencies::ModelDependencies()
    : _DS(DSCfg),
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

void ModelDependencies::compute(EquationTable eqs, EquationTable outputs, EquationTable algs, EventTable events, VarSymbolTable symbols)
{
  Utils::instance().setSymbols(symbols);
  SDGraphBuilder SD = SDGraphBuilder(eqs, algs, symbols);
  _deps.compute(SD.build(), _DS, TRAVERSE::Equation);

  VariableDependencyMatrix DS_int(EmptyCfg);
  DSGraphBuilder DS = DSGraphBuilder(eqs, algs, symbols);
  _deps.compute(DS.build(), DS_int);
  DHGraphBuilder LHSDsc = DHGraphBuilder(events, algs, symbols);
  _deps.compute(LHSDsc.build(), _LHSDsc);
  _deps.merge(_LHSDsc, DS_int, _HD);

  VariableDependencyMatrix DZ_int(HHCfg);
  DZGraphBuilder DZ = DZGraphBuilder(events, algs, symbols);
  _deps.compute(DZ.build(), DZ_int);
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

  VariableDependencyMatrix HZ_int(HHCfg);
  _deps.merge(_LHSSt, _SZ, HZ_int);
  _deps.append(_HZ, HZ_int);

  OutputGraphBuilder SO = OutputGraphBuilder(outputs, algs, symbols);
  _deps.compute(SO.build(), _SO);
  OutputGraphBuilder DO = OutputGraphBuilder(outputs, algs, symbols, OUTPUT::DO);
  _deps.compute(DO.build(), _DO);

  EAGraphBuilder DA = EAGraphBuilder(eqs, algs, symbols);
  _deps.compute(DA.build(), _DA, TRAVERSE::Equation);

  EAGraphBuilder ZCA = EAGraphBuilder(events, algs, symbols);
  _deps.compute(ZCA.build(), _ZCA, TRAVERSE::Equation);

  EAGraphBuilder OA = EAGraphBuilder(outputs, algs, symbols);
  _deps.compute(OA.build(), _OA, TRAVERSE::Equation);
}
}  // namespace Deps
}  // namespace MicroModelica