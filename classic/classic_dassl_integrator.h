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

#ifndef DASSL_INTEGRATOR_H_
#define DASSL_INTEGRATOR_H_

#include "../common/simulator.h"

/**
 *
 * @param t
 * @param x
 * @param dx
 * @param CJ
 * @param res
 * @param iRes
 * @param rPar
 * @param iPar
 */
typedef void
(*dae_system)(const double *t, const double *x, const double *dx,
    const double *CJ, double *res, int iRes, const double *rPar,
    const int *iPar);
/**
 *
 * @param n
 * @param t
 * @param x
 * @param dx
 * @param nrt
 * @param res
 * @param rPar
 * @param iPar
 */
typedef void
(*zero_crossing)(const int *n, const double *t, const double *x,
    const double *dx, const int *nrt, double *res, const double *rPar,
    const int *iPar);
/**
 *
 */
typedef void
(*nullfunc)();

/**
 *
 * @param s
 * @param noOfEquations
 * @param currentTime
 * @param x
 * @param dx
 * @param finalTime
 * @param info
 * @param relativeTolerance
 * @param absoluteTolerance
 * @param outputStatusFlag
 * @param dWorkArray
 * @param lengthOfDWork
 * @param iWorkArray
 * @param lengthOfIWork
 * @param rParArray
 * @param iParArray
 * @param jac
 * @param p
 * @param r
 * @param numConstrains
 * @param numOfOutputs
 */
void
ddaskr_(dae_system s,                 // 1
    const int *noOfEquations, // 2
    double *currentTime,      // 3
    double *x,                // 4
    double *dx,               // 5
    const double *finalTime,  // 6
    int info[20],             // 7
    double *relativeTolerance,             // 8
    double *absoluteTolerance,             // 9
    int *outputStatusFlag,    // 10
    double *dWorkArray,       // 11
    const int *lengthOfDWork, // 12
    int *iWorkArray,          // 13
    const int *lengthOfIWork, // 14
    const double *rParArray,  // 15
    const int *iParArray,     // 16
    nullfunc jac,             // 17
    nullfunc p,               // 18
    zero_crossing r,              // 19
    const int *numConstrains, // 20
    int *numOfOutputs         // 21
    );
/**
 *
 * @param n
 * @param t
 * @param x
 * @param dx
 * @param nrt
 * @param res
 * @param rPar
 * @param iPar
 */
void
DASSL_events(const int *n, const double *t, const double *x, const double *dx,
    const int *nrt, double *res, const double *rPar, const int *iPar);

/**
 *
 * @param x
 */
void
DASSL_reset_x(double *x);
/**
 *
 * @param x
 */
void
DASSL_update_x(const double *x);
/**
 *
 */
void
DASSL_integrate(SIM_simulator simulator);

#endif  /* DASSL_INTEGRATOR_H_ */
