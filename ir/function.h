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
#include "../util/error.h"
#include "../util/model_dependencies.h"
#include "../util/model_tables.h"
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
    setFather(MMO_Class c);
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
    MMO_Class
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
    insert(MMO_Function f);
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
    CL_Type
    classType();
    /**
     *
     * @return
     */
    string
    prototype();
    /**
     *
     * @return
     */
    list<string>
    localDeclarations();
    /**
     *
     * @return
     */
    MMO_StatementTable
    statements();
    /**
     *
     * @return
     */
    string
    returnStatement();
    /**
     *
     * @return
     */
    MMO_ArgumentsTable
    externalFunctionCalls();
    /**
     *
     * @return
     */
    MMO_ImportTable
    imports();
    /**
     *
     * @param it
     */
    void
    setImports(MMO_ImportTable it);
    /**
     *
     * @param prefix
     */
    void
    setPrefix(string prefix);
    /**
     *
     * @return
     */
    MMO_Annotation
    annotation();
    void
    setFunctions(MMO_FunctionTable functions, MMO_FunctionTable externalFunctions, MMO_SymbolRefTable calledFunctions);
  private:
    string _name;
    VarSymbolTable _declarations;
    VarSymbolTable _localDeclarations;
    MMO_StatementTable _statements;
    MMO_FunctionAnnotation _annotations;
    MMO_ArgumentsTable _externalFunctionCalls;
    MMO_SymbolRefTable _calledFunctions;
    MMO_ImportTable _imports;
    unsigned int _externalFuncs;
    int _outputs;
    string _outputName;
    MMO_ModelConfig _cfg;
    MMO_PackageTable _packages;
    MMO_FunctionTable _functions;
    MMO_FunctionTable _externalFunctions;
    string _prefix;
    list<VarInfo> _arguments;
    TypeSymbolTable _types;
};

#endif  /* MMO_FUNCTION_H_ */
