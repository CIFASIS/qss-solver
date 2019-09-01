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

#ifndef CONVERT_OUTPUT_RANGE_H_
#define CONVERT_OUTPUT_RANGE_H_

#include "../../ast/ast_builder.h"
#include "../../deps/graph/graph_helpers.h"
#include "../../ir/index.h"
#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ConvertOutputRange : public AST_Expression_Visitor<AST_Expression> {
  public:
  ConvertOutputRange(VarSymbolTable& symbols) : _symbols(symbols), _range(), _intervals(), _dim(0), _var(){};
  ~ConvertOutputRange() = default;
  Option<IR::Range> range();

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  inline AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
  {
    return newAST_Expression_BinOp(l, r, bot);
  }
  inline AST_Expression foldTraverseElementUMinus(AST_Expression exp) { return exp; };

  VarSymbolTable& _symbols;
  IR::Range _range;
  Deps::IntervalList _intervals;
  int _dim;
  std::string _var;
};

}  // namespace Util
}  // namespace MicroModelica

#endif /* CONVERT_OUTPUT_RANGE_H_ */
