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

#ifndef CQSS_H_
#define CQSS_H_

#include "qss_data.h"
#include "qss_quantizer.h"

/**
 * @brief Recomputes the next time of all the influenced variables of...
 *
 * @param quantizer
 * @param vars
 * @param inf
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 * @param q
 */
void
CQSS_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf,
    double t, double *nTime, double *x, double *lqu, double*q)
        __attribute__((hot));
/**
 *
 * @param quantizer
 * @param vars
 * @param inf
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 * @param q
 */
void
CQSS_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double*q) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 * @param q
 */
void
CQSS_PAR_recomputeNextTime(QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 * @param q
 */
void
CQSS_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 */
void
CQSS_PAR_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param t
 * @param nTime
 * @param x
 * @param lqu
 */
void
CQSS_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param q
 * @param x
 * @param lqu
 */
void
CQSS_PAR_updateQuantizedState(QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param var
 * @param q
 * @param x
 * @param lqu
 */
void
CQSS_updateQuantizedState(QA_quantizer quantizer, int var, double *q, double *x,
    double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param _simData
 * @param _simTime
 */
void
CQSS_PAR_init(QA_quantizer quantizer, QSS_data _simData, QSS_time _simTime);
/**
 *
 * @param quantizer
 * @param _simData
 * @param _simTime
 */
void
CQSS_init(QA_quantizer quantizer, QSS_data _simData, QSS_time _simTime);

#endif  /* CQSS_H_ */
