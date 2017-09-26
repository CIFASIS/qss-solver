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

/** @file qss_integrator.h
 **	@brief This interface defines the Integrator used by the QSS Solver.
 **
 **	Given the state polynomials, the integrator uses structural information
 **	defined in the Model interface, in order to compute \f$ \dot{x} = f(q,t) \f$
 **	\begin{itemize}
 **  	\item Integrating the state derivatives $\dot x_i(t)$ to obtain the {\bf{state trajectories}} $x_i(t)$.
 **   	\item Advancing the {\bf{simulation time}}.
 **    	\item Deciding when each state derivative and zero--crossing function should be re--evaluated.
 **     	\item Searching for {\bf{discontinuities}}.
 ** 	 	\item Invoking the model's discontinuity handlers when zero crossings are detected.
 **  \end{itemize}
 **	\f$ x = a_0 + a_1 t + a_2 t^2 + ... + a_{n} t^{n} \f$
 **
 **	\f$ q = a_0 + a_1 t + a_2 t^2 + ... + a_{n-1} t^{n-1} \f$
 **
 **/

#ifndef QSS_INTEGRATOR_H_
#define QSS_INTEGRATOR_H_

#include "../common/simulator.h"
#include "../common/utils.h"
#include "qss_simulator.h"

/**
 *
 * @param
 * @param
 */
typedef void
(*QSS_externaEventHandler)(QSS_simulator, IBX_message);

typedef void
(*QSS_internalEventHandler)(QSS_simulator);

/**
 *
 * @param
 */
typedef void
(*QSS_parallelIntegrator)(QSS_simulator);

/**
 *
 * @param simulator
 */
void
QSS_SEQ_saveLog(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_SEQ_printSimulationLog(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_SEQ_logMemory(QSS_simulator simulator);

/**
 *
 * @param simulate
 */
void
QSS_SEQ_initialize(SIM_simulator simulate);

/**
 *
 * @param simulator
 */
void
QSS_PAR_saveLog(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_printSimulationLog(QSS_simulator simulator);

/**
 *
 * @param simulator
 * @param code
 */
void
QSS_PAR_printParallelLog(QSS_simulator simulator, int code);

/**
 *
 * @param simulator
 */
void
QSS_PAR_statistics(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_allocRootSimulatorData(QSS_simulator simulator);

/**
 *
 * @param instance
 * @return
 */
QSS_simulator
QSS_PAR_copySimulator(QSS_simulatorInstance *instance);

/**
 *
 * @param simulator
 * @return
 */
double
QSS_PAR_GVT(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_removePendingMessages(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_controlPassiveLPS(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_initializeSimulation(QSS_simulator simulator);

/**
 *
 * @param simulator
 */
void
QSS_PAR_logMemory(QSS_simulator simulator);

/**
 *
 * @param simulate
 */
void
QSS_PAR_initialize(SIM_simulator simulate);

/**
 *
 * @param simulator
 * @param synchronize
 * @param externalEvent
 */
void
QSS_PAR_synchronize(QSS_simulator simulator, int synchronize,
    QSS_externaEventHandler externalEvent,
    QSS_internalEventHandler internalEvent);

/**
 *
 * @param simulator
 * @param externalEvent
 */
double
QSS_PAR_passiveInitialization(QSS_simulator simulator,
    QSS_externaEventHandler externalEvent);

/**
 *
 * @param simulator
 * @param externalEvent
 * @return
 */
double
QSS_PAR_passiveLP(QSS_simulator simulator,
    QSS_externaEventHandler externalEvent);

/**
 *
 * @param simulator
 */
void
QSS_PAR_waitFor(QSS_simulator simulator);

#endif /* QSS_INTEGRATOR_H_ */
