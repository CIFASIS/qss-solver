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

#ifndef CONVERT_CONDITION_H_
#define CONVERT_CONDITION_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ConvertCondition : public AST_Expression_Visitor<AST_Expression> {
  public:
  ConvertCondition();
  ~ConvertCondition(){};
  inline MicroModelica::IR::EVENT::Type zeroCrossing() { return _zc; };
  inline MicroModelica::IR::EVENT::Relation zeroCrossingRelation() { return _zcRelation; };

  private:
  AST_Expression foldTraverseElement(AST_Expression exp) { return exp; };
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp) { return apply(exp->getAsUMinus()->exp()); };
  MicroModelica::IR::EVENT::Type _zc;
  MicroModelica::IR::EVENT::Relation _zcRelation;
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* CONVERT_CONDITION_H_ */