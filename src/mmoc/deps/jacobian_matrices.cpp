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
#include "../util/visitors/replace_index.h"
#include "../util/util.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

JacMatrixGenerator::JacMatrixGenerator() : _matrix(), _dv_dx(), _tabs(0) {}

void JacMatrixGenerator::postProcess(SBG::VertexProperty vertex) {}

void JacMatrixGenerator::init(SBG::VertexProperty vertex)
{
  stringstream code;
  code << "for(row = 1; row <= " << vertex.size() << "; row++) {" << endl;
  code << TAB << "c_row = _c_index(row);" << endl;
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

void JacMatrixGenerator::addDependency(Equation eq, SBG::VariableDep var_dep, SBG::Map map)
{
  stringstream code;
  Range range(var_dep.mdi());
  vector<string> variables;
  variables.push_back("row");
  vector<string> exps = map.exps(variables);
  Expression i_exp = Expression::generate(var_dep.var().name(), exps);
  Index x_ind(i_exp);
  string x_ind_exp = x_ind.print();
  string tabs = Utils::instance().tabs(_tabs);
  string inner_tabs = tabs + TAB;
  stringstream matrix_eq_id;
  if (eq.type() == EQUATION::Algebraic) {
    matrix_eq_id << "dg_dx";
  } else {
    matrix_eq_id << "df_dx";
  }
  matrix_eq_id << "[" << eq.arrayId() << "]";
  string eq_id = matrix_eq_id.str();

  // if (_dv_dx.find(eq_id) == _dv_dx.end()) {
  code << tabs << range.in(exps);
  //  _dv_dx[eq_id] = code.str();
  //}
  string code_guards = code.str();
  // if (!code_guards.empty()) {
  _matrix.alloc.append(code_guards);
  _matrix.init.append(code_guards);
  code.str("");
  code << inner_tabs << "modelData->jac_matrices->" << eq_id << "->size[c_row]++;" << endl;
  _matrix.alloc.append(code.str());
  code.str("");
  code << inner_tabs << "x_ind = " << x_ind_exp << ";" << endl;
  code << inner_tabs << "modelData->jac_matrices->" << eq_id << "->index[c_row][states[c_row]++] = x_ind;" << endl;
  _matrix.init.append(code.str());
  code.str("");
  code << tabs << range.end() << endl;
  _matrix.alloc.append(code.str());
  _matrix.init.append(code.str());
  //}
  cout << _matrix.init << endl;
  cout << _matrix.alloc << endl;
}

void JacMatrixGenerator::visitF(Equation eq, SBG::VariableDep var_dep, SBG::Map map) { addDependency(eq, var_dep, map); }

void JacMatrixGenerator::visitG(Equation v_eq, Equation g_eq, SBG::VariableDep var_dep, SBG::Map n_map, SBG::Map map_m,
                                SBG::Offset index_shift)
{
  addDependency(v_eq, var_dep, n_map);
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
  VarSymbolTable symbols = Utils::instance().symbols();
  SBG::JacobianMatrixBuilder jac_matrix;
  IndexShiftBuilder index_shifts(algebraics, symbols);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics, symbols);
  jac_matrix.compute(SDSBGraph.build(), index_shifts.build());
  _jac_matrix_def = jac_matrix.deps();
}

}  // namespace Deps
}  // namespace MicroModelica