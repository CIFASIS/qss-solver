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

#ifndef DEP_SB_DEPENDENCIES_H_
#define DEP_SB_DEPENDENCIES_H_

#include <string>

#include "../ir/equation.h"
#include "../ir/expression.h"
#include "../ir/jacobian.h"
#include "builders/index_shift_builder.h"
#include "graph/sb_graph_helpers.h"
#include "graph/sb_graph.h"
#include "jacobian_matrices.h"

namespace MicroModelica {
namespace Deps {
namespace SBG {

typedef std::list<std::pair<SBVertex, MDI>> VertexInfo;

template <typename IGenerator, typename R>
class SBDependencies {
  public:
  SBDependencies();
  ~SBDependencies() = default;

  void compute(SBGraph graph, IndexShift index_shift);

  R deps();

  protected:
  void paths(SBGraph graph, SBVertex V);
  IndexShift _index_shift;
  IGenerator _gen;
};

// Typedef for concrete clases.

typedef SBDependencies<Deps::JacMatrixGenerator, Deps::JacMatrixDef> JacobianMatrixBuilder;
typedef SBDependencies<MicroModelica::IR::JacGenerator, MicroModelica::IR::JacDef> JacobianBuilder;

}  // namespace SBG
}  // namespace Deps
}  // namespace MicroModelica

#endif  // DEP_SB_DEPENDENCIES_H
