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
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/process_statement.h"
#include "../util/visitors/called_functions.h"
#include "helpers.h"
#include "statement.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

Statement::Statement(AST_Statement stm, Option<Range> range, bool initial, const string& block)
    : _stm(stm), _range(range), _block(block), _lhs_assignments(), _rhs_assignments(), _lhs_discretes(), _lhs_states()
{
  initialize();
}

Statement::Statement(AST_Statement stm, bool initial, const string& block)
    : _stm(stm), _range(), _block(block), _lhs_assignments(), _rhs_assignments(), _lhs_discretes(), _lhs_states()
{
  initialize();
}

void Statement::initialize()
{
  StatementCalledFunctions cf;
  setRange();
  _calledFunctions = cf.apply(_stm);
  _lhs_assignments = generateExps(STATEMENT::LHS);
  _rhs_assignments = generateExps(STATEMENT::RHS);
  _lhs_discretes = generateExps(STATEMENT::LHS_DISCRETES);
  _lhs_states = generateExps(STATEMENT::LHS_STATES);
}

void Statement::setRange()
{
  // In case of for statements, the statement can have a range even if
  // defined in  a single event, so change the range accordingly.
  if (_stm->statementType() == STFOR) {
    AST_Statement_For for_stm = _stm->getAsFor();
    _range = Range(for_stm);
  }
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
      asgs.push_back(Expression(sti->condition()));
    } else if (asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
      asgs.push_back(emptyRef());
    }
    foreach (stlit, stl) {
      Statement st(current_element(stlit));
      asgs.splice(asgs.end(), st.generateExps(asg));
    }
    AST_Statement_ElseList stelsel = sti->else_if();
    AST_Statement_ElseListIterator stelselit;
    foreach (stelselit, stelsel) {
      if (asg == STATEMENT::RHS) {
        asgs.push_back(Expression(current_element(stelselit)->condition()));
      } else if (asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
        asgs.push_back(emptyRef());
      }
      stl = current_element(stelselit)->statements();
      foreach (stlit, stl) {
        Statement st(current_element(stlit));
        asgs.splice(asgs.end(), st.generateExps(asg));
      }
    }
    stl = sti->else_statements();
    if (!stl->empty()) {
      foreach (stlit, stl) {
        Statement st(current_element(stlit));
        asgs.splice(asgs.end(), st.generateExps(asg));
      }
    }
    break;
  }
  case STASSIGN: {
    if (asg == STATEMENT::LHS || asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
      asgs.push_back(Expression(_stm->getAsAssign()->lhs()));
    } else {
      asgs.push_back(Expression(_stm->getAsAssign()->exp()));
    }
    break;
  }
  case STFOR: {
    AST_Statement_For stf = _stm->getAsFor();
    AST_StatementList stms = stf->statements();
    AST_StatementListIterator stmit;
    Range range(stf);
    foreach (stmit, stms) {
      Statement st(current_element(stmit), range);
      asgs.splice(asgs.end(), st.generateExps(asg));
    }
    break;
  }
  case STOUTASSING: {
    AST_Statement_OutputAssigment out_stm = _stm->getAsOutputAssigment();
    if (asg == STATEMENT::LHS || asg == STATEMENT::LHS_DISCRETES || asg == STATEMENT::LHS_STATES) {
      AST_ExpressionList exps = out_stm->out_expressions();
      AST_ExpressionListIterator exp_it;
      foreach (exp_it, exps) {
        asgs.push_back(Expression(current_element(exp_it)));
      }
    } else {
      AST_Expression call_exp = newAST_Expression_Call(newAST_String(out_stm->function()->cname()), nullptr, out_stm->arguments());
      asgs.push_back(Expression(call_exp));
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

bool Statement::isForStatement() const { return _stm->statementType() == STFOR; }

string Statement::printAssignment(AST_Statement_Assign asg) const
{
  stringstream code;
  switch (asg->exp()->expressionType()) {
  case EXPCALLARG:
    code << "CMD_terminate();";
    break;
  default: {
    Expression lhs(asg->lhs());
    Expression rhs(asg->exp());
    bool state_assignment = checkStateAssignment(lhs);
    if (state_assignment) {
      ModelConfig::instance().setInitialCode(true);
    }
    code << lhs << " = " << rhs << ";";
    if (state_assignment) {
      ModelConfig::instance().setInitialCode(false);
    }
    break;
  }
  }
  return code.str();
}

string Statement::print() const
{
  stringstream buffer;
  switch (_stm->statementType()) {
  case STIF: {
    AST_Statement_If sti = _stm->getAsIf();
    Expression ifcond(sti->condition());
    buffer << _block << "if(" << ifcond << ") {" << endl;
    AST_StatementList stl = sti->statements();
    AST_StatementListIterator stlit;
    foreach (stlit, stl) {
      Statement st(current_element(stlit), false, _block + TAB);
      buffer << st << endl;
    }
    buffer << _block << "}";
    AST_Statement_ElseList stelsel = sti->else_if();
    AST_Statement_ElseListIterator stelselit;
    foreach (stelselit, stelsel) {
      Expression eifcond(current_element(stelselit)->condition());
      buffer << _block << "else if(" << eifcond << ") {" << endl;
      stl = current_element(stelselit)->statements();
      foreach (stlit, stl) {
        Statement st(current_element(stlit), false, _block + TAB);
        buffer << st << endl;
      }
      buffer << _block << "}";
    }
    stl = sti->else_statements();
    if (!stl->empty()) {
      buffer << _block << "else {" << endl;
      foreach (stlit, stl) {
        Statement st(current_element(stlit), false, _block + TAB);
        buffer << st << endl;
      }
      buffer << _block << "}";
    }
    break;
  }
  case STASSIGN: {
    buffer << _block << printAssignment(_stm->getAsAssign());
    break;
  }
  case STFOR: {
    AST_Statement_For stf = _stm->getAsFor();
    Range range(stf);
    buffer << range;
    AST_StatementList stms = stf->statements();
    AST_StatementListIterator stmit;
    foreach (stmit, stms) {
      Statement st(current_element(stmit), false, range.block());
      buffer << st << endl;
    }
    buffer << range.end();
    break;
  }
  case STOUTASSING: {
    AST_Statement_OutputAssigment out_stm = _stm->getAsOutputAssigment();
    AST_Expression call_exp =
        newAST_Expression_Call(newAST_String(out_stm->function()->cname()), nullptr, out_stm->arguments(), out_stm->out_expressions());
    Expression call(call_exp);
    buffer << call << ";";
    break;
  }
  default:
    break;
  }
  return buffer.str();
}

bool Statement::isAssignment() const { return _stm->statementType() == STASSIGN; }

Expression Statement::emptyRef()
{
  AST_Expression lhs = newAST_Expression_ComponentReferenceExp(newAST_String("dummy"));
  return Expression(lhs);
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
