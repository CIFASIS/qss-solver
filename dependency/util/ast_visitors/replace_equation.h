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

#ifndef AST_VISITOR_REPLACE_EQ
#define AST_VISITOR_REPLACE_EQ
#include <boost/variant/static_visitor.hpp>
#include <ast/equation.h>
#include <util/ast_visitors/replace_expression.h>

namespace Modelica {

  using namespace Modelica::AST;
  class ReplaceEquation: public boost::static_visitor<Equation> {
  public:
    ReplaceEquation(Expression, Expression);
    Equation operator()(Connect) const;
    Equation operator()(Equality) const;
    Equation operator()(CallEq) const;
    Equation operator()(ForEq) const;
    Equation operator()(IfEq) const;
    Equation operator()(WhenEq) const;
 
    Expression look,rep;
    ReplaceExpression replace_exp;
  }; 
}
#endif 
