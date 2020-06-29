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
#include "alg_usage.h"
#include "derivative.h"
#include "equation_printer.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

Equation::Equation()
    : _eq(nullptr),
      _lhs(),
      _rhs(),
      _range(),
      _autonomous(true),
      _symbols(),
      _type(EQUATION::QSSDerivative),
      _id(0),
      _offset(0),
      _lhs_exp(),
      _usage()
{
}

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
  case EQUATION::Dependency:
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

string Equation::identifier() const { return getPrinter(*this, _symbols)->identifier(); }

string Equation::applyId() const { return getPrinter(*this, _symbols)->equationId(); }

Option<Variable> Equation::LHSVariable() const
{
  if (isDerivative() || isAlgebraic() || isZeroCrossing() || isOutput()) {
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

bool Equation::isEmpty() const { return _lhs.isEmpty() || _rhs.isEmpty(); }

bool Equation::isRHSReference() const { return _rhs.isReference(); }

bool Equation::hasAlgebraics()
{
  assert(isValid());
  Algebraics has_algebraics;
  return has_algebraics.apply(_rhs.expression());
}

string Equation::macro() const { return getPrinter(*this, _symbols)->macro(); }

string Equation::print() const { return getPrinter(*this, _symbols)->print(); }

void Equation::setType(EQUATION::Type type) { _type = type; }

void Equation::applyUsage(Index usage)
{
  VariableUsage alg_usage(_lhs, _rhs, usage);
  _rhs = alg_usage.rhs();
  _lhs = alg_usage.lhs();
  if (usage.isConstant()) {
    _range = Option<Range>();
  }
  setup();
}

Equation Equation::genAlgEquation(Equation der_eq, Index rhs_usage, Index lhs_usage)
{
  Equation a = *this;
  a.applyUsage(rhs_usage);
  if (lhs_usage.isConstant()) {
    if (!rhs_usage.isConstant()) {
      Expression new_usage = VariableUsage(der_eq.lhs(), rhs_usage.expression(), lhs_usage).rhs();
      a.applyUsage(new_usage);
    }
  } else {
    // Compute new LHS.
    Expression map_usage = rhs_usage.revert().expression();
    Expression new_usage = VariableUsage(map_usage, lhs_usage.revert()).usage();
    a.applyUsage(Index(new_usage));
  }
  return a;
}

void Equation::dependencyUsage(VariableDependency var_dep, Index index)
{
  if (hasRange() && index.isConstant()) {
    Expression new_usage = VariableUsage(var_dep.usage(), lhs(), var_dep.ife()).rhs();
    applyUsage(Index(new_usage));
  }
}

int Equation::arrayId() const { return _id - 1; }

Equation Dependency::generate(Equation eq, Index idx, AlgebraicPath algs)
{
  Equation dep = eq;
  dep.setType(EQUATION::Dependency);
  dep.setUsage(idx);
  if (!algs.empty()) {
    dep.dependencyUsage(algs.back(), idx);
  }
  return dep;
}

list<Equation> Dependency::terms() { return list<Equation>(); }

EQUATION::Type Dependency::type() const { return EQUATION::Dependency; }

}  // namespace IR
}  // namespace MicroModelica
