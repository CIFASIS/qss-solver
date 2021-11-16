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

#pragma once

#include <ir/equation.h>
#include <ir/event.h>
#include <ir/model_matrix.h>
#include <util/symbol_table.h>
#include "dependency_matrix.h"
#include "dependency.h"
#include <deps/sb_dependencies.h>

namespace MicroModelica {
namespace Deps {

class ModelDependencies {
  public:
  ModelDependencies();
  ~ModelDependencies() = default;
  IR::SDMatrix SD();
  IR::SDMatrix DS();
  IR::SZMatrix SZ();
  IR::SZMatrix ZS();
  IR::SOMatrix SO();
  IR::SOMatrix OS();
  IR::DOMatrix DO();
  IR::DOMatrix OD();
  VariableDependencyMatrix HD();
  VariableDependencyMatrix DH();
  VariableDependencyMatrix HZ();
  VariableDependencyMatrix ZH();
  VariableDependencyMatrix HH();
  IR::LHSDSCMatrix LHSDsc();
  IR::LHSSTMatrix LHSSt();
  IR::RHSSTMatrix RHSSt();
  JacobianMatrix JAC();

  void compute(IR::EquationTable eqs, IR::EquationTable outputs, IR::EquationTable algs, IR::EventTable events);

  private:
  IR::SDMatrix _SD;
  IR::SOMatrix _SO;
  IR::DOMatrix _DO;
  IR::SZMatrix _SZ;
  IR::LHSSTMatrix _LHSSt;
  IR::RHSSTMatrix _RHSSt;
  IR::LHSDSCMatrix _LHSDsc;
  VariableDependencyMatrix _HD;
  VariableDependencyMatrix _HZ;
  VariableDependencyMatrix _HH;

  JacobianMatrix _JAC;

  Dependency _deps;
};
}  // namespace Deps
}  // namespace MicroModelica
