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

#include "qss_model_deps.h"

#include <boost/algorithm/string/trim.hpp>
#include <sstream>

#include <deps/builders/sd_sb_graph_builder.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sb_dependencies.h>
#include <parser/parse.h>
#include <util/model_config.h>
#include <util/util.h>
#include <util/symbol_table.h>
#include <ir/derivative.h>
#include <ir/helpers.h>
#include <ir/index.h>

namespace MicroModelica {
using namespace Deps;
using namespace Deps::SBG;
using namespace SB;
using namespace Util;
namespace IR {

QSSModelDepsGenerator::QSSModelDepsGenerator() : _qss_model_deps_def(), _tabs(0), _deps() {}

string QSSModelDepsGenerator::addAlgDeps(int id, std::map<int, set<DefAlgDepsUse,CompDef>> alg_deps)
{
  set<DefAlgDepsUse,CompDef> algs = alg_deps[id];
  stringstream code;
  for (DefAlgDepsUse alg : algs) {
    code << addAlgDeps(alg.id, _alg_deps);
  }
  for (DefAlgDepsUse alg : algs) {
    // Add offset
    SB::Set var_range = alg.use.image(alg.range);
    Range range(var_range, alg.offset);
    EquationTable equations = ModelConfig::instance().algebraics();
    Option<Equation> eq = equations[alg.id];
    assert(eq);
    Equation gen_eq = eq.get();
    Index use_idx = Index(alg.exp);
    if (gen_eq.hasRange() && alg.recursive) {
      code << gen_eq.range()->print(true, true);
      code << "_get" << gen_eq.LHSVariable().get() << "_idxs(" << range.getDimensionVarsString(true) << ");" << endl;
      use_idx = Index(gen_eq.lhs());
    }
    if (eq->hasRange()) {
      gen_eq.setRange(range);
    }
    FunctionPrinter printer;
    if (use_idx.isConstant() && gen_eq.hasRange()) {
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

void QSSModelDepsGenerator::addCode(DepCode dep_code, std::stringstream& code)
{
  if (dep_code.scalar) {
    code << dep_code.begin;
    for (string alg : dep_code.alg_code) {
      code << alg;
    }
    for (string eq : dep_code.code) {
      boost::algorithm::trim_right(eq);
      code << eq << endl;
    }
    code << TAB << TAB << dep_code.end << endl;
  } else {
    assert(dep_code.alg_code.size() == dep_code.code.size());
    int size = dep_code.code.size();
    for (int i = 0; i < size; i++) {
      code << dep_code.begin;
      code << dep_code.alg_code[i];
      code << dep_code.code[i];
      code << dep_code.end << endl;
    }
  }
}

Expression QSSModelDepsGenerator::getUseExp(Variable variable, DepData dep_data)
{
  if (variable.isScalar()) {
    return Expression::generate(variable.name(), vector<string>());
  }
  Expression use_exp = dep_data.var_dep.exp();
  if (dep_data.var_dep.isReduction()) {
    Range range(dep_data.var_dep.variables(), dep_data.var_dep.varOffset());
    vector<string> exps = range.getIndexes();
    return Expression::generate(variable.name(), exps);
  } else {
    Range range(dep_data.var_dep.variables(), dep_data.var_dep.varOffset());
    vector<string> exps = dep_data.var_dep.nMap().apply(range.getIndexes());
    return Expression::generate(variable.name(), exps);
  }
  assert(false);
  return Expression::generate(variable.name(), vector<string>());
}

Option<Range> QSSModelDepsGenerator::getUseRange(Util::Variable variable, DepData dep_data)
{
  Expression use_exp = dep_data.var_dep.exp();
  cout << use_exp << endl;
  if (variable.isScalar() || dep_data.var_dep.nMap().constantExp()) {
    return Option<Range>();
  }
  if (!use_exp.isScalar()) {
    return Range(dep_data.var_dep.variables(), dep_data.var_dep.varOffset());
  }  
  if (use_exp.isScalar() && dep_data.var_dep.isReduction()) {
    return Range(dep_data.var_dep.var());
  } 
  assert(false);
  return Option<Range>();
}

void QSSModelDepsGenerator::postProcess(SB::Deps::SetVertex vertex)
{
  map<string, DepCode> deps_code;
  EquationTable equations = ModelConfig::instance().derivatives();
  VarSymbolTable variables = ModelConfig::instance().symbols();
  EquationTable::iterator it;
  std::stringstream simple;
  std::stringstream generic;
  FunctionPrinter printer;
  for (auto variable : _deps) {
    list<DepData> var_deps = variable.second;
    Option<Variable> ifr = variables[variable.first];
    assert(ifr);
    for (DepData var_dep : var_deps) {
      std::stringstream begin;
      std::stringstream end;
      Option<Equation> eq = equations[var_dep.id];
      assert(eq);
      vector<string> exps;
      Expression use_exp = getUseExp(ifr.get(), var_dep);
      Index use_idx(use_exp);
      string use_id = use_idx.identifier();
      Equation der = eq.get();
      der.setType(IR::EQUATION::Dependency);
      der.setUsage(use_exp);
      if (deps_code.find(use_id) == deps_code.end()) {
        DepCode dep_code;
        Option<Range> range = getUseRange(ifr.get(), var_dep);
        dep_code.begin = printer.beginExpression(use_id, range);
        dep_code.end = printer.endExpression(range, FUNCTION_PRINTER::Break);
        dep_code.scalar = use_exp.isScalar();
        deps_code[use_id] = dep_code;
      }
      DepCode dep_code = deps_code[use_id];
      dep_code.alg_code.push_back(addAlgDeps(der.id(), _der_deps));  
      dep_code.code.push_back(der.print());
      deps_code[use_id] = dep_code;
    }
  }
  for (auto dc : deps_code) {
    DepCode dep_code = dc.second;
    addCode(dep_code, ((dep_code.scalar) ? simple : generic));
  }
  _qss_model_deps_def.simple = simple.str();
  _qss_model_deps_def.generic = generic.str();
}

void QSSModelDepsGenerator::init(SB::Deps::SetVertex vertex) {}

void QSSModelDepsGenerator::end() {}

void QSSModelDepsGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  Equation eq = getEquation(vertex);
  if (eq.type() == EQUATION::QSSDerivative) {
    string var_name = var_dep.var().name();
    DepData dep_data;
    dep_data.id = eq.id();
    dep_data.var_dep = var_dep;
    if (!findDep(dep_data)) {
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    }
  }
}

void QSSModelDepsGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex)
{
}

bool QSSModelDepsGenerator::findDep(DepData dep_data)
{
  string var_name = dep_data.var_dep.var().name();
  list<DepData> deps = _deps[var_name];
  for (DepData dep : deps) {
    if ((dep.id == dep_data.id) && (dep.var_dep.variables() == dep_data.var_dep.variables())) {
      return true;
    }
  }
  return false;
}

void QSSModelDepsGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep,
                                   int index_shift)
{
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);
  if (v_eq.type() == EQUATION::QSSDerivative) {
    if (var_dep.isRecursive()) {
      DefAlgDepsUse new_dep;
      new_dep.id = g_eq.id();
      new_dep.use = var_dep.mapF();
      new_dep.range = var_dep.mapF().wholeDom();
      new_dep.exp = var_dep.exp();
      new_dep.offset = g_vertex.id();
      new_dep.recursive = true;
      new_dep.use_map = var_dep.nMap();
      set<DefAlgDepsUse,CompDef> algs = _der_deps[v_eq.id()];
      algs.insert(new_dep);
      _der_deps[v_eq.id()] = algs;
    } else {
      string var_name = var_dep.var().name();
      DepData dep_data;
      dep_data.id = v_eq.id();
      dep_data.var_dep = var_dep;
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    } 
  } 
}

void QSSModelDepsGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map,
                                   SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp,
                                   SB::Set intersection)
{
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);
  DefAlgDepsUse new_dep;
  new_dep.id = g_eq.id();
  new_dep.use = def_map;
  new_dep.range = intersection;
  new_dep.exp = use_exp;
  new_dep.offset = g_vertex.id();
  new_dep.recursive = false;
  new_dep.use_map = use_map_exp;
  if (v_eq.type() == IR::EQUATION::Algebraic) {
    set<DefAlgDepsUse,CompDef> algs = _alg_deps[v_eq.id()];
    algs.insert(new_dep);
    _alg_deps[v_eq.id()] = algs;
  } else {
    set<DefAlgDepsUse,CompDef> algs = _der_deps[v_eq.id()];
    algs.insert(new_dep);
    _der_deps[v_eq.id()] = algs;
  }
}

void QSSModelDepsGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

QSSModelDepsDef QSSModelDepsGenerator::def() { return _qss_model_deps_def; }

QSSModelDeps::QSSModelDeps() {}

void QSSModelDeps::build()
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EquationTable derivatives = ModelConfig::instance().derivatives();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  QSSModelDepsBuilder qss_model_deps;
  IndexShiftBuilder index_shifts(algebraics);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics);
  qss_model_deps.compute(SDSBGraph.build(), index_shifts.build());
  _qss_model_deps_def = qss_model_deps.def();
}

string QSSModelDeps::simpleDef() { return _qss_model_deps_def.simple; }

string QSSModelDeps::genericDef() { return _qss_model_deps_def.generic; }

}  // namespace IR
}  // namespace MicroModelica
