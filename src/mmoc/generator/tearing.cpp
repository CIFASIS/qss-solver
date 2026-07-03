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

#include "tearing.hpp"

#include <ir/index.hpp>

namespace MicroModelica {
namespace Generator {

Tearing::Tearing(IR::EquationTable derivatives, IR::EquationTable algebraics)
    : _derivatives(derivatives), _algebraics(algebraics), _max_tearing_var(0), _detect(false)
{
}

void Tearing::processEquations(IR::EquationTable& equations, IR::EquationTable& model_equations)
{
  IR::EquationTable::iterator it;
  for (IR::Equation eq = equations.begin(it); !equations.end(it); eq = equations.next(it)) {
    if (eq.tearingEq(IR::EQUATION::Tearing::Var)) {
      _detect = true;
      _max_tearing_var = eq.tearingEqNumber(IR::EQUATION::Tearing::Var);
      _tearing_lhs.insert(_max_tearing_var, eq.lhs());
      _tearing_variable_type[_max_tearing_var] = eq.type();
      continue;
    }

    if (_detect) {
      int curr_tearing_var = eq.tearingEqNumber(IR::EQUATION::Tearing::Res);
      if (eq.tearingEq(IR::EQUATION::Tearing::Res)) {
        if (!_tearing_rhs.lookup(curr_tearing_var)) {
          _tearing_rhs.insert(curr_tearing_var, eq.rhs());
          if (curr_tearing_var == _max_tearing_var) {
            _detect = false;
          }
        }
      } else {
        _iteration_equations.insert(eq.id(), eq);
      }
    } else if (!eq.tearingEq(IR::EQUATION::Tearing::Res)) {
      model_equations.insert(eq.id(), eq);
    }
  }
}

void Tearing::detect()
{
  _detect = false;
  _max_tearing_var = 0;

  processEquations(_algebraics, _algebraic_equations);
  processEquations(_derivatives, _derivative_equations);

  IR::ExpressionTable::iterator it;
  for (IR::Expression lhs = _tearing_lhs.begin(it); !_tearing_lhs.end(it); lhs = _tearing_lhs.next(it)) {
    int id = _tearing_lhs.key(it);
    Option<IR::Expression> rhs = _tearing_rhs[id];
    IR::EQUATION::Type type = _tearing_variable_type[id];
    assert(rhs);
    IR::Equation tearing_eq = IR::Equation(lhs.expression(), rhs.get().expression(), Option<IR::Range>(), type, id);
    _variable_equations.insert(id, tearing_eq);
  }
}

IR::EquationTable Tearing::derivatives() { return _derivative_equations; }

IR::EquationTable Tearing::algebraics() { return _algebraic_equations; }

IR::EquationTable Tearing::iterationEquations() { return _iteration_equations; }

IR::EquationTable Tearing::variableEquations() { return _variable_equations; }

bool Tearing::detected() { return _variable_equations.size() > 0; }

}  // namespace Generator
}  // namespace MicroModelica
