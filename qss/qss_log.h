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

#ifndef QSS_LOG_H_
#define QSS_LOG_H_

#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"

/**
 *
 */
#define	NAME_SIZE  128

/**
 *
 */
typedef struct LG_log_ *LG_log;

/**
 *
 */
typedef struct LG_logOps_ *LG_logOps;

/**
 *
 */
typedef struct LG_logState_ *LG_logState;

/**
 *
 * @param
 * @param
 * @param
 */
typedef void
(*LG_writeFn)(LG_log, int, double, double);

/**
 *
 * @param
 * @param
 * @param
 */
typedef void
(*LG_writeLineFn)(LG_log, int, double, double*);

/**
 *
 */
typedef void
(*LG_toFileFn)();

/**
 *
 */
struct LG_log_
{
    LG_logState state; //!<
    LG_logOps ops; //!<
};

/**
 *
 */
struct LG_logState_
{
    list *states; //!<
    QSS_data data; //!<
    SD_output output; //!<
    FILE **files; //!<
    int size; //!<
    char fileName[NAME_SIZE]; //!<
    int *values; //!<
};

/**
 *
 */
struct LG_logOps_
{
    LG_writeFn write; //!<
    LG_writeLineFn writeLine; //!<
    LG_toFileFn toFile; //!<

};

/**
 *
 * @return
 */
LG_logState
LG_LogState();

/**
 *
 * @return
 */
LG_logOps
LG_LogOps();

/**
 *
 * @param simData
 * @param simOutput
 * @return
 */
LG_log
LG_Log(QSS_data simData, SD_output simOutput);

/**
 *
 * @param v
 */
void
LG_freeLog(LG_log v);

/**
 *
 * @param v
 */
void
LG_freeLogState(LG_logState v);

/**
 *
 * @param v
 */
void
LG_freeLogOps(LG_logOps v);

/**
 *
 * @param
 * @param
 * @param
 */
void
LG_write(LG_log log, int i, double time, double value);

/**
 *
 * @param
 * @param
 * @param
 */
void
LG_writeLine(LG_log log, int i, double time, double *value);

/**
 *
 */
void
LG_toFile(LG_log log);

#endif  /* QSS_LOG_H_ */
