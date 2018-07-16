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

#include <iterator>
#include <sstream>
#include <utility>


#include "package.h" 
#include "../ast/ast_builder.h"
#include "../ast/composition.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"

MMO_Package::MMO_Package(string name) 
{
}

MMO_Package::~MMO_Package()
{
}

CL_Type
MMO_Package::classType()
{
  return CL_PACKAGE;
}

VarSymbolTable
MMO_Package::varTable()
{
  return NULL;
}

void
MMO_Package::insert(string n)
{
  return;
}

void
MMO_Package::insert(AST_Equation eq)
{
  return;
}

void
MMO_Package::insert(AST_Statement stm)
{
  return;
}

void
MMO_Package::insert(AST_Statement stm, bool initial)
{
  return;
}

void
MMO_Package::insert(MMO_Function &f)
{
  return;
}

void
MMO_Package::insert(AST_External_Function_Call efc)
{
  return;
}

void
MMO_Package::insert(VarName n, VarInfo vi, DEC_Type type)
{
  return;
}

void
MMO_Package::insert(VarName n, VarInfo vi)
{
  return;
}

void
MMO_Package::insert(AST_Argument_Modification x)
{
}

string
MMO_Package::name() const
{
  return "";
}
