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

#include <math.h>
#include <util/debug.h>
#include <util/ast_visitors/eval_expression.h>
#include <boost/variant/get.hpp>
#include <ast/modification.h>

namespace Modelica {

    using namespace boost;
    EvalExpression::EvalExpression(const VarSymbolTable &v):vtable(v) {};
    EvalExpression::EvalExpression(const VarSymbolTable &v,Name n, Real r):vtable(v),name(n),val(r) {};
    Real EvalExpression::operator()(Integer v) const { 
      return v;
    }
    Real EvalExpression::operator()(Boolean v) const { 
      if (v.val()) return 1.0;
      return 0.0;
    }
    Real EvalExpression::operator()(String v) const {
      ERROR("EvalExpression: trying to evaluate a String");
      return 0;
    }
    Real EvalExpression::operator()(Name v) const { 
      ERROR("EvalExpression: trying to evaluate a Name");
      return 0;
    }
    Real EvalExpression::operator()(Real v) const { 
      return v;
    }
    Real EvalExpression::operator()(SubAll v) const { 
      ERROR("EvalExpression: trying to evaluate a SubAll");
      return 0;
    }
    Real EvalExpression::operator()(SubEnd v) const { 
      ERROR("EvalExpression: trying to evaluate a SubEnd");
      return 0;
    }
    Real EvalExpression::operator()(BinOp v) const { 
      Expression l=v.left(), r=v.right();
      switch (v.op()) {
        case Add:   
          return ApplyThis(l)+ApplyThis(r);
        case Sub:   
          return ApplyThis(l)-ApplyThis(r);
        case Div:   
          return ApplyThis(l)/ApplyThis(r);
        case Mult:   
          return ApplyThis(l)*ApplyThis(r);
        case Exp:   
          return pow(ApplyThis(l),ApplyThis(r));
        default:
          ERROR("EvalExpression: BinOp %s not supported.", BinOpTypeName[v.op()]);
          return 0;
      }
    } 
    Real EvalExpression::operator()(UnaryOp v) const { 
      if (v.op()==Minus) {
        Expression e=v.exp();
        return -ApplyThis(e);
      } else if (v.op()==Plus) {
		Expression e=v.exp();
        return ApplyThis(e);  
	  }
      ERROR("EvalExpression: trying to evaluate a UnaryOp");
      return 0;
    } 
    Real EvalExpression::operator()(IfExp v) const { 
      ERROR("EvalExpression: trying to evaluate a IfExp");
      return 0;
    }
    Real EvalExpression::operator()(Range v) const { 	
      ERROR("EvalExpression: trying to evaluate a Range");
      return 0;
    }
    Real EvalExpression::operator()(Brace v) const { 
      WARNING("EvalExpression: trying to evaluate a Brace");
      return 0;
    }
    Real EvalExpression::operator()(Bracket v) const { 
      ERROR("EvalExpression: trying to evaluate a Bracket");
      return 0;
    }
    Real EvalExpression::operator()(Call v) const { 
      if ("integer"==v.name()) {
        return ApplyThis(v.args().front());
      }
      if ("exp"==v.name()) {
        return exp(ApplyThis(v.args().front()));
      }
      ERROR("EvalExpression: trying to evaluate a Call");
      return 0;
    }
    Real EvalExpression::operator()(FunctionExp v) const { 
      ERROR("EvalExpression: trying to evaluate a FunctionExp");
      return 0;
    }
    Real EvalExpression::operator()(ForExp v) const {
      ERROR("EvalExpression: trying to evaluate a ForExp");
      return 0;
    }
    Real EvalExpression::operator()(Named v) const {
      ERROR("EvalExpression: trying to evaluate a Named");
      return 0;
    }
    Real EvalExpression::operator()(Output v) const {
      ERROR_UNLESS(v.args().size()==1, "EvalExpression: Output expression with more than one element are not supported");
      if (v.args().front()) {
        Expression e=v.args().front().get();
        return ApplyThis(e);
      }
      ERROR("EvalExpression: Output with no expression");
      return 0;
    }
    Real EvalExpression::operator()(Reference v) const {
      Ref r=v.ref();
      ERROR_UNLESS(r.size()==1,"EvalExpression: conversion of dotted references not implemented");
      Option<ExpList> oel = boost::get<1>(r[0]);
      ERROR_UNLESS((bool)oel,"EvalExpression: conversion of subscripted references not implemented");
      Name s=boost::get<0>(r[0]);
      
      if (name && name.get() == s) return val.get();
			      
      Option<VarInfo> vinfo = vtable[s];
      if (!vinfo)
        ERROR("EvalExpression: Variable %s not found !", s.c_str());
      if (!vinfo.get().modification()) {
        ERROR("EvalExpression: Variable %s without initial value!", s.c_str());
      } 
      Modification m=vinfo.get().modification().get();
      if (is<ModEq>(m)) {
        Expression meq = boost::get<ModEq>(m).exp();
        return ApplyThis(meq);
      }
      if (is<ModAssign>(m)) {
        Expression meq = boost::get<ModAssign>(m).exp();
        return ApplyThis(meq);
      } 
      if (is<ModClass>(m)) {
        OptExp oe = get<ModClass>(m).exp();
        if (oe) {
          return ApplyThis(oe.get());
        }
        foreach_(Argument a, get<ModClass>(m).modification_ref()) {
          if (is<ElMod>(a)) {     
            ElMod em = boost::get<ElMod>(a);
            if (em.name()=="start" && (em.modification()) && is<ModEq>(em.modification_ref().get())) {
              Expression e = get<ModEq>(em.modification_ref().get()).exp();
              return ApplyThis(e);
            }
          }
        } 
      }
      std::cerr << m << "\n";
      ERROR("EvalExpression: cannot evaluate class modification");
      return 0;

        
    }
}
