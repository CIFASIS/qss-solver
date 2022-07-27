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

#include <generator/function.h>
#include <util/model_config.h>

namespace MicroModelica {
using namespace IR;
using namespace Util;
namespace Generator {
  
Function::Function(IR::Function& function, CompileFlags& flags, WriterPtr writer)
    : _function(function), _flags(flags), _writer(writer), _prefix(), _include(), _return_variable(), _symbols(), _void_function(false)
{
  _symbols = ModelConfig::instance().symbols(); 
  ModelConfig::instance().setSymbols(_function.symbols());
  ModelConfig::instance().setFunctionOutputs(_function.outputNbr() > 1);
  ModelConfig::instance().setFunctionCode(true);
}

Function::~Function()
{
  ModelConfig::instance().setSymbols(_symbols);
  ModelConfig::instance().setFunctionOutputs(false);
  ModelConfig::instance().setFunctionCode(false);

}

void Function::definition()
{
  includes();
  body();
}

void Function::includes()
{
  list<string> code;
  ImportTable imports = _function.imports();
  ImportTable::iterator it;
  for (string i = imports.begin(it); !imports.end(it); i = imports.next(it)) {
    string addInclude = Utils::instance().packageName(i);
    if (!_include.lookup(addInclude)) {
      _include.insert(addInclude, addInclude);
      _writer->write("#include \"" + addInclude + ".h\"", WRITER::Function_Header);
    }
  }
  if (_function.annotations().hasInclude()) {
    string addInclude = _function.annotations().include();
    if (!_include.lookup(addInclude)) {
      _include.insert(addInclude, addInclude);
      _writer->write(addInclude, WRITER::Function_Header);
    }
  }
}

string Function::prototype()
{
  stringstream input;
  stringstream output;
  stringstream func;
  VariableList arguments = _function.arguments();
  int outputs = _function.outputNbr();
  string name = _function.name();
  bool array_single_output = false;
  for (Variable var : arguments) {
    if (var.isInput()) {
      if (var.isString()) {
        input << "const char *" << var.name() << ",";
      } else {
        input << "double " << (var.isArray() ? "*" : "") << var.name() << ",";
      }
    } else if (var.isOutput()) {
      output << "double *" << var.name() << ",";
      _return_variable = var.name();
      array_single_output = var.isArray();
    }
  }
  string in = input.str();
  if (outputs <= 1 && !in.empty()) {
    in.erase(in.end() - 1, in.end());
  }
  if (outputs == 0 || (outputs == 1 && array_single_output) || outputs > 1) {
    _void_function = true;
  }
  if (outputs == 0) {
    func << "void " << _prefix << name << "(" << in << ")";
  } else if (outputs == 1 && !array_single_output) {
    func << "double " << _prefix << name << "(" << in << ")";
  } else {
    string out = output.str();
    out.erase(out.end() - 1, out.end());
    func << "void " << _prefix << name << "(" << input.str() << out << ")";
  }
  return func.str();
}

string Function::header() { return prototype() + ";"; }

void Function::body()
{
  stringstream buffer;
  _writer->write(prototype(), WRITER::Function_Code);
  _writer->beginBlock(WRITER::Function_Code);
  localSymbols();
  StatementTable stms = _function.statements();
  StatementTable::iterator it;
  for (Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it)) {
    buffer << stm << endl;
  }
  _writer->write(buffer, WRITER::Function_Code);
  ExternalFunctionTable eft = _function.externalFunctions();
  ExternalFunctionTable::iterator eit;
  for (ExternalFunction ef = eft.begin(eit); !eft.end(eit); ef = eft.next(eit)) {
    buffer << ef;
  }
  _writer->write(buffer.str(), WRITER::Function_Code);
  if (!_void_function) {
    _writer->write("return " + _return_variable + ";", WRITER::Function_Code);
  }
  _writer->endBlock(WRITER::Function_Code);
  _writer->write("\n", WRITER::Function_Code);
}

void Function::localSymbols()
{
  list<string> ret;
  stringstream buffer;
  VarSymbolTable symbols = _function.symbols();
  VarSymbolTable::iterator it;
  bool local_symbols = false;
  for (Variable var = symbols.begin(it); !symbols.end(it); var = symbols.next(it)) {
    if (var.isConstant()) {
      continue;
    }
    if (var.isOutput() && _void_function) {
      continue;
    }
    if (!var.isInput()) {
      local_symbols = true;
      _writer->write(var.declaration(), WRITER::Function_Code);
      _writer->write(var.initialization(), WRITER::Function_Code);
    }
  }
  if (local_symbols) {
    _writer->write("\n", WRITER::Function_Code);
  }
}

void Function::setPrefix(string prefix) { _prefix = prefix; }

void Function::addInclude(string include) { _include[include] = include; }

void Function::addInclude(SymbolTable includes) { _include.merge(includes); }

}  // namespace Generator
}  // namespace MicroModelica
