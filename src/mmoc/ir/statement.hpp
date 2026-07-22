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

#include <ast/ast_types.hpp>
#include <ir/index.hpp>
#include <util/table.hpp>

namespace MicroModelica {

namespace Util {
typedef ModelTable<std::string, std::string> SymbolTable;
}
namespace IR {

namespace STATEMENT {
typedef enum { LHS, RHS, LHS_DISCRETES, LHS_STATES } AssignTerm;
}

class Statement {
  public:
  Statement(AST_Statement stm, bool initial = false, const std::string& block = "");
  Statement(AST_Statement stm, Option<Range> range, bool initial = false, const std::string& block = "");
  Statement(AST_Statement stm, bool replace_algs, Option<Range> range);
  Statement();
  ~Statement() = default;

  inline bool hasRange() { return _range.is_initialized(); };
  inline Util::SymbolTable calledFunctions() { return _calledFunctions; };
  friend std::ostream& operator<<(std::ostream& out, const Statement& s);
  string print() const;
  /**
   * @brief      Returns a expression list containing all the expressions needed
   *             by the graph builders. Without the if/for/when sentences.
   *
   * @return     Expression list used by the graph builders.\see {dh_graph_builder.cpp}
   */
  ExpressionList assignments(STATEMENT::AssignTerm asg) const;
  ExpressionList lhsDiscretes() const { return _lhs_discretes; };
  ExpressionList lhsStates() const { return _lhs_states; };
  bool isAssignment() const;
  bool isForStatement() const;
  inline Option<Range> range() { return _range; };
  bool autonomous() const;
  AST_Statement statement();

  protected:
  void initialize();
  ExpressionList generateExps(STATEMENT::AssignTerm asg);
  Expression emptyRef();
  bool checkStateAssignment(Expression exp) const;
  std::string printAssignment(AST_Statement_Assign asg) const;
  void setRange();
  Expression generateExpression(AST_Expression stm) const;

  private:
  AST_Statement _stm;
  Option<Range> _range;
  Util::SymbolTable _calledFunctions;
  std::string _block;
  ExpressionList _lhs_assignments;
  ExpressionList _rhs_assignments;
  ExpressionList _lhs_discretes;
  ExpressionList _lhs_states;
  bool _replace_algs;
};

typedef ModelTable<int, Statement> StatementTable;
}  // namespace IR
}  // namespace MicroModelica
