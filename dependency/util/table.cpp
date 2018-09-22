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

#include <util/table.h>
// Si se usa type.h si o si tiene que estar mmo_class.h. No se puede ṕoner dentro de type.h
#include <mmo/mmo_class.h>

using namespace Modelica::AST;

VarInfo::VarInfo(TypePrefixes tp, Name n, Option<Comment> comm, Option<Modification> mod, Option<ExpList> ind, bool built):
  prefixes_(tp), type_(n), comment_(comm), modification_(mod), indices_(ind), builtin_(built), state_(false) {};
member_imp(VarInfo,Modelica::AST::TypePrefixes, prefixes);
member_imp(VarInfo,Name, type);
member_imp(VarInfo,Option<Modelica::AST::Comment>, comment);
member_imp(VarInfo,Option<Modelica::AST::Modification>, modification);
member_imp(VarInfo,Option<Modelica::AST::ExpList>, indices);
member_imp(VarInfo,bool, builtin);
member_imp(VarInfo,bool, state);
bool VarInfo::isPrefix(TypePrefix p)
{
	foreach_(Option<TypePrefix> t, prefixes()) 
		if (t && t.get() == p) return true;
	return false;
}

void VarInfo::removePrefix(TypePrefix p)
{
	TypePrefixes::iterator it = std::find(prefixes_ref().begin(),prefixes_ref().end(),Option<TypePrefix>(p));
	if (it != prefixes_ref().end()) prefixes_ref().erase(it);
}

std::ostream& operator<<(std::ostream& out, const VarInfo &v) // output
{
  out << "type: " << v.type() << " state: " << v.state() << " builtin: " << v.builtin();
  return out;
}
 

