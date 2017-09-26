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

#ifndef QSS_OUTPUT_H_
#define QSS_OUTPUT_H_

#include "../common/data.h"
#include "qss_data.h"
#include "qss_log.h"

/**
 *
 */
typedef struct OUT_outputState_ *OUT_outputState;

/**
 *
 */
typedef struct OUT_outputOps_ *OUT_outputOps;

/**
 *
 */
typedef struct OUT_output_ *OUT_output;

/**
 *
 * @param QSS_data
 * @param QSS_time
 * @param SD_output
 */
typedef void
(*OUT_writeFn)(OUT_output, QSS_data, QSS_time, SD_output);

/**
 *
 * @param
 * @return
 */
typedef int
(*OUT_getStepsFn)(OUT_output, int);

/**
 *
 */
typedef void
(*OUT_saveFn)(OUT_output);

/**
 *
 */
struct OUT_outputState_
{
    LG_log log; //!<
    int *steps; //!<
    double *values; //!<
};

/**
 *
 */
struct OUT_outputOps_
{
    OUT_writeFn write; //!<
    OUT_getStepsFn getSteps; //!<
    OUT_saveFn save; //!<
};

/**
 *
 */
struct OUT_output_
{
    OUT_outputState state; //!<
    OUT_outputOps ops; //!<
};

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 * @return
 */
OUT_output
OUT_Output(QSS_data simData, QSS_time simTime, SD_output simOutput);

/**
 *
 * @return
 */
OUT_outputState
OUT_OutputState();

/**
 *
 * @return
 */
OUT_outputOps
OUT_OutputOps();

/**
 *
 * @param o
 */
void
OUT_freeOutput(OUT_output o);

/**
 *
 * @param o
 */
void
OUT_freeOutputState(OUT_outputState o);

/**
 *
 * @param o
 */
void
OUT_freeOutputOps(OUT_outputOps o);

/**
 *
 * @param output
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
OUT_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput);

/**
 *
 * @param
 * @param var
 * @return
 */
int
OUT_getSteps(OUT_output output, int var);

/**
 *
 * @param output
 */
void
OUT_save(OUT_output output);

#endif  /* QSS_OUTPUT_H_ */
