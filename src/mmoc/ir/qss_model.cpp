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

#include "qss_model.h"

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

QSSModelGenerator::QSSModelGenerator() : _qss_model_def(), _tabs(0) {}

string QSSModelGenerator::addAlgDeps(int id, std::map<int, list<DefAlgUse>> alg_deps)
{
  list<DefAlgUse> algs = alg_deps[id];
  stringstream code;
  for (DefAlgUse alg : algs) {
    code << addAlgDeps(alg.id, _alg_deps);
  }
  for (DefAlgUse alg : algs) {
    // Add offset
    Range range(alg.range, 0);
    EquationTable equations = ModelConfig::instance().algebraics();
    Option<Equation> eq = equations[alg.id];
    assert(eq);
    Equation gen_eq = eq.get();
    gen_eq.setRange(range);
    code << gen_eq;
  }
  return code.str();
}


void QSSModelGenerator::postProcess(SB::Deps::SetVertex vertex)
{
  EquationTable equations = ModelConfig::instance().derivatives();
  EquationTable::iterator it;
  std::stringstream simple;
  std::stringstream generic;
  for (Equation der = equations.begin(it); !equations.end(it); der = equations.next(it)) {
    string alg_code  = addAlgDeps(der.id(), _der_deps);
    der.setAlgCode(alg_code);
    if (der.hasRange()) {
      generic << der << endl;
    } else {
      simple << der << endl;
    }
  }
  _qss_model_def.simple = simple.str();
  _qss_model_def.generic = generic.str();
}

void QSSModelGenerator::init(SB::Deps::SetVertex vertex) {}

void QSSModelGenerator::end() {}

void QSSModelGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep) {}

void QSSModelGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

void QSSModelGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift)
{
}

void QSSModelGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp, SB::PWLMap def_map,
              SB::Deps::LMapExp def_map_exp, SB::Set intersection)
{
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);  
  DefAlgUse new_dep;
  new_dep.id = g_eq.id();
  new_dep.use = def_map_exp;
  new_dep.range = intersection;
  if (v_eq.type() == IR::EQUATION::Algebraic) {
    list<DefAlgUse> algs = _alg_deps[v_eq.id()];
    algs.push_back(new_dep);
    _alg_deps[v_eq.id()] = algs;
  } else {
    list<DefAlgUse> algs = _der_deps[v_eq.id()];
    algs.push_back(new_dep);
    _der_deps[v_eq.id()] = algs;
  }
}

void QSSModelGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

QSSModelDef QSSModelGenerator::def() { return _qss_model_def; }

QSSModel::QSSModel() {}

void QSSModel::build()
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EquationTable derivatives = ModelConfig::instance().derivatives();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  QSSModelBuilder qss_model;
  IndexShiftBuilder index_shifts(algebraics);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics);
  qss_model.compute(SDSBGraph.build(), index_shifts.build());
  _qss_model_def = qss_model.def();
}

string QSSModel::simpleDef() { return _qss_model_def.simple; }

string QSSModel::genericDef() { return _qss_model_def.generic; }


}  // namespace IR
}  // namespace MicroModelica
