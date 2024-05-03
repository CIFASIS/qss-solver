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
  vector<int> v;
  _functions.insert("reinit", CompiledFunction("reinit", "", "", s, v, ""));
  _functions.insert("terminate", CompiledFunction("terminate", "", "", s,  v,""));
  _functions.insert("abs", CompiledFunction("abs", "", "", s, v, ""));
  _functions.insert("sign", CompiledFunction("sign", "", "", s, v, ""));
  _functions.insert("sqrt", CompiledFunction("sqrt", "", "", s, v, ""));
  _functions.insert("ceil", CompiledFunction("ceil", "", "", s, v, ""));
  _functions.insert("floor", CompiledFunction("floor", "", "", s, v, ""));
  _functions.insert("sin", CompiledFunction("sin", "", "", s, v, ""));
  _functions.insert("cos", CompiledFunction("cos", "", "", s, v, ""));
  _functions.insert("tan", CompiledFunction("tan", "", "", s, v, ""));
  _functions.insert("asin", CompiledFunction("asin", "", "", s, v, ""));
  _functions.insert("acos", CompiledFunction("acos", "", "", s, v, ""));
  _functions.insert("atan", CompiledFunction("atan", "", "", s, v, ""));
  _functions.insert("atan2", CompiledFunction("atan2", "", "", s, v, ""));
  _functions.insert("sinh", CompiledFunction("sinh", "", "", s, v, ""));
  _functions.insert("cosh", CompiledFunction("cosh", "", "", s, v, ""));
  _functions.insert("tanh", CompiledFunction("tanh", "", "", s, v, ""));
  _functions.insert("exp", CompiledFunction("exp", "", "", s, v, ""));
  _functions.insert("log", CompiledFunction("log", "", "", s, v, ""));
  _functions.insert("log10", CompiledFunction("log10", "", "", s, v, ""));
  _functions.insert("pre", CompiledFunction("pre", "", "", s, v, ""));
  _functions.insert("GQLink_GetB", CompiledFunction("GQLink_GetB", "", "", s, v, ""));
  _functions.insert("GQLink_GetBx", CompiledFunction("GKLink_GetBx", "", "", s, v, ""));
  _functions.insert("GQLink_GetBy", CompiledFunction("GKLink_GetBy", "", "", s, v, ""));
  _functions.insert("GQLink_GetBz", CompiledFunction("GKLink_GetBz", "", "", s, v, ""));
}

}  // namespace IR
}  // namespace MicroModelica
