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


#ifndef MODIFICATION_PARSER
#define MODIFICATION_PARSER
#include <boost/spirit/include/qi.hpp>

#include <ast/modification.h>
#include <parser/skipper.h>
#include <parser/expression.h>

namespace qi = boost::spirit::qi;
using namespace Modelica::AST;
namespace Modelica {
  namespace Parser {

    template <typename Iterator>
    struct ModificationRule: qi::grammar<Iterator,Skipper<Iterator>,Modification()>
    {
      ModificationRule(Iterator &it);


      // Rules
      qi::rule<Iterator, Skipper<Iterator>, Modification()> modification;
      qi::rule<Iterator, Skipper<Iterator>, ModEq()> equal_mod;
      qi::rule<Iterator, Skipper<Iterator>, Declaration()> declaration, component_declaration1;
      qi::rule<Iterator, Skipper<Iterator>, ElRepl()> element_replaceable; 
      qi::rule<Iterator, Skipper<Iterator>, ElMod()> element_modification; 
      qi::rule<Iterator, Skipper<Iterator>, Component1()> component_clause1;
      qi::rule<Iterator, Skipper<Iterator>, ElRedecl()> element_redeclaration;
      qi::rule<Iterator, Skipper<Iterator>, ModAssign()> assign_mod;
      qi::rule<Iterator, Skipper<Iterator>, StringComment()> string_comment;
      qi::rule<Iterator, Skipper<Iterator>, Annotation()> annotation;
      qi::rule<Iterator, Skipper<Iterator>, ModClass()> class_mod;
      qi::rule<Iterator, Skipper<Iterator>, Comment()> comment;
      qi::rule<Iterator, Skipper<Iterator>, ClassPrefixes() > class_prefixes; 
      qi::rule<Iterator, Skipper<Iterator>, TypePrefixes()> type_prefix; 
      qi::rule<Iterator, Skipper<Iterator>, Enum()> enumeration_literal;
      qi::rule<Iterator, Skipper<Iterator>, EnumList()> enum_list;
      qi::rule<Iterator, Skipper<Iterator>, EnumSpec()> enum_spec;
      qi::rule<Iterator, Skipper<Iterator>, ClassModification()> class_modification;
      qi::rule<Iterator, Skipper<Iterator>, ShortClass()> short_class_definition;
      qi::rule<Iterator, Skipper<Iterator>, Argument()> argument;
      qi::rule<Iterator, Skipper<Iterator>, Constrained()> constraining_clause; 
      ExpressionRule<Iterator> expression;

      /* Operators tokens */
      qi::rule<Iterator> EQUAL, ASSIGN, PLUS, ANNOTATION, COMA, COLON, ENUMERATION, OPAREN, CPAREN, REPLACEABLE, CONSTRAINEDBY,
                         EACH, REDECLARE, FINAL;

      Iterator &it;
    };
  }
}
#endif
