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

#include "jacobian.h"

#include <sstream>

#include "../deps/builders/sd_sb_graph_builder.h"
#include "../deps/sb_dependencies.h"
#include "../parser/parse.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/symbol_table.h"
#include "derivative.h"
#include "helpers.h"
#include "index.h"

namespace MicroModelica {
using namespace Deps;
using namespace Deps::SBG;
using namespace Util;
namespace IR {

JacGenerator::JacGenerator() : _jac_def(), _tabs(0) {}

void JacGenerator::postProcess(SBG::VertexProperty vertex)
{
  int size = vertex.size();
  int id = vertex.id() - 1;
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  code << endl;
  code << "// Assign Jacobian Matrix values" << endl;
  code << endl;
  code << tab << "for (row = 0; row < " << size << "; row++) {" << endl;
  code << tab << "  for (col = 0; col < dvdx->df_dx[" << id << "]->size[row]; col++) {" << endl;
  code << tab << "    row_t = dvdx->df_dx[" << id << "]->index[row][col];" << endl;
  code << tab << "    _assign_jac(row_t, dvdx->df_dx[" << id << "]->value[row][col]);" << endl;
  code << tab << "  }" << endl;
  code << tab << "}" << endl;
  _jac_def.code.append(code.str());
}

void JacGenerator::init(SBG::VertexProperty vertex)
{
  stringstream code;
  FunctionPrinter function_printer;
  code << "for(row = 1; row <= " << vertex.size() << "; row++) {" << endl;
  code << TAB << "c_row = _c_index(row);" << endl;
  code << function_printer.jacMacrosAccess(vertex.eq());
  _tabs++;
  _jac_def.code.append(code.str());
}

void JacGenerator::end()
{
  _jac_def.code.append("}\n");
  _tabs--;
}

std::string JacGenerator::guard(SBG::MDI dom, SBG::Map map)
{
  Range range(dom);
  stringstream code;
  vector<string> exps = map.exps(range.getDimensionVars());
  return range.in(exps);
}

void JacGenerator::dependencyPrologue(Equation eq, SBG::VariableDep var_dep, SBG::Map map, std::string guard)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  Range range(var_dep.range());
  vector<string> exps = map.exps(range.getDimensionVars());
  code << tab << "if(" << range.in(exps);
  if (!guard.empty()) {
    code << tab << "&& " << guard;
  }
  code << ") {" << endl;
  Expression i_exp = Expression::generate(var_dep.var().name(), exps);
  Index x_ind(i_exp);
  code << tab << tab << "x_ind = " << x_ind << ";" << endl;
  _jac_def.code.append(code.str());
  _tabs++;
}

void JacGenerator::dependencyEpilogue()
{
  _tabs--;
  string end = Utils::instance().tabs(_tabs) + "}\n";
  _jac_def.code.append(end);
}

void JacGenerator::generatePos(int id, EQUATION::Type type, string row, string col)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  string type_str = "df_dx";
  code << tab << col << " = pos(dvdx->";
  if (type == EQUATION::Algebraic) {
    type_str = "dg_dx";
  }
  code << type_str << "[" << id << "]->index[" << row << "], ";
  code << "dvdx->" << type_str << "[" << id << "]->size[" << row << "], x_ind);" << endl;
  _jac_def.code.append(code.str());
}

void JacGenerator::generateEquation(int id, EQUATION::Type type)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  string mat = (type == EQUATION::Algebraic) ? "dg_dx" : "df_dx";
  code << tab << "dvdx->" << mat << "[" << id << "]->value[c_row][col] +=  aux;" << endl;
  _jac_def.code.append(code.str());
}

void JacGenerator::generateEquation(int v_id, int g_id, EQUATION::Type type)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  string mat = (type == EQUATION::Algebraic) ? "dg_dx" : "df_dx";
  code << tab << "dvdx->" << mat << "[" << v_id << "]->value[c_row][col] += ";
  code << "aux * dvdx->dg_dx[" << g_id << "]->value[c_row_g][col_g];" << endl;
  _jac_def.code.append(code.str());
}

string JacGenerator::getVariableIndexes(Equation eq, Deps::SBG::Map map)
{
  stringstream code;
  if (eq.hasRange()) {
    string tab = Utils::instance().tabs(_tabs);
    string args = Index(eq.lhs()).replace().usageExp();
    code << tab << "_apply_usage" << eq.applyId() << "(" << args << ");" << endl;
    eq.range()->addLocalVariables();
  }
  return code.str();
}

void JacGenerator::visitF(Equation eq, SBG::VariableDep var_dep, SBG::Map map)
{
  stringstream code;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  dependencyPrologue(eq, var_dep, map);
  string tab = Utils::instance().tabs(_tabs);
  generatePos(eq.arrayId(), eq.type());
  code << getVariableIndexes(eq, map);
  code << tab << "aux = " << ExpressionDerivator::partialDerivative(eq, Index(map.exp())) << ";" << endl;
  _jac_def.code.append(code.str());
  generateEquation(eq.arrayId(), eq.type());
  dependencyEpilogue();
}

void JacGenerator::visitG(Equation v_eq, Equation g_eq, SBG::VariableDep var_dep, SBG::Map n_map, Map map_m, SBG::Offset index_shift)
{
  stringstream code;
  string dom_guard = guard(var_dep.dom(), map_m);
  dependencyPrologue(g_eq, var_dep, n_map, dom_guard);
  generatePos(v_eq.arrayId(), v_eq.type());
  vector<string> variables;
  int shift = index_shift[0];
  string tab = Utils::instance().tabs(_tabs);
  static const bool USE_RANGE_IDXS = true;
  vector<string> exps;
  if (v_eq.hasRange()) {
    exps = map_m.exps(v_eq.range()->getDimensionVars(USE_RANGE_IDXS));
  } else {
    Option<Variable> lhs = v_eq.LHSVariable();
    if (lhs->isArray()) {
      for (Expression exp : v_eq.lhs().indexes()) {
        exps.push_back(exp.print());
      }
    }
  }
  Expression a_exp = Expression::generate(g_eq.LHSVariable()->name(), exps);
  Index a_ind(a_exp);
  code << tab << "c_row_g = ";
  code << a_ind << " - " << shift;
  code << ";" << endl;
  _jac_def.code.append(code.str());
  generatePos(g_eq.arrayId(), g_eq.type(), "c_row_g", "col_g");
  generateEquation(v_eq.arrayId(), g_eq.arrayId(), v_eq.type());
  dependencyEpilogue();
}

void JacGenerator::initG(Equation eq, SBG::Map map_m)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  code << getVariableIndexes(eq, map_m);
  code << tab << "aux = " << ExpressionDerivator::partialDerivative(eq, Index(map_m.exp())) << ";" << endl;
  _jac_def.code.append(code.str());
}

JacDef JacGenerator::deps() { return _jac_def; }

Jacobian::Jacobian() {}

void Jacobian::build()
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EquationTable derivatives = ModelConfig::instance().derivatives();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  JacobianBuilder jac;
  IndexShiftBuilder index_shifts(algebraics, symbols);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics, symbols);
  jac.compute(SDSBGraph.build(), index_shifts.build());
  _jac_def = jac.deps();
}

string Jacobian::code() { return _jac_def.code; }

}  // namespace IR
}  // namespace MicroModelica
