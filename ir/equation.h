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
  namespace Util {
    typedef ModelTable<std::string,std::string> SymbolTable;
  }

  namespace IR {
    namespace EQUATION 
    {
      typedef enum
      {
        Derivative,
        Algebraic,
        Dependency,
        CLASSIC,
        Output,
        ZeroCrossing,
        Handler,
        Jacobian
      } Type;
    }
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
        Equation() {};
        Equation(AST_Expression eq, Util::VarSymbolTable& symbols, EQUATION::Type type);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, EQUATION::Type type);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, Range r, EQUATION::Type type);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, Option<Range> r, EQUATION::Type type);
        /**
         *
         */
        ~Equation() {};
        inline bool 
        hasRange() { return _range.is_initialized(); }; 
        inline AST_Expression 
        lhs() { return _lhs; };
        inline AST_Expression 
        rhs() { return _rhs; };
        inline AST_Expression
        equation() { return _exp; };
        inline bool
        autonomous() { return _autonomous; };
        inline Util::SymbolTable 
        calledFunctions() { return _calledFunctions; };
        friend std::ostream& operator<<(std::ostream& out, const Equation& e);
      private:
        AST_Equation    _eq;
        AST_Expression  _lhs;
        AST_Expression  _rhs;
        AST_Expression  _exp;
        Option<Range>   _range;
        bool            _autonomous;
        Util::VarSymbolTable _symbols;
        Util::SymbolTable    _calledFunctions;

    };

    typedef ModelTable<int,Equation> EquationTable;
  }
}
#endif  /* EQUATION_H_ */
