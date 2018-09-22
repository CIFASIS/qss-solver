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


#ifndef AST_CLASS
#define AST_CLASS
#include <ast/equation.h>
#include <ast/statement.h>
#include <ast/element.h>
#include <ast/expression.h>

namespace Modelica {
  namespace AST {
  
  struct External {
    member_(OptExp,comp_ref);
    member_(Name,fun);
    member_(OptExpList,args);
  };
  
  struct Public: public ElemList {
    Public() {};
    Public(ElemList l): ElemList(l) {};
    printable(Public);
  };

  struct Protected: public ElemList {
    Protected() {};
    Protected(ElemList l): ElemList(l) {};
    printable(Protected);
  };
  typedef boost::variant<
    EquationSection,
    StatementSection,
    Public,
    Protected
  > CompElement;

  typedef std::vector<CompElement> CompElemList;

  template<typename T> 
  inline bool is(CompElement c) {
    return c.type()==typeid(T);
  }

  struct Composition {
    Composition(){};
    Composition(ElemList, CompElemList ,boost::optional<boost::fusion::vector3<boost::optional<String>, boost::optional<External>, boost::optional<Annotation> > >);
    member_(ElemList,elements);
    member_(CompElemList,comp_elem);
    member_(bool,external);
    member_(Option<String>,language);
    member_(Option<External>,call);
    member_(Option<Annotation>,ext_annot);
    member_(Option<Annotation>,annotation);
  } ;

  struct ExtendsClass {
    member_(Name, name);
    member_(Option<ClassModification>, modification);
    member_(StringComment, st_comment);
    member_(Composition, composition);
    member_(ClassPrefixes,prefixes);
    member_(bool,encapsulated);
    member_(bool,final);
    printable(ExtendsClass);
  };

  struct DerClass {
    member_(Name, name);
    member_(Name, deriv);
    member_(IdentList, ident_list);
    member_(Comment, comment);
    member_(ClassPrefixes,prefixes);
    member_(bool,encapsulated);
    member_(bool,final);
    printable(DerClass);
  };

  struct EnumClass {
    member_(Name, name);
    member_(EnumSpec, enum_spec);
    member_(Comment, comment);
    member_(ClassPrefixes,prefixes);
    member_(bool,encapsulated);
    member_(bool,final);
    printable(EnumClass);
  };

  struct DefClass {
    member_(Name,name);
    member_(TypePrefixes,type_prefixes);
    member_(Name,definition);
    member_(Option<ExpList>,indices);
    member_(Option<ClassModification>,modification);
    member_(Comment,comment);
    printable(DefClass);
    member_(ClassPrefixes,prefixes);
    member_(bool,encapsulated);
    member_(bool,final);
  };
  struct Class {
    Class(): final_(false), encapsulated_(false) {}
    member_(Name,name);
    member_(StringComment,st_comment);
    member_(Composition, composition);
    member_(Name,end_name);
    member_(ClassPrefixes,prefixes);
    member_(bool,final);
    member_(bool,encapsulated);
    printable(Class);
 };
  typedef boost::variant<
    Class,
    DefClass,
    EnumClass,
    DerClass,
    ExtendsClass
  > ClassType;
  
  struct ClassType_ {
    ClassType_(){};
    ClassType_(ClassType);
    member_(ClassType, cl);
  };

  template<typename T> 
  bool is(ClassType c) {
    return c.type()==typeid(T);
  }

  typedef std::vector<ClassType> ClassList;

  struct StoredDef {
    StoredDef(){};
    StoredDef(boost::optional<Name> name, ClassList cl);
    StoredDef(ClassList cl);
    member_(ClassList,classes);
    member_(Option<Name>,name);
    member_(bool,within);
    printable(StoredDef);
  };

  Name className(ClassType c);

  }
}
#endif
