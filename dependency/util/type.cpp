
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
#include <mmo/mmo_class.h>
#include <util/type.h>

namespace Type {
	using namespace Modelica;
	
	/*******************************************************************
	 * INTEGER TYPE
	 *******************************************************************/ 
	bool Integer::operator==(const Integer & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const Integer &s) { 
      out << "Integer";
      return out; 
    };
    
	/*******************************************************************
	 * REAL TYPE
	 *******************************************************************/
	bool Real::operator==(const Real & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const Real &s) { 
      out << "Real";
      return out; 
    };
    
    /*******************************************************************
	 * STRING TYPE
	 *******************************************************************/
    bool String::operator==(const String & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const String &s) { 
      out << "String";
      return out; 
    };
  
    /*******************************************************************
	 * BOOLEAN TYPE
	 *******************************************************************/
	bool Boolean::operator==(const Boolean & other) const { return true; }
    std::ostream& operator<<(std::ostream& out, const Boolean &s) { 
      out << "Boolean";
      return out; 
    };
  
	/*******************************************************************
	 * CLASS TYPE
	 *******************************************************************/
	Class::Class(Name s, MMO_Class * m): name_(s),clase_(m) {}
	bool Class::operator==(const Class & other) const {
      return (other.name()==name());
    }
    std::ostream& operator<<(std::ostream& out, const Class &c)  
    {
      out <<  * (c.clase()) ;
      return out;
    }
    member_imp(Class,Name,name);
    member_imp(Class,MMO_Class *,clase);
    
    
    /*******************************************************************
	 * TYPEDEF TYPE
	 *******************************************************************/
    TypeDef::TypeDef(AST::ClassPrefixes cp, Name n, AST::TypePrefixes p, Name t, Option<AST::ExpList> i, Option<AST::ClassModification> m ): 
					name_(n),type_prefixes_(p),definition_(t),indices_(i),modification_(m),prefixes_(cp) {}
	bool TypeDef::operator==(const TypeDef & other) const {
	 // if (modification().get() != other.modification().get()) return false;
      return (other.definition()==definition()&&other.name()==name()&&other.type_prefixes()==type_prefixes()
              &&indices()==other.indices()&& other.prefixes()==prefixes());
    }
    std::ostream& operator<<(std::ostream& out, const TypeDef &c) 
    {
      foreach_(Option<ClassPrefix> cp, c.prefixes())
		out << classPrefix(cp);
      out << c.name() << " = ";
      foreach_(Option<TypePrefix> tp, c.type_prefixes()) 
        out << typePrefix(tp);
      out << c.definition() << " ";  
      if (c.indices()) {
        ExpList el = c.indices().get();
        foreach_(Expression e, el)
          out << "[" << e << "]";
      }
      if (c.modification())
        out << c.modification().get();
      out <<  ";";
      return out;
    }
    member_imp(TypeDef,Name,name);
    member_imp(TypeDef,AST::TypePrefixes,type_prefixes);
	member_imp(TypeDef,Name,definition);
    member_imp(TypeDef,Option<AST::ExpList>,indices);
    member_imp(TypeDef,Option<AST::ClassModification>,modification);
    member_imp(TypeDef,AST::ClassPrefixes,prefixes);
    
    
    /*******************************************************************
	 * ENUM TYPE
	 *******************************************************************/
    EnumType::EnumType(AST::ClassPrefixes cp, Name n, AST::EnumSpec sp): name_(n),enum_spec_(sp),prefixes_(cp) {}
	bool EnumType::operator==(const EnumType & other) const {
      return (name()==other.name()&&prefixes()==other.prefixes()/*&&enum_spec()==other.enum_spec()*/);
    }
    std::ostream& operator<<(std::ostream& out, const EnumType &c) // output
    {
      foreach_(Option<ClassPrefix> cp, c.prefixes())
		out << classPrefix(cp);
      out << c.name() << " = " << /*c.enum_spec() <<*/ ";";
      return out;
    }
    member_imp(EnumType,Name, name);
    member_imp(EnumType,AST::EnumSpec, enum_spec);
    member_imp(EnumType,AST::ClassPrefixes,prefixes);

	/*******************************************************************
	 * ARRAY TYPE
	 *******************************************************************/
	Array::Array(AST::ExpList e,Type t): dim_(e),arrayOf_(t) {}
	bool Array::operator==(const Array & other) const {
      return (other.dim()==dim() &&  arrayOf()==other.arrayOf());
    }
    std::ostream& operator<<(std::ostream& out, const Array &c) // output
    {
      out <<  c.arrayOf() << " [ " << c.dim() << "] of " ;
      return out;
    }
    member_imp(Array,AST::ExpList,dim);
    member_imp(Array,Type,arrayOf);

	/*******************************************************************
	 * TUPLA TYPE
	 *******************************************************************/
	Tuple::Tuple(TypeList ts): types_(ts) {}
	bool Tuple::operator==(const Tuple & other) const {
      return (other.types()==types());
    }
    std::ostream& operator<<(std::ostream& out, const Tuple &c) // output
    {
      out << "(" << c.types() << ")" ;
      return out;
    }
    member_imp(Tuple,TypeList,types);

	/*******************************************************************
	 * FUNCTION TYPE
	 *******************************************************************/
	Function::Function(Type out, TypeList t): out_(out),args_(t) {}
	bool Function::operator==(const Function & other) const {
      return (other.out()==out() &&  args()==other.args());
    }
    std::ostream& operator<<(std::ostream& out, const Function &c) // output
    {
      out << c.out() << " (" << c.args() << ")" ;
      return out;
    }
    member_imp(Function,Type,out);
    member_imp(Function,TypeList,args);

}

