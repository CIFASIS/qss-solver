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

#ifndef JAC_ALG_EXPS_H_
#define JAC_ALG_EXPS_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {

class JacAlgTerm {
  public:
  typedef std::pair<AST_Expression, Option<IR::Range>> AlgTermInfo;
  typedef std::list<AlgTermInfo> AlgTermList;

  JacAlgTerm(Option<IR::Range> range);
  ~JacAlgTerm() = default;

  AST_Expression algTerm() const;
  AlgTermList algExps() const;

  void setAlgTerm(AST_Expression alg_term);
  void setAlgExps(AST_ExpressionList alg_exps);

  protected:
  AST_Expression _alg_term;
  AlgTermList _alg_exps;
  Option<IR::Range> _range;
};

class JacAlgExps : public AST_Expression_Fold<AST_Expression> {
  public:
  JacAlgExps(Variable var, Option<IR::Range> range);
  ~JacAlgExps() = default;

  list<JacAlgTerm> algebraicTerms();

  protected:
  AST_Expression addJacAlgTerm(AST_Expression left, AST_Expression right, BinOpType bin_op);

  private:
  AST_Expression foldTraverseElement(AST_Expression);
  AST_Expression foldTraverseElementUMinus(AST_Expression);
  AST_Expression foldTraverseElement(AST_Expression, AST_Expression, BinOpType);

  list<JacAlgTerm> _alg_terms;
  Variable _var;
  Option<IR::Range> _range;
};
}  // namespace Util
}  // namespace MicroModelica

#endif /* JAC_ALG_EXPS_H */
