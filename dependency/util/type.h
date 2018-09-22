
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

#ifndef TYPE_H
#define TYPE_H
#include <string>
#include <iostream>
#include <vector>
#include <ast/expression.h> 
//#include <mmo/mmo_class.h>

namespace Modelica {
	struct MMO_Class;
}

namespace Type {
    using namespace boost;
    using namespace Modelica;
	typedef std::string     Name;
	
    struct Real;
    struct Integer;
    struct Boolean;
    struct String ;
    struct Array;
    struct Tuple;
    struct Function;
    struct Class;
    struct TypeDef;
    struct EnumType;

    typedef variant<
      Integer,
      String,
      Boolean,
      String,
      Real,
      TypeDef,
      EnumType,
      recursive_wrapper<Array>,
      recursive_wrapper<Tuple>,
      recursive_wrapper<Function>,
      recursive_wrapper<Class>
    > Type;

    typedef std::vector<Type> TypeList;

    struct Integer {
      comparable(Integer);
      printable(Integer);
    };
    
    struct Real {
      comparable(Real);
      printable(Real);
    };
    
    struct String {
      comparable(String);
      printable(String);
    };
    
    struct Boolean {
      comparable(Boolean);
      printable(Boolean);
    };

	struct TypeDef {
      TypeDef(AST::ClassPrefixes cp, Name n , AST::TypePrefixes p, Name t, Option<AST::ExpList> i, Option<AST::ClassModification> m );
      comparable(TypeDef);
      printable(TypeDef);
      
      member_(Name,name);
      member_(AST::TypePrefixes,type_prefixes);
	  member_(Name,definition);
      member_(Option<AST::ExpList>,indices);
      member_(Option<AST::ClassModification>,modification);
      member_(AST::ClassPrefixes,prefixes);
    };
	
    struct EnumType {
      EnumType(AST::ClassPrefixes cp, Name n, AST::EnumSpec sp);
      comparable(EnumType);
      printable(EnumType);
      member_(Name, name);
      member_(AST::EnumSpec, enum_spec);
      member_(AST::ClassPrefixes,prefixes);
    };
	
    struct Class {
      Class(Name n,MMO_Class * m);
      comparable(Class);
      printable(Class);
      member_(Name,name);
      member_(MMO_Class * ,clase);
    };

	struct Array {
      Array(AST::ExpList e,Type t);
      comparable(Array);
      printable(Array);
      member_(AST::ExpList,dim);
      member_(Type,arrayOf);
    };

	struct Tuple {
      Tuple(TypeList e);
      comparable(Tuple);
      printable(Tuple);
      member_(TypeList,types);
    };

	struct Function {
      Function(Type out, TypeList e);
      comparable(Function);
      printable(Function);
      member_(Type,out);
      member_(TypeList,args);
    };

    template<typename T> 
    bool is(Type e) {
      return e.type()==typeid(T);
    }
};
#endif
