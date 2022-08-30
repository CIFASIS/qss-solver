%{
#include <FlexLexer.h>
#include <iostream>
#include <stdlib.h>

#include "config.h"
#include <parser/mocc_parser.h>

#ifdef MCC_Lexer_DEBUG
#define TOK(X) printf("TOK%s ",#X);return MCC_Parser::TOK##X;
#else
#define TOK(X) return MCC_Parser::TOK##X;
#endif

#define parser (dynamic_cast<MCC_Lexer*>(this)->parser())
#define YYText (dynamic_cast<MCC_Lexer*>(this)->YYText())

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;

%}

%option noyywrap yylineno

DIGIT    [0-9]
NUM	 {DIGIT}*
ID       [_a-zA-Z][_a-zA-Z0-9]*
IDNUM      [_a-zA-Z0-9][_a-zA-Z0-9]*
QUOTED [0-9a-zA-Z\-_#]* 

%x str
%x line_comment 
%x block_comment 
     
%%

'{QUOTED}' {
#ifdef MCC_Lexer_DEBUG
  printf("TOKID(%s) ",YYText);
#endif
  parser->yylval.string = new string(YYText); 
  return MCC_Parser::TOKID;
}


==              TOK(COMPEQ);
=               TOK(EQUAL);
; 	            TOK(SEMICOLON);
, 	            TOK(COMA);
\.\^            TOK(DOTCARET);
\^            	TOK(CARET);
\( 	            TOK(OPAREN);
\) 	            TOK(CPAREN);
\{ 	            TOK(OBRACE);
\}	            TOK(CBRACE);
\[ 	            TOK(OBRACKET);
\]	            TOK(CBRACKET);
\.\+            TOK(DOTPLUS);
\.-             TOK(DOTMINUS);
\.\*            TOK(DOTSTAR);
\.\/            TOK(DOTSLASH);
\. 	            TOK(DOT);
\+ 	            TOK(PLUS);
- 	            TOK(MINUS);
\* 	            TOK(STAR);
\/\*            BEGIN(block_comment);
\/\/ 	          BEGIN(line_comment);
\/ 	            TOK(SLASH);
\<\>            TOK(COMPNE);
\<=  	          TOK(LOWEREQ);
\<  	          TOK(LOWER);
\>=  	          TOK(GREATEREQ);
\>  	          TOK(GREATER);
:=  	          TOK(ASSING);
:  	            TOK(COLON);
\"	            string_buf_ptr=string_buf;memset(string_buf,'\0',MAX_STR_CONST);BEGIN(str);


initial[\t\ \n]*equation	TOK(INITIALEQ);
initial[\t\ \n]*algorithm TOK(INITIALALG);

algorithm		    TOK(ALGORITHM);
and				      TOK(AND);
annotation			TOK(ANNOTATION);	

block				    TOK(BLOCK);
break				    TOK(BREAK);
class				    TOK(CLASS);
connect				  TOK(CONNECT);
connector			  TOK(CONNECTOR);
constant			  TOK(CONSTANT);
constrainedby		TOK(CONSTRAINEDBY);
der				      TOK(DER);
discrete			  TOK(DISCRETE);
each				    TOK(EACH);
else			    	TOK(ELSE);
elseif				  TOK(ELSEIF);
elsewhen			  TOK(ELSEWHEN);
encapsulated		TOK(ENCAPSULATED);
end				      { if (parser->isParsingSubscript()) { TOK(ENDSUB) } else { TOK(END);} }
enumeration			TOK(ENUMERATION);
equation			  TOK(EQUATION);
expandable			TOK(EXPANDABLE);
extends				  TOK(EXTENDS);
external			  TOK(EXTERNAL);
false				    TOK(FALSE);
final				    TOK(FINAL);
flow				    TOK(FLOW);
for				      TOK(FOR);
function			  TOK(FUNCTION);
if				      TOK(IF);
import				  TOK(IMPORT);
impure				  TOK(IMPURE);
in				      TOK(IN);
inner				    TOK(INNER);
input				    TOK(INPUT);
loop				    TOK(LOOP);
model				    TOK(MODEL);
not				      TOK(NOT);
operator			  TOK(OPERATOR);
or				      TOK(OR);
outer				    TOK(OUTER);
output				  TOK(OUTPUT);
package				  TOK(PACKAGE);
parameter			  TOK(PARAMETER);
partial				  TOK(PARTIAL);
protected			  TOK(PROTECTED);
public				  TOK(PUBLIC);
pure				    TOK(PURE);
record				  TOK(RECORD);
redeclare			  TOK(REDECLARE);
replaceable			TOK(REPLACEABLE);
return				  TOK(RETURN);
stream				  TOK(STREAM);
then				    TOK(THEN);
true				    TOK(TRUE);
type				    TOK(TYPE);
when				    TOK(WHEN);
while				    TOK(WHILE);
within				  TOK(WITHIN);


{ID} {
#ifdef MCC_Lexer_DEBUG
  printf("TOKID(%s) ",YYText);
#endif
  parser->yylval.string = new string(YYText); 
  return MCC_Parser::TOKID;
}

{NUM} {
#ifdef MCC_Lexer_DEBUG
  printf("TOKINT(%d) ",atoi(yytext));
#endif
  parser->yylval.integer = atoi(yytext);
  return MCC_Parser::TOKINT;
}


([0-9]+|[0-9]+\.[0-9]*)([0-9]*[eE][-+]?[0-9]+)?  { 
#ifdef MCC_Lexer_DEBUG
  printf("TOKFLOAT(%s)",yytext);
#endif
  parser->yylval.real = atof(yytext);
  return MCC_Parser::TOKFLOAT;
}

\n  {
#ifdef MCC_Lexer_DEBUG
printf("\n");
#endif
}
[ \t\r] 


<str>[^\\\n\"]+ {
  char *yptr = yytext;
  while ( *yptr )
    *string_buf_ptr++ = *yptr++;
}

<str>\\n *string_buf_ptr++='\\'; *string_buf_ptr++='n';  // a quoted "\n"

<str>\\\" *string_buf_ptr++='"';
<str>\\\\ *string_buf_ptr++='\\';
<str>\\t  *string_buf_ptr++ = '\t';
<str>\\r  *string_buf_ptr++ = '\r';
<str>\\b  *string_buf_ptr++ = '\b';
<str>\\f  *string_buf_ptr++ = '\f';

<str>\" { 
  string_buf_ptr=0; 
#ifdef MCC_Lexer_DEBUG
  printf("TSTRING(%s) ",string_buf);
#endif
BEGIN(INITIAL); parser->yylval.string=new string(string_buf);return MCC_Parser::TOKSTRING;
}

<line_comment>[^\n]
<line_comment>\n BEGIN(INITIAL);


<block_comment>\*\/ BEGIN(INITIAL);
<block_comment>\n 
<block_comment>.
.    printf("Unrecognized token: '%s' at line %d\n",yytext,parser->lineno());
%% 

