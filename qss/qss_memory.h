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

#ifndef QSS_MEMORY_H_
#define QSS_MEMORY_H_

#include "../common/data.h"
#include "qss_data.h"
#include "qss_log.h"

/**
 *
 * @param i
 * @param time
 * @param value
 */
void M_write(LG_log log, int i, double time, double value);

/**
 *
 * @param i
 * @param time
 * @param value
 */
void M_writeLine(LG_log log, int i, double time, double *value);

/**
 *
 */
void M_toFile(LG_log log);

/**
 *
 * @param simData
 * @param simOutput
 */
void M_init(LG_log log, QSS_data simData, SD_output simOutput);

/**
 *
 * @param i
 * @param time
 * @param value
 */
void M_PAR_write(LG_log log, int i, double time, double value);

/**
 *
 * @param i
 * @param time
 * @param value
 */
void M_PAR_writeLine(LG_log log, int i, double time, double *value);

/**
 *
 */
void M_PAR_toFile(LG_log log);

/**
 *
 * @param simData
 * @param simOutput
 */
void M_PAR_init(LG_log log, QSS_data simData, SD_output simOutput);

#endif /* QSS_MEMORY_H_ */
