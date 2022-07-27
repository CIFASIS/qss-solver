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

#include <fstream>
#include <string>
#include <list>

#include <ir/class.h>
#include <ir/index.h>
#include <util/symbol_table.h>

namespace MicroModelica {
namespace Generator {

namespace MACROS {
typedef enum { Modelica, Engine } Offset;
}

class Macros {
  public:
  Macros(IR::Model& model, Util::Variable& variable);
  Macros() : _model(), _variable(){};
  ~Macros() = default;
  std::string print() const;
  std::string parameters(MACROS::Offset offset = MACROS::Modelica) const;
  std::string engineIndex() const;
  std::string engineIndexArguments() const;
  std::string arguments(bool state = true) const;
  std::string usage(std::string token, Option<IR::Range> range, int id) const;
  std::string indexMacro(std::string token, Option<IR::Range> range, int id) const;
  std::string modelAccess(int discretes, int algebraics);
  std::string coeffMultipliers(int order);
  friend std::ostream& operator<<(std::ostream& out, const Macros& m);

  protected:
  void initialize();

  private:
  IR::Model _model;
  Util::Variable _variable;
  bool _is_qss;
  std::stringstream _macros;
};
}  // namespace Generator
}  // namespace MicroModelica
