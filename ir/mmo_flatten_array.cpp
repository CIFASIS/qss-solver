/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <ir/mmo_flatten_array.h>

MMO_FlattenArray_::MMO_FlattenArray_ (string name)
{
}

MMO_FlattenArray_::~MMO_FlattenArray_ ()
{
}

MMO_StoredDefinition
MMO_FlattenArray_::storedDefinition ()
{
}

list<MMO_Class>
MMO_FlattenArray_::classes () const
{
}

void
MMO_FlattenArray_::visit (AST_Class x)
{
}

void
MMO_FlattenArray_::leave (AST_Class x)
{
}

void
MMO_FlattenArray_::visit (AST_Composition x)
{
}

void
MMO_FlattenArray_::leave (AST_Composition x)
{
}

void
MMO_FlattenArray_::visit (AST_CompositionElement x)
{
}

void
MMO_FlattenArray_::leave (AST_CompositionElement x)
{
}

void
MMO_FlattenArray_::visit (AST_CompositionEqsAlgs x)
{
}

void
MMO_FlattenArray_::leave (AST_CompositionEqsAlgs x)
{
}

void
MMO_FlattenArray_::visit (AST_External_Function_Call)
{
}

void
MMO_FlattenArray_::visit (AST_Element x)
{
}

void
MMO_FlattenArray_::visit (AST_Modification x)
{
}

void
MMO_FlattenArray_::leave (AST_Modification x)
{
}

void
MMO_FlattenArray_::visit (AST_Comment x)
{
}

void
MMO_FlattenArray_::visit (AST_Equation x)
{
}

void
MMO_FlattenArray_::visit (AST_ForIndex x)
{
}

void
MMO_FlattenArray_::visit (AST_Equation_Else x)
{
}

void
MMO_FlattenArray_::visit (AST_Expression x)
{
}

void
MMO_FlattenArray_::visit (AST_Argument x)
{
}

void
MMO_FlattenArray_::visit (AST_Statement x)
{
}

void
MMO_FlattenArray_::leave (AST_Statement x)
{
}

void
MMO_FlattenArray_::visit (AST_Statement_Else x)
{
}

void
MMO_FlattenArray_::visit (AST_StoredDefinition x)
{
}

void
MMO_FlattenArray_::leave (AST_StoredDefinition x)
{
}

int
MMO_FlattenArray_::apply (AST_Node x)
{
}

MMO_FlattenArray
newMMO_FlattenArray (string name)
{
}

void
deleteMMO_FlattenArray (MMO_FlattenArray m)
{
}

