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

#include <iomanip>
#include <sstream>

#include <ast/ast_builder.h>
#include <ir/class.h>
#include <ir/helpers.h>
#include <util/error.h>
#include <util/model_config.h>
#include <util/util.h>
#include <util/visitors/is_constant_expression.h>

namespace MicroModelica {
using namespace IR;
namespace Util {

ExpressionPrinter::ExpressionPrinter(int order) : _order(order) {}

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
      FunctionTable package_functions = Utils::instance().packageFunctions();
      Option<Function> pkg_f = package_functions[call->name()->c_str()];
      if (!pkg_f) {
        Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "Function definition: expression_printer.cpp:56 %s",
                              call->name()->c_str());
        break;
      }
      Util::SymbolTable symbols;
      f = CompiledFunction(pkg_f->name(), "", "", symbols, Utils::instance().packagePrefix());
    }
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
    Option<Variable> var = ModelConfig::instance().lookup(ref->name());
    if (!var) {
      Error::instance().add(exp->lineNum(), EM_IR | EM_VARIABLE_NOT_FOUND, ER_Error, "expression_printer.cpp:80 %s", ref->name().c_str());
      break;
    }
    VariablePrinter var_printer(var.get(), ref, _order);
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
    buffer << std::scientific << exp->getAsReal()->val();
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
  case BINOPDIV: {
    IsConstantExpression constant_exp(true, true);
    buffer << l << "/" << ((constant_exp.apply(_right)) ? "(double)" : "") << r;
  } break;
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

VariablePrinter::VariablePrinter(Variable var, AST_Expression_ComponentReference ref, int order)
    : _var(var), _ref(ref), _order(order), _exp(), _begin_delimiter("("), _end_delimiter(")"), _begin_index_access(), _end_index_access()
{
  config();
  generate();
}

void VariablePrinter::config()
{
  if (ModelConfig::instance().functionCode()) {
    _begin_delimiter = "[";
    _end_delimiter = "]";
    _begin_index_access = "(";
    _end_index_access = "-1)";
  }
}

string VariablePrinter::access(bool array_access) const
{
  if (ModelConfig::instance().functionCode()) {
    if (_var.isOutput() && !ModelConfig::instance().functionOutputs() && ModelConfig::instance().compiledFunctionVar()) {
      return "&";
    }
    if (_var.isOutput() && ModelConfig::instance().functionOutputs() && !ModelConfig::instance().compiledFunctionVar() && !_var.isArray()) {
      return "*";
    }
  }
  if (array_access) {
    return "&";
  }
  return "";
}

void VariablePrinter::generate()
{
  stringstream buffer;
  if (_var.isLocal()) {
    _exp = _var.name();
    return;
  }
  ModelConfig& config = ModelConfig::instance();
  const bool PRINT_COEFF = config.isQss() && (_var.isState() || _var.isAlgebraic());
  const bool HAS_INDEXES = _ref->hasIndexes();
  const bool ARRAY_ACCESS = config.compiledFunctionVar() && !HAS_INDEXES && _var.isArray();
  buffer << access(ARRAY_ACCESS);
  if ((config.initialCode() || (config.algorithm() && config.reinit())) && _var.isState()) {
    buffer << "_init";
  } else if (config.isQss() && config.algorithm() && !config.reinit() && _var.isState()) {
    buffer << "_q";
  }
  buffer << _var;
  if (HAS_INDEXES) {
    ExpressionPrinter printer(_order);
    AST_ExpressionList indexes = _ref->firstIndex();
    AST_ExpressionListIterator it;
    int size = indexes->size(), i = 0;
    buffer << _begin_delimiter;
    foreach (it, indexes) {
      buffer << _begin_index_access << printer.apply(current_element(it)) << _end_index_access << (++i < size ? "," : "");
    }
  }
  if (ARRAY_ACCESS) {
    buffer << _begin_delimiter << "0";
    if (PRINT_COEFF) {
      buffer << "," << _order;
    }
    buffer << _end_delimiter;
  } else if (PRINT_COEFF && HAS_INDEXES) {
    buffer << "," << _order << _end_delimiter;
  } else if (PRINT_COEFF) {
    buffer << _begin_delimiter << _order << _end_delimiter;
  } else if (HAS_INDEXES) {
    buffer << _end_delimiter;
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
