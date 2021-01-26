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

#include "convert_cont_red.h"

#include <sstream>

#include "../error.h"
#include "../model_config.h"
#include "../util.h"
#include "../../parser/parse.h"
#include "../../ast/ast_types.h"
#include "../../ast/ast_builder.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ConvertContRed::ConvertContRed() : _has_reduction(false), _code(), _oper_names(), _oper()
{
  setOpers();
}

void ConvertContRed::setOpers()
{
  _oper_names[ContReduction::SUM] = "sum";
  _oper_names[ContReduction::PROD] = "prod";
}

bool ConvertContRed::hasReduction() { return _has_reduction; }

list<AST_Equation> ConvertContRed::code() { return _code; }

list<Variable> ConvertContRed::variables() { return _variables; }

int ConvertContRed::operators() const { return ContReduction::COUNT; }

void ConvertContRed::setLHS(Variable lhs) { return; }

void ConvertContRed::setReduction(int red_operator)
{
  assert(red_operator < ContReduction::COUNT);
  _reduction = static_cast<ContReduction>(red_operator);
  _code.clear();
  _variables.clear();
  _has_reduction = false;
  string opers[] = {"+", "*"};
  _oper = opers[_reduction];
}

AST_Expression ConvertContRed::foldTraverseElement(AST_Expression exp)
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
      Option<Variable> variable = ModelConfig::instance().lookup(cr->name());
      if (!variable) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "convert_cont_red.cpp:61 %s", cr->name());
      }
      stringstream code;
      int res = 0;
      code << "_" << oper_name << "_" << Utils::instance().id();
      string var_name = code.str();
      code.str("");
      code << var_name << "[1] = " << cr->name() << "[1]";
      AST_Equation eq_1 = parseEquation(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_EQ_DEF, ER_Error, "Generating %s reduction function code.", oper_name);
      }
      code.str("");
      _code.push_back(eq_1);
      // @TODO check dimensions for variables.
      code << "for i in 2:" << variable->size() << " loop " << var_name << "[i] = " << var_name << "[i-1] " << _oper << " " << cr->name()
           << "[i]; "
           << "end for";
      AST_Equation eq_i = parseEquation(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_EQ_DEF, ER_Error, "Generating %s reduction function code.", oper_name);
      }
      code.str("");
      _code.push_back(eq_i);
      code << var_name << "[" << variable->size() << "]";
      AST_Expression replace = parseExpression(code.str(), &res);
      if (res) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_WRONG_EXP, ER_Error, "Generating %s reduction function code.", oper_name);
      }
      vector<int> size;
      size.push_back(variable->size());
      TypePrefix eq_type = TP_INPUT;
      Variable aux_var(newType_Real(), eq_type, nullptr, nullptr, size, true);
      aux_var.setName(var_name);
      aux_var.setRealType(Variable::RealType::Algebraic);
      ModelConfig::instance().addVariable(var_name, aux_var);
      _variables.push_back(aux_var);
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

AST_Expression ConvertContRed::foldTraverseElementUMinus(AST_Expression exp)
{
  return newAST_Expression_UnaryMinus(apply(exp->getAsUMinus()->exp()));
}

AST_Expression ConvertContRed::foldTraverseElement(AST_Expression l, AST_Expression r, BinOpType bot)
{
  return newAST_Expression_BinOp(l, r, bot);
}
}  // namespace Util
}  // namespace MicroModelica
