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

#ifndef AST_VISITOR_TO_MICRO
#define AST_VISITOR_TO_MICRO
#include <boost/variant/static_visitor.hpp>
#include <ast/expression.h>
#include <mmo/mmo_class.h>
#include <util/ast_visitors/to_micro/convert_to_micro_expression.h>

namespace Modelica {

  using namespace Modelica::AST;
  class ConvertToMicro: public boost::static_visitor<Equation> {
  public:
    ConvertToMicro(MMO_Class &cl);
    void convert();
    Equation operator() (Connect c) ;
    Equation operator() (Equality c) ;
    Equation operator() (CallEq c) ;
    Equation operator() (WhenEq c) ;
    Equation operator() (ForEq c) ;
    Equation operator() (IfEq c) ;
  private:
    unsigned int disc_count;
    MMO_Class & mmo_class;
    ConvertToMicroExpression tomicro_exp;
  }; 
}
#endif 
