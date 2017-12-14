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
#include "mmo_util.h"

MMO_Annotation_::MMO_Annotation_()
{

}

MMO_Annotation_::~MMO_Annotation_()
{
}

string
MMO_Annotation_::print()
{
  string ret;
  return ret;
}

MMO_FunctionAnnotation_::MMO_FunctionAnnotation_() :
    _annotations(), _derivative(), _include()
{
  _libraryDirectory = Util::getInstance()->environmentVariable(
      "MMOC_LIBRARIES");
  _includeDirectory = Util::getInstance()->environmentVariable("MMOC_INCLUDE");
  _annotations.insert(
      pair<string, MMO_FunctionAnnotation_::type>("derivative", DERIVATIVE));
  _annotations.insert(
      pair<string, MMO_FunctionAnnotation_::type>("Include", INCLUDE));
  _annotations.insert(
      pair<string, MMO_FunctionAnnotation_::type>("IncludeDirectory",
          INCLUDE_DIRECTORY));
  _annotations.insert(
      pair<string, MMO_FunctionAnnotation_::type>("Library", LIBRARY));
  _annotations.insert(
      pair<string, MMO_FunctionAnnotation_::type>("LibraryDirectory",
          LIBRARY_DIRECTORY));
}

MMO_FunctionAnnotation_::~MMO_FunctionAnnotation_()
{
}

bool
MMO_FunctionAnnotation_::hasDerivative()
{
  return !_derivative.empty();
}

bool
MMO_FunctionAnnotation_::hasInclude()
{
  return !_include.empty();
}

bool
MMO_FunctionAnnotation_::hasIncludeDirectory()
{
  return !_includeDirectory.empty();
}

bool
MMO_FunctionAnnotation_::hasLibraries()
{
  return !_libraries.empty();
}

bool
MMO_FunctionAnnotation_::hasLibraryDirectory()
{
  return !_libraryDirectory.empty();
}

bool
MMO_FunctionAnnotation_::insert(AST_Argument_Modification x)
{
  string annot = *(x->name());
  map<string, MMO_FunctionAnnotation_::type>::iterator itf = _annotations.find(
      annot);
  if(itf == _annotations.end())
  {
    return false;
  }
  AST_Expression mod = NULL;
  if(x->hasModification())
  {
    if(x->modification()->modificationType() == MODEQUAL)
    {
      mod = x->modification()->getAsEqual()->exp();
    }
  }
  switch(itf->second)
  {
    case INCLUDE:
      if(mod->expressionType() == EXPSTRING)
      {
        _include = mod->getAsString()->str();
      }
      break;
    case INCLUDE_DIRECTORY:
      if(mod->expressionType() == EXPSTRING)
      {
        _includeDirectory = mod->getAsString()->str();
      }
      break;
    case LIBRARY:
      if(mod->expressionType() == EXPSTRING)
      {
        _libraries.push_back(mod->getAsString()->str());
      }
      else if(mod->expressionType() == EXPBRACE)
      {
        AST_ExpressionList el = mod->getAsBrace()->arguments();
        AST_ExpressionListIterator eli;
        foreach(eli,el)
        {
          _libraries.push_back(
          current_element(eli)->getAsString()->str());
        }
      }
      break;
    case LIBRARY_DIRECTORY:
      if(mod->expressionType() == EXPSTRING)
      {
        _libraryDirectory = mod->getAsString()->str();
      }
      break;
    case DERIVATIVE:
      if(mod->expressionType() == EXPSTRING)
      {
        _derivative = mod->getAsString()->str();
      }
      break;
    default:
      break;
  }
  return true;
}

string
MMO_FunctionAnnotation_::derivative()
{
  return _derivative;
}

string
MMO_FunctionAnnotation_::include()
{
  return _include;
}

string
MMO_FunctionAnnotation_::includeDirectory()
{
  return _includeDirectory;
}

list<string>
MMO_FunctionAnnotation_::libraries()
{
  return _libraries;
}

string
MMO_FunctionAnnotation_::libraryDirectory()
{
  return _libraryDirectory;
}

MMO_FunctionAnnotation
newMMO_FunctionAnnotation()
{
  return new MMO_FunctionAnnotation_();
}

void
deleteMMO_FunctionAnnotation(MMO_FunctionAnnotation m)
{
  delete m;
}

MMO_ModelAnnotation_::MMO_ModelAnnotation_(MMO_ModelData data) :
    _solver(ANT_LIQSS2), _solverString("LIQSS2"), _commInterval("CI_Step"), _symDiff(
        true), _minStep(1e-14), _lps(1), _derDelta(1e-8), _nodeSize(
        10000), _ZCHyst(1e-12), _order(1), _scheduler("ST_Binary"), _storeData(
        "SD_Memory"), _annotations(), _data(data), _DQMin(), _DQRel(), _weight(
        -1), _sample(), _output(), _initialTime(0), _finalTime(0), _partitionMethod(
        ANT_Metis), _partitionMethodString("Metis"), _parallel(
        false), _dt(0), _polyCoeffs(1), _dtSynch(ANT_DT_Fixed), _dtSynchString(
        "SD_DT_Asynchronous"), _desc(), _patohSettings(), _scotchSettings(), _metisSettings(), _jacobian(
        0)
{
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("experiment", EXPERIMENT));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Description", DESC));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("Tolerance", DQREL));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("AbsTolerance", DQMIN));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Weight", WEIGHT));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Solver", SOLVER));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("StartTime", INITIAL_TIME));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("StopTime", FINAL_TIME));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_MinStep", MIN_STEP));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_ZCHyst", ZCHYST));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_DerDelta", DER_DELTA));
  _annotations.insert(pair<string, MMO_ModelAnnotation_::type>("MMO_LPS", LPS));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_NodeSize", NODE_SIZE));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_OutputType",
          COMM_INTERVAL));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Period", STEP_SIZE));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("Jacobian", JACOBIAN));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_SymDiff", SYM_DIFF));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Scheduler", SCHEDULER));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Output", OUTPUT));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_StoreData", STORE_DATA));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_PartitionMethod",
          PARTITION_METHOD));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_Parallel", PARALLEL));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_DT_Min", DELTAT));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_DT_Synch", DELTAT_SYNCH));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_PatohSettings",
          PATOH_SETTINGS));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_MetisSettings",
          METIS_SETTINGS));
  _annotations.insert(
      pair<string, MMO_ModelAnnotation_::type>("MMO_ScotchSettings",
          SCOTCH_SETTINGS));
  _sample.push_back(1e-2);
  _DQMin.push_back(1e-3);
  _DQRel.push_back(1e-3);
}

MMO_ModelAnnotation_::~MMO_ModelAnnotation_()
{
}

void
MMO_ModelAnnotation_::eventComment(AST_Comment x)
{
  _weight = -1;
  AST_ArgumentList al = x->arguments();
  AST_ArgumentListIterator it;
  foreach(it,al)
  {
    AST_Argument a = current_element(it);
    if(a->argumentType() == AR_MODIFICATION)
    {
      insert(a->getAsModification());
    }
  }
}

void
MMO_ModelAnnotation_::_processArgument(AST_Argument_Modification arg)
{
  if(arg->hasModification())
  {
    AST_Modification m = arg->modification();
    if(m->modificationType() == MODEQUAL)
    {
      _processAnnotation(*(arg->name()), m->getAsEqual());
    }
    else
    {
      Error::getInstance()->add(m->lineNum(), EM_IR | EM_ANNOTATION_TYPE,
          ER_Error, "%s", arg->name()->c_str());
    }
  }
  else
  {
    Error::getInstance()->add(arg->lineNum(), EM_IR | EM_ANNOTATION_TYPE,
        ER_Error, "%s", arg->name()->c_str());
  }
}

bool
MMO_ModelAnnotation_::insert(AST_Argument_Modification x)
{
  string annot = *(x->name());
  map<string, MMO_ModelAnnotation_::type>::iterator itf = _annotations.find(
      annot);
  if(itf == _annotations.end())
  {
    return false;
  }
  switch(itf->second)
  {
    case EXPERIMENT:
      {
      if(x->hasModification())
      {
        AST_Modification mod = x->modification();
        if(mod->modificationType() == MODCLASS)
        {
          AST_Modification_Class mc = mod->getAsClass();
          AST_ArgumentListIterator it;
          foreach(it,mc->arguments())
          {
            if(current_element(it)->argumentType() == AR_MODIFICATION)
            {
              _processArgument(
              current_element(it)->getAsModification());
            }
            else
            {
              Error::getInstance()->add(x->lineNum(),
              EM_IR | EM_ANNOTATION_TYPE,
                  ER_Error, "%s", x->name()->c_str());
            }
          }
        }
        else
        {
          Error::getInstance()->add(x->lineNum(),
          EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Error,
              "Missing modification arguments. %s",
              x->name()->c_str());
        }
      }
      else
      {
        Error::getInstance()->add(x->lineNum(),
        EM_IR | EM_ANNOTATION_NOT_FOUND,
            ER_Warning, "%s", x->name()->c_str());
      }
    }
      break;
    case WEIGHT:
      _processArgument(x);
      break;
    default:
      break;
  }
  return true;
}

void
MMO_ModelAnnotation_::_processList(AST_Expression x, list<double> *l)
{
  l->clear();
  MMO_EvalAnnotation_ ea(_data->symbols());
  MMO_AnnotationValue av;
  if(x->expressionType() == EXPBRACE)
  {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach(it,el)
    {
      av = ea.foldTraverse(current_element(it));
      l->push_back(av.real());
    }
  }
  else
  {
    av = ea.foldTraverse(x);
    l->push_back(av.real());
  }
}

void
MMO_ModelAnnotation_::_processList(AST_Expression x, list<string> *l)
{
  l->clear();
  MMO_EvalAnnotation_ ea(_data->symbols());
  MMO_AnnotationValue av;
  if(x->expressionType() == EXPBRACE)
  {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach(it,el)
    {
      av = ea.foldTraverse(current_element(it));
      l->push_back(av.str());
    }
  }
  else
  {
    av = ea.foldTraverse(x);
    l->push_back(av.str());
  }
}

void
MMO_ModelAnnotation_::_processExpressionList(AST_Expression x,
    list<AST_Expression> *l)
{
  if(x->expressionType() == EXPBRACE)
  {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach(it,el)
    {
      l->push_back(current_element(it));
    }
  }
}

ANT_PartitionMethod
MMO_ModelAnnotation_::partitionMethod()
{
  return _partitionMethod;
}

ANT_DT_Synch
MMO_ModelAnnotation_::_getDtSynch(string s)
{
  if(!s.compare("SD_DT_Fixed"))
  {
    return ANT_DT_Fixed;
  }
  else if(!s.compare("SD_DT_Asynchronous"))
  {
    return ANT_DT_Asynchronous;
  }
  return ANT_DT_Fixed;
}

ANT_PartitionMethod
MMO_ModelAnnotation_::_getPartitionMethod(string s)
{
  if(!s.compare("Metis"))
  {
    return ANT_Metis;
  }
  else if(!s.compare("HMetis"))
  {
    return ANT_HMetis;
  }
  else if(!s.compare("Scotch"))
  {
    return ANT_Scotch;
  }
  else if(!s.compare("Patoh"))
  {
    return ANT_Patoh;
  }
  else if(!s.compare("MTPL"))
  {
    return ANT_MTPL;
  }
  else if(!s.compare("MTPL_IT"))
  {
    return ANT_MTPL_IT;
  }
  else if(!s.compare("Manual"))
  {
    return ANT_Manual;
  }
  return ANT_Metis;
}

ANT_Solver
MMO_ModelAnnotation_::_getSolver(string s)
{
  if(!s.compare("QSS"))
  {
    _order = 1;
    _polyCoeffs = 2;
    return ANT_QSS;
  }
  else if(!s.compare("CQSS"))
  {
    _order = 1;
    _polyCoeffs = 2;
    return ANT_CQSS;
  }
  else if(!s.compare("LIQSS"))
  {
    _order = 1;
    _polyCoeffs = 2;
    return ANT_LIQSS;
  }
  else if(!s.compare("QSS2"))
  {
    _order = 2;
    _polyCoeffs = 3;
    return ANT_QSS2;
  }
  else if(!s.compare("LIQSS2"))
  {
    _order = 2;
    _polyCoeffs = 3;
    return ANT_LIQSS2;
  }
  else if(!s.compare("QSS3"))
  {
    _order = 3;
    _polyCoeffs = 4;
    return ANT_QSS3;
  }
  else if(!s.compare("LIQSS3"))
  {
    _order = 3;
    _polyCoeffs = 4;
    return ANT_LIQSS3;
  }
  else if(!s.compare("DASSL"))
  {
    _order = 1;
    _polyCoeffs = 1;
    return ANT_DASSL;
  }
  else if(!s.compare("DOPRI"))
  {
    _order = 1;
    _polyCoeffs = 1;
    return ANT_DOPRI;
  }
  else if(!s.compare("CVODE_BDF"))
  {
    _order = 1;
    _polyCoeffs = 1;
    return ANT_CVODE_BDF;
  }
  else if(!s.compare("IDA"))
  {
    _order = 1;
    _polyCoeffs = 1;
    return ANT_IDA;
  }
  else if(!s.compare("CVODE_AM"))
  {
    _order = 1;
    _polyCoeffs = 1;
    return ANT_CVODE_AM;
  }
  else if(!s.compare("QSS4"))
  {
    _order = 4;
    _polyCoeffs = 5;
    return ANT_QSS4;
  }
  return ANT_QSS;
}

void
MMO_ModelAnnotation_::_processAnnotation(string annot, AST_Modification_Equal x)
{
  map<string, MMO_ModelAnnotation_::type>::iterator itf = _annotations.find(
      annot);
  if(itf == _annotations.end())
  {
    Error::getInstance()->add(x->lineNum(),
    EM_IR | EM_ANNOTATION_NOT_FOUND,
        ER_Warning, "%s", annot.c_str());
  }
  MMO_EvalAnnotation_ ea(_data->symbols());
  MMO_AnnotationValue av;
  if(itf->second != DQMIN && itf->second != DQREL && itf->second != STEP_SIZE)
  {
    av = ea.foldTraverse(x->exp());
  }
  switch(itf->second)
  {
    case DESC:
      _desc = av.str();
      break;
    case DQMIN:
      _processList(x->exp(), &_DQMin);
      break;
    case DQREL:
      _processList(x->exp(), &_DQRel);
      break;
    case WEIGHT:
      _weight = av.real();
      break;
    case SOLVER:
      _solver = _getSolver(av.str());
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
      _processList(x->exp(), &_sample);
      break;
    case SCHEDULER:
      _scheduler = av.str();
      break;
    case JACOBIAN:
      _jacobian = ("Sparse" == av.str() ? 0 : 1);
      break;
    case SYM_DIFF:
      _symDiff = true;
      if(av.integer() == 0)
      {
        _symDiff = false;
      }
      break;
    case OUTPUT:
      _processExpressionList(x->exp(), &_output);
      break;
    case PARTITION_METHOD:
      _partitionMethod = _getPartitionMethod(av.str());
      _partitionMethodString = av.str();
      break;
    case DELTAT_SYNCH:
      _dtSynch = _getDtSynch(av.str());
      _dtSynchString = av.str();
      break;
    case PARALLEL:
      _parallel = true;
      if(av.integer() == 0)
      {
        _parallel = false;
      }
      break;
    case DELTAT:
      _dt = av.real();
      break;
    case STORE_DATA:
      break;
    case PATOH_SETTINGS:
      _processList(x->exp(), &_patohSettings);
      break;
    case SCOTCH_SETTINGS:
      _processList(x->exp(), &_scotchSettings);
      break;
    case METIS_SETTINGS:
      _processList(x->exp(), &_metisSettings);
      break;
    default:
      break;
  }
}

void
MMO_ModelAnnotation_::setDesc(string desc)
{
  _desc = desc;
}

string
MMO_ModelAnnotation_::desc()
{
  return _desc;
}

void
MMO_ModelAnnotation_::setDQMin(double dqmin)
{
  _DQMin.push_back(dqmin);
}

list<double>
MMO_ModelAnnotation_::dqmin()
{
  return _DQMin;
}

void
MMO_ModelAnnotation_::setDT(double dt)
{
  _dt = dt;
}

double
MMO_ModelAnnotation_::DT()
{
  return _dt;
}

list<double>
MMO_ModelAnnotation_::dqrel()
{
  return _DQRel;
}

void
MMO_ModelAnnotation_::setDQRel(double dqrel)
{
  _DQRel.push_back(dqrel);
}

void
MMO_ModelAnnotation_::setWeight(double weight)
{
  _weight = weight;
}

double
MMO_ModelAnnotation_::weight()
{
  return _weight;
}

void
MMO_ModelAnnotation_::setSolver(ANT_Solver solver)
{
  _solver = solver;
}

string
MMO_ModelAnnotation_::solverString()
{
  return _solverString;
}

ANT_Solver
MMO_ModelAnnotation_::solver()
{
  return _solver;
}

void
MMO_ModelAnnotation_::setPartitionMethod(ANT_PartitionMethod pm)
{
  _partitionMethod = pm;
}

string
MMO_ModelAnnotation_::partitionMethodString()
{
  return _partitionMethodString;
}

ANT_DT_Synch
MMO_ModelAnnotation_::dtSynch()
{
  return _dtSynch;
}

string
MMO_ModelAnnotation_::dtSynchString()
{
  return _dtSynchString;
}

void
MMO_ModelAnnotation_::setDtSynch(ANT_DT_Synch synch)
{
  _dtSynch = synch;
}

void
MMO_ModelAnnotation_::setInitialTime(double it)
{
  _initialTime = it;
}

double
MMO_ModelAnnotation_::initialTime()
{
  return _initialTime;
}

void
MMO_ModelAnnotation_::setFinalTime(double ft)
{
  _finalTime = ft;
}

double
MMO_ModelAnnotation_::finalTime()
{
  return _finalTime;
}

void
MMO_ModelAnnotation_::setMinStep(double ms)
{
  _minStep = ms;
}

double
MMO_ModelAnnotation_::minStep()
{
  return _minStep;
}

void
MMO_ModelAnnotation_::setZCHyst(double zch)
{
  _ZCHyst = zch;
}

double
MMO_ModelAnnotation_::ZCHyst()
{
  return _ZCHyst;
}

void
MMO_ModelAnnotation_::setDerDelta(double dd)
{
  _derDelta = dd;
}

double
MMO_ModelAnnotation_::derDelta()
{
  return _derDelta;
}

void
MMO_ModelAnnotation_::setStoreData(string store)
{
  _storeData = store;
}

string
MMO_ModelAnnotation_::storeData()
{
  return _storeData;
}

void
MMO_ModelAnnotation_::setLps(int lps)
{
  _lps = lps;
}

void
MMO_ModelAnnotation_::setJacobian(int jacobian)
{
  _jacobian = jacobian;
}

int
MMO_ModelAnnotation_::jacobian()
{
  return _jacobian;
}

bool
MMO_ModelAnnotation_::classic()
{
  return _solver == ANT_DASSL || _solver == ANT_DOPRI
      || _solver == ANT_CVODE_BDF
      || _solver == ANT_IDA || _solver == ANT_CVODE_AM;
}

int
MMO_ModelAnnotation_::lps()
{
  return _lps;
}

void
MMO_ModelAnnotation_::setNodeSize(int ns)
{
  _nodeSize = ns;
}

int
MMO_ModelAnnotation_::nodeSize()
{
  return _nodeSize;
}

void
MMO_ModelAnnotation_::setCommInterval(string ci)
{
  _commInterval = ci;
}

string
MMO_ModelAnnotation_::commInterval()
{
  return _commInterval;
}

void
MMO_ModelAnnotation_::setSample(double s)
{
  _sample.push_back(s);
}

list<double>
MMO_ModelAnnotation_::sample()
{
  return _sample;
}

void
MMO_ModelAnnotation_::setScheduler(string sched)
{
  _scheduler = sched;
}

string
MMO_ModelAnnotation_::scheduler()
{
  return _scheduler;
}

void
MMO_ModelAnnotation_::setSymDiff(bool sd)
{
  _symDiff = sd;
}

bool
MMO_ModelAnnotation_::symDiff()
{
  return _symDiff;
}

int
MMO_ModelAnnotation_::order()
{
  return _order;
}

list<AST_Expression>
MMO_ModelAnnotation_::output()
{
  return _output;
}

list<string>
MMO_ModelAnnotation_::patohSettings()
{
  return _patohSettings;
}

list<string>
MMO_ModelAnnotation_::scotchSettings()
{
  return _scotchSettings;
}

list<string>
MMO_ModelAnnotation_::metisSettings()
{
  return _metisSettings;
}

void
MMO_ModelAnnotation_::setPatohSettings(string l)
{
  _patohSettings.push_back(l);
}

void
MMO_ModelAnnotation_::setScotchSettings(string l)
{
  _scotchSettings.push_back(l);
}

void
MMO_ModelAnnotation_::setMetisSettings(string l)
{
  _metisSettings.push_back(l);
}

MMO_ModelAnnotation
newMMO_ModelAnnotation(MMO_ModelData data)
{
  return new MMO_ModelAnnotation_(data);
}

void
deleteMMO_ModelAnnotation(MMO_ModelAnnotation m)
{
  delete m;
}

/* MMO_AnnotationValue class */

MMO_AnnotationValue::MMO_AnnotationValue() :
    _integer(0), _real(0), _str("")
{
}

MMO_AnnotationValue::~MMO_AnnotationValue()
{
}

int
MMO_AnnotationValue::integer()
{
  return _integer;
}

void
MMO_AnnotationValue::setInteger(int i)
{
  _integer = i;
}

double
MMO_AnnotationValue::real()
{
  return _real;
}

void
MMO_AnnotationValue::setReal(double d)
{
  _real = d;
}

string
MMO_AnnotationValue::str()
{
  return _str;
}

void
MMO_AnnotationValue::setStr(string s)
{
  _str = s;
}

/* EvalAnnotation class */

MMO_EvalAnnotation_::MMO_EvalAnnotation_(VarSymbolTable st) :
    _st(st), _tokens()
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
  _tokens.insert(
      pair<string, string>("SD_DT_Asynchronous", "SD_DT_Asynchronous"));
}

MMO_AnnotationValue
MMO_EvalAnnotation_::foldTraverseElement(AST_Expression e)
{
  MMO_AnnotationValue av = MMO_AnnotationValue();
  switch(e->expressionType())
  {
    case EXPSTRING:
      av.setStr(e->getAsString()->print());
      break;
    case EXPCOMPREF:
      {
      AST_Expression_ComponentReference cr = e->getAsComponentReference();
      string name = cr->name();
      VarInfo vi = _st->lookup(name);
      if(vi != NULL)
      {
        if(vi->isConstant())
        {
          av.setInteger(vi->value());
        }
      }
      else
      {
        if(_tokens.find(name) != _tokens.end())
        {
          av.setStr(name);
        }
      }
    }
      break;
    case EXPINTEGER:
      av.setInteger(e->getAsInteger()->val());
      av.setReal(e->getAsInteger()->val());
      break;
    case EXPREAL:
      av.setReal(e->getAsReal()->val());
      av.setInteger(e->getAsReal()->val());
      break;
    case EXPBOOLEAN:
      if(e->getAsBoolean()->value())
      {
        av.setInteger(1);
      }
      else
      {
        av.setInteger(0);
      }
      break;
    case EXPBOOLEANNOT:
      {
      MMO_AnnotationValue a = foldTraverse(e->getAsBooleanNot()->exp());
      if(a.integer() == 1)
      {
        av.setInteger(0);
      }
      else
      {
        av.setInteger(1);
      }
      break;
    }
    default:
      break;
  }
  return av;
}

void
MMO_EvalAnnotation_::_setBoolean(bool condition, MMO_AnnotationValue *e)
{
  if(condition)
  {
    e->setInteger(1);
  }
  else
  {
    e->setInteger(0);
  }
}

MMO_AnnotationValue
MMO_EvalAnnotation_::foldTraverseElement(MMO_AnnotationValue e1,
    MMO_AnnotationValue e2, BinOpType bot)
{
  MMO_AnnotationValue av = MMO_AnnotationValue();
  switch(bot)
  {
    case BINOPOR:
      _setBoolean(e1.integer() || e2.integer(), &av);
      break;
    case BINOPAND:
      _setBoolean(e1.integer() && e2.integer(), &av);
      break;
    case BINOPLOWER:
      _setBoolean((e1.integer() < e2.integer()) || (e1.real() < e2.real()),
          &av);
      break;
    case BINOPLOWEREQ:
      _setBoolean((e1.integer() <= e2.integer()) || (e1.real() <= e2.real()),
          &av);
      break;
    case BINOPGREATER:
      _setBoolean((e1.integer() > e2.integer()) || (e1.real() > e2.real()),
          &av);
      break;
    case BINOPGREATEREQ:
      _setBoolean((e1.integer() >= e2.integer()) || (e1.real() >= e2.real()),
          &av);
      break;
    case BINOPCOMPNE:
      _setBoolean((e1.integer() != e2.integer()) || (e1.real() != e2.real()),
          &av);
      break;
    case BINOPCOMPEQ:
      _setBoolean((e1.integer() == e2.integer()) || (e1.real() == e2.real()),
          &av);
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
      if(e2.integer() != 0)
      {
        av.setInteger(e1.integer() / e2.integer());
      }
      if(e2.real() != 0)
      {
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

MMO_AnnotationValue
MMO_EvalAnnotation_::foldTraverseElementUMinus(AST_Expression e)
{
  MMO_AnnotationValue av = foldTraverseElement(e);
  av.setInteger(-1 * av.integer());
  av.setReal(-1 * av.real());
  return av;
}

MMO_EvalAnnotation
newMMO_EvalAnnotation(VarSymbolTable st)
{
  return new MMO_EvalAnnotation_(st);
}

void
deleteMMO_EvalAnnotation(MMO_EvalAnnotation m)
{
  delete m;
}

void
MMO_ModelAnnotation_::setParallel(bool p)
{
  _parallel = p;
}

bool
MMO_ModelAnnotation_::parallel()
{
  return _parallel;
}

int
MMO_ModelAnnotation_::polyCoeffs()
{
  return _polyCoeffs;
}

bool
MMO_Annotation_::hasDerivative()
{
  return true;
}

bool
MMO_Annotation_::hasInclude()
{
  return true;
}

bool
MMO_Annotation_::hasIncludeDirectory()
{
  return true;
}

bool
MMO_Annotation_::hasLibraries()
{
  return true;
}

bool
MMO_Annotation_::hasLibraryDirectory()
{
  return true;
}

string
MMO_Annotation_::derivative()
{
  return "";
}

string
MMO_Annotation_::include()
{
  return "";
}

string
MMO_Annotation_::includeDirectory()
{
  return "";
}

list<string>
MMO_Annotation_::libraries()
{
  return list<string>();
}

string
MMO_Annotation_::libraryDirectory()
{
  return "";
}

void
MMO_Annotation_::eventComment(AST_Comment x)
{
  return;
}

bool
MMO_Annotation_::insert(AST_Argument_Modification x)
{
  return true;
}

void
MMO_Annotation_::setDesc(string desc)
{
  return;
}

string
MMO_Annotation_::desc()
{
  return "";
}

void
MMO_Annotation_::setDQMin(double dqmin)
{
  return;
}

list<double>
MMO_Annotation_::dqmin()
{
  return list<double>();
}

list<double>
MMO_Annotation_::dqrel()
{
  return list<double>();
}

void
MMO_Annotation_::setDQRel(double dqrel)
{
  return;
}

void
MMO_Annotation_::setWeight(double weight)
{
  return;
}

double
MMO_Annotation_::weight()
{
  return 0;
}

void
MMO_Annotation_::setSolver(ANT_Solver solver)
{
  return;
}

ANT_Solver
MMO_Annotation_::solver()
{
  return ANT_QSS;
}

string
MMO_Annotation_::solverString()
{
  return "";
}

void
MMO_Annotation_::setInitialTime(double it)
{
  return;
}

double
MMO_Annotation_::initialTime()
{
  return 0;
}

void
MMO_Annotation_::setFinalTime(double ft)
{
  return;
}

double
MMO_Annotation_::finalTime()
{
  return 0;
}

void
MMO_Annotation_::setMinStep(double ms)
{
  return;
}

double
MMO_Annotation_::minStep()
{
  return 0;
}

void
MMO_Annotation_::setZCHyst(double zch)
{
  return;
}

double
MMO_Annotation_::ZCHyst()
{
  return 0;
}

void
MMO_Annotation_::setDerDelta(double dd)
{
  return;
}

double
MMO_Annotation_::derDelta()
{
  return 0;
}

void
MMO_Annotation_::setLps(int lps)
{
  return;
}

void
MMO_Annotation_::setDT(double dt)
{
  return;
}

double
MMO_Annotation_::DT()
{
  return 0;
}

int
MMO_Annotation_::lps()
{
  return 0;
}

void
MMO_Annotation_::setNodeSize(int ns)
{
  return;
}

int
MMO_Annotation_::nodeSize()
{
  return 0;
}

void
MMO_Annotation_::setCommInterval(string ci)
{
  return;
}

string
MMO_Annotation_::commInterval()
{
  return "";
}

void
MMO_Annotation_::setSample(double s)
{
  return;
}

list<double>
MMO_Annotation_::sample()
{
  return list<double>();
}

void
MMO_Annotation_::setSymDiff(bool sd)
{
  return;
}

bool
MMO_Annotation_::symDiff()
{
  return true;
}

int
MMO_Annotation_::order()
{
  return 1;
}

string
MMO_Annotation_::scheduler()
{
  return "";
}

void
MMO_Annotation_::setScheduler(string sched)
{
  return;
}

list<AST_Expression>
MMO_Annotation_::output()
{
  return list<AST_Expression>();
}

void
MMO_Annotation_::setStoreData(string save)
{
  return;
}

string
MMO_Annotation_::storeData()
{
  return "";
}

void
MMO_Annotation_::setPartitionMethod(ANT_PartitionMethod pm)
{
  return;
}

string
MMO_Annotation_::partitionMethodString()
{
  return "";
}

ANT_PartitionMethod
MMO_Annotation_::partitionMethod()
{
  return ANT_Metis;
}

void
MMO_Annotation_::setParallel(bool p)
{
  return;
}

bool
MMO_Annotation_::parallel()
{
  return true;
}

int
MMO_Annotation_::polyCoeffs()
{
  return 2;
}

void
MMO_Annotation_::setDtSynch(ANT_DT_Synch synch)
{
  return;
}

string
MMO_Annotation_::dtSynchString()
{
  return "";
}

ANT_DT_Synch
MMO_Annotation_::dtSynch()
{
  return ANT_DT_Asynchronous;
}

list<string>
MMO_Annotation_::patohSettings()
{
  return list<string>();
}

list<string>
MMO_Annotation_::scotchSettings()
{
  return list<string>();
}

list<string>
MMO_Annotation_::metisSettings()
{
  return list<string>();
}

void
MMO_Annotation_::setPatohSettings(string l)
{
  return;
}

void
MMO_Annotation_::setScotchSettings(string l)
{
  return;
}

void
MMO_Annotation_::setMetisSettings(string l)
{
  return;
}

