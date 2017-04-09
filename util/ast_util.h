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

#ifndef AST_UTIL_H_
#define AST_UTIL_H_

#include <string>

#include "../ast/ast_types.h"
#include "../ast/expression.h"
#include "../ir/mmo_types.h"
#include "util_types.h"
#include "index.h"

/**
 *
 */
#define IS_CREF(X) ((X)->expressionType()==EXPCOMPREF)
/**
 *
 */
#define IS_UMINUS(X) ((X)->expressionType()==EXPUMINUS)
/**
 *
 */
#define IS_UMINUS_VAR(X) (IS_UMINUS(X) && IS_CREF((X)->getAsUMinus()->exp()))
/**
 *
 */
#define UMINUS_EXP(X) ((X)->getAsUMinus()->exp())
/**
 *
 */
#define CREF_NAME(X) ( IS_UMINUS(X) ?  *(UMINUS_EXP(X)->getAsComponentReference()->names()->front()) : *((X)->getAsComponentReference()->names()->front()) )
/**
 *
 */
#define IS_VAR(X) ((IS_CREF(X) || IS_UMINUS_VAR(X)) && (!IS_PARAMETER(X)))
/**
 *
 */
#define IS_ZERO_REAL(X) ((X)->expressionType()==EXPREAL && (X)->getAsReal()->val()==0.0)
/**
 *
 */
#define IS_ZERO_INT(X) ((X)->expressionType()==EXPINTEGER && (X)->getAsInteger()->val()==0)
/**
 *
 */
#define IS_ZERO(X) (IS_ZERO_REAL(X) || IS_ZERO_INT(X))
/**
 *
 */
#define IS_ADD(X) ((X)->expressionType()==EXPBINOP && (X)->getAsBinOp()->binopType()==BINOPADD)
/**
 *
 */
#define LEFT_EXP(X) ((X)->getAsBinOp()->left())
/**
 *
 */
#define RIGHT_EXP(X) ((X)->getAsBinOp()->right())
/**
 *
 */
#define IS_SUB(X) ((X)->expressionType()==EXPBINOP && (X)->getAsBinOp()->binopType()==BINOPSUB)
/**
 *
 */
#define IS_SUM_(X) (IS_SUB(X) || IS_ADD(X))
/**
 *
 */
#define IS_SUM_OF_VARS(X) (IS_SUM_(X) && (IS_VAR((X)->getAsBinOp()->left()) && IS_VAR((X)->getAsBinOp()->right())))
/**
 *
 */
#define IS_STATE(X) (_varSymbolTable->lookup(CREF_NAME(X))!=NULL && _varSymbolTable->lookup(CREF_NAME(X))->isState())
/**
 *
 */
#define IS_PARAMETER(X) (IS_UMINUS(X) ? _varSymbolTable->lookup(CREF_NAME(UMINUS_EXP(X)))!=NULL && _varSymbolTable->lookup(CREF_NAME(UMINUS_EXP(X)))->isParameter() :  _varSymbolTable->lookup(CREF_NAME(X))!=NULL && _varSymbolTable->lookup(CREF_NAME(X))->isParameter())
/**
 *
 */
#define IS_RELOP(X) ((X)->expressionType()==EXPBINOP && (X)->getAsBinOp()->binopType() >= BINOPLOWER && (X)->getAsBinOp()->binopType() <= BINOPGREATEREQ)
/**
 *
 */
#define IS_BNOT(X) ((X)->expressionType()==EXPBOOLEANNOT)
/**
 *
 */
#define _VAR(v)			newAST_Expression_ComponentReferenceExp(v)
/**
 *
 */
#define GREATER(l,r) 	newAST_Expression_BinOp(l, r, BINOPGREATER )
/**
 *
 */
#define LOWER(l,r) 		newAST_Expression_BinOp(l, r, BINOPLOWER )
/**
 *
 */
#define ADD(l,r) 		newAST_Expression_BinOp(l, r, BINOPADD )
/**
 *
 */
#define MULT(l,r) 		newAST_Expression_BinOp(l, r, BINOPMULT )
/**
 *
 */
#define SUB(l,r) 		newAST_Expression_BinOp(l, r, BINOPSUB )
/**
 *
 */
#define I(n) 			newAST_Expression_Integer(n)
/**
 *
 */
#define _R(n) 			newAST_Expression_Real(n)
/**
 *
 */
#define _PA(e)       	newAST_Expression_OutputExpressions(e)
/**
 *
 */
#define UMENOS(e)   	SUB( I(1) , e )
/**
 *
 */
#define UNARYM(e)		newAST_Expression_UnaryMinus(e)
/**
 *
 */
#define GREATEREQ(l,r) 	newAST_Expression_BinOp(l, r, BINOPGREATEREQ )
/**
 *
 */
#define LOWEREQ(l,r) 	newAST_Expression_BinOp(l, r, BINOPLOWERWQ )
/**
 *
 */
#define EQUAL(l,r)	 	newAST_Expression_BinOp(l, r,  BINOPCOMPEQ)

/**
 *
 */
class AST_Expression_Traverse
{
public:
  virtual ~AST_Expression_Traverse ()
  {
  }
  ;
  /**
   *
   * @param
   * @return
   */
  AST_Expression
  mapTraverse (AST_Expression);
private:
  virtual AST_Expression
  mapTraverseElement (AST_Expression) = 0;
};


/**
 *
 */
template<class R>
  class AST_Expression_Visitor
  {
  public:
  /**
   *
   */
    virtual ~AST_Expression_Visitor ()
    {
    }
    ;
    /**
     *
     * @param e
     * @return
     */
    R
    foldTraverse (AST_Expression e)
    {
      switch (e->expressionType ())
	{
	case EXPBINOP:
	  {
	    AST_Expression_BinOp b = e->getAsBinOp ();
	    AST_Expression left = b->left (), right = b->right ();
	    return (foldTraverseElement (foldTraverse (left),
					 foldTraverse (right), b->binopType ()));
	  }
	case EXPUMINUS:
	  return (foldTraverseElementUMinus (e));
	default:
	  return (foldTraverseElement (e));
	}
    }
    ;
  private:
    virtual R
    foldTraverseElement (AST_Expression) = 0;
    virtual R
    foldTraverseElementUMinus (AST_Expression) = 0;
    virtual R
    foldTraverseElement (R, R, BinOpType) = 0;
  };

/**
 *
 */
template<class R>
  class AST_Expression_Fold
  {
  public:
  /**
   *
   */
    virtual ~AST_Expression_Fold ()
    {
    }
    ;
    /**
     *
     * @param e
     * @return
     */
    R
    foldTraverse (AST_Expression e)
    {
      switch (e->expressionType ())
	{
	case EXPBINOP:
	  {
	    AST_Expression_BinOp b = e->getAsBinOp ();
	    AST_Expression left = b->left (), right = b->right ();
	    return (foldTraverseElement (foldTraverse (left),
					foldTraverse (right), b->binopType ()));
	  }
	case EXPOUTPUT:
	  {
	    AST_Expression_Output o = e->getAsOutput ();
	    return (foldTraverse (AST_ListFirst (o->expressionList ())));
	  }
	case EXPUMINUS:
	  return (foldTraverseElementUMinus (e));
	default:
	  return (foldTraverseElement (e));
	}
    }
    ;
  private:
    virtual R
    foldTraverseElement (AST_Expression) = 0;
    virtual R
    foldTraverseElementUMinus (AST_Expression) = 0;
    virtual R
    foldTraverseElement (R, R, BinOpType) = 0;
  };

/**
 *
 */
class EqualExp
{
public:
  /**
   *
   * @param symbolTable
   */
  EqualExp (VarSymbolTable symbolTable);
  /**
   *
   * @param a
   * @param b
   * @return
   */
  bool
  equalTraverse (AST_Expression a, AST_Expression b);
private:
  bool
  _compareList (AST_ExpressionList ael, AST_ExpressionList bel);
  bool
  equalTraverseElement (AST_Expression a, AST_Expression b);
  VarInfo
  getVarInfo (AST_Expression_ComponentReference compRef);
  bool
  compareArrays (AST_Expression_ComponentReference arrayA,
		 AST_Expression_ComponentReference arrayB);
  VarSymbolTable _symbolTable;
};

/**
 *
 */
class ControlVars_ : public AST_Expression_Fold<bool>
{
public:
  /**
   *
   * @param st
   * @param lst
   */
  ControlVars_ (VarSymbolTable st, VarSymbolTable lst);
  /**
   *
   */
  ~ControlVars_ ();
private:
  bool
  foldTraverseElement (AST_Expression);
  bool
  foldTraverseElement (bool, bool, BinOpType);
  bool
  foldTraverseElementUMinus (AST_Expression);
  VarSymbolTable _st;
  VarSymbolTable _lst;
};
/**
 *
 * @param st
 * @param lst
 * @return
 */
ControlVars
newControlVars (VarSymbolTable st, VarSymbolTable lst);
/**
 *
 * @param m
 */
void
deleteControlVars (ControlVars m);

/**
 *
 */
class IsConstant : public AST_Expression_Fold<bool>
{
public:
  /**
   *
   * @param st
   */
  IsConstant (VarSymbolTable st) :
      _st (st)
  {
  }
  ;
private:
  bool
  foldTraverseElement (AST_Expression);
  bool
  foldTraverseElement (bool, bool, BinOpType);
  bool
  foldTraverseElementUMinus (AST_Expression);
  VarSymbolTable _st;
};

/**
 *
 */
class ReplaceExp : public AST_Expression_Traverse
{
public:
  /**
   *
   * @param rep
   * @param for_exp
   * @param in
   * @param symbol_table
   * @return
   */
  AST_Expression
  replaceExp (AST_Expression rep, AST_Expression for_exp, AST_Expression in,
	      VarSymbolTable symbol_table);
private:
  AST_Expression
  mapTraverseElement (AST_Expression);
  AST_Expression _rep, _for_exp, _in;
  VarSymbolTable _symbol_table;
};

/**
 * 
 */
class ReplaceBoolean : public AST_Expression_Fold<AST_Expression>
{
public:
/**
 *
 */
  ReplaceBoolean ();
private:
  AST_Expression
  foldTraverseElement (AST_Expression);
  AST_Expression
  foldTraverseElement (AST_Expression, AST_Expression, BinOpType);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression);
};

/* WhenEqualityTrasforms: Realiza los cambios necesarios a los 
 * Equation_When antes de transformarlos a Statement 
 * 
 */

/**
 *
 */
class WhenEqualityTrasforms : public AST_Expression_Fold<AST_Expression>
{
public:
  /**
   *
   */
  WhenEqualityTrasforms ();
private:
  AST_Expression
  foldTraverseElement (AST_Expression);
  AST_Expression
  foldTraverseElement (AST_Expression, AST_Expression, BinOpType);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression);
};

/**
 * 
 */
class PreChange : public AST_Expression_Fold<AST_Expression>
{
public:
/**
 *
 */
  PreChange (PreSet);
private:
  PreSet _pre;
  AST_Expression
  foldTraverseElement (AST_Expression);
  AST_Expression
  foldTraverseElement (AST_Expression, AST_Expression, BinOpType);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression);
};

/* FindReference: Devuelve si hay referencia de una variable en una expresion
 * 
 */
/**
 *
 */
class FindReference : public AST_Expression_Fold<bool>
{
public:
  /**
   *
   */
  FindReference (AST_String);
private:
  AST_String _var;
  bool
  foldTraverseElement (AST_Expression);
  bool
  foldTraverseElement (bool, bool, BinOpType);
  bool
  foldTraverseElementUMinus (AST_Expression);
};

/* ReplaceReference: Cambia las referencias de una variable por otra
 * 
 * 
 */
/**
 *
 */
class ReplaceReference : public AST_Expression_Fold<AST_Expression>
{
public:
  /**
   *
   * @param
   * @param
   */
  ReplaceReference (AST_String, AST_String);
private:
  AST_String _pre;
  AST_String _post;
  AST_Expression
  foldTraverseElement (AST_Expression);
  AST_Expression
  foldTraverseElement (AST_Expression, AST_Expression, BinOpType);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression);
};

/**
 * Evaluates an arithmetic expression.
 * TODO Ver el calculo del tipo de la expression de retorno en el caso en el que se llega a obtener un número.
 */
/**
 *
 */
class EvalExp : public AST_Expression_Fold<AST_Expression>
{
public:
  /**
   *
   * @param symbolTable
   */
  EvalExp (VarSymbolTable symbolTable);
  /**
   * Evaluates an arithmetic expression.
   */
  /**
   *
   * @param exp
   * @return
   */
  AST_Expression
  eval (AST_Expression exp);
  /**
   * Evaluates an arithmetic expression replacing the occurrences of compRef with the value provided (compRefValue).
   */
  /**
   *
   * @param compRef
   * @param compRefValue
   * @param exp
   * @return
   */
  AST_Expression
  eval (AST_Expression_ComponentReference compRef, AST_Expression compRefValue,
	AST_Expression exp);
private:
  AST_Expression
  foldTraverseElement (AST_Expression);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression);
  AST_Expression
  foldTraverseElement (AST_Expression, AST_Expression, BinOpType);
  AST_Expression
  evalCompRef (AST_Expression_ComponentReference compRef);
  AST_Expression
  evalArray (AST_Expression_ComponentReference exp);
  bool
  shouldReturnInteger (AST_Expression left, AST_Expression right);
  bool
  shouldReturnReal (AST_Expression left, AST_Expression right);
  AST_Real
  getRealVal (AST_Expression exp);
  AST_Expression_ComponentReference _compRef;
  AST_Expression _compRefVal;
  VarSymbolTable _symbolTable;
};

/**
 *
 */
class AST_Visitor_
{
public:
/**
 *
 */
  virtual
  ~AST_Visitor_ ();
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Class x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_Class x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Composition x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_Composition x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_CompositionElement x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_CompositionElement x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_CompositionEqsAlgs x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_CompositionEqsAlgs x) = 0;
  /**
   *
   * @param
   */
  virtual void
  visit (AST_External_Function_Call) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Element x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Modification x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_Modification x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Comment x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Equation x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_ForIndex x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Equation_Else x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Expression x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Argument x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Statement x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_Statement x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_Statement_Else x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  visit (AST_StoredDefinition x) = 0;
  /**
   *
   * @param x
   */
  virtual void
  leave (AST_StoredDefinition x) = 0;
  /**
   *
   * @param x
   * @return
   */
  virtual int
  apply (AST_Node x) = 0;
};

/**
 *
 */
class ExpressionIndex_ : public AST_Expression_Fold<Index>
{
public:
  /**
   *
   * @param vt
   */
  ExpressionIndex_ (VarSymbolTable vt);
  /**
   *
   */
  ~ExpressionIndex_ ();
  Index
  /**
   *
   * @param exp
   * @return
   */
  index (AST_Expression exp);
  /**
   *
   * @param idx
   * @param index
   */
  void
  setIndex (Index *idx, Index index);
private:
  Index
  foldTraverseElement (AST_Expression);
  Index
  foldTraverseElementUMinus (AST_Expression);
  Index
  foldTraverseElement (Index, Index, BinOpType);
  VarSymbolTable _vt;
};
/**
 *
 * @param vt
 * @return
 */
ExpressionIndex
newExpressionIndex (VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteExpressionIndex (ExpressionIndex m);

/**
 *
 */
class GenerateDeps_ : public AST_Expression_Fold<Dependencies>
{
public:
  /**
   *
   * @param data
   */
  GenerateDeps_ (MMO_ModelData data);
  /**
   *
   */
  ~GenerateDeps_ ();
private:
  VarSymbolTable _vt;
  MMO_EquationTable _algs;
  MMO_ModelData _data;
  Index _idx;
  Dependencies
  foldTraverseElement (AST_Expression exp);
  Dependencies
  foldTraverseElementUMinus (AST_Expression exp);
  Dependencies
  foldTraverseElement (Dependencies l, Dependencies r, BinOpType bot);
  void
  _algebraicsDeps(AST_Expression_ComponentReference cr, Index idx, VarInfo vi, Dependencies ret);
};
/**
 *
 * @param data
 * @return
 */
GenerateDeps
newGenerateDeps (MMO_ModelData data);
/**
 *
 * @param m
 */
void
deleteGenerateDeps (GenerateDeps m);

/**
 *
 */
class ExpIndexes_ : public AST_Expression_Fold<string>
{
public:
  /**
   *
   * @param vt
   */
  ExpIndexes_ (VarSymbolTable vt);
  /**
   *
   */
  ~ExpIndexes_ ();
  /**
   *
   * @param idx
   * @param modelica
   * @param offset
   */
  void
  setEnvironment (string idx, bool modelica = false, int offset = 1);
private:
  string
  foldTraverseElement (AST_Expression exp);
  string
  foldTraverseElementUMinus (AST_Expression exp);
  string
  foldTraverseElement (string l, string r, BinOpType bot);
  VarSymbolTable _vt;
  string _idx;
  int _offset;
  bool _modelica;
};
/**
 *
 * @param vt
 * @return
 */
ExpIndexes
newExpIndexes (VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteExpIndexes (ExpIndexes m);

/**
 *
 */
class ReplaceDer_ : public AST_Expression_Visitor<AST_Expression>
{
public:
  /**
   *
   * @param vt
   */
  ReplaceDer_ (VarSymbolTable vt);
  /**
   *
   */
  ~ReplaceDer_ ();
private:
  AST_Expression
  foldTraverseElement (AST_Expression exp);
  AST_Expression
  foldTraverseElementUMinus (AST_Expression exp);
  AST_Expression
  foldTraverseElement (AST_Expression l, AST_Expression r, BinOpType bot);
  VarSymbolTable _vt;
};
/**
 *
 * @param vt
 * @return
 */
ReplaceDer
newReplaceDer (VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteReplaceDer (ReplaceDer m);

#endif  /* AST_UTIL_H_ */
