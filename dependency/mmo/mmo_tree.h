
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
#ifndef CREATEMMOTREE_H
#define CREATEMMOTREE_H

#include <ast/class.h>
#include <mmo/mmo_class.h>
#include <util/type.h>
#include <ast/ast_types.h>

using namespace Modelica;
using namespace Modelica::AST;

/*	Sirve para crear un arbol de MMO_Class a partir de una StoreDef
 *  Crea una clase _ROOT_ como raiz de todas
 * 
 */
namespace Modelica {

struct MMO_Tree {
public:
	MMO_Tree();
	MMO_Class create(StoredDef);

private:

};

}


#endif
