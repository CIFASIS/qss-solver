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

#ifndef AST_VISITOR_ST
#define AST_VISITOR_ST
#include <boost/variant/static_visitor.hpp>
#include <ast/statement.h>

namespace Modelica {
  using namespace Modelica::AST;
  template <typename Visit>
  class StatementVisitor: public boost::static_visitor<Statement> {
  public:
	#define applyExp(X) boost::apply_visitor(v,X)
	
    StatementVisitor(Visit visit): v(visit) {};
    
    Statement operator()(Assign st) const{ 
      Expression left=st.left();
      OptExp right=st.right();
      ExpList rl;
      
      if (st.rl()) {
		  foreach_(Expression &e, st.rl_ref().get())
			  rl.push_back(applyExp(e));
		  return Assign(applyExp(left), Option<ExpList>(rl));		
	  } else if (right) 
		  return Assign(applyExp(left), OptExp(applyExp(right.get())));
		else 
		   return Assign(applyExp(left));	
    };
       
    Statement operator()(Break st) const { 
      return st;
    };
    
    Statement operator()(Return st) const { 
      return st;
    }
    
    Statement operator()(CallSt st) const { 
		Expression n = st.n();
		ExpList list; 
		OptExpList out;
		foreach_(Expression s, st.arg())
			list.push_back(applyExp(s));
		
		foreach_(OptExp s, st.out())
			if (s) out.push_back(OptExp(applyExp(s.get())));
			else out.push_back(OptExp());
		  return CallSt(out,applyExp(n),list);	
    }
    
    Statement operator()(ForSt st) const { 
	  StatementList eqs;
	  IndexList index;
	  foreach_(Statement e,st.elements())
		eqs.push_back(ApplyThis(e));			
	  foreach_(Index i,st.range().indexes())	
		if (i.exp()) index.push_back(Index(i.name(), OptExp(applyExp(i.exp().get())))); 
		else index.push_back(Index(i.name(), OptExp())); 
      return ForSt(Indexes(index),eqs);
    }
    
    Statement operator()(IfSt st) const { 
		typedef tuple<Expression,std::vector<Statement> >  Else;
        typedef std::vector<Else> ElseList;
		Expression cond = st.cond();
		StatementList elements,elses;
		ElseList elseIf;
		
		foreach_(Statement e, st.elements() )
			elements.push_back(ApplyThis(e));
			
		foreach_(Statement e, st.ifnot() )
			elses.push_back(ApplyThis(e));	
			
		foreach_(Else el, st.elseif()) {
			StatementList list;
			foreach_(Statement s, get<1>(el))
				list.push_back(ApplyThis(s));
			elseIf.push_back(Else(applyExp(get<0>(el)),list));		
		}			
			
		return IfSt(applyExp(cond) , elements, elseIf,elses);
    }
    
    Statement operator()(WhenSt st) const{ 
		typedef tuple<Expression,std::vector<Statement> >  Else;
        typedef std::vector<Else> ElseList;
		Expression cond = st.cond();
		StatementList elements;
		ElseList elsewhen;
		
		foreach_(Statement e, st.elements() )
			elements.push_back(ApplyThis(e));
			
		foreach_(Else el, st.elsewhen()) {
			StatementList list;
			foreach_(Statement e, get<1>(el))
				list.push_back(ApplyThis(e));
			elsewhen.push_back(Else(applyExp(get<0>(el)),list));		
		}			
			
		return WhenSt(applyExp(cond) , elements, elsewhen);
    }
    
    Statement operator()(WhileSt st) const { 
		Expression cond = st.cond();
		StatementList elements;
		foreach_(Statement e, st.elements() )
			elements.push_back(ApplyThis(e));			
		return WhileSt(applyExp(cond) , elements);
    }
    
    Visit v;
  }; 
}
#endif 
