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

#include <string>

#include "../ast/ast_types.h"

/**
 *
 * @param
 * @param
 * @return
 */
AST_StoredDefinition parseFile(string, int *);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Class parseClass(string, int *);
/**
 *
 * @param
 * @param
 * @return
 */
AST_Expression parseExpression(string, int *);
/**
 *
 * @param exp
 * @param r
 * @return
 */
AST_Equation parseEquation(string exp, int *r);
/**
 *
 * @param exp
 * @param r
 * @return
 */
AST_Statement parseStatement(string exp, int *r);
