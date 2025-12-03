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

#include <algorithm>
#include <utility>

#include "annotation.hpp"

#include <ast/element.hpp>
#include <ast/expression.hpp>
#include <ast/modification.hpp>
#include <util/error.hpp>
#include <util/model_config.hpp>
#include <util/symbol_table.hpp>
#include <util/util.hpp>

namespace MicroModelica {
using namespace Util;
namespace IR {

FunctionAnnotation::FunctionAnnotation() : _annotations(), _derivative(), _include() { initialize(); }

void FunctionAnnotation::initialize()
{
  _libraryDirectory = Utils::instance().environmentVariable("MMOC_LIBRARIES");
  _includeDirectory = Utils::instance().environmentVariable("MMOC_INCLUDE");
  _annotations.insert(pair<string, FunctionAnnotation::type>("derivative", type::DERIVATIVE));
  _annotations.insert(pair<string, FunctionAnnotation::type>("Include", type::INCLUDE));
  _annotations.insert(pair<string, FunctionAnnotation::type>("IncludeDirectory", type::INCLUDE_DIRECTORY));
  _annotations.insert(pair<string, FunctionAnnotation::type>("Library", type::LIBRARY));
  _annotations.insert(pair<string, FunctionAnnotation::type>("LibraryDirectory", type::LIBRARY_DIRECTORY));
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
  if (x->hasModification() && (x->modification()->modificationType() == MODEQUAL)) {
    AST_Expression mod = x->modification()->getAsEqual()->exp();
    switch (itf->second) {
    case type::INCLUDE:
      if (mod->expressionType() == EXPSTRING) {
        _include = mod->getAsString()->str();
      }
      break;
    case type::INCLUDE_DIRECTORY:
      if (mod->expressionType() == EXPSTRING) {
        _includeDirectory = mod->getAsString()->str();
      }
      break;
    case type::LIBRARY:
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
    case type::LIBRARY_DIRECTORY:
      if (mod->expressionType() == EXPSTRING) {
        _libraryDirectory = mod->getAsString()->str();
      }
      break;
    case type::DERIVATIVE:
      if (mod->expressionType() == EXPSTRING) {
        _derivative = mod->getAsString()->str();
      }
      break;
    default:
      break;
    }
  }
  return true;
}

string FunctionAnnotation::derivative() { return _derivative; }

string FunctionAnnotation::include() { return _include; }

string FunctionAnnotation::includeDirectory() { return _includeDirectory; }

SymbolTable FunctionAnnotation::libraries() const { return _libraries; }

string FunctionAnnotation::libraryDirectory() { return _libraryDirectory; }

ModelAnnotation::ModelAnnotation()
    : _solver(Solver::LIQSS2),
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
      _partition_method_string("Scotch"),
      _parallel(false),
      _dt(0),
      _polyCoeffs(1),
      _dtSynch(DT_Synch::DT_Fixed),
      _dtSynchString("SD_DT_Asynchronous"),
      _desc(),
      _patohSettings(),
      _scotchSettings(),
      _metisSettings(),
      _jacobian(0),
      _BDFPartition(newAST_ExpressionList()),
      _BDFPartitionDepth(),
      _BDFMaxStep(0),
      _hd_matrix(),
      _hz_matrix(),
      _hh_matrix(),
      _sd_matrix(),
      _sz_matrix(),
      _event_ids(),
      _current_exp_id(-1)
{
  initialize();
}

void ModelAnnotation::initialize()
{
  _annotations.insert(pair<string, ModelAnnotation::type>("experiment", type::EXPERIMENT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Description", type::DESC));
  _annotations.insert(pair<string, ModelAnnotation::type>("Tolerance", type::DQREL));
  _annotations.insert(pair<string, ModelAnnotation::type>("AbsTolerance", type::DQMIN));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Weight", type::WEIGHT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Solver", type::SOLVER));
  _annotations.insert(pair<string, ModelAnnotation::type>("StartTime", type::INITIAL_TIME));
  _annotations.insert(pair<string, ModelAnnotation::type>("StopTime", type::FINAL_TIME));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_MinStep", type::MIN_STEP));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_ZCHyst", type::ZCHYST));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DerDelta", type::DER_DELTA));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_LPS", type::LPS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_NodeSize", type::NODE_SIZE));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_OutputType", type::COMM_INTERVAL));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Period", type::STEP_SIZE));
  _annotations.insert(pair<string, ModelAnnotation::type>("Jacobian", type::JACOBIAN));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_SymDiff", type::SYM_DIFF));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Scheduler", type::SCHEDULER));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Output", type::OUTPUT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_StoreData", type::STORE_DATA));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_PartitionMethod", type::PARTITION_METHOD));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Parallel", type::PARALLEL));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DT_Min", type::DELTAT));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_DT_Synch", type::DELTAT_SYNCH));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_PatohSettings", type::PATOH_SETTINGS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_MetisSettings", type::METIS_SETTINGS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_ScotchSettings", type::SCOTCH_SETTINGS));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_BDF_Part", type::BDF_PARTITION));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_BDF_PDepth", type::BDF_PARTITION_DEPTH));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_BDF_Max_Step", type::BDF_MAX_STEP));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_Event_Id", type::EVENT_ID));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_HD", type::HD_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_HZ", type::HZ_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_HH", type::HH_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_LHS_ST", type::LHS_ST_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_LHS_DSC", type::LHS_DSC_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_RHS_ST", type::RHS_ST_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_SD", type::SD_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_SZ", type::SZ_MATRIX));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_RandomSeed", type::RANDOM_SEED));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_CVODEMaxOrder", type::CV_ODE_MAX_ORDER));
  _annotations.insert(pair<string, ModelAnnotation::type>("MMO_XOutput", type::X_OUTPUT));
  _integer_annotations_map.insert(pair<string, IntegerAnnotations>("MMO_RandomSeed", IntegerAnnotations::RandomSeed));
  _integer_annotations_map.insert(pair<string, IntegerAnnotations>("MMO_CVODEMaxOrder", IntegerAnnotations::CVODEMaxOrder));
  _integer_annotations_map.insert(pair<string, IntegerAnnotations>("MMO_XOutput", IntegerAnnotations::XOutput));
  _sample.push_back(1e-2);
  _DQMin.push_back(1e-3);
  _DQRel.push_back(1e-3);
}

void ModelAnnotation::expComment(AST_Comment x, int id)
{
  if (x == nullptr) {
    return;
  }
  _current_exp_id = id;
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
  case type::EXPERIMENT: {
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
  case type::WEIGHT:
  case type::EVENT_ID:
  case type::HD_MATRIX:
  case type::HZ_MATRIX:
  case type::HH_MATRIX:
  case type::LHS_ST_MATRIX:
  case type::LHS_DSC_MATRIX:
  case type::RHS_ST_MATRIX:
  case type::SD_MATRIX:
  case type::SZ_MATRIX:
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
  EvalAnnotation ea;
  AnnotationValue av;
  if (x->expressionType() == EXPBRACE) {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      if (current_element(it)->expressionType() == EXPOUTPUT) {
        // If we have a pair, select the first element as the
        // value and the second as the size.
        // This is used for DQMin and DQRel for arrays.
        AST_Expression_Output out = current_element(it)->getAsOutput();
        AST_ExpressionList pair_exp = out->expressionList();
        if (pair_exp->size() != 2) {
          Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Warning, "Tolerances expression must be a pair.");
          return;
        }
        AST_ExpressionListIterator pair_exp_it;
        bool first = true;
        AnnotationValue size;
        foreach (pair_exp_it, pair_exp) {
          if (first) {
            av = ea.apply(current_element(pair_exp_it));
            first = false;
          } else {
            size = ea.apply(current_element(pair_exp_it));
          }
        }
        for (int i = 0; i < size.integer(); i++) {
          l->push_back(av.real());
        }
      } else {
        av = ea.apply(current_element(it));
        l->push_back(av.real());
      }
    }
  }
}

void ModelAnnotation::processList(AST_Expression x, list<string> *l)
{
  l->clear();
  EvalAnnotation ea;
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

void ModelAnnotation::processExpressionList(AST_Expression x, AST_ExpressionList exps)
{
  if (x->expressionType() == EXPBRACE) {
    AST_Expression_Brace b = x->getAsBrace();
    AST_ExpressionList el = b->arguments();
    AST_ExpressionListIterator it;
    foreach (it, el) {
      exps->push_back(current_element(it));
    }
  }
}

DT_Synch ModelAnnotation::getDtSynch(string s)
{
  if (!s.compare("SD_DT_Fixed")) {
    return DT_Synch::DT_Fixed;
  } else if (!s.compare("SD_DT_Asynchronous")) {
    return DT_Synch::DT_Asynchronous;
  }
  return DT_Synch::DT_Fixed;
}

Solver ModelAnnotation::getSolver(string s)
{
  if (!s.compare("QSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return Solver::QSS;
  } else if (!s.compare("LIQSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return Solver::LIQSS;
  } else if (!s.compare("QSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return Solver::QSS2;
  } else if (!s.compare("LIQSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return Solver::LIQSS2;
  } else if (!s.compare("LIQSS_BDF")) {
    _order = 2;
    _polyCoeffs = 3;
    return Solver::LIQSS_BDF;
  } else if (!s.compare("QSS3")) {
    _order = 3;
    _polyCoeffs = 4;
    return Solver::QSS3;
  } else if (!s.compare("LIQSS3")) {
    _order = 3;
    _polyCoeffs = 4;
    return Solver::LIQSS3;
  } else if (!s.compare("DASSL")) {
    _order = 1;
    _polyCoeffs = 1;
    return Solver::DASSL;
  } else if (!s.compare("DOPRI")) {
    _order = 1;
    _polyCoeffs = 1;
    return Solver::DOPRI;
  } else if (!s.compare("CVODE_BDF")) {
    _order = 1;
    _polyCoeffs = 1;
    return Solver::CVODE_BDF;
  } else if (!s.compare("IDA")) {
    _order = 1;
    _polyCoeffs = 1;
    return Solver::IDA;
  } else if (!s.compare("CVODE_AM")) {
    _order = 1;
    _polyCoeffs = 1;
    return Solver::CVODE_AM;
  } else if (!s.compare("QSS4")) {
    _order = 4;
    _polyCoeffs = 5;
    return Solver::QSS4;
  } else if (!s.compare("mLIQSS")) {
    _order = 1;
    _polyCoeffs = 2;
    return Solver::mLIQSS;
  } else if (!s.compare("mLIQSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return Solver::mLIQSS2;
  } else if (!s.compare("CQSS1")) {
    _order = 1;
    _polyCoeffs = 2;
    return Solver::CQSS1;
  } else if (!s.compare("CQSS2")) {
    _order = 2;
    _polyCoeffs = 3;
    return Solver::CQSS2;
  } else if (!s.compare("CQSS3")) {
    _order = 3;
    _polyCoeffs = 4;
    return Solver::CQSS3;
  }
  return Solver::QSS;
}

void ModelAnnotation::parseMatrix(AST_Expression exp, IR::MATRIX::UserDefMatrixExps &matrix)
{
  AST_ExpressionList matrix_exps = newAST_ExpressionList();
  processExpressionList(exp, matrix_exps);
  matrix[_current_exp_id] = matrix_exps;
}

void ModelAnnotation::processAnnotation(string annot, AST_Modification_Equal x)
{
  map<string, ModelAnnotation::type>::const_iterator itf = _annotations.find(annot);
  if (itf == _annotations.end()) {
    Error::instance().add(x->lineNum(), EM_IR | EM_ANNOTATION_NOT_FOUND, ER_Warning, "%s", annot.c_str());
  }
  EvalAnnotation ea;
  AnnotationValue av;
  if (itf->second != type::DQMIN && itf->second != type::DQREL && itf->second != type::STEP_SIZE) {
    av = ea.apply(x->exp());
  }
  switch (itf->second) {
  case type::DESC:
    _desc = av.str();
    break;
  case type::DQMIN:
    processList(x->exp(), &_DQMin);
    break;
  case type::DQREL:
    processList(x->exp(), &_DQRel);
    break;
  case type::WEIGHT:
    _weight = av.real();
    break;
  case type::SOLVER:
    _solver = getSolver(av.str());
    _solverString = av.str();
    break;
  case type::INITIAL_TIME:
    _initialTime = av.real();
    break;
  case type::FINAL_TIME:
    _finalTime = av.real();
    break;
  case type::MIN_STEP:
    _minStep = av.real();
    break;
  case type::ZCHYST:
    _ZCHyst = av.real();
    break;
  case type::DER_DELTA:
    _derDelta = av.real();
    break;
  case type::LPS:
    _lps = av.integer();
    break;
  case type::NODE_SIZE:
    _nodeSize = av.integer();
    break;
  case type::COMM_INTERVAL:
    _commInterval = av.str();
    break;
  case type::STEP_SIZE:
    processList(x->exp(), &_sample);
    break;
  case type::SCHEDULER:
    _scheduler = av.str();
    break;
  case type::JACOBIAN:
    _jacobian = ("Sparse" == av.str() ? 0 : 1);
    break;
  case type::SYM_DIFF:
    _symDiff = true;
    if (av.integer() == 0) {
      _symDiff = false;
    }
    break;
  case type::OUTPUT:
    processExpressionList(x->exp(), &_output);
    break;
  case type::PARTITION_METHOD:
    _partition_method_string = av.str();
    break;
  case type::DELTAT_SYNCH:
    _dtSynch = getDtSynch(av.str());
    _dtSynchString = av.str();
    break;
  case type::PARALLEL:
    _parallel = true;
    if (av.integer() == 0) {
      _parallel = false;
    }
    break;
  case type::DELTAT:
    _dt = av.real();
    break;
  case type::STORE_DATA:
    break;
  case type::PATOH_SETTINGS:
    processList(x->exp(), &_patohSettings);
    break;
  case type::SCOTCH_SETTINGS:
    processList(x->exp(), &_scotchSettings);
    break;
  case type::METIS_SETTINGS:
    processList(x->exp(), &_metisSettings);
    break;
  case type::BDF_PARTITION:
    processExpressionList(x->exp(), _BDFPartition);
    break;
  case type::BDF_PARTITION_DEPTH:
    _BDFPartitionDepth = av.integer();
    break;
  case type::BDF_MAX_STEP:
    _BDFMaxStep = av.real();
    break;
  case type::EVENT_ID:
    _event_ids = av.plainStr();
    break;
  case type::HD_MATRIX:
    parseMatrix(x->exp(), _hd_matrix);
    break;
  case type::HZ_MATRIX:
    parseMatrix(x->exp(), _hz_matrix);
    break;
  case type::HH_MATRIX:
    parseMatrix(x->exp(), _hh_matrix);
    break;
  case type::LHS_ST_MATRIX:
    parseMatrix(x->exp(), _lhs_st_matrix);
    break;
  case type::RHS_ST_MATRIX:
    parseMatrix(x->exp(), _rhs_st_matrix);
    break;
  case type::LHS_DSC_MATRIX:
    parseMatrix(x->exp(), _lhs_dsc_matrix);
    break;
  case type::SD_MATRIX:
    parseMatrix(x->exp(), _sd_matrix);
    break;
  case type::SZ_MATRIX:
    parseMatrix(x->exp(), _sz_matrix);
    break;
  case type::RANDOM_SEED:
  case type::CV_ODE_MAX_ORDER:
  case type::X_OUTPUT: {
    IntegerAnnotations int_annot = _integer_annotations_map[itf->first];
    _integer_annotations_val.insert(pair<IntegerAnnotations, int>(int_annot, av.integer()));
    break;
  }
  default:
    break;
  }
}

/// @todo Fix annotations structure, see https://github.com/CIFASIS/qss-solver/issues/261
int ModelAnnotation::getAnnotation(IntegerAnnotations annot) const
{
  if (hasAnnotation(annot)) {
    return _integer_annotations_val.at(annot);
  }
  return 0;
}

bool ModelAnnotation::hasAnnotation(IntegerAnnotations annot) const
{
  return _integer_annotations_val.find(annot) != _integer_annotations_val.end();
}

string ModelAnnotation::desc() { return _desc; }

list<double> ModelAnnotation::dqmin() { return _DQMin; }

double ModelAnnotation::DT() { return _dt; }

list<double> ModelAnnotation::dqrel() { return _DQRel; }

double ModelAnnotation::weight() { return _weight; }

string ModelAnnotation::solverString() { return _solverString; }

Solver ModelAnnotation::solver() { return _solver; }

string ModelAnnotation::partitionMethodString() { return _partition_method_string; }

DT_Synch ModelAnnotation::dtSynch() { return _dtSynch; }

string ModelAnnotation::dtSynchString() { return _dtSynchString; }

double ModelAnnotation::initialTime() { return _initialTime; }

double ModelAnnotation::finalTime() { return _finalTime; }

double ModelAnnotation::minStep() { return _minStep; }

double ModelAnnotation::ZCHyst() { return _ZCHyst; }

double ModelAnnotation::derDelta() { return _derDelta; }

string ModelAnnotation::storeData() { return _storeData; }

int ModelAnnotation::jacobian() { return _jacobian; }

bool ModelAnnotation::isClassic()
{
  return _solver == Solver::DASSL || _solver == Solver::DOPRI || _solver == Solver::CVODE_BDF || _solver == Solver::IDA ||
         _solver == Solver::CVODE_AM;
}

int ModelAnnotation::lps() { return _lps; }

int ModelAnnotation::nodeSize() { return _nodeSize; }

string ModelAnnotation::commInterval() { return _commInterval; }

list<double> ModelAnnotation::sample() { return _sample; }

string ModelAnnotation::scheduler() { return _scheduler; }

bool ModelAnnotation::symDiff() { return _symDiff; }

int ModelAnnotation::order() { return _order; }

list<AST_Expression> ModelAnnotation::output() { return _output; }

list<string> ModelAnnotation::patohSettings() { return _patohSettings; }

list<string> ModelAnnotation::scotchSettings() { return _scotchSettings; }

list<string> ModelAnnotation::metisSettings() { return _metisSettings; }

AST_ExpressionList ModelAnnotation::BDFPartition() { return _BDFPartition; }

int ModelAnnotation::BDFPartitionDepth() { return _BDFPartitionDepth; }

double ModelAnnotation::BDFMaxStep() { return _BDFMaxStep; }

bool ModelAnnotation::parallel() { return _parallel; }

int ModelAnnotation::polyCoeffs() { return _polyCoeffs; }

string ModelAnnotation::EventId() { return _event_ids; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::HDMatrix() { return _hd_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::HZMatrix() { return _hz_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::HHMatrix() { return _hh_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::LHSSTMatrix() { return _lhs_st_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::LHSDSCMatrix() { return _lhs_dsc_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::RHSSTMatrix() { return _rhs_st_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::SDMatrix() { return _sd_matrix; }

IR::MATRIX::UserDefMatrixExps ModelAnnotation::SZMatrix() { return _sz_matrix; }

/* AnnotationValue class */

AnnotationValue::AnnotationValue() : _integer(0), _real(0), _str(), _plain_str() {}

int AnnotationValue::integer() { return _integer; }

void AnnotationValue::setInteger(int i) { _integer = i; }

double AnnotationValue::real() { return _real; }

void AnnotationValue::setReal(double d) { _real = d; }

string AnnotationValue::str() { return _str; }

void AnnotationValue::setStr(string s) { _str = s; }

string AnnotationValue::plainStr() { return _plain_str; }

void AnnotationValue::setPlainStr(string plain_str) { _plain_str = plain_str; }

/* EvalAnnotation class */

EvalAnnotation::EvalAnnotation() : _tokens()
{
  _tokens.emplace_back("QSS");
  _tokens.emplace_back("QSS2");
  _tokens.emplace_back("QSS3");
  _tokens.emplace_back("LIQSS");
  _tokens.emplace_back("LIQSS2");
  _tokens.emplace_back("LIQSS_BDF");
  _tokens.emplace_back("LIQSS3");
  _tokens.emplace_back("QSS4");
  _tokens.emplace_back("mLIQSS");
  _tokens.emplace_back("mLIQSS2");
  _tokens.emplace_back("CQSS1");
  _tokens.emplace_back("CQSS2");
  _tokens.emplace_back("CQSS3");
  _tokens.emplace_back("DASSL");
  _tokens.emplace_back("DOPRI");
  _tokens.emplace_back("CVODE_AM");
  _tokens.emplace_back("IDA");
  _tokens.emplace_back("CVODE_BDF");
  _tokens.emplace_back("ST_Linear");
  _tokens.emplace_back("ST_Binary");
  _tokens.emplace_back("ST_Random");
  _tokens.emplace_back("CI_Step");
  _tokens.emplace_back("CI_Dense");
  _tokens.emplace_back("CI_Sampled");
  _tokens.emplace_back("SD_File");
  _tokens.emplace_back("SD_Memory");
  _tokens.emplace_back("Metis");
  _tokens.emplace_back("HMetis");
  _tokens.emplace_back("Scotch");
  _tokens.emplace_back("Patoh");
  _tokens.emplace_back("KaHIP");
  _tokens.emplace_back("Manual");
  _tokens.emplace_back("SD_DT_Fixed");
  _tokens.emplace_back("Sparse");
  _tokens.emplace_back("Dense");
  _tokens.emplace_back("SD_DT_Asynchronous");
}

AnnotationValue EvalAnnotation::foldTraverseElement(AST_Expression e)
{
  auto av = AnnotationValue();
  switch (e->expressionType()) {
  case EXPSTRING:
    av.setStr(e->getAsString()->print());
    av.setPlainStr(e->getAsString()->str());
    break;
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = e->getAsComponentReference();
    string name = cr->name();
    Option<Variable> vi = ModelConfig::instance().lookup(name);
    if (vi) {
      if (vi->isConstant()) {
        av.setInteger(vi->value());
      }
    } else {
      if (std::find(_tokens.begin(), _tokens.end(), name) != _tokens.end()) {
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
    av.setInteger(static_cast<int>(e->getAsReal()->val()));
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
  auto av = AnnotationValue();
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

}  // namespace IR
}  // namespace MicroModelica
