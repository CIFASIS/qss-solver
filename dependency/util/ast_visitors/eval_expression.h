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

#ifndef AST_VISITOR_EVALEXP
#define AST_VISITOR_EVALEXP
#include <boost/variant/static_visitor.hpp>
#include <ast/expression.h>
#include <util/table.h>

namespace Modelica {

  using namespace Modelica::AST;
  class EvalExpression: public boost::static_visitor<Real> {
  public:
    EvalExpression(const VarSymbolTable &);
    EvalExpression(const VarSymbolTable &,Name,Real);
    Real operator()(Integer v) const;
    Real operator()(Boolean v) const;
    Real operator()(String v) const;
    Real operator()(Name v) const;
    Real operator()(Real v) const;
    Real operator()(SubEnd v) const;
    Real operator()(SubAll v) const;
    Real operator()(BinOp) const;
    Real operator()(UnaryOp) const;
    Real operator()(Brace) const;
    Real operator()(Bracket) const;
    Real operator()(Call) const;
    Real operator()(FunctionExp) const;
    Real operator()(ForExp) const;
    Real operator()(IfExp) const;
    Real operator()(Named) const;
    Real operator()(Output) const;
    Real operator()(Reference) const;
    Real operator()(Range) const;
    const VarSymbolTable &vtable; 
    Option<Name> name;
    Option<Real> val;
  }; 
}
#endif 
