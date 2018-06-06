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
#include "../util/model_tables.h"
#include "../util/type.h"
#include "../util/util.h"
#include "package.h"
#include "annotation.h"
#include "equation.h"
#include "event.h"
#include "expression.h"
#include "mmo_util.h"
#include "statement.h"

MMO_Package::MMO_Package(string name) :
    _name(name), 
    _funcs(0), 
    _packages()
{
  _imports = MMO_ImportTable();
  _functions = MMO_FunctionTable();
}

MMO_Package::~MMO_Package()
{
}

CL_Type
MMO_Package::classType()
{
  return CL_PACKAGE;
}

MMO_ImportTable
MMO_Package::imports()
{
  return _imports;
}

VarSymbolTable
MMO_Package::varTable()
{
  return NULL;
}

void
MMO_Package::insert(string n)
{
  _imports->insert(n);
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
MMO_Package::insert(MMO_Function f)
{
  Index i;
  i.setOffset(_funcs++);
  _functions->insert(i, f);
  return;
}

MMO_FunctionTable
MMO_Package::functions()
{
  return _functions;
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
MMO_Package::fileName() const
{
  return Util::getInstance()->packageName(_name);
}

string
MMO_Package::name() const
{
  return _name;
}

string
MMO_Package::prefix()
{
  return "__" + _name + "__";
}
