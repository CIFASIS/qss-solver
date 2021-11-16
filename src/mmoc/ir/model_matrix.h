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

#include <string>

#include <deps/builders/eq_graph_builder.h>
#include <deps/builders/ev_graph_builder.h>
#include <deps/sb_dependencies.h>
#include <ir/model_matrix_gen.h>

namespace MicroModelica {
namespace IR {

template<typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
class ModelMatrix {
  public:
  ModelMatrix();
  ~ModelMatrix() = default;

  void build(Config config);

  void setMode(MATRIX::Mode mode);
  std::string alloc();
  std::string init();
  std::string accessVector() const;
  bool empty();

  protected:
  ModelMatrixDef _model_matrix_def;
  MATRIX::Mode _mode;
  std::string _access[2];
};

typedef ModelMatrix<Deps::SDSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable> SDMatrix;

typedef ModelMatrix<Deps::SZSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable> SZMatrix;

typedef ModelMatrix<Deps::SOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable> SOMatrix;

typedef ModelMatrix<Deps::DOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable> DOMatrix;

typedef ModelMatrix<Deps::DSCGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable> LHSDSCMatrix;

typedef ModelMatrix<Deps::LHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable> LHSSTMatrix;

typedef ModelMatrix<Deps::RHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable> RHSSTMatrix;

}  // namespace IR
}  // namespace MicroModelica
