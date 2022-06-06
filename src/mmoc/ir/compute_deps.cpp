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

#include "compute_deps.h"

#include <ir/helpers.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace Deps;
using namespace SB;
using namespace Util;
namespace IR {

Expression getUseExp(Variable variable, DepData dep_data)
{
  if (variable.isScalar()) {
    return Expression::generate(variable.name(), vector<string>());
  }
  Index use_idx(dep_data.var_dep.exp());
  vector<string> vars = use_idx.variables();
  if (dep_data.var_dep.isRecursive()) {
    return Expression::generate(variable.name(), vars);
  } else {
    vector<string> exps = dep_data.var_dep.nMap().apply(vars);
    return Expression::generate(variable.name(), exps);
  }
  assert(false);
  return Expression::generate(variable.name(), vector<string>());
}


bool findDep(DepsMap deps, DepData dep_data, bool multiple_nodes)
{
  string var_name = dep_data.var_dep.var().name();
  list<DepData> var_deps = deps[var_name];
  for (DepData dep : var_deps) {
    SB::Set dom = dep.var_dep.mapF().wholeDom();
    SB::Set dep_eq_image = dep.var_dep.mapF().image(dom);
    SB::Set new_dom = dep_data.var_dep.mapF().wholeDom();
    SB::Set new_dep_eq_image = dep_data.var_dep.mapF().image(new_dom);
    if ((dep.id == dep_data.id) && dep.var_dep.isRecursive() && dep_data.var_dep.isRecursive() && dep.var_dep.var().name() == var_name) {
      return true;
    }
    if ((dep.id == dep_data.id) && (dep_eq_image == new_dep_eq_image) && (dep.var_dep.nMap() == dep_data.var_dep.nMap())) {
      return true;
    }
    if (multiple_nodes) {
      SB::Set dep_var_image = dep.var_dep.mapU().image(dom);
      SB::Set new_dep_var_image = dep_data.var_dep.mapU().image(new_dom);
      if ((dep.id == dep_data.id) && (dep_var_image == new_dep_var_image) && (dep.var_dep.nMap() == dep_data.var_dep.nMap())) {
        return true;
      }
    }
  }
  return false;
}


bool findAlgDep(PrintedDeps printed_deps, int id, SB::Set range, SB::Deps::LMapExp use_map, SB::Deps::LMapExp eq_use_map)
{
  for (PrintedDep dep : printed_deps) {
    if (dep.id == id && dep.range == range && dep.use_map == use_map) {
      if (eq_use_map.constantExp()) {
        return dep.eq_use_map == eq_use_map;
      } else {
        return true;
      }
    }
  }
  return false;
}

string addAlgDeps(Equation eq, SB::Deps::LMapExp eq_use, AlgDepsMap alg_deps, AlgDepsMap deps, PrintedDeps& printed_deps)
{
  AlgDeps algs = alg_deps[eq.id()];
  stringstream code;
  for (DefAlgDepsUse alg : algs) {
    code << addAlgDeps(alg.eq, alg.use_map, deps, deps, printed_deps);
  }
  for (DefAlgDepsUse alg : algs) {
    Index use(alg.exp);
    vector<string> vars = use.variables();
    SB::Set var_range = alg.use.image(alg.range);
    Range range(var_range, alg.offset, vars);
    EquationTable equations = ModelConfig::instance().algebraics();
    Option<Equation> eq = equations[alg.eq.id()];
    assert(eq);
    Equation gen_eq = eq.get();
    SB::Deps::LMapExp eq_use_exp = eq_use;
    if (!eq_use.constantExp()) {
      eq_use_exp = eq_use.revert();
    }
    SB::Deps::LMapExp use_map = alg.use_map.compose(eq_use_exp);
    if (!use_map.constantExp() && !alg.use_map.constantExp() && !alg.def_map.isEmpty()) {
      use_map = alg.def_map.solve(use_map);
    }
    vector<string> exps = use_map.apply(vars);
    if (findAlgDep(printed_deps, alg.eq.id(), var_range, use_map, eq_use)) {
      continue;
    }
    PrintedDep printed_dep;
    printed_dep.id = alg.eq.id();
    printed_dep.range = var_range;
    printed_dep.use_map = use_map;
    printed_dep.eq_use_map = eq_use;
    printed_deps.push_back(printed_dep);
    Expression use_exp = Expression::generate(alg.exp.reference().get().name(), exps);
    Index use_idx = Index(use_exp);
    if (gen_eq.hasRange() && alg.recursive) {
      code << gen_eq.range()->print(true, true);
      code << "_get" << gen_eq.LHSVariable().get() << "_idxs(" << range.getDimensionVarsString(true) << ");" << endl;
      use_idx = Index(gen_eq.lhs());
    }
    if (eq->hasRange() && !alg.recursive) {
      gen_eq.setRange(range);
    }
    FunctionPrinter printer;
    if (use_idx.isConstant() && gen_eq.hasRange()) {
      if (!gen_eq.range()->checkUsage(use_idx, gen_eq.index())) {
        continue;
      }
      Equation a = gen_eq;
      a.applyUsage(use_idx);
      gen_eq = a;
    }
    code << printer.printAlgebraicGuards(gen_eq, use_idx);
    code << TAB << gen_eq;
    code << printer.endDimGuards(gen_eq.range());
    if (gen_eq.hasRange() && alg.recursive) {
      code << TAB << gen_eq.range()->end() << endl;
    }
  }
  return code.str();
}

void insertAlg(AlgDepsMap& map, int id, DefAlgDepsUse new_dep)
{
  AlgDeps algs = map[id];
  algs.insert(new_dep);
  map[id] = algs;
}

}  // namespace IR
}  // namespace MicroModelica
