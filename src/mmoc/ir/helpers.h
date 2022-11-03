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

#include <string>

#include <ir/annotation.h>
#include <ir/index.h>
#include <ir/equation.h>
#include <ir/expression.h>
#include <util/symbol_table.h>
#include <util/util.h>

namespace MicroModelica {
namespace IR {

/**
 * @breif Wrapper to external function calls inside function or model definitions.
 *
 */
class ExternalFunction {
  public:
  ExternalFunction(){};
  ExternalFunction(std::string lvalue, std::string name, AST_ExpressionList args);
  ~ExternalFunction() = default;
  friend std::ostream& operator<<(std::ostream& out, const ExternalFunction& e);

  private:
  std::string _lvalue;
  std::string _name;
  AST_ExpressionList _args;
};

typedef ModelTable<int, ExternalFunction> ExternalFunctionTable;

class CompiledFunction {
  public:
  CompiledFunction();
  CompiledFunction(std::string name, std::string includeDir, std::string libraryDir, Util::SymbolTable& libraries,
                   std::string prefix = "__");
  ~CompiledFunction() = default;
  inline bool hasIncludeDirectory() const { return !_includeDirectory.empty(); };
  inline bool hasLibraryDirectory() const { return !_libraryDirectory.empty(); };
  inline bool hasLibraries() const { return _libraries.size() > 0; };
  inline std::string includeDirectory() const { return _includeDirectory; };
  inline std::string libraryDirectory() const { return _libraryDirectory; };
  inline Util::SymbolTable libraries() { return _libraries; };
  inline std::string name() const { return _name; };
  inline std::string prototype() const { return _prototype; };
  friend std::ostream& operator<<(std::ostream& out, const CompiledFunction& cf);
  std::string print() const;
  void setArguments(AST_ExpressionList arguments) { _arguments = arguments; };
  void setOutputArguments(AST_ExpressionList output_arguments) { _output_arguments = output_arguments; };

  private:
  std::string _name;
  std::string _prefix;
  std::string _prototype;
  std::string _includeDirectory;
  std::string _libraryDirectory;
  Util::SymbolTable _libraries;
  AST_ExpressionList _arguments;
  AST_ExpressionList _output_arguments;
};

typedef ModelTable<std::string, CompiledFunction> CompiledFunctionTable;

class CompiledPackage {
  public:
  CompiledPackage();
  CompiledPackage(string name, CompiledFunctionTable cft, Util::ImportTable objects);
  CompiledPackage(string name);
  ~CompiledPackage(){};
  string name();
  string prefix();
  Util::SymbolTable linkLibraries();
  Util::SymbolTable includeDirectories();
  Util::SymbolTable libraryDirectories();
  CompiledFunctionTable definitions();
  Util::ImportTable objects();
  inline void setObjects(Util::ImportTable& objects) { _objects = objects; };
  inline void setDefinitions(CompiledFunctionTable& cft) { _cft = cft; };

  private:
  std::string _name;
  CompiledFunctionTable _cft;
  Util::ImportTable _objects;
};

typedef ModelTable<std::string, CompiledPackage> CompiledPackageTable;

namespace FUNCTION_PRINTER {
typedef enum { Return, Break, Continue } ReturnStatementType;
}

class FunctionPrinter {
  public:
  ~FunctionPrinter() = default;
  std::string loop(int end);
  std::string endLoop();
  std::string beginSwitch();
  std::string endSwitch();
  std::string beginExpression(std::string token, Option<Range> range) const;
  std::string endExpression(Option<Range> range,
                            FUNCTION_PRINTER::ReturnStatementType ret = FUNCTION_PRINTER::ReturnStatementType::Return) const;
  std::string jacobianTerms(list<Equation> eqs);
  std::string getIndexes(string var, Option<Range> range, int offset, bool modelica_index) const;
  std::map<std::string, std::string> parseIndexes(string var, Option<Range> range, int offset, bool modelica_index = true) const;
  std::map<int, std::string> parseConstants(Expression ref) const;
  std::string beginDimGuards(std::string token, string args, Option<Range> range, std::multimap<std::string, int> used_variables) const;
  std::string endDimGuards(Option<Range> range) const;
  std::string printAlgebraicGuards(Equation alg, Index usage);
  /// TODO: Review modelica_index parameter usage.
  std::string accessMacros(std::string token, int offset, Option<Range> range, bool modelica_index = true) const;
  std::string equationVariableMacros(Option<Range> range, Expression lhs, std::string id) const;
  std::string jacMacrosAccess(Equation eq, std::string index = "row", std::string tab = TAB) const;
  std::string outputVariableName(Expression exp, Option<Range> range);
  std::string algebraic(Equation alg, bool reduction);

  protected:
  map<std::string, std::string> _alg_dict;

  private:
  std::string mod(std::string var, int dim, Option<Range> range) const;
  std::string div(std::string var, int dim, Option<Range> range) const;
};

class Input {
  public:
  Input(){};
  Input(Index idx, Option<Range> range, int id);
  ~Input(){};
  std::string print() const;
  std::string macro() const;
  friend std::ostream& operator<<(std::ostream& out, const Input& i);

  private:
  std::string token() const;
  Index _idx;
  Option<Range> _range;
  int _id;
};

typedef ModelTable<int, Input> InputTable;

}  // namespace IR
}  // namespace MicroModelica
