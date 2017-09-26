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

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "simulator.h"

typedef struct INT_integrator_ *INT_integrator;

typedef struct INT_integratorOps_ *INT_integratorOps;

typedef void
(*INT_initializeFn)(SIM_simulator);

typedef void
(*INT_integrateFn)(SIM_simulator);

struct INT_integratorOps_
{
    INT_initializeFn initiliaze;
    INT_integrateFn integrate;
};

struct INT_integrator_
{
    INT_integratorOps ops; //!<
};

INT_integrator
INT_Integrator(SIM_simulator simulator);

void
INT_freeIntegrator(INT_integrator integrator);

INT_integratorOps
INT_IntegratorOps();

void
INT_freeIntegratorOps(INT_integratorOps ops);

void
INT_initialize(INT_integrator integrator, SIM_simulator simulator);

void
INT_integrate(INT_integrator integrator, SIM_simulator simulator);

#endif /* INTEGRATOR_H_ */
