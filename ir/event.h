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

#ifndef MMO_EVENT_H_
#define MMO_EVENT_H_

#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../util/index.h"
#include "../util/util_types.h"

/**
 *
 */
typedef enum
{
  HND_POSITIVE, //!< HND_POSITIVE
  HND_NEGATIVE, //!< HND_NEGATIVE
  HND_ZERO      //!< HND_ZERO
} HND_Type;

/**
 * Define the original relation type of the zero-crossing function
 * needed by the initial algorithm.
 */
typedef enum
{
  ZC_LT,    //!< ZC_LT
  ZC_LE,    //!< ZC_LE
  ZC_GT,    //!< ZC_GT
  ZC_GE     //!< ZC_GE
} ZC_REL;

/**
 *
 */
class MMO_Event
{
  public:
    /**
     *
     * @param cond
     * @param data
     */
    MMO_Event(AST_Expression cond, MMO_ModelConfig data);
    /**
     *
     */
    ~MMO_Event();
    /**
     *
     * @return
     */
    string
    print();
    /**
     *
     * @return
     */
    MMO_Equation
    condition();
    /**
     *
     * @param cond
     */
    void
    setCondition(MMO_Expression cond);
    /**
     *
     * @param cond
     * @return
     */
    bool
    compareCondition(AST_Expression cond);
    /**
     *
     * @param stm
     */
    void
    insert(AST_Statement stm);
    /**
     *
     * @param h
     * @return
     */
    MMO_Statement
    begin(HND_Type h);
    /**
     *
     * @return
     */
    MMO_Statement
    next();
    /**
     *
     * @return
     */
    bool
    end();
    /**
     *
     * @return
     */
    int
    beginRange();
    /**
     *
     * @return
     */
    int
    endRange();
    /**
     *
     * @return
     */
    bool
    hasPositiveHandler();
    /**
     *
     * @return
     */
    bool
    hasNegativeHandler();
    /**
     *
     * @return
     */
    HND_Type
    handlerType();
    /**
     *
     * @param h
     */
    void
    setHandlerType(HND_Type h);
    /**
     *
     * @return
     */
    bool
    hasWeight();
    /**
     *
     * @return
     */
    double
    weight();
    ZC_REL
    zcRelation();
    private:
    AST_Expression
    _getExpression(AST_Expression exp);
    MMO_Equation _cond;
    list<MMO_Statement> _positiveHandlerStatements;
    list<MMO_Statement> _negativeHandlerStatements;
    int _init;
    int _end;
    HND_Type _handler;
    HND_Type _handlerType;
    list<MMO_Statement>::iterator _it;
    MMO_ModelConfig _data;
    VarSymbolTable _lhsVars;
    double _weight;
    ZC_REL _zcRelation;
};

#endif  /*  MMO_EVENT_H_ */
