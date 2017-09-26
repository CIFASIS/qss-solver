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

#ifndef AST_NODE_H_
#define AST_NODE_H_

#include "../util/macros.h"
#include "../util/util_types.h"

using namespace std;

/**
 *
 */
class AST_Node_
{
  public:
    /**
     *
     */
    AST_Node_();
    /**
     *
     */
    virtual
    ~AST_Node_()
    {

    }
    ;
    /**
     *
     */
    DEFINE_CLASS_PRINTER(AST_Node)
    ;
    /**
     *
     * @param linenum
     */
    void
    setLineNum(int linenum);
    /**
     *
     * @return
     */
    int
    lineNum() const;
    /**
     *
     * @param visitor
     */
    virtual void
    accept(AST_Visitor visitor) = 0;
    private:
    int _linenum;
};

#endif  /* AST_NODE_H_ */
