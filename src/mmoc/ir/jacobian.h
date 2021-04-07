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

#ifndef JACOBIAN_H_
#define JACOBIAN_H_

#include <string>
#include <map>

#include "equation.h"
#include "../deps/graph/sb_graph.h"
#include "../deps/graph/sb_graph_helpers.h"
#include "../util/symbol_table.h"
#include "../util/table.h"

namespace MicroModelica {
namespace IR {

struct JacDef {
  std::string code;
};

class JacGenerator {
  public:
  JacGenerator();
  ~JacGenerator() = default;

  void init(Deps::SBG::VertexProperty vertex);
  void end();
  void postProcess(Deps::SBG::VertexProperty vertex);
  void visitF(Equation eq, Deps::SBG::VariableDep var_dep, Deps::SBG::Map map);
  void visitG(Equation v_eq, Equation g_eq, Deps::SBG::VariableDep var_dep, Deps::SBG::Map n_map, Deps::SBG::Map map_m,
              int index_shift);
  void initG(Equation eq, Deps::SBG::Map map_m);
  JacDef deps();

  protected:
  IR::Expression generateExp(string var_name, vector<string> indices);
  void dependencyPrologue(Equation eq, Deps::SBG::VariableDep var_dep, Deps::SBG::Map map, std::string guard = "");
  void dependencyEpilogue(Equation eq, Deps::SBG::VariableDep var_dep);
  void updateMatrix(std::map<std::string, std::set<std::string>>& matrix);
  void generatePos(int id, EQUATION::Type type, std::string row = "c_row", std::string col = "col");
  void generateEquation(int id, EQUATION::Type type);
  void generateEquation(int v_id, int g_id, EQUATION::Type type);
  std::string getVariableIndexes(Equation eq, Deps::SBG::Map map);
  std::string guard(Deps::SBG::MDI dom, Deps::SBG::Map map);

  JacDef _jac_def;
  int _tabs;
};

class Jacobian {
  public:
  Jacobian();
  ~Jacobian() = default;

  void build();

  std::string code();

  protected:
  JacDef _jac_def;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* JACOBIAN_H_ */
