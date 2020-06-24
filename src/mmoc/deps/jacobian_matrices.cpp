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

JacMatrixGenerator::JacMatrixGenerator() : _matrix(), _df_dx(), _dg_dx() {}

void JacMatrixGenerator::postProcess(SBG::VertexProperty vertex) {}

void JacMatrixGenerator::init(SBG::VertexProperty vertex)
{
  stringstream code;
  code << "for(row = 1; row <= " << vertex.size() << "; row++) {" << endl;
  _matrix.alloc.append(code.str());
  _matrix.init.append(code.str());
}

void JacMatrixGenerator::end()
{
  _matrix.alloc.append("}\n");
  _matrix.init.append("}\n");
}

void JacMatrixGenerator::addDependency(Equation eq, SBG::VariableDep var_dep, SBG::Map map, std::map<int, std::string>& der_matrix)
{
  stringstream code;
  Range range(var_dep.mdi());
  vector<string> variables;
  variables.push_back("row");
  vector<string> exps = map.exps(variables);
  Expression i_exp = Expression::generate(var_dep.var().name(), exps);
  Index x_ind(i_exp);
  string x_ind_exp = x_ind.print();
  if (der_matrix.find(eq.id()) == der_matrix.end()) {
    code << range.in(exps);
    code << "x_ind = " << x_ind_exp << endl;
  }
  string code_guards = code.str();
  if (!code_guards.empty()) {
    _matrix.alloc.append(code_guards);
    _matrix.init.append(code_guards);
    code.str("");
    code << "dvdx->dfdx[" << eq.id() << "].size[row]++;" << endl;
    _matrix.alloc.append(code.str());
    code.str("");
    code << "dvdx->dfdx[" << eq.id() << "].index[row][col] = x_ind;" << endl;
    _matrix.init.append(code.str());
  }
  cout << _matrix.init << endl;
  cout << _matrix.alloc << endl;
}

void JacMatrixGenerator::visitF(Equation eq, SBG::VariableDep var_dep, SBG::Map map) { addDependency(eq, var_dep, map, _df_dx); }

void JacMatrixGenerator::visitG(Equation v_eq, Equation g_eq, SBG::VariableDep var_dep, SBG::Map n_map, SBG::Map map_m,
                                SBG::Offset index_shift)
{
  addDependency(g_eq, var_dep, n_map, _df_dx);
}

void JacMatrixGenerator::initG(Equation eq, SBG::Map map_m) {}

JacMatrixDef JacMatrixGenerator::deps() { return _matrix; }

JacobianMatrix::JacobianMatrix() : _jac_matrix_def() {}

string JacobianMatrix::alloc() { return _jac_matrix_def.alloc; }

string JacobianMatrix::init() { return _jac_matrix_def.init; }

string JacobianMatrix::accessVector() { return ""; }

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