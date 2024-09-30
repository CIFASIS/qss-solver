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

#include <ir/equation.hpp>
#include <ir/event.hpp>
#include <ir/jacobian.hpp>
#include <ir/model_matrix_gen.hpp>
#include <ir/qss_model.hpp>
#include <ir/qss_model_deps.hpp>
#include <util/symbol_table.hpp>
#include <deps/builders/index_shift_builder.hpp>
#include <deps/builders/merge_graph_builder.hpp>
#include <deps/sbg_graph/deps_graph.hpp>
#include <deps/jacobian_matrices.hpp>

namespace MicroModelica {
namespace Deps {

template <typename IGenerator, typename R, typename S>
class SBDependencies {
  public:
  SBDependencies();
  ~SBDependencies() = default;

  void compute(SB::Deps::Graph graph, SB::Deps::IndexShift index_shift);

  void setup(S config);

  S config();

  R def();

  protected:
  void paths(SB::Deps::Graph& graph, SB::Deps::Vertex V, Util::Variable visiting_alg);
  void recursiveDeps(SB::Deps::Graph& graph, SB::PWLMap map_u, SB::Deps::Vertex V, SB::Deps::Vertex X, int num_gen,
                     list<SB::Deps::SetEdge> rec_alg_use_maps);

  SB::Deps::IndexShift _index_shift;
  IGenerator _gen;
  SB::PWLMap _map_F;
  SB::PWLMap _map_U;
};

// Typedef for concrete clases.

typedef SBDependencies<Deps::JacMatrixGenerator, Deps::JacMatrixDef, IR::EquationTable> JacobianMatrixBuilder;
typedef SBDependencies<IR::JacGenerator, IR::JacDef, IR::EquationTable> JacobianBuilder;
typedef SBDependencies<IR::QSSModelGenerator, IR::QSSModelDef, IR::QSSModelConfig> QSSModelBuilder;
typedef SBDependencies<IR::QSSModelDepsGenerator, IR::QSSModelDepsDef, IR::QSSModelConfig> QSSModelDepsBuilder;
typedef SBDependencies<IR::ModelMatrixGenerator<IR::EquationTable, IR::Equation, IR::MATRIX::EQMatrixConfig>, IR::ModelMatrixDef,
                       IR::MATRIX::EQMatrixConfig>
    EQModelMatrixBuilder;
typedef SBDependencies<IR::ModelMatrixGenerator<IR::EventTable, IR::Event, IR::MATRIX::EVMatrixConfig>, IR::ModelMatrixDef,
                       IR::MATRIX::EVMatrixConfig>
    EVModelMatrixBuilder;

typedef SBDependencies<Deps::MergeGraphGenerator<Deps::EQSelector>, SB::Deps::Graph, Deps::EQSelector> MergeEQGraphBuilder;

typedef SBDependencies<Deps::MergeGraphGenerator<Deps::EVSelector>, SB::Deps::Graph, Deps::EVSelector> MergeEVGraphBuilder;

}  // namespace Deps
}  // namespace MicroModelica
