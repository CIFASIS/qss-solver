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

#include "mmo_model_checker.h"

#include "../ast/class.h"
#include "../ast/composition.h"
#include "../ast/element.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../ast/stored_definition.h"

/* MicroModelica model checker interface */

MMO_ModelChecker::MMO_ModelChecker(string name) 
{
}

MMO_ModelChecker::~MMO_ModelChecker()
{
}

void
MMO_ModelChecker::visit(AST_Class x)
{
}

void
MMO_ModelChecker::leave(AST_Class x)
{
}

void
MMO_ModelChecker::visit(AST_Composition x)
{
}

void
MMO_ModelChecker::leave(AST_Composition x)
{
}

void
MMO_ModelChecker::visit(AST_CompositionElement x)
{
}

void
MMO_ModelChecker::leave(AST_CompositionElement x)
{
}

void
MMO_ModelChecker::visit(AST_CompositionEqsAlgs x)
{
}

void
MMO_ModelChecker::leave(AST_CompositionEqsAlgs x)
{
}

void
MMO_ModelChecker::visit(AST_External_Function_Call x)
{
}

void
MMO_ModelChecker::visit(AST_Element x)
{
}

void
MMO_ModelChecker::visit(AST_Modification x)
{
}

void
MMO_ModelChecker::leave(AST_Modification x)
{
}

void
MMO_ModelChecker::visit(AST_Comment x)
{
}

void
MMO_ModelChecker::visit(AST_Equation x)
{
}

void
MMO_ModelChecker::visit(AST_ForIndex x)
{
}

void
MMO_ModelChecker::visit(AST_Equation_Else x)
{
}

void
MMO_ModelChecker::visit(AST_Expression x)
{
}

void
MMO_ModelChecker::visit(AST_Argument x)
{
}

void
MMO_ModelChecker::visit(AST_Statement x)
{
}

void
MMO_ModelChecker::leave(AST_Statement x)
{
}

void
MMO_ModelChecker::visit(AST_Statement_Else x)
{
}

void
MMO_ModelChecker::visit(AST_StoredDefinition x)
{
}

void
MMO_ModelChecker::leave(AST_StoredDefinition x)
{
}

int
MMO_ModelChecker::apply(AST_Node x)
{
  return 0;
}
