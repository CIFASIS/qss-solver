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

#include "alg_usage.h"

#include "../util/visitors/apply_alg_usage.h"
#include "../util/visitors/partial_eval_exp.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "expression.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

VariableUsage::VariableUsage(Expression lhs, Expression rhs, Index usage)
    : _lhs(lhs), _rhs(rhs), _replaced(), _usage(usage), _range(), _usage_map()
{
  generateMapFromUsage();
}

VariableUsage::VariableUsage(Expression usage, Range range) : _lhs(usage), _rhs(), _replaced(), _usage(), _range(range), _usage_map()
{
  generateMapFromRange();
}

VariableUsage::VariableUsage(Expression exp, Index usage) : _lhs(), _rhs(exp), _replaced(), _usage(usage), _range(), _usage_map()
{
  generateMapFromLHS();
}

Expression VariableUsage::lhs() const { return _usage.expression(); }

Expression VariableUsage::rhs() const { return _rhs; }

Expression VariableUsage::usage() const { return _replaced; }

void VariableUsage::generateMapFromRange()
{
  Index lhs_index(_lhs);
  RangeDefinitionTable ranges = _range.definition();
  // The second condition could come from a merdeg matrix.
  if (lhs_index.isConstant() || ranges.size() != lhs_index.dimension()) {
    _replaced = _lhs;
    return;
  }
  RangeDefinitionTable::iterator it;
  int i = 0;
  for (RangeDefinition def = ranges.begin(it); !ranges.end(it); def = ranges.next(it), i++) {
    const bool LHS_VARIABLE = lhs_index.hasVariable(i);
    const bool SCALAR_USAGE = def.size() == 1;
    if (LHS_VARIABLE && SCALAR_USAGE) {
      int new_constant = (def.begin() - lhs_index.constant(i)) / lhs_index.factor(i);
      _usage_map[lhs_index.variable(i)] = newAST_Expression_Integer(new_constant);
      ;
    }
  }
  ApplyVariableUsage var_usage(_usage_map);
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  _replaced = Expression(var_usage.apply(_lhs.expression()));
}

void VariableUsage::generateMapFromUsage()
{
  Index lhs_index(_lhs);
  if (lhs_index.isConstant()) {
    return;
  }
  assert(lhs_index.dimension() == _usage.dimension());
  int dimension = lhs_index.dimension();
  for (int i = 0; i < dimension; i++) {
    const bool LHS_VARIABLE = lhs_index.hasVariable(i);
    const bool RHS_VARIABLE = _usage.hasVariable(i);
    if (LHS_VARIABLE && RHS_VARIABLE) {
      int new_constant = (_usage.constant(i) - lhs_index.constant(i)) / lhs_index.factor(i);
      int new_factor = _usage.factor(i) / lhs_index.factor(i);
      AST_Expression idx = newAST_Expression_ComponentReferenceExp(newAST_String(_usage.variable(i)));
      AST_Expression cte = newAST_Expression_Integer(abs(new_constant));
      AST_Expression factor = newAST_Expression_Integer(new_factor);
      AST_Expression left = newAST_Expression_BinOp(factor, idx, BINOPMULT);
      _usage_map[lhs_index.variable(i)] = newAST_Expression_BinOp(left, cte, ((new_constant >= 0) ? BINOPADD : BINOPSUB));
    } else if (LHS_VARIABLE && !RHS_VARIABLE) {
      int new_constant = (_usage.constant(i) - lhs_index.constant(i)) / lhs_index.factor(i);
      _usage_map[lhs_index.variable(i)] = newAST_Expression_Integer(new_constant);
    }
  }
  ApplyVariableUsage var_usage(_usage_map);
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  PartialEvalExp partial_eval(symbols);
  _rhs = Expression(partial_eval.apply(var_usage.apply(_rhs.expression())));
}

void VariableUsage::generateMapFromLHS()
{
  int dimension = _usage.dimension();
  for (int i = 0; i < dimension; i++) {
    const bool REPLACE_VARIABLE = _usage.hasVariable(i);
    if (REPLACE_VARIABLE) {
      int new_constant = _usage.constant(i);
      int new_factor = _usage.factor(i);
      AST_Expression idx = newAST_Expression_ComponentReferenceExp(newAST_String(_usage.variable(i)));
      AST_Expression cte = newAST_Expression_Integer(abs(new_constant));
      AST_Expression factor = newAST_Expression_Integer(new_factor);
      AST_Expression left = newAST_Expression_BinOp(factor, idx, BINOPMULT);
      _usage_map[_usage.variable(i)] = newAST_Expression_BinOp(left, cte, ((new_constant >= 0) ? BINOPADD : BINOPSUB));
    }
  }
  ApplyVariableUsage var_usage(_usage_map);
  VarSymbolTable symbols = ModelConfig::instance().symbols();
  PartialEvalExp partial_eval(symbols);
  _replaced = Expression(partial_eval.apply(var_usage.apply(_rhs.expression())));
}

}  // namespace IR
}  // namespace MicroModelica
