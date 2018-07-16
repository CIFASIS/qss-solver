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

#ifndef MMO_EVENT_H_
#define MMO_EVENT_H_

#include "../ast/ast_types.h"

/**
 *
 */
typedef enum
{
  HND_POSITIVE, //!< HND_POSITIVE
  HND_NEGATIVE, //!< HND_NEGATIVE
  HND_ZERO      //!< HND_ZERO
} HND_Type;

/**
 * Define the original relation type of the zero-crossing function
 * needed by the initial algorithm.
 */
typedef enum
{
  ZC_LT,    //!< ZC_LT
  ZC_LE,    //!< ZC_LE
  ZC_GT,    //!< ZC_GT
  ZC_GE     //!< ZC_GE
} ZC_REL;

/**
 *
 */
class MMO_Event
{
  public:
    /**
     *
     * @param cond
     * @param data
     */
    MMO_Event(AST_Expression cond);
    /**
     *
     */
    ~MMO_Event();
};

#endif  /*  MMO_EVENT_H_ */
