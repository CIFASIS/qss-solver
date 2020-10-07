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

#ifndef REPLACE_INDEX_H_
#define REPLACE_INDEX_H_

#include "../../ast/ast_builder.h"
#include "../../deps/graph/graph_helpers.h"
#include "../../ir/index.h"
#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

class ReplaceIndex : public AST_Expression_Visitor<AST_Expression> {
  public:
  ReplaceIndex(IR::Range range, Deps::Usage usage, bool range_idxs);
  ~ReplaceIndex() = default;

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);

  IR::Range _range;
  Deps::Usage _usage;
  bool _range_idxs;
};

class ReplaceVar : public AST_Expression_Visitor<AST_Expression> {
  public:
  ReplaceVar(std::string var);
  ~ReplaceVar() = default;

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);

  std::string _var;
};

}  // namespace Util
}  // namespace MicroModelica
#endif /* REPLACE_INDEX_H_ */
