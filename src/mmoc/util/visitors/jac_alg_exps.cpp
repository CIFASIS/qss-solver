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

#include "jac_alg_exps.h"

#include <math.h>

#include "../../ast/ast_builder.h"
#include "../debug.h"
#include "../error.h"
#include "../symbol_table.h"
#include "algebraics.h"
#include "has_scalar_index.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

JacAlgTerm::JacAlgTerm(Option<Range> range) : _alg_term(nullptr), _alg_exps(), _range(range) {}

AST_Expression JacAlgTerm::algTerm() const { return _alg_term; }

JacAlgTerm::AlgTermList JacAlgTerm::algExps() const { return _alg_exps; }

void JacAlgTerm::setAlgTerm(AST_Expression alg_term) { _alg_term = alg_term; }

void JacAlgTerm::setAlgExps(AST_ExpressionList alg_exps)
{
  assert(alg_exps->size() > 0);
  AST_ExpressionListIterator it;
  foreach (it, alg_exps) {
    AST_Expression alg = current_element(it);
    assert(alg->expressionType() == EXPCOMPREF);
    HasScalarIndex scalar_index;
    if (scalar_index.apply(alg)) {
      _alg_exps.push_back(make_pair(alg, Range(alg)));
    } else {
      _alg_exps.push_back(make_pair(alg, _range));
    }
  }
}

JacAlgExps::JacAlgExps(Variable var, Option<Range> range) : _alg_terms(), _var(var), _range(range) {}

list<JacAlgTerm> JacAlgExps::algebraicTerms() { return _alg_terms; }

AST_Expression JacAlgExps::foldTraverseElement(AST_Expression exp) { return exp; }

AST_Expression JacAlgExps::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression_Integer zero = (AST_Expression_Integer)newAST_Expression_Integer(0);
  return (foldTraverseElement(zero, apply(exp->getAsUMinus()->exp()), BINOPSUB));
}

AST_Expression JacAlgExps::addJacAlgTerm(AST_Expression left, AST_Expression right, BinOpType bin_op)
{
  Algebraics algs;
  algs.exclude(_var);
  const bool LEFT_ALGEBRAICS = algs.apply(left);
  const bool RIGHT_ALGEBRAICS = algs.apply(right);
  if (LEFT_ALGEBRAICS && RIGHT_ALGEBRAICS) {
    JacAlgTerm jac_alg_term(_range);
    jac_alg_term.setAlgExps(algs.exps());
    jac_alg_term.setAlgTerm(newAST_Expression_BinOp(left, right, BINOPADD));
    return newAST_Expression_Integer(0);
  } else if (LEFT_ALGEBRAICS) {
    JacAlgTerm jac_alg_term(_range);
    jac_alg_term.setAlgExps(algs.exps());
    jac_alg_term.setAlgTerm(left);
    _alg_terms.push_back(jac_alg_term);
    return right;
  } else if (LEFT_ALGEBRAICS) {
    JacAlgTerm jac_alg_term(_range);
    jac_alg_term.setAlgExps(algs.exps());
    jac_alg_term.setAlgTerm(right);
    _alg_terms.push_back(jac_alg_term);
    return left;
  } else {
    return newAST_Expression_BinOp(left, right, bin_op);
  }
}

AST_Expression JacAlgExps::foldTraverseElement(AST_Expression left, AST_Expression right, BinOpType binOpType)
{
  switch (binOpType) {
  case BINOPADD:
    return addJacAlgTerm(left, right, BINOPADD);
  case BINOPSUB:
    return addJacAlgTerm(left, right, BINOPSUB);
  case BINOPMULT:
    return newAST_Expression_BinOp(left, right, BINOPMULT);
  case BINOPDIV:
    return newAST_Expression_BinOp(left, right, BINOPDIV);
  case BINOPEXP:
    return newAST_Expression_BinOp(left, right, BINOPEXP);
  default:
    break;
  }
  assert(false);
  return nullptr;
}

}  // namespace Util
}  // namespace MicroModelica
