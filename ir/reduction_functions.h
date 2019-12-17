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

#ifndef MMO_REDUCTION_FUNCTIONS_H
#define MMO_REDUCTION_FUNCTIONS_H

#include <string>

#include "../util/symbol_table.h"
#include "../util/util.h"
#include "../util/visitors/convert_sum.h"
#include "helpers.h"

namespace MicroModelica {
namespace IR {

template <class T>
class ReductionFunctions {
  public:
  ReductionFunctions(AST_Expression expression, Util::VarSymbolTable &symbols)
      : _has_reduction_function(false), _expression(expression), _symbols(symbols){};
  bool hasReductionFunctions() { return _has_reduction_function; };
  list<T> code() { return _code; };
  list<Util::Variable> variables() { return _variables; };
  AST_Expression apply()
  {
    Util::ConvertSum convert_sum(_symbols);
    AST_Expression converted_exp = convert_sum.apply(_expression);
    _has_reduction_function = convert_sum.hasSum();
    list<T> code = convert_sum.code();
    _code.insert(_code.end(), code.begin(), code.end());
    list<Util::Variable> variables = convert_sum.variables();
    _variables.insert(_variables.end(), variables.begin(), variables.end());
    cout << "Added code: " << _code.size() << endl;
    return converted_exp;
  };

  protected:
  bool _has_reduction_function;
  AST_Expression _expression;
  list<T> _code;
  list<Util::Variable> _variables;
  Util::VarSymbolTable &_symbols;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* MMO_REDUCTION_FUNCTIONS_H */
