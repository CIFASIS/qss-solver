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

struct QSSModelDepsDef {
  std::string simple;
  std::string generic;
};

struct DefAlgDepsUse {
  int id;
  SB::PWLMap use;
  SB::Deps::LMapExp use_map;
  SB::Set range;
  Expression exp;
  int offset;
  bool recursive;
};

struct CompDef {
  bool operator() (const DefAlgDepsUse& lhs, const DefAlgDepsUse& rhs) const
  {return lhs.use_map < rhs.use_map;}
};

struct DepData {
  int id;
  SB::Deps::VariableDep var_dep;
};

struct DepCode {
  std::string begin;
  std::string end;
  vector<std::string> code;
  vector<std::string> alg_code;
  bool scalar;
};


class QSSModelDepsGenerator {
  public:
  QSSModelDepsGenerator();
  ~QSSModelDepsGenerator() = default;

  void init(SB::Deps::SetVertex vertex);
  void end();
  void postProcess(SB::Deps::SetVertex vertex);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep);
  void visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift);
  void visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map,
              SB::Deps::LMapExp def_map_exp, SB::Set intersection);
  void initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge);
  QSSModelDepsDef def();

  protected:
  string addAlgDeps(int id, std::map<int, set<DefAlgDepsUse,CompDef>> alg_deps);
  void addCode(DepCode dep_code, std::stringstream& code);
  bool findDep(DepData dep_data);
  Expression getUseExp(Util::Variable variable, DepData dep_data);
  Option<Range> getUseRange(Util::Variable variable, DepData dep_data);

  QSSModelDepsDef _qss_model_deps_def;
  int _tabs;
  std::map<int, set<DefAlgDepsUse,CompDef>> _der_deps;
  std::map<int, set<DefAlgDepsUse,CompDef>>_alg_deps;
  std::map<std::string, list<DepData>> _deps;
};

class QSSModelDeps {
  public:
  QSSModelDeps();
  ~QSSModelDeps() = default;

  void build();

  std::string simpleDef();

  std::string genericDef();

  protected:
  QSSModelDepsDef _qss_model_deps_def;
};

}  // namespace IR
}  // namespace MicroModelica
