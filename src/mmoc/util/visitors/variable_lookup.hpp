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

#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {

class VariableLookup : public AST_Expression_Fold<bool> {
  public:
  VariableLookup(MicroModelica::Util::VarSymbolTable symbols);
  ~VariableLookup() = default;

  private:
  bool foldTraverseElement(AST_Expression exp);
  bool foldTraverseElement(bool l, bool r, BinOpType);
  bool foldTraverseElementUMinus(AST_Expression exp);
  
  VarSymbolTable _symbols;
};

}  // namespace Util
}  // namespace MicroModelica
