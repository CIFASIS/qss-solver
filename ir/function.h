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

#ifndef MMO_FUNCTION_H_
#define MMO_FUNCTION_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "class.h"
#include "../ast/ast_types.h"
#include "../ir/mmo_util.h"
#include "../util/error.h"
#include "../util/util_types.h"

/**
 *
 */
class MMO_Function: public MMO_Class
{
  public:
    /**
     *
     * @param name
     */
    MMO_Function(string name);
    /**
     *
     */
    ~MMO_Function();
    /**
     *
     * @param c
     */
    void
    setFather(MMO_Class* c);
    /**
     *
     * @return
     */
    bool
    hasFather();
    /**
     *
     * @return
     */
    MMO_Class*
    father() const;
    /**
     *
     * @return
     */
    string
    name() const;
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
    insert(MMO_Function &f);
    /**
     *
     * @param n
     */
    void
    insert(string n);
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
    VarSymbolTable
    varTable();
    /**
     *
     * @return
     */
    MMO_ImportTable
    imports();
};

/**
 *
 */
class MMO_FunctionDefinition
{
  public:
    /**
     *
     */
    MMO_FunctionDefinition();
    /**
     *
     * @param name
     * @param includeDir
     * @param libraryDir
     * @param libraries
     */
    MMO_FunctionDefinition(string name, string includeDir, string libraryDir, list<string> libraries);
    /**
     *
     */
    ~MMO_FunctionDefinition();
    /**
     *
     * @return
     */
    list<string>
    def();
    /**
     *
     * @return
     */
    bool
    hasIncludeDirectory();
    /**
     *
     * @return
     */
    bool
    hasLibraryDirectory();
    /**
     *
     * @return
     */
    bool
    hasLibraries();
    /**
     *
     * @return
     */
    string
    includeDirectory();
    /**
     *
     * @return
     */
    string
    libraryDirectory();
    /**
     *
     * @return
     */
    list<string>
    libraries();
    /**
     *
     * @return
     */
    string
    name();
    /**
     *
     * @return
     */
    string
    prototype();
  private:
    list<string> _def;
    string _name;
    string _prototype;
    string _includeDirectory;
    string _libraryDirectory;
    list<string> _libraries;
};

/**
 *
 */
class MMO_FunctionTable: public ModelTable<MMO_Function>
{
  public:
    /**
     *
     */
    MMO_FunctionTable();
    ~MMO_FunctionTable();
};
#endif  /* MMO_FUNCTION_H_ */
