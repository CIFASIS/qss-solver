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

#ifndef QSS_STEP_H_
#define QSS_STEP_H_

#include "../common/data.h"
#include "qss_data.h"
#include "qss_output.h"

/**
 *
 * @param QSS_data
 * @param QSS_time
 * @param SD_output
 */
void
ST_write (OUT_output output, QSS_data simData, QSS_time simTime, SD_output simOutput);

/**
 *
 * @param
 * @return
 */
int
ST_getSteps (OUT_output output, int var);

/**
 *
 */
void
ST_save (OUT_output output);


/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
ST_init (OUT_output output, QSS_data simData, QSS_time simTime, SD_output simOutput);


/**
 *
 * @param QSS_data
 * @param QSS_time
 * @param SD_output
 */
void
ST_PAR_write (OUT_output output, QSS_data simData, QSS_time simTime, SD_output simOutput);

/**
 *
 * @param
 * @return
 */
int
ST_PAR_getSteps (OUT_output output, int var);

/**
 *
 */
void
ST_PAR_save (OUT_output output);

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
ST_PAR_init (OUT_output output, QSS_data simData, QSS_time simTime, SD_output simOutput);

#endif  /* QSS_STEP_H_ */
