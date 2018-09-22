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

/*
 * findstate.cpp
 *
 *  Created on: 28/04/2013
 *      Author: Moya
 */

#include <iostream>
#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>

#include <causalize/state_variables_finder.h>
#include <ast/expression.h>
#include <util/table.h>
#include <util/debug.h>
#include <cassert>

StateVariablesFinder::StateVariablesFinder(MMO_Class &c): _c(c) { }

void StateVariablesFinder::findStateVariables() {
  foreach_(Equation e, _c.equations().equations()) {
    if (is<Equality>(e)) {
      Equality eq=boost::get<Equality>(e);
      Expression left=eq.left(),right=eq.right();
      ApplyThis(left);
      ApplyThis(right);
    } else if (is<ForEq>(e)) {
      ForEq fe = boost::get<ForEq>(e);    
      foreach_(Equation feq,fe.elements()) {
        if (is<Equality>(feq)) {
          Equality eq=boost::get<Equality>(feq);
          Expression left=eq.left(),right=eq.right();
          ApplyThis(left);
          ApplyThis(right);
        } else {
          ERROR("StateVariablesFinder::findStateVariablesInEquations:\n" "Equation type not supported.\n");
        } 
      } 
    } else {
      ERROR("StateVariablesFinder::findStateVariablesInEquations:\n" "Equation type not supported.\n");
    }

  } 
}

    void StateVariablesFinder::operator()(Integer v) const { return ; } 
    void StateVariablesFinder::operator()(Boolean v) const { return ; } 
    void StateVariablesFinder::operator()(String v) const { return ; } 
    void StateVariablesFinder::operator()(Name v) const { return ; }
    void StateVariablesFinder::operator()(Real v) const { return ; } 
    void StateVariablesFinder::operator()(SubEnd v) const { return ; } 
    void StateVariablesFinder::operator()(SubAll v) const { return ; } 
    void StateVariablesFinder::operator()(BinOp v) const { 
      Expression l=v.left(), r=v.right();
      ApplyThis(l);
      ApplyThis(r);
    } 
    void StateVariablesFinder::operator()(UnaryOp v) const { 
      Expression e=v.exp();
      ApplyThis(e);
    } 
    void StateVariablesFinder::operator()(IfExp v) const { 
      Expression cond=v.cond(), then=v.then(), elseexp=v.elseexp();
      ApplyThis(cond);
      ApplyThis(then);
      ApplyThis(elseexp);
    }
    void StateVariablesFinder::operator()(Range v) const { 
      Expression start=v.start(), end=v.end();
      ApplyThis(start);
      ApplyThis(end);
    }
    void StateVariablesFinder::operator()(Brace v) const { 
      abort();
    }
    void StateVariablesFinder::operator()(Bracket v) const { 
      abort();
    }
    void StateVariablesFinder::operator()(Call v) const { 
      if (v.name()=="der") {
        ERROR_UNLESS(v.args().size()==1, "Call to der() with zero or more than one argument!");
        Expression e = v.args().front();
        ERROR_UNLESS(is<Reference>(e), "Argument to der is not a reference!");
        Reference r = boost::get<Reference>(e);
        ERROR_UNLESS(r.ref().size()==1, "StateVariablesFinder: dotted expression not supported!");
        RefTuple rt = r.ref().front();
        Name name= boost::get<0>(rt);
        VarSymbolTable &vars = _c.syms_ref();
        Option<VarInfo> opt_vinfo = vars[name];
        ERROR_UNLESS((bool)opt_vinfo, "StateVariablesFinder: Variable %s not found", name.c_str());
        VarInfo vinfo = opt_vinfo.get();
        vinfo.state_ref()=true;
        vars.insert(name,vinfo);
      } else {
        foreach_ (Expression e, v.args()) 
          ApplyThis(e);
      }
    }
    void StateVariablesFinder::operator()(FunctionExp v) const { 
      return;
    }
    void StateVariablesFinder::operator()(ForExp v) const {
      return;
    }
    void StateVariablesFinder::operator()(Named v) const {
      return;
    }
    void StateVariablesFinder::operator()(Output v) const {
      foreach_(OptExp oe, v.args()) {
        if (oe)
          ApplyThis(oe.get());
      }
    }
    void StateVariablesFinder::operator()(Reference v) const {
      return;
    }
