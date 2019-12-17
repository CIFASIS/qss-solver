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

#include "model.h"
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
#include "../util/ast_util.h"
#include "../util/symbol_table.h"

MMO_Model::MMO_Model(string name) {}

MMO_Model::~MMO_Model() {}

void MMO_Model::insert(VarName n, VarInfo vi, DEC_Type type) {}

void MMO_Model::insert(VarName n, VarInfo vi) {}

void MMO_Model::insert(AST_Equation eq) {}

void MMO_Model::insert(AST_Statement stm) {}

void MMO_Model::insert(AST_External_Function_Call efc) { return; }

void MMO_Model::insert(AST_Statement stm, bool initial) {}

void MMO_Model::insert(MMO_Function &f) {}

void MMO_Model::insert(AST_Argument_Modification x) {}

VarSymbolTable MMO_Model::varTable() { return NULL; }

void MMO_Model::insert(string n) {}

string MMO_Model::name() const { return ""; }

MMO_ImportTable MMO_Model::imports() {}
