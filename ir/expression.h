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

/**
 *
 */
class MMO_Expression
{
  public:
    /**
     *
     */
    MMO_Expression();
    /**
     *
     * @param exp
     * @param data
     */
    MMO_Expression(AST_Expression exp, MMO_ModelConfig &config);
    /**
     *
     * @param exp
     */
    MMO_Expression(AST_Expression exp);
    /**
     *
     */
    ~MMO_Expression();
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
    AST_Expression _exp;
    MMO_ModelConfig _config;
    string _str;
    int _equationIndex;
    int _expressionOrder;
    MMO_PrintExp _printer;
    MMO_ReplaceInterval _ri;
};

/**
 *
 */
class MMO_EvalInitExp: public AST_Expression_Fold<int>
{
  public:
    /**
     *
     * @param vt
     */
    MMO_EvalInitExp(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_EvalInitExp();
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
 */
class MMO_ConvertCondition: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param data
     */
    MMO_ConvertCondition(MMO_ModelConfig &config);
    /**
     *
     */
    ~MMO_ConvertCondition();
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
    MMO_ModelConfig _config;
    int _zc;
    int _zcRelation;
};

/**
 *
 */
class MMO_ReplaceInterval: public AST_Expression_Visitor<AST_Expression>
{
  public:
    /**
     *
     * @param vt
     */
    MMO_ReplaceInterval(VarSymbolTable vt);
    /**
     *
     */
    ~MMO_ReplaceInterval();
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
 */
class MMO_PrintExp: public AST_Expression_Visitor<string>
{
  public:
    /**
     *
     * @param vt
     * @param ri
     * @param pt
     */
    MMO_PrintExp(VarSymbolTable vt, MMO_ReplaceInterval ri, MMO_PackageTable pt);
    /**
     *
     */
    ~MMO_PrintExp();
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
