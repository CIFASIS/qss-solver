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

#ifndef ALG_USAGE_H
#define ALG_USAGE_H

#include "index.h"
#include "expression.h"

namespace MicroModelica {
namespace IR {

class VariableUsage {
  public:
  VariableUsage(Expression lhs, Expression rhs, Index usage);
  VariableUsage(Expression usage, Range range);
  VariableUsage(Expression exp, Index usage);
  ~VariableUsage() = default;

  Expression lhs() const;
  Expression rhs() const;
  Expression usage() const;

  protected:
  void generateMapFromUsage();
  void generateMapFromRange();
  void generateMapFromLHS();

  Expression _lhs;
  Expression _rhs;
  Expression _replaced;
  Index _usage;
  Range _range;
  map<std::string, AST_Expression> _usage_map;
};

}  // namespace IR
}  // namespace MicroModelica

#endif /* ALG_USAGE_H */
