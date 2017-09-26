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

#ifndef MMO_STATEMENT_H_
#define MMO_STATEMENT_H_

#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../util/dependencies.h"
#include "../util/util_types.h"
#include "mmo_base.h"
#include "mmo_types.h"

/**
 *
 */
class MMO_Statement_: public MMO_Base_
{
  public:
    /**
     *
     * @param stm
     * @param data
     */
    MMO_Statement_(AST_Statement stm, MMO_ModelData data);
    /**
     *
     * @param stm
     */
    MMO_Statement_(AST_Statement stm);
    /**
     *
     */
    MMO_Statement_();
    /**
     *
     */
    ~MMO_Statement_();
    /**
     *
     * @return
     */
    string
    print();
    /**
     *
     * @param indent
     * @param idx
     * @param offset
     * @param order
     * @param forOffset
     * @return
     */
    list<string>
    print(string indent, string idx = "", int offset = 0, int order = 1,
        int forOffset = 0);
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
    Dependencies
    deps();
    /**
     *
     * @return
     */
    Dependencies
    lhs();
    private:
    void
    _getIndexList(AST_Expression_ComponentReference cr, Index index,
        list<Index> *idxs);
    void
    _init();
    void
    _setInitialCode(AST_Statement stm);
    void
    _insertDeps(AST_Expression exp);
    void
    _insertDeps(AST_Statement stm, Range range = Range());
    void
    _insertVectorDeps(Dependencies deps, Dependencies in, DEP_Type type,
        DEP_Type insert, Range range);
    Index
    _getIndex(AST_Expression_ComponentReference cr, VarInfo vi);
    void
    _printAssignment(const string& name, AST_Expression_ComponentReference cr,
        AST_Expression e, const string& indent, const string& idx, int offset,
        int order, int forOffset, list<string>& ret);
    void
    _printList(AST_StatementListIterator it, AST_StatementList stl,
        const string& indent, const string& idx, int offset, int order,
        int forOffset,
        list<string>& ret);
    void
    _printIfExpression(AST_Expression e, string lhs, const string& indent,
        const string& idx, int order, int offset, int forOffset,
        list<string>& ret, list<string>& code);

    AST_Statement _stm;
    MMO_ModelData _data;
    Dependencies _deps;
    Dependencies _lhs;
    bool _initialCode;
    list<string> _variables;
    Index _eventLhs;
};
/**
 *
 * @param stm
 * @param data
 * @return
 */
MMO_Statement
newMMO_Statement(AST_Statement stm, MMO_ModelData data);
/**
 *
 * @param stm
 * @return
 */
MMO_Statement
newMMO_Statement(AST_Statement stm);
/**
 *
 * @return
 */
MMO_Statement
newMMO_Statement();
/**
 *
 * @param m
 */
void
deleteMMO_Statement(MMO_Statement m);

#endif  /* MMO_STATEMENT_H_ */
