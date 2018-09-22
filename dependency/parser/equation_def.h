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


#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <string>

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::EquationSection,
    (bool, initial_)
    (Modelica::AST::EquationList, equations_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Equality,
    (Modelica::AST::Expression, left_)
    (Modelica::AST::Expression, right_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Connect,
    (Modelica::AST::Expression, left_)
    (Modelica::AST::Expression, right_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::CallEq,
    (Modelica::AST::Name, name_)
    (Modelica::AST::ExpList , args_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::IfEq,
    (Modelica::AST::Expression, c)
    (Modelica::AST::EquationList, els)
    (Modelica::AST::IfEq::ElseList, elsesif)
    (Modelica::AST::EquationList, elses)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::ForEq,
    (Modelica::AST::Indexes, r)
    (Modelica::AST::EquationList, els)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::WhenEq,
    (Modelica::AST::Expression, c)
    (Modelica::AST::EquationList, els)
    (Modelica::AST::WhenEq::ElseList , elsew)
)



namespace Modelica
{
  namespace Parser {

    std::string at(std::string::const_iterator where, std::string::const_iterator start) ;
    template <typename Iterator>
    EquationRule<Iterator>::EquationRule(Iterator &it) : EquationRule::base_type(equation), it(it), modification(it), expression(it),
                                                OPAREN("("),CPAREN(")"), COMA(","), CONNECT("connect"), SEMICOLON(";"), 
                                                WHEN("when"), THEN("then"), ELSEWHEN("elsewhen"), END("end"), EQUAL("="),
                                                FOR("for"), IF("if"), ELSEIF("elseif"), ELSE("else"), LOOP("loop"), 
                                                EQUATION("equation"), INITIAL("initial") {
      using qi::_1;
      using qi::_2;
      using qi::_3;
      using qi::_4;
      using qi::_val;
      using phoenix::bind;
      using phoenix::construct;
      using phoenix::val;
      using qi::fail;
      using qi::on_error;
      using qi::matches;

      equation_section =
                          matches[INITIAL] >> EQUATION >> equation_list
                       ;

 
      equation = (
                    equality_equation
                  | for_equation
                  | if_equation
                  | connect_equation
                  | when_equation
                  | call_equation
                  ) > modification.comment
                  ;

      equality_equation = 
                            (expression.simple_expression >> EQUAL > expression)
                        ;

      connect_equation = 
                         CONNECT > OPAREN > expression.component_reference > COMA > expression.component_reference > CPAREN
                       ;

      when_equation = 
                      WHEN > expression > THEN > equation_list > *(ELSEWHEN > expression > THEN > equation_list) > END > WHEN 
                    ;

      if_equation = 
                    IF > expression > THEN > equation_list > *(ELSEIF > expression > THEN > equation_list) > -(ELSE > equation_list) > END > IF
                  ;
 
      for_equation  = 
                      FOR > expression.for_indices > LOOP > equation_list > END > FOR
                    ;
   
      equation_list =
                      *(equation > SEMICOLON)
                     ;

      call_equation = 
                       expression.name >> expression.function_call_args
                    ;

 
      /* Error and debug */
      on_error<fail>
        (
            equation
          , std::cerr
                << val("Parser error. Expecting ")
                << _4                               // what failed?
                << construct<std::string>(_3, _2)
                << std::endl
        );
 
      equation_list.name("equation_list");

      equation.name("equation");
      equation_section.name("equation_section");

      for_equation.name("for equation");

      if_equation.name("if equation");
      when_equation.name("when_equation");
    }
  }
}
