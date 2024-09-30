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

#ifndef CONVERT_DISC_RED_H_
#define CONVERT_DISC_RED_H_

#include "../ast_util.h"

namespace MicroModelica {
namespace Util {
class ConvertDiscRed : public AST_Expression_Visitor<AST_Expression> {
  public:
  typedef enum : int { MAX = 0, MIN, COUNT } DiscReduction;

  ConvertDiscRed();
  ~ConvertDiscRed() = default;

  void setReduction(int red_operator);

  bool hasReduction();

  list<AST_Statement> code();

  list<Variable> variables();

  int operators() const;

  void setLHS(Variable lhs);

  protected:
  std::string operatorTerm(std::string);

  private:
  AST_Expression foldTraverseElement(AST_Expression exp);
  AST_Expression foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot);
  AST_Expression foldTraverseElementUMinus(AST_Expression exp);

  bool _has_reduction;
  list<AST_Statement> _code;
  list<Variable> _variables;
  DiscReduction _reduction;
  std::map<DiscReduction, std::string> _oper_names;
  Variable _lhs;
  std::string _oper;
};
}  // namespace Util
}  // namespace MicroModelica
#endif /* CONVERT_DISC_RED_H_ */