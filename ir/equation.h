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
#include "../util/dependency_matrix.h"
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
        ClassicDerivative,
        QSSDerivative,
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
        Equation(AST_Expression eq, Util::VarSymbolTable& symbols, EQUATION::Type type, int id);
        Equation(AST_Expression eq, Util::VarSymbolTable& symbols, Option<Range> range, EQUATION::Type type, int id);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, EQUATION::Type type, int id);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, Range r, EQUATION::Type type, int id);
        Equation(AST_Equation eq, Util::VarSymbolTable& symbols, Option<Range> r, EQUATION::Type type, int id);
        /**
         *
         */
        ~Equation() {};
        inline bool 
        hasRange() { return _range.is_initialized(); }; 
        inline Expression 
        lhs() { return _lhs; };
        inline Expression 
        rhs() { return _rhs; };
        inline AST_Expression
        equation() { return _rhs.expression(); };
        inline bool
        autonomous() { return _autonomous; };
        inline Util::SymbolTable 
        calledFunctions() { return _calledFunctions; };
        friend std::ostream& operator<<(std::ostream& out, const Equation& e);
        std::string
        print() const;
        std::string 
        macro() const;
      private:
        void 
        process(AST_Equation eq);
        void 
        process(AST_Expression exp);
        Util::EquationDependencyMatrix 
        dependencyMatrix() const;
        std::string 
        functionId() const;
        std::string 
        prefix() const;
        AST_Equation         _eq;
        Expression           _lhs;
        Expression           _rhs;
        Option<Range>        _range;
        bool                 _autonomous;
        Util::VarSymbolTable _symbols;
        Util::SymbolTable    _calledFunctions;
        EQUATION::Type       _type;
        int                  _id;
    };

    typedef ModelTable<int,Equation> EquationTable;
  }
}
#endif  /* EQUATION_H_ */
