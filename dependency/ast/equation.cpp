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


#include <ast/equation.h>

namespace Modelica {
  namespace AST {
  Equality::Equality(Expression l, Expression r): left_(l), right_(r){};
  member_imp(Equality,Expression,left);
  member_imp(Equality,Expression,right);
  std::ostream& operator<<(std::ostream& out, const Equality &e) // output
  {
    out << e.left() << " = " << e.right() ;
    return out;
  }
  bool Equality::operator==(const Equality & other) const {
    return (other.left()==left() && other.right()==right());  
  }

  member_imp(Connect,Expression,left);
  member_imp(Connect,Expression,right);
  bool Connect::operator==(const Connect & other) const {
    return (other.left()==left() && other.right()==right());  
  }



  std::ostream& operator<<(std::ostream& out, const CallEq &e) // output
  {
    out << e.name() << "(";
    int l=e.args().size(),i=0;
    foreach_(Expression exp, e.args()) {
        out << exp;
        if (++i<l)
          out << ",";
    }
    out << ")";
    return out;
  }
 
  member_imp(CallEq,Name,name);
  member_imp(CallEq,ExpList,args);
  bool CallEq::operator==(const CallEq & other) const {
    return (other.name()==name() && other.args()==args());  
  }



  member_imp(EquationSection,bool,initial);
  member_imp(EquationSection,EquationList,equations);
  std::ostream& operator<<(std::ostream& out, const EquationSection &es) {
    if (es.equations().size()) {
      if (es.initial()) 
        out << "initial ";
      out << "equation\n";
    }
    foreach_(Equation e, es.equations()) 
      out << INDENT << e << ";\n"; 
    return out;
  }
  }
}
 
