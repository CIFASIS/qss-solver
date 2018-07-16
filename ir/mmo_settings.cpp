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

#include "mmo_settings.h"

#include "../ast/class.h"
#include "../ast/modification.h"
#include "../util/error.h"
#include "class.h"

/* MicroModelica Intermediate Representation */

MMO_Settings::MMO_Settings(string name) 
{
}

MMO_Settings::~MMO_Settings_()
{
}

void
MMO_Settings::visit(AST_Class x)
{
}

void
MMO_Settings::leave(AST_Class x)
{
}

void
MMO_Settings::visit(AST_Composition x)
{
}

void
MMO_Settings::leave(AST_Composition x)
{
}

void
MMO_Settings::visit(AST_CompositionElement x)
{
}

void
MMO_Settings::leave(AST_CompositionElement x)
{
}

void
MMO_Settings::visit(AST_CompositionEqsAlgs x)
{
}

void
MMO_Settings::leave(AST_CompositionEqsAlgs x)
{
}

void
MMO_Settings::visit(AST_External_Function_Call x)
{
}

void
MMO_Settings::visit(AST_Element x)
{
}

void
MMO_Settings::visit(AST_Modification x)
{
}

void
MMO_Settings::leave(AST_Modification x)
{
}

void
MMO_Settings::visit(AST_Comment x)
{
}

void
MMO_Settings::visit(AST_Equation x)
{
}

void
MMO_Settings::visit(AST_ForIndex x)
{
}

void
MMO_Settings::visit(AST_Equation_Else x)
{
}

void
MMO_Settings::visit(AST_Expression x)
{
}

void
MMO_Settings::visit(AST_Argument x)
{
}

void
MMO_Settings::visit(AST_Statement x)
{
}

void
MMO_Settings::leave(AST_Statement x)
{
}

void
MMO_Settings::visit(AST_Statement_Else x)
{
}

void
MMO_Settings::visit(AST_StoredDefinition x)
{
}

void
MMO_Settings::leave(AST_StoredDefinition x)
{
}

int
MMO_Settings::apply(AST_Node x)
{
}
