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
#include <util/ast_visitors/partial_eval_expression.h>
#include <util/ast_visitors/eval_expression.h>
#include <ast/queries.h>

namespace Modelica {

    using namespace boost;
    PartialEvalExpression::PartialEvalExpression(const VarSymbolTable &v, bool eval):vtable(v),eval_parameters(eval) {};
    Expression PartialEvalExpression::operator()(Integer v) const { 
      if (v<0) return Output(v);
      return v;
    }
    Expression PartialEvalExpression::operator()(Real v) const { 
      if (v<0) return Output(v);
      return v;
    }
    Expression PartialEvalExpression::operator()(Boolean v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(String v) const {
      return v;
    }
    Expression PartialEvalExpression::operator()(Name v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(Expression v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(SubAll v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(SubEnd v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(BinOp v) const { 
      Expression l=v.left(), r=v.right();
      l=ApplyThis(l);r=ApplyThis(r);
      if ((is<Real>(l) || is<Integer>(l)) && (is<Real>(r) || is<Integer>(r))) {
        Expression binop = BinOp(l,v.op(),r);
        Expression e = Apply(EvalExpression(vtable),binop);
        if (!is<Real>(l) && !is<Real>(r) && is<Real>(e)) {
          return Integer(get<Real>(e));
        }
        return Apply(EvalExpression(vtable),binop);
      }
      if (v.op()==Add && isZero(l)) 
        return r;
      if (v.op()==Add && isZero(r)) 
        return l;
      if (v.op()==Sub && isZero(r)) 
        return l;
      if (v.op()==Sub && isZero(l)) 
        return UnaryOp(r,Minus);
      if (v.op()==Mult && isOne(l)) 
        return r;
      if (v.op()==Mult && isOne(r)) 
        return l;
      if (v.op()==Mult && (isZero(r) || isZero(l)))
        return 0;
      return BinOp(l,v.op(),r);
    } 
    Expression PartialEvalExpression::operator()(UnaryOp v) const { 
      Expression e=v.exp();
      Expression res= ApplyThis(e);
      if (v.op()==Not && is<Boolean>(res)) {
        if (get<Boolean>(res)==TRUE) 
          return Boolean(FALSE);  
        return Boolean(TRUE);  
      }
      if (v.op()==Minus && is<Real>(res)) {
        return Real(-get<Real>(res));
      }
      if (v.op()==Minus && is<Integer>(res))
        return Integer(-get<Integer>(res));
      if (v.op()==Minus && is<UnaryOp>(res) && get<UnaryOp>(res).op()==Minus)// (-(-a)) == a
        return get<UnaryOp>(res).exp();
      return UnaryOp(res, v.op());
    } 
    Expression PartialEvalExpression::operator()(IfExp v) const { 
      WARNING("Not evaluating If exp");
      return v;
    }
    Expression PartialEvalExpression::operator()(Range v) const { 
      Expression s=v.start(); 
      Expression e=v.end(); 
      if (!v.step()) 
        return Range(ApplyThis(s),ApplyThis(e));
      Expression st=v.step().get();
      return Range(ApplyThis(s),ApplyThis(st),ApplyThis(e));
    }
    Expression PartialEvalExpression::operator()(Brace v) const { 
      WARNING("Not evaluating brace exp");
      return v;
    }
    Expression PartialEvalExpression::operator()(Bracket v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(Call v) const { 
      ExpList &args = v.args_ref();
      foreach_ (Expression &e, args) 
        e = ApplyThis(e);
      return v;
    }
    Expression PartialEvalExpression::operator()(FunctionExp v) const { 
      return v;
    }
    Expression PartialEvalExpression::operator()(ForExp v) const {
      return v;
    }
    Expression PartialEvalExpression::operator()(Named v) const {
      return v;
    }
    Expression PartialEvalExpression::operator()(Output v) const {
      if (v.args().size()==1 && v.args().front()) {
        Expression e=v.args().front().get();
        if (is<Real>(e)) return e;
        if (is<Integer>(e)) return e;
        if (is<UnaryOp>(e)) return ApplyThis(e);
        return Output(ApplyThis(e));
      }
      return v;
    }
    Expression PartialEvalExpression::operator()(Reference v) const {
      Ref r=v.ref();
      ERROR_UNLESS(r.size()==1,"PartialEvalExpression conversion of dotted references not implemented");
      Option<ExpList> oel = boost::get<1>(r[0]);
      Name s=boost::get<0>(r[0]);
      if ((isConstant(s,vtable) || isParameter(s,vtable)) && (!oel || (oel && oel.get().size()==0))) {
        Option<VarInfo> vinfo = vtable[s];
        if (!vinfo) 
          ERROR("Variable %s not found",s.c_str());
        if (vinfo.get().type()=="Integer" && vinfo.get().modification()) { // evaluate integer parameters
          Expression vv=v;
          Real ret=Apply(EvalExpression(vtable),vv);
          const int i=ret;
          if (i==ret) {
            if (i<0) return Output(i);
            return i;
          }
          if (ret<0) return Output(Real(ret));
          return Real(ret);
        } else if (vinfo.get().type()=="Boolean" && vinfo.get().modification()) { // evaluate boolean parameters
          Expression vv=v;
          Real ret=Apply(EvalExpression(vtable),vv);
          if (ret==1.0)    
            return Boolean(TRUE);
          return Boolean(FALSE);
        } else if (eval_parameters && vinfo.get().type()=="Real" && !vinfo.get().indices() && vinfo.get().modification()) { // evaluate scalar parameters
          Expression vv=v;
          Real ret=Apply(EvalExpression(vtable),vv);
          if (ret<0) return Output(Real(ret));
          return Real(ret);
        } else {
          return v;
        }
      }
      if (oel) {
        ExpList nl;
        foreach_(Expression e, oel.get()) {
          nl.push_back(ApplyThis(e));
        }
        return Ref(1,RefTuple(s,nl));
      }
      return v;
    }
}
