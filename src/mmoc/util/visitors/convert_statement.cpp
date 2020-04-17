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

#include "convert_statement.h"

#include <sstream>

#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"
#include "../visitors/replace_constant.h"
#include "convert_expression.h"
#include "replace_inner_product.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ConvertStatement::ConvertStatement(AST_Statement statement, VarSymbolTable& symbols) : _symbols(symbols)
{
  _statement = convert(statement);
}

AST_Statement ConvertStatement::get() { return _statement; }

AST_StatementList ConvertStatement::convert(AST_StatementList sts)
{
  AST_StatementList stt = newAST_StatementList();
  AST_StatementListIterator stit;
  foreach (stit, sts) {
    AST_ListAppend(stt, convert(current_element(stit)));
  }
  return stt;
}

AST_Statement_ElseList ConvertStatement::convert(AST_Statement_ElseList stel)
{
  AST_Statement_ElseList stelt = newAST_Statement_ElseList();
  AST_Statement_ElseListIterator stelit;
  foreach (stelit, stel) {
    AST_Statement_Else ceif = current_element(stelit);
    AST_StatementList newElseIfSts = newAST_StatementList();
    AST_StatementList elseIfSts = ceif->statements();
    AST_StatementListIterator elseIfStIt;
    foreach (elseIfStIt, elseIfSts) {
      AST_ListAppend(newElseIfSts, convert(current_element(elseIfStIt)));
    }
    AST_Statement_Else newStElse = newAST_Statement_Else(ceif->condition(), newElseIfSts);
    AST_ListAppend(stelt, newStElse);
  }
  return stelt;
}

AST_Statement ConvertStatement::convert(AST_Statement st)
{
  if (st->statementType() == STASSIGN) {
    ReplaceInnerProduct rip(_symbols);
    AST_Expression l = st->getAsAssign()->lhs();
    AST_Expression r = rip.apply(st->getAsAssign()->exp());
    ConvertExpression convert_exp(l, r, _symbols);
    l = convert_exp.left();
    assert(l->expressionType() == EXPCOMPREF);
    r = convert_exp.right();
    string transform = convert_exp.get();
    if (transform.empty()) {
      return newAST_Statement_Assign(l->getAsComponentReference(), r);
    }
    int rValue;
    return parseStatement(transform, &rValue);
  } else if (st->statementType() == STFOR) {
    AST_Statement_For stf = st->getAsFor();
    AST_ForIndexList fil = stf->forIndexList();
    AST_StatementList sts = stf->statements();
    AST_StatementList stt = convert(sts);
    AST_Statement retFor = newAST_Statement_For(fil, stt);
    return retFor;
  } else if (st->statementType() == STIF) {
    AST_Statement_If sti = st->getAsIf();
    AST_StatementList stt = convert(sti->statements());
    AST_StatementList stet = convert(sti->else_statements());
    AST_Statement_ElseList stel = sti->else_if();
    AST_Statement_ElseList stelt = convert(stel);
    AST_Statement retIf = newAST_Statement_If(sti->condition(), stt, stelt, stet);
    return retIf;
  } else if (st->statementType() == STWHEN) {
    AST_Statement_When stWhen = st->getAsWhen();
    AST_StatementList stList = convert(stWhen->statements());
    AST_Statement_ElseList stElseList = convert(stWhen->else_when());
    ReplaceConstant replace_constant(_symbols);
    AST_Expression condition = replace_constant.apply(stWhen->condition());
    AST_Statement retWhen = newAST_Statement_When(condition, stList, stElseList, stWhen->comment());
    return retWhen;
  }
  return st;
}
}  // namespace Util
}  // namespace MicroModelica
