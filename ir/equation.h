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

#ifndef EQUATION_H_
#define EQUATION_H_

#include "../ast/ast_types.h"
#include "../util/table.h"
#include "index.h"

namespace MicroModelica {
  namespace IR {

    /**
     *
     */
    class Equation
    {
      public:
        /**
         *
         * @param exp
         */
        Equation(AST_Expression exp);
        Equation(AST_Expression exp, Range r);
        /**
         *
         */
        ~Equation();
        typedef enum
        {
          DERIVATIVE,
          DEPENDENCIES,
          CLASSIC,
          ALGEBRAIC,
          OUTPUT,
          ZC,
          HANDLER,
          HANDLER_IF,
          JACOBIAN
        } Type;

        friend std::ostream& operator<<(std::ostream& out, const Equation& e);
      private:
        Option<Range>   _range;
        AST_Expression  _eq;

    };

    typedef ModelTable<int,Equation> EquationTable;
  }
}
#endif  /* EQUATION_H_ */
