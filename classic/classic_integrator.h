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

/*! \file qss_integrator.h 
 **	\brief This interface defines the Integrator used by the QSS Solver.
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

#ifndef CLASSIC_INTEGRATOR_H_
#define CLASSIC_INTEGRATOR_H_

#include "../common/data.h"
#include "../common/simulator.h"
#include "classic_data.h"

/**
 *
 */
#define MAX_OUTPUT_POINTS 1<<20

#define HIST 1e-12

/**
 *
 * @param simOutput
 * @param solution
 * @param solution_time
 * @param totalOutputSteps
 */
void
CLC_write_output(SD_output simOutput, double **solution, double *solution_time,
    int totalOutputSteps);
/**
 *
 * @param clcData
 * @param simOutput
 * @param solution
 * @param num_steps
 * @param outvar
 * @param x
 */
void
CLC_compute_outputs(SD_output simOutput, double **solution, int num_steps);

/**
 *
 * @param simOutput
 * @param solution
 * @param solution_time
 * @param t
 * @param totalOutputSteps
 * @param x
 * @param outvar
 */
void
CLC_save_step(SD_output simOutput, double **solution, double *solution_time,
    const double t, const int totalOutputSteps, double *x, double *d,
    double *a);
/**
 *
 * @param clcData
 * @param clcModel
 * @param x
 * @param root_output
 * @param t
 * @param iwork
 */
void
CLC_handle_event(CLC_data clcData, CLC_model clcModel, double *x,
    int *root_output, double t, int *iwork);

void
CLC_initialize(SIM_simulator simulate);

#endif  /* CLASSIC_INTEGRATOR_H_ */
