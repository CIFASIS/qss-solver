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

#include <ir/annotation.hpp>
#include <ir/class.hpp>
#include <ir/statement.hpp>
#include <ir/stored_definition.hpp>
#include <util/error.hpp>
#include <util/symbol_table.hpp>
#include <util/type.hpp>
#include <util/util.hpp>
#include <generator/function.hpp>
#include <generator/generator.hpp>
#include <generator/package.hpp>

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Generator {

Generator::Generator(const StoredDefinition& std, const CompileFlags& flags)
    : _std(std), _flags(flags), _model_instance(nullptr), _writer(nullptr), _includes(), _fheader()
{
  if (_flags.output()) {
    _writer = WriterPtr(std::make_shared<MemoryWriter>());
  } else {
    _writer = WriterPtr(std::make_shared<FileWriter>());
  }
}

int Generator::generate()
{
  if (_std.isModel()) {
    Model model = _std.model();
    string base_name = model.name();
    if (_flags.hasOutputFile()) {
      base_name = _flags.outputFile();
    }
    _writer->setFile(base_name + ".c");
    switch (model.annotations().solver()) {
    case Solver::DOPRI:
    case Solver::DASSL:
    case Solver::CVODE_BDF:
    case Solver::IDA:
    case Solver::CVODE_AM:
      _model_instance = ModelInstancePtr(std::make_shared<ClassicModelInstance>(model, _flags, _writer));
      break;
    default:
      _model_instance = ModelInstancePtr(std::make_shared<QSSModelInstance>(model, _flags, _writer));
    }
    _model_instance->generate();
    _writer->clearFile();
    _writer->setFile(base_name + ".h");
    _model_instance->header();
    _writer->print(WRITER::Model_Header);
    _writer->clearFile();
    Files files(_model_instance, model, _flags);
    files.makefile();
    files.run();
    files.plot();
    files.settings(model.annotations());
    if (model.annotations().solver() == Solver::LIQSS_BDF && !model.annotations().BDFPartition()->empty()) {
      files.bdfPartition();
    }
    if (model.externalFunctions()) {
      string ffname = base_name + "_functions";
      generateIncludes(ffname);
      FunctionTable ft = model.calledFunctions();
      FunctionTable::iterator it;
      Util::SymbolTable includes;
      for (IR::Function f = ft.begin(it); !ft.end(it); f = ft.next(it)) {
        Function func(f, _flags, _writer);
        func.addInclude(includes);
        func.definition();
        includes.merge(func.functionIncludes());
        _fheader.push_back(func.header());
      }
      calledFunctionHeader(ffname);
      _writer->setFile(ffname + ".c");
      _writer->print(WRITER::Function_Header);
      _writer->print(WRITER::Function_Code);
      _writer->clearFile();
    }
  } else {
    Package pkg(_std.package(), _flags, _writer);
    pkg.generate();
  }
  return Error::instance().errors();
}

void Generator::generateIncludes(const string& name)
{
  stringstream buffer;
  buffer << "#include <math.h>" << endl;
  buffer << "#include <stdlib.h>" << endl;
  buffer << "#include \"" << name << ".h\"" << endl;
  _writer->write(buffer, WRITER::Function_Header);
}

void Generator::calledFunctionHeader(const string& file_name)
{
  string indent = _writer->indent(1);
  string file = file_name;
  file.append(".h");
  _writer->setFile(file);
  for (auto it = _fheader.begin(); it != _fheader.end(); it++) {
    _writer->print(*it);
  }
  _writer->clearFile();
}

}  // namespace Generator
}  // namespace MicroModelica
