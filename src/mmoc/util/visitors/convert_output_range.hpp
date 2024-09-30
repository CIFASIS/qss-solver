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

#include <ast/ast_builder.h>
#include <deps/sbg_graph/multi_interval.h>
#include <ir/index.h>
#include <util/ast_util.h>

namespace MicroModelica {
namespace Util {
class ConvertOutputRange : public AST_Expression_Visitor<AST_Expression> {
  public:
  ConvertOutputRange();
  ~ConvertOutputRange() = default;
  Option<IR::Range> range();

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  inline AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
  {
    return newAST_Expression_BinOp(l, r, bot);
  }
  inline AST_Expression foldTraverseElementUMinus(AST_Expression exp) { return exp; };

  AST_Expression generateIndexVariable(int size);
  bool checkExpression(AST_Expression exp);

  IR::Range _range;
  SB::MultiInterval _intervals;
  int _dim;
  std::string _var;
  std::vector<IR::Expression> _begin_exps;
  std::vector<IR::Expression> _end_exps;
};

}  // namespace Util
}  // namespace MicroModelica
