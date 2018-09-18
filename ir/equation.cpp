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
#include "../util/ast_util.h"
#include "../util/util.h"

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    Equation::Equation(AST_Expression eq, VarSymbolTable& symbols, EQUATION::Type type, int id) :
      _eq(),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(symbols),
      _type(type), 
      _id(id)
    {
      process(eq);
    }

    Equation::Equation(AST_Expression eq, VarSymbolTable& symbols, Option<Range> range, EQUATION::Type type, int id) :
      _eq(),
      _lhs(),
      _rhs(),
      _range(range),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id)
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
      _id(id)
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
      _id(id)
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
      _id(id)
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
          block += TAB;
        }
      }
      else 
      {
        buffer << fp.beginExpression(functionId(), _range);
        buffer << fp.algebraics(dependencyMatrix(), _id);
      }
      buffer << block << prefix() << _lhs << " = " << _rhs << ";" << endl; 
      if(_type == EQUATION::ClassicDerivative)
      {
        if(_range) { buffer << endl << _range.get().end(); }
      }
      else 
      {
        buffer << fp.endExpression(_range);
      }
      return buffer.str();
    }

    string 
    Equation::macro() const 
    {
      stringstream buffer;
      buffer << functionId() << _id;
      FunctionPrinter fp;
      return fp.macro(buffer.str(), _range, _id);
    }

    std::ostream& operator<<(std::ostream& out, const Equation& e)
    {
      out << e.print();
      return out;
    }
  }
}
