/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef AST_TYPES_H_
#define AST_TYPES_H_

#include <iostream>
#include <list>
#include <set>
#include <string>

#include "../util/macros.h"

/**
 *
 */
#define MAKE_SPACE for (int __i=0;__i<depth;__i++) ret << " ";
/**
 *
 */
#define BEGIN_BLOCK depth+=2;
/**
 *
 */
#define END_BLOCK depth-=2;
/**
 *
 */
extern int depth;
/**
 *
 */
class MCC_Parser;

using namespace std;

/**
 *
 */
typedef std::set<string> PreSet_;
/**
 *
 */
typedef PreSet_ *PreSet;
/**
 *
 */
typedef bool AST_Boolean;
/**
 *
 */
typedef string *AST_String;
/**
 *
 */
typedef int AST_Integer;
/**
 *
 */
typedef double AST_Real;
/**
 *
 */
typedef string AST_Type;
/**
 *
 */
typedef int AST_TypePrefix;
/**
 *
 */
typedef int AST_ClassPrefix;

/**
 *
 */
DEFINE_TYPE(AST_Node);
/**
 *
 */
DEFINE_TYPE(AST_Argument);
/**
 *
 */
DEFINE_TYPE(AST_Argument_Modification);
/**
 *
 */
DEFINE_TYPE(AST_Argument_Redeclaration);
/**
 *
 */
DEFINE_TYPE(AST_Class);
/**
 *
 */
DEFINE_TYPE(AST_Comment);
/**
 *
 */
DEFINE_TYPE(AST_Element_Component);
/**
 *
 */
DEFINE_TYPE(AST_Composition);
/**
 *
 */
DEFINE_TYPE(AST_CompositionElement);
/**
 *
 */
DEFINE_TYPE(AST_CompositionEqsAlgs);
/**
 *
 */
DEFINE_TYPE(AST_Declaration);
/**
 *
 */
DEFINE_TYPE(AST_Element);
/**
 *
 */
DEFINE_TYPE(AST_Element_Component);
/**
 *
 */
DEFINE_TYPE(AST_Element_ClassWrapper);
/**
 *
 */
DEFINE_TYPE(AST_Equation);
/**
 *
 */
DEFINE_TYPE(AST_Equation_Call);
/**
 *
 */
DEFINE_TYPE(AST_Equation_Connect);
/**
 *
 */
DEFINE_TYPE(AST_Equation_Else);
/**
 *
 */
DEFINE_TYPE(AST_Equation_Equality);
/**
 *
 */
DEFINE_TYPE(AST_Equation_For);
/**
 *
 */
DEFINE_TYPE(AST_Equation_If);
/**
 *
 */
DEFINE_TYPE(AST_Equation_When);
/**
 *
 */
DEFINE_TYPE(AST_Expression);
/**
 *
 */
DEFINE_TYPE(AST_Expression_BinOp);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Boolean);
/**
 *
 */
DEFINE_TYPE(AST_Expression_BooleanNot);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Brace);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Call);
/**
 *
 */
DEFINE_TYPE(AST_Expression_CallArgs);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Colon);
/**
 *
 */
DEFINE_TYPE(AST_Expression_ComponentReference);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Derivative);
/**
 *
 */
DEFINE_TYPE(AST_Expression_End);
/**
 *
 */
DEFINE_TYPE(AST_Expression_If);
/**
 *
 */
DEFINE_TYPE(AST_Expression_If_ElseIf);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Integer);
/**
 *
 */
DEFINE_TYPE(AST_Expression_NamedArgument);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Null);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Output);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Range);
/**
 *
 */
DEFINE_TYPE(AST_Expression_Real);
/**
 *
 */
DEFINE_TYPE(AST_Expression_String);
/**
 *
 */
DEFINE_TYPE(AST_Expression_UMinus);
/**
 *
 */
DEFINE_TYPE(AST_Expression_ArrayIndex);
/**
 *
 */
DEFINE_TYPE(AST_External_Function_Call);
/**
 *
 */
DEFINE_TYPE(AST_Element_ExtendsClause);
/**
 *
 */
DEFINE_TYPE(AST_Element_ImportClause);
/**
 *
 */
DEFINE_TYPE(AST_ForIndex);
/**
 *
 */
DEFINE_TYPE(AST_ImportClause);
/**
 *
 */
DEFINE_TYPE(AST_Modification);
/**
 *
 */
DEFINE_TYPE(AST_Modification_Assign);
/**
 *
 */
DEFINE_TYPE(AST_Modification_Class);
/**
 *
 */
DEFINE_TYPE(AST_Modification_Equal);
/**
 *
 */
DEFINE_TYPE(AST_ShortClassExp);
/**
 *
 */
DEFINE_TYPE(AST_Statement);
/**
 *
 */
DEFINE_TYPE(AST_Statement_Break);
/**
 *
 */
DEFINE_TYPE(AST_Statement_Else);
/**
 *
 */
DEFINE_TYPE(AST_Statement_Return);
/**
 *
 */
DEFINE_TYPE(AST_Statement_When);
/**
 *
 */
DEFINE_TYPE(AST_Statement_While);
/**
 *
 */
DEFINE_TYPE(AST_Statement_For);
/**
 *
 */
DEFINE_TYPE(AST_Statement_If);
/**
 *
 */
DEFINE_TYPE(AST_Statement_Assign);
/**
 *
 */
DEFINE_TYPE(AST_StoredDefinition);
/**
 *
 */
DEFINE_LIST(AST_Argument);
/**
 *
 */
DEFINE_LIST(AST_Class);
/**
 *
 */
DEFINE_LIST(AST_Element_Component);
/**
 *
 */
DEFINE_LIST(AST_CompositionElement);
/**
 *
 */
DEFINE_LIST(AST_Declaration);
/**
 *
 */
DEFINE_LIST(AST_Element);
/**
 *
 */
DEFINE_LIST(AST_Equation);
/**
 *
 */
DEFINE_LIST(AST_Equation_Else);
/**
 *
 */
DEFINE_LIST(AST_Expression);
/**
 *
 */
DEFINE_LIST(AST_ExpressionList);
/**
 *
 */
DEFINE_LIST(AST_Expression_ComponentReference);
/**
 *
 */
DEFINE_LIST(AST_ForIndex);
/**
 *
 */
DEFINE_LIST(AST_Statement);
/**
 *
 */
DEFINE_LIST(AST_Statement_Else);
/**
 *
 */
DEFINE_LIST(AST_String);
/**
 *
 */
DEFINE_LIST(AST_Integer);

/**
 *
 */
enum BinOpType
{
  BINOPOR,        //!< BINOPOR
  BINOPAND,       //!< BINOPAND
  BINOPLOWER,     //!< BINOPLOWER
  BINOPLOWEREQ,   //!< BINOPLOWEREQ
  BINOPGREATER,   //!< BINOPGREATER
  BINOPGREATEREQ, //!< BINOPGREATEREQ
  BINOPCOMPNE,    //!< BINOPCOMPNE
  BINOPCOMPEQ,    //!< BINOPCOMPEQ
  BINOPADD,      //!< BINOPADD
  BINOPELADD,     //!< BINOPELADD
  BINOPSUB,       //!< BINOPSUB
  BINOPELSUB,     //!< BINOPELSUB
  BINOPDIV,       //!< BINOPDIV
  BINOPELDIV,     //!< BINOPELDIV
  BINOPMULT,      //!< BINOPMULT
  BINOPELMULT,    //!< BINOPELMULT
  BINOPEXP,       //!< BINOPEXP
  BINOPELEXP,     //!< BINOPELEXP
  BINOPNULL       //!< BINOPNULL
};

/**
 *
 */
enum ElementType
{
  ELNONE,    //!< ELNONE
  COMPONENT, //!< COMPONENT
  IMPORT,    //!< IMPORT
  EXTENDS,   //!< EXTENDS
  ELCLASS    //!< ELCLASS
};

/**
 *
 */
enum EquationType
{
  EQNONE,     //!< EQNONE
  EQEQUALITY, //!< EQEQUALITY
  EQCONNECT,  //!< EQCONNECT
  EQCALL,     //!< EQCALL
  EQFOR,      //!< EQFOR
  EQWHEN,     //!< EQWHEN
  EQIF        //!< EQIF
};

/**
 *
 */
enum ExpressionType
{
  EXPNONE,        //!< EXPNONE
  EXPCOMPREF,     //!< EXPCOMPREF
  EXPBINOP,       //!< EXPBINOP
  EXPDERIVATIVE,  //!< EXPDERIVATIVE
  EXPNULL,        //!< EXPNULL
  EXPEND,         //!< EXPEND
  EXPIF,          //!< EXPIF
  EXPCALLARG,     //!< EXPCALLARG
  EXPBRACE,       //!< EXPBRACE
  EXPCALL,        //!< EXPCALL
  EXPELSEIF,      //!< EXPELSEIF
  EXPCOLON,       //!< EXPCOLON
  EXPUMINUS,      //!< EXPUMINUS
  EXPBOOLEAN,     //!< EXPBOOLEAN
  EXPSTRING,      //!< EXPSTRING
  EXPREAL,        //!< EXPREAL
  EXPINTEGER,     //!< EXPINTEGER
  EXPBOOLEANNOT,  //!< EXPBOOLEANNOT
  EXPOUTPUT,      //!< EXPOUTPUT
  EXPRANGE,       //!< EXPRANGE
  EXPNAMEDARGUMENT,       //!< EXPNAMEDARGUMENT
  EXPARRAYINDEX       //!< EXPARRAYINDEX
};

/**
 *
 */
enum ModificationType
{
  MODNONE,  //!< MODNONE
  MODEQUAL, //!< MODEQUAL
  MODASSIGN, //!< MODASSIGN
  MODCLASS  //!< MODCLASS
};

/**
 *
 */
enum StatementType
{
  STNONE,     //!< STNONE
  STRETURN,   //!< STRETURN
  STBREAK,    //!< STBREAK
  STWHEN,     //!< STWHEN
  STASSING,   //!< STASSING
  STFOR,      //!< STFOR
  STIF,       //!< STIF
  STWHILE,    //!< STWHILE
  STOUTASSING //!< STOUTASSING
};

/**
 *
 */
enum TypePrefix
{
  TP_INPUT = (1 << 1), //!< TP_INPUT
  TP_OUTPUT = (1 << 2), //!< TP_OUTPUT
  TP_DISCRETE = (1 << 3), //!< TP_DISCRETE
  TP_PARAMETER = (1 << 4), //!< TP_PARAMETER
  TP_FLOW = (1 << 5), //!< TP_FLOW
  TP_STREAM = (1 << 6), //!< TP_STREAM
  TP_CONSTANT = (1 << 7), //!< TP_CONSTANT
  TP_FOR = (1 << 8)  //!< TP_FOR
};

/**
 *
 */
enum ArgumentType
{
  AR_MODIFICATION,  //!< AR_MODIFICATION
  AR_REDECLARATION  //!< AR_REDECLARATION
};
/**
 *
 * @return
 */
AST_TypePrefix
newAST_TypePrefix ();
/**
 *
 * @param
 * @return
 */
AST_TypePrefix
newAST_TypePrefix (AST_TypePrefix);
/**
 *
 * @param
 * @param
 * @return
 */
AST_TypePrefix
AST_TypePrefixAdd (AST_TypePrefix, AST_TypePrefix);
/**
 *
 */
enum ClassPrefix
{
  CP_PARTIAL = (1 << 1), //!< CP_PARTIAL
  CP_CLASS = (1 << 2), //!< CP_CLASS
  CP_MODEL = (1 << 3), //!< CP_MODEL
  CP_BLOCK = (1 << 4), //!< CP_BLOCK
  CP_RECORD = (1 << 5), //!< CP_RECORD
  CP_CONNECTOR = (1 << 6), //!< CP_CONNECTOR
  CP_TYPE = (1 << 7), //!< CP_TYPE
  CP_OPERATOR = (1 << 8), //!< CP_OPERATOR
  CP_EXPANDABLE = (1 << 9), //!< CP_EXPANDABLE
  CP_PURE = (1 << 10), //!< CP_PURE
  CP_IMPURE = (1 << 11), //!< CP_IMPURE
  CP_PACKAGE = (1 << 12), //!< CP_PACKAGE
  CP_FUNCTION = (1 << 13) //!< CP_FUNCTION
};

/**
 *
 * @param l
 * @param n
 * @return
 */
#define current_element(it) (*it)
/**
 *
 * @param l
 * @param n
 * @return
 */
#define foreach(it,list) for (it=list->begin();it!=list->end();it++)
/**
 *
 * @param l
 * @param n
 * @return
 */
#define foreachReverse(it,list) for (it=list->rbegin();it!=list->rend();it++)

/**
 *
 * @param l
 * @param n
 * @return
 */
template<typename T1>
  T1
  AST_ListElement (list<T1> *l, int n)
  {
    typename std::list<T1>::iterator it;
    it = l->begin ();
    for (int j = 1; j < n; j++)
      {
	it++;
      }
    return (*it);
  }

/**
 *
 * @param l
 * @param e
 * @return
 */
template<typename T1>
  list<T1> *
  AST_ListAppend (list<T1> *l, T1 e)
  {
    l->push_back (e);
    return (l);
  }

/**
 *
 * @param l
 * @param e
 * @return
 */
template<typename T1>
  list<T1> *
  AST_ListPrepend (list<T1> *l, T1 e)
  {
    l->push_front (e);
    return (l);
  }

/**
 *
 * @param l
 * @param e
 * @return
 */
template<typename T1>
  list<T1> *
  AST_ListRemove (list<T1> *l, T1 e)
  {
    l->remove (e);
    return (l);
  }

/**
 *
 * @param l
 * @return
 */
template<typename T1>
  T1
  AST_ListFirst (list<T1> *l)
  {
    return (l->front ());
  }

/**
 *
 * @param l1
 * @param l2
 * @return
 */
template<typename T1>
  list<T1> *
  AST_ListConcat (list<T1> *l1, list<T1> *l2)
  {
    l1->insert (l1->end (), l2->begin (), l2->end ());
    return (l1);
  }

/**
 *
 * @param l1
 * @return
 */
template<typename T1>
  list<T1> *
  AST_ListCopy (list<T1> *l1)
  {
    list<T1> * ls = new list<T1>;
    ls->assign (l1->begin (), l1->end ());
    return (ls);
  }

/**
 *
 * @param l1
 * @return
 */
template<typename T1>
  int
  AST_Length (list<T1> *l1)
  {
    return (l1->size ());
  }

/**
 *
 * @param l1
 * @param i
 * @return
 */
template<typename T1>
  T1
  AST_ListAt (list<T1> *l1, int i)
  {
    typename list<T1>::iterator it = l1->begin ();
    for (int c = 0; c < i; c++)
      {
	it++;
      }
    if (it != l1->end ())
      {
	return (*it);
      }
    return (T1 ());
  }

/**
 *
 * @param t
 * @return
 */
template<typename T1>
  list<T1> *
  newAST_SimpleList (T1 t)
  {
    list<T1> * ls = new list<T1>;
    ls->push_back (t);
    return (ls);
  }

/**
 *
 * @param l1
 * @param ret
 * @param sec_name
 * @param separator
 * @param opener
 * @param closer
 * @param block
 */
template<typename T1>
  void
  AST_ListPrint (list<T1> *l1, ostream &ret, string sec_name = "",
		 string separator = " ", string opener = "", string closer = "",
		 bool block = false)
  {
    typename list<T1>::iterator it;
    int size = l1->size (), i = 1;
    if (size)
      {
	ret << sec_name;
	if (block)
	  BEGIN_BLOCK
	;
	ret << opener;
	foreach(it,l1)
	  {
	    ret << current_element(it);
	    ret << (i < size ? separator : "");
	    i++;
	  }
	ret << closer;
	if (block)
	  END_BLOCK
	;
      }
  }

#endif  /* AST_TYPES_H_ */
