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

#ifndef MMO_PACKAGE_H_
#define MMO_PACKAGE_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "../ast/ast_types.h"
#include "../util/error.h"
#include "../util/model_dependencies.h"
#include "../util/model_tables.h"
#include "../util/util_types.h"
#include "class.h"

/**
 *
 */
class MMO_Package: public MMO_Class
{
  public:
    /**
     *
     * @param name
     */
    MMO_Package(string name);
    /**
     *
     */
    ~MMO_Package();
    /**
     *
     * @return
     */
    VarSymbolTable
    varTable();
    /**
     *
     * @return
     */
    CL_Type
    classType();
    /**
     *
     * @return
     */
    string
    name() const;
    /**
     *
     * @return
     */
    string
    fileName() const;
    /**
     *
     * @param n
     */
    void
    insert(string n);
    /**
     *
     * @param eq
     */
    void
    insert(AST_Equation eq);
    /**
     *
     * @param stm
     * @param initial
     */
    void
    insert(AST_Statement stm, bool initial);
    /**
     *
     * @param stm
     */
    void
    insert(AST_Statement stm);
    /**
     *
     * @param f
     */
    void
    insert(MMO_Function f);
    /**
     *
     * @param efc
     */
    void
    insert(AST_External_Function_Call efc);
    /**
     *
     * @param n
     * @param vi
     * @param type
     */
    void
    insert(VarName n, VarInfo vi, DEC_Type type);
    /**
     *
     * @param n
     * @param vi
     */
    void
    insert(VarName n, VarInfo vi);
    /**
     *
     * @param x
     */
    void
    insert(AST_Argument_Modification x);
    /**
     *
     * @return
     */
    MMO_FunctionTable
    functions();
    /**
     *
     * @return
     */
    MMO_ImportTable
    imports();
    /**
     *
     * @return
     */
    string
    prefix();
    private:
    MMO_FunctionTable _functions;
    string _name;
    unsigned int _funcs;
    MMO_ImportTable _imports;
    MMO_PackageTable _packages;
};

#endif  /* MMO_CLASS_H_ */
