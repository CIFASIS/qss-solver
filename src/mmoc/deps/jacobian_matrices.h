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

#ifndef JACOBIAN_MATRICES_H_
#define JACOBIAN_MATRICES_H_

#include <map>

#include "../deps/graph/sb_graph.h"
#include "../ir/equation.h"

namespace MicroModelica {
namespace Deps {

struct JacMatrixDef {
  string init;
  string alloc;
};

class JacMatrixGenerator {
  public:
  JacMatrixGenerator();
  ~JacMatrixGenerator();

  void init(SBG::VertexProperty vertex);
  void end();
  void postProcess(SBG::VertexProperty vertex);
  void visitF(IR::Equation eq, SBG::VariableDep var_dep, SBG::Map map);
  void visitG(IR::Equation v_eq, IR::Equation g_eq, SBG::VariableDep var_dep, SBG::Map n_map, SBG::Map map_m, SBG::Offset index_shift);
  void initG(IR::Equation eq, SBG::Map map);
  JacMatrixDef deps();

  protected:
  void addDependency(IR::Equation eq, SBG::VariableDep var_dep, SBG::Map map, std::string g_map_dom = "");
  std::string guard(SBG::MDI dom, SBG::Map map);
  std::string guard(std::string exp, std::string id);

  JacMatrixDef _matrix;
  std::map<std::string, std::string> _dv_dx;
  int _tabs;
};

class JacobianMatrix {
  public:
  JacobianMatrix();
  ~JacobianMatrix() = default;

  void build();

  std::string alloc();
  std::string init();
  std::string accessVector();
  bool empty();

  protected:
  JacMatrixDef _jac_matrix_def;
};

}  // namespace Deps
}  // namespace MicroModelica

#endif /* JACOBIAN_MATRICES_H_ */