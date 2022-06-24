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

#include "model_matrix.h"

#include <sstream>

#include <deps/builders/eq_graph_builder.h>
#include <ir/user_def_matrix.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace Deps;
using namespace SB;
using namespace Util;
namespace IR {

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::ModelMatrix() : _mode(MATRIX::Normal), _access({"",""})
{
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
void ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::build(Config config)
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  MatrixBuilder model_matrix;
  IndexShiftBuilder index_shifts(algebraics);
  T nodes = config.selector.getNodes();
  GraphBuilder SBGraph(nodes, algebraics, config.search);
  model_matrix.setup(config);
  model_matrix.compute(SBGraph.build(), index_shifts.build());
  _model_matrix_def = model_matrix.def();
  _access[0] = model_matrix.config().access[0];
  _access[1] = model_matrix.config().access[1];
  // Add any user definition entry if any.
  UserDefMatrix<Config, N> user_def(config);
  user_def.compute();
  append(user_def.def());
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::alloc()
{
  return _model_matrix_def.alloc[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::init()
{
  return _model_matrix_def.init[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::accessVector() const
{
  return _access[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
bool ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::empty()
{
  return _model_matrix_def.alloc[MATRIX::Normal].empty();
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
void ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::setMode(MATRIX::Mode mode)
{
  _mode = mode;
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
ModelMatrixDef ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::def()
{
  return _model_matrix_def;
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T, typename N>
void ModelMatrix<GraphBuilder, MatrixBuilder, Config, T, N>::append(ModelMatrixDef def)
{
  _model_matrix_def.alloc[MATRIX::Normal].append(def.alloc[MATRIX::Normal]);
  _model_matrix_def.init[MATRIX::Normal].append(def.init[MATRIX::Normal]);
  _model_matrix_def.alloc[MATRIX::Transpose].append(def.alloc[MATRIX::Transpose]);
  _model_matrix_def.init[MATRIX::Transpose].append(def.init[MATRIX::Transpose]); 
}

template class ModelMatrix<SDSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable, IR::Equation>;

template class ModelMatrix<SZSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable, IR::Equation>;

template class ModelMatrix<SOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable, IR::Equation>;

template class ModelMatrix<DOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable, IR::Equation>;

template class ModelMatrix<Deps::DSCGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::LHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::RHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::HDGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::HZGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::HZSTGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

template class ModelMatrix<Deps::HHGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable, IR::Event>;

}  // namespace IR
}  // namespace MicroModelica
