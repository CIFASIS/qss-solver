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

#include <string>

#include <ir/model_matrix_gen.h>

namespace MicroModelica {
namespace IR {

template<typename Config, typename N>
class UserDefMatrix {
  public:
  UserDefMatrix(Config matrix_config);
  ~UserDefMatrix() = default;

  void compute();  

  ModelMatrixDef def();
  std::vector<std::string> accessVector();
  
  protected:
  string component(MATRIX::Method method, MATRIX::Mode mode) const;
  void printMatrix(MATRIX::Method method, MATRIX::Mode mode);
  AST_Expression transformExp(AST_Expression exp);

  ModelMatrixDef _model_matrix_def;
  std::vector<std::string> _access_vector;
  Config _matrix_config;
};

}  // namespace IR
}  // namespace MicroModelica
