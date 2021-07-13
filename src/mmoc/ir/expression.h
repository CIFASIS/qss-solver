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

#include <ast/ast_types.h>
#include <util/symbol_table.h>

namespace MicroModelica {
namespace IR {

class Expression {
  public:
  Expression();
  Expression(AST_Expression exp, int order = 0);
  ~Expression() = default;
  std::string print() const;
  inline AST_Expression expression() const { return _exp; };
  bool isReference() const;
  std::string usage() const;
  std::string dimVariables(bool range_idxs = false) const;
  bool isEmpty() const { return _exp == nullptr; };
  bool isValid() const { return _exp != nullptr; };
  Option<Util::Variable> reference() const;
  bool isScalar() const;
  list<Expression> indexes() const;
  static Expression generate(std::string var_name, std::vector<std::string> indices);
  bool operator<(const Expression &other) const;

  friend std::ostream& operator<<(std::ostream& out, const Expression& s);

  protected:
  std::vector<Expression> usageExps() const;

  private:
  AST_Expression _exp;
  int _order;
};

typedef list<Expression> ExpressionList;
}  // namespace IR
}  // namespace MicroModelica
