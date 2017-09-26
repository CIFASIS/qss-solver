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

#include "mmo_event.h"

#include "mmo_visitor.h"

MMOEvent::MMOEvent(string id, bool generateInitialAsignment) :
    _id(), _zero_crossing(), _condition(), _handler_pos(), _handler_neg(), _generateInitialAsignment(
        generateInitialAsignment), _initialAlgorithm(
        false)
{
  _id = id;
}

MMOEvent::~MMOEvent()
{
}

void
MMOEvent::accept(MMOVisitor *visitor)
{
  if(_initialAlgorithm && !_generateInitialAsignment)
  {
    return;
  }
  visitor->visit(this);
  visitor->visit(&_zero_crossing);
  if(_condition.isCondition())
  {
    visitor->visit(&_condition);
  }
  for(std::list<MMODecl>::iterator it = _handler_pos.begin();
      it != _handler_pos.end(); ++it)
  {
    visitor->visit(*it);
  }
  for(std::list<MMODecl>::iterator it = _handler_neg.begin();
      it != _handler_neg.end(); ++it)
  {
    visitor->visit(*it);
  }
  visitor->leave(this);
}

void
MMOEvent::add(MMODecl decl)
{
  if(decl.isZeroCrossing())
  {
    _zero_crossing = decl;
  }
  else if(decl.isCondition())
  {
    _condition = decl;
  }
}

void
MMOEvent::add(MMODecl decl, MMOHandlerType type)
{
  if(type == positive)
  {
    _handler_pos.push_back(decl);
  }
  else
  {
    _handler_neg.push_back(decl);
  }
}

bool
MMOEvent::generateInitialAsignments()
{
  return _generateInitialAsignment;
}

void
MMOEvent::setInitialAlgorithm(bool ia)
{
  _initialAlgorithm = ia;
}

bool
MMOEvent::initialAlgorithm()
{
  return _initialAlgorithm;
}
