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

#include "helpers.h"
#include "built_in_functions.h"
#include "expression.h"
#include "equation.h"
#include "../generator/macros.h"
#include "../util/error.h"
#include "../util/util.h"

namespace MicroModelica {
using namespace Deps;
using namespace Util;
using namespace Generator;
namespace IR {

/* ExternalFunction Class Implementation */

ExternalFunction::ExternalFunction(string lvalue, string name, AST_ExpressionList args, const VarSymbolTable& symbols)
    : _lvalue(lvalue), _name(name), _args(args), _symbols(symbols)
{
}

std::ostream& operator<<(std::ostream& out, const ExternalFunction& e)
{
  list<string> ret;
  stringstream buffer;
  if (!e._lvalue.empty()) {
    buffer << e._lvalue << " = ";
  }
  buffer << e._name << "(";
  if (e._args != nullptr) {
    AST_ExpressionListIterator it;
    unsigned int count = 0;
    foreach (it, e._args) {
      Expression ex(current_element(it), e._symbols);
      buffer << ex;
      if (++count < e._args->size()) {
        buffer << ",";
      }
    }
  }
  buffer << ");";
  out << buffer.str();
  return out;
}

/* CompiledFunction Class Implementation */

CompiledFunction::CompiledFunction() : _name(), _prototype(), _includeDirectory(), _libraryDirectory(), _libraries() {}

CompiledFunction::CompiledFunction(string name, string includeDir, string libraryDir, SymbolTable& libraries, string prefix)
    : _name(prefix + name), _prototype(), _includeDirectory(includeDir), _libraryDirectory(libraryDir), _libraries(libraries)
{
}

std::ostream& operator<<(std::ostream& out, const CompiledFunction& cf)
{
  out << cf.print();
  return out;
}

string CompiledFunction::print() const
{
  stringstream buffer;
  Option<BuiltInFunctionPrinter> fp = BuiltInFunction::instance().reductionFunctions(_name);
  if (fp) {
    buffer << fp.get();
  } else {
    buffer << _name << "(";
    AST_ExpressionListIterator it;
    int size = _arguments->size(), i = 0;
    foreach (it, _arguments) {
      i++;
      Expression ex(current_element(it), Utils::instance().symbols());
      buffer << ex;
      buffer << (i < size ? "," : "");
    }
    buffer << ")";
  }
  return buffer.str();
}

string CompiledFunction::code()
{
  Option<BuiltInFunctionPrinter> fp = BuiltInFunction::instance().reductionFunctions(_name);
  if (fp) {
    return fp->code();
  }
  return "";
}

/* CompiledPackage Class Implementation */

CompiledPackage::CompiledPackage() {}

CompiledPackage::CompiledPackage(string name) : _name(name), _cft(), _objects() {}

CompiledPackage::CompiledPackage(string name, CompiledFunctionTable cft, ImportTable objects) : _name(name), _cft(cft), _objects(objects) {}

string CompiledPackage::name() { return _name; }

string CompiledPackage::prefix() { return "__" + _name + "__"; }

CompiledFunctionTable CompiledPackage::definitions() { return _cft; }

ImportTable CompiledPackage::objects() { return _objects; }

SymbolTable CompiledPackage::linkLibraries()
{
  SymbolTable ret;
  CompiledFunctionTable::iterator it;
  for (CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it)) {
    if (cf.hasLibraries()) {
      ret.merge(cf.libraries());
    }
  }
  return ret;
}

SymbolTable CompiledPackage::libraryDirectories()
{
  SymbolTable ret;
  CompiledFunctionTable::iterator it;
  for (CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it)) {
    if (cf.hasLibraryDirectory()) {
      string ld = cf.libraryDirectory();
      ret.insert(ld, ld);
    }
  }
  return ret;
}

SymbolTable CompiledPackage::includeDirectories()
{
  SymbolTable ret;
  CompiledFunctionTable::iterator it;
  for (CompiledFunction cf = _cft.begin(it); !_cft.end(it); cf = _cft.next(it)) {
    if (cf.hasIncludeDirectory()) {
      string id = cf.includeDirectory();
      ret.insert(id, id);
    }
  }
  return ret;
}

/* Function Printer implementation */

string FunctionPrinter::loop(int end)
{
  stringstream buffer;
  buffer << "for (idx = 1; idx <=" << end << "; idx++) {";
  return buffer.str();
}

string FunctionPrinter::endLoop()
{
  stringstream buffer;
  buffer << "}";
  return buffer.str();
}

string FunctionPrinter::beginSwitch()
{
  stringstream buffer;
  buffer << "switch(idx) {";
  return buffer.str();
}

string FunctionPrinter::endSwitch() { return "}"; }

string FunctionPrinter::beginExpression(string token, Option<Range> range) const
{
  stringstream buffer;
  if (range) {
    buffer << "if (_is_var" << token << "(idx)) {" << endl;
  } else {
    buffer << TAB << "case " << token << ": {" << endl;
  }
  if (range) {
    buffer << TAB << "_get" << token << "_idxs(idx);" << endl;
    range->addLocalVariables();
  }
  return buffer.str();
}

string FunctionPrinter::endExpression(Option<Range> range, FUNCTION_PRINTER::ReturnStatementType ret) const
{
  stringstream buffer;
  switch (ret) {
  case FUNCTION_PRINTER::Return:
    buffer << "return;" << endl;
    break;
  case FUNCTION_PRINTER::Break: {
    if (!range) {
      buffer << "break;" << endl;
    }
    break;
  }
  }
  if (!range) {
    buffer << TAB;
  }
  buffer << "}";
  return buffer.str();
}

string FunctionPrinter::endDimGuards(Option<Range> range) const
{
  if (range) {
    stringstream buffer;
    buffer << "}" << endl;
    return buffer.str();
  }
  return "";
}

string FunctionPrinter::beginDimGuards(string token, string args, Option<Range> range) const
{
  stringstream buffer;
  if (range) {
    buffer << TAB << "_apply_usage" << token << "(" << args << ");" << endl;
    RangeDefinitionTable rdt = range->definition();
    RangeDefinitionTable::iterator it;
    int size = rdt.size(), i = 0, idx = 0;
    buffer << TAB << "if (";
    for (RangeDefinition rd = rdt.begin(it); !rdt.end(it); rd = rdt.next(it), idx++) {
      buffer << "(" << rdt.key(it) << " >= " << rd.begin() << " && " << rdt.key(it) << " <= " << rd.end() << ")";
      buffer << (++i < size ? " && " : "");
    }
    buffer << ") {" << endl;
    range->addLocalVariables();
  }
  return buffer.str();
}

string FunctionPrinter::algebraic(int id, bool reduction)
{
  stringstream buffer;
  EquationTable algebraic = ModelConfig::instance().algebraics();
  Option<Equation> alg = algebraic[id];
  if (alg) {
    Equation a = alg.get();
    Option<Range> range = a.range();
    bool reduction_range = reduction && range;
    if (reduction_range) {
      buffer << range.get();
      cout << buffer.str() << endl;
    }
    buffer << a << endl;
    if (reduction_range) {
      buffer << range.get().end() << endl;
      cout << buffer.str() << endl;
    }
  } else {
    Error::instance().add(0, EM_CG | EM_NO_EQ, ER_Error, "Algebraic equation not found.");
  }
  return buffer.str();
}

string FunctionPrinter::jacobianTerms(list<Equation> eqs)
{
  stringstream buffer;
  for (Equation eq : eqs) {
    buffer << eq;
  }
  return buffer.str();
}

string FunctionPrinter::algebraics(AlgebraicDependencies deps)
{
  stringstream buffer;
  for (VariableDependency d : deps) {
    buffer << algebraic(d.equationId(), d.isReduction());
  }
  return buffer.str();
}

string FunctionPrinter::algebraics(EquationDependencyMatrix eqdm, depId key)
{
  stringstream buffer;
  Option<VariableDependencies> eqd = eqdm[key];
  if (eqd) {
    VariableDependencies::iterator eq_it;
    for (eq_it = eqd->begin(); eq_it != eqd->end(); eq_it++) {
      buffer << algebraic(eq_it->ifce.equationId(), eq_it->ifce.isReduction());
      buffer << algebraics(eq_it->algs);
    }
  }
  return buffer.str();
}

string FunctionPrinter::getIndexes(string var, Option<Range> range, int offset, bool modelica_index) const
{
  stringstream buffer;
  if (range) {
    RangeDefinitionTable rdt = range->definition();
    RangeDefinitionTable::iterator it;
    int size = rdt.size(), i = 0, idx = 0;
    stringstream offset_var;
    offset_var << "(" << var << "-" << offset << ")";
    string alligned_var = offset_var.str();
    for (RangeDefinition rd = rdt.begin(it); !rdt.end(it); rd = rdt.next(it), idx++) {
      buffer << TAB << rdt.key(it) << " = "
             << (i + 1 < size ? div(mod(alligned_var, idx - 1, range), idx, range) : mod(alligned_var, idx - 1, range))
             << (modelica_index ? "+ 1;" : ";") << (i + 1 < size ? " \\" : "") << endl;
      i++;
    }
  }
  return buffer.str();
}

string FunctionPrinter::accessMacros(string token, int offset, Option<Range> range, bool modelica_index) const
{
  stringstream macros;
  if (range && !range->isEmpty()) {
    macros << "#define _is_var" << token << "(idx) ";
    macros << "idx >= " << offset << " && ";
    macros << "idx < " << offset + range->size() << endl;
    macros << "#define _get" << token << "_idxs(idx) \\" << endl;
    macros << TAB << getIndexes("idx", range, offset, modelica_index);
  }
  return macros.str();
}

string FunctionPrinter::mod(string idx, int dim, Option<Range> range) const
{
  if (dim < 0) {
    return idx;
  }
  stringstream buffer;
  buffer << "(" << mod(idx, dim - 1, range) << "%" << range->rowSize(dim) << ")";
  return buffer.str();
}

string FunctionPrinter::div(string idx, int dim, Option<Range> range) const
{
  stringstream buffer;
  buffer << "(" << idx << "/" << range->rowSize(dim) << ")";
  return buffer.str();
}

string FunctionPrinter::outputVariableName(Expression exp, Option<Range> range)
{
  assert(exp.isReference());
  stringstream buffer;
  Option<Variable> var = exp.reference();
  assert(var);
  buffer << "\"" << var->name();
  if (var->isArray()) {
    buffer << "[";
  }
  if (range) {
    buffer << "%d]\"," << exp.usage();
  } else if (var->isArray()) {
    buffer << exp.usage() << "]\"";
  } else {
    buffer << exp.usage() << "\"";
  }
  return buffer.str();
}

Input::Input(Index idx, Option<Range> range, int id) : _idx(idx), _range(range), _id(id) {}

string Input::print() const
{
  stringstream buffer;
  Macros m;
  string block = "";
  if (_range) {
    buffer << _range.get();
    block += TAB;
  }
  buffer << block << "modelData->IT[" << m.usage(token(), _range, _id) << "] = " << _idx << ";" << endl;
  if (_range) {
    buffer << _range->end();
  }
  return buffer.str();
}

string Input::token() const
{
  stringstream buffer;
  buffer << "_input_" << _id;
  return buffer.str();
}

string Input::macro() const
{
  Macros m;
  return m.indexMacro(token(), _range, _id);
}

ostream& operator<<(std::ostream& out, const Input& i)
{
  out << i.print();
  return out;
}

ModelConfig::ModelConfig() : _model_annotations(), _algebraics(), _dependencies(), _initial_code(false) {}

bool ModelConfig::generateDerivatives() { return _model_annotations.symDiff() && isQss(); }

}  // namespace IR
}  // namespace MicroModelica
