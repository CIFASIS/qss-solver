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
#include <boost/variant/variant_fwd.hpp>

#include "../ast/ast_types.h"
#include "../util/ast_util.h"
#include "../util/util_types.h"

namespace MicroModelica {
  namespace IR {

    /**
     * @enum ANT_solver
     * @brief Model annotations solver type.
     *
     * @var ANT_solver::ANT_QSS
     * @brief QSS1 method.
     */
    typedef enum
    {
      QSS,
      CQSS,
      LIQSS,
      QSS2,
      LIQSS2,
      QSS3,
      LIQSS3,
      QSS4,
      DASSL,
      DOPRI,
      CVODE_BDF,
      CVODE_AM,
      IDA
    } Solver;

    typedef enum
    {
      Metis,    
      HMetis,   
      Scotch,   
      Patoh,    
      MTPL,     
      MTPL_IT,  
      Manual
    } PartitionMethod;

    typedef enum
    {
      DT_Fixed, 
      DT_Asynchronous 
    } DT_Synch;

    /**
     *
     */
    class FunctionAnnotation
    {
      public:
        /**
         *
         */
        FunctionAnnotation();
        /**
         *
         */
        ~FunctionAnnotation();
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
      private:
        /**
         *
         */
        typedef enum
        {
          INCLUDE,            //!< INCLUDE
          INCLUDE_DIRECTORY,  //!< INCLUDE_DIRECTORY
          LIBRARY,            //!< LIBRARY
          LIBRARY_DIRECTORY,  //!< LIBRARY_DIRECTORY
          DERIVATIVE          //!< DERIVATIVE
        } type;
        map<std::string, FunctionAnnotation::type> _annotations;
        std::string                                _derivative;
        std::string                                _include;
        std::string                                _includeDirectory;
        std::list<std::string>                     _libraries;
        std::string                                _libraryDirectory;
    };

    /**
     *
     */
    class ModelAnnotation
    {
      public:
        /**
         *
         * @param data
         */
        ModelAnnotation(Util::VarSymbolTable& symbolTable);
        /**
         *
         */
        ~ModelAnnotation();
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
        setSolver(Solver solver);
        /**
         *
         * @return
         */
        Solver
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
        setPartitionMethod(PartitionMethod pm);
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
        PartitionMethod
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
        setDtSynch(DT_Synch synch);
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
        DT_Synch
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
          EXPERIMENT,       //!< EXPERIMENT
          DESC,             //!< DESC
          DQMIN,            //!< DQMIN
          DQREL,            //!< DQREL
          WEIGHT,           //!< WEIGHT
          SOLVER,           //!< SOLVER
          INITIAL_TIME,     //!< INITIAL_TIME
          FINAL_TIME,       //!< FINAL_TIME
          MIN_STEP,         //!< MIN_STEP
          ZCHYST,           //!< ZCHYST
          DER_DELTA,        //!< DER_DELTA
          LPS,              //!< LPS
          NODE_SIZE,        //!< NODE_SIZE
          COMM_INTERVAL,    //!< COMM_INTERVAL
          STEP_SIZE,        //!< STEP_SIZE
          SYM_DIFF,         //!< SYM_DIFF
          SCHEDULER,        //!< SCHEDULER
          OUTPUT,           //!< OUTPUT
          STORE_DATA,       //!< STORE_DATA
          PARTITION_METHOD, //!< PARTITION_METHOD
          PARALLEL,         //!< PARALLEL
          DELTAT,           //!< DT
          DELTAT_SYNCH,     //!< DT_SYNCH
          PATOH_SETTINGS,   //!< PATOH_SETTINGS
          SCOTCH_SETTINGS,  //!< SCOTCH_SETTINGS
          METIS_SETTINGS,   //!< METIS_SETTINGS
          JACOBIAN          //!< JACOBIAN
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
        Solver
        _getSolver(string s);
        PartitionMethod
        _getPartitionMethod(string s);
        DT_Synch
        _getDtSynch(string s);
        Solver                _solver;
        string                _solverString;
        string                _commInterval;
        bool                  _symDiff;
        double                _minStep;
        int                   _lps;
        double                _derDelta;
        int                   _nodeSize;
        double                _ZCHyst;
        int                   _order;
        string                _scheduler;
        string                _storeData;
        map<string, ModelAnnotation::type> _annotations;
        list<double>          _DQMin;
        list<double>          _DQRel;
        double                _weight;
        list<double>          _sample;
        list<AST_Expression>  _output;
        double                _initialTime;
        double                _finalTime;
        PartitionMethod       _partitionMethod;
        string                _partitionMethodString;
        bool                  _parallel;
        double                _dt;
        int                   _polyCoeffs;
        DT_Synch              _dtSynch;
        string                _dtSynchString;
        string                _desc;
        list<string>          _patohSettings;
        list<string>          _scotchSettings;
        list<string>          _metisSettings;
        int                   _jacobian;
        Util::VarSymbolTable  _symbolTable;
    };

    typedef boost::variant<
      ModelAnnotation,
      FunctionAnnotation 
      > AnnotationType;

    /**
     *
     */
    class AnnotationValue
    {
      public:
        /**
         *
         */
        AnnotationValue();
        /**
         *
         */
        ~AnnotationValue();
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
    class EvalAnnotation: public AST_Expression_Fold<AnnotationValue>
    {
      public:
        /**
         *
         * @param st
         */
        EvalAnnotation(MicroModelica::Util::VarSymbolTable st);
        /**
         *
         */
        ~EvalAnnotation()
        {
        }
      private:
        void
        _setBoolean(bool condition, AnnotationValue *e);
        AnnotationValue
        foldTraverseElement(AST_Expression);
        AnnotationValue
        foldTraverseElement(AnnotationValue, AnnotationValue, BinOpType);
        AnnotationValue
        foldTraverseElementUMinus(AST_Expression);
        MicroModelica::Util::VarSymbolTable _st;
        map<string, string> _tokens;
    };
  }
}
#endif  /* MMO_ANNOTATION_H_ */
