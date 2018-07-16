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

#include "function.h"

#include <iterator>
#include <sstream>
#include <utility>

#include "../ast/ast_builder.h"
#include "../ast/composition.h"
#include "../ast/equation.h"
#include "../ast/expression.h"
#include "../ast/modification.h"
#include "../ast/statement.h"
#include "../parser/parse.h"
#include "../util/ast_util.h"
#include "../util/dependencies.h"
#include "../util/symbol_table.h"
#include "../util/type.h"
#include "../util/util.h"
#include "annotation.h"
#include "equation.h"
#include "event.h"
#include "expression.h"
#include "mmo_util.h"
#include "statement.h"

/* MMO_Function class */

MMO_Function::MMO_Function(string name) 
{
}

MMO_Function::~MMO_Function()
{
}

CL_Type
MMO_Function::classType()
{
  return CL_FUNCTION;
}

VarSymbolTable
MMO_Function::varTable()
{
  return NULL;
}

void
MMO_Function::insert(string n)
{
}

void
MMO_Function::insert(AST_Equation eq)
{
  return;
}

void
MMO_Function::insert(AST_Statement stm)
{
}

void
MMO_Function::insert(AST_Statement stm, bool initial)
{
}

void
MMO_Function::insert(MMO_Function &f)
{
  return;
}

void
MMO_Function::insert(AST_External_Function_Call efc)
{
}

void
MMO_Function::insert(VarName n, VarInfo vi, DEC_Type type)
{
}

void
MMO_Function::insert(VarName n, VarInfo vi)
{
}

void
MMO_Function::insert(AST_Argument_Modification x)
{
}

string
MMO_Function::name() const
{
  return ""; 
}
