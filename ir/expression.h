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

#ifndef MMO_EXPRESSION_H_
#define MMO_EXPRESSION_H_

#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../util/ast_util.h"
#include "../util/dependencies.h"
#include "../util/util_types.h"
#include "mmo_base.h"
#include "mmo_types.h"

/**
 *
 */
class MMO_Expression_: public MMO_Base_
{
  public:
    /**
     *
     */
    MMO_Expression_();
    /**
     *
     * @param exp
     * @param data
     */
    MMO_Expression_(AST_Expression exp, MMO_ModelData data);
    /**
     *
     * @param exp
     */
    MMO_Expression_(AST_Expression exp);
    /**
     *
     */
    ~MMO_Expression_();
    /**
     *
     * @return
     */
    Dependencies
    deps();
    /**
     *
     * @return
     */
    AST_Expression
    exp();
    /**
     *
     * @param exp
     * @return
     */
    string
    findVar(AST_Expression exp);
    /**
     *
     * @return
     */
    list<string>
    getCode();
    /**
     *
     * @return
     */
    list<string>
    getVariables();
    /**
     *
     * @param idx
     * @param offset
     * @return
     */
    string
    indexes(string idx, int offset = 0);
    /**
     *
     * @return
     */
    bool
    hasIndexes();
    /**
     *
     * @return
     */
    string
    print();
    /**
     *
     * @param idx
     * @param offset
     * @param order
     * @param constant
     * @param expressionOrder
     * @param forOffset
     * @return
     */
    string
    print(string idx, int offset = 0, int order = 0, int constant = -1,
        int expressionOrder = 1, int forOffset = 0);
    /**
     *
     * @param exp
     */
    void
    setExpression(AST_Expression exp);
    private:
    void
    _addDeps(Dependencies deps, DEP_Type type, Index index);
    void
    _addVectorDeps(MMO_Equation eq, Index index);
    void
    _addAlgebriacDeps(Index algIndex, MMO_Equation equation,
        Index equationIndex, Index derivativeIndex, Dependencies derivativeDeps,
        map<Index, Index> *states, map<Index, Index> *discretes,
        Index variableChange, int value);
    void
    _traverseAlgebraics(Dependencies deps, Index derivativeIndex,
        Dependencies derivativeDeps, map<Index, Index> *states,
        map<Index, Index> *discretes, Index variableChange, DEP_Type type,
        int value);
    AST_Expression _exp;
    MMO_ModelData _data;
    string _str;
    int _equationIndex;
    int _expressionOrder;
    Dependencies _deps;
    GenerateDeps _gen;
    MMO_PrintExp _printer;
    MMO_ReplaceInterval _ri;
};

/**
 *
 * @param exp
 * @param data
 * @return
 */
MMO_Expression
newMMO_Expression(AST_Expression exp, MMO_ModelData data);
/**
 *
 * @param exp
 * @return
 */
MMO_Expression
newMMO_Expression(AST_Expression exp);
/**
 *
 * @return
 */
MMO_Expression
newMMO_Expression();
/**
 *
 * @param m
 */
void
deleteMMO_Expression(MMO_Expression m);

/**
 *
 */
class MMO_EvalInitExp_: public AST_Expression_Fold<int>
{
  public:
    /**
     *
     * @param vt
     */
    MMO_EvalInitExp_(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_EvalInitExp_();
    private:
    int
    foldTraverseElement(AST_Expression exp);
    int
    foldTraverseElement(int l, int r, BinOpType bot);
    int
    foldTraverseElementUMinus(AST_Expression exp);
    VarSymbolTable _vt;
};
/**
 *
 * @param vt
 * @return
 */
MMO_EvalInitExp
newMMO_EvalInitExp(VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteMMO_EvalInitExp(MMO_EvalInitExp m);

/**
 *
 */
class MMO_ConvertCondition_: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param data
     */
    MMO_ConvertCondition_(MMO_ModelData data);
    /**
     *
     */
    ~MMO_ConvertCondition_();
    /**
     *
     * @return
     */
    int
    zeroCrossing();
    int
    zeroCrossingRelation();
    private:
    AST_Expression
    foldTraverseElement(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression exp);
    MMO_ModelData _data;
    int _zc;
    int _zcRelation;
};
/**
 *
 * @param data
 * @return
 */
MMO_ConvertCondition
newMMO_ConvertCondition(MMO_ModelData data);
/**
 *
 * @param m
 */
void
deleteMMO_ConvertCondition(MMO_ConvertCondition m);

/**
 *
 */
class MMO_PrintExp_: public AST_Expression_Visitor<string>
{
  public:
    /**
     *
     * @param vt
     * @param ri
     * @param pt
     */
    MMO_PrintExp_(VarSymbolTable vt, MMO_ReplaceInterval ri,
        MMO_PackageTable pt);
    /**
     *
     */
    ~MMO_PrintExp_();
    /**
     *
     * @return
     */
    list<string>
    getCode();
    /**
     *
     * @return
     */
    list<string>
    getVariables();
    /**
     *
     * @param idx
     * @param offset
     * @param order
     * @param constant
     * @param expressionOrder
     * @param forOffset
     */
    void
    setEnvironment(string idx, int offset = 0, int order = 0, int constant = -1,
        int expressionOrder = 1, int forOffset = 0);
    private:
    string
    foldTraverseElement(AST_Expression exp);
    string
    foldTraverseElement(string l, string r, BinOpType bot);
    string
    foldTraverseElementUMinus(AST_Expression exp);
    bool
    _findExpression(AST_ExpressionList el, AST_Expression exp);
    void
    _getIndexList(AST_Expression_ComponentReference cr, Index index,
        list<Index> *idxs);
    string
    _printIndex(Index idx, list<Index> idxs, string sub, int offset,
        int constant);
    VarSymbolTable _vt;
    string _idx;
    int _offset;
    int _order;
    int _constant;
    map<string, list<string> > _functions;
    map<string, string> _variables;
    int _expressionOrder;
    MMO_ReplaceInterval _ri;
    MMO_PackageTable _pt;
    int _forOffset;
};

/**
 *
 * @param vt
 * @param ri
 * @param pt
 * @return
 */
MMO_PrintExp
newMMO_PrintExp(VarSymbolTable vt, MMO_ReplaceInterval ri, MMO_PackageTable pt);
/**
 *
 * @param m
 */
void
deleteMMO_PrintExp(MMO_PrintExp m);

/**
 *
 */
class MMO_PartitionInterval_: public AST_Expression_Visitor<list<int> >
{
  public:
    /**
     *
     * @param vt
     */
    MMO_PartitionInterval_(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_PartitionInterval_();
  private:
    list<int>
    foldTraverseElement(AST_Expression exp);
    list<int>
    foldTraverseElement(list<int> l, list<int> r, BinOpType bot);
    list<int>
    foldTraverseElementUMinus(AST_Expression exp);
    VarSymbolTable _vt;
};
/**
 *
 * @param vt
 * @return
 */
MMO_ReplaceInterval
newMMO_ReplaceInterval(VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteMMO_ReplaceInterval(MMO_ReplaceInterval m);



/**
 *
 */
class MMO_ReplaceInterval_: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param vt
     */
    MMO_ReplaceInterval_(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_ReplaceInterval_();
    /**
     *
     * @return
     */
    int
    indexes();
    /**
     *
     * @return
     */
    bool
    end();
    /**
     *
     * @param exp
     * @return
     */
    string
    find(AST_Expression exp);
    /**
     *
     * @return
     */
    VariableInterval
    first();
    /**
     *
     * @param var
     * @return
     */
    bool
    fixedInterval(string var);
    /**
     *
     * @return
     */
    VariableInterval
    next();
    /**
     *
     * @param exp
     * @return
     */
    list<VariableInterval>
    variables(AST_Expression exp);
    private:
    AST_Expression
    foldTraverseElement(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression exp);
    AST_Expression
    _indexExp(int constant = 0, int factor = 1);
    void
    _setIndex(int constant, int factor, int range, VarInfo vi);
    VarSymbolTable _vt;
    unsigned int _indexes;
    list<VariableInterval> _replacedVars;
    map<string, list<VariableInterval> > _replacedExpsVars;
    map<string, AST_Expression> _replacedExps;
    string _currentVar;
};
/**
 *
 * @param vt
 * @return
 */
MMO_ReplaceInterval
newMMO_ReplaceInterval(VarSymbolTable vt);
/**
 *
 * @param m
 */
void
deleteMMO_ReplaceInterval(MMO_ReplaceInterval m);

/**
 *
 */
class MMO_ReplaceInnerProduct: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param vt
     */
    MMO_ReplaceInnerProduct(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_ReplaceInnerProduct();
    private:
    bool
    _controlArray(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression exp);
    AST_Expression
    foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
    AST_Expression
    foldTraverseElementUMinus(AST_Expression exp);
    VarSymbolTable _vt;
};

#endif /* MMO_EXPRESSION_H_ */
