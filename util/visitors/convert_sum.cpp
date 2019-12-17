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

#include "convert_sum.h"

#include <sstream>

#include "../error.h"
#include "../util.h"
#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ConvertSum::ConvertSum(VarSymbolTable &symbols) : _symbols(symbols), _has_sum(false), _code() {}

bool ConvertSum::hasSum() { return _has_sum; }

list<AST_Equation> ConvertSum::code() { return _code; }

list<Variable> ConvertSum::variables() { return _variables; }

AST_Expression ConvertSum::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCALL: {
    AST_Expression_Call ec = exp->getAsCall();
    string name = *(ec->name());
    if (!name.compare("sum")) {
      if (ec->arguments()->size() > 1) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_ARGUMENTS, ER_Error, "Wrong number of sum function arguments");
      }
      // Get reduction function argument.
      AST_Expression arg = AST_ListFirst(ec->arguments());
      if (arg->expressionType() != EXPCOMPREF) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_ARGUMENTS, ER_Error, "Expect variable for sum function");
      }
      AST_Expression_ComponentReference cr = arg->getAsComponentReference();
      cout << "LOOKING FOR: " << cr->name() << endl;
      Option<Variable> variable = _symbols[cr->name()];
      if (!variable) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "convert_sum.cpp:61 %s", cr->name());
      }
      stringstream code;
      int res = 0;
      code << "_sum_" << Utils::instance().id();
      string var_name = code.str();
      code.str("");
      code << var_name << "[1] = " << cr->name() << "[1]";
      cout << code.str() << endl;
      AST_Equation eq_1 = parseEquation(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_EQ_DEF, ER_Error, "Generating sum reduction function code.");
      }
      code.str("");
      _code.push_back(eq_1);
      // @TODO check dimensions for variables.
      code << "for i in 2:" << variable->size() << " loop " << var_name << "[i] = " << var_name << "[i-1] + " << cr->name() << "[i]; "
           << "end for";
      AST_Equation eq_i = parseEquation(code.str(), &res);
      cout << code.str() << endl;
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_EQ_DEF, ER_Error, "Generating sum reduction function code.");
      }
      code.str("");
      _code.push_back(eq_i);
      code << var_name << "[" << variable->size() << "]";
      AST_Expression replace = parseExpression(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error, "Generating sum reduction function code.");
      }
      vector<int> s;
      s.push_back(variable->size());
      TypePrefix eq_type = TP_EQ;
      Variable vi(newType_Real(), eq_type, nullptr, nullptr, s, true);
      vi.setName(var_name);
      _symbols.insert(var_name, vi);
      _variables.push_back(vi);
      _has_sum = true;
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

AST_Expression ConvertSum::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ConvertSum::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}
}  // namespace Util
}  // namespace MicroModelica
