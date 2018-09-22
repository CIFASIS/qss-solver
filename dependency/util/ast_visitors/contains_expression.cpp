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

#include <util/debug.h>
#include <util/ast_visitors/contains_expression.h>
#include <boost/variant/get.hpp>

namespace Modelica {
    ContainsExpression::ContainsExpression(Expression e): exp(e) {};
    bool ContainsExpression::operator()(Integer v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(Boolean v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(String v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(Name v) const { return exp==Expression(v); }
    bool ContainsExpression::operator()(Real v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(SubEnd v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(SubAll v) const { return exp==Expression(v); } 
    bool ContainsExpression::operator()(BinOp v) const { 
      if (exp==Expression(v)) return true; 
      Expression l=v.left(), r=v.right();
      bool rl = ApplyThis(l);
      bool ll = ApplyThis(r);
      return rl || ll;
    } 
    bool ContainsExpression::operator()(UnaryOp v) const { 
      if (exp==Expression(v)) return true; 
      Expression e=v.exp();
      return ApplyThis(e);
    } 
    bool ContainsExpression::operator()(IfExp v) const { 
      if (exp==Expression(v)) return true; 
      Expression cond=v.cond(), then=v.then(), elseexp=v.elseexp();
      const bool rc = ApplyThis(cond);
      const bool rt = ApplyThis(then);
      const bool re = ApplyThis(elseexp);
      return rc || rt || re;
    }
    bool ContainsExpression::operator()(Range v) const { 
      if (exp==Expression(v)) return true; 
      Expression start=v.start(), end=v.end();
      bool rs = ApplyThis(start);
      bool re = ApplyThis(end);
      return rs || re;
    }
    bool ContainsExpression::operator()(Brace v) const { 
      if (exp==Expression(v)) return true; 
      ERROR("ContainsExpression: Brace expression not supported");
      //TODO
      return false;
    }
    bool ContainsExpression::operator()(Bracket v) const { 
      if (exp==Expression(v)) return true; 
      ERROR("ContainsExpression: Bracket expression not supported");
      //TODO
      return false;
    }
    bool ContainsExpression::operator()(Call v) const { 
      if (exp==Expression(v)) return true; 
      foreach_ (Expression e, v.args()) 
        if (ApplyThis(e)) return true;
      return false;
    }
    bool ContainsExpression::operator()(FunctionExp v) const { 
      if (exp==Expression(v)) return true; 
      //TODO
      return false;
    }
    bool ContainsExpression::operator()(ForExp v) const {
      if (exp==Expression(v)) return true; 
      ERROR("ContainsExpression: For expression not supported");
      //TODO
      return false;
    }
    bool ContainsExpression::operator()(Named v) const {
      if (exp==Expression(v)) return true; 
      //TODO
      return false;
    }
    bool ContainsExpression::operator()(Output v) const {
      if (exp==Expression(v)) return true; 
      foreach_ (OptExp oe, v.args()) 
        if (oe && ApplyThis(oe.get())) return true;
      return false;
    }
    bool ContainsExpression::operator()(Reference v) const {
      if (exp==Expression(v)) return true; 
      if (is<Reference>(exp)) {
        Reference ref = get<Reference>(exp);
        // Check wheter a[1] is contained in a
        if (get<0>(v.ref().front())==get<0>(ref.ref().front()) && get<1>(v.ref().front()).size()==0)
          return true;
      }
      return false;
    }
}
