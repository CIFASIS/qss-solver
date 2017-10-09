#ifndef YY_MCC_Parser_h_included
#define YY_MCC_Parser_h_included
#define YY_USE_CLASS

#line 1 "/usr/share/bison++/bison.h"
/* before anything */
#ifdef c_plusplus
 #ifndef __cplusplus
  #define __cplusplus
 #endif
#endif


 #line 8 "/usr/share/bison++/bison.h"
#define YY_MCC_Parser_YY_MCC_Parser_DEBUG  
#define YY_MCC_Parser_CONSTRUCTOR_PARAM  bool debug
#define YY_MCC_Parser_CONSTRUCTOR_CODE  \
  parsing_subscript = false;\
  yydebug=debug;\
  lexer = new MCC_Lexer();\
  lexer->setParser(this);
#define YY_MCC_Parser_DESSTRUCTOR_CODE  \
  delete lexer;\
  lexer = NULL;
#line 17 "parser/mocc.y"

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
#define YY_MCC_Parser_LSP_NEEDED 
#define YY_MCC_Parser_MEMBERS  \
          virtual ~MCC_Parser() {} \
          int gettoken() { return lexer->yylex(); };   \
          int yyinput() { return lexer->yyinput(); };   \
          static int lineno() { if (lexer==NULL) return 0; return lexer->lineno(); };    \
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
#define YY_MCC_Parser_LEX_BODY  { return lexer->yylex();}
#define YY_MCC_Parser_ERROR_BODY  { cerr << "error encountered at line: "<<lexer->lineno()<<" last word parsed:"<<lexer->YYText()<<"\n";}

#line 61 "parser/mocc.y"
typedef union {
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
} yy_MCC_Parser_stype;
#define YY_MCC_Parser_STYPE yy_MCC_Parser_stype

#line 21 "/usr/share/bison++/bison.h"
 /* %{ and %header{ and %union, during decl */
#ifndef YY_MCC_Parser_COMPATIBILITY
 #ifndef YY_USE_CLASS
  #define  YY_MCC_Parser_COMPATIBILITY 1
 #else
  #define  YY_MCC_Parser_COMPATIBILITY 0
 #endif
#endif

#if YY_MCC_Parser_COMPATIBILITY != 0
/* backward compatibility */
 #ifdef YYLTYPE
  #ifndef YY_MCC_Parser_LTYPE
   #define YY_MCC_Parser_LTYPE YYLTYPE
/* WARNING obsolete !!! user defined YYLTYPE not reported into generated header */
/* use %define LTYPE */
  #endif
 #endif
/*#ifdef YYSTYPE*/
  #ifndef YY_MCC_Parser_STYPE
   #define YY_MCC_Parser_STYPE YYSTYPE
  /* WARNING obsolete !!! user defined YYSTYPE not reported into generated header */
   /* use %define STYPE */
  #endif
/*#endif*/
 #ifdef YYDEBUG
  #ifndef YY_MCC_Parser_DEBUG
   #define  YY_MCC_Parser_DEBUG YYDEBUG
   /* WARNING obsolete !!! user defined YYDEBUG not reported into generated header */
   /* use %define DEBUG */
  #endif
 #endif 
 /* use goto to be compatible */
 #ifndef YY_MCC_Parser_USE_GOTO
  #define YY_MCC_Parser_USE_GOTO 1
 #endif
#endif

/* use no goto to be clean in C++ */
#ifndef YY_MCC_Parser_USE_GOTO
 #define YY_MCC_Parser_USE_GOTO 0
#endif

#ifndef YY_MCC_Parser_PURE

 #line 65 "/usr/share/bison++/bison.h"

#line 65 "/usr/share/bison++/bison.h"
/* YY_MCC_Parser_PURE */
#endif


 #line 68 "/usr/share/bison++/bison.h"

#line 68 "/usr/share/bison++/bison.h"
/* prefix */

#ifndef YY_MCC_Parser_DEBUG

 #line 71 "/usr/share/bison++/bison.h"
#define YY_MCC_Parser_DEBUG 1

#line 71 "/usr/share/bison++/bison.h"
/* YY_MCC_Parser_DEBUG */
#endif

#ifndef YY_MCC_Parser_LSP_NEEDED

 #line 75 "/usr/share/bison++/bison.h"

#line 75 "/usr/share/bison++/bison.h"
 /* YY_MCC_Parser_LSP_NEEDED*/
#endif

/* DEFAULT LTYPE*/
#ifdef YY_MCC_Parser_LSP_NEEDED
 #ifndef YY_MCC_Parser_LTYPE
  #ifndef BISON_YYLTYPE_ISDECLARED
   #define BISON_YYLTYPE_ISDECLARED
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;
  #endif

  #define YY_MCC_Parser_LTYPE yyltype
 #endif
#endif

/* DEFAULT STYPE*/
#ifndef YY_MCC_Parser_STYPE
 #define YY_MCC_Parser_STYPE int
#endif

/* DEFAULT MISCELANEOUS */
#ifndef YY_MCC_Parser_PARSE
 #define YY_MCC_Parser_PARSE yyparse
#endif

#ifndef YY_MCC_Parser_LEX
 #define YY_MCC_Parser_LEX yylex
#endif

#ifndef YY_MCC_Parser_LVAL
 #define YY_MCC_Parser_LVAL yylval
#endif

#ifndef YY_MCC_Parser_LLOC
 #define YY_MCC_Parser_LLOC yylloc
#endif

#ifndef YY_MCC_Parser_CHAR
 #define YY_MCC_Parser_CHAR yychar
#endif

#ifndef YY_MCC_Parser_NERRS
 #define YY_MCC_Parser_NERRS yynerrs
#endif

#ifndef YY_MCC_Parser_DEBUG_FLAG
 #define YY_MCC_Parser_DEBUG_FLAG yydebug
#endif

#ifndef YY_MCC_Parser_ERROR
 #define YY_MCC_Parser_ERROR yyerror
#endif

#ifndef YY_MCC_Parser_PARSE_PARAM
 #ifndef __STDC__
  #ifndef __cplusplus
   #ifndef YY_USE_CLASS
    #define YY_MCC_Parser_PARSE_PARAM
    #ifndef YY_MCC_Parser_PARSE_PARAM_DEF
     #define YY_MCC_Parser_PARSE_PARAM_DEF
    #endif
   #endif
  #endif
 #endif
 #ifndef YY_MCC_Parser_PARSE_PARAM
  #define YY_MCC_Parser_PARSE_PARAM void
 #endif
#endif

/* TOKEN C */
#ifndef YY_USE_CLASS

 #ifndef YY_MCC_Parser_PURE
  #ifndef yylval
   extern YY_MCC_Parser_STYPE YY_MCC_Parser_LVAL;
  #else
   #if yylval != YY_MCC_Parser_LVAL
    extern YY_MCC_Parser_STYPE YY_MCC_Parser_LVAL;
   #else
    #warning "Namespace conflict, disabling some functionality (bison++ only)"
   #endif
  #endif
 #endif


 #line 169 "/usr/share/bison++/bison.h"
#define	TOKALGORITHM	258
#define	TOKAND	259
#define	TOKANNOTATION	260
#define	TOKBLOCK	261
#define	TOKBREAK	262
#define	TOKCLASS	263
#define	TOKCONNECT	264
#define	TOKCONNECTOR	265
#define	TOKCONSTANT	266
#define	TOKCONSTRAINEDBY	267
#define	TOKDER	268
#define	TOKDISCRETE	269
#define	TOKEACH	270
#define	TOKELSE	271
#define	TOKELSEIF	272
#define	TOKELSEWHEN	273
#define	TOKENCAPSULATED	274
#define	TOKEND	275
#define	TOKENUMERATION	276
#define	TOKEQUATION	277
#define	TOKEXPANDABLE	278
#define	TOKEXTENDS	279
#define	TOKEXTERNAL	280
#define	TOKFALSE	281
#define	TOKFINAL	282
#define	TOKFLOW	283
#define	TOKFOR	284
#define	TOKFUNCTION	285
#define	TOKIF	286
#define	TOKIMPORT	287
#define	TOKIMPURE	288
#define	TOKIN	289
#define	TOKINITIAL	290
#define	TOKINNER	291
#define	TOKINPUT	292
#define	TOKLOOP	293
#define	TOKMODEL	294
#define	TOKOPERATOR	295
#define	TOKOR	296
#define	TOKOUTER	297
#define	TOKOUTPUT	298
#define	TOKPACKAGE	299
#define	TOKPARAMETER	300
#define	TOKPARTIAL	301
#define	TOKPROTECTED	302
#define	TOKPUBLIC	303
#define	TOKPURE	304
#define	TOKRECORD	305
#define	TOKREDECLARE	306
#define	TOKREPLACEABLE	307
#define	TOKRETURN	308
#define	TOKSTREAM	309
#define	TOKTHEN	310
#define	TOKTRUE	311
#define	TOKTYPE	312
#define	TOKWHEN	313
#define	TOKWHILE	314
#define	TOKWITHIN	315
#define	TOKINITIALEQ	316
#define	TOKINITIALALG	317
#define	TOKENDSUB	318
#define	TOKSEMICOLON	319
#define	TOKCOMA	320
#define	TOKCARET	321
#define	TOKOPAREN	322
#define	TOKCPAREN	323
#define	TOKOBRACE	324
#define	TOKCBRACE	325
#define	TOKOBRACKET	326
#define	TOKCBRACKET	327
#define	TOKDOT	328
#define	TOKINT	329
#define	TOKFLOAT	330
#define	TOKSTRING	331
#define	TOKID	332
#define	TOKASSING	333
#define	TOKEQUAL	334
#define	TOKCOLON	335
#define	TOKNOT	336
#define	TOKLOWER	337
#define	TOKGREATER	338
#define	TOKLOWEREQ	339
#define	TOKGREATEREQ	340
#define	TOKCOMPEQ	341
#define	TOKCOMPNE	342
#define	TOKPLUS	343
#define	TOKDOTPLUS	344
#define	TOKMINUS	345
#define	TOKDOTMINUS	346
#define	TOKUMINUS	347
#define	TOKUPLUS	348
#define	TOKSLASH	349
#define	TOKDOTSLASH	350
#define	TOKSTAR	351
#define	TOKDOTSTAR	352
#define	TOKDOTCARET	353
#define	TOKFC	354
#define	TOKPARENS	355


#line 169 "/usr/share/bison++/bison.h"
 /* #defines token */
/* after #define tokens, before const tokens S5*/
#else
 #ifndef YY_MCC_Parser_CLASS
  #define YY_MCC_Parser_CLASS MCC_Parser
 #endif

 #ifndef YY_MCC_Parser_INHERIT
  #define YY_MCC_Parser_INHERIT
 #endif

 #ifndef YY_MCC_Parser_MEMBERS
  #define YY_MCC_Parser_MEMBERS 
 #endif

 #ifndef YY_MCC_Parser_LEX_BODY
  #define YY_MCC_Parser_LEX_BODY  
 #endif

 #ifndef YY_MCC_Parser_ERROR_BODY
  #define YY_MCC_Parser_ERROR_BODY  
 #endif

 #ifndef YY_MCC_Parser_CONSTRUCTOR_PARAM
  #define YY_MCC_Parser_CONSTRUCTOR_PARAM
 #endif
 /* choose between enum and const */
 #ifndef YY_MCC_Parser_USE_CONST_TOKEN
  #define YY_MCC_Parser_USE_CONST_TOKEN 0
  /* yes enum is more compatible with flex,  */
  /* so by default we use it */ 
 #endif
 #if YY_MCC_Parser_USE_CONST_TOKEN != 0
  #ifndef YY_MCC_Parser_ENUM_TOKEN
   #define YY_MCC_Parser_ENUM_TOKEN yy_MCC_Parser_enum_token
  #endif
 #endif

class YY_MCC_Parser_CLASS YY_MCC_Parser_INHERIT
{
public: 
 #if YY_MCC_Parser_USE_CONST_TOKEN != 0
  /* static const int token ... */
  
 #line 212 "/usr/share/bison++/bison.h"
static const int TOKALGORITHM;
static const int TOKAND;
static const int TOKANNOTATION;
static const int TOKBLOCK;
static const int TOKBREAK;
static const int TOKCLASS;
static const int TOKCONNECT;
static const int TOKCONNECTOR;
static const int TOKCONSTANT;
static const int TOKCONSTRAINEDBY;
static const int TOKDER;
static const int TOKDISCRETE;
static const int TOKEACH;
static const int TOKELSE;
static const int TOKELSEIF;
static const int TOKELSEWHEN;
static const int TOKENCAPSULATED;
static const int TOKEND;
static const int TOKENUMERATION;
static const int TOKEQUATION;
static const int TOKEXPANDABLE;
static const int TOKEXTENDS;
static const int TOKEXTERNAL;
static const int TOKFALSE;
static const int TOKFINAL;
static const int TOKFLOW;
static const int TOKFOR;
static const int TOKFUNCTION;
static const int TOKIF;
static const int TOKIMPORT;
static const int TOKIMPURE;
static const int TOKIN;
static const int TOKINITIAL;
static const int TOKINNER;
static const int TOKINPUT;
static const int TOKLOOP;
static const int TOKMODEL;
static const int TOKOPERATOR;
static const int TOKOR;
static const int TOKOUTER;
static const int TOKOUTPUT;
static const int TOKPACKAGE;
static const int TOKPARAMETER;
static const int TOKPARTIAL;
static const int TOKPROTECTED;
static const int TOKPUBLIC;
static const int TOKPURE;
static const int TOKRECORD;
static const int TOKREDECLARE;
static const int TOKREPLACEABLE;
static const int TOKRETURN;
static const int TOKSTREAM;
static const int TOKTHEN;
static const int TOKTRUE;
static const int TOKTYPE;
static const int TOKWHEN;
static const int TOKWHILE;
static const int TOKWITHIN;
static const int TOKINITIALEQ;
static const int TOKINITIALALG;
static const int TOKENDSUB;
static const int TOKSEMICOLON;
static const int TOKCOMA;
static const int TOKCARET;
static const int TOKOPAREN;
static const int TOKCPAREN;
static const int TOKOBRACE;
static const int TOKCBRACE;
static const int TOKOBRACKET;
static const int TOKCBRACKET;
static const int TOKDOT;
static const int TOKINT;
static const int TOKFLOAT;
static const int TOKSTRING;
static const int TOKID;
static const int TOKASSING;
static const int TOKEQUAL;
static const int TOKCOLON;
static const int TOKNOT;
static const int TOKLOWER;
static const int TOKGREATER;
static const int TOKLOWEREQ;
static const int TOKGREATEREQ;
static const int TOKCOMPEQ;
static const int TOKCOMPNE;
static const int TOKPLUS;
static const int TOKDOTPLUS;
static const int TOKMINUS;
static const int TOKDOTMINUS;
static const int TOKUMINUS;
static const int TOKUPLUS;
static const int TOKSLASH;
static const int TOKDOTSLASH;
static const int TOKSTAR;
static const int TOKDOTSTAR;
static const int TOKDOTCARET;
static const int TOKFC;
static const int TOKPARENS;


#line 212 "/usr/share/bison++/bison.h"
 /* decl const */
 #else
  enum YY_MCC_Parser_ENUM_TOKEN { YY_MCC_Parser_NULL_TOKEN=0
  
 #line 215 "/usr/share/bison++/bison.h"
	,TOKALGORITHM=258
	,TOKAND=259
	,TOKANNOTATION=260
	,TOKBLOCK=261
	,TOKBREAK=262
	,TOKCLASS=263
	,TOKCONNECT=264
	,TOKCONNECTOR=265
	,TOKCONSTANT=266
	,TOKCONSTRAINEDBY=267
	,TOKDER=268
	,TOKDISCRETE=269
	,TOKEACH=270
	,TOKELSE=271
	,TOKELSEIF=272
	,TOKELSEWHEN=273
	,TOKENCAPSULATED=274
	,TOKEND=275
	,TOKENUMERATION=276
	,TOKEQUATION=277
	,TOKEXPANDABLE=278
	,TOKEXTENDS=279
	,TOKEXTERNAL=280
	,TOKFALSE=281
	,TOKFINAL=282
	,TOKFLOW=283
	,TOKFOR=284
	,TOKFUNCTION=285
	,TOKIF=286
	,TOKIMPORT=287
	,TOKIMPURE=288
	,TOKIN=289
	,TOKINITIAL=290
	,TOKINNER=291
	,TOKINPUT=292
	,TOKLOOP=293
	,TOKMODEL=294
	,TOKOPERATOR=295
	,TOKOR=296
	,TOKOUTER=297
	,TOKOUTPUT=298
	,TOKPACKAGE=299
	,TOKPARAMETER=300
	,TOKPARTIAL=301
	,TOKPROTECTED=302
	,TOKPUBLIC=303
	,TOKPURE=304
	,TOKRECORD=305
	,TOKREDECLARE=306
	,TOKREPLACEABLE=307
	,TOKRETURN=308
	,TOKSTREAM=309
	,TOKTHEN=310
	,TOKTRUE=311
	,TOKTYPE=312
	,TOKWHEN=313
	,TOKWHILE=314
	,TOKWITHIN=315
	,TOKINITIALEQ=316
	,TOKINITIALALG=317
	,TOKENDSUB=318
	,TOKSEMICOLON=319
	,TOKCOMA=320
	,TOKCARET=321
	,TOKOPAREN=322
	,TOKCPAREN=323
	,TOKOBRACE=324
	,TOKCBRACE=325
	,TOKOBRACKET=326
	,TOKCBRACKET=327
	,TOKDOT=328
	,TOKINT=329
	,TOKFLOAT=330
	,TOKSTRING=331
	,TOKID=332
	,TOKASSING=333
	,TOKEQUAL=334
	,TOKCOLON=335
	,TOKNOT=336
	,TOKLOWER=337
	,TOKGREATER=338
	,TOKLOWEREQ=339
	,TOKGREATEREQ=340
	,TOKCOMPEQ=341
	,TOKCOMPNE=342
	,TOKPLUS=343
	,TOKDOTPLUS=344
	,TOKMINUS=345
	,TOKDOTMINUS=346
	,TOKUMINUS=347
	,TOKUPLUS=348
	,TOKSLASH=349
	,TOKDOTSLASH=350
	,TOKSTAR=351
	,TOKDOTSTAR=352
	,TOKDOTCARET=353
	,TOKFC=354
	,TOKPARENS=355


#line 215 "/usr/share/bison++/bison.h"
 /* enum token */
     }; /* end of enum declaration */
 #endif
public:
 int YY_MCC_Parser_PARSE(YY_MCC_Parser_PARSE_PARAM);
 virtual void YY_MCC_Parser_ERROR(char *msg) YY_MCC_Parser_ERROR_BODY;
 #ifdef YY_MCC_Parser_PURE
  #ifdef YY_MCC_Parser_LSP_NEEDED
   virtual int  YY_MCC_Parser_LEX(YY_MCC_Parser_STYPE *YY_MCC_Parser_LVAL,YY_MCC_Parser_LTYPE *YY_MCC_Parser_LLOC) YY_MCC_Parser_LEX_BODY;
  #else
   virtual int  YY_MCC_Parser_LEX(YY_MCC_Parser_STYPE *YY_MCC_Parser_LVAL) YY_MCC_Parser_LEX_BODY;
  #endif
 #else
  virtual int YY_MCC_Parser_LEX() YY_MCC_Parser_LEX_BODY;
  YY_MCC_Parser_STYPE YY_MCC_Parser_LVAL;
  #ifdef YY_MCC_Parser_LSP_NEEDED
   YY_MCC_Parser_LTYPE YY_MCC_Parser_LLOC;
  #endif
  int YY_MCC_Parser_NERRS;
  int YY_MCC_Parser_CHAR;
 #endif
 #if YY_MCC_Parser_DEBUG != 0
  public:
   int YY_MCC_Parser_DEBUG_FLAG;	/*  nonzero means print parse trace	*/
 #endif
public:
 YY_MCC_Parser_CLASS(YY_MCC_Parser_CONSTRUCTOR_PARAM);
public:
 YY_MCC_Parser_MEMBERS 
};
/* other declare folow */
#endif


#if YY_MCC_Parser_COMPATIBILITY != 0
 /* backward compatibility */
 /* Removed due to bison problems
 /#ifndef YYSTYPE
 / #define YYSTYPE YY_MCC_Parser_STYPE
 /#endif*/

 #ifndef YYLTYPE
  #define YYLTYPE YY_MCC_Parser_LTYPE
 #endif
 #ifndef YYDEBUG
  #ifdef YY_MCC_Parser_DEBUG 
   #define YYDEBUG YY_MCC_Parser_DEBUG
  #endif
 #endif

#endif
/* END */

 #line 267 "/usr/share/bison++/bison.h"
#endif
