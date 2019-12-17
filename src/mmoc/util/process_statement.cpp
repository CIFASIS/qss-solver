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

#include "../ast/ast_builder.h"
#include "../ir/helpers.h"
#include "error.h"
#include "util.h"
#include "symbol_table.h"

#include "process_statement.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

AST_Statement processStatement(AST_Statement stm)
{
  switch (stm->statementType()) {
  case STIF: {
    // New statement structs.
    AST_StatementList new_if_stms = newAST_StatementList();
    AST_StatementList new_else_stms = newAST_StatementList();
    AST_Statement_ElseList new_stm_else_list = newAST_Statement_ElseList();

    AST_Statement_If sti = stm->getAsIf();
    AST_StatementList stl = sti->statements();
    AST_StatementListIterator stlit;
    foreach (stlit, stl) {
      AST_ListAppend(new_if_stms, processStatement(current_element(stlit)));
    }
    AST_Statement_ElseList stelsel = sti->else_if();
    AST_Statement_ElseListIterator stelselit;
    foreach (stelselit, stelsel) {
      stl = current_element(stelselit)->statements();
      AST_StatementList else_stms = newAST_StatementList();
      foreach (stlit, stl) {
        AST_ListAppend(else_stms, processStatement(current_element(stlit)));
      }
      AST_Statement_Else new_else = newAST_Statement_Else(current_element(stelselit)->condition(), else_stms);
      AST_ListAppend(new_stm_else_list, new_else);
    }
    stl = sti->else_statements();
    if (!stl->empty()) {
      foreach (stlit, stl) {
        AST_ListAppend(new_else_stms, processStatement(current_element(stlit)));
      }
    }
    return newAST_Statement_If(sti->condition(), new_if_stms, new_stm_else_list, new_else_stms);
  }
  case STASSING: {
    AST_Statement_Assign sa = stm->getAsAssign();
    AST_Expression_ComponentReference cr = sa->lhs();
    string name = cr->name();
    if (sa->exp()->expressionType() == EXPCALLARG) {
      if (Utils::instance().checkBuiltInFunctions("reinit")) {
        AST_Expression_CallArgs eca = sa->exp()->getAsCallArgs();
        if (eca->arguments()->size() > 2) {
          Error::instance().add(eca->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error, "Expected 2 arguments, found %d", eca->arguments()->size());
        }
        AST_Expression e = AST_ListFirst(eca->arguments());
        AST_Expression_ComponentReference lhs = nullptr;
        AST_Expression rhs = nullptr;
        if (e->expressionType() != EXPCOMPREF) {
          Error::instance().add(e->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error,
                                "Only variables references allowed, expressions not allowed.");
        } else {
          lhs = e->getAsComponentReference();
          VarSymbolTable symbols = Utils::instance().symbols();
          string sname = lhs->name();
          Option<Variable> var = symbols[sname];
          if (!var) {
            Error::instance().add(lhs->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "process_statement.cpp:88 %s", sname.c_str());
          }
          if (!var->isState()) {
            Error::instance().add(lhs->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error, "Only state variables allowed.");
          } else {
            AST_ExpressionListIterator eli;
            AST_ExpressionList el = eca->arguments();
            int c = 0;
            foreach (eli, el) {
              if (c == 0) {
                c++;
                continue;
              }
              rhs = current_element(eli);
            }
          }
          return newAST_Statement_Assign(lhs, rhs);
        }
      }
    }
    return stm;
  }
  case STFOR: {
    AST_Statement_For stf = stm->getAsFor();
    AST_StatementList stms = stf->statements();
    AST_StatementListIterator stmit;
    AST_StatementList new_for_stms = newAST_StatementList();
    foreach (stmit, stms) {
      AST_ListAppend(new_for_stms, processStatement(current_element(stmit)));
    }
    return newAST_Statement_For(stf->forIndexList(), new_for_stms);
  }
  default:
    break;
  }
  assert(false);
  return nullptr;
}

}  // namespace Util
}  // namespace MicroModelica