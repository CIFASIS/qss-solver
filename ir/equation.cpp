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

#include "helpers.h"
#include "../ast/expression.h"
#include "../ast/equation.h"
#include "../util/visitors/called_functions.h"
#include "../util/visitors/autonomous.h"
#include "../util/util.h"

namespace MicroModelica {
  using namespace Util;
  using namespace Deps;
  namespace IR {

    Equation::Equation(AST_Expression eq, VarSymbolTable& symbols, EQUATION::Type type, int id, int offset) :
      _eq(),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols),
      _type(type), 
      _id(id), 
      _offset(offset)
    {
      process(eq);
    }

    Equation::Equation(AST_Expression eq, VarSymbolTable& symbols, Option<Range> range, EQUATION::Type type, int id, int offset) :
      _eq(),
      _lhs(),
      _rhs(),
      _range(range),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id),
      _offset(offset)
    {
      process(eq);
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, EQUATION::Type type, int id) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id),
      _offset(0)
    {
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(_rhs.expression());
      CalledFunctions cf;
      _calledFunctions = cf.apply(_rhs.expression());
      process(eq);
    }

    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Range r, EQUATION::Type type, int id) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id),
      _offset(0)
    {
      process(eq);
    }
 
    Equation::Equation(AST_Equation eq, VarSymbolTable& symbols, Option<Range> r, EQUATION::Type type, int id) :
      _eq(eq),
      _lhs(),
      _rhs(),
      _range(r),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id),
      _offset(0)
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
        _lhs = Expression(eout, _symbols);
        _rhs = Expression(eqe->right(), _symbols);
      }
      Autonomous autonomous(_symbols);
      _autonomous = autonomous.apply(_rhs.expression());
      CalledFunctions cf;
      _calledFunctions = cf.apply(_rhs.expression());
    }

    string 
    Equation::prefix() const
    {
      switch(_type)
      {
        case EQUATION::ClassicDerivative: return "_der"; 
        case EQUATION::QSSDerivative: return "_der"; 
        case EQUATION::ZeroCrossing: return "_zc"; 
        case EQUATION::Output: return "_out"; 
        default: return "";
      }
      return "";
    }

    EquationDependencyMatrix 
    Equation::dependencyMatrix() const
    {
      ModelDependencies deps = Utils::instance().dependencies(); 
      switch(_type)
      {
        case EQUATION::ClassicDerivative:  
        case EQUATION::QSSDerivative: return deps.DA(); 
        case EQUATION::ZeroCrossing: return deps.ZCA(); 
        case EQUATION::Output: return deps.OA(); 
        default: return EquationDependencyMatrix();
      }
      return EquationDependencyMatrix();
    }

    string 
    Equation::functionId() const 
    {
      stringstream buffer;
      switch(_type)
      {
        case EQUATION::QSSDerivative: buffer << "_der_eq_" << _id; break; 
        case EQUATION::Dependency: buffer << "_dep_eq_" << _id; break; 
        case EQUATION::ZeroCrossing: buffer << "_event_" << _id; break;  
        case EQUATION::Output: buffer << "_out_eq_" << _id; break;  
        default: return "";
      }
      return buffer.str();
    }

    string 
    Equation::print() const
    {
      stringstream buffer;
      string block = "";
      FunctionPrinter fp;
      if(_type == EQUATION::ClassicDerivative)
      {
        if(_range) 
        { 
          buffer << _range.get(); 
          block = _range->block();
        }
      } else {
        buffer << fp.beginExpression(functionId(), _range);
        buffer << fp.algebraics(dependencyMatrix(), _id);
        block += TAB;
        if (_range) {
          block = _range->block();
        } else {
          block += TAB;
        }
      }
      buffer << block << prefix() << _lhs << " = " << _rhs << ";";
      if (_type == EQUATION::ClassicDerivative) {
        if (_range) {
          buffer << endl << _range.get().end();
        }
      } else {
        buffer << endl << block << fp.endExpression(_range);
      }
      return buffer.str();
    }

    string 
    Equation::macro() const 
    {
      if(_type == EQUATION::ClassicDerivative) { return ""; }
      FunctionPrinter fp;
      return fp.macro(functionId(), _range, _id, _offset);
    }

    std::ostream& operator<<(std::ostream& out, const Equation& e)
    {
      out << e.print();
      return out;
    }
  }
}
