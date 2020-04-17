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

#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <list>
#include <map>
#include <string>

#include "../ast/ast_types.h"
#include "../deps/model_dependencies.h"
#include "compile_flags.h"
#include "util_types.h"
#include "symbol_table.h"
#include "table.h"

namespace MicroModelica {
namespace IR {
class Equation;
class CompiledPackage;
class CompiledFunction;
typedef ModelTable<std::string, CompiledPackage> CompiledPackageTable;
typedef ModelTable<std::string, CompiledFunction> CompiledFunctionTable;
typedef ModelTable<int, Equation> EquationTable;
}  // namespace IR
namespace Util {

typedef ModelTable<std::string, std::string> ImportTable;

typedef ModelTable<std::string, std::string> SymbolTable;

#define SLASH "/"
#define BINOPS 18

#if __x86_64__
typedef long int grp_t;
#else
typedef int grp_t;
#endif

#define TAB "\t"

class Utils {
  public:
  static Utils& instance()
  {
    static Utils _instance;
    return _instance;
  }

  ~Utils();
  std::string trimString(std::string str);
  std::string languageEspecification();
  bool checkTypeString(std::string t);
  bool checkExperimentAnnotations(std::string* annotation);
  std::string opString(BinOpType bot);
  std::string iteratorVar(int dim);
  std::string getVarName(std::string name);
  bool readPackage(std::string fileName, IR::CompiledPackageTable& pt);
  Option<IR::CompiledPackage> readPackage(std::string file_name);
  bool searchCompiledPackage(std::string pname, CompileFlags flags);
  std::string packagePath(std::string pname, CompileFlags flags, std::string ext = ".mo");
  void setCompileFlags(CompileFlags flags);
  std::string packageName(std::string name);
  std::string environmentVariable(std::string ev);
  std::string getFilePath(std::string file);
  std::string getFileName(std::string file);
  bool checkGKLinkFunctions(std::string name) { return false; };
  IR::CompiledFunctionTable compiledFunctions();
  void addCompiledFunction(IR::CompiledFunction f);
  void addCompiledFunctions(IR::CompiledFunctionTable fs);
  bool checkCompiledFunctions(std::string name);
  bool checkBuiltInFunctions(std::string name);
  inline VarSymbolTable& symbols() { return _symbols; };
  inline SymbolTable localSymbols() { return _local_symbols; };
  inline SymbolTable localInitSymbols() { return _local_init_symbols; };
  inline void addLocalSymbol(std::string symbol)
  {
    if (_init_symbols) {
      _local_init_symbols.insert(symbol, symbol);
    } else {
      _local_symbols.insert(symbol, symbol);
    }
  };
  inline void addVariable(VarName name, Variable var) { _symbols.insert(name, var); }
  inline void setSymbols(const VarSymbolTable& symbols) { _symbols = symbols; };
  inline void clearLocalSymbols() { _local_symbols.clear(); };
  inline void setLocalInitSymbols() { _init_symbols = true; };
  inline void unsetLocalInitSymbols() { _init_symbols = false; };
  inline std::string fileName() { return _file_name; };
  inline int id() { return _ids++; };
  Variable variable(AST_Expression exp, VarSymbolTable& symbols);
  IR::Expression variableExpression(string name, Option<IR::Range> range, const VarSymbolTable& symbols);

  private:
  Utils();
  bool checkCodeFiles(std::string name, std::string ext);
  SymbolTable getValue(fstream* package, std::string token);
  std::string packagePath(std::string name);

  std::string _language_especification;
  CompileFlags _flags;
  map<std::string, int> _annotations;
  std::string _binop[BINOPS];
  IR::CompiledFunctionTable _compiled_functions;
  VarSymbolTable _symbols;
  SymbolTable _local_symbols;
  SymbolTable _local_init_symbols;
  std::string _file_name;
  bool _init_symbols;
  int _ids;
};

}  // namespace Util
}  // namespace MicroModelica

#endif /* UTIL_H_ */
