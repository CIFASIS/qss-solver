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

#include <util/error.h>
#include <util/symbol_table.h>
#include <util/visitors/variable_lookup.h>

namespace MicroModelica {
namespace Util {

VariableLookup::VariableLookup(VarSymbolTable symbols) : _symbols(symbols) {}

bool VariableLookup::foldTraverseElement(AST_Expression exp)
{
  if (exp->expressionType() == EXPCOMPREF) {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = _symbols[cr->name()];
    if (!var) {
      return false;
    }
  }
  return true;
}

bool VariableLookup::foldTraverseElement(bool l, bool r, BinOpType bot) { return l && r; }

bool VariableLookup::foldTraverseElementUMinus(AST_Expression exp) { return foldTraverseElement(exp); }

}  // namespace Util
}  // namespace MicroModelica
