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

#ifndef MMO_SOLVER_H_
#define MMO_SOLVER_H_

#include <list>
#include <map>
#include <set>
#include <string>

#include "../ir/mmo_types.h"
#include "../util/dependencies.h"
#include "../util/md_index.h"
#include "../util/graph.h"
#include "../util/symbol_table.h"
#include "../util/util_types.h"
#include "generator_types.h"
#include "generator_utils.h"

/**
 *
 */
typedef enum
{
    SOL_MODEL_SETTINGS,
    SOL_MODEL,
    SOL_DEPS,
    SOL_ZC,
    SOL_HANDLER_POS,
    SOL_HANDLER_NEG,
    SOL_OUTPUT,
    SOL_INIT,
    SOL_CALLBACK
} SOL_Function;

/**
 *
 */
typedef enum
{
    SOL_LIBRARIES,
    SOL_INCLUDES
} SOL_Makefile;

/**
 *
 */
typedef enum
{
    SOL_SERIAL,
    SOL_PARALLEL
} SOL_Engine;

typedef enum
{
    NOD_SD,
    NOD_SZ,
    NOD_HD,
    NOD_HZ,
    NOD_DD
} NOD_Type;

/**
 *
 */
class MMO_Solver_
{
public:
    /**
     *
     */
    virtual
    ~MMO_Solver_ ()
    {
    }
    ;
    /**
     *
     * @return
     */
    virtual string
    initData () = 0;
    /**
     *
     * @return
     */
    virtual string
    initTime () = 0;
    /**
     *
     * @return
     */
    virtual string
    initOutput () = 0;
    /**
     *
     * @return
     */
    virtual string
    initModel () = 0;
    /**
     *
     * @return
     */
    virtual string
    initHeader () = 0;
    /**
     *
     * @return
     */
    virtual string
    runCmd () = 0;
    /**
     *
     * @param m
     * @return
     */
    virtual string
    makefile (SOL_Makefile m) = 0;
    /**
     *
     */
    virtual void
    initializeMatrices () = 0;
    /**
     *
     */
    virtual void
    initialCode () = 0;
    /**
     *
     */
    virtual void
    model () = 0;
    /**
     *
     */
    virtual void
    modelDeps () = 0;
    /**
     *
     */
    virtual void
    zeroCrossing () = 0;
    /**
     *
     */
    virtual void
    handler () = 0;
    /**
     *
     */
    virtual void
    output () = 0;
    /**
     *
     * @param f
     */
    virtual void
    print (SOL_Function f) = 0;
    /**
     *
     * @return
     */
    virtual Graph
    graph () = 0;
};

/**
 *
 */
class QSS_ : public MMO_Solver_
{
public:
    /**
     *
     * @param model
     * @param flags
     * @param writer
     */
    QSS_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer);
    /**
     *
     */
    ~QSS_ ();
    /**
     *
     * @return
     */
    string
    initData ();
    /**
     *
     * @return
     */
    string
    initTime ();
    /**
     *
     * @return
     */
    string
    initOutput ();
    /**
     *
     * @return
     */
    string
    initModel ();
    /**
     *
     * @return
     */
    string
    initHeader ();
    /**
     *
     * @return
     */
    string
    runCmd ();
    /**
     *
     * @param m
     * @return
     */
    string
    makefile (SOL_Makefile m);
    /**
     *
     */
    void
    initializeMatrices ();
    /**
     *
     */
    void
    initialCode ();
    /**
     *
     */
    void
    model ();
    /**
     *
     */
    void
    modelDeps ();
    /**
     *
     */
    void
    zeroCrossing ();
    /**
     *
     */
    void
    handler ();
    /**
     *
     */
    void
    output ();
    /**
     *
     * @param f
     */
    void
    print (SOL_Function f);
    Graph
    graph ();
private:
    bool
    _controlIntersections (list<Intersection> lis, Intersection is);
    void
    _eventDeps (MMO_Event e, Index index, MMO_EventTable evt, DEP_Type type, map<Index, Index> events);
    void
    _print (SOL_Function f, map<string, string> localVars, WR_Section simple, WR_Section generic, bool switchGen);
    void
    _printDeps (Dependencies d, Index derivativeIndex, MMO_EquationTable equations, MMO_EquationTable algebraics, string idxStr, WR_Section s, int i,
                bool constant);
    void
    _setInterval (Index *i1, Index *i2);
    bool
    _indexDependencies (Index idx, Index *dIdx, Index infIdx, Index *infDIdx, map<int, int> *simpleMatrixDeps, WR_Section alloc, WR_Section init,
                        string allocStr, string initStr, string counter, Intersection intersection, int assignments);
    void
    _eventVectorDependencies (Index index, Dependencies deps, WR_Section alloc, WR_Section init, string allocString, string initString, DEP_Type type);
    void
    _init ();
    void
    _eventAlgebraicDeps (MMO_Event e, Index index, MMO_EventTable evt, DEP_Type type, map<Index, Index> events);
    MMO_CompileFlags _flags;
    MMO_DependenciesTable _modelDeps;
    MMO_DependenciesTable _modelVectorDeps;
    MMO_Writer _writer;
    MMO_Engine _engine;
    map<string, string> _modelVars;
    map<string, string> _modelDepsVars;
    map<string, string> _zcVars;
    map<string, string> _handlerPosVars;
    map<string, string> _handlerNegVars;
    map<string, string> _outputVars;
    map<string, string> _initializeVars;
    map<string, string> _freeVars;
    MMO_Model _model;
    SolverCommon _common;
    string _name;
    Graph _graph;
    bool _parallel;
    bool _hasDD;
};
/**
 *
 * @param model
 * @param flags
 * @param writer
 * @return
 */
QSS
newQSS (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer);
/**
 *
 * @param m
 */
void
deleteQSS (QSS m);

/**
 *
 */
class Classic_ : public MMO_Solver_
{
public:
    /**
     *
     * @param model
     * @param flags
     * @param writer
     */
    Classic_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer);
    /**
     *
     */
    ~Classic_ ();
    /**
     *
     * @return
     */
    string
    initData ();
    /**
     *
     * @return
     */
    string
    initTime ();
    /**
     *
     * @return
     */
    string
    initOutput ();
    /**
     *
     * @return
     */
    string
    initModel ();
    /**
     *
     * @return
     */
    string
    initHeader ();
    /**
     *
     * @return
     */
    string
    runCmd ();
    /**
     *
     * @param m
     * @return
     */
    string
    makefile (SOL_Makefile m);
    /**
     *
     */
    void
    initializeMatrices ();
    /**
     *
     */
    void
    initialCode ();
    /**
     *
     */
    void
    model ();
    /**
     *
     */
    void
    modelDeps ();
    /**
     *
     */
    void
    zeroCrossing ();
    /**
     *
     */
    void
    handler ();
    /**
     *
     */
    void
    output ();
    /**
     *
     * @param f
     */
    void
    print (SOL_Function f);
    Graph
    graph ();
private:
    string
    _prototype (SOL_Function f);
    void
    _print (SOL_Function f, map<string, string> localVars, WR_Section simple, WR_Section generic, bool switchGen);
    void
    _init ();
    void
    _printDeps (Dependencies d, Index derivativeIndex, MMO_EquationTable equations, MMO_EquationTable algebraics, string idxStr, WR_Section s, int i,
                bool constant, Index infIdx);
    void
    _jacobian ();
    void
    _reorderSD (Dependencies d, const Index& idx, const std::__cxx11::string& indent, std::stringstream& buffer, WR_InsertType it);

    MMO_CompileFlags _flags;
    MMO_Model _model;
    MMO_Writer _writer;
    map<string, string> _modelVars;
    map<string, string> _modelDepsVars;
    map<string, string> _zcVars;
    map<string, string> _handlerPosVars;
    map<string, string> _handlerNegVars;
    map<string, string> _outputVars;
    map<string, string> _initializeVars;
    SolverCommon _common;
    string _name;
    map<string, string> _freeVars;
    MMO_DependenciesTable _modelDeps;
};
/**
 *
 * @param model
 * @param flags
 * @param writer
 * @return
 */
Classic
newClassic (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer);
/**
 *
 * @param m
 */
void
deleteClassic (Classic m);

/**
 *
 */
class SolverCommon_
{
public:
    /**
     *
     * @param model
     * @param flags
     * @param writer
     * @param modelVectorDeps
     */
    SolverCommon_ (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer, MMO_DependenciesTable modelVectorDeps, Graph *graph = NULL);
    /**
     *
     */
    ~SolverCommon_ ();
    /**
     *
     * @return
     */
    string
    initOutput ();
    /**
     *
     * @param exps
     * @param section
     */
    void
    print (list<string> exps, WR_Section section);
    /**
     *
     * @param local
     * @param vars
     */
    void
    insertLocalVariables (map<string, string> *local, list<string> vars);
    /**
     *
     * @param name
     * @param variables
     * @param type
     * @param size
     */
    void
    addLocalVar (string name, map<string, string> *variables, string type = "", int size = 0);
    /**
     *
     * @param idx
     * @param simple
     * @param generic
     */
    void
    eventPrologue (Index idx, WR_Section simple, WR_Section generic);
    /**
     *
     * @param idx
     * @param simple
     * @param generic
     */
    void
    eventEpilogue (Index idx, WR_Section simple, WR_Section generic);
    /**
     *
     * @param type
     * @param handlerPosVars
     * @param handlerNegVars
     */
    void
    handler (VST_Environment type, map<string, string> *handlerPosVars, map<string, string> *handlerNegVars);
    /**
     *
     * @return
     */
    int
    getOrder ();
    /**
     *
     * @param type
     * @param zcVars
     */
    void
    zeroCrossing (VST_Environment type, map<string, string> *zcVars);
    /**
     *
     * @param var
     * @param size
     */
    void
    printCleanVector (string var, int size);
    /**
     *
     * @param var
     * @param size
     * @param section
     */
    void
    printSection (string var, int size, WR_Section section);
    /**
     *
     * @param initializeVars
     */
    void
    initialCode (map<string, string> *initializeVars);
    /**
     *
     * @param type
     * @param outputVars
     */
    void
    output (VST_Environment type, map<string, string> *outputVars);
    /**
     *
     * @param eqIdx
     * @param idx
     * @param section
     * @param variables
     */
    void
    genericDefCondition (Index eqIdx, Index idx, WR_Section section, map<string, string> *variables);
    /**
     *
     * @param index
     * @param deps
     * @param alloc
     * @param init
     * @param allocString
     * @param initString
     * @param allocInverse
     * @param initInverse
     * @param allocInverseString
     * @param initInverseString
     * @param count
     * @param countInverse
     * @param dependencies
     * @param type
     */
    void
    vectorDependencies (Index index, Dependencies deps, WR_Section alloc, WR_Section init, string allocString, string initString, WR_Section allocInverse,
                        WR_Section initInverse, string allocInverseString, string initInverseString, string count = "states", string countInverse = "states",
                        bool dependencies = true, DEP_Type type = DEP_STATE_VECTOR, map<string, string> *variables =
                        NULL);
    /**
     *
     * @return
     */
    map<Index, double>
    weights ();
    /**
     *
     * @param deps
     * @param derivativeIndex
     * @param states
     * @param alloc
     * @param allocInverse
     * @param init
     * @param initInverse
     * @param allocSection
     * @param allocInverseSection
     * @param initSection
     * @param initInverseSection
     * @param type
     * @param inverseType
     * @param depType
     * @param modelDeps
     */
    void
    addAlgebriacDeps (Dependencies deps, Index derivativeIndex, map<Index, Index> states, string alloc, string allocInverse, string init, string initInverse,
                      WR_Section allocSection, WR_Section allocInverseSection, WR_Section initSection, WR_Section initInverseSection, string type,
                      string inverseType, DEP_Type depType = DEP_ALGEBRAIC_STATE, MMO_DependenciesTable modelDeps = NULL);
    /**
     *
     * @param deps
     * @param state
     * @param infIndex
     * @param modelDeps
     * @param index
     */
    void
    addModelDeps (Dependencies deps, Index state, Index infIndex, MMO_DependenciesTable modelDeps = NULL, int index = 0);
    /**
     *
     * @param row
     * @param col
     * @param offset
     * @param type
     */
    void
    graphInsert (Index row, Index col, int offset = 0, NOD_Type type = NOD_SD, int assignments = 0);
    /**
     *
     * @return
     */
    bool
    hasGraph ();
    /**
     *
     * @param section
     */
    void
    settings ();
private:
    MMO_Model _model;
    MMO_CompileFlags _flags;
    MMO_Writer _writer;
    MMO_DependenciesTable _modelVectorDeps;
    string _name;
    map<Index, double> _weights;
    Graph &_graph;
    bool _generateGraph;
    bool _parallel;
};
/**
 *
 * @param model
 * @param flags
 * @param writer
 * @param modelVectorDeps
 * @return
 */
SolverCommon
newSolverCommon (MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer, MMO_DependenciesTable modelVectorDeps = NULL, Graph *graph = NULL);
/**
 *
 * @param m
 */
void
deleteSolverCommon (SolverCommon m);

/***
 *
 */
class MMO_Engine_
{
public:
    /**
     *
     */
    virtual
    ~MMO_Engine_ ()
    {
    }
    ;
    /**
     *
     * @param f
     * @return
     */
    virtual string
    prototype (SOL_Function f) = 0;
    /**
     *
     * @param m
     * @return
     */
    virtual string
    makefile (SOL_Makefile m) = 0;
    /**
     *
     */
    virtual string
    variableMap (string index) = 0;
    /**
     *
     */
    virtual string
    endMap () = 0;
    /**
     *
     */
    virtual void
    eventTemp () = 0;
};

/**
 *
 */
class MMO_SerialEngine_ : public MMO_Engine_
{
public:
    /**
     *
     * @param flags
     * @param hybrid
     */
    MMO_SerialEngine_ (MMO_CompileFlags flags, bool hybrid);
    /**
     *
     */
    ~MMO_SerialEngine_ ();
    /**
     *
     * @param f
     * @return
     */
    string
    prototype (SOL_Function f);
    /**
     *
     * @param m
     * @return
     */
    string
    makefile (SOL_Makefile m);
    /**
     *
     * @param index
     * @return
     */
    string
    variableMap (string index);
    /**
     *
     * @return
     */
    string
    endMap ();
    /**
     *
     */
    void
    eventTemp ();
private:
    MMO_CompileFlags _flags;
    bool _hybrid;
};
/**
 *
 * @param flags
 * @param hybrid
 * @return
 */
MMO_SerialEngine
newMMO_SerialEngine (MMO_CompileFlags flags, bool hybrid);
/**
 *
 * @param m
 */
void
deleteMMO_SerialEngine (MMO_SerialEngine m);

/**
 *
 */
class MMO_ParallelEngine_ : public MMO_Engine_
{
public:
    /**
     *
     */
    MMO_ParallelEngine_ (MMO_CompileFlags flags, bool hybrid);
    /**
     *
     */
    ~MMO_ParallelEngine_ ();
    /**
     *
     * @param f
     * @return
     */
    string
    prototype (SOL_Function f);
    /**
     *
     * @param m
     * @return
     */
    string
    makefile (SOL_Makefile m);
    /**
     *
     * @param index
     * @return
     */
    string
    variableMap (string index);
    /**
     *
     * @return
     */
    string
    endMap ();
    /**
     *
     */
    void
    eventTemp ();
private:
    MMO_CompileFlags _flags;
    bool _hybrid;
};
/**
 *
 * @param flags
 * @param hybrid
 * @return
 */
MMO_ParallelEngine
newMMO_ParallelEngine (MMO_CompileFlags flags, bool hybrid);
/**
 *
 * @param m
 */
void
deleteMMO_ParallelEngine (MMO_ParallelEngine m);

#endif  /* MMO_SOLVER_H_ */

