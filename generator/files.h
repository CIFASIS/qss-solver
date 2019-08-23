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

#ifndef MMO_FILES_H_
#define MMO_FILES_H_

#include <fstream>
#include <string>
#include <list>

#include "model_instance.h"
#include "writer.h"
#include "../ir/annotation.h"
#include "../ir/class.h"
#include "../util/compile_flags.h"

namespace MicroModelica {
namespace Generator {

/**
 *
 */
class Files {
  public:
  Files();
  Files(ModelInstancePtr modelInstance, IR::Model& model, Util::CompileFlags& flags);
  Files(string name, Util::CompileFlags& flags);
  ~Files();
  void makefile();
  void run();
  void plot();
  void settings(IR::ModelAnnotation annotation);
  void graph();

  private:
  std::string variablePlotSettings();
  void printList(list<string> ann, string tag);
  string _fname;
  IR::Model _model;
  ModelInstancePtr _modelInstance;
  WriterPtr _writer;
  Util::CompileFlags _flags;
  ofstream _file;
};
}  // namespace Generator
}  // namespace MicroModelica

#endif /* MMO_FILES_H_ */
