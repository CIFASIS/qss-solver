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
#include "../ir/index.h"
#include "util_types.h"

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
    virtual
    ~AST_Expression_Traverse() {};
    /**
     *
     * @param
     * @return
     */
    AST_Expression
    apply(AST_Expression);
  private:
    virtual AST_Expression
    mapTraverseElement(AST_Expression) = 0;
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
    virtual
    ~AST_Expression_Visitor() {};
    /**
     *
     * @param e
     * @return
     */
    R
    apply(AST_Expression e)
    {
      switch(e->expressionType())
      {
        case EXPBINOP:
          {
          AST_Expression_BinOp b = e->getAsBinOp();
          AST_Expression left = b->left(), right = b->right();
          return (foldTraverseElement(apply(left), apply(right),
              b->binopType()));
        }
        case EXPUMINUS:
          return foldTraverseElementUMinus(e);
        default:
          return foldTraverseElement(e);
      }
    }
    ;
  private:
    virtual R
    foldTraverseElement(AST_Expression) = 0;
    virtual R
    foldTraverseElementUMinus(AST_Expression) = 0;
    virtual R
    foldTraverseElement(R, R, BinOpType) = 0;
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
    virtual
    ~AST_Expression_Fold() {};
    /**
     *
     * @param e
     * @return
     */
    R
    apply(AST_Expression e)
    {
      switch(e->expressionType())
      {
        case EXPBINOP:
        {
          AST_Expression_BinOp b = e->getAsBinOp();
          AST_Expression left = b->left(), right = b->right();
          return (foldTraverseElement(apply(left), apply(right), b->binopType()));
        }
        case EXPOUTPUT:
        {
          AST_Expression_Output o = e->getAsOutput();
          return apply(AST_ListFirst(o->expressionList()));
        }
        case EXPUMINUS:
          return foldTraverseElementUMinus(e);
        default:
          return foldTraverseElement(e);
      }
    };
  private:
    virtual R
    foldTraverseElement(AST_Expression) = 0;
    virtual R
    foldTraverseElementUMinus(AST_Expression) = 0;
    virtual R
    foldTraverseElement(R, R, BinOpType) = 0;
};

/**
 *
 */
class AST_Visitor
{
  public:
    /**
     *
     */
    ~AST_Visitor(){};
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Class x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_Class x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Composition x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_Composition x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_CompositionElement x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_CompositionElement x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_CompositionEqsAlgs x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_CompositionEqsAlgs x) = 0;
    /**
     *
     * @param
     */
    virtual void
    visit(AST_External_Function_Call) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Element x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Modification x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_Modification x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Comment x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Equation x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_ForIndex x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Equation_Else x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Expression x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Argument x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Statement x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_Statement x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_Statement_Else x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    visit(AST_StoredDefinition x) = 0;
    /**
     *
     * @param x
     */
    virtual void
    leave(AST_StoredDefinition x) = 0;
    /**
     *
     * @param x
     * @return
     */
    virtual int
    apply(AST_Node x) = 0;
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
    EqualExp(MicroModelica::Util::VarSymbolTable symbolTable);
    /**
     *
     * @param a
     * @param b
     * @return
     */
    bool
    equalTraverse(AST_Expression a, AST_Expression b);
  private:
    bool
    _compareList(AST_ExpressionList ael, AST_ExpressionList bel);
    bool
    equalTraverseElement(AST_Expression a, AST_Expression b);
    Option<MicroModelica::Util::Variable>
    getVarInfo(AST_Expression_ComponentReference compRef);
    bool
    compareArrays(AST_Expression_ComponentReference arrayA, AST_Expression_ComponentReference arrayB);
    MicroModelica::Util::VarSymbolTable _symbolTable;
};

/**
 *
 */
class IsConstant: public AST_Expression_Fold<bool>
{
  public:
    /**
     *
     * @param st
     */
    IsConstant(MicroModelica::Util::VarSymbolTable st) :
        _st(st)
    {
    };
  private:
    bool
    foldTraverseElement(AST_Expression);
    bool
    foldTraverseElement(bool, bool, BinOpType);
    bool
    foldTraverseElementUMinus(AST_Expression);
    MicroModelica::Util::VarSymbolTable _st;
};

/**
 *
 */
class ReplaceExp: public AST_Expression_Traverse
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
    replaceExp(AST_Expression rep, AST_Expression for_exp, AST_Expression in, MicroModelica::Util::VarSymbolTable symbol_table);
  private:
    AST_Expression
    mapTraverseElement(AST_Expression);
    AST_Expression _rep, _for_exp, _in;
    MicroModelica::Util::VarSymbolTable _symbol_table;
};

/**
 * 
 */
class ReplaceBoolean: public AST_Expression_Fold<AST_Expression>
{
  public:
    /**
     *
     */
    ReplaceBoolean();
  private:
    AST_Expression
    foldTraverseElement(AST_Expression);
    AST_Expression
    foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression);
};

/* WhenEqualityTrasforms: Realiza los cambios necesarios a los 
 * Equation_When antes de transformarlos a Statement 
 * 
 */

/**
 *
 */
class WhenEqualityTrasforms: public AST_Expression_Fold<AST_Expression>
{
  public:
    /**
     *
     */
    WhenEqualityTrasforms();
  private:
    AST_Expression
    foldTraverseElement(AST_Expression);
    AST_Expression
    foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression);
};

/**
 * 
 */
class PreChange: public AST_Expression_Fold<AST_Expression>
{
  public:
    /**
     *
     */
    PreChange(PreSet);
  private:
    PreSet _pre;
    AST_Expression
    foldTraverseElement(AST_Expression);
    AST_Expression
    foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression);
};

/* FindReference: Devuelve si hay referencia de una variable en una expresion
 * 
 */
/**
 *
 */
class FindReference: public AST_Expression_Fold<bool>
{
  public:
    /**
     *
     */
    FindReference(AST_String);
  private:
    AST_String _var;
    bool
    foldTraverseElement(AST_Expression);
    bool
    foldTraverseElement(bool, bool, BinOpType);
    bool
    foldTraverseElementUMinus(AST_Expression);
};

/**
 *
 */
class ReplaceReference: public AST_Expression_Fold<AST_Expression>
{
  public:
    /**
     *
     * @param
     * @param
     */
    ReplaceReference(AST_String, AST_String);
  private:
    AST_String _pre;
    AST_String _post;
    AST_Expression
    foldTraverseElement(AST_Expression);
    AST_Expression
    foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression);
};

/**
 *
 */
class ReplaceDer: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param vt
     */
    ReplaceDer(MicroModelica::Util::VarSymbolTable vt);
    /**
     *
     */
    ~ReplaceDer();
  private:
    AST_Expression
    foldTraverseElement(AST_Expression exp);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
    MicroModelica::Util::VarSymbolTable _vt;
};

/**
 * Evaluates an arithmetic expression.
 */
class EvalExp: public AST_Expression_Fold<AST_Expression>
{
  public:
    /**
     *
     * @param symbolTable
     */
    EvalExp(MicroModelica::Util::VarSymbolTable symbolTable);
    /**
     * Evaluates an arithmetic expression.
     */
    /**
     *
     * @param exp
     * @return
     */
    AST_Expression
    eval(AST_Expression exp);
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
    eval(AST_Expression_ComponentReference compRef, AST_Expression compRefValue, AST_Expression exp);
  private:
    AST_Expression
    foldTraverseElement(AST_Expression);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression);
    AST_Expression
    foldTraverseElement(AST_Expression, AST_Expression, BinOpType);
    AST_Expression
    evalCompRef(AST_Expression_ComponentReference compRef);
    AST_Expression
    evalArray(AST_Expression_ComponentReference exp);
    bool
    shouldReturnInteger(AST_Expression left, AST_Expression right);
    bool
    shouldReturnReal(AST_Expression left, AST_Expression right);
    AST_Real
    getRealVal(AST_Expression exp);
    AST_Expression_ComponentReference _compRef;
    AST_Expression _compRefVal;
    MicroModelica::Util::VarSymbolTable _symbolTable;
};


/**
 *
 */
class EvalInitExp: public AST_Expression_Fold<int>
{
  public:
    /**
     *
     * @param vt
     */
    EvalInitExp(MicroModelica::Util::VarSymbolTable vt);
    /**
     *
     */
    ~EvalInitExp();
    private:
    int
    foldTraverseElement(AST_Expression exp);
    int
    foldTraverseElement(int l, int r, BinOpType bot);
    int
    foldTraverseElementUMinus(AST_Expression exp);
    MicroModelica::Util::VarSymbolTable _vt;
};

/**
 * @breif Helper class that looks for a variable 
 *        definition in more than one symbol table.
 */
class VariableLookup: public AST_Expression_Fold<bool>
{
  public:
    /**
     *
     * @param st
     * @param lst
     */
    VariableLookup(MicroModelica::Util::VarSymbolTable st, MicroModelica::Util::VarSymbolTable lst);
    /**
     *
     */
    ~VariableLookup();
  private:
    bool
    foldTraverseElement(AST_Expression);
    bool
    foldTraverseElement(bool, bool, BinOpType);
    bool
    foldTraverseElementUMinus(AST_Expression);
    MicroModelica::Util::VarSymbolTable _st;
    MicroModelica::Util::VarSymbolTable _lst;
};

/**
 *
 */
class ReplaceInnerProduct: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param vt
     */
    ReplaceInnerProduct(MicroModelica::Util::VarSymbolTable vt);
    /**
     *
     */
    ~ReplaceInnerProduct();
  private:
    bool
    controlArray(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression exp);
    MicroModelica::Util::VarSymbolTable _symbols;
};

class ConvertExpression 
{
  public:
    ConvertExpression(AST_Expression left, AST_Expression right, MicroModelica::Util::VarSymbolTable& symbols);
    /**
     *
     */
    ~ConvertExpression();
    inline std::string 
    get() { return _convert; };
  private:
    AST_Expression_ComponentReference 
    componentReference(AST_Expression exp);
    MicroModelica::Util::Variable 
    variable(AST_Expression_ComponentReference exp);
    bool 
    scalarExpression(AST_Expression exp);
    double 
    scalarValue(AST_Expression exp);
    void 
    convert();
    AST_Expression                       _left;
    AST_Expression                       _right;
    MicroModelica::Util::VarSymbolTable  _symbols;
    std::string                          _convert;
};

class ConvertEquation
{
  public:
    ConvertEquation(AST_Equation equation, MicroModelica::Util::VarSymbolTable symbols);
    ~ConvertEquation() {};
    inline AST_Equation 
    get() { return _equation; };
  private:
    AST_Equation 
    convert(AST_Equation eq);
    AST_Equation                        _equation;
    MicroModelica::Util::VarSymbolTable _symbols;

};

class ConvertStatement 
{
  public:
    ConvertStatement(AST_Statement statement, MicroModelica::Util::VarSymbolTable symbols);
    ~ConvertStatement() {};
    inline AST_Statement 
    get() { return _statement; };
  private:
    AST_Statement 
    convert(AST_Statement st);
    AST_StatementList 
    convert(AST_StatementList sts);  
    AST_Statement_ElseList 
    convert(AST_Statement_ElseList stel);
    AST_Statement                       _statement;
    MicroModelica::Util::VarSymbolTable _symbols;
};

#endif  /* AST_UTIL_H_ */
