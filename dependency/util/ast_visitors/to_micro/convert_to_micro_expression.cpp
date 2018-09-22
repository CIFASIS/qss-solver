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

#include <util/ast_visitors/to_micro/convert_to_micro_expression.h>
#include <ast/queries.h>
#include <stdio.h>
#include <map>
#include <sstream>

namespace Modelica {

    using namespace boost;
    ConvertToMicroExpression::ConvertToMicroExpression(MMO_Class &cl, unsigned int &discont, bool w, bool in_alg): mmo_class(cl), 
                                                disc_count(discont), when(w), in_algorithm(in_alg) {
    };
    Expression ConvertToMicroExpression::operator()(Integer v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Boolean v) const { 
      if (v==TRUE) return 1.0;
      return 0.0;
    }
    Expression ConvertToMicroExpression::operator()(String v) const {
      WARNING("uModelica does not supports strings\n");
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Name v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Real v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(SubEnd v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(SubAll v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(BinOp v) const { 
      if (in_algorithm) 
        return v;
      if (isRelation(v)) { 
        ExpList::iterator it = std::find(rels.begin(),rels.end(),Expression(v));
        v.left_ref()=ApplyThis(v.left_ref());
        v.right_ref()=ApplyThis(v.right_ref());
        if (it == rels.end()) {
          Expression d=newDiscrete();
          WhenSt::ElseList l(1,WhenSt::Else(negate(v),StatementList(1,Assign(d,Expression(0)))));
          WhenSt st(Expression(v),StatementList(1,Assign(d,Expression(1.0))),l);
          IfSt::ElseList el;
          IfSt ifst(Expression(v),StatementList(1,Assign(d,Expression(1.0))),el ,StatementList(1,Assign(d,Expression(0.0)))); 
          mmo_class.addInitStatement(ifst);
          mmo_class.statements_ref().statements_ref().push_back(st);
          rels.push_back(Expression(v));
          disc.push_back(d);
          return Call("pre",d);
        } else {
          it =disc.begin()+(it-rels.begin());
          return Call("pre",*it);
        }
      } else if (v.op()==And) {
        Expression l=v.left(), r=v.right();
        return BinOp(ApplyThis(l), Mult, ApplyThis(r));
      } else if (v.op()==Or) {
        Expression l=v.left(), r=v.right();
        return Output(BinOp(1,Sub,BinOp(Output(BinOp(1,Sub,ApplyThis(l))),Mult,Output(BinOp(1,Sub,ApplyThis(r))))));
      }
      Expression l=v.left(), r=v.right();
      return BinOp(ApplyThis(l), v.op(), ApplyThis(r));
    } 
    Expression ConvertToMicroExpression::operator()(UnaryOp v) const { 
      if (v.op()==Not) {
        return Output(BinOp(1,Sub,ApplyThis(v.exp_ref())));
      } 
      return UnaryOp(ApplyThis(v.exp_ref()), v.op());
    } 
    Expression ConvertToMicroExpression::operator()(IfExp v) const { 
      if (in_algorithm) 
        return v;
      Expression then = ApplyThis(v.then_ref());
      Expression elseexp = ApplyThis(v.elseexp_ref());
      Expression cond = ApplyThis(v.cond_ref());
      // Check if there is already a disc variable
      ExpList::iterator it = std::find(rels.begin(),rels.end(),cond);
      if (it != rels.end()) {
          it =disc.begin()+(it-rels.begin());
          return Call("pre",*it);
      }
      if (is<Reference>(cond)) {
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList l(1,WhenSt::Else(BinOp(cond,Lower,0.5),StatementList(1,Assign(d,Expression(0)))));
        WhenSt st(BinOp(cond,Greater,0.5),StatementList(1,Assign(d,Expression(1.0))),l);
        mmo_class.statements_ref().statements_ref().push_back(st);
        IfSt::ElseList el;
        IfSt ifst(BinOp(cond,Greater,0.5),StatementList(1,Assign(d,Expression(1.0))),el ,StatementList(1,Assign(d,Expression(0.0)))); 
        mmo_class.addInitStatement(ifst);
        rels.push_back(cond);
        cond=Call("pre",d);
        disc.push_back(d);
      } else if (is<BinOp>(cond)) {
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList l(1,WhenSt::Else(negate(get<BinOp>(cond)),StatementList(1,Assign(d,Expression(0)))));
        WhenSt st(cond,StatementList(1,Assign(d,Expression(1.0))),l);
        mmo_class.statements_ref().statements_ref().push_back(st);
        rels.push_back(cond);
        disc.push_back(d);
      }
      return BinOp(BinOp(cond,Mult, Output(then)), Add, BinOp(Output(BinOp(1,Sub,cond)),Mult,Output(elseexp)));
    }
    Expression ConvertToMicroExpression::operator()(Range v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Brace v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Bracket v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Call v) const { 
      if (in_algorithm) 
        return v;
      if ("abs"==v.name()) {
        ERROR_UNLESS (v.args().size()==1, "Call to abs with more than one or zero arguments");
        Expression arg = ApplyThis(v.args_ref().front());
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList l(1,WhenSt::Else(BinOp(arg,Lower,0) ,StatementList(1,Assign(d,Expression(UnaryOp(arg,Minus)))))); 
        WhenSt st(BinOp(arg,Greater,0),StatementList(1,Assign(d,arg)),l);
        mmo_class.statements_ref().statements_ref().push_back(st);
        return Call("pre",d);
      } else if ("sign"==v.name()) {
        ERROR_UNLESS (v.args().size()==1, "Call to sign with more than one or less arguments");
        Expression l=v.args().front();
        l=ApplyThis(l);
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList el(1,WhenSt::Else(BinOp(l,LowerEq,0) ,StatementList(1,Assign(d,Expression(UnaryOp(1,Minus))))));
        WhenSt st(BinOp(l,Greater,0),StatementList(1,Assign(d,Expression(1))),el);
        mmo_class.statements_ref().statements_ref().push_back(st);
        return Call("pre",d);
      } else if ("min"==v.name()) {
        ERROR_UNLESS (v.args().size()==2, "Call to min with more than two or less arguments");
        Expression l=v.args().front();
        Expression r=v.args().at(1);
        l=ApplyThis(l);
        r=ApplyThis(r);
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList el(1,WhenSt::Else(BinOp(l,GreaterEq,r) ,StatementList(1,Assign(d,r))));
        WhenSt st(BinOp(l,Lower,r),StatementList(1,Assign(d,l)),el);
        mmo_class.statements_ref().statements_ref().push_back(st);
        return Call("pre",d);
      } else if ("max"==v.name()) {
        ERROR_UNLESS (v.args().size()==2, "Call to max with more than two or less arguments");
        Expression l=v.args().front();
        Expression r=v.args().at(1);
        l=ApplyThis(l);
        r=ApplyThis(r);
        Expression d=newDiscrete();
        //Expression d = Call("pre",ExpList(1,newDiscrete()));
        WhenSt::ElseList el(1,WhenSt::Else(BinOp(l,LowerEq,r) ,StatementList(1,Assign(d,r))));
        WhenSt st(BinOp(l,Greater,r),StatementList(1,Assign(d,l)),el);
        mmo_class.statements_ref().statements_ref().push_back(st);
        return Call("pre",d);
      } else if ("noevent"==v.name()) {
        ERROR_UNLESS (v.args().size()==1, "Call to noevent with more than one or zero arguments");
        return v.args().front();
      } else if ("edge"==v.name()) {
        ERROR_UNLESS (v.args().size()==1, "Call to edge with more than one or zero arguments");
        return BinOp(ApplyThis(v.args_ref().front()),Greater,Expression(0.5));
      } else if ("sample"==v.name()) {
        ERROR_UNLESS (v.args().size()==2, "Call to sample with more than two or less arguments");
        if (!when)
          ERROR("Call to sample in dynamic equation");
        // This is a hack to return multiple expressions in one expression
        Expression start = ApplyThis(v.args().front());
        Expression period = ApplyThis(v.args().at(1));
        Expression d=newDiscrete(start);
        Expression cond = BinOp(Reference("time"),Greater,Call("pre",ExpList(1,d)));
        ExpList el;
        el.push_back(cond);
        el.push_back(d);
        el.push_back(BinOp(d,Add,period));
        return Brace(el);
      } else if ("integer"==v.name()) { // Evaluating integer function
        return v.args().front();
        
      }
      return v;
    }
    Expression ConvertToMicroExpression::operator()(FunctionExp v) const { 
      return v;
    }
    Expression ConvertToMicroExpression::operator()(ForExp v) const {
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Named v) const {
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Output v) const {
      foreach_ (OptExp &oe, v.args_ref())
        if (oe)
          oe=ApplyThis(oe.get());
      if (v.args().size()==1 && v.args().front() && is<Reference>(v.args().front().get()))
        return v.args().front().get();
      return v;
    }
    Expression ConvertToMicroExpression::operator()(Reference v) const {
      return v;
    }
    Expression ConvertToMicroExpression::newDiscrete(Option<Expression> oe) const {
        std::stringstream disc_name;
        disc_name << "d" << disc_count++;
        Name name(disc_name.str());
        VarSymbolTable &syms=mmo_class.syms_ref();     
        if (oe) {
          ClassModification cm;
          cm.push_back(ElMod("start",ModEq(oe.get())));
          ModClass mc(cm);
          Modification m = mc;
          syms.insert(name,VarInfo(TypePrefixes(1,discrete) , "Real", Option<Comment>(), m));
        }  else
          syms.insert(name,VarInfo(TypePrefixes(1,discrete) , "Real"));
        mmo_class.variables_ref().push_back(name);
        return Reference(Ref(1,RefTuple(name,ExpList())));
    
    }
    Expression ConvertToMicroExpression::negate(BinOp v) const {
      if (isRelation(v)) { 
        switch (v.op()) {
          case Lower: 
            v.op_ref()=GreaterEq;
            break;
          case LowerEq:
            v.op_ref()=Greater;
            break;
          case GreaterEq: 
            v.op_ref()=Lower;
            break;
          case Greater: 
            v.op_ref()=LowerEq;
            break;
          case CompEq:
            v.op_ref()= CompNotEq;
            break;
          case CompNotEq:
            v.op_ref()= CompEq;
            break;
          default:
            ERROR("Trying to negate an expression that is not a relation");
        }
        return v;
      } else ERROR("Trying to negate an expression that is not a relation");
      return 0;    
    }
}
