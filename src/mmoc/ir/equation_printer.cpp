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

#include "equation_printer.h"

#include <boost/optional/optional_io.hpp>
#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../parser/parse.h"
#include "../util/error.h"
#include "../util/model_config.h"
#include "../util/util.h"
#include "../util/visitors/get_index_variables.h"
#include "../util/visitors/is_recursive_def.h"
#include "../util/visitors/replace_der.h"
#include "../util/visitors/revert_index.h"
#include "alg_usage.h"
#include "derivative.h"
#include "equation.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

EquationPrinter* getPrinter(Equation eq)
{
  switch (eq.type()) {
  case EQUATION::ClassicDerivative:
    return new ClassicPrinter(eq);
  case EQUATION::Output:
    return new OutputPrinter(eq);
  case EQUATION::Algebraic:
    return new AlgebraicPrinter(eq);
  case EQUATION::Dependency:
    return new DependencyPrinter(eq);
  case EQUATION::ZeroCrossing:
    return new ZeroCrossingPrinter(eq);
  default:
    return new DerivativePrinter(eq);
  }
}

EquationPrinter::EquationPrinter(Equation eq)
    : _identifier(), _id(eq.id()), _type(eq.type()), _lhs(eq.lhs()), _alg_code(eq.algCode())
{
  setup(eq);
}

void EquationPrinter::setup(Equation eq)
{
  stringstream buffer;
  if (eq.hasRange()) {
    Option<Variable> var = eq.LHSVariable();
    if (var) {
      buffer << *var;
    }
    _identifier = buffer.str();
  } else {
    buffer << "_eval" << eq.lhs();
    _identifier = buffer.str();
  }
}

string EquationPrinter::equationId() const
{
  stringstream buffer;
  buffer << "_eq_" << _id;
  return buffer.str();
}

string EquationPrinter::lhs(int order) const
{
  stringstream buffer;
  switch (_type) {
  case EQUATION::Algebraic:
  case EQUATION::ClassicDerivative:
  case EQUATION::Dependency:
  case EQUATION::QSSDerivative: {
    Expression exp = Expression(_lhs.expression(), order);
    buffer << exp;
    break;
  }
  case EQUATION::ZeroCrossing: {
    if (ModelConfig::instance().isQss()) {
      buffer << "(" << order << ")";
    }
    break;
  }
  default:
    return "";
  }
  return buffer.str();
}

string EquationPrinter::prefix() const
{
  switch (_type) {
  case EQUATION::ClassicDerivative:
    return "_der";
  case EQUATION::QSSDerivative:
    return "_der";
  case EQUATION::ZeroCrossing:
    return "_zc";
  case EQUATION::Output:
    return "_out";
  case EQUATION::Dependency:
    return "_eval_dep";
  default:
    return "";
  }
  return "";
}

DerivativePrinter::DerivativePrinter(Equation eq)
    : EquationPrinter(eq),
      _fact_init(2),
      _id(eq.id()),
      _range(eq.range()),
      _lhs(eq.lhs()),
      _rhs(eq.rhs()),
      _eq_dep_matrix(eq.dependencyMatrix())
{
  initializeDerivatives();
}

string DerivativePrinter::macro() const
{
  stringstream buffer;
  if (_range) {
    stringstream apply_buffer;
    GetIndexVariables index_usage;
    FunctionPrinter function_printer;
    RangeDefinitionTable range_def = _range->definition();
    apply_buffer << "#define _apply_usage" << equationId();
    apply_buffer << "(" << _lhs.dimVariables() << ") \\" << endl;
    map<string, int> usage = index_usage.apply(_lhs.expression());
    int i = 1, size = usage.size();
    for (auto index : usage) {
      apply_buffer << TAB << index.first << " = " << _range->getDimensionVar(index.second) << ";";
      apply_buffer << ((i + 1 <= size) ? "\\" : "");
      apply_buffer << endl;
      i++;
    }
    buffer << apply_buffer.str();
    buffer << function_printer.equationVariableMacros(_range, _lhs, equationId());
  }
  return buffer.str();
}

void DerivativePrinter::initializeDerivatives()
{
  if (ModelConfig::instance().generateDerivatives()) {
    ExpressionDerivator ed;
    ReplaceDer replace_der;
    AST_Expression exp1 = ed.derivate(_rhs.expression(), _rhs);
    _derivatives[0] = Expression(exp1);
    AST_Expression exp2 = ed.derivate(exp1, _rhs);
    _derivatives[1] = Expression(replace_der.apply(exp2));
    AST_Expression exp3 = ed.derivate(exp2, _rhs);
    _derivatives[2] = Expression(replace_der.apply(exp3));
  }
}

string DerivativePrinter::generateDerivatives(string tabs, int init) const
{
  ModelConfig config = ModelConfig::instance();
  if (config.generateDerivatives()) {
    stringstream buffer;
    int order = config.order() - 1;
    int fact = _fact_init;
    const bool POLY_COEFF = _fact_init > 0;
    for (int i = 0; i < order; i++) {
      buffer << tabs << prefix() << lhs(init + i) << " = ";
      if (POLY_COEFF) {
        buffer << "(";
      }
      buffer << _derivatives[i];
      if (POLY_COEFF) {
        buffer << ")/" << fact;
      }
      buffer << ";";
      if (i + 1 < order) {
        buffer << endl;
        fact *= fact + 1;
      }
    }
    return buffer.str();
  }
  return "";
}

string DerivativePrinter::print() const
{
  stringstream buffer;
  string tabs = "";
  FunctionPrinter printer;
  string arguments;
  tabs += TAB;
  if (_range) {
    tabs = _range->block();
    arguments = _lhs.dimVariables();
  }
  tabs += TAB;
  buffer << printer.beginExpression(identifier(), _range);
  string alg_code = algCode();
  if (alg_code.empty()) {
    buffer << printer.algebraics(_eq_dep_matrix, _id);
  } else {
    buffer << alg_code;
  }
  buffer << printer.beginDimGuards(equationId(), arguments, _range);
  buffer << tabs << prefix() << lhs() << " = " << _rhs << ";" << endl;
  buffer << generateDerivatives(tabs) << endl;
  buffer << endl << TAB << printer.endDimGuards(_range);
  buffer << TAB << printer.endExpression(_range);
  return buffer.str();
}

ClassicPrinter::ClassicPrinter(Equation eq)
    : DerivativePrinter(eq), _range(eq.range()), _rhs(eq.rhs())
{
}

string ClassicPrinter::print() const
{
  stringstream buffer;
  string block = "";
  if (_range) {
    buffer << _range.get();
    block = _range->block();
  }
  buffer << block << prefix() << lhs() << " = " << _rhs << ";";
  if (_range) {
    buffer << endl << _range.get().end();
  }
  return buffer.str();
}

OutputPrinter::OutputPrinter(Equation eq)
    : DerivativePrinter(eq), _id(eq.id()), _range(eq.range()), _rhs(eq.rhs()), _eq_dep_matrix(eq.dependencyMatrix()){};

string OutputPrinter::equationId() const
{
  stringstream buffer;
  buffer << "_out_exp_" << _id;
  return buffer.str();
}

string OutputPrinter::print() const
{
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
  string arguments;
  block += TAB;
  if (_range) {
    block = _range->block();
    arguments = _range->getDimensionVarsString();
  }
  block += TAB;
  buffer << fp.beginExpression(identifier(), _range);
  buffer << fp.beginDimGuards(equationId(), arguments, _range);
  buffer << fp.algebraics(_eq_dep_matrix, _id);
  buffer << block << prefix() << lhs() << " = " << _rhs << ";";
  buffer << endl << TAB << fp.endDimGuards(_range);
  buffer << TAB << fp.endExpression(_range);
  return buffer.str();
}

AlgebraicPrinter::AlgebraicPrinter(Equation eq)
    : DerivativePrinter(eq), _range(eq.range()), _rhs(eq.rhs()), _lhs(eq.lhs()), _id(eq.id())
{
  factorialInit(0);
};

string AlgebraicPrinter::print() const
{
  stringstream buffer;
  string tabs = "";
  tabs += TAB;
  const bool CLASSIC_MODEL = !ModelConfig::instance().isQss();
  if (_range) {
    tabs = _range->block();
    if (CLASSIC_MODEL) {
      buffer << _range.get();
    }
  }
  buffer << tabs << prefix() << _lhs << " = " << _rhs << ";" << endl;
  if (!_range) {
    tabs += TAB;
  }
  buffer << generateDerivatives(tabs) << endl;
  if (_range && CLASSIC_MODEL) {
    buffer << _range->end();
  }
  return buffer.str();
}

string AlgebraicPrinter::equationId() const
{
  stringstream buffer;
  buffer << "_alg_eq_" << _id;
  return buffer.str();
}

DependencyPrinter::DependencyPrinter(Equation eq)
    : DerivativePrinter(eq), _usage(eq.usage()), _range(eq.range()), _rhs(eq.rhs()) {};

string DependencyPrinter::print() const
{
  stringstream buffer;
  string tabs = "";
  static constexpr int FIRST_ORDER = 1;
  static constexpr int SECOND_ORDER = 2;
  FunctionPrinter fp;
  string arguments;
  tabs += TAB;
  // Case 1 -> N
  const bool PRINT_EQ_RANGE = _usage.isConstant() && _range;
  if (_range) {
    tabs = _range->block();
    if (PRINT_EQ_RANGE) {
      buffer << _range.get();
      arguments = _range.get().indexes();
    } else {
      Index revert = _usage.revert().replace();
      arguments = revert.usageExp();
    }
  }
  tabs += TAB;
  buffer << fp.beginDimGuards(equationId(), arguments, _range);
  buffer << tabs << prefix() << lhs(FIRST_ORDER) << " = " << _rhs << ";" << endl;
  buffer << generateDerivatives(tabs, SECOND_ORDER);
  buffer << TAB << fp.endDimGuards(_range);
  if (PRINT_EQ_RANGE) {
    buffer << _range.get().end();
  }
  return buffer.str();
}

ZeroCrossingPrinter::ZeroCrossingPrinter(Equation eq) : DerivativePrinter(eq), _id(eq.id())
{
  factorialInit(1);
};

string ZeroCrossingPrinter::equationId() const
{
  stringstream buffer;
  buffer << "_event_" << _id;
  return buffer.str();
}

}  // namespace IR
}  // namespace MicroModelica
