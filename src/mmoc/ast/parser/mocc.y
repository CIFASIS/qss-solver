%name MCC_Parser

%define YY_MCC_Parser_DEBUG 

%define CONSTRUCTOR_PARAM bool debug
%define CONSTRUCTOR_CODE \
  parsing_subscript = false;\
  yydebug=debug;\
  lexer = new MCC_Lexer();\
  lexer->setParser(this);


%define DESSTRUCTOR_CODE \
  delete lexer;\
  lexer = nullptr;

%header{
#include <FlexLexer.h>
#include <string.h>
#include <ast/ast_builder.h>

class MCC_Lexer: public yyFlexLexer {
public:
  int yyinput() { return yyFlexLexer::yyinput(); };
  int lineno() { return yyFlexLexer::lineno(); };
  void setInput(std::istream* in) { yyin = in; };
  MCC_Parser *parser() { return _p; };
  void setParser (MCC_Parser *p) { _p= p; }
private:
  MCC_Parser *_p;
};
%}

%define LSP_NEEDED
%define MEMBERS \
          virtual ~MCC_Parser() {} \
          int gettoken() { return lexer->yylex(); };   \
          int yyinput() { return lexer->yyinput(); };   \
          static int lineno() { if (lexer==nullptr) return 0; return lexer->lineno(); };    \
          int parseFile(std::istream* in) { lexer->setInput(in); return yyparse(); };    \
          void setParsingSubscript() { parsing_subscript=true; } \
          void unsetParsingSubscript() { parsing_subscript=false; } \
          bool isParsingSubscript() { return parsing_subscript; } \
          AST_StoredDefinition root() { return _root; }\
          void setRoot(AST_StoredDefinition sd) { _root=sd; }\
          private: \
                static MCC_Lexer *lexer; \
                bool parsing_subscript;\
                AST_StoredDefinition _root;
%define LEX_BODY { return lexer->yylex();}
%define ERROR_BODY { cerr << "error encountered at line: "<<lexer->lineno()<<" last word parsed:"<<lexer->YYText()<<"\n";}

%token TOKALGORITHM TOKAND TOKANNOTATION /*TOKASSERT*/ TOKBLOCK TOKBREAK TOKCLASS TOKCONNECT TOKCONNECTOR TOKCONSTANT TOKCONSTRAINEDBY TOKDER TOKDISCRETE TOKEACH TOKELSE TOKELSEIF TOKELSEWHEN TOKENCAPSULATED TOKEND TOKENUMERATION TOKEQUATION TOKEXPANDABLE TOKEXTENDS TOKEXTERNAL TOKFALSE TOKFINAL TOKFLOW TOKFOR TOKFUNCTION TOKIF TOKIMPORT TOKIMPURE TOKIN TOKINITIAL TOKINNER TOKINPUT TOKLOOP TOKMODEL TOKOPERATOR TOKOR TOKOUTER TOKOUTPUT TOKPACKAGE TOKPARAMETER TOKPARTIAL TOKPROTECTED TOKPUBLIC TOKPURE TOKRECORD TOKREDECLARE TOKREPLACEABLE TOKRETURN TOKSTREAM TOKTHEN TOKTRUE TOKTYPE TOKWHEN TOKWHILE TOKWITHIN 

// Special tokens
%token TOKINITIALEQ TOKINITIALALG TOKENDSUB

// Symbols
%token TOKSEMICOLON TOKCOMA TOKCARET TOKOPAREN TOKCPAREN TOKOBRACE TOKCBRACE TOKOBRACKET TOKCBRACKET TOKDOT 

%union {
  AST_Boolean                           opt;
  AST_Real                              real;
  AST_Integer                           integer;
  AST_String                            string;

  AST_Argument                          argument;
  AST_ClassPrefix                       class_prefix;
  AST_Class                             model;
  AST_Comment                           comment;
  AST_Element_Component                         component;
  AST_Composition                       composition;
  AST_CompositionEqsAlgs                composition_eqs_algs;
  AST_CompositionElement                composition_element;
  AST_Declaration                       declaration;
  AST_Equation                          equation;
  AST_Expression                        expression;
  AST_Expression_ComponentReference     component_ref;
  AST_External_Function_Call            external_funciton_call;
  AST_Element                           element;
  AST_Element_ExtendsClause             extends_clause;
  AST_ForIndex                          for_index;
  AST_Element_ImportClause              import_clause;
  AST_Modification                      modification;
  AST_ShortClassExp                     short_class_exp;
  AST_Statement                         statement;
  AST_StoredDefinition                  stored_def;
  AST_TypePrefix                        type_prefix;

  AST_ArgumentList                      argument_list;
  AST_ClassList                         class_list;
  AST_CompositionElementList            composition_element_list;
  AST_Element_ComponentList                     component_list;
  AST_DeclarationList                   declaration_list;
  AST_ElementList                       element_list;
  AST_EquationList                      equation_list;
  AST_Equation_ElseList                 equation_elselist;
  AST_ExpressionList                    expression_list;
  AST_ExpressionListList                expression_list_list;
  AST_ForIndexList                      for_index_list;
  AST_StatementList                     statement_list;
  AST_Statement_ElseList                statement_elselist;
  AST_StringList                        string_list;
}


%token <integer>TOKINT 
%token <real>   TOKFLOAT 
%token <string> TOKSTRING TOKID 


%type <argument> argument element_modification_replaceable element_redeclaration element_redeclaration_1 element_redeclaration_2 short_class_definition  component_clause1 element_modification element_replaceable   
%type <argument_list> opt_argument_list argument_list class_modification opt_class_modification annotation opt_annotation_composition opt_annotation 
%type <class_list> class_definition_list
%type <class_prefix> class_prefix class_prefixes opt_pure_impure_operator 
%type <comment> comment
%type <component> component_clause 
%type <component_ref> component_reference component_reference_list more_cr 
%type <composition> composition composition_aux_1
%type <composition_eqs_algs> eq_alg_section_init 
%type <composition_element> composition_element
%type <composition_element_list> composition_list
%type <declaration> declaration component_declaration component_declaration1
%type <declaration_list> component_list
%type <element> element element_option element_option_1 
%type <element_list> element_list
%type <equation> equation connect_clause if_equation when_equation for_equation 
%type <equation_list> equation_list opt_else_eq
%type <equation_elselist> opt_elseif_eq opt_else_when
%type <expression> expression simple_expression primary opt_comp_call subscript function_argument opt_expression opt_condition_attribute opt_in named_argument opt_assing opt_equal_exp opt_external_function_call
%type <expression_list> subscript_list opt_array_subscripts array_subscripts expression_list function_call_args opt_function_args function_arguments opt_function_arguments output_expression_list output_expression_list_more opt_elseif_exp named_arguments opt_named_arguments opt_more_args expression_list_more
%type <expression_list_list> primary_exp_list 
%type <extends_clause> extends_clause
%type <external_funciton_call> opt_external_composition 
%type <for_index> for_index 
%type <for_index_list> for_indices opt_more_indexes 
%type <import_clause> import_clause
%type <model> class_definition_aux class_definition class_specifier
%type <modification> modification opt_modification
%type <opt> opt_expandable opt_encapsulated opt_final opt_operator opt_partial opt_redeclare opt_inner opt_outer opt_each
%type <short_class_exp> short_class_definition_exp
%type <statement> statement when_statement while_statement if_statement for_statement
%type <statement_list> statement_list opt_else_st 
%type <statement_elselist> opt_esleif_st opt_else_when_list
%type <stored_def> stored_definition input
%type <string> name opt_name type_specifier more_comp_call opt_language_specification opt_within_name enumeration_literal string_comment string_comment_no_empty opt_import
%type <string_list> enum_list enumeration_args more_ids import_list 
%type <type_prefix> type_prefix opt_input_output opt_disc_param_const opt_flow_stream base_prefix

%left TOKASSING
%left TOKEQUAL
%left TOKTHEN 
%left TOKELSE

%left TOKCOLON

%left TOKNOT 
%left TOKAND TOKOR 

%left TOKLOWER TOKGREATER TOKLOWEREQ TOKGREATEREQ TOKCOMPEQ TOKCOMPNE 

%left TOKPLUS TOKDOTPLUS TOKMINUS TOKDOTMINUS

%left TOKUMINUS TOKUPLUS

%left TOKSLASH TOKDOTSLASH TOKSTAR TOKDOTSTAR

%right TOKCARET
%right TOKDOTCARET
%left TOKFC
%left TOKPARENS


%% 

input: 
  stored_definition { this->setRoot($$); }
;

stored_definition: 
  opt_within_name
  class_definition_list { $$ = newAST_StoredDefinition($2,$1); }
;

opt_within_name:
    /* empty */                       { $$ = newAST_StringNull(); }
  | TOKWITHIN opt_name TOKSEMICOLON   { $$ = $2; }
;

opt_name: 
    /* empty */ { $$ = newAST_StringNull(); }
  | name        { $$ = $1; } 
;

name:
    TOKID             { $$ = newAST_String($1); }
  | TOKDOT TOKID      { $$ = newAST_DotString($2); }
  | name TOKDOT TOKID { $$ = AST_StringDotAppend($1,$3); }
;

class_definition_list:
    /* empty */                                 { $$ = newAST_ClassList(); }
  | class_definition_list class_definition_aux  { $$ = AST_ListAppend($1,$2); } 
;

class_definition_aux:
  opt_final class_definition TOKSEMICOLON { $$ = AST_Class_SetFinal($2,$1);} 
;

class_definition:
  opt_encapsulated class_prefix class_specifier { $$ = AST_Class_SetPrefixEncapsulated($3,$2,$1); }
;

opt_encapsulated:
    /* empty */     { $$ = false; }
  | TOKENCAPSULATED { $$ = true; }
;

class_prefix:
  opt_partial class_prefixes { $$ = AST_ClassPrefix_AddPartial($1,$2); }
;

opt_expandable:
    /* empty */     { $$ = false; }
  | TOKEXPANDABLE   { $$ = true; }
;

opt_pure_impure_operator:
    /* empty */             { $$ = AST_ClassPrefix_None(); }
  | TOKPURE opt_operator    { $$ = AST_ClassPrefix_Pure($2); }
  | TOKIMPURE opt_operator  { $$ = AST_ClassPrefix_Impure($2); }
;

opt_final:
    /* empty */  { $$ = false; }
  | TOKFINAL     { $$ = true; }
;

class_prefixes:
    TOKCLASS                                { $$ = AST_ClassPrefix_Class(); }
  | TOKMODEL                                { $$ = AST_ClassPrefix_Model(); }
  | opt_operator TOKRECORD                  { $$ = AST_ClassPrefix_Record($1); }
  | TOKBLOCK                                { $$ = AST_ClassPrefix_Block(); }
  | opt_expandable TOKCONNECTOR             { $$ = AST_ClassPrefix_Connector($1); }
  | TOKTYPE                                 { $$ = AST_ClassPrefix_Type(); }
  | TOKPACKAGE                              { $$ = AST_ClassPrefix_Package(); }
  | opt_pure_impure_operator TOKFUNCTION    { $$ = AST_ClassPrefix_Function($1); }
  | TOKOPERATOR                             { $$ = AST_ClassPrefix_Operator(); }
;

opt_operator:
    /* empty */ { $$ = false; } 
  | TOKOPERATOR { $$ = true; }
;

opt_partial:
    /* empty */ { $$ = false; }
  | TOKPARTIAL  { $$ = true; }
;

class_specifier:
    TOKID string_comment composition TOKEND TOKID                                           { $$ = newAST_Class($1,$3); }
  | TOKID TOKEQUAL base_prefix name opt_array_subscripts opt_class_modification comment     { $$ = newAST_ClassModification($1,$3,$4,$5,$6); }
  | TOKID TOKEQUAL TOKENUMERATION TOKOPAREN enumeration_args TOKCPAREN comment              { $$ = newAST_ClassEnumeration($1,$5); }
  | TOKID TOKEQUAL TOKDER TOKOPAREN name TOKCOMA TOKID more_ids TOKCPAREN comment           { $$ = newAST_ClassDerivation($1,$5,AST_ListPrepend($8,$7)); }
  | TOKEXTENDS TOKID opt_class_modification string_comment composition TOKEND TOKID         { $$ = newAST_ClassExtends($2,$5); }
;

more_ids:
    /* empty */             { $$ = newAST_StringList(); }
  | more_ids TOKCOMA TOKID  { $$ = AST_ListAppend($1,$3); }
;

composition: 
  composition_aux_1 opt_external_composition opt_annotation_composition { $$ = AST_Composition_SetExternalAnnotation($1,$2,$3); }
;

opt_external_composition:
    /* empty */                                                                                   { $$ = newAST_ExternalCall(); }
  	| TOKEXTERNAL opt_language_specification opt_external_function_call opt_annotation TOKSEMICOLON { $$ = newAST_ExternalCall($2,nullptr,$3,$4); } 
  	| TOKEXTERNAL opt_language_specification component_reference TOKEQUAL opt_external_function_call opt_annotation TOKSEMICOLON { $$ = newAST_ExternalCall($2,$3,$5,$6); } 
	;

opt_language_specification:
    /* empty */   { $$ = newAST_StringNull(); }
  | TOKSTRING     { $$ = newAST_String($1); }
;

opt_external_function_call:
    /* empty */ 								 {$$ = nullptr;}
  	| TOKID TOKOPAREN expression_list TOKCPAREN  {$$ = newAST_Expression_Call($1,nullptr,$3);}
  	| TOKID TOKOPAREN TOKCPAREN  {$$ = newAST_Expression_Call($1,nullptr,newAST_ExpressionList());}
	;

opt_annotation_composition:
    /* empty */             { $$ = newAST_ArgumentList(); }
  | annotation TOKSEMICOLON { $$ = $1; } 
;

composition_aux_1: 
    /* empty */                                                             { $$ = newAST_Composition(); }
  | element TOKSEMICOLON element_list                                       { $$ = newAST_Composition(AST_ListPrepend($3,$1),newAST_CompositionElementList()); }
  | element TOKSEMICOLON element_list composition_element composition_list  { $$ = newAST_Composition(AST_ListPrepend($3,$1), AST_ListPrepend($5,$4)); }
  | composition_element composition_list                                    { $$ = newAST_Composition(AST_ListPrepend($2,$1)); }
;

string_comment:
    /* empty */                       { $$ = newAST_StringNull(); }
  | TOKSTRING                                 { $$ = $1; }
  | TOKSTRING TOKPLUS string_comment_no_empty { /* $$ = AST_StringConcat($1,$3);*/ }
;

string_comment_no_empty:
    TOKSTRING                                 { $$ = $1; }
  | TOKSTRING TOKPLUS string_comment_no_empty { /* $$ = AST_StringConcat($1,$3); */}
;

element: 
    import_clause                                                                       { $$ = AST_ImportToElement($1); }
  | extends_clause                                                                      { $$ = AST_ExtendsToElement($1); }
  | opt_redeclare opt_final opt_inner opt_outer element_option opt_constraining_clause  { $$ = $5; }
;

import_clause:
  TOKIMPORT opt_import comment { $$ = newAST_ImportClause(newAST_String($2)); }
;

opt_import:
    TOKID TOKEQUAL name   {$$ = $3}
  | name opt_import_spec  {$$ = $1}
;

opt_import_spec:
    /* empty */
  | TOKDOTSTAR
  | TOKDOT TOKOBRACE import_list TOKCBRACE
;

import_list:
    TOKID                     { $$ = AST_ListAppend(newAST_StringList(),$1); }
  | TOKID TOKCOMA import_list { $$ = AST_ListPrepend($3,$1); }
;

comment:
  string_comment opt_annotation { $$ = newAST_Comment($1,$2); }
;

element_list:
    /* empty */                       { $$ = newAST_ElementList(); }
  | element TOKSEMICOLON element_list { $$ = AST_ListPrepend($3,$1);}
;

extends_clause:
  TOKEXTENDS name opt_class_modification opt_annotation { $$ = newAST_Element_ExtendsClause($2); }
;

opt_redeclare:
    /* empty */  { $$ = false; }
  | TOKREDECLARE { $$ = true; }
;

opt_inner:      
    /* empty */ { $$ = false; }
  | TOKINNER    { $$ = true; }
;

opt_outer:    
    /* empty */ { $$ = false; } 
  | TOKOUTER    { $$ = true; }
;

opt_constraining_clause:
    /* empty */
  | constraining_clause comment
;

constraining_clause:
  TOKCONSTRAINEDBY name opt_class_modification
;

element_option:
    element_option_1                { $$ = $1;} 
  | TOKREPLACEABLE element_option_1 { $$ = $2;} 
;

element_option_1:
    class_definition { $$ = newAST_Element_ClassWrapper($1); }
  | component_clause { $$ = AST_Element_ComponentToElement($1);}
;

component_clause:
  type_prefix type_specifier opt_array_subscripts component_list { $$ = newAST_Element_Component($4,$2,$1,$3); }
;

type_prefix:
    /* empty */                                             { $$ = newAST_TypePrefix(); }
 | opt_input_output                                         { $$ = $1; }
 | opt_disc_param_const                                     { $$ = $1; }  
 | opt_disc_param_const opt_input_output                    { $$ = AST_TypePrefixAdd($1,$2); }
 | opt_flow_stream                                          { $$ = $1; }  
 | opt_flow_stream opt_disc_param_const                     { $$ = AST_TypePrefixAdd($1,$2); } 
 | opt_flow_stream opt_disc_param_const opt_input_output    { $$ = AST_TypePrefixAdd($1,AST_TypePrefixAdd($2,$3)); }
;

opt_flow_stream:
    TOKFLOW       { $$ = newAST_TypePrefix(TP_FLOW); }
  | TOKSTREAM     { $$ = newAST_TypePrefix(TP_STREAM); }
;

opt_disc_param_const:
    TOKDISCRETE   { $$ = newAST_TypePrefix(TP_DISCRETE); }
  | TOKPARAMETER  { $$ = newAST_TypePrefix(TP_PARAMETER); }
  | TOKCONSTANT   { $$ = newAST_TypePrefix(TP_CONSTANT); }
;

opt_input_output:
    TOKINPUT    { $$ = newAST_TypePrefix(TP_INPUT); }
  | TOKOUTPUT   { $$ = newAST_TypePrefix(TP_OUTPUT); }
;
 
type_specifier:
  name { $$ = $1;}
;

opt_array_subscripts:
    /* empty */       { $$ = newAST_ExpressionList(); }
  | array_subscripts  { $$ = $1;}
;

component_list:
    component_declaration                        { $$ = newAST_DeclarationList($1); } 
  | component_list TOKCOMA component_declaration { $$ = AST_ListPrepend($1,$3); }
;

component_declaration:
  declaration opt_condition_attribute comment { $$ = AST_Declaration_AddCondComment($1,$2,$3); }
;

declaration:
  TOKID opt_array_subscripts opt_modification  { $$ = newAST_Declaration($1,$2,$3); }
;

opt_condition_attribute:
    /* empty */      { $$ = newAST_Expression_Null();}
  | TOKIF expression { $$ = $2; }
;

composition_list:
    /* empty */                             { $$ = newAST_CompositionElementList();}
  | composition_list composition_element    { $$ = AST_ListAppend($1,$2); }
;

composition_element:
    TOKPUBLIC element_list    { $$ = newAST_CompositionElement($2);}
  | TOKPROTECTED element_list { $$ = newAST_CompositionElement($2);}
  | eq_alg_section_init       { $$ = newAST_CompositionElement($1);}
;

opt_modification:
    /* empty */   { $$ = newAST_ModificationNull(); }
  | modification  { $$ = $1; }
;

modification:
    class_modification opt_equal_exp  { $$ = newAST_Modification_Class($1,$2); }
  | TOKEQUAL expression               { $$ = newAST_Modification_Equal($2); }
  | TOKASSING expression              { $$ = newAST_Modification_Assign($2); }
;

class_modification:
  TOKOPAREN opt_argument_list TOKCPAREN { $$ = $2; }
;

opt_argument_list:
    /* empty */   { $$ = newAST_ArgumentList(); }
  | argument_list { $$ = $1; }
;

argument_list:
    argument                        { $$ = AST_ListAppend(newAST_ArgumentList(),$1); }
  | argument_list TOKCOMA argument  { $$ = AST_ListAppend($1,$3); }
;

argument:
    element_modification_replaceable  { $$ = $1; }
  | element_redeclaration             { $$ = $1; }
;

opt_each:
    /* empty */  { $$ = false; }
  | TOKEACH      { $$ = true; }
;

element_modification_replaceable:
    opt_each opt_final element_modification  { $$ = AST_ArgumentSet($1,$2,$3); }
  | opt_each opt_final element_replaceable   { $$ = AST_ArgumentSet($1,$2,$3); }
;

element_modification :
  name opt_modification string_comment  { $$ = newAST_ElementModification($1,$2); }
;

element_replaceable: 
    TOKREPLACEABLE short_class_definition opt_constraining_clause { $$ = AST_ArgumentSetReplaceable($2 /*,$3*/); } 
  | TOKREPLACEABLE component_clause1 opt_constraining_clause      { $$ = AST_ArgumentSetReplaceable($2 /*,$3*/); } 
;

opt_equal_exp:
    /* empty */         { $$ = newAST_Expression_Null();}
  | TOKEQUAL expression { $$ = $2; }
;

element_redeclaration:
  TOKREDECLARE opt_each opt_final element_redeclaration_1 { $$ = newAST_Redeclaration($2,$3,$4); }
;

element_redeclaration_1:
   element_redeclaration_2 { $$ = $1; }
 | element_replaceable   { $$ = $1; }
;

element_redeclaration_2:
    short_class_definition  { $$ = $1; }
  | component_clause1       { $$ = $1; }
;
    
short_class_definition:
  class_prefixes TOKID TOKEQUAL short_class_definition_exp { $$ = newAST_ShortClass($1,$2,$4); }
;

short_class_definition_exp:
    base_prefix name opt_array_subscripts opt_class_modification comment  { $$ = newAST_ShortClassExp($1,$2,$3,$4); }
  | TOKENUMERATION TOKOPAREN enumeration_args TOKCPAREN comment           { $$ = newAST_ShortClassExp_Enum($3); }
;

enumeration_args:
    TOKSEMICOLON  { $$ = newAST_StringList(); } // The empty list means the semicolon
  | enum_list     { $$ = $1; }
;

enum_list:
    enumeration_literal                   { $$ = AST_ListAppend(newAST_StringList(),$1); }
  | enum_list TOKCOMA enumeration_literal { $$ = AST_ListAppend($1,$3); }
;

enumeration_literal:
  TOKID comment       { $$ = $1 ;}
;

component_clause1:
  type_prefix type_specifier component_declaration1 {}
;

component_declaration1:
  declaration comment { $$ = $1; }
;

opt_class_modification:
    /* empty */         { $$ = newAST_ArgumentList(); }
  | class_modification  { $$ = $1; }
;

base_prefix:
  type_prefix { $$ = $1; }
;

subscript_list:
    subscript                         { $$ = newAST_ExpressionList($1); }
  | subscript_list TOKCOMA subscript  { $$ = AST_ListAppend($1,$3); }
;

subscript:
    TOKCOLON    { $$ = newAST_Expression_Colon();}
  | expression  { $$ = $1; }
;

array_subscripts:
  TOKOBRACKET { setParsingSubscript(); }  subscript_list TOKCBRACKET { unsetParsingSubscript(); $$ = $3; }
;

opt_annotation:
    /* empty */         { $$ = newAST_ArgumentList(); }
  | annotation { $$ = $1; }
;

annotation:
  TOKANNOTATION class_modification { $$ = $2; } 
;

eq_alg_section_init:
    TOKINITIALEQ equation_list    { $$ = newAST_CompositionInitialEquations($2);}
  | TOKEQUATION equation_list     { $$ = newAST_CompositionEquations($2);}
  | TOKINITIALALG statement_list  { $$ = newAST_CompositionInitialAlgorithms($2); }
  | TOKALGORITHM statement_list   { $$ = newAST_CompositionAlgorithms($2); }
;

equation_list:
    /* empty */                         { $$ = newAST_EquationList(); }
  | equation_list equation TOKSEMICOLON { $$ = AST_ListAppend($1,$2); } 
;

equation: 
    connect_clause comment                        { $$ = $1; }
  | if_equation comment                           { $$ = $1; }
  | simple_expression TOKEQUAL expression comment { $$ = newAST_Equation_Equality($1,$3,$4); }
  | primary                                       { $$ = newAST_Equation_Call($1); } /* This must be a call only!!!!*/ 
  | for_equation comment                          { $$ = $1; }
  | when_equation                                 { $$ = $1; }
;

for_indices: 
  for_index opt_more_indexes { $$ = AST_ListPrepend($2,$1); }
;

opt_more_indexes:
    /* empty */                        { $$ = newAST_ForIndexList(); }
  | opt_more_indexes TOKCOMA for_index { $$ = AST_ListAppend($1,$3); }
;

for_index:
  TOKID opt_in  { $$ = newAST_ForIndex($1,$2); }
;

opt_in:
    /* empty */       { $$ = newAST_Expression_Null();} 
  | TOKIN expression  { $$ = $2; }
;

for_equation:
  TOKFOR for_indices TOKLOOP equation_list TOKEND TOKFOR { $$ = newAST_Equation_For($2,$4); }
;
  
when_equation:
  TOKWHEN expression TOKTHEN equation_list opt_else_when TOKEND TOKWHEN comment { $$ = newAST_Equation_When($2,$4,$5,$8); }

;

opt_else_when:
    /* empty */                                                 { $$ = newAST_Equation_ElseList(); }
  | opt_else_when TOKELSEWHEN expression TOKTHEN equation_list  { $$ = AST_ListAppend($1,newAST_Equation_Else($3,$5)); }
;

if_equation: 
    TOKIF expression TOKTHEN equation_list opt_elseif_eq opt_else_eq TOKEND TOKIF { $$ = newAST_Equation_If($2,$4,$5,$6); }
;

opt_elseif_eq:
    /* empty */                                               { $$ = newAST_Equation_ElseList(); }
  | opt_elseif_eq TOKELSEIF expression TOKTHEN equation_list  { $$ = AST_ListAppend($1,newAST_Equation_Else($3,$5)); }
;

opt_else_eq:
    /* empty */           { $$ = newAST_EquationList(); }
  | TOKELSE equation_list { $$ = $2; }
;

connect_clause:
  TOKCONNECT TOKOPAREN component_reference TOKCOMA component_reference TOKCPAREN { $$ = newAST_Equation_Connect($3,$5); }
;

statement_list:
    /* empty */                            { $$ = newAST_StatementList(); }
  | statement_list statement TOKSEMICOLON  { $$ = AST_ListAppend($1,$2); }
;

statement: 
    component_reference opt_assing comment           { $$ = newAST_Statement_Assign($1,$2);}
  | TOKOPAREN output_expression_list TOKCPAREN 
    TOKASSING component_reference function_call_args { $$ = newAST_Statement_OutputAssign($2,$5,$6); }
  | while_statement comment                          { $$ = $1; }
  | when_statement 				                           { $$ = $1; }
  | for_statement comment                            { $$ = $1; } 
  | if_statement comment                             { $$ = $1; }
  | TOKBREAK comment                                 { $$ = newAST_Statement_Break(); } 
  | TOKRETURN comment                                { $$ = newAST_Statement_Return(); }
;

while_statement:
  TOKWHILE expression TOKLOOP statement_list TOKEND TOKWHILE { $$ = newAST_Statement_While($2,$4); }
;

when_statement:
  TOKWHEN expression TOKTHEN statement_list opt_else_when_list TOKEND TOKWHEN comment { $$ = newAST_Statement_When($2,$4,$5,$8); }
;

opt_else_when_list:
    /* empty */                                                       { $$ = newAST_Statement_ElseList(); }
  | opt_else_when_list TOKELSEWHEN expression TOKTHEN statement_list  { $$ = AST_ListAppend($1,newAST_Statement_Else($3,$5)); }
;
  
for_statement:
  TOKFOR for_indices TOKLOOP statement_list TOKEND TOKFOR { $$ = newAST_Statement_For($2,$4); }
;

if_statement: 
  TOKIF expression TOKTHEN statement_list opt_esleif_st opt_else_st TOKEND TOKIF { $$ = newAST_Statement_If($2,$4,$5,$6); }
;

opt_esleif_st:
    /* empty */                                               { $$ = newAST_Statement_ElseList(); }
  | opt_esleif_st TOKELSEIF expression TOKTHEN statement_list { $$ = AST_ListAppend($1,newAST_Statement_Else($3,$5)); }
;

opt_else_st:
    /* empty */            { $$ = newAST_StatementList(); }
  | TOKELSE statement_list { $$ = $2; }
;

output_expression_list:
    /* empty */                             { $$ = newAST_ExpressionList(); }
  | expression output_expression_list_more  { $$ = AST_ListPrepend($2,$1); }
;

output_expression_list_more:
    /* empty */                                         { $$ = newAST_ExpressionList(); }          
  | TOKCOMA opt_expression output_expression_list_more  { $$ = AST_ListPrepend($3,$2); }
;

opt_named_arguments:
    /* empty */       { $$ = newAST_ExpressionList(); }          
  | named_arguments   { $$ = $1; }
;

named_arguments:
    named_argument opt_more_args { $$ = AST_ListPrepend($2,$1); }
;

named_argument:
  TOKID TOKEQUAL function_argument  { $$ = newAST_Expression_NamedArgument($1,$3); }
;

opt_more_args:
    /* empty */             { $$ = newAST_ExpressionList(); }          
  | TOKCOMA named_arguments { $$ = $2; }
;

opt_assing:
    TOKASSING expression  { $$ = $2; }
  | function_call_args    { $$= newAST_Expression_FunctionCallArgs($1);  }
;

function_argument:
    TOKFUNCTION name TOKOPAREN opt_named_arguments TOKCPAREN  { newAST_Expression_Function($2,$4); }
  | expression                                                { $$ = $1; }
;

function_call_args:
  TOKOPAREN opt_function_args TOKCPAREN { $$ = $2; } 
;

opt_function_args:
    /* empty */         { $$ = newAST_ExpressionList(); }
  | function_arguments  { $$ = $1; }
;

function_arguments: 
    function_argument opt_function_arguments { $$ = AST_ListPrepend($2,$1); } 
  | named_argument opt_function_arguments    { $$ = AST_ListPrepend($2,$1); }
;

opt_function_arguments:
    /* empty */                 { $$ = newAST_ExpressionList(); }
  | TOKCOMA function_arguments  { $$ = $2; }
  | TOKFOR for_indices          { $$ = newAST_ExpressionList(); /* TODO */ }
;

/************** Expressions *************************/
expression:  
    primary                                                     { $$ = $1; }
  | TOKOPAREN output_expression_list TOKCPAREN %prec TOKPARENS  { $$ = newAST_Expression_OutputExpressions($2); } 
  | TOKMINUS expression %prec TOKUMINUS                         { $$ = newAST_Expression_UnaryMinus($2); }
  | TOKPLUS expression %prec TOKUPLUS                           { $$ = $2; }
  | TOKNOT expression                                           { $$ = newAST_Expression_BooleanNot($2);}
  | expression TOKCOLON expression                              { $$ = newAST_Expression_Range($1,$3); }
  | expression TOKLOWER expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPLOWER); }
  | expression TOKLOWEREQ expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPLOWEREQ); } 
  | expression TOKGREATER expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPGREATER); }
  | expression TOKGREATEREQ expression                          { $$ = newAST_Expression_BinOp($1,$3,BINOPGREATEREQ); }
  | expression TOKCOMPNE expression                             { $$ = newAST_Expression_BinOp($1,$3,BINOPCOMPNE); }
  | expression TOKCOMPEQ expression                             { $$ = newAST_Expression_BinOp($1,$3,BINOPCOMPEQ); }
  | expression TOKSLASH expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPDIV); }
  | expression TOKDOTSLASH expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELDIV); }
  | expression TOKSTAR expression                               { $$ = newAST_Expression_BinOp($1,$3,BINOPMULT); }
  | expression TOKDOTSTAR expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPELMULT); }
  | expression TOKPLUS expression                               { $$ = newAST_Expression_BinOp($1,$3,BINOPADD); }
  | expression TOKMINUS expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPSUB); }
  | expression TOKDOTPLUS expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPELADD); }
  | expression TOKDOTMINUS expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELSUB); }
  | expression TOKCARET expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPEXP); }
  | expression TOKDOTCARET expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELEXP); }
  | expression TOKAND expression                                { $$ = newAST_Expression_BinOp($1,$3,BINOPAND); }
  | expression TOKOR expression                                 { $$ = newAST_Expression_BinOp($1,$3,BINOPOR); }
  | TOKIF expression TOKTHEN expression    
    opt_elseif_exp TOKELSE expression                           { $$ = newAST_Expression_If($2,$4,$5,$7); }
;

opt_expression:
    /* empty */ { $$ = newAST_Expression_Null();}
  | expression  { $$ = $1;}
;

opt_elseif_exp: 
    /* empty */                                            { $$ = newAST_ExpressionList(); }
  | opt_elseif_exp TOKELSEIF expression TOKTHEN expression { $$ = AST_ListAppend($1, newAST_Expression_ElseIf($3,$5)); }
;

primary: 
    TOKINT                                                        { $$ = newAST_Expression_Integer($1); }
  | TOKFLOAT                                                      { $$ = newAST_Expression_Real($1); }
  | TOKSTRING                                                     { $$ = newAST_Expression_String($1); }
  | TOKFALSE                                                      { $$ = newAST_Expression_Boolean(false); } 
  | TOKTRUE                                                       { $$ = newAST_Expression_Boolean(true); }
  | TOKDER function_call_args                                     { $$ = newAST_Expression_Derivative($2); }
  | TOKINITIAL function_call_args                                 { $$ = newAST_Expression_Derivative($2); }
  | opt_comp_call                                                 { $$ = $1; }
  | TOKOBRACKET expression_list primary_exp_list TOKCBRACKET      { $$ = newAST_BracketExpList(AST_ListPrepend($3,$2)); }
  | TOKOBRACE function_arguments TOKCBRACE                        { $$ = newAST_Expression_Brace($2); }
  | TOKENDSUB                                                     { $$ = newAST_Expression_End(); }
;

primary_exp_list:
    /* empty */                                     { $$ = newAST_ExpressionListList(); }
  | primary_exp_list TOKSEMICOLON expression_list   { $$ = AST_ListAppend($1,$3); }
;


component_reference:
    TOKID opt_array_subscripts component_reference_list         { $$ = AST_Expression_ComponentReference_Add($3,$1,$2);} 
  | TOKDOT TOKID opt_array_subscripts component_reference_list  { $$ = AST_Expression_ComponentReference_AddDot($4,$2,$3); } 
;

expression_list_more:
    /* empty */              { $$ = newAST_ExpressionList(); }
  | TOKCOMA expression_list  { $$ = $2; }
;

expression_list:
  expression expression_list_more   { $$ = AST_ListPrepend($2,$1);   }
;

component_reference_list:
    /* empty */                                                 { $$ = newAST_Expression_ComponentReference(); }
  | component_reference_list TOKDOT TOKID opt_array_subscripts  { $$ = AST_Expression_ComponentReference_Add($1,$3,$4); }
;


simple_expression: 
    primary                                                            { $$ = $1; }
  | TOKOPAREN output_expression_list TOKCPAREN %prec TOKPARENS         { $$ = newAST_Expression_OutputExpressions($2); } 
  | TOKMINUS expression %prec TOKUMINUS                                { $$ = newAST_Expression_UnaryMinus($2); }         
  | TOKPLUS expression %prec TOKUPLUS                                  { $$ = $2; }
  | TOKNOT expression                                                  { $$ = newAST_Expression_BooleanNot($2);}
  | simple_expression TOKCOLON expression                              { $$ = newAST_Expression_Range($1,$3); }
  | simple_expression TOKLOWER expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPLOWER); }
  | simple_expression TOKLOWEREQ expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPLOWEREQ); } 
  | simple_expression TOKGREATER expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPGREATER); }
  | simple_expression TOKGREATEREQ expression                          { $$ = newAST_Expression_BinOp($1,$3,BINOPGREATEREQ); }
  | simple_expression TOKCOMPNE expression                             { $$ = newAST_Expression_BinOp($1,$3,BINOPCOMPNE); }
  | simple_expression TOKCOMPEQ expression                             { $$ = newAST_Expression_BinOp($1,$3,BINOPCOMPEQ); }
  | simple_expression TOKSLASH expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPDIV); }
  | simple_expression TOKDOTSLASH expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELDIV); }
  | simple_expression TOKSTAR expression                               { $$ = newAST_Expression_BinOp($1,$3,BINOPMULT); }
  | simple_expression TOKDOTSTAR expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPELMULT); }
  | simple_expression TOKPLUS expression                               { $$ = newAST_Expression_BinOp($1,$3,BINOPADD); }
  | simple_expression TOKMINUS expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPSUB); }
  | simple_expression TOKDOTPLUS expression                            { $$ = newAST_Expression_BinOp($1,$3,BINOPELADD); }
  | simple_expression TOKDOTMINUS expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELSUB); }
  | simple_expression TOKCARET expression                              { $$ = newAST_Expression_BinOp($1,$3,BINOPEXP); }
  | simple_expression TOKDOTCARET expression                           { $$ = newAST_Expression_BinOp($1,$3,BINOPELEXP); }
  | simple_expression TOKAND expression                                { $$ = newAST_Expression_BinOp($1,$3,BINOPAND); }
  | simple_expression TOKOR expression                                 { $$ = newAST_Expression_BinOp($1,$3,BINOPOR); }
;

more_cr:
    /* empty */                                     { $$ = newAST_Expression_ComponentReference(); }
  | more_cr TOKDOT TOKID opt_array_subscripts       { $$ = AST_Expression_ComponentReference_Add($1,$3,$4); }
;

more_comp_call:
    /* empty */                   { $$ = newAST_StringNull(); }
  | more_comp_call TOKDOT TOKID   { $$ = AST_StringDotAppend($1,$3); }
;

opt_comp_call:
    TOKDOT TOKID more_comp_call                             { $$ = newAST_Expression_ComponentReferenceExpDot($2,$3); }
  | TOKID more_comp_call                                    { $$ = newAST_Expression_ComponentReferenceExp($1,$2); }
  | TOKDOT TOKID more_comp_call array_subscripts more_cr    { $$ = AST_Expression_ComponentReferenceExpAddDot($5,$2,$3,$4);}
  | TOKID more_comp_call array_subscripts more_cr           { $$ = AST_Expression_ComponentReferenceExpAdd($4,$1,$2,$3);}
  | TOKDOT TOKID more_comp_call function_call_args          { $$ = newAST_Expression_DotCall($2,$3,$4); } 
  | TOKID more_comp_call function_call_args  %prec TOKFC    { $$ = newAST_Expression_Call($1,$2,$3); }
; 
