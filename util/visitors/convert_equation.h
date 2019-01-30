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

#ifndef CONVERT_EQUATION_H_
#define CONVERT_EQUATION_H_

#include "../ast_util.h"

namespace MicroModelica {
  namespace Util {

    class ConvertEquation
    {
      public:
        ConvertEquation(AST_Equation equation, MicroModelica::Util::VarSymbolTable& symbols);
        ~ConvertEquation() {};
        inline AST_Equation 
        get() { return _equation; };
      private:
        AST_Equation 
        convert(AST_Equation eq);
        AST_Equation                        _equation;
        MicroModelica::Util::VarSymbolTable _symbols;

    };
  }
}
#endif  /* CONVERT_EQUATION_H_ */
