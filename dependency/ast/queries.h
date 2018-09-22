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

#ifndef QUERIES_H
#define QUERIES_H

#include <ast/expression.h>
#include <util/table.h>
#include <util/debug.h>
#include <boost/variant/get.hpp>

namespace Modelica {

inline bool isZero(Expression e) {
  return e==Expression(Integer(0)) || e==Expression(Real(0.0));
}

inline bool isRelation(Expression e) {
  return is<BinOp>(e) && (
      boost::get<BinOp>(e).op()==Greater    ||
      boost::get<BinOp>(e).op()==GreaterEq  ||
      boost::get<BinOp>(e).op()==LowerEq    ||
      boost::get<BinOp>(e).op()==Lower      ||
      boost::get<BinOp>(e).op()==CompEq     ||
      boost::get<BinOp>(e).op()==CompNotEq 
      );
}

inline bool isOne(Expression e) {
  return e==Expression(Integer(1)) || e==Expression(Real(1.0));
}

inline bool isSum(Expression e) {
  BinOp b = boost::get<Modelica::AST::BinOp>(e);
  return is<BinOp>(e) && boost::get<BinOp>(e).op()==Add;
}

inline bool isBuiltIn(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  return var_info.get().builtin();
}

inline bool isRef(Expression e) {
  return is<Reference>(e);
}

inline bool isParameter(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  foreach_(Option<TypePrefix> t, var_info.get().prefixes()) 
    if (t && t.get() == parameter) 
      return true;
  return false;
}

inline bool isDiscrete(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  foreach_(Option<TypePrefix> t, var_info.get().prefixes()) {
    if (t && t.get() == discrete) 
      return true;
  }
  return false;
}

inline bool isConstant(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  foreach_(Option<TypePrefix> t, var_info.get().prefixes()) {
    if (t && t.get() == constant) 
      return true;
  }
  return false;
}

inline Name refName(Reference r) {
  return get<0>(r.ref().front());
}

inline bool isVariable(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  bool is_var=true;
  if (var_info.get().builtin())
    is_var = false;
  if (var_info.get().type()!="Real")
    is_var = false;
  foreach_(Option<TypePrefix> t, var_info.get().prefixes()) {
    if (t && t.get() == constant) 
      is_var = false;
    if (t && t.get() == parameter) 
      is_var = false;
    if (t && t.get() == discrete) 
      is_var = false;
  }
  return is_var;
}

inline bool isState(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info)
    ERROR("No symbol %s", n.c_str());
  return var_info.get().state();
}

inline bool isArray1(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  if (!var_info.get().indices())
    return false;
  return var_info.get().indices().get().size()==1;
}

inline Expression arraySize(Name n, const VarSymbolTable & syms) {
  Option<VarInfo> var_info = syms[n];
  if (!var_info) 
    ERROR("No symbol %s", n.c_str());
  return var_info.get().indices().get().front();
}



}

#endif
