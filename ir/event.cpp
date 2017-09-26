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

#include "../ast/expression.h"
#include "../util/ast_util.h"
#include "../util/dependencies.h"
#include "equation.h"
#include "expression.h"
#include "mmo_util.h"
#include "statement.h"

MMO_Event_::MMO_Event_(AST_Expression cond, MMO_ModelData data) :
    _positiveHandlerStatements(), _negativeHandlerStatements(),
        _init(0), _end(0), _handler(HND_ZERO), _handlerType(HND_ZERO), _data(
            data), _weight(-1), _zcRelation(ZC_GT)
{
  MMO_ConvertCondition_ cc(_data);
  _data->setCalculateAlgegraics(true);
  _cond = newMMO_Equation(cc.foldTraverse(_getExpression(cond)), _data);
  _data->setCalculateAlgegraics(false);
  if(cc.zeroCrossing() > 0)
  {
    _handler = HND_POSITIVE;
    _handlerType = HND_POSITIVE;
  }
  else
  {
    _handler = HND_NEGATIVE;
    _handlerType = HND_NEGATIVE;
  }
  if(cc.zeroCrossingRelation() == 0)
  {
    _zcRelation = ZC_LT;
  }
  else if(cc.zeroCrossingRelation() == 1)
  {
    _zcRelation = ZC_LE;
  }
  else if(cc.zeroCrossingRelation() == 2)
  {
    _zcRelation = ZC_GT;
  }
  else if(cc.zeroCrossingRelation() == 3)
  {
    _zcRelation = ZC_GE;
  }
  _weight = data->weight();
  _deps = newDependencies();
  _lhs = newDependencies();
  _index = data->lhs();
  _end = _index.hi();
  _init = _index.low();
  _data = data;
}

MMO_Event_::~MMO_Event_()
{
  delete _cond;
  delete _deps;
  delete _lhs;
}

MMO_Equation
MMO_Event_::condition()
{
  return _cond;
}

bool
MMO_Event_::hasWeight()
{
  return _weight >= 0;
}

double
MMO_Event_::weight()
{
  return _weight;
}

void
MMO_Event_::setCondition(MMO_Expression cond)
{
  _data->setCalculateAlgegraics(true);
  _cond = newMMO_Equation(cond, _data);
  _data->setCalculateAlgegraics(false);
}

/*! \brief Evaluates the eslewhen statement condition and sets the appropiate handler.
 *
 * 	\param cond: AST_Expression condition of the when statement.
 *  \return True if the elsewhen condition is the opposite condition of the when statement, False otherwise.
 *
 * 	Depscription:
 * 		Sets the handler of the event according to the condition expression, if the condition of the elsewhen statement 
 * 		is the opporite of the original event zero--crossing condition, otherwise we don't change anything.
 *
 */

bool
MMO_Event_::compareCondition(AST_Expression cond)
{
  MMO_ConvertCondition_ cc(_data);
  AST_Expression c = cc.foldTraverse(_getExpression(cond));
  EqualExp ee(_data->symbols());
  bool cr = ee.equalTraverse(c, _cond->exp()->exp());
  if(cr)
  {
    if(_handler == HND_POSITIVE)
    {
      _handler = HND_NEGATIVE;
      _handlerType = HND_ZERO;
    }
    else if(_handler == HND_NEGATIVE)
    {
      _handler = HND_POSITIVE;
      _handlerType = HND_ZERO;
    }
  }
  return cr;
}

void
MMO_Event_::insert(AST_Statement stm)
{
  _data->setWhenStatement(true);
  Index tmp = _data->lhs();
  _data->setLHS(_index);
  MMO_Statement s = newMMO_Statement(stm, _data);
  _data->setLHS(tmp);
  _data->setWhenStatement(false);
  _deps->join(s->deps());
  _lhs->join(s->lhs());
  if(_handler == HND_POSITIVE)
  {
    _positiveHandlerStatements.push_back(s);
  }
  else if(_handler == HND_NEGATIVE)
  {
    _negativeHandlerStatements.push_back(s);
  }
}

MMO_Statement
MMO_Event_::begin(HND_Type h)
{
  _handler = h;
  if(h == HND_POSITIVE)
  {
    _it = _positiveHandlerStatements.begin();
    return *_it;
  }
  else if(h == HND_NEGATIVE)
  {
    _it = _negativeHandlerStatements.begin();
    return *_it;
  }
  return NULL;
}

MMO_Statement
MMO_Event_::next()
{
  _it++;
  return *_it;
}

bool
MMO_Event_::end()
{
  if(_handler == HND_POSITIVE)
  {
    return _it == _positiveHandlerStatements.end();
  }
  else if(_handler == HND_NEGATIVE)
  {
    return _it == _negativeHandlerStatements.end();
  }
  return false;
}

Index
MMO_Event_::index()
{
  return _index;
}

void
MMO_Event_::setIndex(Index idx)
{
  _index = idx;
}

int
MMO_Event_::beginRange()
{
  return _init;
}

int
MMO_Event_::endRange()
{
  return _end;
}

HND_Type
MMO_Event_::handlerType()
{
  return _handlerType;
}

void
MMO_Event_::setHandlerType(HND_Type h)
{
  _handlerType = h;
}

string
MMO_Event_::print()
{
  string ret;
  return ret;
}

Dependencies
MMO_Event_::deps()
{
  return _deps;
}

Dependencies
MMO_Event_::lhs()
{
  return _lhs;
}

bool
MMO_Event_::hasPositiveHandler()
{
  return !_positiveHandlerStatements.empty();
}

bool
MMO_Event_::hasNegativeHandler()
{
  return !_negativeHandlerStatements.empty();
}

MMO_Event
newMMO_Event(AST_Expression cond, MMO_ModelData data)
{
  return new MMO_Event_(cond, data);
}

void
deleteMMO_Event(MMO_Event m)
{
  delete m;
}

ZC_REL
MMO_Event_::zcRelation()
{
  return _zcRelation;
}

AST_Expression
MMO_Event_::_getExpression(AST_Expression exp)
{
  if(exp->expressionType() == EXPOUTPUT)
  {
    return _getExpression(AST_ListFirst(exp->getAsOutput()->expressionList()));
  }
  return exp;
}
