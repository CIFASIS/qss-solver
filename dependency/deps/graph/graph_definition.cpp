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

/*
 *  Created on: 21 jul. 2016
 *      Author: Diego Hollmann
 */

#include <causalize/graph/graph_definition.h>
#include <ast/expression.h>
#include <boost/variant/get.hpp>
#include <util/debug.h>


namespace Causalize {

Unknown::Unknown(Modelica::AST::Expression exp): expression(exp) {}

Unknown::Unknown(VarInfo varInfo, Modelica::AST::Reference var) {
  if (varInfo.state()) {
    expression = Modelica::AST::Call("der",Modelica::AST::Reference(var));
  } else {
    expression = Modelica::AST::Reference(var);
  }
  if (!varInfo.indices()) {
    dimension = 0;
  } else {
    dimension = varInfo.indices().get().size();
  }
}

void Unknown::SetIndex(Modelica::AST::Expression index) {
  if (dimension!=0) {
    if (Modelica::AST::is<Modelica::AST::Call>(expression)) {
      get<Modelica::AST::Reference>(get<Modelica::AST::Call>(expression).args_.front()).ref_.front().get<1>()=Modelica::AST::ExpList(1,index);
    } else if (Modelica::AST::is<Modelica::AST::Reference>(expression)) {
      get<Modelica::AST::Reference>(expression).ref_.front().get<1>()=Modelica::AST::ExpList(1,index);
    } else {
      ERROR("Wrong unknown expression type");
    }
  }
}

Expression Unknown::operator() () const {
  return expression;
}

}
