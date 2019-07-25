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

#ifndef CALLED_FUNCTIONS_H_
#define CALLED_FUNCTIONS_H_

#include <string>

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

/**
 *
 */
class CalledFunctions : public AST_Expression_Visitor<SymbolTable> {
  public:
  CalledFunctions(){};
  ~CalledFunctions(){};

  private:
  SymbolTable foldTraverseElement(AST_Expression exp);
  SymbolTable foldTraverseElement(SymbolTable l, SymbolTable r, BinOpType bot);
  SymbolTable foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };
  SymbolTable _symbols;
};

/**
 *
 */
class StatementCalledFunctions : public AST_Statement_Visitor<SymbolTable, SymbolTable, CalledFunctions> {
  public:
  StatementCalledFunctions() : AST_Statement_Visitor(CalledFunctions()){};
  ~StatementCalledFunctions(){};

  private:
  inline SymbolTable foldTraverse(SymbolTable symbols) { return symbols; };
  SymbolTable foldTraverse(SymbolTable s1, SymbolTable s2);
};

}  // namespace Util
}  // namespace MicroModelica

#endif /* CALLED_FUNCTIONS_H_ */
