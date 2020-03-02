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

namespace MicroModelica {
using namespace IR;
namespace Util {

JacAlgTerm::JacAlgTerm(Option<Range> range) : _alg_term(nullptr), _alg_exps(newAST_ExpressionList()), _range(range) {}

AST_Expression JacAlgTerm::algTerm() const { return _alg_term; }

AST_ExpressionList JacAlgTerm::algExps() const { return _alg_exps; }

Option<Range> JacAlgTerm::range() const { return _range; };

void JacAlgTerm::setAlgTerm(AST_Expression alg_term) { _alg_term = alg_term; }

void JacAlgTerm::setAlgExps(AST_ExpressionList alg_exps) { _alg_exps = alg_exps; }

JacAlgExps::JacAlgExps(Variable var, Option<Range> range) : _alg_terms(), _var(var), _range(range) {}

list<JacAlgTerm> JacAlgExps::algebraicTerms() { return _alg_terms; }

AST_Expression JacAlgExps::foldTraverseElement(AST_Expression exp) { return exp; }

AST_Expression JacAlgExps::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression_Integer zero = (AST_Expression_Integer)newAST_Expression_Integer(0);
  return (foldTraverseElement(zero, apply(exp->getAsUMinus()->exp()), BINOPSUB));
}

AST_Expression JacAlgExps::foldTraverseElement(AST_Expression left, AST_Expression right, BinOpType binOpType)
{
  Algebraics algs;
  algs.exclude(_var);
  switch (binOpType) {
  case BINOPADD:
    if (algs.apply(left)) {
      JacAlgTerm jac_alg_term(_range);
      jac_alg_term.setAlgExps(algs.exps());
      jac_alg_term.setAlgTerm(left);
      _alg_terms.push_back(jac_alg_term);
      return right;
    } else if (algs.apply(right)) {
      JacAlgTerm jac_alg_term(_range);
      jac_alg_term.setAlgExps(algs.exps());
      jac_alg_term.setAlgTerm(right);
      _alg_terms.push_back(jac_alg_term);
      return left;
    } else {
      return newAST_Expression_BinOp(left, right, BINOPADD);
    }
  case BINOPSUB:
    if (algs.apply(left)) {
      JacAlgTerm jac_alg_term(_range);
      jac_alg_term.setAlgExps(algs.exps());
      jac_alg_term.setAlgTerm(left);
      _alg_terms.push_back(jac_alg_term);
      return right;
    } else if (algs.apply(right)) {
      JacAlgTerm jac_alg_term(_range);
      jac_alg_term.setAlgExps(algs.exps());
      jac_alg_term.setAlgTerm(right);
      _alg_terms.push_back(jac_alg_term);
      return left;
    } else {
      return newAST_Expression_BinOp(left, right, BINOPSUB);
    }
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