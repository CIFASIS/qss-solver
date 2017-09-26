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

#ifndef QSS_SAMPLED_H_
#define QSS_SAMPLED_H_

#include "../common/data.h"
#include "qss_data.h"
#include "qss_output.h"

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
SAM_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput);
/**
 *
 * @param var
 * @return
 */
int
SAM_getSteps(OUT_output output, int var);

/**
 *
 */
void
SAM_save(OUT_output output);

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
SAM_init(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput);

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
SAM_PAR_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput);
/**
 *
 * @param var
 * @return
 */
int
SAM_PAR_getSteps(OUT_output output, int var);

/**
 *
 */
void
SAM_PAR_save(OUT_output output);

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
SAM_PAR_init(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput);

#endif  /* QSS_SAMPLED_H_ */
