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


#include <ast/element.h>
#include <ast/class.h>
#include <boost/foreach.hpp>
#include <boost/variant/get.hpp>

namespace Modelica {
  namespace AST {
    Component::Component(){
      redeclare_=false;
      final_=false;
      inner_=false;
      outer_=false;
      replaceable_=false;
 
    }
    Component::Component(TypePrefixes tp, Name ty, Option<ExpList> ind, DeclList decl): prefixes_(tp), type_(ty), indices_(ind), declarations_(decl) {
      redeclare_=false;
      final_=false;
      inner_=false;
      outer_=false;
      replaceable_=false;
    }
    member_imp(Component,Name,type);
    member_imp(Component,Option<ExpList>, indices);
    member_imp(Component,DeclList,declarations);
    member_imp(Component,TypePrefixes,prefixes);
    member_imp(Component,bool,redeclare);
    member_imp(Component,bool,final);
    member_imp(Component,bool,inner);
    member_imp(Component,bool,outer);
    member_imp(Component,bool,replaceable);
    member_imp(Component,Option<Constrained>,constrained);
    member_imp(Component,Option<Comment>,constrained_comment);
 
    std::ostream& operator<<(std::ostream& out, const Component &c) // output
    {
      if (c.redeclare())
	out << "redeclare ";
      if (c.final())
	out << "final ";
      if (c.inner())
	out << "inner ";
      if (c.outer())
	out << "outer ";
      if (c.replaceable())
	out << "replaceable ";
      foreach_(Option<TypePrefix> tp, c.prefixes()) 
        out << typePrefix(tp);
      out << c.type() << " ";
      int size=c.declarations().size(),i=0;
      foreach_(Declaration d, c.declarations()) {
        out << d;
        if (++i<size) out << ",";
      }
      return out;
    }
 
    std::ostream& operator<<(std::ostream& out, const Declaration &d) // output
    {
      out << d.name();
      if (d.modification()) 
        out << d.modification().get();
      if (d.conditional()) 
        out << " if " << d.conditional().get();
      out << d.comment();
      return out;
    }
 
    member_imp(Declaration,Name,name);
    member_imp(Declaration,Option<ExpList>, indices);
    member_imp(Declaration,Option<Modification>,modification);
    member_imp(Declaration,Comment, comment);
    member_imp(Declaration,OptExp,conditional);

    ElemClass::ElemClass(ClassType_ c) { 
      class_element_=c;
      replaceable_=false;
      redeclare_=false;
      inner_=false;
      outer_=false;
      final_=false;
    };
    member_imp(ElemClass,Option<Constrained>,constrained);
    member_imp(ElemClass,Option<Comment>,constrained_comment);
    member_imp(ElemClass,bool,final);
    member_imp(ElemClass,bool,inner);
    member_imp(ElemClass,bool,outer);
    member_imp(ElemClass,bool,redeclare);
    member_imp(ElemClass,bool,replaceable);
    member_imp(ElemClass,boost::recursive_wrapper<ClassType_>,class_element);

    std::ostream& operator<<(std::ostream& out, const Import &i) // output
    {
      out << "import";
      return out;
    }
    member_imp(Extends,Name,name);
    member_imp(Extends,Option<Annotation>,annotation);
    member_imp(Extends,Option<ClassModification>,modification);
    std::ostream& operator<<(std::ostream& out, const Extends &e) // output
    {
      out << "extends " << e.name();
      if (e.modification())
        out << e.modification().get();
      if (e.annotation())
        out << e.annotation().get();
      return out;
    }
    std::ostream& operator<<(std::ostream& out, const ElemClass &e) // output
    {
      if (e.redeclare())
	out << "redeclare ";
      if (e.final())
	out << "final ";
      if (e.inner())
	out << "inner ";
      if (e.outer())
	out << "outer ";
      if (e.replaceable())
	out << "replaceable ";

      out << e.class_element().get().cl();
      return out;
    }
  }
}
