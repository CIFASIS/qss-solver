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

#ifndef AST_VISITOR_REPLACE_ST
#define AST_VISITOR_REPLACE_ST
#include <boost/variant/static_visitor.hpp>
#include <ast/statement.h>
#include <util/ast_visitors/replace_expression.h>

namespace Modelica {

  using namespace Modelica::AST;
  class ReplaceStatement: public boost::static_visitor<Statement> {
  public:
    ReplaceStatement(Expression, Expression);
    Statement operator()(Assign) const;
    Statement operator()(Break) const;
    Statement operator()(Return) const;
    Statement operator()(CallSt) const;
    Statement operator()(IfSt) const;
    Statement operator()(WhenSt) const;
    Statement operator()(WhileSt) const;
    Statement operator()(ForSt) const;
 
    Expression look,rep;
    ReplaceExpression replace_exp;
  }; 
}
#endif 
