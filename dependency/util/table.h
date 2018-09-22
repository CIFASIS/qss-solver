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

#ifndef TABLE_H
#define TABLE_H

#include <map>
#include <string>
#include <ast/ast_types.h>
#include <ast/class.h>
#include <util/type.h>

template<typename Key,typename Value>
struct SymbolTable: public std::map<Key,Value> {
  void insert(Key k, Value v) {
	std::map<Key,Value>::erase(k);
    std::map<Key,Value>::insert(std::pair<Key,Value>(k,v));
  }
  Option<Value> operator[](Key k) const {
    if (std::map<Key,Value>::find(k)==std::map<Key,Value>::end())
      return Option<Value>();
    return std::map<Key,Value>::at(k); 
  }
  void remove(Key k) {
    std::map<Key,Value>::erase(k);
  }
  void dump() {
    typename std::map<Key,Value>::iterator it;
    for(it=std::map<Key,Value>::begin();it!=std::map<Key,Value>::end();it++) {
      std::cerr << it->first << ":" /*<< it->second */<< "\n";
    } 
  }
};


using namespace Modelica::AST;
struct VarInfo {
  VarInfo() { builtin_ = false; state_=false;};
  VarInfo(TypePrefixes tp, Name n, Option<Comment> comm = Option<Comment>(), Option<Modification> mod = Option<Modification>(), Option<ExpList> ind = Option<ExpList>(), bool built = false);
  member_(TypePrefixes, prefixes);
  member_(Name, type);
  member_(Option<Comment>, comment);
  member_(Option<Modification>, modification);
  member_(Option<ExpList>, indices);
  member_(bool, builtin);
  member_(bool, state);
  bool isPrefix(TypePrefix);
  printable(VarInfo);
  void removePrefix(TypePrefix);
};

struct VarSymbolTable: public SymbolTable<Name, VarInfo>
{
  typedef std::map<Name,VarInfo> table_type;
  VarSymbolTable() {
    VarInfo v(TypePrefixes(),"Real");
    v.builtin_ref()=true;
    insert("time",v);
  }
};

struct TypeSymbolTable: public SymbolTable<Name, Type::Type>
{
	TypeSymbolTable() {
		insert("String",Type::String());
		insert("Integer",Type::Integer());
		insert("Real",Type::Real());
		insert("Boolean",Type::Boolean());
	}
};

#endif
