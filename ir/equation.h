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

#ifndef MMO_EQUATION_H_
#define MMO_EQUATION_H_

#include "../ast/ast_types.h"

typedef enum
{
  EQ_DERIVATIVE,
  EQ_DEPENDENCIES,
  EQ_CLASSIC,
  EQ_ALGEBRAIC,
  EQ_OUTPUT,
  EQ_ZC,
  EQ_HANDLER,
  EQ_HANDLER_IF,
  EQ_JACOBIAN
} EQ_Type;

/**
 *
 */
class MMO_Equation
{
  public:
    /**
     *
     * @param exp
     */
    MMO_Equation(AST_Expression exp);
    /**
     *
     */
    ~MMO_Equation();
};

#endif  /* MMO_EQUATION_H_ */
