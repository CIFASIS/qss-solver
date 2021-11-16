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

#include "model_matrix_gen.h"

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

template <typename NT, typename N, typename Config>
ModelMatrixGenerator<NT, N, Config>::ModelMatrixGenerator() : _model_matrix_def(), _tabs(0), _deps(), _post_process_eval(false), _config() {}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::setup(Config config) { _config = config; }

template <typename NT, typename N, typename Config>
Config ModelMatrixGenerator<NT, N, Config>::config() { return _config; }

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::addCode(MatrixCode dep_code, std::stringstream& code)
{
  int size = dep_code.code.size();
  for (int i = 0; i < size; i++) {
    code << dep_code.begin[i];
    code << dep_code.code[i];
    if (!dep_code.end[i].empty()) {
      code << dep_code.end[i] << endl;
    }
  }
}

template <typename NT, typename N, typename Config>
string ModelMatrixGenerator<NT, N, Config>::component(MATRIX::Method method) const
{
  stringstream buffer;
  string component = _config.component[0];
  if (method == MATRIX::Init) {
    component = _config.component[1];
  }
  if (!component.empty()) {
    buffer << "." << component;
  }
  return buffer.str();
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::printMatrix(MATRIX::Method method, MATRIX::Mode mode)
{
  map<string, MatrixCode> deps_code;
  NT nodes = _config.selector.getNodes();
  VarSymbolTable variables = ModelConfig::instance().symbols();
  std::stringstream code;
  FunctionPrinter printer;
  for (auto variable : _deps) {
    list<DepData> var_deps = variable.second;
    Option<Variable> ifr = variables[variable.first];
    assert(ifr);
    for (DepData var_dep : var_deps) {
      string matrix = _config.names[method + mode];
      string access = _config.access[mode];
      std::stringstream begin;
      std::stringstream end;
      std::stringstream buffer;
      N node = _config.selector.getNode(var_dep.id);
      Expression use_exp = getUseExp(ifr.get(), var_dep);
      Option<Range> range = getUseRange<N>(ifr.get(), var_dep, node);
      Index ifr_idx(use_exp);
      string ifr_id = ifr_idx.identifier();
      N ifr_node = _config.selector.setUsage(ifr_idx, node, range);
      ifr_node = _config.selector.getScalarUsage(var_dep.var_dep.equations(), use_exp, ifr_node, ifr_idx);
      Index ife_idx(_config.selector.exp(ifr_node));
      Index ife_orig_idx = ife_idx;
      Index ifr_orig_idx = ifr_idx; 
      if (mode == MATRIX::Transpose) {
        Index swap = ifr_idx;
        ifr_idx = ife_idx;
        ife_idx = swap;
      }
      Index range_idx = ifr_idx;
      ifr_idx = ifr_idx.replace();
      ife_idx = ife_idx.replace();
      if (deps_code.find(ifr_id) == deps_code.end()) {
        MatrixCode dep_code;
        deps_code[ifr_id] = dep_code;
      }
      MatrixCode dep_code = deps_code[ifr_id];
      if (range) {
        if (!var_dep.var_dep.isRecursive()) {
          range->replace(range_idx);
        }
        dep_code.begin.push_back(range->print());
        dep_code.end.push_back(range->end());
        buffer << range->block();
      } else {
        if (ifr_orig_idx.isConstant() && ifr_node.hasRange()) {
          Range der_range = ifr_node.range().get();
          der_range.replace(ife_orig_idx);
          dep_code.begin.push_back(der_range.print());
          dep_code.end.push_back(der_range.end()); 
        } else {
          dep_code.begin.push_back("");
          dep_code.end.push_back("");
        }
      }
      if (method == MATRIX::Alloc) {
        buffer << _config.container << matrix << "[" << ifr_idx << "]" << component(MATRIX::Alloc) << "++;" << endl;
      } else {
        buffer << _config.container << matrix << "[" << ifr_idx << "]" << component(MATRIX::Init) << "[" << access << "[" << ifr_idx
               << "]++] = " << ife_idx << ";" << endl;
      }
      dep_code.code.push_back(buffer.str());
      deps_code[ifr_id] = dep_code;
    }
  }
  for (auto dc : deps_code) {
    MatrixCode dep_code = dc.second;
    addCode(dep_code, code);
  }
  if (method == MATRIX::Alloc) {
    _model_matrix_def.alloc[mode] = code.str();
  } else {
    _model_matrix_def.init[mode] = code.str();
  }
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::postProcess(SB::Deps::SetVertex vertex)
{
  if (_post_process_eval) {
    return;
  }
  printMatrix(MATRIX::Alloc, MATRIX::Normal);
  printMatrix(MATRIX::Alloc, MATRIX::Transpose);
  printMatrix(MATRIX::Init, MATRIX::Normal);
  printMatrix(MATRIX::Init, MATRIX::Transpose);
  _post_process_eval = true;
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::init(SB::Deps::SetVertex vertex) {}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::end() {}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep)
{
  if (!_config.selector.isAlgebraic(vertex)) {
    string var_name = var_dep.var().name();
    DepData dep_data(_config.selector.id(vertex), var_dep);
    if (!findDep(_deps, dep_data, _config.selector.multipleNodes())) {
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    }
  }
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::visitF(SB::Deps::SetVertex vertex, SB::Deps::VariableDep var_dep, SB::Deps::SetVertex gen_vertex) {}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::Deps::VariableDep var_dep,
                                  int index_shift)
{
  if (!_config.selector.isAlgebraic(v_vertex)) {
    string var_name = var_dep.var().name();
    const bool FROM_ALG = true;
    DepData dep_data(_config.selector.id(v_vertex), var_dep, FROM_ALG);
    if (!findDep(_deps, dep_data, _config.selector.multipleNodes())) {
      list<DepData> deps = _deps[var_name];
      deps.push_back(dep_data);
      _deps[var_name] = deps;
    }
  }
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::visitG(SB::Deps::SetVertex v_vertex, SB::Deps::SetVertex g_vertex, SB::PWLMap use_map,
                                  SB::Deps::LMapExp use_map_exp, Expression use_exp, SB::PWLMap def_map, SB::Deps::LMapExp def_map_exp,
                                  SB::Set intersection)
{
}

template <typename NT, typename N, typename Config>
void ModelMatrixGenerator<NT, N, Config>::initG(SB::Deps::SetVertex vertex, SB::Deps::SetEdge edge) {}

template <typename NT, typename N, typename Config>
ModelMatrixDef ModelMatrixGenerator<NT, N, Config>::def() { return _model_matrix_def; }

namespace MATRIX {

template class  MatrixConfig<Deps::EQSelector>;

template class  MatrixConfig<Deps::EVSelector>;

} // namespace MATRIX

template class ModelMatrixGenerator<EquationTable, Equation, MATRIX::EQMatrixConfig>;

template class ModelMatrixGenerator<EventTable, Event, MATRIX::EVMatrixConfig>;

}  // namespace IR
}  // namespace MicroModelica
