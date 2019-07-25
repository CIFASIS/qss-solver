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

#include "annotation.h"

#include <utility>

#include "../ast/element.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/util.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

FunctionAnnotation::FunctionAnnotation() : _annotations(), _derivative(), _include()
{
  _libraryDirectory = Utils::instance().environmentVariable("MMOC_LIBRARIES");
  _includeDirectory = Utils::instance().environmentVariable("MMOC_INCLUDE");
  _annotations.insert(pair<string, FunctionAnnotation::type>("derivative", DERIVATIVE));
  _annotations.insert(pair<string, FunctionAnnotation::type>("Include", INCLUDE));
  _annotations.insert(pair<string, FunctionAnnotation::type>("IncludeDirectory", INCLUDE_DIRECTORY));
  _annotations.insert(pair<string, FunctionAnnotation::type>("Library", LIBRARY));
  _annotations.insert(pair<string, FunctionAnnotation::type>("LibraryDirectory", LIBRARY_DIRECTORY));
}

bool FunctionAnnotation::hasDerivative() { return !_derivative.empty(); }

bool FunctionAnnotation::hasInclude() { return !_include.empty(); }

bool FunctionAnnotation::hasIncludeDirectory() { return !_includeDirectory.empty(); }

bool FunctionAnnotation::hasLibraries() { return !_libraries.empty(); }

bool FunctionAnnotation::hasLibraryDirectory() { return !_libraryDirectory.empty(); }

bool FunctionAnnotation::insert(AST_Argument_Modification x)
{
  string annot = *(x->name());
  map<string, FunctionAnnotation::type>::const_iterator itf = _annotations.find(annot);
  if (itf == _annotations.end()) {
    return false;
  }
  AST_Expression mod = NULL;
  if (x->hasModification()) {
    if (x->modification()->modificationType() == MODEQUAL) {
      mod = x->modification()->getAsEqual()->exp();
    }
  }
  switch (itf->second) {
  case INCLUDE:
    if (mod->expressionType() == EXPSTRING) {
      _include = mod->getAsString()->str();
    }
    break;
  case INCLUDE_DIRECTORY:
    if (mod->expressionType() == EXPSTRING) {
      _includeDirectory = mod->getAsString()->str();
    }
    break;
  case LIBRARY:
    if (mod->expressionType() == EXPSTRING) {
      string l = mod->getAsString()->str();
      _libraries.insert(l, l);
    } else if (mod->expressionType() == EXPBRACE) {
      AST_ExpressionList el = mod->getAsBrace()->arguments();
      AST_ExpressionListIterator eli;
      foreach (eli, el) {
        string l = current_element(eli)->getAsString()->str();
        _libraries.insert(l, l);
      }
    }
    break;
  case LIBRARY_DIRECTORY:
    if (mod->expressionType() == EXPSTRING) {
      _libraryDirectory = mod->getAsString()->str();
    }
    break;
  case DERIVATIVE:
    if (mod->expressionType() == EXPSTRING) {
      _derivative = mod->getAsString()->str();
    }
    break;
  default:
    break;
  }
  return true;
}

string FunctionAnnotation::derivative() { return _derivative; }

string FunctionAnnotation::include() { return _include; }

string FunctionAnnotation::includeDirectory() { return _includeDirectory; }

SymbolTable FunctionAnnotation::libraries() const { return _libraries; }

string FunctionAnnotation::libraryDirectory() { return _libraryDirectory; }

ModelAnnotation::ModelAnnotation(VarSymbolTable &symbolTable)
    : _solver(LIQSS2),
      _solverString("LIQSS2"),
      _commInterval("CI_Step"),
      _symDiff(true),
      _minStep(1e-14),
      _lps(1),
      _derDelta(1e-8),
      _nodeSize(10000),
      _ZCHyst(1e-12),
      _order(1),
      _scheduler("ST_Binary"),
      _storeData("SD_Memory"),
      _annotations(),
      _DQMin(),
      _DQRel(),
      _weight(-1),
      _sample(),
      _output(),
      _initialTime(0),
      _finalTime(0),
      _partitionMethod(Metis),
      _partitionMethodString("Metis"),
      _parallel(false),
      _dt(0),
      _polyCoeffs(1),
      _dtSynch(DT_Fixed),
      _dtSynchString("SD_DT_Asynchronous"),
      _desc(),
      _patohSettings(),
      _scotchSettings(),
      _metisSettings(),
      _jacobian(0),
      _symbolTable(symbolTable)
{
  _annotations.insert(pair<string, ModelAnnotation::type>("experiment", EXPERIMENT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Description", DESC));
  _annotations.insert(pair<string, ModelAnnotation::type>("Tolerance", DQREL));
  _annotations.insert(pair<string, ModelAnnotation::type>("AbsTolerance", DQMIN));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Weight", WEIGHT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Solver", SOLVER));
  _annotations.insert(pair<string, ModelAnnotation::type>("StartTime", INITIAL_TIME));
  _annotations.insert(pair<string, ModelAnnotation::type>("StopTime", FINAL_TIME));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_MinStep", MIN_STEP));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_ZCHyst", ZCHYST));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DerDelta", DER_DELTA));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_LPS", LPS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_NodeSize", NODE_SIZE));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_OutputType", COMM_INTERVAL));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Period", STEP_SIZE));
  _annotations.insert(pair<string, ModelAnnotation::type>("Jacobian", JACOBIAN));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_SymDiff", SYM_DIFF));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Scheduler", SCHEDULER));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Output", OUTPUT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_StoreData", STORE_DATA));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_PartitionMethod", PARTITION_METHOD));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Parallel", PARALLEL));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DT_Min", DELTAT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DT_Synch", DELTAT_SYNCH));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_PatohSettings", PATOH_SETTINGS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_MetisSettings", METIS_SETTINGS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_ScotchSettings", SCOTCH_SETTINGS));
  _sample.push_back(1e-2);
  _DQMin.push_back(1e-3);
  _DQRel.push_back(1e-3);
}

void ModelAnnotation::eventComment(AST_Comment x)
{
  _weight = -1;
  AST_ArgumentList al = x->arguments();
  AST_ArgumentListIterator it;
  foreach (it, al) {
    AST_Argument a = current_element(it);
    if (a->argumentType() == AR_MODIFICATION) {
      insert(a->getAsModification());
    }
  }
}

void ModelAnnotation::processArgument(AST_Argument_Modification arg)
{
  if (arg->hasModification()) {
    AST_Modification m = arg->modification();
    if (m->modificationType() == MODEQUAL) {
      processAnnotation(*(arg->name()), m->getAsEqual());
    } else {
      Error::instance().add(m->lineNum(), EM_IR | EM_ANNOTATION_TYPE, ER_Error, "%s", arg->name()->c_str());
    }
  } else {
    Error::instance().add(arg->lineNum(), EM_IR | EM_ANNOTATION_TYPE, ER_Error, "%s", arg->name()->c_str());
  }
}

bool ModelAnnotation::insert(AST_Argument_Modification x)
{
  string annot = *(x->name());
  map<string, ModelAnnotation::type>::const_iterator itf = _annotations.find(annot);
  if (itf == _annotations.end()) {
    return false;
  }
  switch (itf->second) {
  case EXPERIMENT: {
    if (x->hasModification()) {
      AST_Modification mod = x->modification();
      if (mod->modificationType() == MODCLASS) {
        AST_Modification_Class mc = mod->getAsClass();
        AST_ArgumentListIterator it;
        foreach (it, mc->arguments()) {
          if (current_element(it)->argumentType() == AR_MODIFICATION) {
            processArgument(current_element(it)->getAsModification());
          } else {
            Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_TYPE, ER_Error, "%s", x->name()->c_str());
          }
        }
      } else {
        Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error, "Missing modification arguments. %s",
                              x->name()->c_str());
      }
    } else {
      Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Warning, "%s", x->name()->c_str());
    }
  } break;
  case WEIGHT:
    processArgument(x);
    break;
  default:
    break;
  }
  return true;
}

void ModelAnnotation::processList(AST_Expression x, list<double> *l)
{
  l->clear();
  EvalAnnotation ea(_symbolTable);
  AnnotationValue av;
  if (x->expressionType() == EXPBRACE) {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      av = ea.apply(current_element(it));
      l->push_back(av.real());
    }
  } else {
    av = ea.apply(x);
    l->push_back(av.real());
  }
}

void ModelAnnotation::processList(AST_Expression x, list<string> *l)
{
  l->clear();
  EvalAnnotation ea(_symbolTable);
  AnnotationValue av;
  if (x->expressionType() == EXPBRACE) {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      av = ea.apply(current_element(it));
      l->push_back(av.str());
    }
  } else {
    av = ea.apply(x);
    l->push_back(av.str());
  }
}

void ModelAnnotation::processExpressionList(AST_Expression x, list<AST_Expression> *l)
{
  if (x->expressionType() == EXPBRACE) {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      l->push_back(current_element(it));
    }
  }
}

PartitionMethod ModelAnnotation::partitionMethod() { return _partitionMethod; }

DT_Synch ModelAnnotation::getDtSynch(string s)
{
  if (!s.compare("SD_DT_Fixed")) {
    return DT_Fixed;
  } else if (!s.compare("SD_DT_Asynchronous")) {
    return DT_Asynchronous;
  }
  return DT_Fixed;
}

PartitionMethod ModelAnnotation::getPartitionMethod(string s)
{
  if (!s.compare("Metis")) {
    return Metis;
  } else if (!s.compare("HMetis")) {
    return HMetis;
  } else if (!s.compare("Scotch")) {
    return Scotch;
  } else if (!s.compare("Patoh")) {
    return Patoh;
  } else if (!s.compare("MTPL")) {
    return MTPL;
  } else if (!s.compare("MTPL_IT")) {
    return MTPL_IT;
  } else if (!s.compare("Manual")) {
    return Manual;
  }
  return Metis;
}

Solver ModelAnnotation::getSolver(string s)
{
  if (!s.compare("QSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return QSS;
  } else if (!s.compare("CQSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return CQSS;
  } else if (!s.compare("LIQSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return LIQSS;
  } else if (!s.compare("QSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return QSS2;
  } else if (!s.compare("LIQSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return LIQSS2;
  } else if (!s.compare("QSS3")) {
    _order = 3;
    _polyCoeffs = 4;
    return QSS3;
  } else if (!s.compare("LIQSS3")) {
    _order = 3;
    _polyCoeffs = 4;
    return LIQSS3;
  } else if (!s.compare("DASSL")) {
    _order = 1;
    _polyCoeffs = 1;
    return DASSL;
  } else if (!s.compare("DOPRI")) {
    _order = 1;
    _polyCoeffs = 1;
    return DOPRI;
  } else if (!s.compare("CVODE_BDF")) {
    _order = 1;
    _polyCoeffs = 1;
    return CVODE_BDF;
  } else if (!s.compare("IDA")) {
    _order = 1;
    _polyCoeffs = 1;
    return IDA;
  } else if (!s.compare("CVODE_AM")) {
    _order = 1;
    _polyCoeffs = 1;
    return CVODE_AM;
  } else if (!s.compare("QSS4")) {
    _order = 4;
    _polyCoeffs = 5;
    return QSS4;
  }
  return QSS;
}

void ModelAnnotation::processAnnotation(string annot, AST_Modification_Equal x)
{
  map<string, ModelAnnotation::type>::const_iterator itf = _annotations.find(annot);
  if (itf == _annotations.end()) {
    Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Warning, "%s", annot.c_str());
  }
  EvalAnnotation ea(_symbolTable);
  AnnotationValue av;
  if (itf->second != DQMIN && itf->second != DQREL && itf->second != STEP_SIZE) {
    av = ea.apply(x->exp());
  }
  switch (itf->second) {
  case DESC:
    _desc = av.str();
    break;
  case DQMIN:
    processList(x->exp(), &_DQMin);
    break;
  case DQREL:
    processList(x->exp(), &_DQRel);
    break;
  case WEIGHT:
    _weight = av.real();
    break;
  case SOLVER:
    _solver = getSolver(av.str());
    _solverString = av.str();
    break;
  case INITIAL_TIME:
    _initialTime = av.real();
    break;
  case FINAL_TIME:
    _finalTime = av.real();
    break;
  case MIN_STEP:
    _minStep = av.real();
    break;
  case ZCHYST:
    _ZCHyst = av.real();
    break;
  case DER_DELTA:
    _derDelta = av.real();
    break;
  case LPS:
    _lps = av.integer();
    break;
  case NODE_SIZE:
    _nodeSize = av.integer();
    break;
  case COMM_INTERVAL:
    _commInterval = av.str();
    break;
  case STEP_SIZE:
    processList(x->exp(), &_sample);
    break;
  case SCHEDULER:
    _scheduler = av.str();
    break;
  case JACOBIAN:
    _jacobian = ("Sparse" == av.str() ? 0 : 1);
    break;
  case SYM_DIFF:
    _symDiff = true;
    if (av.integer() == 0) {
      _symDiff = false;
    }
    break;
  case OUTPUT:
    processExpressionList(x->exp(), &_output);
    break;
  case PARTITION_METHOD:
    _partitionMethod = getPartitionMethod(av.str());
    _partitionMethodString = av.str();
    break;
  case DELTAT_SYNCH:
    _dtSynch = getDtSynch(av.str());
    _dtSynchString = av.str();
    break;
  case PARALLEL:
    _parallel = true;
    if (av.integer() == 0) {
      _parallel = false;
    }
    break;
  case DELTAT:
    _dt = av.real();
    break;
  case STORE_DATA:
    break;
  case PATOH_SETTINGS:
    processList(x->exp(), &_patohSettings);
    break;
  case SCOTCH_SETTINGS:
    processList(x->exp(), &_scotchSettings);
    break;
  case METIS_SETTINGS:
    processList(x->exp(), &_metisSettings);
    break;
  default:
    break;
  }
}

void ModelAnnotation::setDesc(string desc) { _desc = desc; }

string ModelAnnotation::desc() { return _desc; }

void ModelAnnotation::setDQMin(double dqmin) { _DQMin.push_back(dqmin); }

list<double> ModelAnnotation::dqmin() { return _DQMin; }

void ModelAnnotation::setDT(double dt) { _dt = dt; }

double ModelAnnotation::DT() { return _dt; }

list<double> ModelAnnotation::dqrel() { return _DQRel; }

void ModelAnnotation::setDQRel(double dqrel) { _DQRel.push_back(dqrel); }

void ModelAnnotation::setWeight(double weight) { _weight = weight; }

double ModelAnnotation::weight() { return _weight; }

void ModelAnnotation::setSolver(Solver solver) { _solver = solver; }

string ModelAnnotation::solverString() { return _solverString; }

Solver ModelAnnotation::solver() { return _solver; }

void ModelAnnotation::setPartitionMethod(PartitionMethod pm) { _partitionMethod = pm; }

string ModelAnnotation::partitionMethodString() { return _partitionMethodString; }

DT_Synch ModelAnnotation::dtSynch() { return _dtSynch; }

string ModelAnnotation::dtSynchString() { return _dtSynchString; }

void ModelAnnotation::setDtSynch(DT_Synch synch) { _dtSynch = synch; }

void ModelAnnotation::setInitialTime(double it) { _initialTime = it; }

double ModelAnnotation::initialTime() { return _initialTime; }

void ModelAnnotation::setFinalTime(double ft) { _finalTime = ft; }

double ModelAnnotation::finalTime() { return _finalTime; }

void ModelAnnotation::setMinStep(double ms) { _minStep = ms; }

double ModelAnnotation::minStep() { return _minStep; }

void ModelAnnotation::setZCHyst(double zch) { _ZCHyst = zch; }

double ModelAnnotation::ZCHyst() { return _ZCHyst; }

void ModelAnnotation::setDerDelta(double dd) { _derDelta = dd; }

double ModelAnnotation::derDelta() { return _derDelta; }

void ModelAnnotation::setStoreData(string store) { _storeData = store; }

string ModelAnnotation::storeData() { return _storeData; }

void ModelAnnotation::setLps(int lps) { _lps = lps; }

void ModelAnnotation::setJacobian(int jacobian) { _jacobian = jacobian; }

int ModelAnnotation::jacobian() { return _jacobian; }

bool ModelAnnotation::classic()
{
  return _solver == DASSL || _solver == DOPRI || _solver == CVODE_BDF || _solver == IDA || _solver == CVODE_AM;
}

int ModelAnnotation::lps() { return _lps; }

void ModelAnnotation::setNodeSize(int ns) { _nodeSize = ns; }

int ModelAnnotation::nodeSize() { return _nodeSize; }

void ModelAnnotation::setCommInterval(string ci) { _commInterval = ci; }

string ModelAnnotation::commInterval() { return _commInterval; }

void ModelAnnotation::setSample(double s) { _sample.push_back(s); }

list<double> ModelAnnotation::sample() { return _sample; }

void ModelAnnotation::setScheduler(string sched) { _scheduler = sched; }

string ModelAnnotation::scheduler() { return _scheduler; }

void ModelAnnotation::setSymDiff(bool sd) { _symDiff = sd; }

bool ModelAnnotation::symDiff() { return _symDiff; }

int ModelAnnotation::order() { return _order; }

list<AST_Expression> ModelAnnotation::output() { return _output; }

list<string> ModelAnnotation::patohSettings() { return _patohSettings; }

list<string> ModelAnnotation::scotchSettings() { return _scotchSettings; }

list<string> ModelAnnotation::metisSettings() { return _metisSettings; }

void ModelAnnotation::setPatohSettings(string l) { _patohSettings.push_back(l); }

void ModelAnnotation::setScotchSettings(string l) { _scotchSettings.push_back(l); }

void ModelAnnotation::setMetisSettings(string l) { _metisSettings.push_back(l); }

/* AnnotationValue class */

AnnotationValue::AnnotationValue() : _integer(0), _real(0), _str("") {}

AnnotationValue::~AnnotationValue() {}

int AnnotationValue::integer() { return _integer; }

void AnnotationValue::setInteger(int i) { _integer = i; }

double AnnotationValue::real() { return _real; }

void AnnotationValue::setReal(double d) { _real = d; }

string AnnotationValue::str() { return _str; }

void AnnotationValue::setStr(string s) { _str = s; }

/* EvalAnnotation class */

EvalAnnotation::EvalAnnotation(VarSymbolTable st) : _st(st), _tokens()
{
  _tokens.insert(pair<string, string>("QSS", "QSS"));
  _tokens.insert(pair<string, string>("CQSS", "CQSS"));
  _tokens.insert(pair<string, string>("QSS2", "QSS2"));
  _tokens.insert(pair<string, string>("QSS3", "QSS3"));
  _tokens.insert(pair<string, string>("LIQSS", "LIQSS"));
  _tokens.insert(pair<string, string>("LIQSS2", "LIQSS2"));
  _tokens.insert(pair<string, string>("LIQSS3", "LIQSS3"));
  _tokens.insert(pair<string, string>("QSS4", "QSS4"));
  _tokens.insert(pair<string, string>("DASSL", "DASSL"));
  _tokens.insert(pair<string, string>("DOPRI", "DOPRI"));
  _tokens.insert(pair<string, string>("CVODE_AM", "CVODE_AM"));
  _tokens.insert(pair<string, string>("IDA", "IDA"));
  _tokens.insert(pair<string, string>("CVODE_BDF", "CVODE_BDF"));
  _tokens.insert(pair<string, string>("ST_Linear", "ST_Linear"));
  _tokens.insert(pair<string, string>("ST_Binary", "ST_Binary"));
  _tokens.insert(pair<string, string>("ST_Random", "ST_Random"));
  _tokens.insert(pair<string, string>("CI_Step", "CI_Step"));
  _tokens.insert(pair<string, string>("CI_Dense", "CI_Dense"));
  _tokens.insert(pair<string, string>("CI_Sampled", "CI_Sampled"));
  _tokens.insert(pair<string, string>("SD_File", "SD_File"));
  _tokens.insert(pair<string, string>("SD_Memory", "SD_Memory"));
  _tokens.insert(pair<string, string>("Metis", "Metis"));
  _tokens.insert(pair<string, string>("HMetis", "HMetis"));
  _tokens.insert(pair<string, string>("Scotch", "Scotch"));
  _tokens.insert(pair<string, string>("Patoh", "Patoh"));
  _tokens.insert(pair<string, string>("MTPL", "MTPL"));
  _tokens.insert(pair<string, string>("MTPL_IT", "MTPL_IT"));
  _tokens.insert(pair<string, string>("Manual", "Manual"));
  _tokens.insert(pair<string, string>("SD_DT_Fixed", "SD_DT_Fixed"));
  _tokens.insert(pair<string, string>("Sparse", "Sparse"));
  _tokens.insert(pair<string, string>("Dense", "Dense"));
  _tokens.insert(pair<string, string>("SD_DT_Asynchronous", "SD_DT_Asynchronous"));
}

AnnotationValue EvalAnnotation::foldTraverseElement(AST_Expression e)
{
  AnnotationValue av = AnnotationValue();
  switch (e->expressionType()) {
  case EXPSTRING:
    av.setStr(e->getAsString()->print());
    break;
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    string name = cr->name();
    Option<Variable> vi = _st[name];
    if (vi) {
      if (vi->isConstant()) {
        av.setInteger(vi->value());
      }
    } else {
      if (_tokens.find(name) != _tokens.end()) {
        av.setStr(name);
      }
    }
  } break;
  case EXPINTEGER:
    av.setInteger(e->getAsInteger()->val());
    av.setReal(e->getAsInteger()->val());
    break;
  case EXPREAL:
    av.setReal(e->getAsReal()->val());
    av.setInteger(e->getAsReal()->val());
    break;
  case EXPBOOLEAN:
    if (e->getAsBoolean()->value()) {
      av.setInteger(1);
    } else {
      av.setInteger(0);
    }
    break;
  case EXPBOOLEANNOT: {
    AnnotationValue a = apply(e->getAsBooleanNot()->exp());
    if (a.integer() == 1) {
      av.setInteger(0);
    } else {
      av.setInteger(1);
    }
    break;
  }
  default:
    break;
  }
  return av;
}

void EvalAnnotation::setBoolean(bool condition, AnnotationValue *e)
{
  if (condition) {
    e->setInteger(1);
  } else {
    e->setInteger(0);
  }
}

AnnotationValue EvalAnnotation::foldTraverseElement(AnnotationValue e1, AnnotationValue e2, BinOpType bot)
{
  AnnotationValue av = AnnotationValue();
  switch (bot) {
  case BINOPOR:
    setBoolean(e1.integer() || e2.integer(), &av);
    break;
  case BINOPAND:
    setBoolean(e1.integer() && e2.integer(), &av);
    break;
  case BINOPLOWER:
    setBoolean((e1.integer() < e2.integer()) || (e1.real() < e2.real()), &av);
    break;
  case BINOPLOWEREQ:
    setBoolean((e1.integer() <= e2.integer()) || (e1.real() <= e2.real()), &av);
    break;
  case BINOPGREATER:
    setBoolean((e1.integer() > e2.integer()) || (e1.real() > e2.real()), &av);
    break;
  case BINOPGREATEREQ:
    setBoolean((e1.integer() >= e2.integer()) || (e1.real() >= e2.real()), &av);
    break;
  case BINOPCOMPNE:
    setBoolean((e1.integer() != e2.integer()) || (e1.real() != e2.real()), &av);
    break;
  case BINOPCOMPEQ:
    setBoolean((e1.integer() == e2.integer()) || (e1.real() == e2.real()), &av);
    break;
  case BINOPADD:
    av.setInteger(e1.integer() + e2.integer());
    av.setReal(e1.real() + e2.real());
    break;
  case BINOPSUB:
    av.setInteger(e1.integer() - e2.integer());
    av.setReal(e1.real() - e2.real());
    break;
  case BINOPDIV:
    if (e2.integer() != 0) {
      av.setInteger(e1.integer() / e2.integer());
    }
    if (e2.real() != 0) {
      av.setReal(e1.real() / e2.real());
    }
    break;
  case BINOPMULT:
    av.setInteger(e1.integer() * e2.integer());
    av.setReal(e1.real() * e2.real());
    break;
  default:
    break;
  }
  return av;
}

AnnotationValue EvalAnnotation::foldTraverseElementUMinus(AST_Expression e)
{
  AnnotationValue av = foldTraverseElement(e);
  av.setInteger(-1 * av.integer());
  av.setReal(-1 * av.real());
  return av;
}

void ModelAnnotation::setParallel(bool p) { _parallel = p; }

bool ModelAnnotation::parallel() { return _parallel; }

int ModelAnnotation::polyCoeffs() { return _polyCoeffs; }
}  // namespace IR
}  // namespace MicroModelica
