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


#ifndef STATEMENT_PARSER
#define STATEMENT_PARSER
#include <boost/spirit/include/qi.hpp>

#include <ast/statement.h>
#include <parser/skipper.h>
#include <parser/modification.h>

namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {

    template <typename Iterator>
    struct StatementRule: qi::grammar<Iterator,Skipper<Iterator>,Statement()>
    {
      StatementRule(Iterator &it);


      // Rules
      qi::rule<Iterator,Skipper<Iterator>,Statement()> statement;
      ModificationRule<Iterator> modification;
      ExpressionRule<Iterator> expression;
      qi::rule<Iterator, Skipper<Iterator>, Assign() > assign_statement;
      qi::rule<Iterator, Skipper<Iterator>, CallSt() > call_statement;
      qi::rule<Iterator, Skipper<Iterator>, ForSt() > for_statement;
      qi::rule<Iterator, Skipper<Iterator>, WhenSt() > when_statement;
      qi::rule<Iterator, Skipper<Iterator>, WhileSt() > while_statement;
      qi::rule<Iterator, Skipper<Iterator>, IfSt() > if_statement;
      qi::rule<Iterator, Skipper<Iterator>, StatementList() > statement_list;
      qi::rule<Iterator, Skipper<Iterator>, StatementSection() > algorithm_section;
 

      /* Operators tokens */
      qi::rule<Iterator> ASSIGN, CONNECT, OPAREN, COMA, CPAREN, WHEN, THEN, ELSEWHEN, END, SEMICOLON, IF, ELSEIF, ELSE, FOR, LOOP, ALGORITHM, INITIAL, WHILE;

      /* Keywords tokens */
      Iterator &it;
    };
  }
}
#endif
