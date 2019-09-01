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

#ifndef PROCESS_STATEMENT_H_
#define PROCESS_STATEMENT_H_

#include "../ast/ast_types.h"
#include "../ast/statement.h"

namespace MicroModelica {
namespace Util {

namespace STATEMENT {
typedef enum { LHS, RHS, LHS_DISCRETES, LHS_STATES } AssignTerm;
}

AST_Statement processStatement(AST_Statement stm);

}  // namespace Util
}  // namespace MicroModelica
#endif /* PROCESS_STATEMENT_H_ */