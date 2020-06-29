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

#include "expression.h"

#include <sstream>

#include "../ast/expression.h"
#include "../parser/parse.h"
#include "../util/visitors/expression_printer.h"
#include "../util/visitors/is_constant_index.h"
#include "../util/util.h"
#include "../util/error.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

/* MMO_Expression class. */
Expression::Expression(AST_Expression exp, const VarSymbolTable& symbols, int order) : _exp(exp), _symbols(symbols), _order(order) {}

Expression::Expression() : _exp(nullptr), _symbols(), _order(0) {}

string Expression::print() const
{
  stringstream buffer, exp;
  if (!isEmpty()) {
    ExpressionPrinter printer(_symbols, ModelConfig::instance().isQss(), _order);
    exp << printer.apply(_exp);
    buffer << exp.str();
    return buffer.str();
  }
  return "";
}

bool Expression::isScalar() const
{
  if (isReference()) {
    Option<Variable> var = reference();
    if (var->isArray()) {
      IsConstantIndex constant_index;
      return constant_index.apply(_exp);
    }
    return true;
  }
  return false;
}

bool Expression::isReference() const
{
  if (isEmpty()) {
    return false;
  }
  return _exp->expressionType() == EXPCOMPREF;
}

Option<Variable> Expression::reference() const
{
  assert(isReference());
  AST_Expression_ComponentReference cr = _exp->getAsComponentReference();
  return _symbols[cr->name()];
}

string Expression::usage() const
{
  stringstream buffer;
  if (isReference()) {
    AST_Expression_ComponentReference cr = _exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      int size = indexes->size(), i = 0;
      foreach (it, indexes) {
        Expression idx = Expression(current_element(it), _symbols);
        buffer << idx << (++i < size ? "," : "");
      }
    }
  }
  return buffer.str();
}

list<Expression> Expression::indexes() const
{
  list<Expression> exps;
  if (isReference()) {
    AST_Expression_ComponentReference cr = _exp->getAsComponentReference();
    if (cr->hasIndexes()) {
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      foreach (it, indexes) {
        Expression idx = Expression(current_element(it), _symbols);
        exps.push_back(idx);
      }
    }
  }
  return exps;
}

Expression Expression::generate(string var_name, vector<string> indices)
{
  stringstream code;
  code << var_name << "[";
  int i = 0, size = indices.size();
  for (string exp : indices) {
    code << exp << ((++i == size) ? "]" : ",");
  }
  AST_Expression ast_exp = parseExpression(code.str(), &i);
  assert(i == 0);
  return Expression(ast_exp, Utils::instance().symbols());
}

std::ostream& operator<<(std::ostream& out, const Expression& s)
{
  out << s.print();
  return out;
}

}  // namespace IR
}  // namespace MicroModelica
