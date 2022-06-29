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

#pragma once 

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

class Generator {
  public:
  Generator(const IR::StoredDefinition& std, Util::CompileFlags& flags);
  ~Generator() = default;
  int generate();

  private:
  void generateIncludes(string name);
  void generateModel();
  void calledFunctionHeader(string fileName);
  IR::StoredDefinition _std;
  IR::Function _function;
  IR::Package _package;
  Util::CompileFlags _flags;
  ModelInstancePtr _modelInstance;
  WriterPtr _writer;
  ofstream _file;
  std::map<string, string> _includes;
  list<string> _fheader;
};
}  // namespace Generator
}  // namespace MicroModelica

