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

#ifndef MMO_ANNOTATION_H_
#define MMO_ANNOTATION_H_

#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../util/ast_util.h"
#include "../util/util_types.h"
#include "mmo_base.h"
#include "mmo_types.h"

/**
 * @enum ANT_solver
 * @brief Model annotations solver type.
 *
 * @var ANT_solver::ANT_QSS
 * @brief QSS1 method.
 */
typedef enum
{
  ANT_QSS,
  ANT_CQSS,
  ANT_LIQSS,
  ANT_QSS2,
  ANT_LIQSS2,
  ANT_QSS3,
  ANT_LIQSS3,
  ANT_QSS4,
  ANT_DASSL,
  ANT_DOPRI,
  ANT_CVODE_BDF,
  ANT_CVODE_AM,
  ANT_IDA
} ANT_Solver;

typedef enum
{
  ANT_Metis, //!< ANT_Metis
  ANT_HMetis, //!< ANT_HMetis
  ANT_Scotch, //!< ANT_Scoth
  ANT_Patoh, //!< ANT_Patoh
  ANT_MTPL, //!< ANT_MTPL
  ANT_MTPL_IT, //!< ANT_MTPL_IT
  ANT_Manual
} ANT_PartitionMethod;

typedef enum
{
  ANT_DT_Fixed, //!< ANT_DT_Fixed
  ANT_DT_Asynchronous //!< ANT_DT_Asynchronous
} ANT_DT_Synch;

/**
 *
 */
class MMO_Annotation_: public MMO_Base_
{
  public:
    MMO_Annotation_();
    ~MMO_Annotation_();
    string
    print();
    virtual bool
    hasDerivative();
    virtual bool
    hasInclude();
    virtual bool
    hasIncludeDirectory();
    virtual bool
    hasLibraries();
    virtual bool
    hasLibraryDirectory();
    virtual string
    derivative();
    virtual string
    include();
    virtual string
    includeDirectory();
    virtual list<string>
    libraries();
    virtual string
    libraryDirectory();
    virtual void
    eventComment(AST_Comment x);
    virtual bool
    insert(AST_Argument_Modification x);
    virtual void
    setDesc(string desc);
    virtual string
    desc();
    virtual void
    setDQMin(double dqmin);
    virtual list<double>
    dqmin();
    virtual list<double>
    dqrel();
    virtual void
    setDQRel(double dqrel);
    virtual void
    setWeight(double weight);
    virtual double
    weight();
    virtual void
    setSolver(ANT_Solver solver);
    virtual ANT_Solver
    solver();
    virtual string
    solverString();
    virtual void
    setInitialTime(double it);
    virtual double
    initialTime();
    virtual void
    setFinalTime(double ft);
    virtual double
    finalTime();
    virtual void
    setMinStep(double ms);
    virtual double
    minStep();
    virtual void
    setZCHyst(double zch);
    virtual double
    ZCHyst();
    virtual void
    setDerDelta(double dd);
    virtual double
    derDelta();
    virtual void
    setLps(int lps);
    virtual void
    setDT(double dt);
    virtual double
    DT();
    virtual int
    lps();
    virtual void
    setNodeSize(int ns);
    virtual int
    nodeSize();
    virtual void
    setCommInterval(string ci);
    virtual string
    commInterval();
    virtual void
    setSample(double s);
    virtual list<double>
    sample();
    virtual void
    setSymDiff(bool sd);
    virtual bool
    symDiff();
    virtual int
    order();
    virtual string
    scheduler();
    virtual void
    setScheduler(string sched);
    virtual list<AST_Expression>
    output();
    virtual void
    setStoreData(string save);
    virtual string
    storeData();
    virtual void
    setPartitionMethod(ANT_PartitionMethod pm);
    virtual string
    partitionMethodString();
    virtual ANT_PartitionMethod
    partitionMethod();
    virtual void
    setParallel(bool p);
    virtual bool
    parallel();
    virtual int
    polyCoeffs();
    virtual void
    setDtSynch(ANT_DT_Synch synch);
    virtual string
    dtSynchString();
    virtual ANT_DT_Synch
    dtSynch();
    virtual list<string>
    patohSettings();
    virtual list<string>
    scotchSettings();
    virtual list<string>
    metisSettings();
    virtual void
    setPatohSettings(string l);
    virtual void
    setScotchSettings(string l);
    virtual void
    setMetisSettings(string l);
    virtual void
    setJacobian(int l) = 0;
    virtual int
    jacobian() = 0;
    virtual bool
    classic() = 0;
};

/**
 *
 */
class MMO_FunctionAnnotation_: public MMO_Annotation_
{
  public:
    /**
     *
     */
    MMO_FunctionAnnotation_();
    /**
     *
     */
    ~MMO_FunctionAnnotation_();
    /**
     *
     * @return
     */
    bool
    hasDerivative();
    /**
     *
     * @return
     */
    bool
    hasInclude();
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
    hasLibraries();
    /**
     *
     * @return
     */
    bool
    hasLibraryDirectory();
    /**
     *
     * @param x
     * @return
     */
    bool
    insert(AST_Argument_Modification x);
    /**
     *
     * @return
     */
    string
    derivative();
    /**
     *
     * @return
     */
    string
    include();
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
    list<string>
    libraries();
    /**
     *
     * @return
     */
    string
    libraryDirectory();
    void
    setJacobian(int l)
    {
    }
    ;
    int
    jacobian()
    {
      return 0;
    }
    ;
    bool
    classic()
    {
      return false;
    }
    ;
    private:
    /**
     *
     */
    typedef enum
    {
      INCLUDE,          //!< INCLUDE
      INCLUDE_DIRECTORY,          //!< INCLUDE_DIRECTORY
      LIBRARY,          //!< LIBRARY
      LIBRARY_DIRECTORY,          //!< LIBRARY_DIRECTORY
      DERIVATIVE        //!< DERIVATIVE
    } type;
    map<string, MMO_FunctionAnnotation_::type> _annotations;
    string _derivative;
    string _include;
    string _includeDirectory;
    list<string> _libraries;
    string _libraryDirectory;
};
/**
 *
 * @return
 */
MMO_FunctionAnnotation
newMMO_FunctionAnnotation();
/**
 *
 * @param m
 */
void
deleteMMO_FunctionAnnotation(MMO_FunctionAnnotation m);

/**
 *
 */
class MMO_ModelAnnotation_: public MMO_Annotation_
{
  public:
    /**
     *
     * @param data
     */
    MMO_ModelAnnotation_(MMO_ModelData data);
    /**
     *
     */
    ~MMO_ModelAnnotation_();
    /**
     *
     * @param x
     */
    void
    eventComment(AST_Comment x);
    /**
     *
     * @param x
     * @return
     */
    bool
    insert(AST_Argument_Modification x);
    /**
     *
     * @param desc
     */
    void
    setDesc(string desc);
    /**
     *
     * @return
     */
    string
    desc();
    /**
     *
     * @param dqmin
     */
    void
    setDQMin(double dqmin);
    /**
     *
     * @return
     */
    list<double>
    dqmin();
    /**
     *
     * @return
     */
    list<double>
    dqrel();
    /**
     *
     * @param dqrel
     */
    void
    setDQRel(double dqrel);
    /**
     *
     * @param weight
     */
    void
    setWeight(double weight);
    /**
     *
     * @return
     */
    double
    weight();
    /**
     *
     * @param solver
     */
    void
    setSolver(ANT_Solver solver);
    /**
     *
     * @return
     */
    ANT_Solver
    solver();
    /**
     *
     * @return
     */
    string
    solverString();
    /**
     *
     * @param it
     */
    void
    setInitialTime(double it);
    /**
     *
     * @return
     */
    double
    initialTime();
    /**
     *
     * @param ft
     */
    void
    setFinalTime(double ft);
    /**
     *
     * @return
     */
    double
    finalTime();
    /**
     *
     * @param ms
     */
    void
    setMinStep(double ms);
    /**
     *
     * @return
     */
    double
    minStep();
    /**
     *
     * @param zch
     */
    void
    setZCHyst(double zch);
    /**
     *
     * @return
     */
    double
    ZCHyst();
    /**
     *
     * @param dd
     */
    void
    setDerDelta(double dd);
    /**
     *
     * @return
     */
    double
    derDelta();
    /**
     *
     * @param lps
     */
    void
    setLps(int lps);
    /**
     *
     * @param dt
     */
    void
    setDT(double dt);
    /**
     *
     * @return
     */
    double
    DT();
    /**
     *
     * @return
     */
    int
    lps();
    /**
     *
     * @param ns
     */
    void
    setNodeSize(int ns);
    /**
     *
     * @return
     */
    int
    nodeSize();
    /**
     *
     * @param ci
     */
    void
    setCommInterval(string ci);
    /**
     *
     * @return
     */
    string
    commInterval();
    /**
     *
     * @param s
     */
    void
    setSample(double s);
    /**
     *
     * @return
     */
    list<double>
    sample();
    /**
     *
     * @param sd
     */
    void
    setSymDiff(bool sd);
    /**
     *
     * @return
     */
    bool
    symDiff();
    /**
     *
     * @return
     */
    int
    order();
    /**
     *
     * @return
     */
    string
    scheduler();
    /**
     *
     * @param sched
     */
    void
    setScheduler(string sched);
    /**
     *
     * @return
     */
    list<AST_Expression>
    output();
    /**
     *
     * @param save
     */
    void
    setStoreData(string save);
    string
    storeData();
    /**
     *
     * @param pm
     */
    void
    setPartitionMethod(ANT_PartitionMethod pm);
    /**
     *
     * @return
     */
    string
    partitionMethodString();
    /**
     *
     * @return
     */
    ANT_PartitionMethod
    partitionMethod();
    /**
     *
     * @param p
     */
    void
    setParallel(bool p);
    /**
     *
     * @return
     */
    bool
    parallel();
    /**
     *
     * @return
     */
    int
    polyCoeffs();
    /**
     *
     * @param synch
     */
    void
    setDtSynch(ANT_DT_Synch synch);
    /**
     *
     * @return
     */
    string
    dtSynchString();
    /**
     *
     * @return
     */
    ANT_DT_Synch
    dtSynch();
    list<string>
    patohSettings();
    list<string>
    scotchSettings();
    list<string>
    metisSettings();
    void
    setPatohSettings(string l);
    void
    setScotchSettings(string l);
    void
    setMetisSettings(string l);
    void
    setJacobian(int l);
    int
    jacobian();
    bool
    classic();
    private:
    /**
     *
     */
    typedef enum
    {
      EXPERIMENT,   //!< EXPERIMENT
      DESC,         //!< DESC
      DQMIN,        //!< DQMIN
      DQREL,        //!< DQREL
      WEIGHT,       //!< WEIGHT
      SOLVER,       //!< SOLVER
      INITIAL_TIME, //!< INITIAL_TIME
      FINAL_TIME,   //!< FINAL_TIME
      MIN_STEP,     //!< MIN_STEP
      ZCHYST,       //!< ZCHYST
      DER_DELTA,    //!< DER_DELTA
      LPS,          //!< LPS
      NODE_SIZE,    //!< NODE_SIZE
      COMM_INTERVAL,    //!< COMM_INTERVAL
      STEP_SIZE,    //!< STEP_SIZE
      SYM_DIFF,     //!< SYM_DIFF
      SCHEDULER,    //!< SCHEDULER
      OUTPUT,       //!< OUTPUT
      STORE_DATA,    //!< STORE_DATA
      PARTITION_METHOD, //!< PARTITION_METHOD
      PARALLEL, //!< PARALLEL
      DELTAT, //!< DT
      DELTAT_SYNCH, //!< DT_SYNCH
      PATOH_SETTINGS, //!< PATOH_SETTINGS
      SCOTCH_SETTINGS, //!< SCOTCH_SETTINGS
      METIS_SETTINGS, //!< METIS_SETTINGS
      JACOBIAN //!< JACOBIAN
    } type;
    void
    _processAnnotation(string annot, AST_Modification_Equal x);
    void
    _processArgument(AST_Argument_Modification arg);
    void
    _processList(AST_Expression x, list<double> *l);
    void
    _processList(AST_Expression x, list<string> *l);
    void
    _processExpressionList(AST_Expression x, list<AST_Expression> *l);
    ANT_Solver
    _getSolver(string s);
    ANT_PartitionMethod
    _getPartitionMethod(string s);
    ANT_DT_Synch
    _getDtSynch(string s);
    ANT_Solver _solver;
    string _solverString;
    string _commInterval;
    bool _symDiff;
    double _minStep;
    int _lps;
    double _derDelta;
    int _nodeSize;
    double _ZCHyst;
    int _order;
    string _scheduler;
    string _storeData;
    map<string, MMO_ModelAnnotation_::type> _annotations;
    MMO_ModelData _data;
    list<double> _DQMin;
    list<double> _DQRel;
    double _weight;
    list<double> _sample;
    list<AST_Expression> _output;
    double _initialTime;
    double _finalTime;
    ANT_PartitionMethod _partitionMethod;
    string _partitionMethodString;
    bool _parallel;
    double _dt;
    int _polyCoeffs;
    ANT_DT_Synch _dtSynch;
    string _dtSynchString;
    string _desc;
    list<string> _patohSettings;
    list<string> _scotchSettings;
    list<string> _metisSettings;
    int _jacobian;
};
/**
 *
 * @param data
 * @return
 */
MMO_ModelAnnotation
newMMO_ModelAnnotation(MMO_ModelData data);
/**
 *
 * @param m
 */
void
deleteMMO_ModelAnnotation(MMO_ModelAnnotation m);

/**
 *
 */
class MMO_AnnotationValue
{
  public:
    /**
     *
     */
    MMO_AnnotationValue();
    /**
     *
     */
    ~MMO_AnnotationValue();
    /**
     *
     * @return
     */
    int
    integer();
    /**
     *
     * @param i
     */
    void
    setInteger(int i);
    /**
     *
     * @return
     */
    double
    real();
    /**
     *
     * @param d
     */
    void
    setReal(double d);
    /**
     *
     * @return
     */
    string
    str();
    /**
     *
     * @param s
     */
    void
    setStr(string s);
    private:
    int _integer;
    double _real;
    string _str;
};

/**
 *
 */
class MMO_EvalAnnotation_: public AST_Expression_Fold<MMO_AnnotationValue>
{
  public:
    /**
     *
     * @param st
     */
    MMO_EvalAnnotation_(VarSymbolTable st);
    /**
     *
     */
    ~MMO_EvalAnnotation_()
    {
    }
    ;
    private:
    void
    _setBoolean(bool condition, MMO_AnnotationValue *e);
    MMO_AnnotationValue
    foldTraverseElement(AST_Expression);
    MMO_AnnotationValue
    foldTraverseElement(MMO_AnnotationValue, MMO_AnnotationValue,
        BinOpType);
    MMO_AnnotationValue
    foldTraverseElementUMinus(AST_Expression);
    VarSymbolTable _st;
    map<string, string> _tokens;
};
/**
 *
 * @param st
 * @return
 */
MMO_EvalAnnotation
newMMO_EvalAnnotation(VarSymbolTable st);
/**
 *
 * @param m
 */
void
deleteMMO_EvalAnnotation(MMO_EvalAnnotation m);

#endif  /* MMO_ANNOTATION_H_ */
