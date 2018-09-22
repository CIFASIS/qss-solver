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


#include <ast/statement.h>

namespace Modelica {
  namespace AST {
  member_imp(StatementSection,bool,initial);
  member_imp(StatementSection,StatementList,statements);
  std::ostream& operator<<(std::ostream& out, const StatementSection &ss) {
    if (ss.statements().size()) { 
      if (ss.initial()) 
        out << "initial ";
      out << "algorithm\n";
    }
    BEGIN_BLOCK;
    foreach_(Statement s, ss.statements()) 
      out << INDENT<< s << ";\n";
    END_BLOCK;
    return out;
  }
  std::ostream& operator<<(std::ostream& out, const Assign &a) // output
  {
    if (a.right())
      out << a.left() << ":=" << a.right().get();
    else  if (a.rl()) {
      out << a.left() << "(";
      ExpList el = a.rl().get();
      int size=el.size(),i=0;
      foreach_ (Expression e, el) 
        out << e << (++i<size ? "," : "");
      out << ")";
    } else 
      out << a.left();
    return out;
  }
 

  std::ostream& operator<<(std::ostream& out, const CallSt &ce) {
    int l=ce.out().size(),i=0;
    if (l) {
      out << "(";
      foreach_(OptExp oe, ce.out()) {
        if (oe) {
            out << oe.get();
        }
        if (++i<l)
            out << ", ";
      }
      out << ") :=";
    }  
    l=ce.arg().size(),i=0;
    if (ce.arg().size()) {
    out << ce.n();
    out << "(";
    l=ce.arg().size();i=0;
    foreach_(Expression exp, ce.arg()) {
        out << exp;
        if (++i<l)
          out << ",";
    }
    out << ")";
    }
    return out;
  }
  member_imp(CallSt,OptExpList,out);
  member_imp(CallSt,Expression,n);
  member_imp(CallSt,ExpList,arg);
  CallSt::CallSt(OptExpList out, Expression name, ExpList args):out_(out), n_(name), arg_(args){
  };
 
  member_imp(Assign,Modelica::AST::Expression,left);
  member_imp(Assign,OptExp,right);
  member_imp(Assign,Option<ExpList>,rl);
  
  member_imp(WhileSt,StatementList,elements);
  member_imp(WhileSt,Expression,cond);
  
  std::ostream& operator<<(std::ostream& out, const WhileSt &st) // output
  {
    out << "while " << st.cond() << " loop\n";
    BEGIN_BLOCK;
    foreach_(Statement s, st.elements()) {
        out << INDENT<< s << ";\n";
    }
    END_BLOCK;
    out << INDENT << "end while";
    return out;
  }

  }
}
 
