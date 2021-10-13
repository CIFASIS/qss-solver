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

QSSModelDepsGenerator::QSSModelDepsGenerator() : _qss_model_deps_def(), _tabs(0), _deps(), _post_process_eval(false) {}

void QSSModelDepsGenerator::addCode(DepCode dep_code, std::stringstream& code)
{
  if (dep_code.scalar) {
    code << dep_code.begin[0];
    for (string alg : dep_code.alg_code) {
      code << alg;
    }
    for (string eq : dep_code.code) {
      boost::algorithm::trim_right(eq);
      code << eq << endl;
    }
    code << TAB << TAB << dep_code.end[0] << endl;
  } else {
    assert(dep_code.alg_code.size() == dep_code.code.size());
    int size = dep_code.code.size();
    for (int i = 0; i < size; i++) {
      code << dep_code.begin[i];
      code << dep_code.alg_code[i];
      code << dep_code.code[i];
      code << dep_code.end[i] << endl;
    }
  }
}

Expression QSSModelDepsGenerator::getUseExp(Variable variable, DepData dep_data)
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

Option<Range> QSSModelDepsGenerator::getUseRange(Util::Variable variable, DepData dep_data, Equation eq)
{
  Expression use_exp = dep_data.var_dep.exp();
  const bool SCALAR_EXP = dep_data.var_dep.nMap().constantExp();
  if (dep_data.var_dep.isRecursive()) {
    return Range(dep_data.var_dep.var());
  }
  if (variable.isScalar() || SCALAR_EXP) {
    return Option<Range>();
  }
  if (!SCALAR_EXP) {
    return eq.range();
  }
  assert(false);
  return Option<Range>();
}

void QSSModelDepsGenerator::postProcess(SB::Deps::SetVertex vertex)
{
  if (_post_process_eval) {
    return;
  }
  map<string, DepCode> deps_code;
  EquationTable equations = ModelConfig::instance().derivatives();
  VarSymbolTable variables = ModelConfig::instance().symbols();
  EquationTable::iterator it;
  std::stringstream simple;
  std::stringstream generic;
  FunctionPrinter printer;
  for (auto variable : _deps) {
    PrintedDeps printed_deps;
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
      Option<Range> range = getUseRange(ifr.get(), var_dep, eq.get());
      Index use_idx(use_exp);
      string use_id = use_idx.identifier();
      Equation der = eq.get();
      der.setType(IR::EQUATION::Dependency);
      der.setUsage(use_idx);
      if (range && der.hasRange()) {
        der.setRange(range);
      }
      if ((var_dep.var_dep.equations().size() == 1) && use_exp.isScalar() && der.hasRange()) {
        Equation use_eq = der;
        use_eq.applyUsage(use_idx);
        der = use_eq;
      }
      if (deps_code.find(use_id) == deps_code.end()) {
        DepCode dep_code;
        dep_code.scalar = use_exp.isScalar();
        deps_code[use_id] = dep_code;
      }
      DepCode dep_code = deps_code[use_id];
      dep_code.begin.push_back(printer.beginExpression(use_id, range));
      dep_code.end.push_back(printer.endExpression(range, FUNCTION_PRINTER::Break));
      dep_code.alg_code.push_back(addAlgDeps(der, var_dep.var_dep.nMap(), _der_deps, _alg_deps, printed_deps));
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
  _post_process_eval = true;
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

void QSSModelDepsGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

bool QSSModelDepsGenerator::findDep(DepData dep_data)
{
  string var_name = dep_data.var_dep.var().name();
  list<DepData> deps = _deps[var_name];
  for (DepData dep : deps) {
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
  }
  return false;
}

void QSSModelDepsGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep,
                                   int index_shift)
{
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);
  if (var_dep.isRecursive()) {
    DefAlgDepsUse new_dep(g_eq, var_dep);
    insertAlg(_der_deps, v_eq.id(), new_dep);
  }
  if (v_eq.type() == EQUATION::QSSDerivative) {
    string var_name = var_dep.var().name();
    DepData dep_data;
    dep_data.id = v_eq.id();
    dep_data.var_dep = var_dep;
    if (!findDep(dep_data)) {
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
  DefAlgDepsUse new_dep(g_eq, def_map, use_exp, use_map_exp, def_map_exp,  g_vertex.id());
  insertAlg(((v_eq.type() == IR::EQUATION::Algebraic) ? _alg_deps : _der_deps), v_eq.id(), new_dep);
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
