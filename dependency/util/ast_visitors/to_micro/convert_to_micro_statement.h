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

#ifndef AST_VISITOR_TO_MICRO_ST
#define AST_VISITOR_TO_MICRO_ST
#include <boost/variant/static_visitor.hpp>
#include <ast/statement.h>
#include <mmo/mmo_class.h>

namespace Modelica {

  using namespace Modelica::AST;
  class ConvertToMicroStatement: public boost::static_visitor<Statement> {
  public:
    ConvertToMicroStatement(MMO_Class &cl, unsigned int &discont);
    Statement operator()(Assign v) const ;
    Statement operator()(Break v) const ;
    Statement operator()(Return v) const ;
    Statement operator()(CallSt v) const ;
    Statement operator()(IfSt v) const ;
    Statement operator()(ForSt v) const ;
    Statement operator()(WhenSt v) const ;
    Statement operator()(WhileSt v) const ;
    StatementList statements() const ;
  private:
    StatementList statements_;
    Expression newDiscrete(Option<Expression> s=Option<Expression>()) const;
    MMO_Class & mmo_class;
    unsigned int &disc_count;
  }; 
}
#endif 
