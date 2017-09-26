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

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "data.h"

/**
 *
 */
typedef struct SIM_simulator_ *SIM_simulator;

/**
 *
 */
typedef struct SIM_simulatorState_ *SIM_simulatorState;

/**
 *
 */
typedef struct SIM_simulatorOps_ *SIM_simulatorOps;

/**
 *
 * @param
 */
typedef void
(*SIM_simulateFn)(SIM_simulator);

/**
 *
 * @param
 */
typedef void
(*SIM_freeSimulatorFn)(SIM_simulator);

/**
 *
 */
struct SIM_simulatorOps_
{
    SIM_simulateFn simulate; //!<
    SIM_freeSimulatorFn freeSimulator; //!<
};

/**
 *
 */
struct SIM_simulatorState_
{
    void *sim; //!<
    SD_simulationSettings settings; //!<
};

/**
 *
 */
struct SIM_simulator_
{
    SIM_simulatorState state; //!<
    SIM_simulatorOps ops; //!<
};

/**
 *
 * @param settings
 * @return
 */
SIM_simulator
SIM_Simulator(SD_simulationSettings settings);

/**
 *
 * @param simulator
 */
void
SIM_freeSimulator(SIM_simulator simulator);

/**
 *
 * @return
 */
SIM_simulatorOps
SIM_SimulatorOps();

/**
 *
 * @param ops
 */
void
SIM_freeSimulatorOps(SIM_simulatorOps ops);

/**
 *
 * @return
 */
SIM_simulatorState
SIM_SimulatorState();

/**
 *
 * @param state
 */
void
SIM_freeSimulatorState(SIM_simulatorState state);

/**
 *
 * @param simulator
 */
void
SIM_simulate(SIM_simulator simulator);

#endif /* SIMULATOR_H_ */
