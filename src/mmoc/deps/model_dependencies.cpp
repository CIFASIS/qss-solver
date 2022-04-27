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

#include <util/model_config.h>
#include <util/util.h>

#include "builders/dh_graph_builder.h"
#include "builders/ds_graph_builder.h"
#include "builders/dz_graph_builder.h"
#include "builders/ea_graph_builder.h"
#include "builders/sd_graph_builder.h"
#include "builders/so_graph_builder.h"
#include "builders/sz_graph_builder.h"
#include <deps/builders/index_shift_builder.h>

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

static MatrixConfig _HHCfg = {INT_CONTAINER, {"nHE", "nEH", "HE", "EH"}, {EVENTS, EVENTS}, EMPTY_COMPONENT};
static MatrixConfig _HDCfg = {INT_CONTAINER, {"nHD", "nDH", "HD", "DH"}, {EVENTS, STATES}, EMPTY_COMPONENT};
static MatrixConfig _HZCfg = {INT_CONTAINER, {"nHZ", "nZH", "HZ", "ZH"}, {EVENTS, EVENTS}, EMPTY_COMPONENT};

ModelDependencies::ModelDependencies()
    : _SD(), _SO(), _DO(), _SZ(), _LHSSt(), _RHSSt(), _LHSDsc(), _HD(), _HZ(), _HH(), _JAC(), __HD(_HDCfg), __HZ(_HZCfg), __HH(_HHCfg), _deps()
{
}

SDMatrix ModelDependencies::SD() { return _SD; }

SDMatrix ModelDependencies::DS()
{
  _SD.setMode(IR::MATRIX::Transpose);
  return _SD;
}

SZMatrix ModelDependencies::SZ() { return _SZ; }

SZMatrix ModelDependencies::ZS()
{
  _SZ.setMode(IR::MATRIX::Transpose);
  return _SZ;
}

SOMatrix ModelDependencies::SO()
{
  _SO.setMode(IR::MATRIX::Normal);
  return _SO;
}

SOMatrix ModelDependencies::OS()
{
  _SO.setMode(IR::MATRIX::Transpose);
  return _SO;
}

DOMatrix ModelDependencies::DO()
{
  _DO.setMode(IR::MATRIX::Normal);
  return _DO;
}

DOMatrix ModelDependencies::OD()
{
  _DO.setMode(IR::MATRIX::Transpose);
  return _DO;
}

VariableDependencyMatrix ModelDependencies::HD()
{
  __HD.setMode(VDM::Normal);
  return __HD;
}

VariableDependencyMatrix ModelDependencies::DH()
{
  __HD.setMode(VDM::Transpose);
  return __HD;
}

VariableDependencyMatrix ModelDependencies::HZ()
{
  __HZ.setMode(VDM::Normal);
  return __HZ;
}

VariableDependencyMatrix ModelDependencies::ZH()
{
  __HZ.setMode(VDM::Transpose);
  return __HZ;
}

VariableDependencyMatrix ModelDependencies::HH()
{
  __HH.setMode(VDM::Normal);
  return __HH;
}

LHSDSCMatrix ModelDependencies::LHSDsc()
{
  _LHSDsc.setMode(IR::MATRIX::Transpose);
  return _LHSDsc;
}

LHSSTMatrix ModelDependencies::LHSSt()
{
  _LHSSt.setMode(IR::MATRIX::Transpose);
  return _LHSSt;
}

RHSSTMatrix ModelDependencies::RHSSt()
{
  _RHSSt.setMode(IR::MATRIX::Transpose);
  return _RHSSt;
}

JacobianMatrix ModelDependencies::JAC() { return _JAC; }

void ModelDependencies::compute(EquationTable eqs, EquationTable outputs, EquationTable algs, EventTable events)
{
  ModelConfig::instance().setLocalInitSymbols();
  IR::MATRIX::EQMatrixConfig SDCfg(INT_CONTAINER, vector<string>{"nSD", "nDS", "SD", "DS"}, vector<string>{STATES, STATES},
                                   vector<string>{"", ""}, EQSelector(eqs));

  _SD.build(SDCfg);

  _JAC.build();

  IR::MATRIX::EQMatrixConfig SOCfg(OUT_CONTAINER, vector<string>{"nSO", "nOS", "SO", "OS"}, vector<string>{STATES, OUTPUTS},
                                   vector<string>{"", ""}, EQSelector(outputs));
  _SO.build(SOCfg);

  IR::MATRIX::EQMatrixConfig DOCfg(OUT_CONTAINER, vector<string>{"nDO", "nOD", "DO", "OD"}, vector<string>{DISCRETES, OUTPUTS},
                                   vector<string>{"", ""}, EQSelector(outputs));
  _DO.build(DOCfg);

  IR::MATRIX::EQMatrixConfig SZCfg(INT_CONTAINER, vector<string>{"nSZ", "nZS", "SZ", "ZS"}, vector<string>{STATES, EVENTS},
                                   vector<string>{"", ""}, EQSelector(zeroCrossingTable(events)));
  _SZ.build(SZCfg);

  IR::MATRIX::EVMatrixConfig LHSSTCfg(INT_CONTAINER, vector<string>{"event", "event", "event", "event"}, vector<string>{EVENTS, EVENTS},
                                      vector<string>{"nLHSSt", "LHSSt"}, STATEMENT::LHS, EVSelector(events));

  _LHSSt.build(LHSSTCfg);

  IR::MATRIX::EVMatrixConfig RHSSTCfg(INT_CONTAINER, vector<string>{"event", "event", "event", "event"}, vector<string>{EVENTS, EVENTS},
                                      vector<string>{"nRHSSt", "RHSSt"}, STATEMENT::RHS, EVSelector(events));

  _RHSSt.build(RHSSTCfg);

  IR::MATRIX::EVMatrixConfig LHSDSCCfg(INT_CONTAINER, vector<string>{"event", "event", "event", "event"}, vector<string>{EVENTS, EVENTS},
                                       vector<string>{"nLHSDsc", "LHSDsc"}, STATEMENT::LHS, EVSelector(events));
  _LHSDsc.build(LHSDSCCfg);

  IR::MATRIX::EQMatrixConfig HDCfg(INT_CONTAINER, vector<string>{"nHD", "nDH", "HD", "DH"}, vector<string>{EVENTS, STATES},
                                       vector<string>{"", ""}, STATEMENT::LHS, EQSelector(eqs));
  _HD.build(HDCfg);

  IR::MATRIX::EQMatrixConfig HZCfg(INT_CONTAINER, vector<string>{"nHZ", "nZH", "HZ", "ZH"}, vector<string>{EVENTS, EVENTS},
                                       vector<string>{"", ""}, STATEMENT::LHS, EQSelector(zeroCrossingTable(events)));
  _HZ.build(HZCfg);

  IR::MATRIX::EVMatrixConfig HHCfg(INT_CONTAINER, vector<string>{"nHE", "nEH", "HE", "EH"}, vector<string>{EVENTS, EVENTS},
                                       vector<string>{"", ""}, STATEMENT::LHS, EVSelector(events));
  _HH.build(HHCfg);

  ModelConfig::instance().unsetLocalInitSymbols();

  VariableDependencyMatrix DS_int(EmptyCfg);
  DSGraphBuilder DS = DSGraphBuilder(eqs, algs);
  _deps.compute(DS.build(), DS_int);
  DHGraphBuilder LHSDsc = DHGraphBuilder(events, algs);
  MatrixConfig LHSDSCCfg_int = {INT_CONTAINER, {"event", "event", "event", "event"}, {EVENTS, EVENTS}, {"nLHSDsc", "LHSDsc"}};
  VariableDependencyMatrix LHSDsc_int(LHSDSCCfg_int);

  _deps.compute(LHSDsc.build(), LHSDsc_int);
  _deps.merge(LHSDsc_int, DS_int, __HD);

  VariableDependencyMatrix DZ_int(_HHCfg);
  DZGraphBuilder DZ = DZGraphBuilder(events, algs);
  _deps.compute(DZ.build(), DZ_int);
  _deps.merge(LHSDsc_int, DZ_int, __HZ);

  DHGraphBuilder LHSSt = DHGraphBuilder(events, algs, STATEMENT::LHS, DHGRAPHBUILDER::State);
  MatrixConfig LHSSTCfg_int = {INT_CONTAINER, {"event", "event", "event", "event"}, {EVENTS, EVENTS}, {"nLHSSt", "LHSSt"}};
  VariableDependencyMatrix LHSSt_int(LHSSTCfg_int);

  _deps.compute(LHSSt.build(), LHSSt_int);

  VariableDependencyMatrix DD_int(EmptyCfg);
  DHGraphBuilder RHSDsc = DHGraphBuilder(events, algs, STATEMENT::RHS);
  _deps.compute(RHSDsc.build(), DD_int);
  _deps.merge(LHSDsc_int, DD_int, __HH);

  SZGraphBuilder SZ = SZGraphBuilder(events, algs);
  MatrixConfig SZCfg_int = {INT_CONTAINER, {"nSZ", "nZS", "SZ", "ZS"}, {STATES, EVENTS}, EMPTY_COMPONENT};

  VariableDependencyMatrix SZ_int(SZCfg_int);
  _deps.compute(SZ.build(), SZ_int);

  VariableDependencyMatrix HZ_int(_HHCfg);
  _deps.merge(LHSSt_int, SZ_int, HZ_int);
  _deps.append(__HZ, HZ_int);
}
}  // namespace Deps
}  // namespace MicroModelica