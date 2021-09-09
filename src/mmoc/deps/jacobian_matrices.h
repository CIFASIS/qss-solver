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

#include <map>

#include <deps/sbg_graph/deps_graph.h>
#include <ir/equation.h>

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

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  JacMatrixDef deps();

  protected:
  void addDependency(IR::Equation v_eq, IR::Equation g_eq, SB::Deps::VariableDep var_dep, int id, std::string g_map_dom = "");
  std::string guard(SB::Set dom, int offset, SB::Deps::LMapExp map, std::string var_name);
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
