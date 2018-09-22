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

#ifndef AST_VISITOR_EQ
#define AST_VISITOR_EQ
#include <boost/variant/static_visitor.hpp>
#include <ast/equation.h>

#define applyExp(X) boost::apply_visitor(v,X)
namespace Modelica {

  using namespace Modelica::AST;
  template <typename Visit>
  class EquationVisitor: public boost::static_visitor<Equation> {
  public:
    EquationVisitor(Visit visit): v(visit) {

    };
    Equation operator()(Connect eq) const {
      Expression left=eq.left();
      Expression right=eq.right();
      return Connect(applyExp(left),applyExp(right));
    };    
    Equation operator()(Equality eq) const {
      Expression left=eq.left();
      Expression right=eq.right();
      return Equality(applyExp(left),applyExp(right));
    };
    Equation operator()(CallEq eq) const {
	    ExpList list;
  	  foreach_(Expression e, eq.args())
	  	list.push_back(applyExp(e));	
        return CallEq(eq.name(),list);
    };
    Equation operator()(ForEq eq) const {
	  EquationList eqs;
	  IndexList index;
	  foreach_(Equation e,eq.elements())
		eqs.push_back(ApplyThis(e));			
	  foreach_(Index i,eq.range().indexes())	
		if (i.exp()) index.push_back(Index(i.name(), OptExp(applyExp(i.exp().get())))); 
		else index.push_back(Index(i.name(), OptExp())); 
      return ForEq(Indexes(index),eqs);
    };
    Equation operator()(IfEq eq) const {
		typedef tuple<Expression,std::vector<Equation> >  Else;
        typedef std::vector<Else> ElseList;
		Expression cond = eq.cond();
		EquationList elements,elses;
		ElseList elseIf;
		
		foreach_(Equation e, eq.elements() )
			elements.push_back(ApplyThis(e));
			
		foreach_(Equation e, eq.ifnot() )
			elses.push_back(ApplyThis(e));	
			
		foreach_(Else el, eq.elseif()) {
			EquationList list;
			foreach_(Equation e, get<1>(el))
				list.push_back(ApplyThis(e));
			elseIf.push_back(Else(applyExp(get<0>(el)),list));		
		}			
			
		return IfEq(applyExp(cond) , elements, elseIf,elses);
    };
    Equation operator()(WhenEq eq) const {
		typedef tuple<Expression,std::vector<Equation> >  Else;
        typedef std::vector<Else> ElseList;
		Expression cond = eq.cond();
		EquationList elements;
		ElseList elsewhen;
		
		foreach_(Equation e, eq.elements() )
			elements.push_back(ApplyThis(e));
			
		foreach_(Else el, eq.elsewhen()) {
			EquationList list;
			foreach_(Equation e, get<1>(el))
				list.push_back(ApplyThis(e));
			elsewhen.push_back(Else(applyExp(get<0>(el)),list));		
		}			
			
		return WhenEq(applyExp(cond) , elements, elsewhen);

    };
    Visit v;
  }; 
}
#endif 
