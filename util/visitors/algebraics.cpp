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

#include "algebraics.h"

namespace MicroModelica {
namespace Util {

bool Algebraics::foldTraverseElement(AST_Expression e)
{
  bool has_algebraics = false;
  switch (e->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    Option<Variable> var = _symbols[cr->name()];
    if (var && var->isAlgebraic()) {
      has_algebraics = true;
    }
  }
  default:
    return has_algebraics;
  }
}
}  // namespace Util
}  // namespace MicroModelica
