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

#include "parse_index.h"

#include "../error.h"
#include "../util.h"
#include "partial_eval_exp.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ParseIndex::ParseIndex() : _constant(0), _factor(1), _variable() {}

int ParseIndex::constant() { return _constant; }

int ParseIndex::factor() { return _factor; }

std::string ParseIndex::variable() { return _variable; }

AST_Expression ParseIndex::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    _variable = cr->name();
    break;
  }
  case EXPOUTPUT: {
    AST_Expression_Output output = exp->getAsOutput();
    AST_ExpressionList outputs = output->expressionList();
    AST_ExpressionListIterator it;
    foreach (it, outputs) {
      apply(current_element(it));
    }
  }
  case EXPINTEGER:
    _constant = exp->getAsInteger()->val();
  default:
    break;
  }
  return exp;
}

// @TODO: review if this is needed.
AST_Expression ParseIndex::foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); }

int ParseIndex::getConstant(AST_Expression left, AST_Expression right)
{
  if (left->expressionType() == EXPINTEGER) {
    return left->getAsInteger()->val();
  }
  if (right->expressionType() == EXPINTEGER) {
    return right->getAsInteger()->val();
  }
  assert(false);
  return 0;
}

AST_Expression ParseIndex::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  switch (bot) {
  case BINOPADD:
    _constant = getConstant(l, r);
    break;
  case BINOPSUB:
    _constant = -1 * getConstant(l, r);
    break;
  case BINOPMULT:
    _factor = getConstant(l, r);
    break;
  default:
    break;
  }
  return newAST_Expression_BinOp(l, r, bot);
}

}  // namespace Util
}  // namespace MicroModelica
