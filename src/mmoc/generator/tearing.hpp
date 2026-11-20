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

#include <set>
#include <vector>

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
  IR::EquationTable residualEquations();
  std::vector<Util::Variable> guessVariables();
  std::vector<Util::Variable> residualVariables();
  int size();
  bool detected();

  protected:
  IR::EquationTable _derivatives;
  IR::EquationTable _algebraics;
  IR::EquationTable _iteration_equations;
  IR::EquationTable _residual_equations;
  IR::EquationTable _algebraic_equations;
  IR::EquationTable _derivative_equations;
  std::vector<Util::Variable> _guess_variables;
  std::vector<Util::Variable> _residual_variables;
  std::set<std::string> _open_loops;

  void processEquations(IR::EquationTable& equations, IR::EquationTable& model_equations);
  void addVariable(std::vector<Util::Variable>& variables, Util::Variable var);
  int variablesSize(std::vector<Util::Variable> variables);
};

}  // namespace Generator
}  // namespace MicroModelica
