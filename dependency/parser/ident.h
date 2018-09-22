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


#ifndef IDENT_PARSER
#define IDENT_PARSER
#include <boost/spirit/include/qi.hpp>
#include <ast/expression.h>
namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {
    template <typename Iterator>
    struct IdentRule: qi::grammar<Iterator,Name()>
    {
      IdentRule(Iterator &it);
      qi::rule<Iterator,Name()> ident, keyword_ident;
    };
  }
}
  
 
#endif
