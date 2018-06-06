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

#ifndef MMO_UTIL_H_
#define MMO_UTIL_H_

#include <list>
#include <string>

#include "../ast/ast_types.h"
#include "../util/index.h"
#include "../util/model_tables.h"

/**
 *
 */
class MMO_ModelConfig
{
  public:
    /**
     *
     */
    MMO_ModelConfig();
    /**
     *
     */
    ~MMO_ModelConfig();
    /**
     *
     * @return
     */
    MMO_EquationTable
    algebraics();
    /**
     *
     * @param algs
     */
    void
    setAlgebraics(MMO_EquationTable algs);
    /**
     *
     * @param algs
     */
    void
    setSymbols(VarSymbolTable algs);
    /**
     *
     * @return
     */
    VarSymbolTable
    symbols();
    /**
     *
     * @param f
     */
    void
    setCalledFunctions(MMO_SymbolRefTable f);
    /**
     *
     * @return
     */
    MMO_SymbolRefTable
    calledFunctions();
    /**
     *
     * @param p
     */
    void
    setPackages(MMO_PackageTable p);
    /**
     *
     * @return
     */
    MMO_PackageTable
    packages();
    /**
     *
     * @param f
     */
    void
    setExternalFunctions(MMO_FunctionTable f);
    /**
     *
     * @return
     */
    MMO_FunctionTable
    externalFunctions();
    /**
     *
     * @return
     */
    bool
    hasExternalFunctions();
    /**
     *
     * @param f
     */
    void
    setFunctions(MMO_FunctionTable f);
    /**
     *
     * @return
     */
    MMO_FunctionTable
    functions();
    /**
     *
     * @param s
     */
    void
    setInitialCode(bool s);
    /**
     *
     * @return
     */
    bool
    initialCode();
    /**
     *
     * @return
     */
    bool
    generateDerivatives();
    /**
     *
     * @param gd
     */
    void
    setGenerateDerivatives(bool gd);
    /**
     *
     * @return
     */
    int
    begin();
    void
    setBegin(int begin, int dim = 0);
    /**
     *
     * @return
     */
    int
    end();
    void
    setEnd(int end, int dim = 0);
    /**
     *
     * @param range
     */
    void
    setRange(Range range);
    Range
    range();
    /**
     *
     * @return
     */
    Index
    lhs();
    /**
     *
     * @param idx
     */
    void
    setLHS(Index idx);
    /**
     *
     */
    void
    clear();
    /**
     *
     * @param ws
     */
    void
    setWhenStatement(bool ws);
    /**
     *
     * @return
     */
    bool
    whenStatement();
    /**
     *
     * @param w
     */
    void
    setWeight(double w);
    /**
     *
     * @return
     */
    double
    weight();
    /**
     *
     * @return
     */
    bool
    hasWeight();
    /**
     *
     * @param ca
     */
    void
    setCalculateAlgegraics(bool ca);
    /**
     *
     * @return
     */
    bool
    calculateAlgebraics();
    void
    setArguments(AST_Expression arguments);
    bool
    hasArguments();
    AST_Expression
    arguments();
    void
    setAnnotation(MMO_Annotation annotation);
    MMO_Annotation
    annotation();
    bool
    hasAnnotation();
    void
    setDisableSymDiff(bool d);
    bool
    disableSymDiff();
    private:
    VarSymbolTable _symbols;
    MMO_EquationTable _algebraics;
    MMO_FunctionTable _functions;
    MMO_FunctionTable _externalFunctions;
    MMO_PackageTable _packages;
    bool _initialCode;
    bool _generateDerivatives;
    Range _range;
    Index _lhs;
    bool _whenStatement;
    double _weight;
    MMO_SymbolRefTable _calledFunctions;
    bool _calculateAlgebraics;
    AST_Expression _arguments;
    MMO_Annotation _annotation;
    bool _disableSymDiff;
};

/**
 *
 */
class MMO_FunctionConfig
{
  public:
    /**
     *
     * @param var
     * @param name
     * @param args
     * @param data
     */
    MMO_FunctionConfig(string var, string name, AST_ExpressionList args, MMO_ModelData data);
    /**
     *
     */
    ~MMO_FunctionConfig();
    /**
     *
     * @param indent
     * @return
     */
    list<string>
    print(string indent);
    private:
    string _var;
    string _name;
    AST_ExpressionList _args;
    MMO_ModelData _data;
};

/**
 *
 */
class MMO_PackageConfig
{
  public:
    /**
     *
     * @param name
     * @param fit
     * @param objects
     */
    MMO_PackageConfig(string name, MMO_FunctionDefinitionTable fit, MMO_ImportTable objects);
    /**
     *
     */
    ~MMO_PackageConfig();
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
    prefix();
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
     * @return
     */
    MMO_FunctionDefinitionTable
    functions();
    /**
     *
     * @return
     */
    MMO_ImportTable
    objects();
    private:
    string _name;
    MMO_FunctionDefinitionTable _fit;
    MMO_ImportTable _objects;
};

/**
 *
 */
class MMO_FunctionDefinition
{
  public:
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
class MMO_FunctionInfo
{
  public:
    /**
     *
     * @param fd
     */
    MMO_FunctionInfo(MMO_FunctionDefinition fd);
    /**
     *
     * @param f
     */
    MMO_FunctionInfo(MMO_Function f);
    /**
     *
     */
    ~MMO_FunctionInfo();
    /**
     *
     * @return
     */
    bool
    isFunction();
    /**
     *
     * @return
     */
    bool
    isFunctionDefinition();
    /**
     *
     * @return
     */
    MMO_Function
    function();
    /**
     *
     * @return
     */
    MMO_FunctionDefinition
    functionDefinition();
    private:
    MMO_Function _f;
    MMO_FunctionDefinition _fd;
};

#endif  /* MMO_UTIL_H_ */
