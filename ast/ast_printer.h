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

#ifndef AST_PRINTER_H_
#define AST_PRINTER_H_

#include "../util/macros.h"
#include "ast_types.h"

using namespace std;

/**
 *
 */
DEFINE_PRINTER(AST_Argument);
/**
 *
 */
DEFINE_PRINTER(AST_Comment);
/**
 *
 */
DEFINE_PRINTER(AST_CompositionElement);
/**
 *
 */
DEFINE_PRINTER(AST_Composition);
/**
 *
 */
DEFINE_PRINTER(AST_Class);
/**
 *
 */
DEFINE_PRINTER(AST_Element);
/**
 *
 */
DEFINE_PRINTER(AST_Expression);
/**
 *
 */
DEFINE_PRINTER(AST_Expression_ComponentReference);
/**
 *
 */
DEFINE_PRINTER(AST_Equation);
/**
 *
 */
DEFINE_PRINTER(AST_Modification);
/**
 *
 */
DEFINE_PRINTER(AST_Statement);
/**
 *
 */
DEFINE_PRINTER(AST_StoredDefinition);
/**
 *
 */
DEFINE_PRINTER(AST_String);

#endif  /* AST_PRINTER_H_ */
