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
#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/statement.h"
#include "../util/util.h"
#include "../util/process_statement.h"
#include "../util/visitors/called_functions.h"
#include "helpers.h"
#include "statement.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

Statement::Statement(AST_Statement stm, const VarSymbolTable& symbols, Option<Range> range, bool initial, const string& block)
    : _stm(stm), _range(range), _symbols(symbols), _block(block), _lhs_assignments(), _rhs_assignments(), _lhs_discretes(), _lhs_states()
{
  initialize();
}

Statement::Statement(AST_Statement stm, const VarSymbolTable& symbols, bool initial, const string& block)
    : _stm(stm), _range(), _symbols(symbols), _block(block), _lhs_assignments(), _rhs_assignments(), _lhs_discretes(), _lhs_states()
{
  initialize();
}

void Statement::initialize()
{
  StatementCalledFunctions cf;
  Utils::instance().setSymbols(_symbols);
  _stm = processStatement(_stm);
  _calledFunctions = cf.apply(_stm);
  _lhs_assignments = generateExps(STATEMENT::LHS);
  _rhs_assignments = generateExps(STATEMENT::RHS);
  _lhs_discretes = generateExps(STATEMENT::LHS_DISCRETES);
  _lhs_states = generateExps(STATEMENT::LHS_STATES);
}

ExpressionList Statement::generateExps(STATEMENT::AssignTerm asg)
{
  ExpressionList asgs;
  switch (_stm->statementType()) {
  case STIF: {
    AST_Statement_If sti = _stm->getAsIf();
    AST_StatementList stl = sti->statements();
    AST_StatementListIterator stlit;
    if (asg == STATEMENT::RHS) {
      asgs.push_back(Expression(sti->condition(), _symbols));
    } else if (asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
      asgs.push_back(emptyRef());
    }
    foreach (stlit, stl) {
      Statement st(current_element(stlit), _symbols);
      asgs.splice(asgs.end(), st.generateExps(asg));
    }
    AST_Statement_ElseList stelsel = sti->else_if();
    AST_Statement_ElseListIterator stelselit;
    foreach (stelselit, stelsel) {
      if (asg == STATEMENT::RHS) {
        asgs.push_back(Expression(current_element(stelselit)->condition(), _symbols));
      } else if (asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
        asgs.push_back(emptyRef());
      }
      stl = current_element(stelselit)->statements();
      foreach (stlit, stl) {
        Statement st(current_element(stlit), _symbols);
        asgs.splice(asgs.end(), st.generateExps(asg));
      }
    }
    stl = sti->else_statements();
    if (!stl->empty()) {
      foreach (stlit, stl) {
        Statement st(current_element(stlit), _symbols);
        asgs.splice(asgs.end(), st.generateExps(asg));
      }
    }
    break;
  }
  case STASSING: {
    if (asg == STATEMENT::LHS || asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
      asgs.push_back(Expression(_stm->getAsAssign()->lhs(), _symbols));
    } else {
      asgs.push_back(Expression(_stm->getAsAssign()->exp(), _symbols));
    }
    break;
  }
  case STFOR: {
    AST_Statement_For stf = _stm->getAsFor();
    AST_StatementList stms = stf->statements();
    AST_StatementListIterator stmit;
    foreach (stmit, stms) {
      Statement st(current_element(stmit), _symbols);
      asgs.splice(asgs.end(), st.generateExps(asg));
    }
    break;
  }
  default:
    break;
  }
  return asgs;
}

bool Statement::checkStateAssignment(Expression exp) const
{
  assert(exp.isReference());
  Option<Variable> var = exp.reference();
  assert(var);
  return var->isState();
}

string Statement::print() const
{
  stringstream buffer;
  switch (_stm->statementType()) {
  case STIF: {
    AST_Statement_If sti = _stm->getAsIf();
    Expression ifcond(sti->condition(), _symbols);
    buffer << _block << "if(" << ifcond << ")" << endl;
    buffer << _block << "{" << endl;
    AST_StatementList stl = sti->statements();
    AST_StatementListIterator stlit;
    foreach (stlit, stl) {
      Statement st(current_element(stlit), _symbols, false, _block + TAB);
      buffer << st << endl;
    }
    buffer << _block << "}";
    AST_Statement_ElseList stelsel = sti->else_if();
    AST_Statement_ElseListIterator stelselit;
    foreach (stelselit, stelsel) {
      Expression eifcond(current_element(stelselit)->condition(), _symbols);
      buffer << _block << "else if(" << eifcond << ")" << endl << _block << "{" << endl;
      stl = current_element(stelselit)->statements();
      foreach (stlit, stl) {
        Statement st(current_element(stlit), _symbols, false, _block + TAB);
        buffer << st << endl;
      }
      buffer << _block << "}";
    }
    stl = sti->else_statements();
    if (!stl->empty()) {
      buffer << _block << "else" << endl << _block << "{" << endl;
      foreach (stlit, stl) {
        Statement st(current_element(stlit), _symbols, false, _block + TAB);
        buffer << st << endl;
      }
      buffer << _block << "}";
    }
    break;
  }
  case STASSING: {
    AST_Statement_Assign asg = _stm->getAsAssign();
    Expression lhs(asg->lhs(), _symbols);
    Expression rhs(asg->exp(), _symbols);
    bool state_assignment = checkStateAssignment(lhs);
    if (state_assignment) {
      ModelConfig::instance().setInitialCode(true);
    }
    buffer << _block << lhs << " = " << rhs << ";";
    if (state_assignment) {
      ModelConfig::instance().setInitialCode(false);
    }
    break;
  }
  case STFOR: {
    AST_Statement_For stf = _stm->getAsFor();
    Range range(stf, _symbols);
    buffer << range;
    AST_StatementList stms = stf->statements();
    AST_StatementListIterator stmit;
    foreach (stmit, stms) {
      Statement st(current_element(stmit), _symbols, false, range.block());
      buffer << st << endl;
    }
    buffer << range.end();
    break;
  }
  default:
    break;
  }
  return buffer.str();
}

bool Statement::isAssignment() const { return _stm->statementType() == STASSING; }

Expression Statement::emptyRef()
{
  AST_Expression lhs = newAST_Expression_ComponentReferenceExp(newAST_String("dummy"));
  return Expression(lhs, _symbols);
}

ExpressionList Statement::assignments(STATEMENT::AssignTerm asg) const
{
  assert(asg != STATEMENT::LHS_DISCRETES);
  switch (asg) {
  case STATEMENT::LHS:
    return _lhs_assignments;
  case STATEMENT::RHS:
    return _rhs_assignments;
  default:
    return ExpressionList();
  }
  return ExpressionList();
}

std::ostream& operator<<(std::ostream& out, const Statement& s) { return out << s.print(); }
}  // namespace IR
}  // namespace MicroModelica
