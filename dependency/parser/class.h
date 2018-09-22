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


#ifndef CLASS_PARSER
#define CLASS_PARSER
#include <boost/spirit/include/qi.hpp>

#include <ast/class.h>
#include <parser/skipper.h>
#include <parser/equation.h>
#include <parser/statement.h>

namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {

    template <typename Iterator>
    struct ClassRule: qi::grammar<Iterator,Skipper<Iterator>,StoredDef()>
    {
      ClassRule(Iterator &it);


      // Rules
      qi::rule<Iterator,Skipper<Iterator>,ClassType()> class_;
      ModificationRule<Iterator> modification;
      ExpressionRule<Iterator> expression;
      EquationRule<Iterator> equation;
      StatementRule<Iterator> statement;
      qi::rule<Iterator, Skipper<Iterator>, Import()> import_clause;
      qi::rule<Iterator, Skipper<Iterator>, Component()> component_clause;
      qi::rule<Iterator, Skipper<Iterator>, Extends()> extends_clause;
      qi::rule<Iterator, Skipper<Iterator>, Declaration()> component_declaration;
      qi::rule<Iterator, Skipper<Iterator>, Expression()> condition_attribute;
      qi::rule<Iterator, Skipper<Iterator>, DeclList()> component_list;
      qi::rule<Iterator, Skipper<Iterator>, Element()> element, element2; 
      qi::rule<Iterator, Skipper<Iterator>, ElemList()> element_list; 
      qi::rule<Iterator, Skipper<Iterator>, CompElement()> comp_element;
      qi::rule<Iterator, Skipper<Iterator>, External()> external_function_call;
      qi::rule<Iterator, Skipper<Iterator>, DefClass()> def_class;
      qi::rule<Iterator, Skipper<Iterator>, EnumClass()> enum_class;
      qi::rule<Iterator, Skipper<Iterator>, DerClass()> deriv_class;
      qi::rule<Iterator, Skipper<Iterator>, ExtendsClass()> extends_class;
      qi::rule<Iterator, Skipper<Iterator>, Class()> class_t;
      qi::rule<Iterator, Skipper<Iterator>, ClassList()> class_list; 
      qi::rule<Iterator, Skipper<Iterator>, Composition()> composition;
      qi::rule<Iterator, Skipper<Iterator>, ClassType()> class_specifier, class_definition, class_def;
      qi::rule<Iterator, Skipper<Iterator>, StoredDef()> stored_definition; 
 

      /* Operators tokens */
      qi::rule<Iterator> EQUAL, IMPORT, EXTENDS, DOT, STAR, OBRACE, COMA, CBRACE, IF, SEMICOLON, PUBLIC, PROTECTED, OPAREN, CPAREN,DER, ENUMERATION, EXTERNAL, END, ENCAPSULATED, FINAL, WITHIN, REPLACEABLE, REDECLARE, INNER, OUTER;

      /* Keywords tokens */
      Iterator &it;
    };
  }
}
#endif
