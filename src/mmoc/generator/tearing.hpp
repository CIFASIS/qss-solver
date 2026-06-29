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

#pragma once

#include <ir/equation.hpp>
#include <ir/expression.hpp>

namespace MicroModelica {
namespace Generator {

class Tearing {
  public:
  Tearing(IR::EquationTable derivatives, IR::EquationTable algebraics);
  ~Tearing() = default;
  void detect();
  IR::EquationTable derivatives();
  IR::EquationTable algebraics();
  IR::EquationTable iterationEquations();  
  IR::EquationTable variableEquations();
  bool detected();

  protected:
  IR::EquationTable _derivatives;
  IR::EquationTable _algebraics;
  IR::EquationTable _iteration_equations;
  IR::EquationTable _variable_equations;
  IR::EquationTable _algebraic_equations;
  IR::EquationTable _derivative_equations;
  IR::ExpressionTable _tearing_lhs;
  IR::ExpressionTable _tearing_rhs;
  std::map<int, IR::EQUATION::Type> _tearing_variable_type;
  int _max_tearing_var;
  bool _detect;

  void processEquations(IR::EquationTable& equations, IR::EquationTable& model_equations);

};

}  // namespace Generator
}  // namespace MicroModelica
