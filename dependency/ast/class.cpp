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


#include <ast/class.h>
#include <ast/element.h>
#include <boost/fusion/include/boost_tuple.hpp>
#include <boost/version.hpp>
#if defined BOOST_VERSION && BOOST_VERSION>105600
#include <boost/optional/optional_io.hpp>
#endif
#include <boost/variant/get.hpp>
#include <util/debug.h>

namespace Modelica {
  namespace AST {
    const char *TypePrefixName[] =  { "input ", "output ", "discrete ", "parameter ", "flow ", "stream ", "constant "};
    const char *ClassPrefixName[] = {"partial ", "class ", "model ", "block ", "record ", "connector ", "type ", "operator ", "expandable ", "pure ", "impure ", "package ", "function " };

    const char *classPrefix(Option<ClassPrefix> cp) {
      if (cp)
        return ClassPrefixName[cp.get()];
      return "";
    };
    const char *typePrefix(Option<TypePrefix> tp) {
      if (tp)
        return TypePrefixName[tp.get()];
      return "";
    };
 
    std::ostream& operator<<(std::ostream& out, const Class &c) // output
    {
      if (c.final()) 
        out << "final ";
      if (c.encapsulated()) 
        out << "encapsulated ";
      foreach_(Option<ClassPrefix> cp, c.prefixes())
        out << classPrefix(cp);
      out << c.name();
      StringComment st=c.st_comment();
      if (st) { 
        int size=st.get().size(),i=0;
        out << " ";
        foreach_ (const String &s, st.get()) {
          out << s;
          if (++i<size) 
            out << "+";
        }
        out << "\n";
      }
      out << "\n";
      BEGIN_BLOCK;
      foreach_(Element e, c.composition().elements())
        out << INDENT << e << ";\n";
      END_BLOCK;
      BEGIN_BLOCK;
      foreach_(CompElement ce, c.composition().comp_elem()) 
        out << INDENT << ce;
      END_BLOCK;
      if (c.composition().annotation())  
        out << c.composition().annotation().get() << ";\n";
      if (c.composition().external())  {
        out << "external ";
        out << "\n";
      }
      if (c.composition().annotation())  
        out << c.composition().annotation().get() << ";\n";
      out << INDENT<<  "end " << c.end_name();
      return out;
    }
 
  member_imp(External,OptExp,comp_ref);
  member_imp(External,Name,fun);
  member_imp(External,OptExpList,args);
  member_imp(Class,Name,name);
  member_imp(Class,StringComment,st_comment);
  member_imp(Class,Composition, composition);
  member_imp(Class,Name,end_name);
  member_imp(Class,bool,encapsulated);
  member_imp(Class,bool,final);
  member_imp(Class,ClassPrefixes,prefixes);
  member_imp(ClassType_,ClassType,cl);
  ClassType_::ClassType_(ClassType c): cl_(c) {
  }
 
  member_imp(StoredDef,ClassList,classes);
  member_imp(StoredDef,bool,within);
  member_imp(StoredDef,Option<Name>,name);
  std::ostream& operator<<(std::ostream& out, const StoredDef &s) // output
  {
    if (s.within()) {
      out << "within ";
      if (s.name()) 
        out << s.name().get();
      out << ";\n";
    }
    foreach_(ClassType e, s.classes())
      out << e << ";" << std::endl;
    return out;
  }
 
    member_imp(Composition,ElemList,elements);
    member_imp(Composition,Option<String>,language);
    member_imp(Composition,Option<External>,call);
    member_imp(Composition,Option<Annotation>,ext_annot);
    member_imp(Composition,CompElemList,comp_elem);
    member_imp(Composition,bool,external);
    member_imp(Composition,Option<Annotation>,annotation);
 
    StoredDef::StoredDef(ClassList cl): classes_(cl), within_(false) {}
    StoredDef::StoredDef(boost::optional<Name> name, ClassList cl): classes_(cl), name_(name), within_(true) {}
    member_imp(DerClass,bool,encapsulated);
    member_imp(DerClass,Name,name);
    member_imp(DerClass,Name,deriv);
    std::ostream& operator<<(std::ostream& out, const DerClass &c) {
      if (c.final()) 
        out << "final ";
      if (c.encapsulated()) 
        out << "encapsulated ";
      out << c.name() << "= der(" << c.deriv();
      out << ")";
      return out;
    }
    member_imp(DefClass,bool,encapsulated);
    member_imp(DefClass,Name,name);
    member_imp(DefClass,Name,definition);
    member_imp(DefClass,TypePrefixes,type_prefixes);
    member_imp(DefClass,Option<ExpList>,indices);
    member_imp(DefClass,Option<ClassModification>,modification);
    std::ostream& operator<<(std::ostream& out, const DefClass &c) {
      if (c.final()) 
        out << "final ";
      if (c.encapsulated()) 
        out << "encapsulated ";
      out << c.name() << " = ";
      foreach_(Option<TypePrefix> tp, c.type_prefixes()) 
          out << typePrefix(tp);
      out << c.definition();
      if (c.modification()) 
        out << c.modification().get();
      return out;
    }
    std::ostream& operator<<(std::ostream& out, const ExtendsClass &c) {
      if (c.final()) 
        out << "final ";
      if (c.encapsulated()) 
        out << "encapsulated ";
      out << "extends " << c.name();
      if (c.modification())
        out << c.modification();
      return out;
    }
    member_imp(ExtendsClass,bool,encapsulated);
    member_imp(ExtendsClass,Name,name);
    member_imp(ExtendsClass,Option<ClassModification>,modification);
    member_imp(EnumClass,bool,encapsulated);
    std::ostream& operator<<(std::ostream& out, const EnumClass &c) {
      if (c.final()) 
        out << "final ";
      if (c.encapsulated()) 
        out << "encapsulated ";
      out << c.name() << " =  enumeration (" ;
      out << ")";
      return out;
    }
    member_imp(DerClass,bool,final);
    member_imp(DefClass,bool,final);
    member_imp(ExtendsClass,bool,final);
    member_imp(EnumClass,bool,final);
    member_imp(EnumClass,Name,name);
    member_imp(EnumClass,EnumSpec,enum_spec);
    member_imp(DerClass,ClassPrefixes,prefixes);
    member_imp(DefClass,ClassPrefixes,prefixes);
    member_imp(ExtendsClass,ClassPrefixes,prefixes);
    member_imp(EnumClass, ClassPrefixes,prefixes);

   std::ostream& operator<<(std::ostream& out, const Public &p) // output
    {
      out << "Public ";
      return out;
    }

    std::ostream& operator<<(std::ostream& out, const Protected &p) // output
    {
      out << "Protected ";
      return out;
    }
 

    Composition::Composition(ElemList el, CompElemList comp_el
      , boost::optional<boost::fusion::vector3<boost::optional<String>, boost::optional<External>, boost::optional<Annotation> > > ext
      ): elements_(el), comp_elem_(comp_el)
    {
      if (ext)
        external_=true;
    }

    Name className(ClassType c) {
      if (is<Class>(c)) 
        return get<Class>(c).name();
      if (is<DefClass>(c)) 
        return get<DefClass>(c).name();
      if (is<DerClass>(c)) 
        return get<DerClass>(c).name();
      if (is<EnumClass>(c)) 
        return get<EnumClass>(c).name();
      if (is<ExtendsClass>(c)) 
        return get<ExtendsClass>(c).name();
      ERROR("className over unknown class type");
      return "";


    }
  }
}
