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

#ifndef EXPRESSION_PRINTER_H_
#define EXPRESSION_PRINTER_H_

#include <string>

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ExpressionPrinter : public AST_Expression_Visitor<std::string> {
  public:
  ExpressionPrinter(const VarSymbolTable& symbols);
  /**
   *
   */
  ~ExpressionPrinter(){};
  inline std::string code() const { return _code; };

  private:
  std::string foldTraverseElement(AST_Expression exp);
  std::string foldTraverseElement(std::string l, std::string r, BinOpType bot);
  std::string foldTraverseElementUMinus(AST_Expression exp);
  VarSymbolTable _symbols;
  MicroModelica::IR::Expression _exp;
  std::string _code;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* EXPRESSION_PRINTER_H_ */
