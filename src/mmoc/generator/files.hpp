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
#include <list>
#include <string>

#include <generator/model_instance.hpp>
#include <ir/annotation.hpp>
#include <ir/class.hpp>
#include <util/compile_flags.hpp>

namespace MicroModelica {
namespace Generator {

class Files {
  public:
  Files(ModelInstancePtr modelInstance, IR::Model& model, Util::CompileFlags& flags);
  Files(string name, Util::CompileFlags& flags);
  ~Files() = default;
  void makefile();
  void run();
  void settings(IR::ModelAnnotation annotation);
  void graph();
  void bdfPartition();

  protected:
  void printList(const list<string>& ann, const string& tag) const;
  void addAnnotation(const IR::ModelAnnotation& annotation, const string& mmo_name, IR::IntegerAnnotations name);

  string _fname;
  IR::Model _model;
  ModelInstancePtr _modelInstance;
  WriterPtr _writer;
  Util::CompileFlags& _flags;
  ofstream _file;
};
}  // namespace Generator
}  // namespace MicroModelica
