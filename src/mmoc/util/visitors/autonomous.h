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

#ifndef AUTONOMOUS_H_
#define AUTONOMOUS_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
/**
 *
 */
class Autonomous : public AST_Expression_Visitor<bool> {
  public:
  /**
   *
   */
  Autonomous(VarSymbolTable symbols) : _symbols(symbols){};
  /**
   *
   */
  ~Autonomous(){};

  private:
  bool foldTraverseElement(AST_Expression exp);
  bool foldTraverseElement(bool l, bool r, BinOpType bot) { return l && r; };
  bool foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };
  VarSymbolTable _symbols;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* AUTONOMOUS_H_ */
