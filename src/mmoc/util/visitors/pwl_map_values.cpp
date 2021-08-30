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

#include "pwl_map_values.h"

#include <util/error.h>
#include <util/model_config.h>
#include <util/symbol_table.h>
#include <util/visitors/eval_init_exp.h>
#include <util/visitors/is_constant_expression.h>
#include <util/visitors/partial_eval_exp.h>

namespace MicroModelica {
namespace Util {

PWLMapValues::PWLMapValues() : _constant(0), _slope(0), _variable() {}

AST_Expression PWLMapValues::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = ModelConfig::instance().lookup(cr->name());
    if (var && var->isConstant()) {
      _constant = var->value();
      _slope = 0;
    } else if (var && var->isForType()) {
      _variable = var->name();
      _slope = 1;
    }
    break;
  }
  case EXPINTEGER:
    _constant = exp->getAsInteger()->val();
    _slope = 0;
    break;
  default:
    return exp;
  }
  return exp;
}

AST_Expression PWLMapValues::foldTraverseElement(AST_Expression left, AST_Expression right, BinOpType type)
{ 
  EvalInitExp eval_exp;
  static const bool EVAL_INT = true;
  IsConstantExpression constant_exp(EVAL_INT);
  bool cte_left = constant_exp.apply(left);
  bool cte_right = constant_exp.apply(right);
  if (!cte_left && !cte_right) {
    Error::instance().add(left->lineNum(), EM_IR | EM_UNKNOWN_EXP, ER_Error, "pwl_map_values.cpp:58");
    return left;
  }
  if (type == BinOpType::BINOPMULT) {
    if (cte_left && cte_right) {
      _constant = eval_exp.apply(left) * eval_exp.apply(right);
    } else if (cte_left) {
      _slope = eval_exp.apply(left);
    } else {
      _slope = eval_exp.apply(right);
    }
  }
  if (type == BinOpType::BINOPADD) {
    assign(left, right, cte_left, cte_right, 1);
  }
  if (type == BinOpType::BINOPSUB) {
    assign(left, right, cte_left, cte_right, -1);
  }
  return newAST_Expression_BinOp(left, right, type);
}

AST_Expression PWLMapValues::foldTraverseElementUMinus(AST_Expression exp)
{
  AST_Expression ret = apply(exp->getAsUMinus()->exp());
  PartialEvalExp eval;
  if (eval.isIntegerOrConstant(ret)) {
    _constant *= -1;
  } else {
    _slope *= -1;
  }
  return exp;
};

void PWLMapValues::assign(AST_Expression left, AST_Expression right, bool cte_left, bool cte_right, int sign)
{
  EvalInitExp eval_exp;
  if (cte_left && cte_right) {
    _constant = sign * (eval_exp.apply(left) +  eval_exp.apply(right));
  } else if (cte_right) {
    _constant = sign * eval_exp.apply(right);
  } else {
    _constant = sign * eval_exp.apply(left);
  }
}

int PWLMapValues::constant() const  { return _constant; }

int PWLMapValues::slope() const  { return _slope; }

std::string PWLMapValues::variable() const  { return _variable; }

bool PWLMapValues::isScalar() const { return _slope == 0;}

}  // namespace Util
}  // namespace MicroModelica
