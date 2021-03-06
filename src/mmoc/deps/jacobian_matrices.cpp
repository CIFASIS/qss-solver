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

#include "jacobian_matrices.h"

#include "../deps/builders/sd_sb_graph_builder.h"
#include "../deps/sb_dependencies.h"
#include "../ir/alg_usage.h"
#include "../ir/helpers.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/visitors/replace_index.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

JacMatrixGenerator::JacMatrixGenerator() : _matrix(), _dv_dx(), _tabs(0) { ModelConfig::instance().setLocalInitSymbols(); }

JacMatrixGenerator::~JacMatrixGenerator() { ModelConfig::instance().unsetLocalInitSymbols(); }

void JacMatrixGenerator::postProcess(SBG::VertexProperty vertex) {}

void JacMatrixGenerator::init(SBG::VertexProperty vertex)
{
  stringstream code;
  FunctionPrinter function_printer;
  code << "for(row = 1; row <= " << vertex.size() << "; row++) {" << endl;
  code << TAB << "c_row = _c_index(row);" << endl;
  code << function_printer.jacMacrosAccess(vertex.eq());
  _matrix.alloc.append(code.str());
  _matrix.init.append(code.str());
  _tabs++;
}

void JacMatrixGenerator::end()
{
  stringstream code;
  _tabs--;
  _matrix.alloc.append("}\n");
  _matrix.init.append("}\n");
  code << "cleanVector(states, 0, " << ModelConfig::instance().stateNbr() << ");" << endl;
  _matrix.init.append(code.str());
}

string JacMatrixGenerator::guard(string exp, string id)
{
  stringstream code;
  string inner_tabs = Utils::instance().tabs(_tabs + 1);
  code << inner_tabs << "x_ind = " << exp << ";" << endl;
  code << inner_tabs << "if(in("
       << "modelData->jac_matrices->" << id << "->index[c_row],"
       << "modelData->jac_matrices->" << id << "->size[c_row], x_ind))"
       << "{" << endl;
  return code.str();
}

void JacMatrixGenerator::addDependency(Equation v_eq, Equation g_eq, SBG::VariableDep var_dep, SBG::Map map, string g_map_dom)
{
  stringstream code;
  Range range(var_dep.range());
  vector<string> exps = map.exps(range.getDimensionVars());
  Expression i_exp = Expression::generate(var_dep.var().name(), exps);
  Index x_ind(i_exp);
  string x_ind_exp = x_ind.print();
  string tabs = Utils::instance().tabs(_tabs);
  string inner_tabs = tabs + TAB;
  stringstream matrix_eq_id;
  if (v_eq.type() == EQUATION::Algebraic) {
    matrix_eq_id << "dg_dx";
  } else {
    matrix_eq_id << "df_dx";
  }
  matrix_eq_id << "[" << v_eq.arrayId() << "]";
  string eq_id = matrix_eq_id.str();
  if ((v_eq.type() == EQUATION::ClassicDerivative || v_eq.type() == EQUATION::QSSDerivative) && var_dep.isRecursive()) {
      code << tabs << g_eq.range().get();
      tabs = Utils::instance().tabs(_tabs+1);
      vector<string> exps = g_eq.range()->getIndexes();
      FunctionPrinter printer;
      Expression a_exp = Expression::generate(g_eq.LHSVariable()->name(), exps);
      Index a_ind(a_exp);
      code << TAB << printer.jacMacrosAccess(g_eq, a_ind.print());
  }
  code << tabs << "if(" << range.in(exps);
  if (!g_map_dom.empty()) {
    code << " && " << g_map_dom;
  }
  code << ") {" << endl;
  string code_guards = code.str();
  string include_guard = guard(x_ind_exp, eq_id);
  _matrix.alloc.append(code_guards);
  _matrix.init.append(code_guards);
  code.str("");
  code << inner_tabs << "modelData->jac_matrices->" << eq_id << "->size[c_row]++;" << endl;
  _matrix.alloc.append(code.str());
  code.str("");
  code << include_guard;
  code << TAB << inner_tabs << "modelData->jac_matrices->" << eq_id << "->size[c_row]--;" << endl;
  code << inner_tabs << "} else {" << endl;
  code << TAB << inner_tabs << "modelData->jac_matrices->" << eq_id << "->index[c_row][states[c_row]++] = x_ind;" << endl;
  code << inner_tabs << "}" << endl;
  _matrix.init.append(code.str());
  code.str("");
  code << tabs << "}" << endl;
  if (var_dep.isRecursive()) {
    code << g_eq.range()->end();
  }
  _matrix.alloc.append(code.str());
  _matrix.init.append(code.str());
}

std::string JacMatrixGenerator::guard(SBG::MDI dom, SBG::Map map)
{
  Range range(dom);
  stringstream code;
  vector<string> exps = map.exps(range.getDimensionVars());
  return range.in(exps);
}

void JacMatrixGenerator::visitF(Equation eq, SBG::VariableDep var_dep, SBG::Map map) { addDependency(eq, eq,var_dep, map); }

void JacMatrixGenerator::visitG(Equation v_eq, Equation g_eq, SBG::VariableDep var_dep, SBG::Map n_map, SBG::Map map_m,
                                int index_shift)
{
  addDependency(v_eq, g_eq, var_dep, n_map, guard(var_dep.dom(), map_m));
}

void JacMatrixGenerator::initG(Equation eq, SBG::Map map_m) {}

JacMatrixDef JacMatrixGenerator::deps() { return _matrix; }

JacobianMatrix::JacobianMatrix() : _jac_matrix_def() {}

string JacobianMatrix::alloc() { return _jac_matrix_def.alloc; }

string JacobianMatrix::init() { return _jac_matrix_def.init; }

string JacobianMatrix::accessVector() { return "states"; }

bool JacobianMatrix::empty() { return _jac_matrix_def.alloc.empty(); }

void JacobianMatrix::build()
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EquationTable derivatives = ModelConfig::instance().derivatives();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  SBG::JacobianMatrixBuilder jac_matrix;
  IndexShiftBuilder index_shifts(algebraics);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics);
  jac_matrix.compute(SDSBGraph.build(), index_shifts.build());
  _jac_matrix_def = jac_matrix.deps();
}

}  // namespace Deps
}  // namespace MicroModelica