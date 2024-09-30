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
#include <map>

#include <ir/equation.h>
#include <deps/sbg_graph/deps_graph.h>
#include <util/symbol_table.h>
#include <util/table.h>

namespace MicroModelica {
namespace IR {

struct JacDef {
  std::string code;
};

class JacGenerator {
  public:
  JacGenerator();
  ~JacGenerator() = default;

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp,
              Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp, SB::Set intersection);
  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  JacDef def();
  void setup(EquationTable algebraics) { _algebraics = algebraics; };
  EquationTable config() { return _algebraics; }

  protected:
  IR::Expression generateExp(string var_name, vector<string> indices);
  void dependencyPrologue(Equation eq, SB::Deps::VariableDep var_dep, std::string guard = "");
  void dependencyEpilogue(Equation eq, SB::Deps::VariableDep var_dep);
  void updateMatrix(std::map<std::string, std::set<std::string>>& matrix);
  void generatePos(int id, EQUATION::Type type, std::string row = "c_row", std::string col = "col");
  void generateEquation(int id, EQUATION::Type type);
  void generateEquation(int v_id, int g_id, EQUATION::Type type);
  std::string getVariableIndexes(Equation eq);
  std::string guard(SB::Set dom, int offset, std::string var_name, SB::Deps::LMapExp map, Equation v_eq);
  void Fvisitor(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, int eq_id);
  JacDef _jac_def;
  int _tabs;
  EquationTable _algebraics;
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
