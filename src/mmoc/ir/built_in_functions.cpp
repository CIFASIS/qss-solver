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

#include <sstream>

#include "built_in_functions.h"
#include "../util/error.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

BuiltInFunction::BuiltInFunction() : _functions()
{
  SymbolTable s;
  _functions.insert("reinit", CompiledFunction("reinit", "", "", s, ""));
  _functions.insert("terminate", CompiledFunction("terminate", "", "", s, ""));
  _functions.insert("abs", CompiledFunction("abs", "", "", s, ""));
  _functions.insert("sign", CompiledFunction("sign", "", "", s, ""));
  _functions.insert("sqrt", CompiledFunction("sqrt", "", "", s, ""));
  _functions.insert("ceil", CompiledFunction("ceil", "", "", s, ""));
  _functions.insert("floor", CompiledFunction("floor", "", "", s, ""));
  _functions.insert("sin", CompiledFunction("sin", "", "", s, ""));
  _functions.insert("cos", CompiledFunction("cos", "", "", s, ""));
  _functions.insert("tan", CompiledFunction("tan", "", "", s, ""));
  _functions.insert("asin", CompiledFunction("asin", "", "", s, ""));
  _functions.insert("acos", CompiledFunction("acos", "", "", s, ""));
  _functions.insert("atan", CompiledFunction("atan", "", "", s, ""));
  _functions.insert("atan2", CompiledFunction("atan2", "", "", s, ""));
  _functions.insert("sinh", CompiledFunction("sinh", "", "", s, ""));
  _functions.insert("cosh", CompiledFunction("cosh", "", "", s, ""));
  _functions.insert("tanh", CompiledFunction("tanh", "", "", s, ""));
  _functions.insert("exp", CompiledFunction("exp", "", "", s, ""));
  _functions.insert("log", CompiledFunction("log", "", "", s, ""));
  _functions.insert("log10", CompiledFunction("log10", "", "", s, ""));
  _functions.insert("pre", CompiledFunction("pre", "", "", s, ""));
  _functions.insert("GQLink_GetB", CompiledFunction("GQLink_GetB", "", "", s, ""));
  _functions.insert("GQLink_GetBx", CompiledFunction("GKLink_GetBx", "", "", s, ""));
  _functions.insert("GQLink_GetBy", CompiledFunction("GKLink_GetBy", "", "", s, ""));
  _functions.insert("GQLink_GetBz", CompiledFunction("GKLink_GetBz", "", "", s, ""));
}

}  // namespace IR
}  // namespace MicroModelica
