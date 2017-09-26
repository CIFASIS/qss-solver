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

#include <sstream>

#include "generator.h"

#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../ir/mmo_util.h"
#include "../ir/statement.h"
#include "../ir/stored_definition.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "files.h"
#include "generator_utils.h"
#include "solver.h"

MMO_Generator_::MMO_Generator_(MMO_StoredDefinition std, MMO_CompileFlags flags) :
    _std(std), _model(NULL), _function(NULL), _package(NULL), _flags(flags), _solver(
    NULL), _files(NULL), _writer(NULL), _includes(), _fheader()
{
  if(_flags->output())
  {
    _writer = newMMO_MemoryWriter();
  }
  else
  {
    _writer = newMMO_FileWriter();
  }
}

MMO_Generator_::~MMO_Generator_()
{
  delete _writer;
  delete _files;
}

int
MMO_Generator_::generate()
{
  for(MMO_Class c = _std->begin(); _std->end(); c = _std->next())
  {
    string fname = c->name();
    switch(c->classType())
    {
      case CL_MODEL:
        {
        if(_flags->hasOutputFile())
        {
          fname = _flags->outputFile();
        }
        fname.append(".c");
        _writer->setFile(fname);
        _model = c->getAsModel();
        switch(_model->annotation()->solver())
        {
          case ANT_DOPRI:
          case ANT_DASSL:
          case ANT_CVODE_BDF:
          case ANT_IDA:
          case ANT_CVODE_AM:
            _solver = newClassic(_model, _flags, _writer);
            break;
          default:
            _solver = newQSS(_model, _flags, _writer);
        }
        _files = newMMO_Files(_model, _solver, _flags);
        _generateModel();
        _writer->clearFile();
        _files->makefile();
        _files->run();
        _files->plot();
        _files->settings(_model->annotation());
        if(_flags->graph())
        {
          _files->graph();
        }
        if(_model->calledFunctions()->count())
        {
          string ffname = c->name();
          if(_flags->hasOutputFile())
          {
            ffname = _flags->outputFileName();
          }
          ffname.append("_functions");
          _generateHeader(ffname);
          MMO_SymbolRefTable srt = _model->calledFunctions();
          for(MMO_FunctionInfo *i = srt->begin(); !srt->end(); i = srt->next())
          {
            _generateFunction(i->function(), ffname);
          }
          if(_flags->hasOutputFile())
          {
            ffname.insert(0, _flags->outputFilePath() + SLASH);
          }
          _generateFunctionHeader(ffname);
          ffname.append(".c");
          _writer->setFile(ffname);
          _writer->print(WR_FUNCTION_HEADER);
          _writer->print(WR_FUNCTION_CODE);
          _writer->clearFile();
        }
      }
        break;
      case CL_PACKAGE:
        {
          MMO_Package p = c->getAsPackage();
          _generatePackage(p);
        }
        break;
      default:
        break;
    }
  }
  return Error::getInstance()->errors();
}

void
MMO_Generator_::_generateHeader(string name)
{
  stringstream buffer;
  buffer << "#include <math.h>" << endl;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include \"" << name << ".h\"" << endl;
  _writer->write(&buffer, WR_FUNCTION_HEADER);
}

void
MMO_Generator_::_generateModel()
{
  _initSections();
  _variables();
  _header();
  _initMatrices();
  _solver->model();
  _solver->modelDeps();
  _solver->zeroCrossing();
  _solver->handler();
  _solver->output();
  _solver->initialCode();
  _variablesInitCode();
  _writer->print(WR_HEADER);
  _writer->print(WR_GLOBAL_VARS);
  _solver->print(SOL_MODEL_SETTINGS);
  _solver->print(SOL_MODEL);
  _solver->print(SOL_DEPS);
  if(!_writer->isEmpty(WR_ZC_SIMPLE) || !_writer->isEmpty(WR_ZC_GENERIC))
  {
    _solver->print(SOL_ZC);
    if(!_writer->isEmpty(WR_HANDLER_POS_SIMPLE)
        || !_writer->isEmpty(WR_HANDLER_POS_GENERIC))
    {
      _solver->print(SOL_HANDLER_POS);
    }
    if(!_writer->isEmpty(WR_HANDLER_NEG_SIMPLE)
        || !_writer->isEmpty(WR_HANDLER_NEG_GENERIC))
    {
      _solver->print(SOL_HANDLER_NEG);
    }
  }
  if(!_writer->isEmpty(WR_OUTPUT_SIMPLE)
      || !_writer->isEmpty(WR_OUTPUT_GENERIC))
  {
    _solver->print(SOL_OUTPUT);
  }
  _solver->print(SOL_INIT);
}

/*! \brief Generate the code needed by the header files.
 *
 */

void
MMO_Generator_::_header()
{
  stringstream buffer;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include <stdio.h>" << endl;
  buffer << "#include <string.h>" << endl;
  buffer << "#include <math.h>" << endl;
  buffer << endl;
  if(_model->hasExternalFunctions())
  {
    buffer << "#include \"" << _model->name() << "_functions.h\"" << endl;
  }
  if(_model->imps())
  {
    MMO_ImportTable it = _model->imports();
    for(string i = it->begin(); !it->end(); i = it->next())
    {
      buffer << "#include \"" << Util::getInstance()->packageName(i) << ".h\""
          << endl;
    }
  }
  buffer << "#include <common/utils.h>" << endl;
  buffer << endl;
  _writer->write(&buffer, WR_HEADER);
  _writer->write(_solver->initHeader(), WR_HEADER);
}

/*! \brief Generate code for the model variables.
 *
 * 	Description:
 * 		Iterate over all the symbol table and look for the variables defined in the model, generate the  
 * code and store the result in the corresponding section. We consider the following situations:  
 * 		-# Parameters: Declare the global variables for each parameter. 
 * 		-# Initial expression: For states, discretes and parameters generate the initial code if the variables have initial expressions. We identify three different initial conditions:
 * 			+# Initial assignments of the form: \f$ Real \ x = value\f$.
 * 			+# Start modifier: \f$ Real \ x(start = value) \f$.
 * 			+# Each modifier: \f$ Real \ x[N](each start = value) \f$.
 *
 * 	\note{All the parameters and the algebraic variables array are declared global in the C file}
 */

void
MMO_Generator_::_variables()
{
  stringstream buffer;
  VarSymbolTable vt = _model->varTable();
  vt->setPrintEnvironment(VST_INIT);
  if(_model->annotation()->classic())
  {
    vt->setPrintEnvironment(VST_CLASSIC_INIT);
  }
  string indent = _writer->indent(1);
  list<VarInfo>::iterator it;
  list<VarInfo> parameters = vt->parameters();
  for(it = parameters.begin(); it != parameters.end(); it++)
  {
    VarInfo vi = *it;
    Index idx = vi->index();
    stringstream reverse;
    if(vi->type()->getType() == TYREAL)
    {
      buffer << "double " << vt->print(vi);
    }
    else if(vi->type()->getType() == TYINTEGER)
    {
      buffer << "int " << vt->print(vi);
      reverse << "int __reverse" << vt->print(vi);
    }
    if(vi->isArray())
    {
      buffer << "[" << vi->size() << "]";
      reverse << "[" << vi->size() << "]";
    }
    else
    {
      buffer << " = 0";
      reverse << " = 0";
    }
    buffer << ";";
    if(vi->type()->getType() == TYINTEGER)
    {
      reverse << ";";
      _writer->write(&reverse, WR_GLOBAL_VARS);
    }
    _writer->write(&buffer, WR_GLOBAL_VARS);
    if(vi->hasAssignment() || vi->hasStartModifier() || vi->hasEachModifier())
    {
      buffer << _model->printInitialAssignment(vi, indent);
      _writer->write(&buffer, WR_START_CODE);
    }
  }
  _writer->newLine(WR_GLOBAL_VARS);
}

/*! \brief Write the prologue of each writer section of the model.
 *
 */

void
MMO_Generator_::_initSections()
{
  _writer->write("// Allocate main data structures.", WR_ALLOC_LD);
  _writer->write("// Initialize model data.", WR_INIT_LD);
  _writer->write("// Initialize model time.", WR_INIT_TIME);
  _writer->write("// Initialize model code.", WR_INIT_CODE);
}

/*! \brief Generate code to allocate and initialize model variables.
 *
 *  Description:
 *  	-# Generate code to allocate auxiliary arrays used to count the number of dependencies.
 *  	-# Generate code to allocate global algebraic variables if neccesary.
 *  	-# Call the solver to generate the incidence matrices according to the method selected.
 *
 */

void
MMO_Generator_::_initMatrices()
{
  _solver->initializeMatrices();
}

void
MMO_Generator_::_functions()
{
}

void
MMO_Generator_::_printList(list<string> l)
{
  for(list<string>::iterator it = l.begin(); it != l.end(); it++)
  {
    _writer->write(*it, WR_FUNCTION_CODE);
  }
}

void
MMO_Generator_::_generateFunctionCode(MMO_Function f)
{
  list<string> code;
  MMO_ImportTable it = f->imports();
  for(string i = it->begin(); !it->end(); i = it->next())
  {
    string addInclude = Util::getInstance()->packageName(i);
    if(_includes.find(addInclude) == _includes.end())
    {
      _includes[addInclude] = addInclude;
      _writer->write("#include \"" + addInclude + ".h\"", WR_FUNCTION_HEADER);
    }
  }
  if(f->annotation()->hasInclude())
  {
    string addInclude = f->annotation()->include();
    if(_includes.find(addInclude) == _includes.end())
    {
      _includes[addInclude] = addInclude;
      _writer->write(addInclude, WR_FUNCTION_HEADER);
    }
  }
  string indent = _writer->indent(1);
  stringstream buffer;
  buffer << f->prototype();
  _writer->write(&buffer, WR_FUNCTION_CODE);
  _writer->write("{", WR_FUNCTION_CODE);
  _writer->beginBlock();
  _printList(f->localDeclarations());
  MMO_StatementTable stt = f->statements();
  VarSymbolTable vt = f->varTable();
  vt->setPrintEnvironment(VST_FUNCTION);
  for(MMO_Statement st = stt->begin(); !stt->end(); st = stt->next())
  {
    list<string> sts = st->print(indent);
    _printList(st->getVariables());
    code.insert(code.end(), sts.begin(), sts.end());
  }
  _printList(code);
  MMO_ArgumentsTable at = f->externalFunctionCalls();
  for(MMO_FunctionData fd = at->begin(); !at->end(); fd = at->next())
  {
    _printList(fd->print(indent));
  }
  _writer->write(f->returnStatement(), WR_FUNCTION_CODE);
  _writer->endBlock();
  _writer->write("}", WR_FUNCTION_CODE);
}

void
MMO_Generator_::_generateFunctionHeader(string fileName)
{
  string indent = _writer->indent(1);
  string file = fileName;
  file.append(".h");
  _writer->setFile(file);
  for(list<string>::iterator it = _fheader.begin(); it != _fheader.end(); it++)
  {
    _writer->print(*it);
  }
  _writer->clearFile();
}

void
MMO_Generator_::_generateFunction(MMO_FunctionDefinition f, string fileName)
{
  _printList(f->def());
}

void
MMO_Generator_::_generateFunction(MMO_Function f, string fileName)
{
  _fheader.push_back(f->prototype() + ";");
  _generateFunctionCode(f);
}

void
MMO_Generator_::_generatePackage(MMO_Package p)
{
  string indent = _writer->indent(1);
  string name;
  if(_flags->hasOutputFile())
  {
    name = _flags->outputFilePath();
    name.append(SLASH);
  }
  name.append(p->fileName());
  string fileName;
  stringstream buffer;
  fileName = name;
  fileName.append(".moo");
  _writer->setFile(fileName);
  if(_flags->hasObjects())
  {
    list<string> objects = _flags->objects();
    _writer->print("DEPENDENCES");
    for(list<string>::iterator it = objects.begin(); it != objects.end(); it++)
    {
      _writer->print(*it);
    }
    _writer->print("ENDDEPENDENCES");
  }
  MMO_FunctionTable ft = p->functions();
  list<string> includes;
  for(MMO_Function f = ft->begin(); !ft->end(); f = ft->next())
  {
    _writer->print("DEFINITION");
    _writer->print("FUNCTION");
    _writer->print(p->prefix() + f->name());
    _writer->print("ENDFUNCTION");
    MMO_Annotation annot = f->annotation();
    if(annot->hasInclude())
    {
      includes.push_back(annot->include());
    }
    if(annot->hasDerivative())
    {
      _writer->print("DERIVATIVE");
      _writer->print(p->prefix() + f->name());
      _writer->print(annot->derivative());
      _writer->print("ENDDERIVATIVE");
    }
    if(annot->hasIncludeDirectory())
    {
      _writer->print("INCLUDEDIRECTORY");
      _writer->print(annot->includeDirectory());
      _writer->print("ENDINCLUDEDIRECTORY");
    }
    if(annot->hasLibraryDirectory())
    {
      _writer->print("LIBRARYDIRECTORY");
      _writer->print(annot->libraryDirectory());
      _writer->print("ENDLIBRARYDIRECTORY");
    }
    if(annot->hasLibraries())
    {
      _writer->print("LIBRARIES");
      list<string> libs = annot->libraries();
      for(list<string>::iterator it = libs.begin(); it != libs.end(); it++)
      {
        _writer->print(*it);
      }
      _writer->print("ENDLIBRARIES");
    }
    _writer->print("ENDDEFINITION");
  }
  _writer->clearFile();
  fileName = name;
  fileName.append(".h");
  _writer->setFile(fileName);
  for(MMO_Function f = ft->begin(); !ft->end(); f = ft->next())
  {
    f->setPrefix(p->prefix());
    _writer->print(f->prototype() + ";");
  }
  _writer->clearFile();
  fileName = name;
  fileName.append(".c");
  _writer->setFile(fileName);
  buffer << "#include <math.h>" << endl;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include \"" << name << ".h\"" << endl;
  MMO_ImportTable it = p->imports();
  for(string i = it->begin(); !it->end(); i = it->next())
  {
    string addInclude = Util::getInstance()->packageName(i);
    buffer << "#include \"" << addInclude << ".h\"" << endl;
    _includes[addInclude] = addInclude;
  }
  _writer->write(&buffer, WR_FUNCTION_HEADER);
  for(MMO_Function f = ft->begin(); !ft->end(); f = ft->next())
  {
    f->setPrefix(p->prefix());
    _generateFunctionCode(f);
  }
  _writer->print(WR_FUNCTION_HEADER);
  _writer->print(WR_FUNCTION_CODE);
  _writer->clearFile();
}

MMO_Generator
newMMO_Generator(MMO_StoredDefinition std, MMO_CompileFlags flags)
{
  return new MMO_Generator_(std, flags);
}

void
deleteMMO_Generator(MMO_Generator m)
{
  delete m;
}

void
MMO_Generator_::_variablesInitCode()
{
  stringstream buffer;
  VarSymbolTable vt = _model->varTable();
  vt->setPrintEnvironment(VST_INIT);
  if(_model->annotation()->classic())
  {
    vt->setPrintEnvironment(VST_CLASSIC_INIT);
  }
  string indent = _writer->indent(1);
  for(VarInfo vi = vt->begin(); !vt->end(); vi = vt->next())
  {
    Index idx = vi->index();
    if(!vi->isConstant() && !vi->isParameter()
        && (vi->hasAssignment() || vi->hasStartModifier()
            || vi->hasEachModifier()))
    {
      buffer << _model->printInitialAssignment(vi, indent);
      _writer->write(&buffer, WR_START_CODE);
    }
  }
}
