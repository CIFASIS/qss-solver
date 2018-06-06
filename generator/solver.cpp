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

#include "solver.h"

#include <sstream>
#include <utility>

#include "../ast/expression.h"
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/equation.h"
#include "../ir/event.h"
#include "../ir/expression.h"
#include "../ir/statement.h"
#include "../util/error.h"
#include "../util/util.h"

/* QSS Solver class. */

QSS::QSS(MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer) :
    _flags(flags), 
    _writer(writer), 
    _model(model), 
    _name(model.name())
{
}

QSS::~QSS()
{
}

string
QSS::modelHeader()
{
  return "";
}

string
QSS::runCmd()
{
  return "";
}

string
QSS::makefile(SOL_Makefile m)
{
  return "";
}

void
QSS::initializeDataStructures()
{
  return;
}

void
QSS::modelDefinition()
{
  return;
}

void
QSS::modelDependencies()
{
  return;
}

void
QSS::zeroCrossing()
{
  return;
}

void
QSS::handler()
{
  return;
}

void
QSS::output()
{
  return;
}
    
Graph
QSS::computationalGraph()
{
  return Graph(0, 0);
}

/* Classic Solver class. */

Classic::Classic(MMO_Model model, MMO_CompileFlags flags, MMO_Writer writer) :
  _flags(flags), 
  _model(model), 
  _writer(writer),
  _name(model->name())
{
  if(_flags->hasOutputFile())
  {
    _name = _flags->outputFileName();
  }
}

Classic::~Classic()
{
}

string
Classic::modelHeader()
{
  return "";
}

string
Classic::runCmd()
{
  return "";
}
    
string
Classic::makefile(SOL_Makefile m)
{
  return "";
}
    
void
Classic::initializeDataStructures()
{
}
    
void
Classic::modelDefinition()
{
}
    
void
Classic::modelDependencies()
{
}
    
void
Classic::zeroCrossing()
{
}
    
void
Classic::handler()
{
}
    
void
Classic::output()
{
}
 
Graph
Classic::computationalGraph()
{
  return Graph(0,0);
}

SolverCommon::SolverCommon(MMO_Model model, MMO_CompileFlags flags,
    MMO_Writer writer, MMO_DependenciesTable modelVectorDeps, Graph *graph) :
    _model(model), _flags(flags), _writer(writer), _modelVectorDeps(
        modelVectorDeps), _name(model->name()), _weights(), _graph(*graph), _generateGraph(
        true), _parallel(flags->graph())
{
  if(_flags->hasOutputFile())
  {
    _name = _flags->outputFileName();
  }
}

SolverCommon::~SolverCommon()
{
}

void
SolverCommon::addModelDeps(Dependencies deps, Index state, Index infIndex,
    MMO_DependenciesTable modelDeps, int index)
{
  Dependencies d = modelDeps->lookup(state);
  if(d == NULL)
  {
    d = newDependencies();
  }
  d->insert(infIndex, DEP_STATE);
  if(infIndex.isArray() && !infIndex.hasRange())
  {
    d->join(deps, infIndex, DEP_ALGEBRAIC, index);
  }
  else
  {
    d->join(deps, infIndex, DEP_ALGEBRAIC);
  }
  d->join(deps, infIndex, DEP_ALGEBRAIC_VECTOR);
  modelDeps->insert(state, d);
}

void
SolverCommon::graphInsert(Index row, Index col, int offset, NOD_Type type,
    int assignments)
{
  if(_model->annotation()->partitionMethod() == ANT_Manual
      || _generateGraph == false)
  {
    return;
  }
  if(row.hasMap() || col.hasMap())
  {
    _generateGraph = false;
    return;
  }
  int rowOffset = 0, colOffset = 0;
  if(type == NOD_HD)
  {
    rowOffset = offset;
  }
  else if(type == NOD_HZ || type == NOD_DD)
  {
    colOffset = offset;
    rowOffset = offset;
  }
  else if(type == NOD_SZ)
  {
    colOffset = offset;
  }
  if(row.hasRange())
  {
    int i, begin = row.begin(), end = row.end();
    for(i = begin; i <= end; i++)
    {
      if(!((type == NOD_SD || type == NOD_HZ || type == NOD_DD)
          && row.mappedValue(i) == col.mappedValue(i)))
      {
        _graph.addGraphEdge(row.mappedValue(i) + rowOffset,
            col.mappedValue(i) + colOffset);
        _graph.addGraphEdge(col.mappedValue(i) + colOffset,
            row.mappedValue(i) + rowOffset);
        _graph.addHyperGraphEdge(row.mappedValue(i) + rowOffset,
            col.mappedValue(i) + colOffset);
        _graph.addHyperGraphEdge(row.mappedValue(i) + rowOffset,
            row.mappedValue(i) + rowOffset);
        _graph.addNodeWeight(row.mappedValue(i) + rowOffset, assignments);
      }
    }
  }
  else
  {
    if(!((type == NOD_SD || type == NOD_HZ || type == NOD_DD)
        && row.mappedValue() == col.mappedValue()))
    {
      _graph.addGraphEdge(row.mappedValue() + rowOffset,
          col.mappedValue() + colOffset);
      _graph.addGraphEdge(col.mappedValue() + colOffset,
          row.mappedValue() + rowOffset);
      _graph.addHyperGraphEdge(row.mappedValue() + rowOffset,
          col.mappedValue() + colOffset);
      _graph.addHyperGraphEdge(row.mappedValue() + rowOffset,
          row.mappedValue() + rowOffset);
      _graph.addNodeWeight(row.mappedValue() + rowOffset, assignments);
    }
  }
}

void
SolverCommon::addAlgebriacDeps(Dependencies deps, Index derivativeIndex,
    map<Index, Index> states, string alloc, string allocInverse, string init,
    string initInverse, WR_Section allocSection, WR_Section allocInverseSection,
    WR_Section initSection,
    WR_Section initInverseSection, string type, string inverseType,
    DEP_Type depType, MMO_DependenciesTable modelDeps)
{
  bool matrix = true;
  if(allocInverseSection == WR_NULL)
  {
    matrix = false;
  }
  bool eval = false;
  int nodOffset = 0;
  NOD_Type nt = NOD_SD;
  if(_parallel)
  {
    if(allocInverseSection == WR_ALLOC_LD_ALG_SZ
        || allocSection == WR_ALLOC_EVENT_ALG_RHSST
        || allocInverseSection == WR_ALLOC_LD_ALG_SD)
    {
      eval = true;
    }
    if(allocInverseSection == WR_ALLOC_LD_ALG_SZ
        || allocSection == WR_ALLOC_EVENT_ALG_RHSST)
    {
      nodOffset = _model->states();
      nt = NOD_SZ;
    }
  }
  for(Index *index = deps->begin(depType); !deps->end(depType);
      index = deps->next(depType))
  {
    Index eqIndex = *index;
    Index idx = deps->key(depType);
    if(states.find(idx) == states.end())
    {
      stringstream buffer;
      string indent;
      string variableString = "i";
      if(idx.hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[i];
        stringstream bufferGen;
        string eqIdx = derivativeIndex.print("i");
        string stIdx = idx.print(variableString);
        bufferGen << "for ( i0 = " << eqIndex.begin() << "; i0 <= "
            << eqIndex.end() << "; i0++) ";
        _writer->write(&bufferGen, allocSection, false);
        _writer->write("{", allocSection);
        buffer << indent << alloc;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "++;";
        _writer->write(&buffer, allocSection);
        _writer->write("}", allocSection);
        _writer->write(&bufferGen, initSection, false);
        _writer->write("{", initSection);
        buffer << indent << init;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
        _writer->write(&buffer, initSection);
        _writer->write("}", initSection);
        _writer->write(&bufferGen, allocInverseSection, false);
        _writer->write("{", allocInverseSection);
        buffer << indent << allocInverse << "[" << stIdx << "]++;";
        _writer->write(&buffer, allocInverseSection);
        _writer->write("}", allocInverseSection);
        _writer->write(&bufferGen, initInverseSection);
        _writer->write("{", initInverseSection);
        buffer << indent << initInverse << "[" << stIdx << "][" << inverseType
            << "[" << stIdx << "]++] = " << eqIdx << ";";
        _writer->write(&buffer, initInverseSection);
        _writer->write("}", initInverseSection);
        Index infIndex = derivativeIndex;
        if(modelDeps != NULL)
        {
          idx.setHi(eqIndex.hi());
          idx.setLow(eqIndex.low());
          addModelDeps(deps, idx, derivativeIndex, modelDeps);
        }
        if(eval && _parallel)
        {
          graphInsert(idx, derivativeIndex, nodOffset, nt);
        }
      }
      else if(!idx.hasRange() && eqIndex.hasRange())
      {
        // X[i] = B[N];
        stringstream bufferGen;
        string eqIdx = derivativeIndex.print("i");
        string stIdx = idx.print(variableString); // Has to be a constant definition.
        bufferGen << "for ( i0 = " << eqIndex.begin() << "; i0 <= "
            << eqIndex.end() << "; i0++) ";
        _writer->write(&bufferGen, allocSection, false);
        _writer->write("{", allocSection);
        buffer << indent << alloc;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "++;";
        _writer->write(&buffer, allocSection);
        _writer->write("}", allocSection);
        _writer->write(&bufferGen, initSection, false);
        _writer->write("{", initSection);
        buffer << indent << init;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
        _writer->write(&buffer, initSection);
        _writer->write("}", initSection);
        buffer << indent << allocInverse << "[" << stIdx << "] += "
            << eqIndex.range() << ";";
        _writer->write(&buffer, allocInverseSection);
        _writer->write(&bufferGen, initInverseSection);
        _writer->write("{", initInverseSection);
        buffer << indent << initInverse << "[" << stIdx << "][" << inverseType
            << "[" << stIdx << "]++] = " << eqIdx << ";";
        _writer->write(&buffer, initInverseSection);
        _writer->write("}", initInverseSection);
        if(modelDeps != NULL)
        {
          addModelDeps(deps, idx, derivativeIndex, modelDeps);
        }
        if(eval && _parallel)
        {
          graphInsert(idx, derivativeIndex, nodOffset, nt);
        }
      }
      else if(idx.hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[i];
        Error::getInstance()->add(0, EM_CG | EM_ALG_INDEX, ER_Error,
            "Algebraic index out of range %s.", idx.print().c_str());
      }
      else if(!idx.hasRange() && !eqIndex.hasRange())
      {
        // X[N] = B[N];
        int eqIdx = derivativeIndex.mappedValue(eqIndex.constant());
        string stIdx = idx.print(variableString);
        buffer << indent << alloc;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "++;";
        _writer->write(&buffer, allocSection);
        buffer << indent << init;
        if(matrix)
        {
          buffer << "[" << eqIdx << "]";
        }
        buffer << "[" << type << "[" << eqIdx << "]++] = " << stIdx << ";";
        _writer->write(&buffer, initSection);
        buffer << indent << allocInverse << "[" << stIdx << "]++;";
        _writer->write(&buffer, allocInverseSection);
        buffer << indent << initInverse << "[" << stIdx << "][" << inverseType
            << "[" << stIdx << "]++] = " << eqIdx << ";";
        _writer->write(&buffer, initInverseSection);
        if(modelDeps != NULL)
        {
          addModelDeps(deps, idx,
              derivativeIndex.indexValue(eqIndex.operConstant()), modelDeps,
              eqIndex.constant());
        }
        if(eval && _parallel)
        {
          graphInsert(idx, derivativeIndex.indexValue(eqIndex.operConstant()),
              nodOffset, nt);
        }
      }
    }
  }
}

string
SolverCommon::initOutput()
{
  stringstream buffer;
  MMO_Annotation annot = _model->annotation();
  string period = "NULL";
  string indent = "";
  int ssize = 0;
  if(annot->commInterval() == "CI_Sampled")
  {
    indent = _writer->indent(1);
    list<double> sample = annot->sample();
    ssize = sample.size();
    period = "period";
    buffer << indent << "double period[" << ssize << "];" << endl;
    int n = 0;
    for(list<double>::iterator i = sample.begin(); i != sample.end(); i++)
    {
      buffer << indent << "period[" << n << "] = " << *i << ";" << endl;
    }
  }
  string outputFunction = "NULL";
  if(_model->outs())
  {
    outputFunction = "MOD_output";
  }
  buffer << indent << "simulator->output = SD_Output(\"" << _name << "\","
      << _model->outs() << ",";
  buffer << _model->discretes() << "," << _model->states() << "," << period
      << "," << ssize << ",";
  buffer << annot->initialTime() << "," << annot->commInterval() << ","
      << annot->storeData();
  buffer << "," << outputFunction << ");" << endl;
  buffer << indent << "SD_output modelOutput = simulator->output;" << endl;
  return buffer.str();
}

void
SolverCommon::print(list<string> exps, WR_Section section)
{
  for(list<string>::iterator it = exps.begin(); it != exps.end(); it++)
  {
    _writer->write(*it, section);
  }
}

void
SolverCommon::insertLocalVariables(map<string, string> *local,
    list<string> vars)
{
  for(list<string>::iterator it = vars.begin(); it != vars.end(); it++)
  {
    local->insert(pair<string, string>(*it, *it));
  }
}

void
SolverCommon::addLocalVar(string name, map<string, string> *variables,
    int dimensions, string type, int size)
{
  for(int i = 0; i < dimensions; i++)
  {
    stringstream v;
    v << name << i;
    addLocalVar(v.str(), variables, type, size);
    v.str("");
  }
}

int
SolverCommon::beginForLoops(Index idx, WR_Section section)
{
  stringstream buffer;
  int dim = idx.dimension(), idt = 0;
  for(int i = 0; i < dim; i++)
  {
    string indent = _writer->indent(idt);
    if(idx.hasRange(i))
    {
      buffer << indent << "for(i" << i << " = " << idx.begin(i) << "; i" << i
          << " <= " << idx.end(i) << "; i" << i << "++)";
      _writer->write(&buffer, section);
      buffer << indent << "{";
      _writer->write(&buffer, section);
      idt++;
    }
  }
  return idt;
}

void
SolverCommon::endForLoops(Index idx, WR_Section section)
{
  stringstream buffer;
  int dim = idx.dimension(), idt = dim - 1;
  for(int i = 0; i < dim; i++)
  {
    string indent = _writer->indent(idt);
    if(idx.hasRange(i))
    {
      buffer << indent << "}";
      _writer->write(&buffer, section);
    }
    idt--;
  }
}

void
SolverCommon::addLocalVar(string name, map<string, string> *variables,
    string type, int size, string assign)
{
  string var;
  if(type.empty())
  {
    var = "int " + name;
  }
  else
  {
    var = type + " " + name;
  }
  if(size > 0)
  {
    stringstream buffer;
    buffer << "[" << size << "];";
    var.append(buffer.str());
  }
  else
  {
    stringstream buffer;
    buffer << " = " << assign << ";";
    var.append(buffer.str());
  }
  if(variables->find(var) == variables->end())
  {
    variables->insert(pair<string, string>(var, var));
  }
}

void
SolverCommon::eventPrologue(Index idx, WR_Section simple, WR_Section generic)
{
  stringstream buffer;
  string indent = _writer->indent(1);
  if(idx.hasRange())
  {
    buffer << "if(i0 >= " << idx.mappedBegin() << " && i0 <= "
        << idx.mappedEnd() << ")";
    _writer->write(&buffer, generic);
    buffer << "{";
    _writer->write(&buffer, generic);
  }
  else
  {
    buffer << indent << "case " << idx.print() << ":";
    _writer->write(&buffer, simple);
  }
}

void
SolverCommon::eventEpilogue(Index idx, WR_Section simple, WR_Section generic)
{
  stringstream buffer;
  string indent = _writer->indent(2);
  if(idx.hasRange())
  {
    buffer << "}";
    _writer->write(&buffer, generic);
  }
  else
  {
    buffer << indent << "return;";
    _writer->write(&buffer, simple);
  }
}

void
SolverCommon::handler(VST_Environment type,
    map<string, string> *handlerPosVars, map<string, string> *handlerNegVars)
{
  MMO_EventTable events = _model->events();
  VarSymbolTable vt = _model->varTable();
  stringstream buffer;
  vt->setPrintEnvironment(type);
  string indent = _writer->indent(2);
  addLocalVar("i0", handlerPosVars, "int", 0, "i");
  addLocalVar("i0", handlerNegVars, "int", 0, "i");
  for(MMO_Event ev = events->begin(); !events->end(); ev = events->next())
  {
    Index idx = events->key();
    if(ev->hasPositiveHandler())
    {
      eventPrologue(idx, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC);
    }
    for(MMO_Statement st = ev->begin(HND_POSITIVE); !ev->end(); st = ev->next())
    {
      if(idx.hasRange())
      {
        print(
            st->print(_writer->indent(1), "i", -idx.mappedBegin() + idx.begin(),
                1, idx.low()), WR_HANDLER_POS_GENERIC);
      }
      else
      {
        print(st->print(indent), WR_HANDLER_POS_SIMPLE);
      }
      insertLocalVariables(handlerPosVars, st->getVariables());
    }
    if(ev->hasPositiveHandler())
    {
      eventEpilogue(idx, WR_HANDLER_POS_SIMPLE, WR_HANDLER_POS_GENERIC);
    }
    if(ev->hasNegativeHandler())
    {
      eventPrologue(idx, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC);
    }
    for(MMO_Statement st = ev->begin(HND_NEGATIVE); !ev->end(); st = ev->next())
    {
      if(idx.hasRange())
      {
        print(
            st->print(_writer->indent(1), "i", -idx.mappedBegin() + idx.begin(),
                1, idx.low()), WR_HANDLER_NEG_GENERIC);
      }
      else
      {
        print(st->print(indent), WR_HANDLER_NEG_SIMPLE);
      }
      insertLocalVariables(handlerNegVars, st->getVariables());
    }
    if(ev->hasNegativeHandler())
    {
      eventEpilogue(idx, WR_HANDLER_NEG_SIMPLE, WR_HANDLER_NEG_GENERIC);
    }
  }
}

void
SolverCommon::zeroCrossing(VST_Environment type, map<string, string> *zcVars)
{
  MMO_EventTable events = _model->events();
  VarSymbolTable vt = _model->varTable();
  MMO_EquationTable algebraics = _model->algebraics();
  stringstream buffer;
  vt->setPrintEnvironment(type);
  string indent = _writer->indent(1);
  int order = getOrder();
  addLocalVar("i0", zcVars, "int", 0, "i");
  for(MMO_Event ev = events->begin(); !events->end(); ev = events->next())
  {
    Index idx = events->key();
    if(ev->hasWeight())
    {
      _weights.insert(pair<Index, double>(idx, ev->weight()));
    }
    MMO_Equation exp = ev->condition();
    if(idx.hasRange())
    {
      eventPrologue(idx, WR_ZC_SIMPLE, WR_ZC_GENERIC);
      print(
          exp->print(indent, "zc", "i", true, algebraics, EQ_ZC, order, false,
              -idx.mappedBegin() + idx.begin(), true, idx.low()),
          WR_ZC_GENERIC);
      buffer << "}";
      _writer->write(&buffer, WR_ZC_GENERIC);
      insertLocalVariables(zcVars, exp->getVariables());
    }
    else
    {
      eventPrologue(idx, WR_ZC_SIMPLE, WR_ZC_GENERIC);
      print(
          exp->print(_writer->indent(2), "zc", "i", true, algebraics, EQ_ZC,
              order, true), WR_ZC_SIMPLE);
      buffer << _writer->indent(2) << "return;";
      _writer->write(&buffer, WR_ZC_SIMPLE);
      insertLocalVariables(zcVars, exp->getVariables());
    }
  }
}

map<Index, double>
SolverCommon::weights()
{
  return _weights;
}

int
SolverCommon::getOrder()
{
  MMO_Annotation annot = _model->annotation();
  int order = 1;
  if(annot->symDiff())
  {
    order = annot->order();
  }
  return order;
}

void
SolverCommon::printCleanVector(string var, int size)
{
  stringstream buffer;
  string indent = _writer->indent(1);
  buffer << indent << "cleanVector(" << var << "," << 0 << "," << size << ");"
      << endl;
  _writer->print(&buffer);
}

void
SolverCommon::printSection(string var, int size, WR_Section section)
{
  if(!_writer->isEmpty(section))
  {
    printCleanVector(var, size);
    _writer->print(section);
  }
}

void
SolverCommon::initialCode(map<string, string> *initializeVars)
{
  string indent = _writer->indent(1);
  MMO_StatementTable st = _model->initialCode();
  _model->varTable()->setPrintEnvironment(VST_INIT);
  if(_model->annotation()->classic())
  {
    _model->varTable()->setPrintEnvironment(VST_CLASSIC_INIT);
  }
  for(MMO_Statement stm = st->begin(); !st->end(); stm = st->next())
  {
    list<string> stms = stm->print(indent);
    for(list<string>::iterator it = stms.begin(); it != stms.end(); it++)
    {
      _writer->write(*it, WR_INIT_CODE);
    }
    insertLocalVariables(initializeVars, stm->getVariables());
  }
}

void
SolverCommon::output(VST_Environment type, map<string, string> *outputVars)
{
  MMO_EquationTable outputs = _model->outputs();
  VarSymbolTable vt = _model->varTable();
  stringstream buffer, bufferGen, bufferGenDsc, bufferGenInit;
  vt->setPrintEnvironment(type);
  string indent = _writer->indent(1);
  bool genericEquation = false;
  bool hasInit = false, hasInitDsc = false, hasOutput = false;
  for(MMO_Equation eq = outputs->begin(); !outputs->end(); eq = outputs->next())
  {
    Index index = outputs->key();
    string indexStr = index.print("i");
    Dependencies deps = eq->exp()->deps();
    if(outputs->endGenericDefinition())
    {
      genericEquation = false;
      if(hasInit || hasInitDsc || hasOutput)
      {
        indent = "";
        _writer->write("}", WR_INIT_OUTPUT);
        hasOutput = false;
      }

      if(hasInit)
      {
        indent = "";
        hasInit = false;
        _writer->write("}", WR_ALLOC_OUTPUT_STATES);
        _writer->write("}", WR_INIT_OUTPUT_STATES);
      }
      if(hasInitDsc)
      {
        indent = "";
        hasInitDsc = false;
        _writer->write("}", WR_ALLOC_OUTPUT_DSC);
        _writer->write("}", WR_INIT_OUTPUT_DSC);
      }
    }
    if(outputs->beginGenericDefinition())
    {
      genericEquation = true;
      stringstream bufferTemp;
      bufferTemp << "for(i0 = " << index.begin() << "; i0 <= " << index.end()
          << "; i0++)";
      if(!hasInit)
      {
        bufferGen << bufferTemp.str();
      }
      if(!hasInitDsc)
      {
        bufferGenDsc << bufferTemp.str();
      }
      bufferGenInit << bufferTemp.str();
    }
    if(((deps->hasStates() || deps->hasDiscretes() || deps->hasAlgebraicStates()
        || deps->hasAlgebraicDiscretes()) && genericEquation)
        && !bufferGenInit.str().empty())
    {
      hasOutput = deps->hasAlgebraicStates() || deps->hasAlgebraicDiscretes();
      _writer->write(&bufferGenInit, WR_INIT_OUTPUT);
      _writer->write("{", WR_INIT_OUTPUT);
    }
    if((deps->hasStates() && !bufferGen.str().empty()) && genericEquation)
    {
      indent = _writer->indent(1);
      hasInit = true;
      _writer->write(&bufferGen, WR_ALLOC_OUTPUT_STATES, false);
      _writer->write(&bufferGen, WR_INIT_OUTPUT_STATES);
      _writer->write("{", WR_ALLOC_OUTPUT_STATES);
      _writer->write("{", WR_INIT_OUTPUT_STATES);
    }
    if((deps->hasDiscretes() && !bufferGenDsc.str().empty()) && genericEquation)
    {
      indent = _writer->indent(1);
      hasInitDsc = true;
      _writer->write(&bufferGenDsc, WR_ALLOC_OUTPUT_DSC, false);
      _writer->write(&bufferGenDsc, WR_INIT_OUTPUT_DSC);
      _writer->write("{", WR_ALLOC_OUTPUT_DSC);
      _writer->write("{", WR_INIT_OUTPUT_DSC);
    }
    if(index.hasRange())
    {
      genericDefCondition(index, eq->lhs(), WR_OUTPUT_GENERIC, outputVars);
      _model->varTable()->setPrintEnvironment(type);
      print(
          eq->print(indent, "out", "j", true, _model->algebraics(), EQ_OUTPUT,
              1, false, 0, false, index.low()), WR_OUTPUT_GENERIC);
      buffer << "}";
      insertLocalVariables(outputVars, eq->getVariables());
      _writer->write(&buffer, WR_OUTPUT_GENERIC);
      _model->varTable()->setPrintEnvironment(VST_OUTPUT);
      string eqExpStr = eq->exp()->print("%d");
      if(eq->exp()->exp()->expressionType() == EXPNAMEDARGUMENT)
      {
        eqExpStr = *(eq->exp()->exp()->getAsNamedArgument()->name());
        if(eq->exp()->hasIndexes())
        {
          eqExpStr.append("[%d]");
        }
        buffer << indent << "sprintf(modelOutput->variable[" << indexStr
            << "].name,\"" << eqExpStr << "\",i0);";
        _writer->write(&buffer, WR_INIT_OUTPUT);
      }
      else
      {
        buffer << indent << "sprintf(modelOutput->variable[" << indexStr
            << "].name,\"" << eqExpStr << "\",";
        buffer << eq->exp()->indexes("i0") << ");";
        _writer->write(&buffer, WR_INIT_OUTPUT);
      }
    }
    else
    {
      buffer << indent << "case " << eq->lhs().mappedConstant() << ":";
      _writer->write(&buffer, WR_OUTPUT_SIMPLE);
      _model->varTable()->setPrintEnvironment(type);
      print(
          eq->print(_writer->indent(2), "out", "", true, _model->algebraics(),
              EQ_OUTPUT, 1, false, 0, false), WR_OUTPUT_SIMPLE);
      insertLocalVariables(outputVars, eq->getVariables());
      buffer << _writer->indent(2) << "return;";
      _writer->write(&buffer, WR_OUTPUT_SIMPLE);
      _model->varTable()->setPrintEnvironment(VST_OUTPUT);
      string eqExpStr = eq->exp()->print("");
      if(eq->exp()->exp()->expressionType() == EXPNAMEDARGUMENT)
      {
        eqExpStr = *(eq->exp()->exp()->getAsNamedArgument()->name());
      }
      buffer << indent << "sprintf(modelOutput->variable[" << indexStr
          << "].name,\"" << eqExpStr << "\");";
      _writer->write(&buffer, WR_INIT_OUTPUT);
    }
    if(deps->states())
    {
      buffer << indent << "modelOutput->nOS[" << indexStr << "] = "
          << deps->states() << ";";
      _writer->write(&buffer, WR_ALLOC_OUTPUT_STATES);
    }
    map<Index, Index> defStates;
    for(Index *idx = deps->begin(DEP_STATE); !deps->end(DEP_STATE);
        idx = deps->next(DEP_STATE))
    {
      defStates[*idx] = *idx;
      string idxStr = idx->print("i", -index.begin());
      buffer << indent << "modelOutput->nSO[" << idxStr << "]++;";
      _writer->write(&buffer, WR_ALLOC_OUTPUT_STATES);
      buffer << indent << "modelOutput->SO[" << idxStr << "][states[" << idxStr
          << "]++] = " << indexStr << ";";
      _writer->write(&buffer, WR_INIT_OUTPUT_STATES);
      buffer << indent << "modelOutput->OS[" << indexStr << "][outputs["
          << indexStr << "]++] = " << idxStr << ";";
      _writer->write(&buffer, WR_INIT_OUTPUT_STATES);
    }
    addAlgebriacDeps(deps, eq->lhs(), defStates, "modelOutput->nOS",
        "modelOutput->nSO", "modelOutput->OS", "modelOutput->SO",
        WR_ALLOC_OUTPUT_ALG_STATES,
        WR_ALLOC_OUTPUT_ALG_STATES, WR_INIT_OUTPUT_ALG_STATES,
        WR_INIT_OUTPUT_ALG_STATES, "outputs", "states");
    if(deps->discretes())
    {
      buffer << indent << "modelOutput->nOD[" << indexStr << "] = "
          << deps->discretes() << ";";
      _writer->write(&buffer, WR_ALLOC_OUTPUT_DSC);
    }
    defStates.clear();
    for(Index *idx = deps->begin(DEP_DISCRETE); !deps->end(DEP_DISCRETE); idx =
        deps->next(DEP_DISCRETE))
    {
      defStates[*idx] = *idx;
      string idxStr = idx->print("i", -index.begin());
      buffer << indent << "modelOutput->nDO[" << idxStr << "]++;";
      _writer->write(&buffer, WR_ALLOC_OUTPUT_DSC);
      buffer << indent << "modelOutput->DO[" << idxStr << "][discretes["
          << idxStr << "]++] = " << index.print("i") << ";";
      _writer->write(&buffer, WR_INIT_OUTPUT_DSC);
      buffer << indent << "modelOutput->OD[" << indexStr << "][outputs["
          << indexStr << "]++] = " << idxStr << ";";
      _writer->write(&buffer, WR_INIT_OUTPUT_DSC);
    }
    addAlgebriacDeps(deps, eq->lhs(), defStates, "modelOutput->nOD",
        "modelOutput->nDO", "modelOutput->OD", "modelOutput->DO",
        WR_ALLOC_OUTPUT_ALG_DSC,
        WR_ALLOC_OUTPUT_ALG_DSC, WR_INIT_OUTPUT_ALG_DSC, WR_INIT_OUTPUT_ALG_DSC,
        "outputs", "discretes", DEP_ALGEBRAIC_DISCRETE);
    vectorDependencies(index, deps, WR_ALLOC_OUTPUT_STATES,
        WR_INIT_OUTPUT_STATES, "modelOutput->nOS", "modelOutput->OS",
        WR_ALLOC_OUTPUT_STATES,
        WR_INIT_OUTPUT_STATES, "modelOutput->nSO", "modelOutput->SO", "outputs",
        "states", false, DEP_STATE_VECTOR, outputVars);
    vectorDependencies(index, deps, WR_ALLOC_OUTPUT_DSC, WR_INIT_OUTPUT_DSC,
        "modelOutput->nOD", "modelOutput->OD", WR_ALLOC_OUTPUT_DSC,
        WR_INIT_OUTPUT_DSC, "modelOutput->nDO", "modelOutput->DO", "outputs",
        "discretes", false, DEP_DISCRETE_VECTOR, outputVars);
  }
  if((genericEquation && hasInit) || (genericEquation && hasInitDsc)
      || (genericEquation && hasOutput))
  {
    _writer->write("}", WR_INIT_OUTPUT);
  }
  if(genericEquation && hasInit)
  {
    _writer->write("}", WR_ALLOC_OUTPUT_STATES);
    _writer->write("}", WR_INIT_OUTPUT_STATES);
  }
  if(genericEquation && hasInitDsc)
  {
    _writer->write("}", WR_ALLOC_OUTPUT_DSC);
    _writer->write("}", WR_INIT_OUTPUT_DSC);
  }
}

void
SolverCommon::genericDefCondition(Index eqIdx, Index idx, WR_Section section,
    map<string, string> *variables)
{
  stringstream buffer;
  addLocalVar("j0", variables);
  if(!_model->annotation()->classic() || section == WR_OUTPUT_GENERIC)
  {
    addLocalVar("i0", variables, "int", 0, "i");
  }
  if(idx.factor() == 1)
  {
    buffer << "j0 = " << idx.printReverse("i") << ";";
    _writer->write(&buffer, section);
    buffer << "if(j0 >=" << idx.begin() << " && j0 <= " << idx.end() << ")";
    _writer->write(&buffer, section);
    buffer << "{";
    _writer->write(&buffer, section);
  }
  else if(eqIdx.factor() != 0)
  {
    string tmp = Util::getInstance()->newVarName("tmp", _model->varTable());
    addLocalVar(tmp, variables, "double");
    buffer << tmp << " = (double)" << idx.printReverse("i") << ";";
    _writer->write(&buffer, section);
    buffer << "if((int)" << tmp << " == " << tmp << " && " << tmp << " >= "
        << eqIdx.begin() << " && " << tmp << " <= " << eqIdx.end() << ")";
    _writer->write(&buffer, section);
    buffer << "{";
    _writer->write(&buffer, section);
    buffer << "j0 = (int) " << tmp << ";";
    _writer->write(&buffer, section);

  }
  else
  {
    buffer << "j0 = i;";
    _writer->write(&buffer, section);
    buffer << "if(j0 == " << idx.printReverse("i") << ")";
    _writer->write(&buffer, section);
    buffer << "{";
    _writer->write(&buffer, section);
  }
}

void
SolverCommon::vectorDependencies(Index index, Dependencies deps,
    WR_Section alloc, WR_Section init, string allocString, string initString,
    WR_Section allocInverse, WR_Section initInverse, string allocInverseString,
    string initInverseString, string count,
    string countInverse, bool dependencies, DEP_Type type,
    map<string, string> *variables)
{
  stringstream buffer;
  string indent = _writer->indent(1);
  bool genericIndex = false;
  string forIdx = "i0";
  bool eval = false;
  int offset = 0;
  NOD_Type nt = NOD_SD;
  if(_parallel)
  {
    if(alloc == WR_ALLOC_LD_DD)
    {
      offset = _model->states();
      nt = NOD_DD;
      eval = true;
    }
    if(allocInverse == WR_ALLOC_LD_SZ)
    {
      offset = _model->states();
      nt = NOD_SZ;
    }
    if(allocInverse == WR_ALLOC_LD_SZ || allocInverse == WR_ALLOC_LD_SD)
    {
      eval = true;
    }
  }
  if(index.factor() != 0)
  {
    genericIndex = true;
    forIdx = "j0";
    addLocalVar("j0", variables);
  }
  string eqsIdx = index.print("i");
  int numDeps = deps->stateVectors();
  if(type == DEP_DISCRETE_VECTOR)
  {
    numDeps = deps->discreteVectors();
  }
  if(numDeps)
  {
    buffer << indent << allocString << "[" << eqsIdx << "] += " << numDeps
        << ";";
    _writer->write(&buffer, alloc);
  }
  for(Index *idx = deps->begin(type); !deps->end(type); idx = deps->next(type))
  {
    Index dIdx(*idx);
    string sIdx = dIdx.print(forIdx, 0, false);
    buffer << "for(" << forIdx << " = " << idx->begin() << "; " << forIdx
        << " <= " << idx->end() << "; " << forIdx << "++)";
    _writer->write(&buffer, init, false);
    _writer->write("{", init);
    if(init != initInverse && !initInverseString.empty())
    {
      _writer->write(&buffer, initInverse, false);
      _writer->write("{", initInverse);
    }
    if(genericIndex)
    {
      buffer.str("");
    }
    else if(!allocInverseString.empty())
    {
      _writer->write(&buffer, allocInverse);
      _writer->write("{", allocInverse);
    }
    buffer << indent << initString << "[" << eqsIdx << "][" << count << "["
        << eqsIdx << "]++] = " << sIdx << ";";
    _writer->write(&buffer, init);
    if(!allocInverseString.empty())
    {
      if(genericIndex)
      {
        buffer << indent << allocInverseString << "[" << dIdx.print("i")
            << "] += " << index.range() << ";";
        _writer->write(&buffer, allocInverse);
      }
      else
      {
        buffer << indent << allocInverseString << "[" << sIdx << "]++;";
        _writer->write(&buffer, allocInverse);
      }
    }
    if(!initInverseString.empty())
    {
      buffer << indent << initInverseString << "[" << sIdx << "]["
          << countInverse << "[" << sIdx << "]++] = " << eqsIdx << ";";
      _writer->write(&buffer, initInverse);
    }
    if(!genericIndex && !allocInverseString.empty())
    {
      _writer->write("}", allocInverse);
    }
    if(init != initInverse && !initInverseString.empty())
    {
      _writer->write("}", initInverse);
    }
    _writer->write("}", init);
    if(dependencies)
    {
      Dependencies d = _modelVectorDeps->lookup(dIdx);
      if(d == NULL)
      {
        d = newDependencies();
      }
      d->insert(index, DEP_STATE_VECTOR);
      _modelVectorDeps->insert(dIdx, d);
    }
    if(eval && _parallel)
    {
      graphInsert(dIdx, index, offset, nt);
    }
  }
}

bool
SolverCommon::hasGraph()
{
  return _generateGraph;
}

void
SolverCommon::settings()
{
  stringstream buffer;
  buffer << "void\nMOD_settings(SD_simulationSettings settings)\n{";
  _writer->print(&buffer);
  buffer << "\t settings->debug = " << _flags->debug() << ";";
  _writer->print(&buffer);
  if(_flags->parallel())
  {
    buffer << "\t settings->parallel = TRUE;";
    _writer->print(&buffer);
  }
  else
  {
    buffer << "\t settings->parallel = FALSE;";
    _writer->print(&buffer);
  }
  if(_model->evs() > 0)
  {
    buffer << "\t settings->hybrid = TRUE;";
    _writer->print(&buffer);
  }
  else
  {
    buffer << "\t settings->hybrid = FALSE;";
    _writer->print(&buffer);
  }
  buffer << "\t settings->method = " << _model->annotation()->solver() << ";";
  _writer->print(&buffer);
  _writer->print("}\n");
}

