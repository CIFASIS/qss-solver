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

#include <util/ast_visitors/to_micro/convert_to_micro.h>
#include <util/ast_visitors/to_micro/convert_to_micro_statement.h>
#include <ast/queries.h>
#include <util/ast_visitors/partial_eval_expression.h>

namespace Modelica {
  ConvertToMicro::ConvertToMicro(MMO_Class &cl): mmo_class(cl), tomicro_exp(cl,disc_count) {
    disc_count=0;
  } 
  void ConvertToMicro::convert() {
    EquationList &eqs = mmo_class.equations_ref().equations_ref();
    EquationList remove;
    foreach_(Equation &eq, eqs) {
      if (is<Equality>(eq)) {
        eq = ApplyThis(eq);
      } else if (is<Connect>(eq)) {
        ERROR("Cannot convert non-flat models. There are connect equations!");
      } else if (is<IfEq>(eq)) {
        ERROR("If equation not supported yet!");
      } else if (is<WhenEq>(eq)) {
        ApplyThis(eq);
        remove.push_back(eq);
      } else if (is<CallEq>(eq)) {
        if (get<CallEq>(eq).name()=="assert")
          remove.push_back(eq);
      } else if (is<ForEq>(eq)) {
        ForEq feq = get<ForEq>(eq);
        Option<Expression> oe = feq.range().indexes().front().exp();
        if (oe) {
          PartialEvalExpression ev(mmo_class.syms_ref());
          Expression exp = Apply(ev,oe.get());
          if (is<Range>(exp)) {
            Range range = get<Range>(exp);
            if (is<Integer>(range.end()) && is<Integer>(range.start()) && (get<Integer>(range.end()) < get<Integer>(range.start()))) { // Removing empty for eq
              remove.push_back(eq);
            }
          }
        }
      }

    }
    foreach_(Equation eq, remove) {
      eqs.erase(std::find(eqs.begin(),eqs.end(), eq));
    }
    // Remove initial equations
    EquationList &init_eqs = mmo_class.initial_eqs_ref().equations_ref();
    remove.clear();
    foreach_(Equation &eq, init_eqs) {
      if (is<Equality>(eq)) {
        Equality eq_eq = get<Equality>(eq);
        mmo_class.initial_sts_ref().statements_ref().push_back(Assign(eq_eq.left(),eq_eq.right()));
        remove.push_back(eq);
      } else {
        WARNING("Skiping initial equation");
      }
    }
    foreach_(Equation eq, remove) {
      init_eqs.erase(std::find(init_eqs.begin(),init_eqs.end(), eq));
    }
    ConvertToMicroStatement tom(mmo_class,disc_count);
    foreach_(Statement &st, mmo_class.statements_ref().statements_ref()) {
      st=Apply(tom, st);
    }
    // Add new statements
    foreach_(Statement st, tom.statements()) {
      mmo_class.statements_ref().statements_ref().push_back(st);
    }
    foreach_(Name var, mmo_class.variables()) {
      Option<VarInfo> opt_vinfo = mmo_class.syms_ref()[var];
      ERROR_UNLESS((bool)opt_vinfo, "No variable info for %s", var.c_str());
      VarInfo vinfo = opt_vinfo.get();  
      if (vinfo.type()=="Boolean" || vinfo.type()=="Integer") {
        //if ((vinfo.type()=="Boolean" && !isParameter(var,mmo_class.syms_ref())) || (vinfo.type()=="Integer" && !isParameter(var,mmo_class.syms_ref()) && !isConstant(var,mmo_class.syms_ref()))) 
        //  vinfo.prefixes_ref().push_back(discrete);
        if (vinfo.type()=="Boolean" && vinfo.modification()) {
          Modification &mod = boost::get<Modification>(vinfo.modification_ref());
          //ConvertToMicroExpression exp(mmo_class,disc_count);
          if (is<ModEq>(mod)) {
            get<ModEq>(mod).exp_ref()=Apply(tomicro_exp,get<ModEq>(mod).exp_ref());
          } else if (is<ModAssign>(mod)) {
            get<ModEq>(mod).exp_ref()=Apply(tomicro_exp,get<ModAssign>(mod).exp_ref());
          } else {
            ModClass &mc = get<ModClass>(mod);
            if (mc.exp())
              mc.exp_ref()=Apply(tomicro_exp, mc.exp_ref().get());
            foreach_(Argument &arg, mc.modification_ref()) {
              if (is<ElMod>(arg) && get<ElMod>(arg).name()=="start" && get<ElMod>(arg).modification()) {
                ERROR_UNLESS(is<ModEq>(get<ElMod>(arg).modification_ref().get()), "Modification for start value not supported");
                Expression start = get<ModEq>(get<ElMod>(arg).modification_ref().get()).exp();
                get<ElMod>(arg).modification_ref() = Modification(ModEq(Apply(tomicro_exp,start)));
              }
            }
          } 
        }
        if ( (!isConstant(var,mmo_class.syms_ref()) && !isParameter(var,mmo_class.syms_ref())) ||
            (vinfo.type()=="Boolean"))
          vinfo.type_ref()="Real";
      } 
      if (vinfo.modification()) {
        Modification &mod = boost::get<Modification>(vinfo.modification_ref());
        if (is<ModEq>(mod)) {
          Expression exp=get<ModEq>(mod).exp();
          //ConvertToMicroExpression exp_visit(mmo_class,disc_count);
          get<ModEq>(mod).exp_ref()=Apply(tomicro_exp,exp);
          if (is<Brace>(exp)) {
            DEBUG('m', "Removing brace modification");
//            vinfo.modification_ref()=Option<Modification>();
            ModClass mc;
            ClassModification cm;
            ElMod em("fixed",ModEq(Boolean(FALSE)));
            em.each_ref()=true;
            cm.push_back(em);
            mc.modification_ref()=cm;
            vinfo.modification_ref()=Option<Modification>(mc);
            StatementList sl;
            int index=1;
            foreach_(Expression e, get<Brace>(exp).args()) {
              sl.push_back(Assign(Reference(Ref(1,RefTuple(var,ExpList(1,index++)))),e));
            }
            mmo_class.initial_sts_ref().statements_ref() += sl;
            TypePrefixes tp;
            bool was_const=false;
            foreach_(Option<TypePrefix> otp, vinfo.prefixes()) { // Remove constant
              if (otp && (otp.get()==constant || otp.get()==parameter)) {
                was_const=true;
              }
              if (otp && otp.get()!=constant && otp.get()!=parameter)
                tp.push_back(otp);
            }
            if (was_const)
              tp.push_back(parameter);
            vinfo.prefixes_ref() = tp;
          } else if (is<Reference>(exp)) {
            Reference ref = get<Reference>(exp);
            if (ref.ref().size()==1) {
              Name n =  get<0>(ref.ref().front());
              ExpList exp_l =  get<1>(ref.ref().front());
              if (exp_l.size()==1) {
                Expression ind = exp_l.front(); 
                if (is<Range>(ind)) {
                  ForSt fs (Indexes(IndexList(1,Index("i", ind))), StatementList(1,Assign(Reference(Ref(1,RefTuple(var, ExpList(1,Reference("i"))))), OptExp(Reference(Ref(1,RefTuple(n,ExpList(1,Reference("i")))))))));
                  ModClass mc;
                  ClassModification cm;
                  ElMod em("fixed",ModEq(Boolean(FALSE)));
                  em.each_ref()=true;
                  cm.push_back(em);
                  mc.modification_ref()=cm;
                  vinfo.modification_ref()=Option<Modification>(mc);
                  mmo_class.initial_sts_ref().statements_ref().push_back(fs);
                }
              }
            }
          }

  
        }
      }
 
      mmo_class.syms_ref().insert(var,vinfo);
    }    
    // Convert everything to a model
    mmo_class.prefixes_ref().clear(); 
    mmo_class.prefixes_ref().push_back(model);
  }
  Equation ConvertToMicro::operator() (Connect c) {
    ERROR("Can not convert connect equation. ");
    return c;
  }
  Equation ConvertToMicro::operator() (Equality eqeq) {
    //ConvertToMicroExpression exp(mmo_class,disc_count);
    return Equality(Apply(tomicro_exp, eqeq.left_ref()), Apply(tomicro_exp,eqeq.right_ref()));
  }
  Equation ConvertToMicro::operator() (CallEq c) {
    return c;
  }
  Equation ConvertToMicro::operator() (WhenEq c) { 
    //ConvertToMicroExpression exp(mmo_class,disc_count,true);
    Expression cond = Apply(tomicro_exp,c.cond_ref());
    StatementList sls;
    if (is<Brace>(cond)) {
      // This is sample 
      ExpList el = get<Brace>(cond).args();
      c.cond_ref()=el.front();
      sls.push_back(Assign(el.at(1),el.at(2)));
    } else
      c.cond_ref()=cond;
    if (c.elsewhen().size())
      ERROR("Elsewhen not supported yet");
    foreach_ (Equation e, c.elements()) {
      if (is<Equality>(e)) {
        Equality eq=boost::get<Equality>(e);
        sls.push_back(Assign(eq.left(),eq.right()));
      } else if (is<CallEq>(e)) {
        const CallEq &ce = boost::get<CallEq>(e);
        if (ce.name()=="reinit") {
          sls.push_back(CallSt(OptExpList(),"reinit", ce.args())); 
        } else {
          ERROR("Conversion of call equations inside when is not supported yet");
        }
      } else {
        ERROR("Conversion of non-equality equations inside when is not supported yet");
      }
    }
    WhenSt st(c.cond(), sls, WhenSt::ElseList(0));
    mmo_class.statements_ref().statements_ref().push_back(st);
    return c;
  }
  Equation ConvertToMicro::operator() (ForEq c) {
    return c;
  }
  Equation ConvertToMicro::operator() (IfEq c) {
    return c;
  }
}
