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

#include <ir/compute_deps.h>
#include <ir/statement.h>
#include <ir/node_selector.h>
#include <deps/sbg_graph/deps_graph.h>
#include <util/symbol_table.h>

namespace MicroModelica {
namespace IR {

namespace MATRIX {
typedef enum { Normal, Transpose } Mode;

typedef enum { Alloc = 0, Init = 2 } Method;

typedef std::map<int ,AST_ExpressionList> UserDefMatrixExps;

template <typename S>
class MatrixConfig {
  public:
  MatrixConfig<S>(std::string cont, std::vector<std::string> n, std::vector<std::string> ac, std::vector<std::string> comp, S sel)
      : container(cont), names(n), access(ac), component(comp), search(IR::STATEMENT::LHS), selector(sel), user_def(), generate_user_def_matrix(false) {};
  MatrixConfig<S>(std::string cont, std::vector<std::string> n, std::vector<std::string> ac, std::vector<std::string> comp,
                  IR::STATEMENT::AssignTerm s, UserDefMatrixExps ud, bool gen_user_def_matrix, S sel)
      : container(cont), names(n), access(ac), component(comp), search(s), selector(sel), user_def(ud), generate_user_def_matrix(gen_user_def_matrix){};
  MatrixConfig<S>(std::string cont, std::vector<std::string> n, std::vector<std::string> ac, std::vector<std::string> comp,
                  IR::STATEMENT::AssignTerm s, S sel)
      : container(cont), names(n), access(ac), component(comp), search(s), selector(sel), user_def(), generate_user_def_matrix(false){};
  MatrixConfig<S>(){};
  std::string container;
  std::vector<std::string> names;
  std::vector<std::string> access;
  std::vector<std::string> component;
  IR::STATEMENT::AssignTerm search;
  S selector;
  UserDefMatrixExps user_def;
  bool generate_user_def_matrix; 
};

typedef MatrixConfig<Deps::EQSelector> EQMatrixConfig;

typedef MatrixConfig<Deps::EVSelector> EVMatrixConfig;

}  // namespace MATRIX

struct ModelMatrixDef {
  std::string alloc[2];
  std::string init[2];
};

struct MatrixCode {
  vector<std::string> begin;
  vector<std::string> end;
  vector<std::string> code;
};

template <typename NT, typename N, typename Config>
class ModelMatrixGenerator {
  public:
  ModelMatrixGenerator();
  ~ModelMatrixGenerator() = default;

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp,
              Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp, SB::Set intersection);
  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  ModelMatrixDef def();

  void setup(Config config);
  Config config();

  protected:
  void addCode(MatrixCode dep_code, std::stringstream& code);
  std::string component(MATRIX::Method method) const;
  void printMatrix(MATRIX::Method method, MATRIX::Mode mode);

  ModelMatrixDef _model_matrix_def;
  int _tabs;
  DepsMap _deps;
  bool _post_process_eval;
  Config _config;
};

}  // namespace IR
}  // namespace MicroModelica
