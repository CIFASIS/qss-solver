
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

#include <boost/variant/variant.hpp>
#include <boost/variant/get.hpp>
#include <mmo/mmo_tree.h>
#include <iostream>

using namespace std;

MMO_Tree::MMO_Tree() {}

MMO_Class MMO_Tree::create(StoredDef sd)
{
	MMO_Class * mmo_root = new MMO_Class();
	mmo_root->set_name("_ROOT_");
		
	foreach_(ClassType cl, sd.classes()) {
		MMO_Class * mmo_aux = new MMO_Class(get<Class>(cl));
		mmo_aux->set_father(mmo_root);
		Type::Class tt = Type::Class(mmo_aux->name(),mmo_aux);
	   	mmo_root->tyTable_ref().insert(mmo_aux->name(),tt);
	   	mmo_root->types_ref().push_back(mmo_aux->name());
	}
	return *mmo_root;
}


