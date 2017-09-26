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

#ifndef QSS_LP_H_
#define QSS_LP_H_

#include "qss_partition.h"
#include "qss_simulator.h"

/**
 *
 */
typedef struct LP_partitionInfo_ *LP_partitionInfo;

/**
 *
 */
struct LP_partitionInfo_
{
    int *stateInput; //!<
    int *handlerInput; //!<
    int *output; //!<
    int *stateReinits; //!<
    int nLPS; //!<
    int lps; //!<
    int states; //!<
    int handlers; //!<
    int outputs; //!<
    int stateInputs; //!<
    int handlerInputs; //!<
    int lastHandler; //!<
    int lastState; //!<
};

/**
 *
 * @param states
 * @param events
 * @param out
 * @return
 */
LP_partitionInfo
LP_PartitionInfo(int states, int events, int out);

/**
 *
 * @param simulator
 * @param partitions
 */
void
LP_initializeDataStructs(QSS_simulator simulator, PRT_partition partitions);

#endif /* QSS_LP_H_ */
