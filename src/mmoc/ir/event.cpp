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

#include "event.h"

#include <assert.h>
#include <sstream>

#include "../ast/expression.h"
#include "../util/util.h"
#include "../util/visitors/convert_condition.h"
#include "helpers.h"

namespace MicroModelica {
using namespace Util;
namespace IR {

Event::Event(AST_Expression cond, int id, int offset, VarSymbolTable &symbols, Option<Range> range)
    : _zeroCrossing(),
      _positiveHandler(),
      _negativeHandler(),
      _type(EVENT::Zero),
      _current(EVENT::Zero),
      _zcRelation(EVENT::GE),
      _symbols(symbols),
      _range(range),
      _positiveHandlerId(0),
      _negativeHandlerId(0),
      _id(id),
      _offset(offset)
{
  ConvertCondition cc;
  _zeroCrossing = Equation(cc.apply(getExpression(cond)), symbols, range, EQUATION::ZeroCrossing, id, offset);
  _type = cc.zeroCrossing();
  _current = _type;
  _zcRelation = cc.zeroCrossingRelation();
}

void Event::add(AST_Statement stm)
{
  Statement s(stm, _symbols, _range);
  if (_current == EVENT::Positive) {
    _positiveHandler.insert(_positiveHandlerId++, s);
  } else if (_current == EVENT::Negative) {
    _negativeHandler.insert(_negativeHandlerId++, s);
  }
}

bool Event::compare(AST_Expression zc)
{
  ConvertCondition cc;
  AST_Expression c = cc.apply(getExpression(zc));
  EqualExp ee(_symbols);
  bool cr = ee.equalTraverse(c, _zeroCrossing.equation());
  if (cr) {
    if (_current == EVENT::Positive) {
      _current = EVENT::Negative;
      _type = EVENT::Zero;
    } else if (_current == EVENT::Negative) {
      _current = EVENT::Positive;
      _type = EVENT::Zero;
    }
  }
  return cr;
}

AST_Expression Event::getExpression(AST_Expression exp)
{
  if (exp->expressionType() == EXPOUTPUT) {
    return getExpression(AST_ListFirst(exp->getAsOutput()->expressionList()));
  }
  return exp;
}

string Event::handler(EVENT::Type type) const
{
  StatementTable stms = (type == EVENT::Positive ? _positiveHandler : _negativeHandler);
  if (stms.empty()) {
    return "";
  };
  stringstream buffer;
  string block = "";
  FunctionPrinter fp;
  string arguments;
  block += TAB;
  if (_range) {
    block = _range->block();
    arguments = _range->getDimensionVars();
  }
  block += TAB;
  buffer << fp.beginExpression(_zeroCrossing.identifier(), _range);
  buffer << fp.beginDimGuards(_zeroCrossing.identifier(), arguments, _range);
  StatementTable::iterator it;
  for (Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it)) {
    buffer << block << stm << endl;
  }
  buffer << block << fp.endDimGuards(_range);
  buffer << block << fp.endExpression(_range);
  return buffer.str();
}

string Event::macro() const { return _zeroCrossing.macro(); }

Expression Event::exp()
{
  assert(isValid());
  return _zeroCrossing.lhs();
}

string Event::config() const
{
  stringstream buffer;
  int direction = (_type == EVENT::Negative) ? -1 : _type;
  string tabs = "";
  if (_range) {
    buffer << _range.get();
    tabs = _range->block();
  }
  buffer << tabs << "modelData->event[" << _zeroCrossing.index() << "].direction = " << direction << ";" << endl;
  buffer << tabs << "modelData->event[" << _zeroCrossing.index() << "].relation = " << _zcRelation << ";" << endl;
  if (_range) {
    buffer << _range->end();
    tabs = _range->block();
  }
  return buffer.str();
}

}  // namespace IR
}  // namespace MicroModelica