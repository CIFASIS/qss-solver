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

#include "package.h"
#include "function.h"
#include "../util/util.h"

namespace MicroModelica {
  using namespace IR;
  using namespace Util;
  namespace Generator {

    Package::Package(IR::Package& package, CompileFlags& flags, WriterPtr writer) :
      _package(package),
      _flags(flags),
      _writer(writer)
    {
    }

    void
    Package::generate()
    {
      string indent = _writer->indent(1);
      string name;
      if(_flags.hasOutputFile())
      {
        name = _flags.outputFilePath();
      }
      name.append(_package.fileName());
      string fileName;
      stringstream buffer;
      fileName = name;
      fileName.append(".moo");
      _writer->setFile(fileName);
      if(_flags.hasObjects())
      {
        list<string> objects = _flags.objects();
        _writer->print("DEPENDENCES");
        for(list<string>::iterator it = objects.begin(); it != objects.end(); it++)
        {
          _writer->print(*it);
        }
        _writer->print("ENDDEPENDENCES");
      }
      FunctionTable ft = _package.definitions();
      SymbolTable includes;
      FunctionTable::iterator it;
      for(IR::Function f = ft.begin(it); !ft.end(it); f = ft.next(it))
      {
        _writer->print("DEFINITION");
        _writer->print("FUNCTION");
        _writer->print(f.name());
        _writer->print("ENDFUNCTION");
        FunctionAnnotation annot = f.annotations();
        if(annot.hasInclude())
        {
          string inc = annot.include();
          includes.insert(inc, inc);
        }
        if(annot.hasDerivative())
        {
          _writer->print("DERIVATIVE");
          _writer->print(_package.prefix() + f.name());
          _writer->print(annot.derivative());
          _writer->print("ENDDERIVATIVE");
        }
        if(annot.hasIncludeDirectory())
        {
          _writer->print("INCLUDEDIRECTORY");
          _writer->print(annot.includeDirectory());
          _writer->print("ENDINCLUDEDIRECTORY");
        }
        if(annot.hasLibraryDirectory())
        {
          _writer->print("LIBRARYDIRECTORY");
          _writer->print(annot.libraryDirectory());
          _writer->print("ENDLIBRARYDIRECTORY");
        }
        if(annot.hasLibraries())
        {
          _writer->print("LIBRARIES");
          SymbolTable libs = annot.libraries();
          SymbolTable::iterator it;
          for(string i = libs.begin(it); !libs.end(it); i = libs.next(it))
          {
            _writer->print(i);
          }
          _writer->print("ENDLIBRARIES");
        }
        _writer->print("ENDDEFINITION");
      }
      _writer->clearFile();
      fileName = name;
      fileName.append(".h");
      _writer->setFile(fileName);
      for(IR::Function f = ft.begin(it); !ft.end(it); f = ft.next(it))
      {
        Function func(f,_flags, _writer);
        func.setPrefix(_package.prefix());
        _writer->print(func.header());
      }
      _writer->clearFile();
      fileName = name;
      fileName.append(".c");
      _writer->setFile(fileName);
      buffer << "#include <math.h>" << endl;
      buffer << "#include <stdlib.h>" << endl;
      buffer << "#include \"" << name << ".h\"" << endl;
      SymbolTable::iterator incIt;
      for(string i = includes.begin(incIt); !includes.end(incIt); i = includes.next(incIt))
      {
        buffer <<  i;
      }
      ImportTable imports = _package.imports();
      ImportTable::iterator impIt;
      for(string i = imports.begin(impIt); !imports.end(impIt); i = imports.next(impIt))
      {
        string addInclude = Utils::instance().packageName(i);
        buffer << "#include \"" << addInclude << ".h\"" << endl;
        includes.insert(addInclude, addInclude);
      }
      _writer->write(buffer, WRITER::Function_Header);
      for(IR::Function f = ft.begin(it); !ft.end(it); f = ft.next(it))
      {
        Function func(f,_flags, _writer);
        func.setPrefix(_package.prefix());
        func.addInclude(includes);
        func.definition();
      }
      _writer->print(WRITER::Function_Header);
      _writer->print(WRITER::Function_Code);
      _writer->clearFile();
    }
  }
}


