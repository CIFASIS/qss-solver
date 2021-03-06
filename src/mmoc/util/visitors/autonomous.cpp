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

#include "autonomous.h"
#include "../model_config.h"

namespace MicroModelica {
namespace Util {

bool Autonomous::foldTraverseElement(AST_Expression e)
{
  bool autonomous = true;
  switch (e->expressionType()) {
  case EXPCALL: {
    AST_Expression_Call call = e->getAsCall();
    AST_ExpressionListIterator it;
    foreach (it, call->arguments()) {
      autonomous &= apply(current_element(it));
    }
    return autonomous;
  }
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (var && var->isTime()) {
      autonomous = false;
    }
  }
  default:
    return autonomous;
  }
}
}  // namespace Util
}  // namespace MicroModelica
