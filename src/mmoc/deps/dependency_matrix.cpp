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

#include "dependency_matrix.h"

#include "../ir/alg_usage.h"
#include "../ir/helpers.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/visitors/replace_index.h"

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Deps {

VariableDependency::VariableDependency()
    : _dom(),
      _ran(),
      _ifr(),
      _ife(),
      _ifr_exp(),
      _ife_exp(),
      _alg_usage(),
      _range(),
      _id(0),
      _equationId(0),
      _variable(),
      _is_reduction(false),
      _ifr_range(),
      _swap_usage(false),
      _equation_range()
{
}

bool VariableDependency::operator==(const VariableDependency &other) const
{
  string dep_value;
  string other_dep_value;
  dep_value = _ifr_exp.print();
  other_dep_value = other._ifr_exp.print();
  if (dep_value.compare(other_dep_value)) {
    return false;
  }
  dep_value = _ife_exp.print();
  other_dep_value = other._ife_exp.print();
  if (dep_value.compare(other_dep_value)) {
    return false;
  }
  dep_value = _alg_usage.print();
  other_dep_value = other._alg_usage.print();
  if (dep_value.compare(other_dep_value)) {
    return false;
  }
  if (_dom != other._dom) {
    return false;
  }
  if (_ran != other._ran) {
    return false;
  }
  return true;
}

bool VariableDependency::operator<(const VariableDependency &other) const
{
  string dep_value;
  string other_dep_value;
  dep_value = _ifr_exp.print();
  other_dep_value = other._ifr_exp.print();
  if (dep_value.compare(other_dep_value) < 0) {
    return true;
  }
  dep_value = _ife_exp.print();
  other_dep_value = other._ife_exp.print();
  if (dep_value.compare(other_dep_value) < 0) {
    return true;
  }
  if (_dom < other._dom) {
    return true;
  }
  return false;
}

MDI VariableDependency::getImage(MDI sub_dom)
{
  IndexPair p(sub_dom, _ran, _ifr.GetOffset(), _ifr.GetUsage(), _ifr.exp());
  INDEX_PAIR::Rel rel = p.Type();
  switch (rel) {
  case INDEX_PAIR::RN_N:
    return sub_dom.RevertOffset(_ifr.GetOffset(), _ifr.GetUsage(), _ifr.Ran());
  case INDEX_PAIR::R1_1:
  case INDEX_PAIR::R1_N:
  case INDEX_PAIR::RN_1:
    return _ran;
  default:
    return MDI();
  }
}

void VariableDependency::setRange(int equation_id)
{
  if (_is_reduction) {
    _range.generate(_ifr_range);
  } else {
    _range.generate(_ran);
  }
  // First apply the incoming range to the variable
  _ifr_exp = VariableUsage(_ifr.exp(), _range).usage();
  _ife_exp = VariableUsage(_ife.exp(), _range).usage();
  // If there's an algebraic usage in the variable dependency, apply the usage.
  if (_alg_usage.isEmpty() || _is_reduction) {
    _alg_usage = _ife_exp;
  } else {
    if (_swap_usage) {
      Expression tmp = _alg_usage;
      _alg_usage = _ife_exp;
      _ife_exp = tmp;
    }
    EquationTable algebraics = ModelConfig::instance().algebraics();
    Option<Equation> alg = algebraics[equation_id];
    if (alg) {
      Expression eq_lhs = alg->lhs();
      //cout << "LHS de la ecuacion: " << eq_lhs << endl;
      //cout << "Uso algebraico: " << _alg_usage << endl;
      //cout << "Se aplica a: " << _ife_exp << endl;
      _ife_exp = VariableUsage(eq_lhs, _ife_exp, Index(_alg_usage)).rhs();
    } else {
      _ife_exp = VariableUsage(_ife_exp, _ife_exp, Index(_alg_usage)).rhs();
    }
  }

  Index reverted_index = ife().revert();
  if (!reverted_index.isConstant()) {
    _range.applyUsage(ife().revert());
  }
  // cout << "Rango en el que se aplica: " << _ran << endl;
  // cout << "Expression IFR original: " << _ifr.exp() << endl;
  // cout << "Expression IFE original: " << _ife.exp() << endl;
  // cout << "Expression IFR en la matriz: " << _ifr_exp << endl;
  // cout << "Expression IFE en la matriz: " << _ife_exp << endl;
}

void VariableDependency::setSwap(bool swap_usage) { _swap_usage = swap_usage; }

Range VariableDependency::ifrRange()
{
  Range range;
  range.generate(_ifr_range);
  return range;
}

void VariableDependency::setEquationRange(Range equation_range) { _equation_range = equation_range; }

Range VariableDependency::equationRange() { return _equation_range; }

}  // namespace Deps
}  // namespace MicroModelica