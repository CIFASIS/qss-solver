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

#ifndef QSS_COMMANDS_H_
#define QSS_COMMANDS_H_

#include "qss_simulator.h"

/**
 *
 */
void
QSS_CMD_terminate ();

/**
 *
 * @param simulator
 */
void
QSS_CMD_init (QSS_simulator simulator);

void
QSS_CMD_free ();

/**
 *
 */
void
QSS_CMD_alloc (QSS_simulator simulator);

#endif /* QSS_COMMANDS_H_ */
