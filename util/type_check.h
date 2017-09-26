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

#include "../ast/ast_types.h"
#include "type.h"
#include "util_types.h"

#ifndef TYPECHECK_H_
#define TYPECHECK_H_

/**
 *
 */
class TypeCheck_
{
  public:
    /**
     *
     * @param
     * @param
     */
    TypeCheck_(TypeSymbolTable, VarSymbolTable);
    /**
     *
     * @param t
     * @return
     */
    Type
    check_expression(AST_Expression t);
    private:
    TypeSymbolTable tyEnv;
    VarSymbolTable varEnv;
    Type
    check_binop(AST_Expression l, AST_Expression r, BinOpType op);

    /* Mira si dos tipos son iguales (Real == Integer === true) */
    bool
    check_equal(Type t1, Type t2);
};

#endif  /* TYPECHECK_H_ */
