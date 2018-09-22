
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

#include <string>
#include <map>
#include <assert.h>
#include <stdio.h>
#include <sstream>
#include <ginac/ginac.h>
#include <util/ast_visitors/ginac_interface.h>
#include <boost/tuple/tuple.hpp>
#include <util/debug.h>
#include <ast/queries.h>
#include <parser/parser.h>

using namespace GiNaC;
REGISTER_FUNCTION(der, dummy())
REGISTER_FUNCTION(pre, dummy())

static ex var_derivative(const ex & x,const ex & y, unsigned diff_param) {
  return der(x);
}
REGISTER_FUNCTION(var, derivative_func(var_derivative))


void my_print_power_dflt(const power & p, const print_dflt & c, unsigned level) ;

/*
void my_print_mul_dflt(const mul & m, const print_dflt & c, unsigned level) {
  // get the precedence of the 'power' class
  unsigned power_prec = m.precedence();
  if (level >= power_prec)
    c.s << '(';
  if (m.op(1).match(pow(wild(),-1))) {
    c.s << "(" << m.op(0) << "/" << m.op(1).op(0) << ")";
  } else if (m.op(0).match(pow(wild(),-1))) {
    c.s << "(" << m.op(1) << "/" << m.op(0).op(0) << ")";
  } else {
    c.s << m.op(0) << "*" << m.op(1);
  }
  if (level >= power_prec)
    c.s << ')';
}
void my_print_add_dflt(const add & s, const print_dflt & c, unsigned level) {
  // get the precedence of the 'power' class
  unsigned power_prec = s.precedence();
  if (level >= power_prec)
    c.s << '(';
  if (s.op(0).match(-wild()) && !s.op(1).match(-wild())) {
    c.s << s.op(1) << "-" << s.op(0).op(0);
  } else {
    c.s << s.op(0) << "+" << s.op(1);
  }
  if (level >= power_prec)
    c.s << ')';
}
*/
namespace Modelica {
  
    using namespace GiNaC;
ConvertToGiNaC::ConvertToGiNaC(VarSymbolTable  &var, bool forDerivation): varEnv(var),_forDerivation(forDerivation) {}

    GiNaC::ex ConvertToGiNaC::operator()(Integer v) const { 
      return ex(v);
    }
    GiNaC::ex ConvertToGiNaC::operator()(Boolean v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(String v) const {
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Name v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Real v) const { 
      return ex(v);
    }
    GiNaC::ex ConvertToGiNaC::operator()(SubEnd v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(SubAll v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(BinOp v) const { 
      Expression l=v.left(),r=v.right();
      switch (v.op()) {
        case Add:
          return ApplyThis(l)+ApplyThis(r);
        case Sub:
          return ApplyThis(l)-ApplyThis(r);
        case Mult:
          return ApplyThis(l)*ApplyThis(r);
        case Div:
          return ApplyThis(l)/ApplyThis(r);
        case Exp:
          return pow(ApplyThis(l),ApplyThis(r));
        default:
          WARNING("ConvertToGiNaC: BinOp conversion not implemented. Returning 0");
          return 0;
      }
    } 
    GiNaC::ex ConvertToGiNaC::operator()(UnaryOp v) const { 
      Expression e=v.exp();
      if (v.op()==Minus) 
        return -ApplyThis(e);
      WARNING("ConvertToGiNaC: Not conversion not implemented. Returning 0");
      return 0;
    } 
    GiNaC::ex ConvertToGiNaC::operator()(IfExp v) const { 
      WARNING("ConvertToGiNaC: IfExp conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Range v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Brace v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Bracket v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Call v) const { 
      if ("sin"==v.name()) {
        return sin(ApplyThis(v.args()[0]));
      } 
      if ("cos"==v.name()) {
        return cos(ApplyThis(v.args()[0]));
      } 
      if ("pre"==v.name()) {
        return ApplyThis(v.args()[0]);
      } 
      if ("der"==v.name()) {
        Expression arg = v.args().front();
        ERROR_UNLESS(is<Reference>(arg),"Argument to der operator is not a reference\n");
        Reference r = get<Reference>(arg);
        GiNaC::ex exp = ConvertToGiNaC::operator()(r);
        std::stringstream ss;
        ss << "der(" << exp << ")";
        return getSymbol(ss.str());
      } 
      if ("exp"==v.name()) {
        return exp(ApplyThis(v.args()[0]));
      } 
      if ("sum"==v.name()) {
        std::stringstream ss;
        ss << v;
        return getSymbol(ss.str());
      } 
      if ("log"==v.name()) {
        return log(ApplyThis(v.args()[0]));
      } 
      if ("log10"==v.name()) {
        return log(ApplyThis(v.args()[0]))/log(10);
      } 
      WARNING("ConvertToGiNaC: conversion of function %s implemented. Returning 0.\n", v.name().c_str());
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(FunctionExp v) const { 
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(ForExp v) const {
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Named v) const {
      WARNING("ConvertToGiNaC: conversion not implemented. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Output v) const {
      OptExpList el = v.args();
      ERROR_UNLESS(el.size()==1,"GiNaC conversion of output expression not implemented");
      if (el[0]) {
        return ApplyThis(el[0].get());
      }
      WARNING("ConvertToGiNaC: conversion of output expression. Returning 0");
      return 0;
    }
    GiNaC::ex ConvertToGiNaC::operator()(Reference v) const {
      Ref r=v.ref();
      ERROR_UNLESS(r.size()==1,"GiNaC conversion of dotted references not implemented");
      Option<ExpList> oel = boost::get<1>(r[0]);
      std::string s=boost::get<0>(r[0]);
      if (!_forDerivation || isConstant(s,varEnv) || isParameter(s,varEnv) || isDiscrete(s,varEnv)) {
        if (oel && oel.get().size()==1) {
          std::stringstream ss;
          ss << s << "[" <<  oel.get().front() << "]";
          return getSymbol(ss.str());
        } else if (oel && oel.get().size()>1) {
          ERROR("Multidimensional array not supported");
        } 
        return getSymbol(s);
      } else if (_forDerivation) {
          return var(getSymbol(s),ConvertToGiNaC::getTime());
      } else {
        ERROR("converting a parameter");
        return 0;
      }
    }

    GiNaC::symbol &ConvertToGiNaC::getSymbol(Name s) const {
          std::map<std::string, GiNaC::symbol>::iterator i = directory.find(s);
          if (i != directory.end())
            return i->second;
          else
            return directory.insert(make_pair(s, symbol(s))).first->second;
    }
    GiNaC::symbol &ConvertToGiNaC::getTime() const {
      std::string s="time";
      std::map<std::string, GiNaC::symbol>::iterator i = directory.find(s);
      if (i != directory.end())
        return i->second;
      return directory.insert(make_pair(s, symbol(s))).first->second;
    }
    
    Expression ConvertToExp(GiNaC::ex e) {
      bool r;
      std::stringstream s(std::ios_base::out),der_s(std::ios_base::out);
     // set_print_func<power,print_dflt>(my_print_power_dflt);
      s << e;
      Expression exp = Parser::ParseExpression(s.str(),r);
      if (!r) {
        WARNING("ConvertToGiNaC: conversion of output expression. Returning 0");
        return 0;
      } 
      return exp;
    }

}
