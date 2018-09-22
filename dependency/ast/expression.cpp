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


#include <boost/foreach.hpp>
#include <boost/variant/get.hpp>
#include <ast/expression.h>
#include <util/debug.h>

long depth;
namespace Modelica {
  namespace AST {

    const char *BinOpTypeName[] =  {" or ", " and ", "<", "<=", ">", ">=", "<>", "==", "+", ".+", "-", ".-", "/", "./","*", ".*", "^", ".^" };
    const char *UnaryOpTypeName[] =  {" not ", "-", "+" };
    
    // String
    String::String(std::string s): val_(s) {}
    String::String(): val_("") {}
    String::String(std::vector<boost::optional<char> > ss): val_("") {
      foreach_(boost::optional<char> c, ss) 
        val_ += (c ? std::string(1,c.get()) : "\\\"");
    }
    bool String::operator==(const String & other) const {
      return (other.val()==val());
    }
    std::ostream& operator<<(std::ostream& out, const String &s) // output
    {
      out << "\"" << s.val() << "\"";
      return out;
    }
    member_imp(String,std::string,val);
  
    Boolean::Boolean (Bool b): val_(b==TRUE ? true : false) { }
    member_imp(Boolean,bool,val);
    std::ostream& operator<<(std::ostream& out, const Boolean &b) // output
    {
      out << (b.val() ? " true " : " false ");
      return out;
    }
    bool Boolean::operator==(const Boolean & other) const {
        return (other.val()==val());
    }

    BinOp::BinOp(Expression l, BinOpType op, Expression r): left_(l),right_(r), op_(op) {}

    bool BinOp::operator==(const BinOp & other) const {
      return (other.op()==op() && other.left()==left() && other.right()==right());
    }

    std::ostream& operator<<(std::ostream& out, const BinOp  &b) // output
    {
      if (b.op()==Exp) {
        
        if (printAsCActive (out))
            out << "pow(" << b.left() << ", " << b.right() << ")";
        else
            out << "(" << b.left() << "^" << b.right() << ")";
        return out;
      }
      out << b.left() << BinOpTypeName[b.op()] << b.right();
      return out;
    }
    
    member_imp(BinOp,Expression,left);
    member_imp(BinOp,Expression,right);
    member_imp(BinOp,BinOpType,op);
    
    IfExp::IfExp () {};
    IfExp::IfExp (Expression a, Expression b, std::vector<fusion::vector2<Expression, Expression> > elseif, Expression c): cond_(a), then_(b), elseexp_(c) {
      typedef fusion::vector2<Expression,Expression> P;
      foreach_ (P f, elseif) 
        elseif_.push_back(ExpPair(get<0>(f),get<1>(f)));
    };
    IfExp::IfExp (Expression a, Expression b, List<ExpPair> elseif, Expression c): cond_(a), then_(b), elseif_(elseif), elseexp_(c) {}
    bool IfExp::operator==(const IfExp & other) const {
      return other.cond()==cond() && other.then()==then() && other.elseexp() == elseexp();
    }
    std::ostream& operator<<(std::ostream& out, const IfExp & f) // output
    {
      out << "if " << f.cond() << " then " << f.then();
      foreach_(ExpPair p, f.elseif()) 
        out << " elseif " << get<0>(p) << " then " << get<1>(p);
      out << " else " << f.elseexp();
      return out;
    }
    member_imp(IfExp,Expression,cond);
    member_imp(IfExp,Expression,then);
    member_imp(IfExp,Expression,elseexp);
    member_imp(IfExp,List<ExpPair>,elseif);

    Call:: Call(Name n, Expression arg): name_(n) { Call::args_.push_back(arg);};
    Call:: Call(Name n, ExpList args): name_(n), args_(args) {};
    std::ostream& operator<<(std::ostream& out, const Call & c) // output
    {
      out << c.name() << "(" ;
      int l=c.args().size(),i=0;
      foreach_(Expression e, c.args()) {
        out << e;
        if (++i<l)
          out << ",";
      }
      out << ")";
      return out;
    }
    bool Call::operator==(const Call & other) const {
      return other.name()==name() && other.args()==args(); 
    }
    member_imp(Call,Name,name);
    member_imp(Call,ExpList,args);

     Brace::Brace(Expression arg) { Brace::args_.push_back(arg);};
     Brace::Brace(ExpList args): args_(args) {};
     std::ostream& operator<<(std::ostream& out, const Brace & c) // output
     {
       out << "{" ;
       int l=c.args().size(),i=0;
       foreach_(Expression e, c.args()) {
         out << e;
         if (++i<l)
           out << ",";
       }
       out << "}";
       return out;
     }
      bool Brace::operator==(const Brace & other) const {
        return other.args()==args(); 
      }
      member_imp(Brace,ExpList,args)

     Bracket::Bracket(ExpListList args): args_(args) {};
     std::ostream& operator<<(std::ostream& out, const Bracket & c) // output
     {
       out << "[" ;
       int l=c.args().size(),i=0;
       foreach_(ExpList le, c.args()) {
         int ll=le.size(),ii=0;
         foreach_(Expression e, le) {
           out << e;
           if (++ii<ll)
             out << ",";
         }
         if (++i<l)
           out << ";";
       }
       out << "]";
       return out;
     }
     bool Bracket::operator==(const Bracket & other) const {
       return other.args()==args(); 
     }
     member_imp(Bracket,ExpListList,args)



    UnaryOp::UnaryOp(Expression e, UnaryOpType op): exp_(e), op_(op) {};
    std::ostream& operator<<(std::ostream& out, const UnaryOp &n) // output
    {
      if (n.op()==Minus)
        out << "(" <<  UnaryOpTypeName[n.op()] << n.exp() << ")";
      else if (n.op()==Plus)
        out << n.exp();
      else
        out << UnaryOpTypeName[n.op()] << n.exp();
      return out;
    }
    bool UnaryOp::operator==(const UnaryOp & other) const {
      return other.exp()==exp() && other.op() == op();
    }
    member_imp(UnaryOp,Expression,exp)
    member_imp(UnaryOp,UnaryOpType,op)

    ForExp::ForExp(Expression e,Indexes ind): indices_(ind), exp_(e) {};
    std::ostream& operator<<(std::ostream& out, const ForExp &n) // output
    {
      out << n.exp() << " for " << n.indices();
      return out;
    }
    bool ForExp::operator==(const ForExp & other) const {
      return other.indices()==indices();
    }
    member_imp(ForExp, Indexes, indices);
    member_imp(ForExp, Expression, exp);

    Reference::Reference(Ref r): ref_(r) { }
    Reference::Reference(Name n): ref_(Ref(1,RefTuple(n,ExpList(0)))) { }
    Reference::Reference(Name n, ExpList is): ref_(Ref(1,RefTuple(n,is))) { }
    Reference::Reference(Name n, Expression i): ref_(Ref(1,RefTuple(n,ExpList(1, i)))) { }

    Reference::Reference(Reference other, Name cr, Option<ExpList> indices) {
      ref_=other.ref();
      if (indices) 
        ref_.push_back(RefTuple(cr,indices.get()));
      else
        ref_.push_back(RefTuple(cr,ExpList()));
    }
    Reference::Reference(Option<Name> dot, Name cr, Option<ExpList> indices) {
      if (dot)
        cr = "." + cr;
      if (indices) 
        ref_.push_back(RefTuple(cr,indices.get()));
     else 
        ref_.push_back(RefTuple(cr,ExpList()));
    }

    std::ostream& operator<<(std::ostream& out, const Reference &r) // output
    {
      int l=r.ref().size(),i=0;
      foreach_(RefTuple t, r.ref()) {
        out << get<0>(t);
        if (get<1>(t).size()) {
          out << "[";
          int ll=get<1>(t).size(), ii=0;
          foreach_(const Expression e, get<1>(t)) {
            out << e;
            if (++ii<ll)
              out << ",";
          }
          out << "]";
        };
        if (++i<l)
          out << ".";
      }
      return out;
    }
    bool Reference::operator==(const Reference & other) const {
      return other.ref()==ref();
    }
    member_imp(Reference,Ref,ref);
      
    Range::Range (Expression s, Expression e): start_(s), end_(e) {};
    Range::Range (Expression s, Expression i, Expression e): start_(s),  step_(i), end_(e) {};
 
    std::ostream& operator<<(std::ostream& out, const Range &r) 
    {
      out << r.start() << ":";
      if (r.step()) 
        out << r.step().get() << ":";
      out << r.end();
      return out;
    }
    bool Range::operator==(const Range & other) const {
      if (other.step()) {
        if (!step())
          return false;
        if (!(other.step().get()==step().get())) 
          return false;
      } else {
        if (step())
          return false;
      }
      return other.start()==start() && other.end()==end();
    }
    member_imp(Range,Expression,start);
    member_imp(Range,Expression,end);
    member_imp(Range,OptExp,step);

    Output::Output (Expression e) { args_.push_back(e); }
    Output::Output (OptExpList l): args_(l) {};
 
    std::ostream& operator<<(std::ostream& out, const Output &o) 
    {
      int l=o.args().size(),i=0;
      if (l==1 && o.args().front() && is<Output>(o.args().front().get())) {
            out << o.args().front().get();
            return out;
      }
      out << "(";
      foreach_(OptExp e, o.args()) {
        if (e) 
          out << e.get();
        if (++i<l)
          out << ",";
      }
      out << ")";
      return out;
    }
    bool Output::operator==(const Output & other) const {
      return other.args()==args(); 
    }
      
    member_imp(Output, OptExpList,args) ;
 
    bool SubEnd::operator==(const SubEnd & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const SubEnd &s) { 
      out << "end";
      return out; 
    };
 
    bool SubAll::operator==(const SubAll & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const SubAll &s) { 
      out << ":";
      return out; 
    };
 
    Named::Named(Name n, Expression e):  name_(n), exp_() {};
    std::ostream& operator<<(std::ostream& out, const Named &n) {
      out << n.name() << "=" << n.exp() ;
        return out;
    };
    bool Named::operator==(const Named & other) const {
      return other.exp()==exp() && other.name()==name(); 
    };
    member_imp(Named,Expression,exp);
    member_imp(Named,Name,name);
 
    Index::Index(Name n, OptExp e): exp_(e), name_(n) {};
    std::ostream& operator<<(std::ostream& out, const Index &n) {
      out << n.name();
      if (n.exp())
        out << " in " << n.exp().get() ;
      return out;
    };
    bool Index::operator==(const Index & other) const {
      return other.exp()==exp() && other.name()==name(); 
    };
    std::ostream& operator<<(std::ostream& out, const Indexes &n) {
      int size=n.indexes().size(),ii=0;
      foreach_(Index i, n.indexes()) {
        out << i;
        if (++ii<size)
          out << ",";
      } 
      return out;
    };
    member_imp(Indexes,IndexList, indexes);
    bool Indexes::operator==(const Indexes & other) const {
      return other.indexes()==indexes();
    };
 
    member_imp(Index,OptExp,exp);
    member_imp(Index,Name,name);
 
   FunctionExp:: FunctionExp(Name n, ExpList args): name_(n), args_(args) {};
   std::ostream& operator<<(std::ostream& out, const FunctionExp & c) // output
   {
     out << "function " << c.name() << "(" ;
     out << c.name() << "(" ;
     int l=c.args().size(),i=0;
     foreach_(Expression e, c.args()) {
        out << e;
        if (++i<l)
          out << ",";
     }
     out << ")";
     return out;
   }
   bool FunctionExp::operator==(const FunctionExp & other) const {
     return other.name()==name() && other.args()==args(); 
   }
 
   member_imp(FunctionExp,Name,name);
   member_imp(FunctionExp,ExpList,args);
   Boolean True(TRUE);
   Boolean False(FALSE);

   long & printAsC(std::ios_base& s) {
    static int my_index = std::ios_base::xalloc();
    return s.iword(my_index);
   }

    long printAsCActive (std::ios_base& s) {
        return printAsC(s);
    }

    void setCFlag(std::ios_base& s, long n) {
        printAsC(s) = n;
    } 


  }
}

