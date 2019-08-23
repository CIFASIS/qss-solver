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
#include "../util/derivative.h"
#include "../util/error.h"
#include "../util/util.h"
#include "../util/visitors/algebraics.h"
#include "../util/visitors/autonomous.h"
#include "../util/visitors/called_functions.h"
#include "../util/visitors/get_index_usage.h"
#include "../util/visitors/replace_der.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

Equation::Equation(AST_Expression eq, VarSymbolTable &symbols, EQUATION::Type type, int id, int offset)
    : _eq(), _lhs(), _rhs(), _range(), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(offset), _lhs_exp()
{
  initialize(eq);
}

Equation::Equation(AST_Expression lhs, AST_Expression rhs, VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id)
    : _eq(), _lhs(), _rhs(), _range(range), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp()
{
  initialize(lhs, rhs);
}

Equation::Equation(AST_Expression eq, VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id, int offset)
    : _eq(), _lhs(), _rhs(), _range(range), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(offset), _lhs_exp()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Range r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp()
{
  initialize(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Option<Range> r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0), _lhs_exp()
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
  if (_range) {
    AST_Expression_ComponentReference lhs = newAST_Expression_ComponentReference();
    AST_Expression idx = newAST_Expression_ComponentReferenceExp(newAST_String("i"));
    AST_ExpressionList l = newAST_ExpressionList();
    l = AST_ListAppend(l, idx);
    lhs = AST_Expression_ComponentReference_Add(lhs, newAST_String(identifier()), l);
    _lhs = Expression(lhs, _symbols);
  } else {
    AST_Expression lhs = newAST_Expression_ComponentReferenceExp(newAST_String(identifier()));
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
  stringstream buffer, id;
  buffer << identifier(_type);
  switch (_type) {
  case EQUATION::QSSDerivative:
    buffer << _lhs_exp;
    break;
  case EQUATION::Dependency:
    buffer << _lhs_exp;
    break;
  case EQUATION::Jacobian:
    buffer << _lhs_exp;
    break;
  case EQUATION::ZeroCrossing:
    buffer << _id;
    break;
  case EQUATION::Output:
    buffer << _id;
    break;
  default:
    return "";
  }
  return buffer.str();
}

string Equation::identifier(EQUATION::Type type)
{
  stringstream buffer;
  switch (type) {
  case EQUATION::QSSDerivative:
    buffer << "_eval";
    break;
  case EQUATION::Jacobian:
    buffer << "_eval";
    break;
  case EQUATION::Dependency:
    buffer << "_dep_eq_";
    break;
  case EQUATION::ZeroCrossing:
    buffer << "_event_";
    break;
  case EQUATION::Output:
    buffer << "_out_exp_";
    break;
  default:
    return "";
  }
  return buffer.str();
}

Option<Variable> Equation::LHSVariable()
{
  if (isDerivative() || isAlgebraic() || isZeroCrossing() || isOutput()) {
    AST_Expression_ComponentReference cr = _lhs.expression()->getAsComponentReference();
    return _symbols[cr->name()];
  }
  return Option<Variable>();
}

bool Equation::isValid() const { return _lhs.isValid() && _rhs.isValid(); }

std::ostream &operator<<(std::ostream &out, const Equation &e)
{
  out << e.print();
  return out;
}

bool Equation::isRHSReference() const { return _rhs.isReference(); }

bool Equation::hasAlgebraics()
{
  assert(isValid());
  Algebraics has_algebraics(_symbols);
  return has_algebraics.apply(_rhs.expression());
}

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
  default: {
    EquationConfig printer = EquationConfig(*this, _symbols);
    return printer.print();
  }
  }
  assert(false);
}

Dependency::Dependency(Variable var, VariableDependencies deps) : _var(var), _scalar(), _vector() { initialize(deps); }

void Dependency::initialize(VariableDependencies deps)
{
  EquationTable derivatives = ModelConfig::instance().derivatives();
  FunctionPrinter fp;
  stringstream buffer;
  if (_var.isScalar()) {
    buffer << fp.beginExpression(identifier(), Option<Range>());
  }
  for (Influences inf : deps) {
    Option<Equation> ifce = derivatives[inf.ifce.equationId()];
    if (ifce) {
      Range range = inf.ifce.range();
      if (_var.isArray()) {
        buffer << fp.beginExpression(identifier(), range);
      }
      Equation eq = ifce.get();
      eq.setType(EQUATION::Dependency);
      buffer << fp.algebraics(inf.algs);
      buffer << eq << endl;
      if (_var.isArray()) {
        static bool PRINT_RETURN = false;
        buffer << fp.endExpression(range, PRINT_RETURN) << endl;
      }
    }
  }
  if (_var.isScalar()) {
    buffer << TAB << TAB << fp.endExpression(Option<Range>());
    _scalar << buffer.str();
  } else {
    _vector << buffer.str();
  }
}

string Dependency::identifier()
{
  stringstream buffer;
  buffer << "_deps_var" << _var;
  return buffer.str();
}

Jacobian::Jacobian(Equation eq, VarSymbolTable &symbols) : _eq(eq), _symbols(symbols) {}

string Jacobian::print() const
{
  stringstream buffer;
  ExpressionDerivator exp_der;
  map<string, Expression> jac_exps = exp_der.generateJacobianExps(_eq.rhs().expression(), _symbols);
  VarSymbolTable symbols = _symbols;
  int last = 1, size = jac_exps.size();
  for (auto j : jac_exps) {
    int res = 0;
    AST_Expression lhs = parseExpression(j.first, &res);
    if (res) {
      Error::instance().add(0, EM_IR | EM_PARSE_FILE, ER_Fatal, "Can't generate Jacobian equation left hand side for: %s", j.first.c_str());
    }
    Equation jac = Equation(lhs, j.second.expression(), symbols, _eq.range(), EQUATION::Jacobian, _eq.id());
    buffer << jac << ((last++ != size) ? "\n" : "");
  }
  return buffer.str();
}

std::ostream &operator<<(std::ostream &out, const Jacobian &j)
{
  out << j.print();
  return out;
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
  string block = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  buffer << fp.beginExpression(_eq.identifier(), range);
  block += TAB;
  if (range) {
    block = range->block();
  } else {
    block += TAB;
  }
  buffer << block << prefix() << lhs() << " = " << _eq.rhs() << ";";
  buffer << endl << fp.endExpression(range, false);
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

string OutputConfig::print() const
{
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  buffer << fp.beginExpression(_eq.identifier(), range);
  buffer << fp.algebraics(_eq.dependencyMatrix(), _eq.id());
  block += TAB;
  if (range) {
    block = range->block();
  } else {
    block += TAB;
  }
  buffer << block << prefix() << lhs() << " = " << _eq.rhs() << ";";
  buffer << endl << block << fp.endExpression(range);
  return buffer.str();
}

EquationConfig::EquationConfig(Equation eq, VarSymbolTable symbols)
    : EquationPrinter(eq, symbols), _eq(eq), _symbols(symbols), _identifier()
{
  setup();
}

void EquationConfig::setup()
{
  initializeDerivatives();
  stringstream buffer;
  if (_eq.hasRange()) {
    // @TODO: Review this, why the initial extra space?
    buffer << _eq.LHSVariable();
    _identifier = buffer.str();
    _identifier.erase(0, 1);
  } else {
    buffer << "_eval" << _eq.lhs();
    _identifier = buffer.str();
  }
}

string EquationConfig::macro() const
{
  stringstream buffer;
  if (_eq.hasRange()) {
    GetIndexUsage index_usage;
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
    static constexpr int SECOND_ORDER = 1;
    static constexpr int THIRD_ORDER = 2;
    static constexpr int FOURTH_ORDER = 3;
    ExpressionDerivator ed;
    ReplaceDer replace_der(_symbols);
    Expression rhs = _eq.rhs();
    AST_Expression exp1 = ed.derivate(rhs.expression(), _symbols, rhs);
    _derivatives[0] = Expression(exp1, _symbols, SECOND_ORDER);
    AST_Expression exp2 = ed.derivate(exp1, _symbols, rhs);
    _derivatives[1] = Expression(replace_der.apply(exp2), _symbols, THIRD_ORDER);
    AST_Expression exp3 = ed.derivate(exp2, _symbols, rhs);
    _derivatives[2] = Expression(replace_der.apply(exp3), _symbols, FOURTH_ORDER);
  }
}

string EquationConfig::generateDerivatives(string tabs, int init) const
{
  ModelConfig config = ModelConfig::instance();
  if (config.generateDerivatives()) {
    stringstream buffer;
    int order = config.order() - 1;
    for (int i = 0; i < order; i++) {
      buffer << tabs << prefix() << lhs(init + i) << " = " << _derivatives[i] << ";";
      if (i + 1 < order) {
        buffer << endl;
      }
    }
    return buffer.str();
  }
  return "";
}

string EquationConfig::equationId() const
{
  stringstream buffer;
  buffer << "_eq_" << _eq.id();
  return buffer.str();
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
  buffer << fp.beginExpression(identifier(), range);
  buffer << fp.beginDimGuards(equationId(), arguments, range);
  buffer << fp.algebraics(_eq.dependencyMatrix(), _eq.id());
  tabs += TAB;
  buffer << tabs << prefix() << lhs() << " = " << _eq.rhs() << ";" << endl;
  buffer << generateDerivatives(tabs);
  buffer << endl << TAB << fp.endDimGuards(range);
  buffer << endl << TAB << fp.endExpression(range);
  return buffer.str();
}

string AlgebraicConfig::print() const
{
  stringstream buffer;
  string tabs = "";
  FunctionPrinter fp;
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
  tabs += TAB;
  if (range) {
    tabs = range->block();
  } else {
    tabs += TAB;
  }
  buffer << tabs << prefix() << lhs(FIRST_ORDER) << " = " << _eq.rhs() << ";" << endl;
  buffer << generateDerivatives(tabs, SECOND_ORDER);
  return buffer.str();
}

}  // namespace IR
}  // namespace MicroModelica
