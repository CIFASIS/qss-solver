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
#include <util/error.hpp>

namespace MicroModelica {
namespace Generator {

Tearing::Tearing(IR::EquationTable derivatives, IR::EquationTable algebraics) : _derivatives(derivatives), _algebraics(algebraics) {}

void Tearing::addVariable(std::vector<Util::Variable>& variables, Util::Variable var)
{
  for (Util::Variable& v : variables) {
    if (v.name() == var.name()) {
      return;
    }
  }
  variables.push_back(var);
}

int Tearing::variablesSize(std::vector<Util::Variable> variables)
{
  int size = 0;
  for (Util::Variable& var : variables) {
    size += var.size();
  }
  return size;
}

// A tearing loop starts with the equations that assign the guess variables (v = tearing_var_guess_<id>),
// followed by the iteration equations and the residual equations (tearing_var_res_<id> = <exp> and
// tearing_var_res_<id> = 0). Each of them can be defined inside a for loop, the loop ends
// when the residuals of all the open guess variables are found.
void Tearing::processEquations(IR::EquationTable& equations, IR::EquationTable& model_equations)
{
  IR::EquationTable::iterator it;
  for (IR::Equation eq = equations.begin(it); !equations.end(it); eq = equations.next(it)) {
    if (eq.tearingEq(IR::EQUATION::Tearing::Var)) {
      _open_loops.insert(eq.tearingId(IR::EQUATION::Tearing::Var));
      addVariable(_guess_variables, eq.tearingVariable(IR::EQUATION::Tearing::Var).get());
      _iteration_equations.insert(eq.id(), eq);
      continue;
    }

    if (eq.tearingEq(IR::EQUATION::Tearing::Res)) {
      if (!_open_loops.empty() && !eq.isTearingResidualZero()) {
        addVariable(_residual_variables, eq.tearingVariable(IR::EQUATION::Tearing::Res).get());
        _residual_equations.insert(eq.id(), eq);
        _open_loops.erase(eq.tearingId(IR::EQUATION::Tearing::Res));
      }
    } else if (!_open_loops.empty()) {
      _iteration_equations.insert(eq.id(), eq);
    } else {
      model_equations.insert(eq.id(), eq);
    }
  }
}

void Tearing::detect()
{
  _open_loops.clear();
  processEquations(_algebraics, _algebraic_equations);
  processEquations(_derivatives, _derivative_equations);
  if (detected() && variablesSize(_guess_variables) != variablesSize(_residual_variables)) {
    Util::Error::instance().add(0, EM_CG | EM_EQ_DEF, Util::ER_Fatal,
                                "Tearing: the number of guess variables (%d) and residual variables (%d) differ.",
                                variablesSize(_guess_variables), variablesSize(_residual_variables));
  }
}

IR::EquationTable Tearing::derivatives() { return _derivative_equations; }

IR::EquationTable Tearing::algebraics() { return _algebraic_equations; }

IR::EquationTable Tearing::iterationEquations() { return _iteration_equations; }

IR::EquationTable Tearing::residualEquations() { return _residual_equations; }

std::vector<Util::Variable> Tearing::guessVariables() { return _guess_variables; }

std::vector<Util::Variable> Tearing::residualVariables() { return _residual_variables; }

int Tearing::size() { return variablesSize(_guess_variables); }

bool Tearing::detected() { return !_guess_variables.empty(); }

}  // namespace Generator
}  // namespace MicroModelica
