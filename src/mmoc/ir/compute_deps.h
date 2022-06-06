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

#include <ir/equation.h>
#include <deps/sbg_graph/deps_graph.h>

namespace MicroModelica {
namespace IR {

struct DefAlgDepsUse {
  DefAlgDepsUse(Equation e, SB::Deps::VariableDep var_dep) {
    eq = e;
    use = var_dep.mapF();
    range = var_dep.mapF().wholeDom();
    exp = var_dep.exp();
    offset = var_dep.eqOffset();
    recursive = var_dep.isRecursive();
    use_map = var_dep.nMap();
    def_map = SB::Deps::LMapExp();
  }
  DefAlgDepsUse(Equation e, SB::PWLMap pwl_def_map, Expression use_exp, SB::Deps::LMapExp use_map_exp, SB::Deps::LMapExp def_map_exp, int off) {
    eq = e;
    use = pwl_def_map;
    range = pwl_def_map.wholeDom();
    exp = use_exp;
    recursive = false;
    use_map = use_map_exp;
    def_map = def_map_exp;
    offset = off;
  }
  DefAlgDepsUse(const DefAlgDepsUse& other)
  {
    eq = other.eq;
    use = other.use;
    range = other.range;
    exp = other.exp;
    recursive = other.recursive;
    use_map = other.use_map;
    def_map = other.def_map;
    offset = other.offset;
  }
  Equation eq;
  SB::PWLMap use;
  SB::Deps::LMapExp use_map;
  SB::Deps::LMapExp def_map;
  SB::Set range;
  Expression exp;
  int offset;
  bool recursive;
};

struct CompDef {
  bool operator() (const DefAlgDepsUse& lhs, const DefAlgDepsUse& rhs) const
  { 
    if (lhs.eq.id() != rhs.eq.id()) {
      return lhs.eq.id() < rhs.eq.id();
    } else {
      return lhs.use_map < rhs.use_map; 
    }
  }
};

typedef set<DefAlgDepsUse,CompDef> AlgDeps;

typedef std::map<int, AlgDeps> AlgDepsMap;

struct PrintedDep {
  SB::Set range;
  int id;
  SB::Deps::LMapExp use_map;
  SB::Deps::LMapExp eq_use_map;
};

typedef list<PrintedDep> PrintedDeps;

struct DepData {
  DepData(int id_exp, SB::Deps::VariableDep var_dep_exp) : id(id_exp), var_dep(var_dep_exp), from_alg(false) {}
  DepData(int id_exp, SB::Deps::VariableDep var_dep_exp, bool from_alg_exp) : id(id_exp), var_dep(var_dep_exp), from_alg(from_alg_exp) {}
  int id;
  SB::Deps::VariableDep var_dep;
  bool from_alg;
};

typedef list<DepData> DepsData;

typedef std::map<std::string, DepsData> DepsMap;

string addAlgDeps(Equation eq, SB::Deps::LMapExp eq_use, std::map<int, AlgDeps> der_deps, std::map<int, AlgDeps> alg_deps, PrintedDeps& printed_deps);

void insertAlg(AlgDepsMap& map, int id, DefAlgDepsUse new_dep);

Expression getUseExp(Util::Variable variable, DepData dep_data);

template<typename N>
Option<Range> getUseRange(Util::Variable variable, DepData dep_data, N node)
{
  Expression use_exp = dep_data.var_dep.exp();
  const bool SCALAR_EXP = dep_data.var_dep.nMap().constantExp();
  if (dep_data.var_dep.isRecursive()) {
    return Range(dep_data.var_dep.var());
  }
  Index use_idx(use_exp);
  std::vector<std::string> var_names = (use_idx.isConstant() && node.range()) ? node.range()->getIndexes() : use_idx.variables();
  if (dep_data.var_dep.equations().size() == 1 && dep_data.var_dep.variables().size() > 1) {
      return Range(dep_data.var_dep.variables(),dep_data.var_dep.varOffset(), var_names);
  } else if (dep_data.var_dep.equations().size() > 1 && dep_data.var_dep.variables().size() == 1) {
      return Range(dep_data.var_dep.equations(),dep_data.var_dep.eqOffset(), var_names);
  }
  if (variable.isScalar() || SCALAR_EXP) {
    return Option<Range>(); 
  }
  if (!SCALAR_EXP) {
    if (dep_data.from_alg) {
      return Range(dep_data.var_dep.equations(),dep_data.var_dep.eqOffset(), use_idx.variables());
    }
    return node.range();
  }
  assert(false);
  return Option<Range>();
}

Option<Range> getUseRange(Util::Variable variable, DepData dep_data);

bool findDep(DepsMap deps, DepData dep_data, bool multiple_nodes = false);

}  // namespace IR
}  // namespace MicroModelica