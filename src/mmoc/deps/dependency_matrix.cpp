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
#include "../util/visitors/replace_index.h"
#include "../util/util.h"

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

void VariableDependency::setRange()
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
  if (_alg_usage.isEmpty()) {
    _alg_usage = _ife_exp;
  } else {
    if (_swap_usage) {
      Expression tmp = _alg_usage;
      _alg_usage = _ife_exp;
      _ife_exp = tmp;
    }
    _ife_exp = VariableUsage(_ife_exp, _ife_exp, Index(_alg_usage)).rhs();
  }
  cout << "Rango en el que seaplica: " << _ran << endl;
  cout << "Expression IFR original: " << _ifr.exp() << endl;
  cout << "Expression IFE original: " << _ife.exp() << endl;
  cout << "Expression IFR en la matriz: " << _ifr_exp << endl;
  cout << "Expression IFE en la matriz: " << _ife_exp << endl;
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