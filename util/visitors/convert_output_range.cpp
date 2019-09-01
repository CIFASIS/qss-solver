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

#include "convert_output_range.h"

#include <sstream>

#include "../util.h"
#include "../error.h"
#include "eval_init_exp.h"

namespace MicroModelica {
using namespace IR;
using namespace Deps;
namespace Util {

AST_Expression ConvertOutputRange::foldTraverseElement(AST_Expression exp)
{
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    _var = cr->name();
    Option<Variable> var = _symbols[_var];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "Convert output expression: %s", cr->name().c_str());
    }
    if (cr->hasIndexes()) {
      AST_ExpressionList indexes = cr->firstIndex();
      AST_ExpressionListIterator it;
      AST_Expression_ComponentReference ret = newAST_Expression_ComponentReference();
      AST_ExpressionList new_indexes = newAST_ExpressionList();
      foreach (it, indexes) {
        string index = Utils::instance().iteratorVar(_dim);
        Variable vi(newType_Integer(), TP_FOR, NULL, NULL, vector<int>(1, 1), false);
        _symbols.insert(index, vi);
        AST_ListAppend(new_indexes, apply(current_element(it)));
        _dim++;
      }
      ret->append(newAST_String(cr->name()), new_indexes);
      return ret;
    }
    return exp;
  }
  case EXPRANGE: {
    AST_ExpressionList range_exps = exp->getAsRange()->expressionList();
    AST_ExpressionListIterator it;
    int count = 0;
    int range[3];
    EvalInitExp eval_init_exp = EvalInitExp(_symbols);
    foreach (it, range_exps) {
      range[count++] = eval_init_exp.apply(current_element(it));
    }
    AST_Expression lhs = nullptr;
    AST_Expression rhs = nullptr;
    string it_var = Utils::instance().iteratorVar(_dim);
    AST_Expression_ComponentReference idx = newAST_Expression_ComponentReference();
    AST_Expression_ComponentReference_Add(idx, newAST_String(it_var), newAST_ExpressionList());
    if (count == 2) {
      _intervals.push_back(Interval::closed(1, range[1] - range[0] + 1));
      int cte = range[0] - 1;
      lhs = newAST_Expression_Integer(cte);
      rhs = idx;
    } else {
      if (range[1] > 0) {
        _intervals.push_back(Interval::closed(1, ((range[2] - range[0]) / range[1]) + 1));
        int cte = range[0] - range[1];
        lhs = newAST_Expression_Integer(cte);
        rhs = newAST_Expression_BinOp(newAST_Expression_Integer(range[1]), idx, BINOPMULT);
      } else {
        Error::instance().add(exp->lineNum(), EM_IR | EM_FOR_DEF, ER_Fatal, "Range interval not valid");
      }
    }
    return newAST_Expression_BinOp(lhs, rhs, BINOPADD);
  }
  case EXPCOLON: {
    Option<Variable> var = _symbols[_var];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Fatal, "Convert output expression: %s", _var.c_str());
    }
    _intervals.push_back(Interval::closed(1, var->size(_dim)));
    string it_var = Utils::instance().iteratorVar(_dim);
    AST_Expression_ComponentReference idx = newAST_Expression_ComponentReference();
    AST_Expression_ComponentReference_Add(idx, newAST_String(it_var), newAST_ExpressionList());
    return idx;
  }
  default:
    break;
  }
  return exp;
}

Option<Range> ConvertOutputRange::range()
{
  if (_intervals.empty()) {
    return Option<Range>();
  }
  _range.generate(MDI(_intervals));
  return _range;
}

}  // namespace Util
}  // namespace MicroModelica
