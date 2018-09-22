/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef AST_VISITOR_PARTEVALEXP
#define AST_VISITOR_PARTEVALEXP
#include <boost/variant/static_visitor.hpp>
#include <ast/expression.h>
#include <util/table.h>

namespace Modelica {

  using namespace Modelica::AST;
  class PartialEvalExpression: public boost::static_visitor<Expression> {
  public:
    PartialEvalExpression(const VarSymbolTable &, bool eval=false);
    Expression operator()(Integer v) const;
    Expression operator()(Real v) const;
    Expression operator()(Boolean v) const;
    Expression operator()(String v) const;
    Expression operator()(Name v) const;
    Expression operator()(Expression v) const;
    Expression operator()(SubEnd v) const;
    Expression operator()(SubAll v) const;
    Expression operator()(BinOp) const;
    Expression operator()(UnaryOp) const;
    Expression operator()(Brace) const;
    Expression operator()(Bracket) const;
    Expression operator()(Call) const;
    Expression operator()(FunctionExp) const;
    Expression operator()(ForExp) const;
    Expression operator()(IfExp) const;
    Expression operator()(Named) const;
    Expression operator()(Output) const;
    Expression operator()(Reference) const;
    Expression operator()(Range) const;
    const VarSymbolTable &vtable; 
    bool eval_parameters;
  }; 
}
#endif 
