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

struct QSSModelConfig {
  EquationTable eqs;
  SB::Deps::Graph graph;
};

struct DefAlgDepsUse {
  DefAlgDepsUse(Equation e, SB::Deps::VariableDep var_dep)
  {
    eq = e;
    use = var_dep.mapF();
    range = var_dep.mapF().wholeDom();
    exp = var_dep.exp();
    offset = var_dep.eqOffset();
    recursive = var_dep.isRecursive();
    use_map = var_dep.nMap();
    def_map = SB::Deps::LMapExp();
  }
  DefAlgDepsUse(Equation e, SB::PWLMap pwl_def_map, Expression use_exp, SB::Deps::LMapExp use_map_exp, SB::Deps::LMapExp def_map_exp,
                int off, bool rec = false)
  {
    eq = e;
    use = pwl_def_map;
    range = pwl_def_map.wholeDom();
    exp = use_exp;
    recursive = rec;
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
  bool operator()(const DefAlgDepsUse& lhs, const DefAlgDepsUse& rhs) const
  {
    // if (lhs.eq.id() != rhs.eq.id()) {
    if (lhs.use_map.constantExp() && rhs.use_map.constantExp()) {
      return lhs.eq.id() < rhs.eq.id();
    }
    if (lhs.use_map.constantExp() && !rhs.use_map.constantExp()) {
      return lhs.use_map.constants() < rhs.use_map.initValues();
    }
    if (!lhs.use_map.constantExp() && rhs.use_map.constantExp()) {
      return lhs.use_map.initValues() < rhs.use_map.constants();
    }
    if (!lhs.use_map.constantExp() && !rhs.use_map.constantExp()) {
      return lhs.use_map.initValues() < rhs.use_map.initValues();
    }
    assert(false);
    return false;
    //} else {
    //  return lhs.use_map < rhs.use_map;
    //}
  }
};

typedef set<DefAlgDepsUse, CompDef> AlgDeps;

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

string addAlgDeps(Equation eq, SB::Deps::LMapExp eq_use, std::map<int, AlgDeps> der_deps, std::map<int, AlgDeps> alg_deps,
                  PrintedDeps& printed_deps, bool comes_from_rec = false);

void insertAlg(AlgDepsMap& map, int id, DefAlgDepsUse new_dep);

Expression getUseExp(Util::Variable variable, DepData dep_data);

bool checkEventRange(Index index, Range range);

std::vector<std::string> getVariables(Index index, Range range);

template <typename N>
Option<Range> getUseRange(Util::Variable variable, DepData dep_data, N node, bool event = false, bool path_recursive_deps = false)
{
  Expression use_exp = dep_data.var_dep.exp();
  const bool SCALAR_EXP = dep_data.var_dep.nMap().constantExp();
  if (variable.isScalar()) {
    return Option<Range>();
  }
  /// If the variable is recursive, return the entire variable range.
  if (variable.isArray() && path_recursive_deps) {
    return Range(variable);
  }
  if (dep_data.var_dep.isRecursive()) {
    return Range(dep_data.var_dep.var());
  }
  if (SCALAR_EXP) {
    return Option<Range>();
  }
  Index use_idx(use_exp);
  std::vector<std::string> var_names = (use_idx.isConstant() && node.range()) ? node.range()->getIndexes() : use_idx.variables();
  /// If a range of variables affects one equation, generate a range with variable size and offsets,
  /// the same is done if the opposite condition is met, return the range of the equations.
  if (dep_data.var_dep.equations().size() == 1 && dep_data.var_dep.variables().size() > 1) {
    return Range(dep_data.var_dep.variables(), dep_data.var_dep.varOffset(), var_names);
  } else if (dep_data.var_dep.equations().size() > 1 && dep_data.var_dep.variables().size() == 1) {
    return Range(dep_data.var_dep.equations(), dep_data.var_dep.eqOffset(), var_names);
  }
  if (!SCALAR_EXP) {
    if (dep_data.from_alg) {
      return Range(dep_data.var_dep.equations(), dep_data.var_dep.eqOffset(), use_idx.variables(), node.range());
    }
    if (event) {
      if (!node.range()) {
        return Range(dep_data.var_dep.variables(), dep_data.var_dep.varOffset(), var_names);
      } else if (!checkEventRange(use_idx, node.range().get())) {
        var_names = getVariables(use_idx, node.range().get());
        return Range(dep_data.var_dep.equations(), dep_data.var_dep.eqOffset(), var_names);
      }
    }
    return node.range();
  }
  assert(false);
  return Option<Range>();
}

Option<Range> getUseRange(Util::Variable variable, DepData dep_data);

bool findDep(DepsMap deps, DepData dep_data, bool multiple_nodes = false);

bool checkAlgRecursiveDeps(Equation eq, AlgDepsMap alg_deps, AlgDepsMap deps);

}  // namespace IR
}  // namespace MicroModelica
