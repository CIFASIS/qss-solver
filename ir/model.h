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

#ifndef MMO_MODEL_H_
#define MMO_MODEL_H_

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
class MMO_Model: public MMO_Class
{
  public:
    /**
     *
     * @param name
     */
    MMO_Model(string name);
    /**
     *
     */
    ~MMO_Model();
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
    CL_Type
    classType();
    /**
     *
     * @param n
     */
    void
    insert(string n);
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
     * @param efc
     */
    void
    insert(AST_External_Function_Call efc);
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
    MMO_EquationTable
    derivatives();
    /**
     *
     * @return
     */
    MMO_EquationTable
    algebraics();
    /**
     *
     * @return
     */
    MMO_EquationTable
    outputs();
    /**
     *
     * @return
     */
    MMO_Annotation
    annotation();
    /**
     *
     * @return
     */
    MMO_StatementTable
    initialCode();
    /**
     *
     * @return
     */
    MMO_EventTable
    events();
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
    MMO_FunctionTable
    functions();
    /**
     *
     * @return
     */
    MMO_SymbolRefTable
    calledFunctions();
    /**
     *
     * @return
     */
    bool
    hasExternalFunctions();
    /**
     *
     */
    void
    setEquations();
    /**
     *
     */
    void
    setEvents();
    /**
     *
     */
    void
    initOutput();
    /**
     *
     * @return
     */
    unsigned int
    states();
    /**
     *
     * @return
     */
    unsigned int
    discretes();
    /**
     *
     * @return
     */
    unsigned int
    algs();
    /**
     *
     * @return
     */
    unsigned int
    evs();
    /**
     *
     * @return
     */
    unsigned int
    funcs();
    /**
     *
     * @return
     */
    unsigned int
    inputs();
    /**
     *
     * @return
     */
    unsigned int
    outs();
    /**
     *
     * @return
     */
    int
    imps();
    /**
     *
     * @return
     */
    list<string>
    includeDirectories();
    /**
     *
     * @return
     */
    list<string>
    libraryDirectories();
    /**
     *
     * @return
     */
    list<string>
    linkLibraries();
    /**
     *
     * @param ft
     */
    void
    setExternalFunctions(MMO_FunctionTable ft);
  private:
    MMO_StatementTable _initialCode;
    unsigned int _states;
    unsigned int _discretes;
    unsigned int _evs;
    unsigned int _algs;
    unsigned int _parameters;
    unsigned int _funcs;
    unsigned int _inputs;
    unsigned int _output;
    unsigned int _stateEquations;
    unsigned int _algebraicEquations;
    string _name;
    list<AST_Equation>  _eqs;
    list<AST_Statement> _stms;
    map<string, string> _includeDirectories;
    map<string, string> _libraryDirectories;
    map<string, string> _linkLibraries;
    map<string, string> _symbolicDerivatives;
    ModelDependencies   _modelDependencies;
    VarSymbolTable      _declarations;
    TypeSymbolTable     _types;
    MMO_ImportTable     _imports;
    MMO_FunctionTable   _functions;
    MMO_FunctionTable   _externalFunctions;
    MMO_SymbolRefTable  _calledFunctions;
    MMO_PackageTable    _packages;
    MMO_EventTable      _events;
    MMO_ModelAnnotation _annotations;
};

#endif  /* MMO_MODEL_H_ */
