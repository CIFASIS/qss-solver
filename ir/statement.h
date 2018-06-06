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

/**
 *
 */
class MMO_Statement
{
  public:
    /**
     *
     * @param stm
     * @param data
     */
    MMO_Statement(AST_Statement stm, MMO_ModelConfig &config);
    /**
     *
     * @param stm
     */
    MMO_Statement(AST_Statement stm);
    /**
     *
     */
    MMO_Statement();
    /**
     *
     */
    ~MMO_Statement();
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
  private:
    void
    _init();
    void
    _setInitialCode(AST_Statement stm);
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
    MMO_ModelConfig _config;
    bool _initialCode;
    list<string> _variables;
};

#endif  /* MMO_STATEMENT_H_ */
