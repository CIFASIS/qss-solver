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
        string baseName = model.name();
        if(_flags.hasOutputFile())
        {
          baseName = _flags.outputFileName();
        }
        _writer->setFile(baseName+".c");
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
        generateModel();
        _writer->clearFile();
        _writer->setFile(baseName+".h");
        _modelInstance->header();
        _writer->print(MODEL_HEADER);
        _writer->clearFile();
        Files files(_modelInstance, model, _flags);
        files.makefile();
        files.run();
        files.plot();
        files.settings(model.annotations());
        if(_flags.graph()) { files.graph(); }
        if(!model.externalFunctions())
        {
          string ffname = baseName+"_functions";
          generateIncludes(ffname);
          FunctionTable ft = _model.calledFunctions();
          FunctionTable::iterator it;
          for(IR::Function f = ft.begin(it); !ft.end(it); f = ft.next(it))
          {
            Function func(f,_flags, _writer);
            func.definition();
            _fheader.push_back(func.header());
          }
          if(_flags.hasOutputFile())
          {
            ffname.insert(0, _flags.outputFilePath() + SLASH);
          }
          calledFunctionHeader(ffname);
          _writer->setFile(ffname+".c");
          _writer->print(FUNCTION_HEADER);
          _writer->print(FUNCTION_CODE);
          _writer->clearFile();
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
    Generator::generateIncludes(string name)
    {
      stringstream buffer;
      buffer << "#include <math.h>" << endl;
      buffer << "#include <stdlib.h>" << endl;
      buffer << "#include \"" << name << ".h\"" << endl;
      _writer->write(buffer, FUNCTION_HEADER);
    }

    void
    Generator::generateModel()
    {
    }

    void
    Generator::calledFunctionHeader(string fileName)
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

  }
}
