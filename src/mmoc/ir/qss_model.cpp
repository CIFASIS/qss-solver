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

#include <deps/builders/eq_graph_builder.h>
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

QSSModelGenerator::QSSModelGenerator() : _qss_model_def(), _tabs(0), _post_process_eval(false) {}

void QSSModelGenerator::setup(EquationTable eqs) { _eqs = eqs; }

void QSSModelGenerator::postProcess(SB::Deps::SetVertex vertex)
{
  if (_post_process_eval) {
    return;
  }
  EquationTable equations = _eqs;
  EquationTable::iterator it;
  std::stringstream simple;
  std::stringstream generic;
  for (Equation der = equations.begin(it); !equations.end(it); der = equations.next(it)) {
    PrintedDeps printed_deps;
    string alg_code = addAlgDeps(der, SB::Deps::LMapExp(), _der_deps, _alg_deps, printed_deps);
    der.setAlgCode(alg_code);
    if (der.hasRange()) {
      generic << der << endl;
    } else {
      simple << der << endl;
    }
  }
  _qss_model_def.simple = simple.str();
  _qss_model_def.generic = generic.str();
  _post_process_eval = true;
}

void QSSModelGenerator::init(SB::Deps::SetVertex vertex) {}

void QSSModelGenerator::end() {}

void QSSModelGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep) {}

void QSSModelGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

void QSSModelGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift)
{
  if (var_dep.isRecursive()) {
    Equation v_eq = getEquation(v_vertex, _eqs);
    Equation g_eq = getEquation(g_vertex, _eqs);
    DefAlgDepsUse new_dep(g_eq, var_dep);
    insertAlg(_der_deps, v_eq.id(), new_dep);
  }
}

void QSSModelGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map,
                               SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp,
                               SB::Set intersection)
{
  Equation v_eq = getEquation(v_vertex, _eqs);
  Equation g_eq = getEquation(g_vertex, _eqs);
  DefAlgDepsUse new_dep(g_eq, def_map, use_exp, use_map_exp, def_map_exp, g_vertex.id());
  insertAlg(((v_eq.type() == IR::EQUATION::Algebraic) ? _alg_deps : _der_deps), v_eq.id(), new_dep);
}

void QSSModelGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

QSSModelDef QSSModelGenerator::def() { return _qss_model_def; }

template<typename GraphBuilder>
QSSModel<GraphBuilder>::QSSModel() {}

template<typename GraphBuilder>
void QSSModel<GraphBuilder>::build(EquationTable eqs)
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  QSSModelBuilder qss_model;
  IndexShiftBuilder index_shifts(algebraics);
  GraphBuilder EQSBGraph(eqs, algebraics);
  qss_model.setup(eqs);
  qss_model.compute(EQSBGraph.build(), index_shifts.build());
  _qss_model_def = qss_model.def();
}

template<typename GraphBuilder>
string QSSModel<GraphBuilder>::simpleDef() { return _qss_model_def.simple; }

template<typename GraphBuilder>
string QSSModel<GraphBuilder>::genericDef() { return _qss_model_def.generic; }

template class QSSModel<SDSBGraphBuilder>;

template class QSSModel<SZSBGraphBuilder>;

template class QSSModel<SOSBGraphBuilder>;

}  // namespace IR
}  // namespace MicroModelica
