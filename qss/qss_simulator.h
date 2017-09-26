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

#ifndef QSS_SIMULATOR_H_
#define QSS_SIMULATOR_H_

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_dt.h"
#include "qss_frw.h"
#include "qss_output.h"
#include "qss_quantizer.h"
#include "qss_scheduler.h"
#include "qss_sim_steps.h"

/**
 *
 */
typedef struct QSS_simulator_ *QSS_simulator;

/**
 *
 */
struct QSS_simulator_
{
    QA_quantizer quantizer;  //!<
    OUT_output log;  //!<
    SC_scheduler scheduler;  //!<
    FRW_framework frw;  //!<
    QSS_data data;  //!<
    QSS_time time;  //!<
    QSS_model model;  //!<
    SD_output output;  //!<
    SD_simulationSettings settings;  //!<
    SD_simulationLog simulationLog;  //!<
    QSS_dt dt; //!<
    QSS_dtSynch dtSynch;
    int id;  //!< ID of the simulator.
    double *lpTime; //!< Array that contains the time of the last step taken in the LP or \f $ \inf $ $ \f otherwise.
    double *lpDtMin; //!< Array that contains the minimum step time for each LP defined in the simulation.
    MLB_mailbox mailbox; //!<
    IBX_inbox inbox; //!<
    IBX_inbox ack; //!<
    QSS_LP_dataArray lps; //!<
    SD_statistics stats; //!<
    QSS_simSteps simSteps; //!<
};

/**
 *
 */
typedef struct QSS_simulatorInstance_ QSS_simulatorInstance;

/**
 *
 */
struct QSS_simulatorInstance_
{
    QSS_simulator root; //!<
    int id; //!<
};

/**
 *
 * @return
 */
QSS_simulator
QSS_Simulator();

/**
 *
 * @param simulator
 */
void
QSS_initSimulator(SIM_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_freeSimulator(QSS_simulator simulator);

#endif /* QSS_SIMULATOR_H_ */
