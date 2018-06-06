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

#include "class.h"

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
#include "../util/md_index.h"
#include "../util/symbol_table.h"
#include "../util/model_tables.h"
#include "../util/type.h"
#include "../util/util.h"
#include "annotation.h"
#include "equation.h"
#include "event.h"
#include "expression.h"
#include "mmo_util.h"
#include "statement.h"

/* MMO_Class class. */

MMO_Class::MMO_Class()
{
}

MMO_Class::~MMO_Class()
{
}

MMO_Package*
MMO_Class::getAsPackage() const
{
  return (MMO_Package*)(this);
}

MMO_Model*
MMO_Class::getAsModel() const
{
  return (MMO_Model*)(this);
}

MMO_Function*
MMO_Class::getAsFunction() const
{
  return (MMO_Function*)(this);
}
