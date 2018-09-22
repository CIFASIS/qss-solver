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

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::StatementSection,
    (bool, initial_)
    (Modelica::AST::StatementList, statements_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::CallSt,
    (Modelica::AST::OptExpList, out_)
    (Modelica::AST::Expression, n_)
    (Modelica::AST::ExpList, arg_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::IfSt,
    (Modelica::AST::Expression, c)
    (Modelica::AST::StatementList, els)
    (Modelica::AST::IfSt::ElseList, elsesif)
    (Modelica::AST::StatementList, elses)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::ForSt,
    (Modelica::AST::Indexes, r)
    (Modelica::AST::StatementList, els)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::WhenSt,
    (Modelica::AST::Expression, c)
    (Modelica::AST::StatementList, els)
    (Modelica::AST::WhenSt::ElseList , elsew)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::WhileSt,
    (Modelica::AST::Expression, cond_)
    (Modelica::AST::StatementList, elements_)
)


namespace Modelica
{
  namespace Parser {

    struct ret_break_st_ : qi::symbols<char, Statement>
    {
      ret_break_st_() 
      {
        add
          ("return", Return())
          ("break", Break())
        ;
      }
    } ret_break_st;
 
    std::string at(std::string::const_iterator where, std::string::const_iterator start) ;
    template <typename Iterator>
    StatementRule<Iterator>::StatementRule(Iterator &it) : StatementRule::base_type(statement), it(it), modification(it), expression(it),
                                                OPAREN("("),CPAREN(")"), COMA(","), SEMICOLON(";"), ASSIGN(":="),
                                                WHEN("when"), THEN("then"), ELSEWHEN("elsewhen"), END("end"), WHILE("while"),
                                                FOR("for"), IF("if"), ELSEIF("elseif"), ELSE("else"), LOOP("loop"), 
                                                ALGORITHM("algorithm"), INITIAL("initial") {
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

      algorithm_section = 
                           matches[INITIAL] >> ALGORITHM >> statement_list
                        ;

      statement = 
                 (
                   assign_statement
                 | call_statement
                 | ret_break_st
                 | if_statement
                 | for_statement
                 | when_statement
                 | while_statement
                ) >> modification.comment
                ;

 
      if_statement = 
                     IF > expression > THEN > statement_list > *(ELSEIF > expression > THEN > statement_list) > -(ELSE > statement_list) > END > IF
                   ;

 
      assign_statement =
                         (expression.component_reference >> ASSIGN > expression) [_val=construct<Assign>(_1,_2)]
                       | (expression.component_reference >> expression.function_call_args) [_val=construct<Assign>(_1,_2)]
                       |  expression.component_reference [_val=construct<Assign>(_1)]
                       ;


      when_statement = 
                       WHEN > expression > THEN > statement_list > *(ELSEWHEN > expression > THEN > statement_list) > END > WHEN;
                     ;

      while_statement = 
                        WHILE > expression > LOOP > statement_list > END > WHILE
                      ;

 
 
      call_statement =
                          (OPAREN > expression.output_expression_list > CPAREN > ASSIGN > expression.component_reference > expression.function_call_args)
                      ;

      for_statement = 
                      FOR > expression.for_indices > LOOP > statement_list > END > FOR
                    ;


 
      statement_list =  
                        *(statement > SEMICOLON)
                      ;

 
      /* Error and debug */
      on_error<fail>
        (
            statement
          , std::cerr
                << val("Parser error. Expecting ")
                << _4                               // what failed?
                << construct<std::string>(_3, _2)
                << std::endl
        );
 
      algorithm_section.name("algorithm_section");
      for_statement.name("for statement");
      if_statement.name("if statement");
      statement_list.name("statement_list");
      statement.name("statement");
      when_statement.name("when_statement");
      while_statement.name("while_statement");
    }
  }
}
