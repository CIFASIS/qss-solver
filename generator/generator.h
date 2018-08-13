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

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

#include "../util/compile_flags.h"
#include "../ir/stored_definition.h"
#include "../ir/class.h"
#include "model_instance.h"
#include "files.h"
#include "writer.h"

namespace MicroModelica {
  namespace Generator {

    /**
     *
     */
    class Generator
    {
      public:
        /**
         *
         * @param std
         * @param flags
         */
        Generator(IR::StoredDefinition& std, Util::CompileFlags& flags);
        /**
         *
         */
        ~Generator();
        /**
         *
         * @return
         */
        int
        generate();
      private:
        void
        _generateFunctionCode(IR::Function f);
        void
        _generateHeader(string name);
        void
        _printList(list<string> l);
        void
        _variablesInitCode();
        void
        _variables();
        void
        generateModel();
        void
        _generateFunction(IR::Function f, string fileName);
        void
        _generateFunction(IR::CompiledFunction f, string fileName);
        void
        _generateFunctionHeader(string fileName);
        void
        _generatePackage(IR::Package p);
        void
        _header();
        IR::StoredDefinition      _std;
        IR::Model                 _model;
        IR::Function              _function;
        IR::Package               _package;
        Util::CompileFlags        _flags;
        ModelInstancePtr          _modelInstance;
        Files                     _files;
        WriterPtr                 _writer;
        ofstream                  _file;
        std::map<string, string>  _includes;
        list<string>              _fheader;
    };
  }
}
#endif  /* GENERATOR_H_ */
