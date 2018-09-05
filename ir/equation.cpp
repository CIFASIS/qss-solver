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

#include <sstream>

#include "../ast/expression.h"
#include "../ast/equation.h"
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
      process(eq);
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols)
    {
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(_rhs.expression());
      CalledFunctions cf;
      _calledFunctions = cf.apply(_rhs.expression());
      process(eq);
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Range r, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols)
    {
      process(eq);
    }
 
    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Option<Range> r, EQUATION::Type type) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols)
    {
      process(eq);
    }   

    void
    Equation::process(AST_Expression exp)
    {
      _rhs = Expression(exp, _symbols);
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(exp);
      CalledFunctions cf;
      _calledFunctions = cf.apply(exp);
    }

    void 
    Equation::process(AST_Equation eq)
    {
      AST_Equation_Equality eqe = eq->getAsEquality();
      if(eqe->left()->expressionType() == EXPDERIVATIVE)
      {
        _lhs = Expression(AST_ListFirst(eqe->left()->getAsDerivative()->arguments()), _symbols);
        _rhs = Expression(eqe->right(), _symbols);
      }
      else if(eqe->left()->expressionType() == EXPCOMPREF)
      {
        _lhs = Expression(eqe->left(), _symbols);
        _rhs = Expression(eqe->right(), _symbols);
      }
      else if(eqe->left()->expressionType() == EXPOUTPUT)
      {
        AST_Expression_Output eout = eqe->left()->getAsOutput();
        AST_ExpressionList el = eout->expressionList();
        AST_ExpressionListIterator it;
        foreach(it,el)
        {
          _lhs = Expression(eout, _symbols);
          _rhs = Expression(eqe->right(), _symbols);
        }
      }
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(_rhs.expression());
      CalledFunctions cf;
      _calledFunctions = cf.apply(_rhs.expression());
    }

    string 
    Equation::print() const
    {
      stringstream buffer;
      string block = "";
      if(_range) 
      { 
        buffer << _range.get(); 
        block += TAB;
      }
      buffer << block << _lhs << " = " << _rhs << ";"; 
      if(_range) { buffer << endl << _range.get().end(); }
      return buffer.str();
    }

    std::ostream& operator<<(std::ostream& out, const Equation& e)
    {
      out << e.print();
      return out;
    }
  }
}
