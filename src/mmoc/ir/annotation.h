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

typedef enum { QSS, CQSS, LIQSS, QSS2, LIQSS2, LIQSS_BDF, QSS3, LIQSS3, QSS4, DASSL, DOPRI, CVODE_BDF, CVODE_AM, IDA } Solver;

typedef enum { Metis, HMetis, Scotch, Patoh, MTPL, MTPL_IT, Manual } PartitionMethod;

typedef enum { DT_Fixed, DT_Asynchronous } DT_Synch;

class FunctionAnnotation {
  public:
  FunctionAnnotation();
  ~FunctionAnnotation() = default;
  bool hasDerivative();
  bool hasInclude();
  bool hasIncludeDirectory();
  bool hasLibraries();
  bool hasLibraryDirectory();
  bool insert(AST_Argument_Modification x);
  string derivative();
  string include();
  string includeDirectory();
  Util::SymbolTable libraries() const;
  string libraryDirectory();

  private:
  typedef enum {
    INCLUDE,            //!< INCLUDE
    INCLUDE_DIRECTORY,  //!< INCLUDE_DIRECTORY
    LIBRARY,            //!< LIBRARY
    LIBRARY_DIRECTORY,  //!< LIBRARY_DIRECTORY
    DERIVATIVE          //!< DERIVATIVE
  } type;
  map<std::string, FunctionAnnotation::type> _annotations;
  std::string _derivative;
  std::string _include;
  std::string _includeDirectory;
  Util::SymbolTable _libraries;
  std::string _libraryDirectory;
};

class ModelAnnotation {
  public:
  ModelAnnotation();
  ~ModelAnnotation() = default;
  void eventComment(AST_Comment x);
  bool insert(AST_Argument_Modification x);
  void setDesc(string desc);
  string desc();
  void setDQMin(double dqmin);
  list<double> dqmin();
  list<double> dqrel();
  void setDQRel(double dqrel);
  void setWeight(double weight);
  double weight();
  void setSolver(Solver solver);
  Solver solver();
  string solverString();
  void setInitialTime(double it);
  double initialTime();
  void setFinalTime(double ft);
  double finalTime();
  void setMinStep(double ms);
  double minStep();
  void setZCHyst(double zch);
  double ZCHyst();
  void setDerDelta(double dd);
  double derDelta();
  void setLps(int lps);
  void setDT(double dt);
  double DT();
  int lps();
  void setNodeSize(int ns);
  int nodeSize();
  void setCommInterval(string ci);
  string commInterval();
  void setSample(double s);
  list<double> sample();
  void setSymDiff(bool sd);
  bool symDiff();
  int order();
  string scheduler();
  void setScheduler(string sched);
  list<AST_Expression> output();
  void setStoreData(string save);
  string storeData();
  void setPartitionMethod(PartitionMethod pm);
  string partitionMethodString();
  PartitionMethod partitionMethod();
  void setParallel(bool p);
  bool parallel();
  int polyCoeffs();
  void setDtSynch(DT_Synch synch);
  string dtSynchString();
  DT_Synch dtSynch();
  list<string> patohSettings();
  list<string> scotchSettings();
  list<string> metisSettings();
  void setPatohSettings(string l);
  void setScotchSettings(string l);
  void setMetisSettings(string l);
  void setJacobian(int l);
  int jacobian();
  bool isClassic();
  list<AST_Expression> BDFPartition();
  int BDFPartitionDepth();
  double BDFMaxStep();

  private:
  typedef enum {
    EXPERIMENT,
    DESC,
    DQMIN,
    DQREL,
    WEIGHT,
    SOLVER,
    INITIAL_TIME,
    FINAL_TIME,
    MIN_STEP,
    ZCHYST,
    DER_DELTA,
    LPS,
    NODE_SIZE,
    COMM_INTERVAL,
    STEP_SIZE,
    SYM_DIFF,
    SCHEDULER,
    OUTPUT,
    STORE_DATA,
    PARTITION_METHOD,
    PARALLEL,
    DELTAT,
    DELTAT_SYNCH,
    PATOH_SETTINGS,
    SCOTCH_SETTINGS,
    METIS_SETTINGS,
    JACOBIAN,
    BDF_PARTITION,
    BDF_PARTITION_DEPTH,
    BDF_MAX_STEP
  } type;
  void processAnnotation(string annot, AST_Modification_Equal x);
  void processArgument(AST_Argument_Modification arg);
  void processList(AST_Expression x, list<double> *l);
  void processList(AST_Expression x, list<string> *l);
  void processExpressionList(AST_Expression x, list<AST_Expression> *l);
  Solver getSolver(string s);
  PartitionMethod getPartitionMethod(string s);
  DT_Synch getDtSynch(string s);

  Solver _solver;
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
  map<string, ModelAnnotation::type> _annotations;
  list<double> _DQMin;
  list<double> _DQRel;
  double _weight;
  list<double> _sample;
  list<AST_Expression> _output;
  double _initialTime;
  double _finalTime;
  PartitionMethod _partitionMethod;
  string _partitionMethodString;
  bool _parallel;
  double _dt;
  int _polyCoeffs;
  DT_Synch _dtSynch;
  string _dtSynchString;
  string _desc;
  list<string> _patohSettings;
  list<string> _scotchSettings;
  list<string> _metisSettings;
  int _jacobian;
  list<AST_Expression> _BDFPartition;
  int _BDFPartitionDepth;
  double _BDFMaxStep;
};

typedef boost::variant<ModelAnnotation, FunctionAnnotation> AnnotationType;

class AnnotationValue {
  public:
  AnnotationValue();
  ~AnnotationValue();
  int integer();
  void setInteger(int i);
  double real();
  void setReal(double d);
  string str();
  void setStr(string s);

  private:
  int _integer;
  double _real;
  string _str;
};

class EvalAnnotation : public AST_Expression_Fold<AnnotationValue> {
  public:
  EvalAnnotation();
  ~EvalAnnotation() = default;

  private:
  void setBoolean(bool condition, AnnotationValue *e);
  AnnotationValue foldTraverseElement(AST_Expression);
  AnnotationValue foldTraverseElement(AnnotationValue, AnnotationValue, BinOpType);
  AnnotationValue foldTraverseElementUMinus(AST_Expression);
  map<string, string> _tokens;
};
}  // namespace IR
}  // namespace MicroModelica
#endif /* MMO_ANNOTATION_H_ */
