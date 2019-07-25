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

#include <sstream>

#include "../ast/ast_builder.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../util/derivative.h"
#include "../util/util.h"
#include "../util/visitors/algebraics.h"
#include "../util/visitors/autonomous.h"
#include "../util/visitors/called_functions.h"
#include "../util/visitors/replace_der.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
using namespace Deps;
namespace IR {

Equation::Equation(AST_Expression eq, VarSymbolTable &symbols, EQUATION::Type type, int id, int offset)
    : _eq(), _lhs(), _rhs(), _range(), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(offset)
{
  process(eq);
}

Equation::Equation(AST_Expression eq, VarSymbolTable &symbols, Option<Range> range, EQUATION::Type type, int id, int offset)
    : _eq(), _lhs(), _rhs(), _range(range), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(offset)
{
  process(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0)
{
  process(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Range r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0)
{
  process(eq);
}

Equation::Equation(AST_Equation eq, VarSymbolTable &symbols, Option<Range> r, EQUATION::Type type, int id)
    : _eq(eq), _lhs(), _rhs(), _range(r), _autonomous(true), _symbols(symbols), _type(type), _id(id), _offset(0)
{
  process(eq);
}

void Equation::process(AST_Expression exp)
{
  if (_range) {
    AST_Expression_ComponentReference lhs = newAST_Expression_ComponentReference();
    AST_Expression idx = newAST_Expression_ComponentReferenceExp(newAST_String("i"));
    AST_ExpressionList l = newAST_ExpressionList();
    l = AST_ListAppend(l, idx);
    lhs = AST_Expression_ComponentReference_Add(lhs, newAST_String(functionId()), l);
    _lhs = Expression(lhs, _symbols);
  } else {
    AST_Expression lhs = newAST_Expression_ComponentReferenceExp(newAST_String(functionId()));
    _lhs = Expression(lhs, _symbols);
  }
  _rhs = Expression(exp, _symbols);
  Autonomous autonomous(_symbols);
  _autonomous = autonomous.apply(exp);
  CalledFunctions cf;
  _calledFunctions = cf.apply(exp);
}

void Equation::process(AST_Equation eq)
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
  Autonomous autonomous(_symbols);
  _autonomous = autonomous.apply(_rhs.expression());
  CalledFunctions cf;
  _calledFunctions = cf.apply(_rhs.expression());
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

string Equation::functionId() const
{
  stringstream buffer;
  buffer << functionId(_type) << _id;
  return buffer.str();
}

string Equation::functionId(EQUATION::Type type)
{
  stringstream buffer;
  switch (type) {
  case EQUATION::QSSDerivative:
    buffer << "_der_eq_";
    break;
  case EQUATION::Dependency:
    buffer << "_dep_eq_";
    break;
  case EQUATION::ZeroCrossing:
    buffer << "_event_";
    break;
  case EQUATION::Output:
    buffer << "_out_eq_";
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
  if (_type == EQUATION::ClassicDerivative) {
    return "";
  }
  FunctionPrinter fp;
  return fp.macro(functionId(), _range, _id, _offset);
}

string Equation::print() const
{
  switch (_type) {
  case EQUATION::ClassicDerivative: {
    ClassicConfig printer = ClassicConfig(*this);
    return printer.print();
  }
  case EQUATION::Output: {
    OutputConfig printer = OutputConfig(*this);
    return printer.print();
  }
  default: {
    EquationConfig printer = EquationConfig(*this, _symbols);
    return printer.print();
  }
  }
  assert(false);
  return "";
}

string EquationPrinter::lhs() const
{
  stringstream buffer;
  switch (_eq.type()) {
  case EQUATION::ClassicDerivative:
    buffer << _eq.lhs();
    break;
  case EQUATION::QSSDerivative:
    buffer << _eq.lhs();
    break;
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
  default:
    return "";
  }
  return "";
}

string ClassicConfig::print() const
{
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
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
  buffer << fp.beginExpression(_eq.functionId(), range);
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

void EquationConfig::initializeDerivatives()
{
  if (ModelConfig::instance().generateDerivatives()) {
    ExpressionDerivator ed;
    ReplaceDer replace_der(_symbols);
    Expression rhs = _eq.rhs();
    AST_Expression exp1 = ed.derivate(rhs.expression(), _symbols, rhs);
    _derivatives[0] = Expression(exp1, _symbols);
    AST_Expression exp2 = ed.derivate(exp1, _symbols, rhs);
    _derivatives[1] = Expression(replace_der.apply(exp2), _symbols);
    AST_Expression exp3 = ed.derivate(exp2, _symbols, rhs);
    _derivatives[2] = Expression(replace_der.apply(exp3), _symbols);
  }
}

string EquationConfig::generateDerivatives() const
{
  ModelConfig config = ModelConfig::instance();
  if (config.generateDerivatives()) {
    stringstream buffer;
    int order = config.order() - 1;
    for (int i = 0; i < order; i++) {
      buffer << block << prefix() << lhs() << " = " << _derivatives[i] << ";" << endl;
    }
    return buffer.str();
  }
  return "";
}

string EquationConfig::print() const
{
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
  Option<Range> range = _eq.range();
  buffer << fp.beginExpression(_eq.functionId(), range);
  buffer << fp.algebraics(_eq.dependencyMatrix(), _eq.id());
  block += TAB;
  if (range) {
    block = range->block();
  } else {
    block += TAB;
  }
  buffer << generateDerivatives();
  buffer << block << prefix() << lhs() << " = " << _eq.rhs() << ";";
  buffer << endl << block << fp.endExpression(range);
  return buffer.str();
}

}  // namespace IR
}  // namespace MicroModelica
