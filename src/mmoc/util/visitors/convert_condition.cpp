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

#include "convert_condition.hpp"

#include <sstream>

#include <ast/ast_builder.hpp>
#include <ast/ast_types.hpp>
#include <ast/parser/parse.hpp>
#include "convert_expression.hpp"
#include <ir/event.hpp>
#include <util/error.hpp>
#include <util/util.hpp>
#include "replace_inner_product.hpp"

namespace MicroModelica {
using namespace IR;
namespace Util {

ConvertCondition::ConvertCondition() : _zc(EVENT::Zero), _zcRelation(EVENT::GT) {}

AST_Expression ConvertCondition::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  switch (bot) {
  case BINOPLOWER: {
    AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
    _zc = EVENT::Negative;
    _zcRelation = EVENT::LT;
    return bo;
  }
  case BINOPLOWEREQ: {
    AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
    _zc = EVENT::Negative;
    _zcRelation = EVENT::LE;
    return bo;
  }
  case BINOPGREATER: {
    AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
    _zc = EVENT::Positive;
    _zcRelation = EVENT::GT;
    return bo;
  }
  case BINOPGREATEREQ: {
    AST_Expression bo = newAST_Expression_BinOp(l, newAST_Expression_OutputExpressions(newAST_ExpressionList(r)), BINOPSUB);
    _zc = EVENT::Positive;
    _zcRelation = EVENT::GE;
    return bo;
  }
  default:
    break;
  }
  AST_Expression bo = newAST_Expression_BinOp(l, r, bot);
  return bo;
}
}  // namespace Util
}  // namespace MicroModelica
