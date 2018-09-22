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


#ifndef EXPRESSION_PARSER
#define EXPRESSION_PARSER
#include <boost/spirit/include/qi.hpp>

#include <ast/expression.h>
#include <parser/skipper.h>
#include <parser/ident.h>

namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {

   /* Modelica rules for floating point values */
   template <typename T>
   struct ModelicaRealNumberPolicy : qi::strict_ureal_policies<T>
   {
     static bool const expect_dot = true;
     static bool const allow_leading_dot = false;
     static bool const allow_trailing_dot = true;

     template <typename Iter, typename Attribute>
     static bool
     parse_nan(Iter& first, Iter const& last, Attribute& attr) {
       return false;
     }

     template <typename Iter, typename Attribute>
     static bool
     parse_inf(Iter& first, Iter const& last, Attribute& attr) {
       return false;
     }
   };


    template <typename Iterator>
    struct ExpressionRule: qi::grammar<Iterator,Skipper<Iterator>,Modelica::AST::Expression()>
    {
      ExpressionRule(Iterator &it);


      // Rules
      qi::rule<Iterator,Skipper<Iterator>,Expression()> expression;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> factor;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> term;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> arithmetic_expression;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> relation;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> logical_expression;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> logical_term;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> logical_factor;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> simple_expression;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> primary;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> subscript;
      qi::rule<Iterator,Skipper<Iterator>,Expression()> function_argument;
      qi::rule<Iterator, Skipper<Iterator>, Reference()> component_reference;
      qi::rule<Iterator, Skipper<Iterator>, Call()> call_exp;
      qi::rule<Iterator, Skipper<Iterator>, Output()> output_exp;
      qi::rule<Iterator, Skipper<Iterator>, IfExp()> if_expression;
      qi::rule<Iterator, Skipper<Iterator>, FunctionExp()> function_exp;
      qi::rule<Iterator, Skipper<Iterator>, Named()> named_argument;
      qi::rule<Iterator, Skipper<Iterator>, Name()> name;
      qi::rule<Iterator, Skipper<Iterator>, Brace()> brace_exp;
      qi::rule<Iterator, Skipper<Iterator>, Index()> for_index;
      qi::rule<Iterator, Skipper<Iterator>, Indexes()> for_indices;
      qi::rule<Iterator, Skipper<Iterator>, Bracket()> bracket_exp;
      qi::rule<Iterator, Skipper<Iterator>, OptExpList()> output_expression_list;
      qi::rule<Iterator, Skipper<Iterator>, ExpList()> 
          function_call_args, function_arguments, named_arguments, 
          expression_list, array_subscripts;

      /* Rules with no skip */
      qi::rule<Iterator, String()> string_;
      IdentRule<Iterator> ident;

      /* Modelica rules for floating point and integers values */
      qi::real_parser<double,ModelicaRealNumberPolicy<double> > modelica_double;
      qi::uint_parser<int> modelica_int;

      /* Operators tokens */
      qi::rule<Iterator> OPAREN, CPAREN, COLON, SEMICOLON, QUOTE,
                  OBRACKET, CBRACKET, COMA, OBRACE, CBRACE, EQUAL, ELSE, ELSEIF, IF, IN, FOR, THEN ;

      /* Keywords with value */
      qi::rule<Iterator, Name()> DOT, DER, INITIAL;

      /* Keywords tokens */
      qi::rule<Iterator> FUNCTION1;
      Iterator &it;
    };
  }
}
#endif
