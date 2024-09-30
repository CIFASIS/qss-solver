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
#include "../util/visitors/convert_cont_red.h"
#include "../util/visitors/convert_disc_red.h"
#include "helpers.h"

namespace MicroModelica {
namespace IR {

template <class T, class C>
class ReductionFunctions {
  public:
  ReductionFunctions(AST_Expression expression, Util::Variable lhs)
      : _has_reduction_function(false), _expression(expression), _lhs(lhs){};

  bool hasReductionFunctions() { return _has_reduction_function; };

  list<T> code() { return _code; };

  list<Util::Variable> variables() { return _variables; };

  AST_Expression apply()
  {
    C convert_red;
    convert_red.setLHS(_lhs);
    AST_Expression converted_exp = _expression;
    int operators = convert_red.operators();
    for (int i = 0; i < operators; i++) {
      convert_red.setReduction(i);
      converted_exp = convert_red.apply(converted_exp);
      _has_reduction_function |= convert_red.hasReduction();
      insertCode(convert_red.code(), convert_red.variables());
    }
    return converted_exp;
  };

  protected:
  void insertCode(list<T> code, list<Util::Variable> variables)
  {
    _code.insert(_code.end(), code.begin(), code.end());
    _variables.insert(_variables.end(), variables.begin(), variables.end());
  }

  bool _has_reduction_function;
  AST_Expression _expression;
  list<T> _code;
  list<Util::Variable> _variables;
  Util::Variable _lhs;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* MMO_REDUCTION_FUNCTIONS_H */
