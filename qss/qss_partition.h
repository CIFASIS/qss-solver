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

#ifndef QSS_PARTITION_H_
#define QSS_PARTITION_H_

#include "qss_data.h"

/**
 *
 */
typedef struct PRT_partition_ *PRT_partition;

/**
 *
 */
struct PRT_partition_
{
    long int *values; //!<
    int *nOutputs; //!<
    int *nDsc;
    int **dscInf;
    int **asgDscInf;
    int **outputs; //!<
    int beginStates; //!<
    int endStates; //!<
    int beginHandlers; //!<
    int endHandlers; //!<
    int lps;
};

/**
 *
 * @param data
 * @return
 */
PRT_partition
PRT_Partition(QSS_data data, char *name);

/**
 *
 * @param partition
 */
void
PRT_freePartition(PRT_partition partition);

#endif /* QSS_PARTITION_H_ */
