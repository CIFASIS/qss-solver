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

#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {
class ExpressionPrinter : public AST_Expression_Visitor<std::string> {
  public:
  ExpressionPrinter(int order);
  ~ExpressionPrinter() = default;

  private:
  std::string foldTraverseElement(AST_Expression exp);
  std::string foldTraverseElement(std::string l, std::string r, BinOpType bot);
  std::string foldTraverseElementUMinus(AST_Expression exp);

  IR::Expression _exp;
  int _order;
};

class VariablePrinter {
  public:
  VariablePrinter(Variable var, AST_Expression_ComponentReference ref, int order);
  ~VariablePrinter() = default;
  friend std::ostream &operator<<(std::ostream &out, const VariablePrinter &var);

  protected:
  void generate();
  void config();
  std::string access(bool arrray_access) const;

  private:
  Variable _var;
  AST_Expression_ComponentReference _ref;
  int _order;
  std::string _exp;
  std::string _begin_delimiter;
  std::string _end_delimiter;
  std::string _begin_index_access;
  std::string _end_index_access;
};

}  // namespace Util
}  // namespace MicroModelica
