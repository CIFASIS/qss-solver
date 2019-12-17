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

#include "eval_init_exp.h"

#include "../error.h"
#include "../symbol_table.h"

namespace MicroModelica {
namespace Util {

EvalInitExp::EvalInitExp(VarSymbolTable symbols) : _symbols(symbols) {}

int EvalInitExp::foldTraverseElement(AST_Expression exp)
{
  int ret = 0;
  switch (exp->expressionType()) {
  case EXPCOMPREF: {
    AST_Expression_ComponentReference cr = exp->getAsComponentReference();
    Option<Variable> var = _symbols[cr->name()];
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "eval_init_exp.cpp:38 %s", cr->name().c_str());
      return ret;
    }
    if (!var->isConstant()) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_INIT_EXP, ER_Error, "Only constants allowed inside initial expressions. %s",
                            cr->name().c_str());
      return ret;
    }
    return var->value();
  }
  case EXPBOOLEAN: {
    AST_Expression_Boolean eb = exp->getAsBoolean();
    if (eb->value()) {
      return 1;
    } else {
      return 0;
    }
  }
  case EXPBOOLEANNOT: {
    AST_Expression_BooleanNot ebn = exp->getAsBooleanNot();
    int res = apply(ebn->exp());
    if (res == 0) {
      return 1;
    } else {
      return 0;
    }
  }
  case EXPREAL:
    Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Implicit conversion from Real to Integer, in initial expression.");
    return exp->getAsReal()->val();
  case EXPINTEGER:
    return exp->getAsInteger()->val();
  default:
    Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression not recognized, returning zero as default value.");
    break;
  }
  return ret;
}

int EvalInitExp::foldTraverseElementUMinus(AST_Expression exp) { return -1 * apply(exp->getAsUMinus()->exp()); }

int EvalInitExp::foldTraverseElement(int l, int r, BinOpType bot)
{
  switch (bot) {
  case BINOPOR:
    return ((l != 0 || r != 0) ? 1 : 0);
  case BINOPAND:
    return ((l != 0 && r != 0) ? 1 : 0);
  case BINOPLOWER:
    return (l < r ? 1 : 0);
  case BINOPLOWEREQ:
    return (l <= r ? 1 : 0);
  case BINOPGREATER:
    return (l > r ? 1 : 0);
  case BINOPGREATEREQ:
    return (l >= r ? 1 : 0);
  case BINOPCOMPNE:
    return (l != r ? 1 : 0);
  case BINOPCOMPEQ:
    return (l == r ? 1 : 0);
  case BINOPADD:
    return l + r;
  case BINOPSUB:
    return l - r;
  case BINOPDIV:
    if (r != 0) {
      return l / r;
    } else {
      Error::instance().add(0, EM_IR | EM_INIT_EXP, ER_Warning, "Initial expression zero divarsion, returning zero as default value.");
    }
    break;
  case BINOPMULT:
    return l * r;
  default:
    break;
  }
  return 0;
}
}  // namespace Util
}  // namespace MicroModelica
