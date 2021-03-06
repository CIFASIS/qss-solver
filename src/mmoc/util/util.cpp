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

#include "util.h"

#include <stddef.h>
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <utility>

#include "../ir/built_in_functions.h"
#include "../ir/class.h"
#include "../ir/expression.h"
#include "../ir/equation.h"
#include "../ir/helpers.h"
#include "../ir/index.h"
#include "compile_flags.h"
#include "error.h"
#include "model_config.h"
#include "symbol_table.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

Utils::Utils()
    : _language_especification("C"),
      _flags(),
      _compiled_functions(),
      _file_name(),
      _ids(0)
{
  _annotations.insert(pair<string, int>("StartTime", 0));
  _annotations.insert(pair<string, int>("StopTime", 1));
  _annotations.insert(pair<string, int>("Tolerance", 2));
  _annotations.insert(pair<string, int>("AbsTolerance", 3));
  _annotations.insert(pair<string, int>("StepSize", 4));
  _binop[BINOPOR] = "||";
  _binop[BINOPAND] = "&&";
  _binop[BINOPLOWER] = "<";
  _binop[BINOPLOWEREQ] = "<=";
  _binop[BINOPGREATER] = ">";
  _binop[BINOPGREATEREQ] = ">=";
  _binop[BINOPCOMPNE] = "!=";
  _binop[BINOPCOMPEQ] = "==";
  _binop[BINOPADD] = "+";
  _binop[BINOPELADD] = "+";
  _binop[BINOPSUB] = "-";
  _binop[BINOPELSUB] = "-";
  _binop[BINOPDIV] = "/";
  _binop[BINOPELDIV] = "/";
  _binop[BINOPMULT] = "*";
  _binop[BINOPELMULT] = "*";
  _binop[BINOPEXP] = "^";
  _binop[BINOPELEXP] = "^";
  addCompiledFunctions(BuiltInFunction::instance().functions());
}

Utils::~Utils() {}

string Utils::trimString(string str)
{
  // trim trailing spaces
  string ret = str;
  size_t endpos = ret.find_last_not_of(" \t");
  if (string::npos != endpos) {
    ret = ret.substr(0, endpos + 1);
  }

  // trim leading spaces
  size_t startpos = ret.find_first_not_of(" \t");
  if (string::npos != startpos) {
    ret = ret.substr(startpos);
  }
  return ret;
}

string Utils::languageEspecification() { return _language_especification; }

void Utils::setCompileFlags(CompileFlags flags) { _flags = flags; }

bool Utils::checkTypeString(string t)
{
  if (t != "Integer" && t != "Real" && t != "Boolean") {
    return false;
  }
  return true;
}

bool Utils::checkExperimentAnnotations(string *annotation)
{
  map<string, int>::iterator it = _annotations.find(*annotation);
  return it != _annotations.end();
}

string Utils::opString(BinOpType bot) { return _binop[bot]; }

string Utils::getVarName(string name)
{
  string ret;
  int ub = name.length() - 1;
  for (int i = 0; i < ub; i++) {
    ret += name[i];
  }
  return ret;
}

string Utils::iteratorVar(int dim)
{
  stringstream buffer;
  buffer << "_d" << dim + 1;
  return buffer.str();
}

SymbolTable Utils::getValue(fstream *package, string token)
{
  SymbolTable ret;
  string line;
  while (getline(*package, line)) {
    if (!line.compare(token)) {
      break;
    }
    ret.insert(line, line);
  }
  return ret;
}

Option<CompiledPackage> Utils::readPackage(string fileName)
{
  fstream package;
  string pname = packageName(fileName);
  string name = packagePath(pname + ".c");
  name.append(pname).append(".moo");
  package.open(name.c_str());
  if (package.good()) {
    string line;
    CompiledFunctionTable cft;
    ImportTable objects;
    CompiledPackage cp(fileName);
    string fname;
    string prefix = cp.prefix();
    string derivative;
    string includeDir;
    string libraryDir;
    SymbolTable libraries;
    while (getline(package, line)) {
      if (!line.compare("DEPENDENCES")) {
        objects = getValue(&package, "ENDDEPENDENCES");
      } else if (!line.compare("FUNCTION")) {
        fname = getValue(&package, "ENDFUNCTION").first();
      } else if (!line.compare("DERIVATIVE")) {
        derivative = getValue(&package, "ENDDERIVATIVE").first();
      } else if (!line.compare("INCLUDEDIRECTORY")) {
        includeDir = getValue(&package, "ENDINCLUDEDIRECTORY").first();
      } else if (!line.compare("LIBRARYDIRECTORY")) {
        libraryDir = getValue(&package, "ENDLIBRARYDIRECTORY").first();
      } else if (!line.compare("LIBRARIES")) {
        libraries = getValue(&package, "ENDLIBRARIES");
      } else if (!line.compare("ENDDEFINITION")) {
        CompiledFunction fi(fname, includeDir, libraryDir, libraries, prefix);
        cft.insert(fname, fi);
      }
    }
    package.close();
    cp.setObjects(objects);
    cp.setDefinitions(cft);
    return Option<CompiledPackage>(cp);
  }
  return Option<CompiledPackage>();
}

bool Utils::readPackage(string fileName, CompiledPackageTable &pt)
{
  Option<CompiledPackage> cp = readPackage(fileName);
  if (!cp) {
    return false;
  }
  pt.insert(cp.get().name(), cp.get());
  return true;
}

bool Utils::searchCompiledPackage(string pname, CompileFlags flags) { return !packagePath(packageName(pname), flags, ".moo").empty(); }

bool Utils::checkCodeFiles(string name, string ext)
{
  if (!ext.compare(".mo")) {
    return true;
  }
  ifstream h((name + ".h").c_str());
  ifstream c((name + ".c").c_str());
  if (h.good() && c.good()) {
    h.close();
    c.close();
    return true;
  }
  return false;
}

string Utils::packagePath(string name)
{
  list<string> objs = _flags.objects();
  for (list<string>::iterator it = objs.begin(); it != objs.end(); it++) {
    string i = *it;
    string p = *it;
    size_t found = i.rfind("/");
    if (found != std::string::npos) {
      p.erase(p.begin() + found + 1, p.end());
      i.erase(i.begin(), i.begin() + found + 1);
    }
    if (!i.compare(name)) {
      return p;
    }
  }
  return "";
}

string Utils::packagePath(string pname, CompileFlags flags, string ext)
{
  string fname = flags.path();
  fname.append(pname + ext);
  ifstream f(fname.c_str());
  if (f.good() && checkCodeFiles(flags.path() + pname, ext)) {
    f.close();
    return flags.path();
  } else {
    f.close();
    if (flags.hasOutputFile()) {
      fname = flags.outputFilePath();
      fname.append(SLASH + pname + ext);
      f.open(fname.c_str());
      if (f.good() && checkCodeFiles(flags.outputFilePath() + SLASH + pname, ext)) {
        f.close();
        return flags.outputFilePath();
      }
      f.close();
    }
    list<string> paths = flags.libraryPaths();
    for (list<string>::iterator it = paths.begin(); it != paths.end(); it++) {
      fname = *it;
      fname.append(SLASH + pname + ext);
      f.open(fname.c_str());
      if (f.good() && checkCodeFiles(*it + SLASH + pname, ext)) {
        f.close();
        return *it;
      }
      f.close();
    }
  }
  return "";
}

string Utils::packageName(string name)
{
  size_t f = name.find("pkg_");
  if (f == string::npos) {
    return "pkg_" + name;
  }
  return name;
}

string Utils::environmentVariable(string ev)
{
  char *li = getenv(ev.c_str());
  if (li != nullptr) {
    return li;
  }
  return "";
}

string Utils::getFilePath(string file)
{
  _file_name = file;
  string path = file;
  size_t found = path.rfind("/");
  if (found != std::string::npos) {
    path.erase(path.begin() + found + 1, path.end());
  }
  return path;
}

string Utils::getFileName(string file)
{
  string fn = file;
  size_t f = fn.rfind("/");
  if (f == std::string::npos) {
    return file;
  }
  fn.erase(fn.begin(), fn.begin() + f + 1);
  return fn;
}

string Utils::baseFileName(string file)
{
  string base_file = getFileName(file);
  size_t f = base_file.rfind(".");
  if (f == std::string::npos) {
    return base_file;
  }
  base_file.erase(base_file.begin() + f, base_file.end());
  return base_file;
}

CompiledFunctionTable Utils::compiledFunctions() { return _compiled_functions; }

void Utils::addCompiledFunction(CompiledFunction f) { _compiled_functions.insert(f.name(), f); }

void Utils::addCompiledFunctions(CompiledFunctionTable fs) { _compiled_functions.merge(fs); }

bool Utils::checkCompiledFunctions(string name)
{
  Option<CompiledFunction> cf = _compiled_functions[name];
  return cf.is_initialized();
}

bool Utils::checkBuiltInFunctions(string name) { return BuiltInFunction::instance().lookup(name); }

Variable Utils::variable(AST_Expression exp)
{
  string var_name;
  ExpressionType type = exp->expressionType();
  if (type == EXPCOMPREF) {
    AST_Expression_ComponentReference var_exp = exp->getAsComponentReference();
    var_name = *AST_ListFirst(var_exp->names());
  } else if (type == EXPDERIVATIVE) {
    AST_Expression_Derivative der = exp->getAsDerivative();
    AST_Expression args = AST_ListFirst(der->arguments());
    assert(args->expressionType() == EXPCOMPREF);
    AST_Expression_ComponentReference var_exp = args->getAsComponentReference();
    var_name = *AST_ListFirst(var_exp->names());
  }
  Option<Variable> var = ModelConfig::instance().lookup(var_name);
  if (!var) {
    Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "utils.cpp:342 %s", var_name.c_str());
  }
  return var.get();
}

Expression Utils::variableExpression(string name, Option<IR::Range> range)
{
  AST_Expression_ComponentReference var = newAST_Expression_ComponentReference();
  if (range) {
    RangeDefinitionTable ranges = range->definition();
    AST_ExpressionList l = newAST_ExpressionList();
    int dim = 0;
    for (auto r : ranges) {
      string idx_var = range->iterator(dim++);
      AST_Expression idx = newAST_Expression_ComponentReferenceExp(newAST_String(idx_var));
      l = AST_ListAppend(l, idx);
    }
    var = AST_Expression_ComponentReference_Add(var, newAST_String(name), l);
  }
  return Expression(var);
}

string Utils::tabs(int t)
{
  string tab;
  for (int i = 0; i < t; i++) {
    tab.append(TAB);
  }
  return tab;
}

CompileFlags Utils::compileFlags()
{
  return _flags;
}


}  // namespace Util
}  // namespace MicroModelica
