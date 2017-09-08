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
#include "../util/md_index.h"
#include "../util/util_types.h"
#include "mmo_types.h"

/**
 *
 */
class MMO_ModelData_
{
public:
    /**
     *
     */
    MMO_ModelData_ ();
    /**
     *
     */
    ~MMO_ModelData_ ();
    /**
     *
     * @return
     */
    MMO_EquationTable
    algebraics ();
    /**
     *
     * @param algs
     */
    void
    setAlgebraics (MMO_EquationTable algs);
    /**
     *
     * @param algs
     */
    void
    setSymbols (VarSymbolTable algs);
    /**
     *
     * @return
     */
    VarSymbolTable
    symbols ();
    /**
     *
     * @param f
     */
    void
    setCalledFunctions (MMO_SymbolRefTable f);
    /**
     *
     * @return
     */
    MMO_SymbolRefTable
    calledFunctions ();
    /**
     *
     * @param p
     */
    void
    setPackages (MMO_PackageTable p);
    /**
     *
     * @return
     */
    MMO_PackageTable
    packages ();
    /**
     *
     * @param f
     */
    void
    setExternalFunctions (MMO_FunctionTable f);
    /**
     *
     * @return
     */
    MMO_FunctionTable
    externalFunctions ();
    /**
     *
     * @return
     */
    bool
    hasExternalFunctions ();
    /**
     *
     * @param f
     */
    void
    setFunctions (MMO_FunctionTable f);
    /**
     *
     * @return
     */
    MMO_FunctionTable
    functions ();
    /**
     *
     * @param s
     */
    void
    setInitialCode (bool s);
    /**
     *
     * @return
     */
    bool
    initialCode ();
    /**
     *
     * @return
     */
    bool
    generateDerivatives ();
    /**
     *
     * @param gd
     */
    void
    setGenerateDerivatives (bool gd);
    /**
     *
     * @return
     */
    int
    begin ();
    void
    setBegin (int begin, int dim = 0);
    /**
     *
     * @return
     */
    int
    end ();
    void
    setEnd (int end, int dim = 0);
    /**
     *
     * @param range
     */
    void
    setRange (Range range);
    Range
    range ();
    /**
     *
     * @return
     */
    Index
    lhs ();
    /**
     *
     * @param idx
     */
    void
    setLHS (Index idx);
    /**
     *
     */
    void
    clear ();
    /**
     *
     * @param ws
     */
    void
    setWhenStatement (bool ws);
    /**
     *
     * @return
     */
    bool
    whenStatement ();
    /**
     *
     * @param w
     */
    void
    setWeight (double w);
    /**
     *
     * @return
     */
    double
    weight ();
    /**
     *
     * @return
     */
    bool
    hasWeight ();
    /**
     *
     * @param ca
     */
    void
    setCalculateAlgegraics (bool ca);
    /**
     *
     * @return
     */
    bool
    calculateAlgebraics ();
    void
    setArguments (AST_Expression arguments);
    bool
    hasArguments ();
    AST_Expression
    arguments ();
    void
    setAnnotation (MMO_Annotation annotation);
    MMO_Annotation
    annotation ();
    bool
    hasAnnotation ();
    void
    setDisableSymDiff (bool d);
    bool
    disableSymDiff ();
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
 * @return
 */
MMO_ModelData
newMMO_ModelData ();
/**
 *
 * @param m
 */
void
deleteMMO_ModelData (MMO_ModelData m);

/**
 *
 */
class MMO_FunctionData_
{
public:
    /**
     *
     * @param var
     * @param name
     * @param args
     * @param data
     */
    MMO_FunctionData_ (string var, string name, AST_ExpressionList args, MMO_ModelData data);
    /**
     *
     */
    ~MMO_FunctionData_ ();
    /**
     *
     * @param indent
     * @return
     */
    list<string>
    print (string indent);
private:
    string _var;
    string _name;
    AST_ExpressionList _args;
    MMO_ModelData _data;
};
/**
 *
 * @param var
 * @param name
 * @param args
 * @param data
 * @return
 */
MMO_FunctionData
newMMO_FunctionData (string var, string name, AST_ExpressionList args, MMO_ModelData data);
/**
 *
 * @param m
 */
void
deleteMMO_FunctionData (MMO_FunctionData m);

/**
 *
 */
class MMO_PackageData_
{
public:
    /**
     *
     * @param name
     * @param fit
     * @param objects
     */
    MMO_PackageData_ (string name, MMO_FunctionDefinitionTable fit, MMO_ImportTable objects);
    /**
     *
     */
    ~MMO_PackageData_ ();
    /**
     *
     * @return
     */
    string
    name ();
    /**
     *
     * @return
     */
    string
    prefix ();
    /**
     *
     * @return
     */
    list<string>
    includeDirectories ();
    /**
     *
     * @return
     */
    list<string>
    libraryDirectories ();
    /**
     *
     * @return
     */
    list<string>
    linkLibraries ();
    /**
     *
     * @return
     */
    MMO_FunctionDefinitionTable
    functions ();
    /**
     *
     * @return
     */
    MMO_ImportTable
    objects ();
private:
    string _name;
    MMO_FunctionDefinitionTable _fit;
    MMO_ImportTable _objects;
};
/**
 *
 * @param name
 * @param fit
 * @param objects
 * @return
 */
MMO_PackageData
newMMO_PackageData (string name, MMO_FunctionDefinitionTable fit, MMO_ImportTable objects);
/**
 *
 * @param m
 */
void
deleteMMO_PackageData (MMO_PackageData m);

/**
 *
 */
class MMO_FunctionDefinition_
{
public:
    /**
     *
     * @param name
     * @param includeDir
     * @param libraryDir
     * @param libraries
     */
    MMO_FunctionDefinition_ (string name, string includeDir, string libraryDir, list<string> libraries);
    /**
     *
     */
    ~MMO_FunctionDefinition_ ();
    /**
     *
     * @return
     */
    list<string>
    def ();
    /**
     *
     * @return
     */
    bool
    hasIncludeDirectory ();
    /**
     *
     * @return
     */
    bool
    hasLibraryDirectory ();
    /**
     *
     * @return
     */
    bool
    hasLibraries ();
    /**
     *
     * @return
     */
    string
    includeDirectory ();
    /**
     *
     * @return
     */
    string
    libraryDirectory ();
    /**
     *
     * @return
     */
    list<string>
    libraries ();
    /**
     *
     * @return
     */
    string
    name ();
    /**
     *
     * @return
     */
    string
    prototype ();
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
 * @param name
 * @param includeDir
 * @param libraryDir
 * @param libraries
 * @return
 */
MMO_FunctionDefinition
newMMO_FunctionDefinition (string name, string includeDir, string libraryDir, list<string> libraries);
/**
 *
 * @param m
 */
void
deleteMMO_FunctionDefinition (MMO_FunctionDefinition m);

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
    MMO_FunctionInfo (MMO_FunctionDefinition fd);
    /**
     *
     * @param f
     */
    MMO_FunctionInfo (MMO_Function f);
    /**
     *
     */
    ~MMO_FunctionInfo ();
    /**
     *
     * @return
     */
    bool
    isFunction ();
    /**
     *
     * @return
     */
    bool
    isFunctionDefinition ();
    /**
     *
     * @return
     */
    MMO_Function
    function ();
    /**
     *
     * @return
     */
    MMO_FunctionDefinition
    functionDefinition ();
private:
    MMO_Function _f;
    MMO_FunctionDefinition _fd;
};

#endif  /* MMO_UTIL_H_ */
