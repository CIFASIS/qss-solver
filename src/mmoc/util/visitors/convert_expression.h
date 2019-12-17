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

#ifndef CONVERT_EXPRESSION_H_
#define CONVERT_EXPRESSION_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

class ConvertExpression {
  public:
  ConvertExpression(AST_Expression left, AST_Expression right, MicroModelica::Util::VarSymbolTable& symbols);
  /**
   *
   */
  ~ConvertExpression(){};
  inline std::string get() { return _convert; };

  private:
  AST_Expression_ComponentReference componentReference(AST_Expression exp);
  MicroModelica::Util::Variable variable(AST_Expression_ComponentReference exp);
  bool scalarExpression(AST_Expression exp);
  double scalarValue(AST_Expression exp);
  void convert();
  AST_Expression _left;
  AST_Expression _right;
  MicroModelica::Util::VarSymbolTable _symbols;
  std::string _convert;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* CONVERT_EXPRESSION_H_ */