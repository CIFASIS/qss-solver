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


#ifndef EQUATION_PARSER
#define EQUATION_PARSER
#include <boost/spirit/include/qi.hpp>

#include <ast/equation.h>
#include <parser/skipper.h>
#include <parser/modification.h>

namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {

    template <typename Iterator>
    struct EquationRule: qi::grammar<Iterator,Skipper<Iterator>,Equation()>
    {
      EquationRule(Iterator &it);


      // Rules
      qi::rule<Iterator,Skipper<Iterator>,Equation()> equation;
      qi::rule<Iterator, Skipper<Iterator>, Equality()> equality_equation;
      qi::rule<Iterator, Skipper<Iterator>, Connect()> connect_equation;
      qi::rule<Iterator, Skipper<Iterator>, IfEq()> if_equation;
      qi::rule<Iterator, Skipper<Iterator>, ForEq()> for_equation;
      qi::rule<Iterator, Skipper<Iterator>, CallEq()> call_equation;
      qi::rule<Iterator, Skipper<Iterator>, WhenEq()> when_equation;
      qi::rule<Iterator, Skipper<Iterator>, EquationList() > equation_list;
      qi::rule<Iterator, Skipper<Iterator>, EquationSection()> equation_section;
      ModificationRule<Iterator> modification;
      ExpressionRule<Iterator> expression;

      /* Operators tokens */
      qi::rule<Iterator> EQUAL, CONNECT, OPAREN, COMA, CPAREN, WHEN, THEN, ELSEWHEN, END, SEMICOLON, IF, ELSEIF, ELSE, FOR, LOOP, EQUATION, INITIAL;

      /* Keywords tokens */
      Iterator &it;
    };
  }
}
#endif
