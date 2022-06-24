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

Event::Event()
    : _zero_crossing(),
      _positive_handler(),
      _negative_handler(),
      _type(EVENT::Zero),
      _current(EVENT::Zero),
      _zc_relation(EVENT::GE),
      _range(),
      _positive_handler_id(0),
      _negative_handler_id(0),
      _id(0),
      _offset(0),
      _event_id()
{
}

Event::Event(AST_Expression cond, int id, int offset, Option<Range> range, string event_id)
    : _zero_crossing(),
      _positive_handler(),
      _negative_handler(),
      _type(EVENT::Zero),
      _current(EVENT::Zero),
      _zc_relation(EVENT::GE),
      _range(range),
      _positive_handler_id(0),
      _negative_handler_id(0),
      _id(id),
      _offset(offset),
      _event_id(event_id)
{
  ConvertCondition cc;
  _zero_crossing = Equation(cc.apply(getExpression(cond)), range, EQUATION::ZeroCrossing, id, offset);
  _type = cc.zeroCrossing();
  _current = _type;
  _zc_relation = cc.zeroCrossingRelation();
}

void Event::add(AST_Statement stm)
{
  Statement s(stm, _range);
  if (_current == EVENT::Positive) {
    _positive_handler.insert(_positive_handler_id++, s);
  } else if (_current == EVENT::Negative) {
    _negative_handler.insert(_negative_handler_id++, s);
  }
}

bool Event::compare(AST_Expression zc)
{
  ConvertCondition cc;
  AST_Expression c = cc.apply(getExpression(zc));
  EqualExp ee;
  bool cr = ee.equalTraverse(c, _zero_crossing.equation());
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
  StatementTable stms = (type == EVENT::Positive ? _positive_handler : _negative_handler);
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
    arguments = _range->getDimensionVarsString();
  }
  block += TAB;
  buffer << fp.beginExpression(_zero_crossing.identifier(), _range);
  buffer << fp.beginDimGuards(_zero_crossing.identifier(), arguments, _range);
  StatementTable::iterator it;
  for (Statement stm = stms.begin(it); !stms.end(it); stm = stms.next(it)) {
    buffer << block << stm << endl;
  }
  buffer << block << fp.endDimGuards(_range);
  buffer << block << fp.endExpression(_range);
  return buffer.str();
}

string Event::macro() const { return _zero_crossing.macro(); }

Expression Event::exp()
{
  assert(isValid());
  return _zero_crossing.lhs();
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
  buffer << tabs << "modelData->event[" << _zero_crossing.index() << "].direction = " << direction << ";" << endl;
  buffer << tabs << "modelData->event[" << _zero_crossing.index() << "].relation = " << _zc_relation << ";" << endl;
  if (_range) {
    buffer << _range->end();
    tabs = _range->block();
  }
  return buffer.str();
}

bool Event::compareEventID(std::string event_id) { cout << "EV " << _event_id << endl; return _event_id.compare(event_id) == 0; }

EquationTable zeroCrossingTable(EventTable events)
{
  EquationTable zero_crossings;
  EventTable::iterator ev_it;
  for (Event ev = events.begin(ev_it); !events.end(ev_it); ev = events.next(ev_it)) {
    zero_crossings.insert(ev.id(), ev.zeroCrossing());
  }
  return zero_crossings;
}

}  // namespace IR
}  // namespace MicroModelica
