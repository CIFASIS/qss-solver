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

#include "../deps/dependency_matrix.h"
#include "../ir/annotation.h"
#include "../ir/index.h"
#include "../ir/equation.h"
#include "../ir/expression.h"
#include "symbol_table.h"
#include "util.h"

namespace MicroModelica {
namespace Util {

class ModelConfig {
  public:
  static ModelConfig& instance()
  {
    static ModelConfig _instance;
    return _instance;
  }

  ~ModelConfig() = default;

  inline void setAlgebraics(IR::EquationTable algebraics) { _algebraics = algebraics; };
  inline IR::EquationTable algebraics() { return _algebraics; };
  inline Deps::ModelDependencies dependencies() { return _dependencies; };
  inline void setDependencies(Deps::ModelDependencies dependencies) { _dependencies = dependencies; };
  inline IR::ModelAnnotation modelAnnotations() { return _model_annotations; };
  inline void setModelAnnotations(IR::ModelAnnotation model_annotations) { _model_annotations = model_annotations; };
  bool generateDerivatives() { return _model_annotations.symDiff() && isQss(); };
  inline int order() { return _model_annotations.order(); };
  inline bool isQss() { return !_model_annotations.isClassic(); };
  inline void setDerivatives(IR::EquationTable derivatives) { _derivatives = derivatives; };
  inline IR::EquationTable derivatives() { return _derivatives; };
  inline void setInitialCode(bool initial_code) { _initial_code = initial_code; };
  inline bool initialCode() { return _initial_code; };
  inline void setStateNbr(int state_nbr) { _state_nbr = state_nbr; }
  inline int stateNbr() const { return _state_nbr; }

  // Symbol table access.
  inline VarSymbolTable& symbols() { return _symbols; };
  inline SymbolTable localSymbols() { return _local_symbols; };
  inline SymbolTable localInitSymbols() { return _local_init_symbols; };
  inline Option<Variable> lookup(std::string var_name) { return _symbols[var_name]; };
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

  inline void setEvents(IR::EventTable events) { _events = events; }
  inline IR::EventTable events() const { return _events; }
  private:
  ModelConfig()
      : _model_annotations(),
        _algebraics(),
        _dependencies(),
        _derivatives(),
        _events(),
        _initial_code(false),
        _state_nbr(0),
        _symbols(),
        _local_symbols(),
        _local_init_symbols(),
        _init_symbols(false),
        _types()
        { _symbols.initialize(_types); };

  IR::ModelAnnotation _model_annotations;
  IR::EquationTable _algebraics;
  Deps::ModelDependencies _dependencies;
  IR::EquationTable _derivatives;
  IR::EventTable _events;
  bool _initial_code;
  int _state_nbr;
  VarSymbolTable _symbols;
  SymbolTable _local_symbols;
  SymbolTable _local_init_symbols;
  bool _init_symbols;
  TypeSymbolTable _types;
};

}  // namespace Util
}  // namespace MicroModelica
