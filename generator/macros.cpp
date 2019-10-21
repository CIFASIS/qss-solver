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

#include "macros.h"

#include "../ir/helpers.h"

#include <sstream>

using namespace std;

namespace MicroModelica {
using namespace Util;
using namespace IR;
namespace Generator {

Macros::Macros(IR::Model& model, Util::Variable& variable) : _model(model), _variable(variable), _is_qss(false)
{
  _is_qss = !_model.annotations().isClassic() && (_variable.isAlgebraic() || _variable.isState());
  initialize();
}

string Macros::parameters(MACROS::Offset offset) const
{
  stringstream buffer;
  string dim_offset = "";
  if (offset == MACROS::Engine) {
    dim_offset = "-1";
  }
  int dim = _variable.dimensions();
  if (dim) {
    buffer << "(";
  }
  for (int i = 0; i < dim; i++) {
    buffer << "d" << i + 1 << dim_offset << (((i == dim - 1) && !_is_qss) ? ")" : ",");
  }
  if (_is_qss && dim) {
    buffer << "coeff)";
  } else if (_is_qss) {
    buffer << "(coeff)";
  }
  return buffer.str();
}

string Macros::arguments(bool state) const
{
  stringstream arguments;
  string index_offset = "-1";
  int dim = _variable.dimensions();
  stringstream end;
  if (_is_qss && state) {
    end << "*" << _model.annotations().polyCoeffs() << " + coeff";
  }
  if (dim) {
    arguments << "(";
    for (int i = 0; i < dim; i++) {
      stringstream variable;
      variable << "*" << _variable.rowSize(i) << "+";
      if (_variable.offset()) {
        arguments << _variable.offset() << "+";
      }
      arguments << "(d" << i + 1 << index_offset << ")" << (i == dim - 1 ? ")" + end.str() : variable.str());
    }
  } else if (_variable.isDiscrete() || _variable.isState() || _variable.isAlgebraic() || _variable.isOutput() || _variable.isEqType()) {
    arguments << _variable.offset() << end.str();
  }
  arguments << endl;
  return arguments.str();
}

string Macros::engineIndexArguments() const
{
  stringstream arguments;
  int dim = _variable.dimensions();
  stringstream end;
  if (dim) {
    arguments << "(";
    for (int i = 0; i < dim; i++) {
      stringstream variable;
      variable << "*" << _variable.rowSize(i) << "+";
      if (_variable.offset()) {
        arguments << _variable.offset() << "+";
      }
      arguments << "(d" << i + 1 << "-1)" << (i == dim - 1 ? ")" + end.str() : variable.str());
    }
  } else {
    arguments << _variable.offset() << end.str();
  }
  return arguments.str();
}

string Macros::engineIndex() const
{
  stringstream buffer;
  int dim = _variable.dimensions();
  if (dim) {
    buffer << "(";
  }
  for (int i = 0; i < dim; i++) {
    buffer << "d" << i + 1 << (i == dim - 1 ? ")" : ",");
  }
  return buffer.str();
}

void Macros::initialize()
{
  stringstream index, def, state_index, state_def, init_code;
  int dim = _variable.dimensions();
  bool idx = !_variable.isParameter() || dim;
  string params = parameters();
  if (idx) {
    index << "_idx" << _variable << params;
    static const bool MATRIX_INDEX = false;
    _macros << "#define " << index.str() << " " << arguments(MATRIX_INDEX);
    if (_is_qss) {
      state_index << "_state_idx" << _variable << params;
      _macros << "#define " << state_index.str() << " " << arguments();
    }
  }
  if (!_variable.isEqType() && !_variable.isOutput()) {
    _macros << "#define " << _variable << params << " ";
    if (_variable.isState()) {
      _macros << "x";
      init_code << "#define _init" << _variable << params << " x";
    }
    if (_variable.isAlgebraic()) {
      _macros << "a";
    }
    if (_variable.isDiscrete()) {
      _macros << "d";
    }
    if (_variable.isParameter()) {
      _macros << "__PAR__" << _variable.name();
    }
    if (idx) {
      def << "[";
    }
    if (_is_qss) {
      def << state_index.str();
    } else {
      def << index.str();
    }
    if (idx) {
      def << "]";
    }
    state_def << def.str();
    if (_is_qss) {
      state_def << " * COEFF_MULTIPLIER(coeff)";
    }
    _macros << state_def.str() << endl;
    if (_variable.isState()) {
      init_code << def.str() << endl;
      _macros << init_code.str();
    }
  }
  if (_variable.isState() || _variable.isOutput() || _variable.isEqType()) {
    FunctionPrinter fp = FunctionPrinter();
    int offset = _variable.offset();
    Range range = Range(_variable);
    _macros << "#define _eval" << _variable << parameters() << " ";
    _macros << engineIndexArguments() << endl;
    _macros << fp.accessMacros(_variable.print(), offset, range, _variable.isState());
  }
  if (_is_qss && _variable.isState()) {
    _macros << "#define _eval_dep" << _variable << params << " ";
    _macros << "dx" << def.str() << endl;
  }
}

string Macros::usage(string token, Option<Range> range, int id) const
{
  stringstream buffer, index;
  if (range) {
    index << "(" << range->indexes() << ")";
  }
  buffer << token << index.str();
  return buffer.str();
}

string Macros::indexMacro(string token, Option<Range> range, int id) const
{
  stringstream buffer, index, arguments;
  if (range) {
    index << "(" << range->indexes() << ")";
  }
  buffer << "#define " << token << index.str() << " ";
  if (range) {
    int dim = range->definition().size();
    stringstream end;
    RangeDefinitionTable rdt = range->definition();
    RangeDefinitionTable::iterator it;
    arguments << "(";
    int i = 0;
    for (RangeDefinition rd = rdt.begin(it); !rdt.end(it); rd = rdt.next(it), i++) {
      stringstream variable;
      variable << "*" << range->rowSize(i) << "+";
      arguments << "(" << rdt.key(it) << "-1)" << (i == dim - 1 ? ")" : variable.str());
    }
    buffer << arguments.str();
  } else {
    buffer << id - 1 << endl;
  }
  return buffer.str();
}

string Macros::modelAccess(int discretes, int algebraics)
{
  stringstream buffer;
  buffer << "#define MODEL_DATA_ACCESS(m) \\" << endl;
  if (discretes && algebraics) {
    buffer << "  double* x = m->x; \\" << endl;
    buffer << "  double* d = m->d; \\" << endl;
    buffer << "  double* a = m->alg;" << endl;
  } else if (discretes) {
    buffer << "  double* x = m->x; \\" << endl;
    buffer << "  double* d = m->d;" << endl;
  } else if (algebraics) {
    buffer << "  double* x = m->x; \\" << endl;
    buffer << "  double* a = m->alg;" << endl;
  } else {
    buffer << "  double* x = m->x; " << endl;
  }
  buffer << endl;
  return buffer.str();
}

string Macros::coeffMultipliers(int order)
{
  stringstream buffer;
  buffer << "#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c" << endl;
  switch (order) {
  case 1:
    buffer << "#define COEFF_MULTIPLIER_0 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_1 1" << endl;
    break;
  case 2:
    buffer << "#define COEFF_MULTIPLIER_0 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_1 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_2 2" << endl;
    break;
  case 3:
    buffer << "#define COEFF_MULTIPLIER_0 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_1 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_2 2" << endl;
    buffer << "#define COEFF_MULTIPLIER_3 6" << endl;
    break;
  case 4:
    buffer << "#define COEFF_MULTIPLIER_0 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_1 1" << endl;
    buffer << "#define COEFF_MULTIPLIER_2 2" << endl;
    buffer << "#define COEFF_MULTIPLIER_3 6" << endl;
    buffer << "#define COEFF_MULTIPLIER_4 24" << endl;
    break;
  }
  buffer << endl;
  return buffer.str();
}

std::ostream& operator<<(std::ostream& out, const Macros& m) { return out << m.print(); }
}  // namespace Generator
}  // namespace MicroModelica
