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

#include "called_functions.h"
#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

SymbolTable CalledFunctions::foldTraverseElement(AST_Expression exp)
{
  SymbolTable symbols;
  switch (exp->expressionType()) {
  case EXPCALL: {
    AST_Expression_Call call = exp->getAsCall();
    symbols.insert(*call->name(), *call->name());
    return symbols;
  }
  default:
    return symbols;
  }
}

SymbolTable CalledFunctions::foldTraverseElement(SymbolTable l, SymbolTable r, BinOpType bot)
{
  r.merge(l);
  return r;
}

SymbolTable StatementCalledFunctions::foldTraverse(SymbolTable s1, SymbolTable s2)
{
  s2.merge(s1);
  return s2;
}

}  // namespace Util
}  // namespace MicroModelica
