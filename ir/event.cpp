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

namespace MicroModelica {
  using namespace Util;
  namespace IR {

    Event::Event(AST_Expression cond, VarSymbolTable& symbols) : 
      _zeroCrossing(),
      _positiveHandler(),
      _negativeHandler(),
      _type(EVENT::Zero),
      _current(EVENT::Zero),
      _zcRelation(EVENT::GE),
      _symbols(symbols)
    {
      ConvertCondition cc;
      _zeroCrossing = Equation(cc.apply(getExpression(cond)), symbols, EQUATION::ZeroCrossing);
      _type = cc.zeroCrossing();
      _current = _type;
      _zcRelation = cc.zeroCrossingRelation();
    }

    void 
    Event::add(AST_Statement stm)
    {
      Statement s(stm, _symbols);
      if(_current == EVENT::Positive)
      {
        _positiveHandler.insert(_positiveHandlerId++, s);
      }
      else if(_current == EVENT::Negative)
      {
        _negativeHandler.insert(_negativeHandlerId++, s);
      }
    }

    bool 
    Event::compare(AST_Expression zc)
    {
      ConvertCondition cc;
      AST_Expression c = cc.apply(getExpression(zc));
      EqualExp ee(_symbols);
      bool cr = ee.equalTraverse(c, _zeroCrossing.equation());
      if(cr)
      {
        if(_current == EVENT::Positive)
        {
          _current = EVENT::Negative;
          _type = EVENT::Zero;
        }
        else if(_current == EVENT::Negative)
        {
          _current = EVENT::Positive;
          _type = EVENT::Zero;
        }
      }
      return cr;
    }

    AST_Expression
    Event::getExpression(AST_Expression exp)
    {
      if(exp->expressionType() == EXPOUTPUT)
      {
        return getExpression(AST_ListFirst(exp->getAsOutput()->expressionList()));
      }
      return exp;
    }

    std::ostream& operator<<(std::ostream& out, const Event& e)
    {
      return out;
    }
  }
}
