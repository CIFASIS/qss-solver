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

#include "has_scalar_index.h"

#include "is_constant_index.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

HasScalarIndex::HasScalarIndex() {}

bool HasScalarIndex::foldTraverseElement(AST_Expression exp)
{
  bool ret = false;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      IsConstantIndex constant_index;
      return constant_index.apply(cr);
    }
    break;
  }
  default:
    break;
  }
  return ret;
}

}  // namespace Util
}  // namespace MicroModelica
