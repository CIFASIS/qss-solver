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


#ifndef AST_MODIFICATION
#define AST_MODIFICATION
#include <ast/expression.h>

namespace Modelica {
  namespace AST {
    typedef boost::optional<StringList> StringComment;
    struct ModEq {
      ModEq() {};
      ModEq(Expression exp): exp_(exp){};
      member_(Expression,exp);
      printable(ModEq);
      comparable(ModEq);
    };

    struct ModAssign {
      ModAssign() {};
      ModAssign(Expression exp): exp_(exp){};
      member_(Expression,exp);
      printable(ModAssign);
      comparable(ModAssign);
    };

    struct ModClass;
    typedef boost::variant<
      ModEq,
      ModAssign,
      boost::recursive_wrapper<ModClass>
    > Modification;
    template<typename T> 
    inline bool is(Modification e) {
      return e.type()==typeid(T);
    }


    struct ElMod {
      ElMod() {};
      ElMod(Name, Modification);
      //ElMod(Name);
      member_(Name,name);
      member_(Option<Modification>, modification);
      member_(StringComment, st_comment);
      member_(bool, each);
      member_(bool, final);
      printable(ElMod);
      comparable(ElMod);
    };
   struct ElRedecl;
   struct ElRepl;
    typedef boost::variant<
      ElMod,
      boost::recursive_wrapper<ElRepl>,
      boost::recursive_wrapper<ElRedecl>
    > Argument;
    template<typename T> 
    inline bool is(Argument e) {
      return e.type()==typeid(T);
    }
    
    struct ClassModification: List<Argument> {
      ClassModification(){};
      ClassModification(Argument);
      ClassModification(Argument, Argument);
      printable(ClassModification);
    };
    
    struct Annotation {
      Annotation(){};
      Annotation(ClassModification);
      member_(ClassModification,modification);
      printable(Annotation);
    };
    struct ModClass {
      ModClass(){};
      ModClass(ClassModification);
      ModClass(ClassModification, Expression);
      member_(ClassModification,modification);
      member_(OptExp,exp);
      printable(ModClass);
      comparable(ModClass);
    };   
    struct Comment {
      member_(StringComment,st_comment);
      member_(Option<Annotation>,annotation);
      printable(Comment);
    };

 
  struct Enum {
    member_(Name, name);
    member_(Comment, comment);
    comparable(Enum);
  };
  typedef std::vector<Enum> EnumList;
  
  struct EnumSpec {
    EnumSpec(){};
    EnumSpec(EnumList){};
    member_(EnumList,list);
    comparable(EnumSpec);
    printable(EnumSpec);
  };


  struct ShortClass {
    ShortClass();
    ShortClass(ClassPrefixes, Name, TypePrefixes, Name, Option<ExpList>, Option<ClassModification>, Comment);
    ShortClass(ClassPrefixes, Name, EnumSpec, Comment );
    comparable(ShortClass);
    printable(ShortClass);
    member_(ClassPrefixes,prefixes);
    member_(Name,name);
    member_(Option<TypePrefixes>, type_prefixes);
    member_(Option<Name>, derived);
    member_(Option<ExpList>, indices);
    member_(Option<ClassModification>, modification);
    member_(Comment, comment);
    member_(Option<EnumSpec>, enum_spec);
    
  };

  struct Constrained {
    member_(Name, name);
    member_(Option<ClassModification>, modification);
    comparable(Constrained);
  };

  struct Declaration {
    Declaration();
    Declaration(Name, Option<ExpList>, Option<Modification>, OptExp);
    Declaration(Name);
    member_(Name,name);
    member_(Option<ExpList>, indices);
    member_(Option<Modification>,modification);
    member_(Comment, comment);
    member_(OptExp,conditional);
    printable(Declaration);
    comparable(Declaration);
  };
  typedef std::vector<Declaration> DeclList;

  struct Component {
    Component();
    Component(TypePrefixes, Name, Option<ExpList>, DeclList);
    member_(TypePrefixes,prefixes);
    member_(Name,type);
    member_(Option<ExpList>, indices);
    member_(DeclList,declarations);
    member_(bool,redeclare);
    member_(bool,final);
    member_(bool,inner);
    member_(bool,outer);
    member_(bool,replaceable);
    member_(Option<Constrained>,constrained);
    member_(Option<Comment>,constrained_comment);
    printable(Component);
  };

  struct Component1 {
    Component1();
    Component1(TypePrefixes, Name, DeclList);
    member_(TypePrefixes,prefixes);
    member_(Name,type);
    member_(Declaration,declaration);
    printable(Component1);
    comparable(Component1);
  };
  typedef boost::variant<ShortClass, Component1> ReplArg;
  template <typename T> 
  inline bool is(ReplArg r) {
    return r.type()==typeid(T);
  }
  struct ElRepl {
    ElRepl();
    ElRepl(ReplArg, Option<Constrained>);
    member_(bool, each);
    member_(bool, final);
    member_(ReplArg, argument);
    member_(Option<Constrained>, constrain);
    printable(ElRepl);
    comparable(ElRepl);
  };
  typedef boost::variant<ReplArg, ElRepl> RedeclArg;

  template <typename T> 
  inline bool is(RedeclArg r) {
    return r.type()==typeid(T);
  }
  struct ElRedecl {
      ElRedecl();
      ElRedecl(bool each, bool final, RedeclArg arg);
      member_(bool, each);
      member_(bool, final);
      member_(RedeclArg, argument);
      comparable(ElRedecl);
      printable(ElRedecl);
    };
 
  }
}
#endif
