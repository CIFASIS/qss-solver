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

#include "mmo_ir.h"

#include <iterator>

#include "../ast/class.h"
#include "../ast/composition.h"
#include "../ast/element.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../util/error.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "class.h"
#include "expression.h"
#include "stored_definition.h"

/* MicroModelica Intermediate Representation */

MMO_MicroModelicaIR::MMO_MicroModelicaIR(string name) 
{
}

MMO_MicroModelicaIR::~MMO_MicroModelicaIR()
{
}

void
MMO_MicroModelicaIR::visit(AST_Class x)
{
}

/**
 * @brief Leave the class and set the corresponding father and child pointers if needed.
 *
 * When leaving the class, if the class has a father then reset the child pointer and
 * child prefix fields and add the child (that must be a function) into the father's children
 * list.
 *
 * @param x @ref AST_Class to visit.
 *
 */
void
MMO_MicroModelicaIR::leave(AST_Class x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Composition x)
{
}

void
MMO_MicroModelicaIR::leave(AST_Composition x)
{
}

void
MMO_MicroModelicaIR::visit(AST_CompositionElement x)
{
}

void
MMO_MicroModelicaIR::leave(AST_CompositionElement x)
{
}

void
MMO_MicroModelicaIR::visit(AST_CompositionEqsAlgs x)
{
}

void
MMO_MicroModelicaIR::leave(AST_CompositionEqsAlgs x)
{
}

void
MMO_MicroModelicaIR::visit(AST_External_Function_Call x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Element x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Modification x)
{
}

void
MMO_MicroModelicaIR::leave(AST_Modification x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Comment x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Equation x)
{
}

void
MMO_MicroModelicaIR::visit(AST_ForIndex x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Equation_Else x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Expression x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Argument x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Statement x)
{
}

void
MMO_MicroModelicaIR::leave(AST_Statement x)
{
}

void
MMO_MicroModelicaIR::visit(AST_Statement_Else x)
{
}

void
MMO_MicroModelicaIR::visit(AST_StoredDefinition x)
{
}

void
MMO_MicroModelicaIR::leave(AST_StoredDefinition x)
{
}

int
MMO_MicroModelicaIR::apply(AST_Node x)
{
}

MMO_StoredDefinition
MMO_MicroModelicaIR::storedDefinition()
{
  return MMO_StoredDefinition();
}

list<MMO_Class>
MMO_MicroModelicaIR::classes() const
{
  return _storedDefinition->classes();
}
