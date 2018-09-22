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
 * findstate.h
 *
 *  Created on: 28/04/2013
 *      Author: fede
 */

#include <ast/expression.h>
#include <mmo/mmo_class.h>
#include <util/table.h>
#include <boost/variant/static_visitor.hpp>

using namespace std;
using namespace Modelica;
using namespace Modelica::AST;

class StateVariablesFinder : public boost::static_visitor<void> {
public:
	StateVariablesFinder(Modelica::MMO_Class &c);
	void findStateVariables();
  void operator()(Modelica::AST::Integer v) const;
  void operator()(Boolean v) const;
  void operator()(String v) const;
  void operator()(Name v) const;
  void operator()(Real v) const;
  void operator()(SubEnd v) const;
  void operator()(SubAll v) const;
  void operator()(BinOp) const;
  void operator()(UnaryOp) const;
  void operator()(Brace) const;
  void operator()(Bracket) const;
  void operator()(Call) const;
  void operator()(FunctionExp) const;
  void operator()(ForExp) const;
  void operator()(IfExp) const;
  void operator()(Named) const;
  void operator()(Output) const;
  void operator()(Reference) const;
  void operator()(Range) const;

private:
	Modelica::MMO_Class &_c;
};

