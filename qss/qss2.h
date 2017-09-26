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

#ifndef QSS2_H_
#define QSS2_H_

#include "qss_data.h"
#include "qss_quantizer.h"

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
QSS2_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
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
QSS2_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
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
QSS2_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param i
 * @param q
 * @param x
 * @param lqu
 */
void
QSS2_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x,
    double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param simData
 * @param simTime
 */
void
QSS2_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);

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
QSS2_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf,
    double t, double *nTime, double *x, double *lqu, double *q)
        __attribute__((hot));
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
QSS2_PAR_recomputeNextTime(QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q) __attribute__((hot));
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
QSS2_PAR_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param i
 * @param q
 * @param x
 * @param lqu
 */
void
QSS2_PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q,
    double *x, double *lqu) __attribute__((hot));
/**
 *
 * @param quantizer
 * @param simData
 * @param simTime
 */
void
QSS2_PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);

#endif  /* QSS2_H_ */
