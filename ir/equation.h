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

#ifndef MMO_EQUATION_H_
#define MMO_EQUATION_H_

#include <list>
#include <set>
#include <string>
#include <map>

#include "../ast/ast_types.h"
#include "../util/index.h"
#include "../util/util_types.h"
#include "../util/dependencies.h"
#include "mmo_base.h"
#include "mmo_types.h"

typedef enum
{
  EQ_DERIVATIVE,
  EQ_DEPENDENCIES,
  EQ_CLASSIC,
  EQ_ALGEBRAIC,
  EQ_OUTPUT,
  EQ_ZC,
  EQ_HANDLER,
  EQ_HANDLER_IF,
  EQ_JACOBIAN
} EQ_Type;

/**
 *
 */
class MMO_Equation_: public MMO_Base_
{
  public:
    /**
     *
     * @param exp
     * @param data
     */
    MMO_Equation_(AST_Expression exp, MMO_ModelData data);
    /**
     *
     * @param exp
     * @param data
     */
    MMO_Equation_(MMO_Expression exp, MMO_ModelData data);
    /**
     *
     */
    ~MMO_Equation_();
    /**
     *
     * @return
     */
    MMO_Expression
    exp();
    /**
     *
     * @return
     */
    int
    end();
    /**
     *
     * @return
     */
    list<string>
    getVariables();
    /**
     *
     * @return
     */
    bool
    hasRange();
    /**
     *
     * @return
     */
    int
    init();
    /**
     *
     * @return
     */
    Index
    lhs();
    /**
     *
     * @return
     */
    string
    print();
    /**
     *
     * @param indent
     * @param lhs
     * @param idx
     * @param palgs
     * @param algs
     * @param type
     * @param order
     * @param constant
     * @param offset
     * @param dereq
     * @param forOffset
     * @param constantOffset
     * @return
     */
    list<string>
    print(string indent, string lhs = "", string idx = "", bool palgs = false,
        MMO_EquationTable algs = NULL, EQ_Type type = EQ_DERIVATIVE, int order =
            1,
        bool constant = false, int offset = 0, bool dereq = true,
        int forOffset = 0, int constantOffset = 0);
    /**
     *
     * @param variable
     * @param idx
     * @param indent
     * @param lhs
     * @param variableChange
     * @return
     */
    string
    printRange(string variable, string idx, string indent, Index lhs,
        bool variableChange = false);
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
    getEquation();
    /**
     *
     * @return
     */
    list<string>
    getAlgebraics();
    /**
     *
     */
    void
    controlAlgebraicDefinition();
    /**
     *
     * @return
     */
    set<Index>
    algebraicArguments();
    MMO_Equation
    jacobianExp(Index idx, DEP_Type type = DEP_STATE);
    bool
    controlAlgebraicArguments(set<Index> *algs, set<Index> eqAlgs);
    private:
    void
    _initDerivatives();
    MMO_Expression
    _generateChainRule(Index idx);
    void
    _generateJacobianExps();
    string
    _printArguments(int i, string idx, int offset, int cte, int order,
        int forOffset);
    MMO_ModelData _data;
    int _end;
    MMO_Expression _exp[4];
    int _init;
    Index _lhs;
    list<string> _variables;
    AST_Expression _arguments;
    list<string> _code;
    list<string> _equation;
    list<string> _algebraics;
    set<Index> _algebraicArguments;
    map<string, MMO_Expression> _jacobianExps;
    int _coeffs;
};
/**
 *
 * @param exp
 * @param data
 * @return
 */
MMO_Equation
newMMO_Equation(AST_Expression exp, MMO_ModelData data);
/**
 *
 * @param exp
 * @param data
 * @return
 */
MMO_Equation
newMMO_Equation(MMO_Expression exp, MMO_ModelData data);
/**
 *
 * @param m
 */
void
deleteMMO_Equation(MMO_Equation m);

#endif  /* MMO_EQUATION_H_ */
