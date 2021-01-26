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

#ifndef CONVERT_CONT_RED_H_
#define CONVERT_CONT_RED_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ConvertContRed : public AST_Expression_Visitor<AST_Expression> {
  public:
  typedef enum : int { SUM = 0, PROD, COUNT } ContReduction;

  ConvertContRed();
  ~ConvertContRed() = default;

  void setReduction(int red_operator);
  bool hasReduction();
  list<AST_Equation> code();
  list<Variable> variables();
  int operators() const;
  void setLHS(Variable lhs);

  protected:
  void setOpers();
  std::string operatorTerm(std::string);

  bool _has_reduction;
  list<AST_Equation> _code;
  list<Variable> _variables;
  ContReduction _reduction;
  std::map<ContReduction, std::string> _oper_names;
  std::string _oper;

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* CONVERT_CONT_RED_H_ */
