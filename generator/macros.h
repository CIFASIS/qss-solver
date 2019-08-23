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

#ifndef MMO_MACROS_H_
#define MMO_MACROS_H_

#include <fstream>
#include <string>
#include <list>

#include "../util/symbol_table.h"
#include "../ir/class.h"
#include "../ir/index.h"

namespace MicroModelica {
namespace Generator {

/**
 *
 */
class Macros {
  public:
  Macros(IR::Model& model, Util::Variable& variable);
  Macros() : _model(), _variable(){};
  ~Macros(){};
  std::string print() const { return _macros.str(); };
  std::string parameters() const;
  std::string engineIndex() const;
  std::string engineIndexArguments() const;
  std::string arguments() const;
  std::string usage(std::string token, Option<IR::Range> range, int id) const;
  std::string indexMacro(std::string token, Option<IR::Range> range, int id) const;
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

#endif /* MMO_MACROS_H_ */
