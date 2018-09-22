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

#include <util/ast_visitors/constant_expression.h>

namespace Modelica {

    using namespace boost;
    ConstantExpression::ConstantExpression() {};
    bool ConstantExpression::operator()(Integer v) const { 
      return true;
    }
    
    bool ConstantExpression::operator()(Boolean v) const { 
      return true;
    }
    
    bool ConstantExpression::operator()(String v) const {
      return true;
    }
    
    bool ConstantExpression::operator()(Name v) const { 
      return true;
    }
    
    bool ConstantExpression::operator()(Real v) const { 
      return true;
    }
    
    bool ConstantExpression::operator()(SubEnd v) const { 
      return false;
    }
    
    bool ConstantExpression::operator()(SubAll v) const { 
      return false;
    }
    
    bool ConstantExpression::operator()(BinOp v) const { 
      Expression l=v.left(), r=v.right();
      return ApplyThis(l) && ApplyThis(r);
    }
     
    bool ConstantExpression::operator()(UnaryOp v) const { 
      Expression e =v.exp();
      return ApplyThis(e);
    } 
    
    bool ConstantExpression::operator()(IfExp v) const { 
      Expression cond = v.cond();
      Expression then = v.then();
      Expression elseexp = v.elseexp();
      bool list = true;
      foreach_(ExpPair p, v.elseif())
		list = list &&  ApplyThis(get<0>(p)) && ApplyThis(get<1>(p));
      return ApplyThis(cond) && ApplyThis(then) && list && ApplyThis(elseexp);
    }
    
    bool ConstantExpression::operator()(Range v) const { 
	  Expression start = v.start(),end=v.end();	
	  if (v.step()) {
		Expression step = v.step().get();  
		return ApplyThis(start) && ApplyThis(step) && ApplyThis(end);
	  } else 
		return ApplyThis(start) && ApplyThis(end);
    }
    
    bool ConstantExpression::operator()(Brace v) const { 
      bool list = true;
      foreach_(Expression e, v.args())
		list = list && ApplyThis(e);
      return list;
    }
    
    bool ConstantExpression::operator()(Bracket v) const { 
	  bool list = true;
	  foreach_(ExpList els, v.args()) 
		  foreach_(Expression e, els)
			list  &= ApplyThis(e);
      return list;
    }
    
    bool ConstantExpression::operator()(Call v) const { 
      bool list = true;
      foreach_(Expression e, v.args())
		list &= ApplyThis(e);
      return list;
    }
    
    bool ConstantExpression::operator()(FunctionExp v) const { 
      bool list = false;
      foreach_(Expression e, v.args())
		list &= ApplyThis(e);
      return list;
    }
    
    bool ConstantExpression::operator()(ForExp v) const {
      Expression exp = v.exp();
      bool indices = true;
      foreach_(Index i, v.indices().indexes()) {
        if (i.exp())
		      indices &= ApplyThis(i.exp().get());
      } 
      return indices;
    }
    
    bool ConstantExpression::operator()(Named v) const {
	  Expression exp = v.exp();	
      return ApplyThis(exp);
    }
    
    bool ConstantExpression::operator()(Output v) const {
      bool list = true;
      foreach_(OptExp e, v.args())
	     if (e)	
			list &= ApplyThis(e.get()); 
      return list;
    }
    
    bool ConstantExpression::operator()(Reference v) const {
		return false;
    }
}
