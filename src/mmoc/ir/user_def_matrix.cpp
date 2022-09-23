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

#include <assert.h>
#include <string>

#include <util/model_config.h>
#include <ir/user_def_matrix.h>
#include <util/error.h>
#include <util/visitors/convert_output_range.h>

namespace MicroModelica {
using namespace Util;
namespace IR {

template <typename Config, typename N>
UserDefMatrix<Config, N>::UserDefMatrix(Config matrix_config) : _model_matrix_def(), _access_vector(), _matrix_config(matrix_config)
{
}

template <typename Config, typename N>
void UserDefMatrix<Config, N>::compute()
{
  printMatrix(MATRIX::Alloc, MATRIX::Normal);
  printMatrix(MATRIX::Alloc, MATRIX::Transpose);
  printMatrix(MATRIX::Init, MATRIX::Normal);
  printMatrix(MATRIX::Init, MATRIX::Transpose);
}

template <typename Config, typename N>
string UserDefMatrix<Config, N>::component(MATRIX::Method method, MATRIX::Mode mode) const
{
  stringstream buffer;
  string component = _matrix_config.component[0];
  bool use_component = _matrix_config.use_component[mode];
  if (method == MATRIX::Init) {
    component = _matrix_config.component[1];
  }
  if (!component.empty() && use_component) {
    buffer << "." << component;
  }
  return buffer.str();
}

template <typename Config, typename N>
AST_Expression UserDefMatrix<Config, N>::transformExp(AST_Expression exp)
{
  assert(exp->expressionType() == EXPCOMPREF);
  AST_Expression_ComponentReference comp_ref = exp->getAsComponentReference();
  string comp_ref_name = comp_ref->name();
  Option<Variable> var = ModelConfig::instance().lookup(comp_ref_name);
  if (var) {
    return exp;
  }
  AST_Expression_ComponentReference new_var = newAST_Expression_ComponentReference();
  EventTable events = ModelConfig::instance().events();
  EventTable::iterator ev_it;
  string event_var_name;
  for (Event ev = events.begin(ev_it); !events.end(ev_it); ev = events.next(ev_it)) {
    if (ev.compareEventID(comp_ref_name)) {
      event_var_name = ev.zeroCrossing().LHSVariable()->name();
      break;
    }
  }
  if (event_var_name.empty()) {
    Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Fatal, "Wrong Event ID %s.", comp_ref_name);
  }
  if (comp_ref->hasIndexes()) {
    AST_ExpressionList indexes = comp_ref->firstIndex();
    AST_ExpressionListIterator exp_it;
    AST_ExpressionList new_indexes = newAST_ExpressionList();
    foreach (exp_it, indexes) {
      new_indexes = AST_ListAppend(new_indexes, current_element(exp_it));
    }
    new_var = AST_Expression_ComponentReference_Add(new_var, newAST_String(event_var_name), new_indexes);
  } else {
    new_var = AST_Expression_ComponentReference_Add(new_var, newAST_String(event_var_name), newAST_ExpressionList());
  }
  return new_var;
}

template <typename Config, typename N>
void UserDefMatrix<Config, N>::printMatrix(MATRIX::Method method, MATRIX::Mode mode)
{
  string matrix = _matrix_config.names[method + mode];
  string access = _matrix_config.access[mode];
  stringstream code;
  for (auto entry_def : _matrix_config.user_def) {
    int entry_id = entry_def.first;
    AST_ExpressionList entry_infs = entry_def.second;
    AST_ExpressionListIterator exp_it;
    foreach (exp_it, entry_infs) {
      AST_Expression exp = current_element(exp_it);
      N node = _matrix_config.selector.getNode(entry_id);
      Option<Range> range;
      Index ife_idx;
      Index ifr_idx;
      if (exp->expressionType() == EXPOUTPUT) {
        AST_Expression_Output matrix_exps = exp->getAsOutput();
        AST_ExpressionList matrix_entry = matrix_exps->expressionList();
        AST_ExpressionListIterator matrix_entry_it;
        if (matrix_entry->size() != 2) {
          Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Fatal, "Wrong user defined matrix expression");
        }
        bool first_exp = true;
        ConvertOutputRange convert;
        AST_Expression matrix_exp = nullptr;
        foreach (matrix_entry_it, matrix_entry) {
          if (first_exp) {
            matrix_exp = current_element(matrix_entry_it);
            first_exp = false;
          } else {
            assert(current_element(matrix_entry_it)->expressionType() == EXPBRACKET);
            AST_Expression_Bracket range_def = current_element(matrix_entry_it)->getAsBracket();
            AST_ExpressionList ranges = range_def->ranges();
            AST_ExpressionListIterator range_it;
            foreach (range_it, ranges) {
              convert.apply(current_element(range_it));
            }
            range = convert.range();
          }
        }
        ife_idx = Index(Expression(transformExp(matrix_exp)));
        ife_idx = ife_idx.replace(range);
      } else if (exp->expressionType() == EXPCOMPREF) {
        AST_Expression_ComponentReference comp_ref = exp->getAsComponentReference();
        ife_idx = Index(Expression(transformExp(comp_ref)));
        range = node.range();
        if (range) {
          range->replace(ife_idx);
        }
      } else {
        Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Fatal, "Wrong user defined matrix expression");
      }
      ifr_idx = Index(_matrix_config.selector.exp(node));
      ifr_idx = ifr_idx.replace();
      ife_idx = ife_idx.replace();

      if (range) {
        const bool GENERATE_RANGE_INDEX = false;
        const bool C_INDEX = false;
        const bool CONVERT_PARAMS = true;
        code << range->print(GENERATE_RANGE_INDEX, C_INDEX, CONVERT_PARAMS);
        code << range->block();
      }
      if (method == MATRIX::Alloc) {
        code << _matrix_config.container << matrix << "[" << ifr_idx << "]" << component(MATRIX::Alloc, mode) << "++;" << endl;
      } else {
        code << _matrix_config.container << matrix << "[" << ifr_idx << "]" << component(MATRIX::Init, mode) << "[" << access << "["
             << ifr_idx << "]++] = " << ife_idx << ";" << endl;
      }
      if (range) {
        code << range->end() << endl;
      }
    }
  }
  if (method == MATRIX::Alloc) {
    _model_matrix_def.alloc[mode] = code.str();
  } else {
    _model_matrix_def.init[mode] = code.str();
  }
}

template <typename Config, typename N>
ModelMatrixDef UserDefMatrix<Config, N>::def()
{
  return _model_matrix_def;
}

template <typename Config, typename N>
std::vector<std::string> UserDefMatrix<Config, N>::accessVector()
{
  return _access_vector;
}

template class UserDefMatrix<MATRIX::EQMatrixConfig, Equation>;

template class UserDefMatrix<MATRIX::EVMatrixConfig, Event>;

}  // namespace IR
}  // namespace MicroModelica
