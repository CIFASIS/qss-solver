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

#include "convert_disc_red.h"

#include <sstream>

#include "../error.h"
#include "../util.h"
#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ConvertDiscRed::ConvertDiscRed(VarSymbolTable &symbols) : _symbols(symbols), _has_reduction(false), _code(), _oper_names(), _lhs(), _oper()
{
  _oper_names[DiscReduction::MAX] = "max";
  _oper_names[DiscReduction::MIN] = "min";
}

bool ConvertDiscRed::hasReduction() { return _has_reduction; }

list<AST_Statement> ConvertDiscRed::code() { return _code; }

list<Variable> ConvertDiscRed::variables() { return _variables; }

int ConvertDiscRed::operators() const { return DiscReduction::COUNT; }

void ConvertDiscRed::setLHS(Variable lhs) { _lhs = lhs; }

void ConvertDiscRed::setReduction(int red_operator)
{
  assert(red_operator < DiscReduction::COUNT);
  _reduction = static_cast<DiscReduction>(red_operator);
  _code.clear();
  _variables.clear();
  _has_reduction = false;
  switch (_reduction) {
  case DiscReduction::MAX:
    _oper = ">";
    break;
  case DiscReduction::MIN:
    _oper = "<";
    break;
  default:
    break;
  }
}

AST_Expression ConvertDiscRed::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCALL: {
    AST_Expression_Call ec = exp->getAsCall();
    string name = *(ec->name());
    string oper_name = _oper_names[_reduction];
    if (!name.compare(oper_name)) {
      if (ec->arguments()->size() > 1) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_ARGUMENTS, ER_Error, "Wrong number of %s function arguments", oper_name);
      }
      // Get reduction function argument.
      AST_Expression arg = AST_ListFirst(ec->arguments());
      if (arg->expressionType() != EXPCOMPREF) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_ARGUMENTS, ER_Error, "Expect variable for %s function", oper_name);
      }
      AST_Expression_ComponentReference cr = arg->getAsComponentReference();
      Option<Variable> variable = _symbols[cr->name()];
      if (!variable) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "convert_cont_red.cpp:61 %s", cr->name());
      }
      stringstream code;
      int res = 0;
      code << cr->name() << "[1]";
      AST_Expression replace = parseExpression(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error, "Generating %s reduction function code.", oper_name);
      }
      // @TODO check dimensions for variables.
      code << "for i in 2:" << variable->size() << " loop "
           << "if " << cr->name() << "[i] " << _oper << _lhs << "then " << _lhs << " := " << cr->name() << "[i]; "
           << "end if";
      AST_Statement stm = parseStatement(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_EQ_DEF, ER_Error, "Generating %s reduction function code.", oper_name);
      }
      code.str("");
      _code.push_back(stm);
      _has_reduction = true;
      return replace;
    }
  } break;
  case EXPOUTPUT: {
    AST_Expression_Output out = exp->getAsOutput();
    return newAST_Expression_OutputExpressions(newAST_ExpressionList(apply(AST_ListFirst(out->expressionList()))));
  }
  default:
    break;
  }
  return exp;
}

AST_Expression ConvertDiscRed::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ConvertDiscRed::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}
}  // namespace Util
}  // namespace MicroModelica
