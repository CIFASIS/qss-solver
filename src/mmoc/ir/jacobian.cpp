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
using namespace SB;
using namespace Util;
namespace IR {

JacGenerator::JacGenerator() : _jac_def(), _tabs(0) {}

void JacGenerator::postProcess(SB::Deps::SetVertex vertex)
{
  int size = vertex.range().size();
  int id = vertex.index() - 1;
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  code << "// Assign Jacobian Matrix values for equation: " << id << endl;
  code << tab << "for (row = 0; row < " << size << "; row++) {" << endl;
  code << tab << "  for (col = 0; col < dvdx->df_dx[" << id << "]->size[row]; col++) {" << endl;
  code << tab << "    row_t = dvdx->df_dx[" << id << "]->index[row][col];" << endl;
  code << tab << "    _assign_jac(row_t, dvdx->df_dx[" << id << "]->value[row][col]);" << endl;
  code << tab << "  }" << endl;
  code << tab << "}" << endl;
  _jac_def.code.append(code.str());
}

void JacGenerator::init(SB::Deps::SetVertex vertex)
{
  stringstream code;
  FunctionPrinter function_printer;
  Equation eq = getEquation(vertex);
  code << "for(row = 1; row <= " << vertex.range().size() << "; row++) {" << endl;
  code << TAB << "c_row = _c_index(row);" << endl;
  code << function_printer.jacMacrosAccess(eq);
  _tabs++;
  _jac_def.code.append(code.str());
}

void JacGenerator::end()
{
  _jac_def.code.append("}\n");
  _tabs--;
}

std::string JacGenerator::guard(SB::Set dom, int offset, std::string var_name, SB::Deps::LMapExp map)
{
  if (map.constantExp()) {
    return "";
  }
  Range range(dom, offset);
  stringstream code;
  vector<string> exps = map.apply(range.getDimensionVars());
  Expression map_exp = Expression::generate(var_name, exps);
  range.applyUsage(Index(map_exp));
  return range.in(exps);
}

void JacGenerator::dependencyPrologue(Equation eq, SB::Deps::VariableDep var_dep, std::string guard)
{
  stringstream code;
  string tabs = Utils::instance().tabs(_tabs);
  SB::Deps::LMapExp map = var_dep.nMap(); 
  Range range(var_dep.variables(), var_dep.varOffset());
  if (var_dep.isRecursive() && eq.hasRange()) {
    FunctionPrinter printer;
    code << tabs << eq.range().get();
    _tabs++;
    tabs = Utils::instance().tabs(_tabs);
    vector<string> exps = eq.range()->getIndexes();
    Expression a_exp = Expression::generate(eq.LHSVariable()->name(), exps);
    Index a_ind(a_exp);
    code << TAB << printer.jacMacrosAccess(eq, a_ind.print());
  }
  vector<string> exps = map.apply(range.getDimensionVars());
  if (!map.constantExp()) {
    code << tabs << "if(" << range.in(exps); 
    if (!guard.empty()) {
      code << " && " << guard;
    }
    code << ") {" << endl;
  } else if (!guard.empty()) {
    code << tabs << " if(" << guard << ") {" << endl;
  }
  Expression i_exp = Expression::generate(var_dep.var().name(), exps);
  Index x_ind(i_exp);
  if (!map.constantExp() || !guard.empty()) {
    _tabs++;
    tabs = tabs + TAB;
  }
  code << tabs << "x_ind = " << x_ind << ";" << endl;
  _jac_def.code.append(code.str());
}

void JacGenerator::dependencyEpilogue(Equation eq, SB::Deps::VariableDep var_dep)
{
  stringstream code;
  if (!var_dep.nMap().constantExp() || !var_dep.mMap().constantExp()) {
    _tabs--;
    code << Utils::instance().tabs(_tabs) + "}\n";
  }
  if (var_dep.isRecursive() && eq.hasRange()) {
    code << eq.range()->end();
  }
  _jac_def.code.append(code.str());
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

string JacGenerator::getVariableIndexes(Equation eq)
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

void JacGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  Equation eq = getEquation(vertex);
  Fvisitor(vertex, var_dep, eq.arrayId());
}

void JacGenerator::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex)
{
  Equation eq = getEquation(gen_vertex);
  Fvisitor(vertex, var_dep, eq.arrayId());
}

void JacGenerator::Fvisitor(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, int eq_id)
{
  stringstream code;
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  Equation eq = getEquation(vertex);
  dependencyPrologue(eq, var_dep);
  string tab = Utils::instance().tabs(_tabs);
  generatePos(eq_id, eq.type());
  code << getVariableIndexes(eq);
  code << tab << "aux = " << ExpressionDerivator::partialDerivative(eq, Index(var_dep.exp())) << ";" << endl;
  _jac_def.code.append(code.str());
  generateEquation(eq_id, eq.type());
  dependencyEpilogue(eq, var_dep);
}

void JacGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep, int index_shift)
{
  stringstream code;
  Equation v_eq = getEquation(v_vertex);
  Equation g_eq = getEquation(g_vertex);
  // Generate composed expression for guards
  SB::Deps::LMapExp map_m = var_dep.mMap();
  SB::Deps::LMapExp n_map = var_dep.nMap();
  string dom_guard = guard(var_dep.equations(), var_dep.eqOffset(), var_dep.var().name(), map_m);
  dependencyPrologue(g_eq, var_dep, dom_guard);
  generatePos(v_eq.arrayId(), v_eq.type());
  vector<string> variables;
  string tab = Utils::instance().tabs(_tabs);
  static const bool USE_RANGE_IDXS = true;
  vector<string> exps;
  if (v_eq.hasRange()) {
    exps = map_m.apply(v_eq.range()->getDimensionVars(USE_RANGE_IDXS));
  } else {
    Option<Variable> v_lhs = v_eq.LHSVariable();
    Option<Variable> g_lhs = g_eq.LHSVariable();
    if (var_dep.isRecursive()) {
      Range range(var_dep.variables(), var_dep.varOffset());
      exps = n_map.apply(range.getDimensionVars(USE_RANGE_IDXS));
    } else if (g_lhs->isArray()) { 
      Range range(var_dep.variables(), var_dep.varOffset());
      exps = range.getInitValues();
    } else if (v_lhs->isArray()) {
      for (Expression exp : v_eq.lhs().indexes()) {
        exps.push_back(exp.print());
      }
    } 
  }
  Expression a_exp = Expression::generate(g_eq.LHSVariable()->name(), exps);
  Index a_ind(a_exp);
  code << tab << "c_row_g = ";
  code << a_ind << " - " << index_shift;
  code << ";" << endl;
  _jac_def.code.append(code.str());
  int g_eq_id = g_eq.arrayId();
  generatePos(g_eq_id, g_eq.type(), "c_row_g", "col_g");
  generateEquation(v_eq.arrayId(), g_eq_id, v_eq.type());
  dependencyEpilogue(g_eq, var_dep);
}

void JacGenerator::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map, SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map,
              SB::Deps::LMapExp def_map_exp, SB::Set intersection)
{

}

void JacGenerator::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge)
{
  stringstream code;
  string tab = Utils::instance().tabs(_tabs);
  Equation eq = getEquation(vertex);
  code << getVariableIndexes(eq);
  code << tab << "aux = " << ExpressionDerivator::partialDerivative(eq, Index(edge.desc().exp())) << ";" << endl;
  _jac_def.code.append(code.str());
}

JacDef JacGenerator::def() { return _jac_def; }

Jacobian::Jacobian() {}

void Jacobian::build()
{
  EquationTable algebraics = ModelConfig::instance().algebraics();
  EquationTable derivatives = ModelConfig::instance().orderedDerivatives();
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  JacobianBuilder jac;
  IndexShiftBuilder index_shifts(algebraics);
  SDSBGraphBuilder SDSBGraph = SDSBGraphBuilder(derivatives, algebraics);
  SDSBGraph.setOrigEquations(ModelConfig::instance().derivatives());
  jac.compute(SDSBGraph.build(), index_shifts.build());
  _jac_def = jac.def();
}

string Jacobian::code() { return _jac_def.code; }

}  // namespace IR
}  // namespace MicroModelica
