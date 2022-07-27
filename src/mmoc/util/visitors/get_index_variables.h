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

#include <ir/index.h>
#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {
class GetIndexVariables : public AST_Expression_Visitor<multimap<std::string, int>> {
  public:
  GetIndexVariables();
  ~GetIndexVariables() = default;

  private:
  multimap<std::string, int> foldTraverseElement(AST_Expression exp);
  inline multimap<std::string, int> foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); }
  inline multimap<std::string, int> foldTraverseElement(multimap<std::string, int> l, multimap<std::string, int> r, BinOpType bot)
  {
    l.insert(r.begin(), r.end());
    return l;
  }

  /// Note that these markers only work for arrays with scalar index expressions.
  bool _in_index_list;
  int _pos;
};

}  // namespace Util
}  // namespace MicroModelica
