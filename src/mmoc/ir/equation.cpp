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

#include "equation.h"

#include <boost/optional/optional_io.hpp>
#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../parser/parse.h"
#include "../util/error.h"
#include "../util/util.h"
#include "../util/visitors/algebraics.h"
#include "../util/visitors/autonomous.h"
#include "../util/visitors/called_functions.h"
#include "../util/visitors/get_index_variables.h"
#include "../util/visitors/is_recursive_def.h"
#include "../util/visitors/replace_der.h"
#include "../util/visitors/revert_index.h"
#include "derivative.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

Equation::Equation(AST_Expression lhs, AST_Expression rhs, VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id)
    : _eq(), _lhs(), _rhs(), _range(range), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp(), _usage()
{
  initialize(lhs, rhs);
}

Equation::Equation(AST_Expression eq, VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id, int offset)
    : _eq(),
      _lhs(),
      _rhs(),
      _range(range),
      _autonomous(true),
      _symbols(symbols),
      _type(type),
      _id(id),
      _offset(offset),
      _lhs_exp(),
      _usage()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp(), _usage()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Range r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp(), _usage()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Option<Range> r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp(), _usage()
{
  initialize(eq);
}

Equation::~Equation() {}

void Equation::initialize(AST_Expression lhs, AST_Expression rhs)
{
  _lhs = Expression(lhs, _symbols);
  _rhs = Expression(rhs, _symbols);
  setup();
}

void Equation::initialize(AST_Expression exp)
{
  string model_variable = EquationVariable::modelVariables(_id, _type);
  if (_range) {
    _lhs = Utils::instance().variableExpression(model_variable, _range, _symbols);
  } else {
    AST_Expression lhs = newAST_Expression_ComponentReferenceExp(newAST_String(model_variable));
    _lhs = Expression(lhs, _symbols);
  }
  _rhs = Expression(exp, _symbols);
  setup();
}

void Equation::initialize(AST_Equation eq)
{
  AST_Equation_Equality eqe = eq->getAsEquality();
  if (eqe->left()->expressionType() == EXPDERIVATIVE) {
    _lhs = Expression(AST_ListFirst(eqe->left()->getAsDerivative()->arguments()), _symbols);
    _rhs = Expression(eqe->right(), _symbols);
  } else if (eqe->left()->expressionType() == EXPCOMPREF) {
    _lhs = Expression(eqe->left(), _symbols);
    _rhs = Expression(eqe->right(), _symbols);
  } else if (eqe->left()->expressionType() == EXPOUTPUT) {
    AST_Expression_Output eout = eqe->left()->getAsOutput();
    _lhs = Expression(eout, _symbols);
    _rhs = Expression(eqe->right(), _symbols);
  }
  setup();
}

void Equation::setup()
{
  stringstream buffer;
  Autonomous autonomous(_symbols);
  _autonomous = autonomous.apply(_rhs.expression());
  CalledFunctions cf;
  _calledFunctions = cf.apply(_rhs.expression());
  if (_range) {
    buffer << LHSVariable();
  } else {
    buffer << _lhs;
  }
  _lhs_exp = buffer.str();
}

EquationDependencyMatrix Equation::dependencyMatrix() const
{
  ModelDependencies deps = ModelConfig::instance().dependencies();
  switch (_type) {
  case EQUATION::ClassicDerivative:
  case EQUATION::QSSDerivative:
    return deps.DA();
  case EQUATION::ZeroCrossing:
    return deps.ZCA();
  case EQUATION::Output:
    return deps.OA();
  default:
    return EquationDependencyMatrix();
  }
  return EquationDependencyMatrix();
}

string Equation::identifier() const
{
  EquationPrinter equation_printer = EquationPrinter(*this, _symbols);
  return equation_printer.identifier();
}

Option<Variable> Equation::LHSVariable()
{
  if (isDerivative() || isAlgebraic() || isZeroCrossing() || isOutput() || isJacobian()) {
    return _lhs.reference();
  }
  return Option<Variable>();
}

bool Equation::isRecursive() const
{
  if (_lhs.isScalar()) {
    return false;
  }
  Variable var = _lhs.reference().get();
  IsRecursiveDef is_recursive(var.name());
  return is_recursive.apply(_rhs.expression());
}

bool Equation::isValid() const { return _lhs.isValid() && _rhs.isValid(); }

std::ostream &operator<<(std::ostream &out, const Equation &e)
{
  out << e.print();
  return out;
}

Index Equation::index() const
{
  assert(isValid());
  Index idx = Index(_lhs);
  return idx;
}

bool Equation::isRHSReference() const { return _rhs.isReference(); }

bool Equation::hasAlgebraics()
{
  assert(isValid());
  Algebraics has_algebraics;
  return has_algebraics.apply(_rhs.expression());
}

// @TODO: Initialize and use a pointer in these functions.

string Equation::macro() const
{
  switch (_type) {
  case EQUATION::ClassicDerivative: {
    ClassicConfig printer = ClassicConfig(*this, _symbols);
    return printer.macro();
  }
  case EQUATION::Output: {
    OutputConfig printer = OutputConfig(*this, _symbols);
    return printer.macro();
  }
  case EQUATION::Algebraic: {
    if (ModelConfig::instance().isQss()) {
      AlgebraicConfig printer = AlgebraicConfig(*this, _symbols);
      return printer.macro();
    } else {
      ClassicConfig printer = ClassicConfig(*this, _symbols);
      return printer.macro();
    }
  }
  case EQUATION::Jacobian: {
    JacobianConfig printer = JacobianConfig(*this, _symbols);
    return printer.macro();
  }
  case EQUATION::Dependency: {
    DependencyConfig printer = DependencyConfig(*this, _symbols);
    return printer.macro();
  }
  case EQUATION::ZeroCrossing: {
    ZeroCrossingConfig printer = ZeroCrossingConfig(*this, _symbols);
    return printer.macro();
  }
  default: {
    EquationConfig printer = EquationConfig(*this, _symbols);
    return printer.macro();
  }
  }
  assert(false);
}

string Equation::print() const
{
  switch (_type) {
  case EQUATION::ClassicDerivative: {
    ClassicConfig printer = ClassicConfig(*this, _symbols);
    return printer.print();
  }
  case EQUATION::Output: {
    OutputConfig printer = OutputConfig(*this, _symbols);
    return printer.print();
  }
  case EQUATION::Algebraic: {
    if (ModelConfig::instance().isQss()) {
      AlgebraicConfig printer = AlgebraicConfig(*this, _symbols);
      return printer.print();
    } else {
      ClassicConfig printer = ClassicConfig(*this, _symbols);
      return printer.print();
    }
  }
  case EQUATION::Jacobian: {
    JacobianConfig printer = JacobianConfig(*this, _symbols);
    return printer.print();
  }
  case EQUATION::Dependency: {
    DependencyConfig printer = DependencyConfig(*this, _symbols);
    return printer.print();
  }
  case EQUATION::ZeroCrossing: {
    ZeroCrossingConfig printer = ZeroCrossingConfig(*this, _symbols);
    return printer.print();
  }
  default: {
    EquationConfig printer = EquationConfig(*this, _symbols);
    return printer.print();
  }
  }
  assert(false);
}

Equation Dependency::generate(Equation eq, Index idx, AlgebraicDependencies algs)
{
  Equation dep = eq;
  dep.setType(EQUATION::Dependency);
  dep.setUsage(idx);
  return dep;
}

list<Equation> Dependency::terms() { return list<Equation>(); }

Equation Jacobian::generate(Equation eq, Index idx, AlgebraicDependencies algs)
{
  ExpressionDerivator exp_der;
  for (VariableDependency var : algs) {
    exp_der.generateJacobianTerm(idx, var);
  }
  Equation jac = exp_der.generateJacobianExp(idx, eq);
  _jac_terms = exp_der.terms();
  return jac;
}

list<Equation> Jacobian::terms() { return _jac_terms; }

EquationPrinter::EquationPrinter(Equation eq, Util::VarSymbolTable symbols) : _eq(eq), _symbols(symbols), _identifier() { setup(); }

void EquationPrinter::setup()
{
  stringstream buffer;
  if (_eq.hasRange()) {
    Option<Variable> var = _eq.LHSVariable();
    if (var) {
      buffer << *var;
    }
    _identifier = buffer.str();
  } else {
    buffer << "_eval" << _eq.lhs();
    _identifier = buffer.str();
  }
}

string EquationPrinter::equationId() const
{
  stringstream buffer;
  buffer << "_eq_" << _eq.id();
  return buffer.str();
}

string EquationPrinter::lhs(int order) const
{
  stringstream buffer;
  switch (_eq.type()) {
  case EQUATION::Algebraic:
  case EQUATION::ClassicDerivative:
  case EQUATION::Dependency:
  case EQUATION::QSSDerivative: {
    Expression exp = Expression(_eq.lhs().expression(), _symbols, order);
    buffer << exp;
    break;
  }
  case EQUATION::ZeroCrossing: {
    if (ModelConfig::instance().isQss()) {
      buffer << "(" << order << ")";
    }
    break;
  }
  case EQUATION::Jacobian:
    return "_jac(jit)";
  default:
    return "";
  }
  return buffer.str();
}

string EquationPrinter::prefix() const
{
  switch (_eq.type()) {
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

string JacobianConfig::print() const
{
  stringstream buffer;
  string tabs = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  string arguments;
  tabs += TAB;
  Index idx = _eq.usage();
  // Case 1 -> N
  const bool PRINT_EQ_RANGE = idx.isConstant() && range;
  if (range) {
    tabs = range->block();
    if (PRINT_EQ_RANGE) {
      buffer << range.get();
      arguments = range.get().indexes();
    } else {
      Index revert = Index(_eq.lhs()).revert().replace();
      arguments = revert.usageExp();
    }
  }
  tabs += TAB;
  buffer << fp.beginDimGuards(equationId(), arguments, range);
  buffer << tabs << prefix() << lhs() << " = " << _eq.rhs() << ";";
  buffer << endl << TAB << fp.endDimGuards(range);
  if (PRINT_EQ_RANGE) {
    buffer << range.get().end();
  }

  return buffer.str();
}

string ClassicConfig::print() const
{
  stringstream buffer;
  string block = "";
  Option<Range> range = _eq.range();
  if (range) {
    buffer << range.get();
    block = range->block();
  }
  buffer << block << prefix() << lhs() << " = " << _eq.rhs() << ";";
  if (range) {
    buffer << endl << range.get().end();
  }
  return buffer.str();
}

string OutputConfig::equationId() const
{
  stringstream buffer;
  buffer << "_out_exp_" << _eq.id();
  return buffer.str();
}

string OutputConfig::print() const
{
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  string arguments;
  block += TAB;
  if (range) {
    block = range->block();
    arguments = range->getDimensionVars();
  }
  block += TAB;
  buffer << fp.beginExpression(identifier(), range);
  buffer << fp.beginDimGuards(equationId(), arguments, range);
  buffer << fp.algebraics(_eq.dependencyMatrix(), _eq.id());
  buffer << block << prefix() << lhs() << " = " << _eq.rhs() << ";";
  buffer << endl << TAB << fp.endDimGuards(range);
  buffer << TAB << fp.endExpression(range);
  return buffer.str();
}

EquationConfig::EquationConfig(Equation eq, VarSymbolTable symbols)
    : EquationPrinter(eq, symbols), _eq(eq), _symbols(symbols), _fact_init(2)
{
  initializeDerivatives();
}

string EquationConfig::macro() const
{
  stringstream buffer;
  if (_eq.hasRange()) {
    GetIndexVariables index_usage;
    Option<Range> range = _eq.range();
    buffer << "#define _apply_usage" << equationId();
    buffer << "(" << range->getDimensionVars() << ") \\" << endl;
    list<string> usage = index_usage.apply(_eq.lhs().expression());
    int i = 1, size = usage.size();
    for (string index : usage) {
      buffer << TAB << index << " = " << range->getDimensionVar(i++) << ";";
      buffer << ((i + 1 <= size) ? " \\" : "");
      buffer << endl;
    }
  }
  return buffer.str();
}

void EquationConfig::initializeDerivatives()
{
  if (ModelConfig::instance().generateDerivatives()) {
    ExpressionDerivator ed;
    ReplaceDer replace_der(_symbols);
    Expression rhs = _eq.rhs();
    AST_Expression exp1 = ed.derivate(rhs.expression(), rhs);
    _derivatives[0] = Expression(exp1, _symbols);
    AST_Expression exp2 = ed.derivate(exp1, rhs);
    _derivatives[1] = Expression(replace_der.apply(exp2), _symbols);
    AST_Expression exp3 = ed.derivate(exp2, rhs);
    _derivatives[2] = Expression(replace_der.apply(exp3), _symbols);
  }
}

string EquationConfig::generateDerivatives(string tabs, int init) const
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

string EquationConfig::print() const
{
  stringstream buffer;
  string tabs = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  string arguments;
  tabs += TAB;
  if (range) {
    tabs = range->block();
    arguments = range->getDimensionVars();
  }
  tabs += TAB;
  buffer << fp.beginExpression(identifier(), range);
  buffer << fp.beginDimGuards(equationId(), arguments, range);
  buffer << fp.algebraics(_eq.dependencyMatrix(), _eq.id());
  buffer << tabs << prefix() << lhs() << " = " << _eq.rhs() << ";" << endl;
  buffer << generateDerivatives(tabs);
  buffer << endl << TAB << fp.endDimGuards(range);
  buffer << TAB << fp.endExpression(range);
  return buffer.str();
}

string AlgebraicConfig::print() const
{
  stringstream buffer;
  string tabs = "";
  Option<Range> range = _eq.range();
  tabs += TAB;
  if (range) {
    tabs = range->block();
  } else {
    tabs += TAB;
  }
  buffer << tabs << prefix() << lhs() << " = " << _eq.rhs() << ";" << endl;
  buffer << generateDerivatives(tabs);
  return buffer.str();
}

string DependencyConfig::print() const
{
  stringstream buffer;
  string tabs = "";
  static constexpr int FIRST_ORDER = 1;
  static constexpr int SECOND_ORDER = 2;
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  string arguments;
  tabs += TAB;
  Index idx = _eq.usage();
  // Case 1 -> N
  const bool PRINT_EQ_RANGE = idx.isConstant() && range;
  if (range) {
    tabs = range->block();
    if (PRINT_EQ_RANGE) {
      buffer << range.get();
      arguments = range.get().indexes();
    } else {
      Index revert = _eq.usage().revert().replace();
      arguments = revert.usageExp();
    }
  }
  tabs += TAB;
  buffer << fp.beginDimGuards(equationId(), arguments, range);
  buffer << tabs << prefix() << lhs(FIRST_ORDER) << " = " << _eq.rhs() << ";" << endl;
  buffer << generateDerivatives(tabs, SECOND_ORDER);
  buffer << TAB << fp.endDimGuards(range);
  if (PRINT_EQ_RANGE) {
    buffer << range.get().end();
  }
  return buffer.str();
}

string ZeroCrossingConfig::equationId() const
{
  stringstream buffer;
  buffer << "_event_" << _eq.id();
  return buffer.str();
}

}  // namespace IR
}  // namespace MicroModelica
