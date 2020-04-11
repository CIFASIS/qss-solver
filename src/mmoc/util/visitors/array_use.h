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

#ifndef ARRAY_USE_H_
#define ARRAY_USE_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ArrayUse : public AST_Expression_Visitor<bool> {
  public:
  ArrayUse(VarSymbolTable symbols) : _symbols(symbols){};
  ~ArrayUse() = default;

  protected:
  ArrayUse() : _symbols() {}

  VarSymbolTable _symbols;

  private:
  bool foldTraverseElement(AST_Expression exp);
  bool foldTraverseElement(bool l, bool r, BinOpType bot) { return l && r; };
  bool foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };
};

class StatementArrayUse : public AST_Statement_Visitor<bool, bool, ArrayUse> {
  public:
  StatementArrayUse(VarSymbolTable symbols) : AST_Statement_Visitor(ArrayUse(symbols)){};
  ~StatementArrayUse() = default;

  private:
  inline bool foldTraverse(bool ret) { return ret; };
  inline bool foldTraverse(bool l, bool r) { return l && r; };
};

}  // namespace Util
}  // namespace MicroModelica

#endif /* ARRAY_USE_H_ */
