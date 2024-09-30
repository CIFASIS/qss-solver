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

#include <ir/helpers.h>
#include <util/table.h>

namespace MicroModelica {
namespace IR {

class BuiltInFunction {
  public:
  static BuiltInFunction& instance()
  {
    static BuiltInFunction _instance;
    return _instance;
  }
  ~BuiltInFunction() = default;
  inline CompiledFunctionTable functions() { return _functions; };
  inline bool isValid(std::string func) { return _functions[func].is_initialized(); };
  inline bool lookup(std::string func_name) { return _functions.lookup(func_name); };

  private:
  BuiltInFunction();
  CompiledFunctionTable _functions;
};
}  // namespace IR
}  // namespace MicroModelica
