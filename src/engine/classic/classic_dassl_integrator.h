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

#pragma once

#include <common/simulator.h>

typedef void (*dae_system)(const double *t, const double *x, const double *dx, const double *CJ, double *res, int iRes, const double *rPar,
                           const int *iPar);

typedef void (*zero_crossing)(const int *n, const double *t, const double *x, const double *dx, const int *nrt, double *res,
                              const double *rPar, const int *iPar);

typedef void (*nullfunc)();

void ddaskr_(dae_system s, const int *noOfEquations, double *currentTime, double *x, double *dx, const double *finalTime, int info[20],
             double *relativeTolerance, double *absoluteTolerance, int *outputStatusFlag, double *dWorkArray, const int *lengthOfDWork,
             int *iWorkArray, const int *lengthOfIWork, const double *rParArray, const int *iParArray, nullfunc jac, nullfunc p,
             zero_crossing r, const int *numConstrains, int *numOfOutputs);

void DASSL_events(const int *n, const double *t, const double *x, const double *dx, const int *nrt, double *res, const double *rPar,
                  const int *iPar);

void DASSL_reset_x(double *x);

void DASSL_update_x(const double *x);

void DASSL_integrate(SIM_simulator simulator);
