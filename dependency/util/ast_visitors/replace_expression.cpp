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

#include <ast/queries.h>
#include <util/ast_visitors/replace_expression.h>

namespace Modelica {

    using namespace boost;
    ReplaceExpression::ReplaceExpression(Expression l, Expression r): look(l), rep(r), check_indexes(true) {};
    Expression ReplaceExpression::operator()(Integer v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Boolean v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(String v) const {
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Name v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Real v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(SubEnd v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(SubAll v) const { 
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(BinOp v) const { 
      if (look==Expression(v))
        return rep; 
      Expression l=v.left(), r=v.right();
      return BinOp(ApplyThis(l), v.op(), ApplyThis(r));
    } 
    Expression ReplaceExpression::operator()(UnaryOp v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      Expression exp = v.exp();  
      return UnaryOp(ApplyThis(exp),v.op());
    } 
    Expression ReplaceExpression::operator()(IfExp v) const { 
      if (look==Expression(v))
        return rep; 
      Expression c=v.cond(),then=v.then(),elseexp=v.elseexp();
      return IfExp(ApplyThis(c), ApplyThis(then), List<ExpPair>(),ApplyThis(elseexp));
    }
    Expression ReplaceExpression::operator()(Range v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Brace v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Bracket v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Call v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      foreach_ (Expression &e, v.args_ref()) 
        e = ApplyThis(e);
      return v;
    }
    Expression ReplaceExpression::operator()(FunctionExp v) const { 
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(ForExp v) const {
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Named v) const {
      // TODO
      if (look==Expression(v))
        return rep; 
      return v;
    }
    Expression ReplaceExpression::operator()(Output v) const {
      if (look==Expression(v))
        return rep; 
      foreach_ (OptExp &oe, v.args_ref()) {
        if (oe)    
          oe = ApplyThis(oe.get());
      }
      return v;
    }
    Expression ReplaceExpression::operator()(Reference v) const {
      // TODO
      if (look==Expression(v))
        return rep; 
      if (!check_indexes && is<Reference>(look) && is<Reference>(rep)) {
        Reference rl = get<Reference>(look);
        Reference rrep = get<Reference>(rep);
        if (refName(rl)==refName(v)) {
          get<0>(v.ref_ref().front()) = refName(rrep);
        }
      }
      Ref aux;  
      foreach_(RefTuple rt, v.ref()) {
		  Name n = get<0>(rt);
		  ExpList exps;
		  foreach_(Expression e, get<1>(rt))
			exps.push_back(ApplyThis(e));
		  aux.push_back(RefTuple(n,exps));
	  }
      return Reference(aux);
    }
}
