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
#include "writer.h"
#include "solver.h"

MMO_Generator::MMO_Generator(MMO_StoredDefinition std, MMO_CompileFlags &flags) :
  _std(std), 
  _model(NULL), 
  _function(NULL), 
  _package(NULL), 
  _flags(flags), 
  _solver(NULL), 
  _files(NULL), 
  _writer(NULL), 
  _includes(), 
  _fheader()
{
  if(_flags.output())
  {
    _writer = MMO_MemoryWriter();
  }
  else
  {
    _writer = MMO_FileWriter();
  }
}

MMO_Generator::~MMO_Generator()
{
  delete _writer;
  delete _files;
}

int
MMO_Generator::generate()
{
  for(MMO_Class &c = _std->begin(); _std->end(); c = _std->next())
  {
    string fname = c.name();
    switch(c.classType())
    {
      case CL_MODEL:
        {
        if(_flags->hasOutputFile())
        {
          fname = _flags->outputFile();
        }
        fname.append(".c");
        _writer->setFile(fname);
        _model = c.getAsModel();
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
          string ffname = c.name();
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
MMO_Generator::_generateHeader(string name)
{
  stringstream buffer;
  buffer << "#include <math.h>" << endl;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include \"" << name << ".h\"" << endl;
  _writer->write(&buffer, WR_FUNCTION_HEADER);
}

void
MMO_Generator::_generateModel()
{
  _solver->modelHeader();
  _solver->modelDefinition();
  _solver->modelDependencies();
  _solver->zeroCrossing();
  _solver->handler();
  _solver->output();
  _solver->initializeDataStructures();
}

void 
MMO_Generator::_printList(list<string> l)
{
  for(list<string>::iterator it = l.begin(); it != l.end(); it++)
  {
    _writer->write(*it, WR_FUNCTION_CODE);
  }
}

void
MMO_Generator::_generateFunctionCode(MMO_Function f)
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
MMO_Generator::_generateFunctionHeader(string fileName)
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
MMO_Generator::_generateFunction(MMO_FunctionDefinition f, string fileName)
{
  _printList(f->def());
}

void
MMO_Generator::_generateFunction(MMO_Function f, string fileName)
{
  _fheader.push_back(f->prototype() + ";");
  _generateFunctionCode(f);
}

void
MMO_Generator::_generatePackage(MMO_Package p)
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
  return new MMO_Generator(std, flags);
}

void
deleteMMO_Generator(MMO_Generator m)
{
  delete m;
}

void
MMO_Generator::_variablesInitCode()
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
