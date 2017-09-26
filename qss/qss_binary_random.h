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

#ifndef QSS_BINARY_RANDOM_H_
#define QSS_BINARY_RANDOM_H_

#include "qss_data.h"
#include "qss_scheduler.h"

void
BTR_update(SC_scheduler scheduler, QSS_data simData, QSS_time simTime);
/**
 *
 */
void
BTR_free(void);
/**
 *
 * @param simData
 * @param simTime
 */
void
BTR_init(SC_scheduler scheduler, QSS_data simData, QSS_time simTime);

#endif  /* QSS_BINARY_RANDOM_H_ */
