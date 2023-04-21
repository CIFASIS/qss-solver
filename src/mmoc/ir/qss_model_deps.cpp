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

#include <deps/builders/eq_graph_builder.h>
#include <deps/sbg_graph/build_from_exps.h>
#include <deps/sb_dependencies.h>
#include <ast/parser/parse.h>
#include <util/model_config.h>
#include <util/util.h>
#include <util/symbol_table.h>
#include <ir/derivative.h>
#include <ir/helpers.h>
#include <ir/index.h>

namespace MicroModelica {
using namespace Deps;
using namespace SB;
using namespace Util;
namespace IR {

QSSModelDepsGenerator::QSSModelDepsGenerator() : _qss_model_deps_def(), _tabs(0), _deps(), _post_process_eval(false) {}

void QSSModelDepsGenerator::setup(QSSModelConfig config) { _config = config; }

QSSModelConfig QSSModelDepsGenerator::config() { return _config; }

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
      bool alg_recursive_use = checkAlgRecursiveDeps(eq.get(), _der_deps, _alg_deps);
      Option<Range> range = getUseRange(ifr.get(), var_dep, eq.get(), false, alg_recursive_use);
      Index use_idx(use_exp);
      string use_id = use_idx.identifier(alg_recursive_use && !ifr.get().isScalar());
      Equation der = eq.get();
      der.setType(IR::EQUATION::Dependency);
      der.setUsage(use_idx);
      if (range && der.hasRange()) {
        der.setRange(range);
      }
      if ((var_dep.var_dep.equations().size() == 1) && use_exp.isScalar() && der.hasRange() && !alg_recursive_use) {
        Equation use_eq = der;
        use_eq.applyUsage(use_idx);
        der = use_eq;
      }
      if (deps_code.find(use_id) == deps_code.end()) {
        DepCode dep_code;
        dep_code.scalar = (use_exp.isScalar() && !alg_recursive_use) || ifr.get().isScalar();
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
    DepData dep_data(eq.id(), var_dep);
    if (!findDep(_deps, dep_data)) {
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    }
  }
}

void QSSModelDepsGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

void QSSModelDepsGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep,
                                   int index_shift)
{
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);
  if (var_dep.isRecursive()) {
    DefAlgDepsUse new_dep(g_eq, var_dep);
    insertAlg(((v_eq.type() == IR::EQUATION::Algebraic) ? _alg_deps : _der_deps), v_eq.id(), new_dep);
  }
  if (v_eq.type() == EQUATION::QSSDerivative && var_dep.var().isState()) {
    string var_name = var_dep.var().name();
    DepData dep_data(v_eq.id(), var_dep);
    if (!findDep(_deps, dep_data)) {
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
  DefAlgDepsUse new_dep(g_eq, def_map, use_exp, use_map_exp, def_map_exp, g_vertex.id());
  insertAlg(((v_eq.type() == IR::EQUATION::Algebraic) ? _alg_deps : _der_deps), v_eq.id(), new_dep);
}

void QSSModelDepsGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

QSSModelDepsDef QSSModelDepsGenerator::def() { return _qss_model_deps_def; }

QSSModelDeps::QSSModelDeps() {}

void QSSModelDeps::build(EquationTable eqs)
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  QSSModelDepsBuilder qss_model_deps;
  IndexShiftBuilder index_shifts(algebraics);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(eqs, algebraics);
  SB::Deps::Graph graph = SDSBGraph.build();
  QSSModelConfig config;
  config.eqs = eqs;
  config.graph = graph;
  qss_model_deps.setup(config);
  qss_model_deps.compute(graph, index_shifts.build());
  _qss_model_deps_def = qss_model_deps.def();
}

string QSSModelDeps::simpleDef() { return _qss_model_deps_def.simple; }

string QSSModelDeps::genericDef() { return _qss_model_deps_def.generic; }

}  // namespace IR
}  // namespace MicroModelica
