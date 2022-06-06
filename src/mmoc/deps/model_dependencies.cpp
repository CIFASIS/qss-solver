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

ModelDependencies::ModelDependencies()
    : _SD(), _SO(), _DO(), _SZ(), _LHSSt(), _RHSSt(), _LHSDsc(), _HD(), _HZ(), _HH(), _JAC()
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

HDMatrix ModelDependencies::HD()
{
  _HD.setMode(IR::MATRIX::Transpose);
  return _HD;
}

HDMatrix ModelDependencies::DH()
{
  _HD.setMode(IR::MATRIX::Normal);
  return _HD;
}

HZMatrix ModelDependencies::HZ()
{
  _HZ.setMode(IR::MATRIX::Transpose);
  return _HZ;
}

HZMatrix ModelDependencies::ZH()
{
  _HZ.setMode(IR::MATRIX::Normal);
  return _HZ;
}

IR::HHMatrix ModelDependencies::HH()
{
  _HH.setMode(IR::MATRIX::Normal);
  return _HH;
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

  IR::MATRIX::EVMatrixConfig HDCfg(INT_CONTAINER, vector<string>{"nDH", "nHD", "DH", "HD"}, vector<string>{STATES, EVENTS},
                                       vector<string>{"", ""}, STATEMENT::LHS, EVSelector(events));
  _HD.build(HDCfg);

  IR::MATRIX::EVMatrixConfig HZCfg(INT_CONTAINER, vector<string>{"nZH", "nHZ", "ZH", "HZ"}, vector<string>{EVENTS, EVENTS},
                                       vector<string>{"", ""}, STATEMENT::LHS, EVSelector(events));
  _HZ.build(HZCfg);

  IR::HZSTMatrix HZST;
  HZST.build(HZCfg);
  _HZ.append(HZST.def());

  IR::MATRIX::EVMatrixConfig HHCfg(INT_CONTAINER, vector<string>{"nHH", "nHH", "HH", "HH"}, vector<string>{EVENTS, EVENTS},
                                       vector<string>{"", ""}, STATEMENT::LHS, EVSelector(events));
  _HH.build(HHCfg);

  ModelConfig::instance().unsetLocalInitSymbols();
}
}  // namespace Deps
}  // namespace MicroModelica