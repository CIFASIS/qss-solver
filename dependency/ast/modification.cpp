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


#include <ast/modification.h>
#include <boost/variant/get.hpp>

namespace Modelica {
  namespace AST {
    member_imp(ModEq,Expression,exp);
    std::ostream& operator<<(std::ostream& out, const ModEq &c) // output
    {
      out << "=" << c.exp();
      return out;
    } 
    member_imp(ModAssign,Expression,exp);
    std::ostream& operator<<(std::ostream& out, const ModAssign &c) // output
    {
      out << ":=" << c.exp();
      return out;
    } 
    member_imp(ModClass,OptExp,exp);
    member_imp(ModClass,ClassModification,modification);
    ModClass::ModClass(ClassModification cm): modification_(cm), exp_(OptExp()) {};
    ModClass::ModClass(ClassModification cm, Expression e): modification_(cm),exp_(e) {};
    ClassModification::ClassModification(Argument a) {
      push_back(a);
    } 
    ClassModification::ClassModification(Argument a, Argument b) {
      push_back(a);
      push_back(b);
    }
    std::ostream& operator<<(std::ostream& out, const ModClass &c) // output
    {
      out << c.modification();
      if (c.exp()) 
        out << "=" << c.exp().get();
      return out;
    } 
    Annotation::Annotation(ClassModification cm): modification_(cm) {};
    member_imp(Annotation,ClassModification,modification);
    std::ostream& operator<<(std::ostream& out, const Annotation &a) // output
    {
      BEGIN_BLOCK;
      out << INDENT << "annotation" << a.modification();
      END_BLOCK;
      return out;
    } 
    std::ostream& operator<<(std::ostream& out, const ClassModification &m) // output
    {
      out << "(";
      int size=m.size(),i=0;
      foreach_(Argument a, m) {
        out << a;
        if (++i<size)
          out << ",";
      }

      out << ")";
      return out;
    }
    ElMod::ElMod(Name n, Modification m):name_(n),modification_(Option<Modification>(m)), each_(false), final_(false){}
    member_imp(ElMod,Name,name);
    member_imp(ElMod,Option<Modification>, modification);
    member_imp(ElMod,bool,each);
    member_imp(ElMod,bool,final);
    member_imp(ElMod,StringComment, st_comment);
 
    std::ostream& operator<<(std::ostream& out, const ElMod &e) // output
    {
      if (e.each())
        out << "each ";
      out << e.name();
      if (e.modification())
        out << e.modification().get();
      return out;
    }
    std::ostream& operator<<(std::ostream& out, const ElRedecl &e) // output
    {
      out << "redeclare ";
      if (e.each())
        out << "each ";
      if (e.final())
        out << "final ";
      if (is<ReplArg>(e.argument())) {
        ReplArg arg=boost::get<ReplArg>(e.argument());
        if (is<Component1>(arg)) {
          out << boost::get<Component1>(arg);
        } else {
	        ShortClass scl = boost::get<ShortClass>(arg);
          out << scl;
	}	
      } else {
	      ElRepl elr = boost::get<ElRepl>(e.argument());
    	  out << elr;
      }
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const ShortClass &c) // output
    {
      foreach_(Option<ClassPrefix> cp, c.prefixes())
        out << classPrefix(cp);
      out << c.name() << " = ";
      if (c.derived()) {
	if (c.type_prefixes()) {
      		foreach_(Option<TypePrefix> tp, c.type_prefixes().get()) 
          	out << typePrefix(tp);
	}
	
	out << c.derived().get();
	if (c.indices()) {
	  out << "[....]";
        }
	if (c.modification()) {
          out << c.modification().get();
        }
      } 
      return out;
    }

    ElRepl::ElRepl() {
      each_=false;
      final_=false;
    } 

    ElRepl::ElRepl(ReplArg arg, Option<Constrained> cons):argument_(arg), constrain_(cons) {
      each_=false;
      final_=false;
    }
    member_imp(ElRedecl,bool,each);
    member_imp(ElRedecl,bool,final);
    member_imp(ElRedecl,RedeclArg,argument);
    member_imp(ElRepl,bool,each);
    member_imp(ElRepl,Option<Constrained>,constrain);
    member_imp(ElRepl,ReplArg,argument);
    member_imp(ElRepl,bool,final);

    ElRedecl::ElRedecl() {
      each_=false;
      final_=false;
    }
    ElRedecl::ElRedecl(bool each, bool final, RedeclArg arg): each_(each), final_(final), argument_(arg) {
      each_=false;
      final_=false;
    }
    std::ostream& operator<<(std::ostream& out, const ElRepl &e) // output
    {
      out << "replaceable ";
      return out;
    }
 
    Component1::Component1(){}
    Component1::Component1(TypePrefixes tp, Name ty, DeclList decl): prefixes_(tp), type_(ty), declaration_(decl.front()) {
    }
    member_imp(Component1, Declaration,declaration);
    member_imp(Component1, TypePrefixes,prefixes);
    member_imp(Component1,Name,type);
    std::ostream& operator<<(std::ostream& out, const Component1 &c) // output
    {
      out << c.type() << " " ;
      out << c.declaration();
      return out;
    }

    bool ElMod::operator==(const ElMod& other) const {
      return (other.name()==name() && modification()==other.modification() && other.each()==each() && other.final()==final());
    }
	
    bool ElRepl::operator==(const ElRepl& other) const {
      return (other.argument()==argument() && other.constrain()==constrain() && other.each()==each() && other.final()==final());
    }
	
    bool ElRedecl::operator==(const ElRedecl& other) const {
      return (other.argument()==argument() && other.each()==each() && other.final()==final());
    }
 
    bool ModEq::operator==(const ModEq& other) const {
      return (other.exp()==exp());
    }

    bool ModAssign::operator==(const ModAssign& other) const {
      return (other.exp()==exp());
    }

    member_imp(Constrained,Name,name);
    member_imp(Constrained,Option<ClassModification>,modification);
    bool Constrained::operator==(const Constrained& other) const {
      return (other.name()==name() && other.modification()==modification());
    }

    bool Component1::operator==(const Component1& other) const {
      return (other.type()==type() && other.declaration()==declaration());
    }
    ShortClass::ShortClass() { 
      type_prefixes_= Option<TypePrefixes>();
      derived_ =Option<Name>();
      modification_ = Option<ClassModification>();
      indices_ = Option<ExpList>();
      enum_spec_ = Option<EnumSpec>();
 
    };
    ShortClass::ShortClass(ClassPrefixes cp, Name n, TypePrefixes tp, Name der, Option<ExpList> ind, Option<ClassModification> mod, Comment comm): prefixes_(cp), name_(n), derived_(der), indices_(ind), modification_(mod), comment_(comm) {
    
      		foreach_(Option<TypePrefix> t, tp) {
            if (t) {
              if (!type_prefixes_)
                type_prefixes_=TypePrefixes();
              type_prefixes_.get().push_back(t);
            }
          }
      enum_spec_ = Option<EnumSpec>();
 
    };
    ShortClass::ShortClass(ClassPrefixes, Name, EnumSpec, Comment ) {
      abort();
    }
    bool ShortClass::operator==(const ShortClass& other) const {
      // TODO
      abort();
      return false;
    }
    member_imp(ShortClass,ClassPrefixes,prefixes);
    member_imp(ShortClass,Name,name);
    member_imp(ShortClass,Option<TypePrefixes>, type_prefixes);
    member_imp(ShortClass,Option<Name>, derived);
    member_imp(ShortClass,Option<ClassModification>, modification);
    member_imp(ShortClass,Option<ExpList>, indices);
    member_imp(ShortClass,Comment, comment);
    member_imp(ShortClass,Option<EnumSpec>, enum_spec);
 

    bool Declaration::operator==(const Declaration& other) const {
      return (other.name()==name() && other.indices()==indices() && other.modification()==modification() && other.conditional()==conditional()); 
    }
	
    bool ModClass::operator==(const ModClass& other) const {
      return (other.modification()==modification() && other.exp()==exp());
    }
    Declaration::Declaration(): indices_(Option<ExpList>()), conditional_(OptExp()) {};
    Declaration::Declaration(Name name): name_(name){};
    Declaration::Declaration(Name name, Option<ExpList> indices, Option<Modification> mod, OptExp cond): name_(name), indices_(indices), modification_(mod),
      conditional_(cond) { };


    member_imp(Comment,StringComment,st_comment);
    member_imp(Comment,Option<Annotation>,annotation);
    std::ostream& operator<<(std::ostream& out, const Comment &c) {
      if (c.st_comment()) {
        const StringList st = c.st_comment().get();
        int i=0,l=st.size();
        out << " ";
        foreach_(const String &s, st) {
          out << s << (i++<l-1 ? " + " : "");
        }
      }
      if (c.annotation()) {
        out << " ";
        SKIP_BLOCK_START;
        out << c.annotation().get();
        SKIP_BLOCK_END;

      }
      return out;
    }
  }
}
