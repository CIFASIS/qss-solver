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

MMO_Event::MMO_Event(AST_Expression cond, MMO_ModelConfig &config) :
    _positiveHandlerStatements(), 
    _negativeHandlerStatements(),
    _init(0), 
    _end(0), 
    _handler(HND_ZERO), 
    _handlerType(HND_ZERO), 
    _config(config), 
    _weight(-1), 
    _zcRelation(ZC_GT)
{
  MMO_ConvertCondition cc(_config);
  _config->setCalculateAlgegraics(true);
  _cond = newMMO_Equation(cc.foldTraverse(_getExpression(cond)), _config);
  _config->setCalculateAlgegraics(false);
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
  _weight = config.weight();
}

MMO_Event::~MMO_Event()
{
}

MMO_Equation
MMO_Event::condition()
{
  return _cond;
}

bool
MMO_Event::hasWeight()
{
  return _weight >= 0;
}

double
MMO_Event::weight()
{
  return _weight;
}

void
MMO_Event::setCondition(MMO_Expression cond)
{
  _config->setCalculateAlgegraics(true);
  _cond = newMMO_Equation(cond, _config);
  _config->setCalculateAlgegraics(false);
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
MMO_Event::compareCondition(AST_Expression cond)
{
  MMO_ConvertCondition_ cc(_config);
  AST_Expression c = cc.foldTraverse(_getExpression(cond));
  EqualExp ee(_config->symbols());
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
MMO_Event::insert(AST_Statement stm)
{
  _config->setWhenStatement(true);
  Index tmp = _config->lhs();
  _config->setLHS(_index);
  MMO_Statement s = newMMO_Statement(stm, _config);
  _config->setLHS(tmp);
  _config->setWhenStatement(false);
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
MMO_Event::begin(HND_Type h)
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
MMO_Event::next()
{
  _it++;
  return *_it;
}

bool
MMO_Event::end()
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

HND_Type
MMO_Event::handlerType()
{
  return _handlerType;
}

void
MMO_Event::setHandlerType(HND_Type h)
{
  _handlerType = h;
}

string
MMO_Event::print()
{
  string ret;
  return ret;
}

bool
MMO_Event::hasPositiveHandler()
{
  return !_positiveHandlerStatements.empty();
}

bool
MMO_Event::hasNegativeHandler()
{
  return !_negativeHandlerStatements.empty();
}

ZC_REL
MMO_Event::zcRelation()
{
  return _zcRelation;
}

AST_Expression
MMO_Event::_getExpression(AST_Expression exp)
{
  if(exp->expressionType() == EXPOUTPUT)
  {
    return _getExpression(AST_ListFirst(exp->getAsOutput()->expressionList()));
  }
  return exp;
}
