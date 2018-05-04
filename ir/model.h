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

#ifndef MMO_CLASS_H_
#define MMO_CLASS_H_

#include <list>
#include <map>
#include <string>
#include <vector>

#include "../ast/ast_types.h"
#include "../util/error.h"
#include "../util/model_dependencies.h"
#include "../util/model_tables.h"
#include "../util/util_types.h"

/**
 *
 */
typedef enum
{
  CL_MODEL,  //!< CL_MODEL
  CL_PACKAGE,  //!< CL_PACKAGE
  CL_FUNCTION  //!< CL_FUNCTION
} CL_Type;

/**
 *
 */
typedef enum
{
  DEC_PUBLIC,  //!< DEC_PUBLIC
  DEC_LOCAL  //!< DEC_LOCAL
} DEC_Type;


class MMO_Model;

class MMO_Function;

class MMO_Package;

class MMO_Annotation;

class MMO_ModelAnnotation;

/**
 *
 */
class MMO_Class
{
  public:
    /**
     *
     */
    MMO_Class();
    /**
     *
     */
    ~MMO_Class();
    /**
     *
     * @return
     */
    MMO_Package*
    getAsPackage() const;
    /**
     *
     * @return
     */
    MMO_Model*
    getAsModel() const;
    /**
     *
     * @return
     */
    MMO_Function*
    getAsFunction() const;
    /**
     *
     * @return
     */
    virtual string
    name() const = 0;
    /**
     *
     * @return
     */
    virtual CL_Type
    classType() = 0;
    /**
     *
     * @param n
     */
    virtual void
    insert(string n) = 0;
    /**
     *
     * @param eq
     */
    virtual void
    insert(AST_Equation eq) = 0;
    /**
     *
     * @param stm
     * @param initial
     */
    virtual void
    insert(AST_Statement stm, bool initial) = 0;
    /**
     *
     * @param stm
     */
    virtual void
    insert(AST_Statement stm) = 0;
    /**
     *
     * @param f
     */
    virtual void
    insert(MMO_Function f) = 0;
    /**
     *
     * @param efc
     */
    virtual void
    insert(AST_External_Function_Call efc) = 0;
    /**
     *
     * @param n
     * @param vi
     * @param type
     */
    virtual void
    insert(VarName n, VarInfo vi, DEC_Type type) = 0;
    /**
     *
     * @param n
     * @param vi
     */
    virtual void
    insert(VarName n, VarInfo vi) = 0;
    /**
     *
     * @param x
     */
    virtual void
    insert(AST_Argument_Modification x) = 0;
    /**
     *
     * @return
     */
    virtual VarSymbolTable
    varTable() = 0;
    /**
     *
     * @return
     */
    virtual MMO_ImportTable
    imports() = 0;
};

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
    setFunctions(MMO_FunctionTable functions,
        MMO_FunctionTable externalFunctions,
        MMO_SymbolRefTable calledFunctions);
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
    MMO_ModelData _data;
    MMO_PackageTable _packages;
    MMO_FunctionTable _functions;
    MMO_FunctionTable _externalFunctions;
    string _prefix;
    list<VarInfo> _arguments;
    TypeSymbolTable _types;
};

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
