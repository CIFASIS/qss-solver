/*****************************************************************************

    This file is part of Modelica C Compiler.

    Modelica C Compiler is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Modelica C Compiler is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Modelica C Compiler.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include <causalize/for_unrolling/for_index_iterator.h>
#include <ast/expression.h>
#include <util/ast_visitors/eval_expression.h>
#include <ast/modification.h>
#include <util/debug.h>

namespace Causalize {

RangeIterator::RangeIterator(Range range, VarSymbolTable symbolTable) {
  _rangeBegin = eval(range.start(), symbolTable);
  _rangeEnd = eval(range.end(),symbolTable);
  if (!range.step()) 
   _rangeStep = 1.0;
  else 
   _rangeStep = eval(range.step().get(),symbolTable);
  _current = _rangeBegin;
}

Real RangeIterator::eval(Expression exp, VarSymbolTable symbolTable) {
  return Apply(Modelica::EvalExpression(symbolTable),exp);
}

bool RangeIterator::hasNext() {
  return _current <= _rangeEnd;
}

Real RangeIterator::next() {
  Real value = _current;
  _current += _rangeStep;
  return value;
}

BraceIterator::BraceIterator(Brace braceExp, VarSymbolTable &v):vtable(v) {
  _braceExpElements = braceExp.args();
  _braceExpElementsIter = _braceExpElements.begin();
}

bool BraceIterator::hasNext() {
  return _braceExpElementsIter == _braceExpElements.end();
}

Real BraceIterator::next() {
  Expression exp = *_braceExpElementsIter;
  _braceExpElementsIter++;
  return Apply(Modelica::EvalExpression(vtable),exp);
}
}
