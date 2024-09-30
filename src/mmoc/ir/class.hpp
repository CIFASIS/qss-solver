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

#include <boost/variant/variant.hpp>
#include <string>
#include <list>
#include <map>
#include <vector>

#include <ast/ast_types.h>
#include <deps/model_dependencies.h>
#include <util/util_types.h>
#include <ir/annotation.h>
#include <ir/equation.h>
#include <ir/event.h>
#include <ir/helpers.h>
#include <ir/reduction_functions.h>
#include <ir/statement.h>

namespace MicroModelica {
namespace IR {

typedef enum { DEC_PUBLIC, DEC_LOCAL } DEC_Type;

class Class {
  public:
  virtual ~Class() = default;
  virtual string name() const = 0;
  virtual void insert(string n) = 0;
  virtual void insert(AST_Equation eq) = 0;
  virtual void insert(AST_Statement stm, bool initial) = 0;
  virtual void insert(AST_Statement stm) = 0;
  virtual void insert(AST_External_Function_Call efc) = 0;
  virtual void insert(VarName n, Util::Variable& vi, DEC_Type type) = 0;
  virtual void insert(VarName n, Util::Variable& vi) = 0;
  virtual void insert(AST_Argument_Modification x) = 0;
  virtual Util::VarSymbolTable symbols() const = 0;
  virtual Util::ImportTable imports() const = 0;
};

class Function : public Class {
  public:
  Function();
  Function(string name);
  virtual ~Function() = default;
  string name() const;
  void insert(AST_External_Function_Call efc);
  void insert(VarName n, Util::Variable& vi, DEC_Type type);
  void insert(VarName n, Util::Variable& vi);
  void insert(AST_Equation eq);
  void insert(AST_Statement stm, bool initial);
  void insert(AST_Statement stm);
  void insert(string n);
  void insert(AST_Argument_Modification x);
  Util::VarSymbolTable symbols() const;
  Util::ImportTable imports() const;
  StatementTable statements() const;
  ExternalFunctionTable externalFunctions() const;
  CompiledPackageTable packages() const;
  unsigned int outputNbr() const;
  FunctionAnnotation annotations() const;
  Util::VariableList arguments() const;

  private:
  Util::ImportTable _imports;
  std::string _name;
  Util::VarSymbolTable _local_symbols;
  FunctionAnnotation _annotations;
  StatementTable _statements;
  CompiledPackageTable _packages;
  Util::VariableList _arguments;
  unsigned int _output_nbr;
  unsigned int _external_function_id;
  unsigned int _statement_id;
  ExternalFunctionTable _external_functions;
};

typedef ModelTable<std::string, Function> FunctionTable;

class Package : public Class {
  public:
  Package(){};
  Package(string name);
  virtual ~Package() = default;
  Util::VarSymbolTable symbols() const;
  string name() const;
  void insert(string n);
  void insert(AST_Equation eq);
  void insert(AST_Statement stm, bool initial);
  void insert(AST_Statement stm);
  void setFunctions(FunctionTable& fs);
  void insert(AST_External_Function_Call efc);
  void insert(VarName n, Util::Variable& vi, DEC_Type type);
  void insert(VarName n, Util::Variable& vi);
  void insert(AST_Argument_Modification x);
  Util::ImportTable imports() const;
  FunctionTable definitions();
  std::string fileName();
  std::string prefix();

  private:
  Util::ImportTable _imports;
  std::string _name;
  FunctionTable _functions;
  CompiledPackageTable _packages;
};

class Model : public Class {
  public:
  Model();
  Model(string name);
  virtual ~Model() = default;
  inline string name() const { return _name; };
  void insert(string n);
  void insert(VarName n, Util::Variable& vi, DEC_Type type);
  void insert(VarName n, Util::Variable& vi);
  void insert(AST_Equation eq);
  void insert(AST_Statement stm, bool initial);
  void insert(AST_Statement stm);
  void setCalledFunctions(FunctionTable& fs);
  void insert(AST_External_Function_Call efc);
  void insert(AST_Argument_Modification x);
  Util::VarSymbolTable symbols() const;
  inline Util::ImportTable imports() const { return _imports; };
  inline ModelAnnotation annotations() const { return _annotations; };
  inline FunctionTable calledFunctions() const { return _called_functions; };
  inline int algebraicNbr() const { return _algebraic_nbr; };
  inline int stateNbr() const { return _state_nbr; };
  inline int discreteNbr() const { return _discrete_nbr; };
  inline int inputNbr() const { return _input_nbr; };
  inline int outputNbr() const { return _output_nbr; };
  inline int eventNbr() const { return _event_nbr; };
  inline Util::SymbolTable linkLibraries() const { return _link_libraries; };
  inline Util::SymbolTable includeDirectories() const { return _include_directories; };
  inline Util::SymbolTable libraryDirectories() const { return _library_directories; };
  inline EquationTable derivatives() { return _derivatives; };
  EquationTable BDFDerivatives();
  inline EquationTable algebraics() { return _algebraics; };
  inline Deps::ModelDependencies dependencies() { return _dependencies; };
  inline EquationTable outputs() { return _outputs; };
  inline EventTable events() { return _events; };
  inline bool externalFunctions() { return _external_functions; };
  inline InputTable inputs() { return _inputs; };
  void setEquations();
  void setEvents();
  void setOutputs();
  void setInputs();
  inline StatementTable initialCode() { return _initial_code; };
  void computeDependencies();
  void setModelConfig();

  protected:
  /**
   * @brief      Adds a new variable for events and output equations that
   *             doesn't have associated variables in the model.
   *
   * @param[in]  id    The identifier
   * @param[in]  size  The size
   * @param[in]  type  The type
   */
  void addVariable(int id, Option<Range> range, EQUATION::Type type, unsigned int& offset);
  void setVariableOffset(Util::Variable var, unsigned int& offset, Util::Variable::RealType type, bool set_variable_count = true);
  void setRealVariables(AST_Equation eq);
  void addEquation(AST_Equation eq, Option<Range> range);
  void reduceEquation(AST_Equation_Equality eq, list<AST_Equation>& new_eqs);
  void addEvent(AST_Statement stm, Option<Range> range);
  void reduceEvent(AST_Statement_When ev);
  void addFunction(Util::SymbolTable symbols, FunctionTable& fs);
  void addInput(Equation eq);
  void orderEquations();
  EquationDefOrder getEquationDefOrder(Equation eq);
  int computeEventOffsetShift(Option<Range> range);

  std::string _name;
  Util::ImportTable _imports;
  ModelAnnotation _annotations;
  FunctionTable _called_functions;
  EquationTable _derivatives;
  EquationTable _ordered_derivatives;
  EquationTable _algebraics;
  EquationTable _outputs;
  EventTable _events;
  InputTable _inputs;
  Deps::ModelDependencies _dependencies;
  CompiledPackageTable _packages;
  StatementTable _initial_code;
  Util::SymbolTable _library_directories;
  Util::SymbolTable _link_libraries;
  Util::SymbolTable _include_directories;
  std::list<AST_Equation> _ast_equations;
  std::list<AST_Statement> _ast_statements;
  unsigned int _state_nbr;
  unsigned int _discrete_nbr;
  unsigned int _algebraic_nbr;
  unsigned int _event_nbr;
  unsigned int _output_nbr;
  unsigned int _input_nbr;
  int _derivative_id;
  int _algebraic_id;
  int _statement_id;
  int _event_id;
  int _output_id;
  int _input_id;
  bool _external_functions;
};

typedef boost::variant<Function, Package, Model> ClassType;

typedef Class* ClassPtr;

}  // namespace IR
}  // namespace MicroModelica
