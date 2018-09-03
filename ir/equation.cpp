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

#include "equation.h"

#include "../ast/expression.h"
#include "../util/ast_util.h"
#include "../util/util.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    Equation::Equation(AST_Expression eq, VarSymbolTable& symbols, EQUATION::Type type) :
      _eq(),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols)
    {
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(eq);
      CalledFunctions cf;
      _calledFunctions = cf.apply(eq);
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols)
    {
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Range r, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols)
    {
    }
 
    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Option<Range> r, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols)
    {
    }   
    
    std::ostream& operator<<(std::ostream& out, const Equation& e)
    {
      return out;
    }
  }
}
