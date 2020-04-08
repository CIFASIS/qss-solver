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

#ifndef ALGEBRAICS_H_
#define ALGEBRAICS_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

class Algebraics : public AST_Expression_Visitor<bool> {
  public:
  Algebraics();
  ~Algebraics() = default;

  void exclude(Variable var);

  AST_ExpressionList exps() const { return _exps; };

  protected:
  VarSymbolTable _symbols;

  private:
  bool foldTraverseElement(AST_Expression exp);
  bool foldTraverseElement(bool l, bool r, BinOpType bot) { return l || r; };
  bool foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };

  AST_ExpressionList _exps;
  Variable _var;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* ALGEBRAICS_H_ */
