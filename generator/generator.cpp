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
#include "../ir/statement.h"
#include "../ir/stored_definition.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "function.h"
#include "package.h"
#include "model_instance.h"
#include "files.h"
#include "writer.h"


namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Generator {
    
    Generator::Generator(StoredDefinition& std, CompileFlags& flags) :
      _std(std), 
      _flags(flags), 
      _modelInstance(NULL), 
      _files(), 
      _writer(NULL), 
      _includes(), 
      _fheader()
    {
      if(_flags.output())
      {
        _writer = WriterPtr(new MemoryWriter());
      }
      else
      {
        _writer = WriterPtr(new FileWriter());
      }
    }

    Generator::~Generator()
    {
    }

    int
    Generator::generate()
    {
      if(_std.isModel())
      {
        Model model = _std.model();
        string fname = model.name();
        if(_flags.hasOutputFile())
        {
          fname = _flags.outputFile();
        }
        fname.append(".c");
        _writer->setFile(fname);
        switch(model.annotations().solver())
        {
          case DOPRI:
          case DASSL:
          case CVODE_BDF:
          case IDA:
          case CVODE_AM:
            _modelInstance = ModelInstancePtr(new ClassicModelInstance(model, _flags, _writer));
            break;
          default:
            _modelInstance = ModelInstancePtr(new QSSModelInstance(model, _flags, _writer));
        }
        _files = Files(_modelInstance, model, _flags);
        generateModel();
        _writer->clearFile();
        _files.makefile();
        _files.run();
        _files.plot();
        _files.settings(model.annotations());
        if(_flags.graph())
        {
          _files.graph();
        }
        if(!model.externalFunctions().empty())
        {
          string ffname = model.name();
          if(_flags.hasOutputFile())
          {
            ffname = _flags.outputFileName();
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
            ffname.insert(0, _flags.outputFilePath() + SLASH);
          }
          _generateFunctionHeader(ffname);
          ffname.append(".c");
          _writer->setFile(ffname);
          _writer->print(FUNCTION_HEADER);
          _writer->print(FUNCTION_CODE);
          _writer->clearFile();
        }
      }

      }
      else 
      {
        Package pkg(_std.package(), _flags, _writer);
        pkg.generate();
      }
      return Error::instance().errors();
    }

    void
    Generator::_generateHeader(string name)
    {
      stringstream buffer;
      buffer << "#include <math.h>" << endl;
      buffer << "#include <stdlib.h>" << endl;
      buffer << "#include \"" << name << ".h\"" << endl;
      _writer->write(&buffer, FUNCTION_HEADER);
    }

    void
    Generator::generateModel()
    {
      _modelInstance->header();
      _modelInstance->definition();
      _modelInstance->dependencies();
      _modelInstance->zeroCrossing();
      _modelInstance->handler();
      _modelInstance->output();
      _modelInstance->initializeDataStructures();
    }

    void 
    Generator::_printList(list<string> l)
    {
      for(list<string>::iterator it = l.begin(); it != l.end(); it++)
      {
        _writer->write(*it, FUNCTION_CODE);
      }
    }

    void
    Generator::_generateFunctionHeader(string fileName)
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
    Generator::_generateFunction(IR::CompiledFunction f, string fileName)
    {
      _printList(f.def());
    }

    void
    Generator::_generateFunction(IR::Function f, string fileName)
    {
      /*_fheader.push_back(f.prototype() + ";");
      _generateFunctionCode(f);*/
    }


    void
    Generator::_variablesInitCode()
    {
      /*stringstream buffer;
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
      }*/
    }
  }
}
