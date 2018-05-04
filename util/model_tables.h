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

#ifndef MODEL_TABLES_H_
#define MODEL_TABLES_H_

#include <list>
#include <map>
#include <string>

#include "util_types.h"
#include "../ir/equation.h"
#include "../ir/statement.h"
#include "../ir/event.h"

typedef map<int,MMO_Equation> MMO_EquationTable;
typedef map<int,MMO_Event> MMO_EventTable;
typedef map<int,MMO_Statement> MMO_StatementTable;
typedef map<int,MMO_identifier> MMO_ImportTable;
typedef map<int,MMO_identifier> MMO_SymbolRefTable;
typedef map<int,MMO_identifier> MMO_PackageTable;

#endif /* MODEL_TABLES_H_ */
