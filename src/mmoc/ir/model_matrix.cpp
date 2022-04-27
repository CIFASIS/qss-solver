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
#include <util/model_config.h>

namespace MicroModelica {
using namespace Deps;
using namespace Deps::SBG;
using namespace SB;
using namespace Util;
namespace IR {

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::ModelMatrix() : _mode(MATRIX::Normal), _access({"",""})
{
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
void ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::build(Config config)
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
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::alloc()
{
  return _model_matrix_def.alloc[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::init()
{
  return _model_matrix_def.init[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
string ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::accessVector() const
{
  return _access[_mode];
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
bool ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::empty()
{
  return _model_matrix_def.alloc[MATRIX::Normal].empty();
}

template <typename GraphBuilder, typename MatrixBuilder, typename Config, typename T>
void ModelMatrix<GraphBuilder, MatrixBuilder, Config, T>::setMode(MATRIX::Mode mode)
{
  _mode = mode;
}

template class ModelMatrix<SDSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<SZSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<SOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<DOSBGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<Deps::DSCGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable>;

template class ModelMatrix<Deps::LHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable>;

template class ModelMatrix<Deps::RHSStGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable>;

template class ModelMatrix<Deps::HDGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<Deps::HZGraphBuilder, Deps::EQModelMatrixBuilder, MATRIX::EQMatrixConfig, IR::EquationTable>;

template class ModelMatrix<Deps::HHGraphBuilder, Deps::EVModelMatrixBuilder, MATRIX::EVMatrixConfig, IR::EventTable>;


}  // namespace IR
}  // namespace MicroModelica
