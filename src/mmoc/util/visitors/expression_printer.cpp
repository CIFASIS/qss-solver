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

#include "expression_printer.h"

#include <sstream>

#include "../../ast/ast_builder.h"
#include "../../ir/helpers.h"
#include "../error.h"
#include "../model_config.h"
#include "../util.h"

namespace MicroModelica {
using namespace IR;
namespace Util {

ExpressionPrinter::ExpressionPrinter(const VarSymbolTable& symbols, bool is_qss, int order)
    : _symbols(symbols), _is_qss(is_qss), _order(order)
{
}

string ExpressionPrinter::foldTraverseElement(AST_Expression exp)
{
  stringstream buffer;

  switch (exp->expressionType()) {
  case EXPBOOLEAN:
    buffer << (exp->getAsBoolean()->value() ? 1 : 0);
    break;
  case EXPBOOLEANNOT:
    buffer << "!" << apply(exp->getAsBooleanNot()->exp());
    break;
  case EXPBRACE:
    break;
  case EXPCALL: {
    AST_Expression_Call call = exp->getAsCall();
    CompiledFunctionTable fs = Utils::instance().compiledFunctions();
    Option<CompiledFunction> f = fs[*call->name()];
    if (!f) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "Function definition: expression_printer.cpp:56 %s",
                            call->name()->c_str());
      break;
    }
    ModelConfig::instance().setSymbols(_symbols);
    f->setArguments(call->arguments());
    f->setOutputArguments(call->outputArguments());
    buffer << f.get();
    break;
  }
  case EXPCALLARG: {
    AST_Expression_CallArgs call = exp->getAsCallArgs();
    AST_ExpressionList el = call->arguments();
    AST_ExpressionListIterator it;
    buffer << "(";
    foreach (it, el) {
      buffer << apply(current_element(it));
    }
    buffer << ")";
    break;
  }
  case EXPCOMPREF: {
    AST_Expression_ComponentReference ref = exp->getAsComponentReference();
    Option<Variable> var = _symbols.lookup(ref->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "expression_printer.cpp:80 %s", ref->name().c_str());
      break;
    }
    VariablePrinter var_printer(var.get(), ref, _symbols, _is_qss, _order);
    buffer << var_printer;
    break;
  }
  case EXPDERIVATIVE: {
    AST_Expression_Derivative der = exp->getAsDerivative();
    int order = _order;
    _order = 1;
    AST_Expression args = AST_ListFirst(der->arguments());
    while (args->expressionType() == EXPDERIVATIVE) {
      _order++;
      args = AST_ListFirst(args->getAsDerivative()->arguments());
    }
    buffer << apply(args);
    _order = order;
    break;
  }
  case EXPINTEGER:
    buffer << exp->getAsInteger()->val();
    break;
  case EXPOUTPUT: {
    AST_Expression_Output out = exp->getAsOutput();
    AST_ExpressionListIterator it;
    int size = out->expressionList()->size(), i = 0;
    buffer << "(";
    foreach (it, out->expressionList()) {
      buffer << apply(current_element(it));
      buffer << (++i == size ? "" : ",");
    }
    buffer << ")";
    break;
  }
  case EXPREAL:
    buffer << exp->getAsReal()->val();
    break;
  case EXPSTRING:
    buffer << exp->getAsString()->str();
    break;
  default:
    return "";
  }
  return buffer.str();
}

string ExpressionPrinter::foldTraverseElement(string l, string r, BinOpType bot)
{
  stringstream buffer;
  switch (bot) {
  case BINOPOR:
    buffer << l << "||" << r;
    break;
  case BINOPAND:
    buffer << l << "&&" << r;
    break;
  case BINOPLOWER:
    buffer << l << "<" << r;
    break;
  case BINOPLOWEREQ:
    buffer << l << "<=" << r;
    break;
  case BINOPGREATER:
    buffer << l << ">" << r;
    break;
  case BINOPGREATEREQ:
    buffer << l << ">=" << r;
    break;
  case BINOPCOMPNE:
    buffer << l << "!=" << r;
    break;
  case BINOPCOMPEQ:
    buffer << l << "==" << r;
    break;
  case BINOPADD:
    buffer << l << "+" << r;
    break;
  case BINOPSUB:
    buffer << l << "-" << r;
    break;
  case BINOPDIV:
    buffer << l << "/" << r;
    break;
  case BINOPMULT:
    buffer << l << "*" << r;
    break;
  case BINOPEXP:
    buffer << "pow(" << l << "," << r << ")";
    break;
  default:
    return "NOT IMPLEMENTED";
  }
  return buffer.str();
}

string ExpressionPrinter::foldTraverseElementUMinus(AST_Expression exp)
{
  stringstream buffer;
  buffer << "-" << apply(exp->getAsUMinus()->exp());
  return buffer.str();
}

VariablePrinter::VariablePrinter(Variable var, AST_Expression_ComponentReference ref, const VarSymbolTable& symbols, bool is_qss, int order)
    : _var(var), _ref(ref), _is_qss(is_qss), _order(order), _exp(), _symbols(symbols)
{
  generate();
}

void VariablePrinter::generate()
{
  stringstream buffer;
  if (_var.isLocal()) {
    _exp = _var.name();
    return;
  }
  if (ModelConfig::instance().initialCode() && _var.isState()) {
    buffer << "_init";
  }
  buffer << _var;
  const bool PRINT_COEFF = _is_qss && (_var.isState() || _var.isAlgebraic());
  const bool HAS_INDEXES = _ref->hasIndexes();
  if (HAS_INDEXES) {
    ExpressionPrinter printer(_symbols, _is_qss, _order);
    AST_ExpressionList indexes = _ref->firstIndex();
    AST_ExpressionListIterator it;
    int size = indexes->size(), i = 0;
    buffer << "(";
    foreach (it, indexes) {
      buffer << printer.apply(current_element(it)) << (++i < size ? "," : "");
    }
  }
  if (PRINT_COEFF && HAS_INDEXES) {
    buffer << "," << _order << ")";
  } else if (PRINT_COEFF) {
    buffer << "(" << _order << ")";
  } else if (HAS_INDEXES) {
    buffer << ")";
  }
  _exp = buffer.str();
}

std::ostream& operator<<(std::ostream& out, const VariablePrinter& var)
{
  out << var._exp;
  return out;
}

}  // namespace Util
}  // namespace MicroModelica
