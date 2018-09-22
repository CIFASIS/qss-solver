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

#include <util/ast_visitors/mark_connector.h>
#include <boost/variant/get.hpp>

using namespace Modelica;
using namespace Modelica::AST;

MarkConnector::MarkConnector(){};

Expression MarkConnector::mark(Expression exp) const{
	if (is<UnaryOp>(exp)) return exp;
	else if (is<Reference>(exp)) {
		Reference reference = boost::get<Reference>(exp);
		Ref refs = reference.ref();
		if (refs.size() > 1 ) return UnaryOp(exp,Plus);
		else return UnaryOp(exp,Minus);
	} else {
		std::cout << "Error marcando Connect. No deberia llegar [" << exp << "]" << std::endl; 
		return exp;
	}
}

Equation MarkConnector::operator()(Connect eq) const {
	Expression left=eq.left();
	Expression right=eq.right();
	return Connect(mark(left),mark(right));
};    

Equation MarkConnector::operator()(Equality eq) const {
	return eq;
};
    
Equation MarkConnector::operator()(CallEq eq) const {
	return eq;
};

Equation MarkConnector::operator()(ForEq eq) const {
	EquationList eqs;
	IndexList index;
	foreach_(Equation e,eq.elements())
		eqs.push_back(ApplyThis(e));	
	return ForEq(eq.range(),eqs);
};

Equation MarkConnector::operator()(IfEq eq) const {
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
		elseIf.push_back(Else(get<0>(el),list));		
	}			
	return IfEq(cond, elements, elseIf,elses);
};

Equation MarkConnector::operator()(WhenEq eq) const {
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
		elsewhen.push_back(Else(get<0>(el),list));		
	}			
	return WhenEq(cond, elements, elsewhen);
};

 
