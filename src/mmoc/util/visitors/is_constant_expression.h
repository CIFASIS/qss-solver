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

#include <ast/ast_builder.h>
#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {
class IsConstantExpression : public AST_Expression_Visitor<bool> {
  public:
  IsConstantExpression();
  IsConstantExpression(bool eval_int);
  ~IsConstantExpression() = default;

  private:
  bool foldTraverseElement(AST_Expression exp);
  inline bool foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); }
  inline bool foldTraverseElement(bool l, bool r, BinOpType bot) { return l && r; }
  bool _in_index_list;
  bool _eval_int;
};

}  // namespace Util
}  // namespace MicroModelica
