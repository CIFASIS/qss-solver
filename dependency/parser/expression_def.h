
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

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Index,
    (Modelica::AST::Name, name_)
    (Modelica::AST::OptExp, exp_)
)


BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::FunctionExp,
    (Modelica::AST::Name, name_)
    (Modelica::AST::ExpList, args_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Named,
    (Modelica::AST::Name, name_)
    (Modelica::AST::Expression, exp_)
)

BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Bracket,
    (Modelica::AST::ExpListList, args_)
)



BOOST_FUSION_ADAPT_STRUCT(
    Modelica::AST::Call,
    (Modelica::AST::Name, name_)
    (Modelica::AST::ExpList, args_)
)



namespace Modelica
{
  namespace Parser {
  Expression range1(Expression a, Expression b) {
    return Range(a,b);
  }
  Expression range2(Expression a, Expression b) {
    Range r=boost::get<Range>(a);
    return Range(r.start(),r.end(),b);
  }
  ExpList create_for(Expression e, Indexes i) {
    return ExpList(1, ForExp(e,i));
  }
 
    struct or_op_ : qi::symbols<char, BinOpType>
    {
      or_op_()
      {
        add
          ("or", Or)
        ;
      }
    } or_op;

 
    struct and_op_ : qi::symbols<char, BinOpType>
    {
      and_op_()
      {
        add
          ("and", And)
        ;
      }
    } and_op;

 
    struct not_op_ : qi::symbols<char, UnaryOpType>
    {
      not_op_()
      {
        add
          ("not", Not)
        ;
      }
    } not_op;


    struct rel_op_ : qi::symbols<char, BinOpType>
    {
      rel_op_()
      {
        add
          ("<=", LowerEq)
          ("<", Lower) 
          (">=", GreaterEq)
          (">", Greater)
          ("==", CompEq)
          ("<>", CompNotEq)
        ;
      }
    } rel_op;


    struct unary_op_ : qi::symbols<char, UnaryOpType>
    {
      unary_op_()
      {
        add
          ("+", Plus)
          ("-", Minus)
        ;
      }
    } unary_op;
 
    struct add_op_ : qi::symbols<char, BinOpType>
    {
      add_op_()
      {
        add
          ("+",Add)
          ("-",Sub)
          (".+",ElAdd)
          (".-",ElSub)
        ;
      }
    } add_op;

 
    struct mul_op_ : qi::symbols<char, BinOpType>
    {
      mul_op_()
      {
        add
          ("*", Mult)
          ("/", Div)
          (".*", ElMult)
          ("./", ElDiv)
        ;
      }
    } mul_op;


    struct exp_op_ : qi::symbols<char, BinOpType>
    {
      exp_op_()
      {
        add
          (".^", ElExp)
          ("^", Exp)
        ;
      }
    } exp_op;
 
    struct end_exp_ : qi::symbols<char, Expression>
    {
      end_exp_() 
      {
        add
          ("end", SubEnd())
        ;
      }
    } end_exp;

 
    struct colon_exp_ : qi::symbols<char, Expression>
    {
      colon_exp_() 
      {
        add
          (":", SubAll())
        ;
      }
    } colon_exp;
 
    struct boolean_ : qi::symbols<char,Boolean>
    {
      boolean_()
      {
        add
            ("true",True)
            ("false",False)
        ;
      }
    } boolean;
  
    template <typename Iterator>
    ExpressionRule<Iterator>::ExpressionRule(Iterator &it) : ExpressionRule::base_type(expression), ident(it), QUOTE("\""), OPAREN("("),CPAREN(")"),
                                                      OBRACKET("["), CBRACKET("]"), OBRACE("{"), CBRACE("}"), COMA(","), it(it), DOT(qi::char_('.')), 
                                                      DER(qi::string("der")),  FOR("for"), INITIAL(qi::string("initial")), EQUAL("="), 
                                                      COLON(":"),SEMICOLON(";"),
                                                      FUNCTION1("function"), IF("if"), IN("in"), THEN("then"), ELSE("else"), ELSEIF("elseif") {
      using qi::char_;
      using qi::_1;
      using qi::_2;
      using qi::_3;
      using qi::_4;
      using qi::_val;
      using phoenix::construct;
      using phoenix::bind;
      using phoenix::val;
      using qi::fail;
      using qi::on_error;
      using qi::lexeme;

      expression = 
                   simple_expression 
                 | if_expression
                 ;

      if_expression = 
                      (IF > expression > THEN > expression > *(ELSEIF > expression > THEN > expression ) > ELSE > expression)
                      [_val=construct<IfExp>(_1, _2, _3, _4)]
                    ;


      simple_expression = 
                          logical_expression [_val=_1] >> (-(COLON > logical_expression [_val=bind(range1,_val,_1)])) 
                          >> (-(COLON > logical_expression [_val=bind(range2,_val,_1)])) 
                        ;


      logical_expression = 
                           logical_term [_val=_1] >> *(or_op > logical_term) [_val=construct<BinOp>(_val, _1, _2)]
                         ;

      logical_term = 
                     logical_factor [_val=_1] >> *(and_op > logical_factor) [_val=construct<BinOp>(_val, _1, _2)]
                   ;

 
      logical_factor =  
                       (not_op > relation) [_val=construct<UnaryOp>(_2,_1)]
                     | relation [_val=_1]
                     ;

 
      relation =  
                 arithmetic_expression [_val= _1] >> -(rel_op > arithmetic_expression) [_val=construct<BinOp>(_val, _1, _2)]
               ;

      arithmetic_expression = 
                              term [_val=_1] >> *(add_op > term) [_val=construct<BinOp>(_val, _1, _2)]
                            | (unary_op > term) [_val=construct<UnaryOp>(_2,_1)] >> *(add_op > term) [_val=construct<BinOp>(_val, _1, _2)]
                            ;
      term =
             factor [_val=_1] >> *(mul_op > factor) [_val=construct<BinOp>(_val, _1, _2)]
           ;

      factor =
               primary [_val=_1] >> -(exp_op > primary) [_val=construct<BinOp>(_val, _1, _2)]
             ;
    
      primary = 
                lexeme[modelica_double]  
              | lexeme[modelica_int] 
              | lexeme[string_] 
              | boolean
              | call_exp 
              | component_reference 
              | output_exp
              | bracket_exp
              | brace_exp
              | end_exp 
              ;

      brace_exp =
                  OBRACE > function_arguments > CBRACE
                ;



      bracket_exp =
                    OBRACKET > (expression_list % SEMICOLON)  > CBRACKET
                  ;

      expression_list = 
                        expression % COMA
                      ;


      output_exp =
                   OPAREN > output_expression_list > CPAREN
                 ;

      output_expression_list = 
                              (-expression) % COMA
                             ;

 
      name =
             (-DOT) >> ident [_val+=_1] >> *(DOT >> ident) [_val+=_1+_2]
           ;

      component_reference = ((-DOT) >> ident >> (-array_subscripts)) [_val=construct<Reference>(_1,_2,_3)]
       >> *(DOT >> ident >> (-array_subscripts)) [ _val = construct<Reference>(_val,_2,_3) ]
                          ;

      call_exp =
                 (name  | DER | INITIAL) >> function_call_args 
               ;

      function_call_args = 
                           OPAREN > (-function_arguments) > CPAREN 
                         ;

      // TODO: forexp is missing
      function_arguments = 
                           named_arguments [_val=_1]
                         | (function_argument [_val=bind(&consume_one<Expression>,_1)] >> (COMA > function_arguments [ _val = bind(&append_list<Expression>, _val, _1) ])) 
                         | ((function_argument >> FOR > for_indices) [_val = bind(&create_for, _1, _2)])   
                         | function_argument [_val=bind(&consume_one<Expression>,_1)] 
                         ;

 
      for_indices =
                     (for_index % COMA) [_val=construct<Indexes>(_1)]
                  ;

      for_index = 
                  ident >> -(IN > expression) 
                ;
 
 
      named_arguments =
                        named_argument % COMA
                      ;

      named_argument =  
                        ident >> EQUAL >> function_argument 
                     ;

      array_subscripts = 
                           OBRACKET > (subscript % COMA) > CBRACKET 
                       ;
 
      function_argument = 
                           function_exp
                        |  expression 
                        ;


      function_exp =
                     FUNCTION1 > name > OPAREN >> (-named_arguments) > CPAREN
                   ;
      subscript = 
                  colon_exp 
                | expression
                ;


      string_ = 
                (QUOTE >> *("\\\"" | (char_ - QUOTE)) > QUOTE) [_val=construct<String>(_1)]
              ;
         /* Error and debug */
      on_error<fail>
        (
            expression
          , std::cerr
                << val("Parser error. Expecting ")
                << _4                               // what failed?
                << construct<std::string>(_3, _2) 
                << std::endl
        );
 
      OPAREN.name("(");
      CPAREN.name(")");
      COLON.name(":");
      SEMICOLON.name(";");
      QUOTE.name("\""); 
      OBRACKET.name("[");
      CBRACKET.name("]");
      COMA.name(",");
      OBRACE.name("{");
      CBRACE.name("}");
      EQUAL.name("=");
      arithmetic_expression.name("arithmetic_expression");
      array_subscripts.name("array_subscripts");
      expression_list.name("expression list");
      expression.name("expression");
      factor.name("factor");
      for_index.name("for index");
      for_indices.name("for indices");
      function_argument.name("function_argument");
      function_arguments.name("function_arguments");
      function_call_args.name("function_call_args");
      ident.name("identifier");
      logical_expression.name("logical_expression");
      logical_factor.name("logical_factor");
      logical_term.name("logical_term");
      named_argument.name("named_argument");
      named_arguments.name("named_arguments");
      name.name("name");
      output_expression_list.name("output_expression_list");
      primary.name("primary");
      relation.name("relation");
      simple_expression.name("expression");
      string_.name("string");
      subscript.name("subscript");
      term.name("term");
 

    }
  }
}
