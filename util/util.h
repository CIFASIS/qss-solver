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

#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../ir/mmo_types.h"
#include "util_types.h"

using namespace std;

/**
 *
 */
#define SLASH "/"
/**
 *
 */
#define 	BINOPS 18

#if __x86_64__
typedef long int grp_t;
#else
typedef int grp_t;
#endif

/**
 *
 */
typedef enum
{
    BIF_NONE,  //!< BIF_NONE
    BIF_REINIT,  //!< BIF_REINIT
    BIF_TERMINATE, //!< BIF_TERMINATE
    BIF_SUM,   //!< BIF_SUM
    BIF_PRODUCT,   //!< BIF_PRODUCT
    BIF_INNER_PRODUCT,   //!< BIF_INNER_PRODUCT
    BIF_MIN,   //!< BIF_MIN
    BIF_MAX,   //!< BIF_MAX
    BIF_ABS,   //!< BIF_ABS
    BIF_SIGN,  //!< BIF_SIGN
    BIF_SQRT,  //!< BIF_SQRT
    BIF_CEIL,  //!< BIF_CEIL
    BIF_FLOOR, //!< BIF_FLOOR
    BIF_SIN,   //!< BIF_SIN
    BIF_COS,   //!< BIF_COS
    BIF_TAN,   //!< BIF_TAN
    BIF_ASIN,  //!< BIF_ASIN
    BIF_ACOS,  //!< BIF_ACOS
    BIF_ATAN,  //!< BIF_ATAN
    BIF_ATAN2, //!< BIF_ATAN2
    BIF_SINH,  //!< BIF_SINH
    BIF_COSH,  //!< BIF_COSH
    BIF_TANH,  //!< BIF_TANH
    BIF_EXP,   //!< BIF_EXP
    BIF_LOG,   //!< BIF_LOG
    BIF_LOG10, //!< BIF_LOG10
    BIF_PRE,    //!< BIF_PRE
    BIF_GQLINK  //!< BIF_GQLINK
} BIF_NAMES;

/**
 *
 */
typedef enum
{
    BIV_NONE, //!< BIV_NONE
    BIV_TIME, //!< BIV_TIME
    BIV_SUM,   //!< BIV_SUM
    BIV_PRODUCT,   //!< BIV_PRODUCT
    BIV_INNER_PRODUCT,   //!< BIV_INNER_PRODUCT
    BIV_MIN,   //!< BIV_MIN
    BIV_MAX   //!< BIV_MAX
} BIV_NAMES;

class BIF
{
public:
    /**
     *
     */
    virtual
    ~BIF ()
    {
    }
    ;
    list<string>
    generateCode (string variableMap, string variableIndex, list<VariableInterval> variableInterval, int expOrder);
    void
    setSymbolTable (VarSymbolTable vt);
    VarSymbolTable
    symbolTable (VarSymbolTable vt);
    string
    variableName (VariableInterval vin);
    string
    expressionOrderStr (int order, VariableInterval vin);
    bool
    isState (VariableInterval vin);
    void
    setExpressionOrder (int expressionOrder);
    int
    expressionOrder ();
    string
    print (Index idx, string variableIndex);
private:
    VarSymbolTable _vt;
    int _expressionOrder;
    bool
    _hasStates (list<VariableInterval> variables);
    Index
    _index (list<VariableInterval> variables);
    VarInfo
    _variableInfo (VariableInterval vin);
    virtual string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates) = 0;
    virtual string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates) = 0;
};

/**
 *
 */
class Util
{
public:
    /**
     *
     * @return
     */
    static Util *
    getInstance ();
    /**
     *
     */
    ~Util ();
    string
    trimString (string str);
    /**
     *
     * @return
     */
    string
    languageEspecification ();
    /**
     *
     * @param t
     * @return
     */
    bool
    checkTypeString (string t);
    /**
     *
     * @param annotation
     * @return
     */
    bool
    checkExperimentAnnotations (string *annotation);
    /**
     *
     * @param fname
     * @return
     */
    BIF_NAMES
    checkBuiltInFunctions (string fname);
    /**
     *
     * @param fname
     * @return
     */
    BIF_NAMES
    checkBuiltInReductionFunctions (string fname);
    /**
     *
     * @param fname
     * @return
     */
    BIV_NAMES
    checkBuiltInVariables (string fname);
    /**
     *
     * @param fname
     * @param type
     */
    void
    addBuiltInVariables (string fname, BIV_NAMES type);
    /**
     *
     * @param bot
     * @return
     */
    string
    opString (BinOpType bot);
    /**
     *
     * @param n
     * @param vt
     * @return
     */
    string
    newVarName (string n, VarSymbolTable vt);
    /**
     *
     * @param name
     * @return
     */
    string
    getVarName (string name);
    /**
     *
     * @param fileName
     * @param pt
     * @return
     */
    bool
    readPackage (string fileName, MMO_PackageTable pt);
    /**
     *
     * @param fileName
     * @return
     */
    MMO_PackageData
    readPackage (string fileName);
    /**
     *
     * @param pname
     * @param flags
     * @return
     */
    bool
    searchCompiledPackage (string pname, MMO_CompileFlags flags);
    /**
     *
     * @param pname
     * @param flags
     * @param ext
     * @return
     */
    string
    packagePath (string pname, MMO_CompileFlags flags, string ext = ".mo");
    /**
     *
     * @param flags
     */
    void
    setCompileFlags (MMO_CompileFlags flags);
    /**
     *
     * @param name
     * @return
     */
    string
    packageName (string name);
    /**
     *
     * @param data
     */
    void
    setData (MMO_ModelData data);
    /**
     *
     * @param vi
     * @param indent
     * @param localVar
     * @return
     */
    string
    printInitialAssignment (VarInfo vi, string indent, string localVar = "i0");
    /**
     *
     * @param ev
     * @return
     */
    string
    environmentVariable (string ev);
    /**
     *
     * @param file
     * @return
     */
    string
    getFilePath (string file);
    /**
     *
     * @param file
     * @return
     */
    string
    getFileName (string file);
    BIF *
    builtInReductionFunctions (BIF_NAMES fn);
    bool
    checkGKLinkFunctions (string name);
private:
    Util ();
    static Util *_instance;
    bool
    _checkCodeFiles (string name, string ext);
    list<string>
    _getValue (fstream *package, string token);
    string
    _packagePath (string name);
    string _languageEspecification;
    int _varCounter;
    MMO_CompileFlags _flags;
    MMO_ModelData _data;
    map<string, BIF_NAMES> _builtInFunctions;
    map<string, BIV_NAMES> _builtInVariables;
    map<BIF_NAMES, BIF*> _builtInFunctionImp;
    map<string, int> _annotations;
    string _binop[BINOPS];
};

class BuiltInFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

class BuiltInSumFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInSumFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

class BuiltInProductFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInProductFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

class BuiltInInnerProductFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInInnerProductFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

class BuiltInMinFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInMinFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

class BuiltInMaxFunction : public BIF
{
public:
    /**
     *
     */
    ~BuiltInMaxFunction ();
private:
    string
    _reduce (string variableMap, string variableIndex, int variableOrder, list<VariableInterval> variableInterval, bool hasStates);
    string
    _init (string variableMap, string variableIndex, list<VariableInterval> variableInterval, bool hasStates);
};

#endif  /* UTIL_H_ */
