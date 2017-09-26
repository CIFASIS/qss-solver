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

#ifndef QSS_QUANTIZER_H_
#define QSS_QUANTIZER_H_

#include <qss/qss_data.h>
/**
 *
 */
typedef struct QA_quantizerOps_ *QA_quantizerOps;
/**
 *
 */
typedef struct QA_quantizerState_ *QA_quantizerState;
/**
 *
 */
typedef struct QA_quantizer_ *QA_quantizer;
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QA_recomputeNextTimesFn)(QA_quantizer, int, int *, double, double*, double*,
    double*, double*);
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QA_recomputeNextTimeFn)(QA_quantizer, int, double, double*, double*, double*,
    double*);
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QA_nextTimeFn)(QA_quantizer, int, double, double*, double*, double*);
/**
 *
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QA_updateQuantizedStateFn)(QA_quantizer, int, double *, double *, double *);
/**
 *
 */
struct QA_quantizerState_
{
    int order;
    int xOrder;
    double *dq; //!<
    double *a; //!<
    double *oldDx; //!<
    double *qAux; //!<
    double *u0; //!<
    double *u1; //!<
    double *u2; //!<
    double *lt; //!<
    double *ltq; //!<
    double *lquOld; //!<
    double *qinf; //!<
    double *qsup; //!<
    double *simTime; //!<
    double minStep; //!<
    double finTime; //!<
    int *flag2; //!<
    int *flag3; //!<
    int *flag4; //!<
    QSS_time lSimTime; //!<
    QSS_idxMap qMap; //!<
    double **A;
    double **U;
    int sts;
    int *nSD;
    int **SD;
    bool *change;
    double *next;
    double *nTime;
    bool band;
    double *qstate;
    double **U0;
    double **U1;
    double *tx;
    double cont[10];
};
/**
 *
 */
struct QA_quantizerOps_
{
    QA_recomputeNextTimesFn recomputeNextTimes; //!<
    QA_recomputeNextTimeFn recomputeNextTime; //!<
    QA_nextTimeFn nextTime; //!<
    QA_updateQuantizedStateFn updateQuantizedState; //!<
};
/**
 *
 */
struct QA_quantizer_
{
    QA_quantizerState state; //!<
    QA_quantizerOps ops; //!<
};
/**
 *
 * @return
 */
QA_quantizerOps
QA_QuantizerOps();
/**
 *
 * @return
 */
QA_quantizerState
QA_QuantizerState();
/**
 *
 * @param simData
 * @param simTime
 * @return
 */
QA_quantizer
QA_Quantizer(QSS_data simData, QSS_time simTime);
/**
 *
 * @param state
 */
void
QA_freeQuantizerState(QA_quantizerState state);
/**
 *
 * @param ops
 */
void
QA_freeQuantizerOps(QA_quantizerOps ops);
/**
 *
 * @param q
 */
void
QA_freeQuantizer(QA_quantizer quantizer);
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
QA_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double * lqu, double *q);
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
QA_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q);
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
QA_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x,
    double *lqu);
/**
 *
 * @param quantizer
 * @param x
 * @param q
 * @param x
 * @param lqu
 */
void
QA_updateQuantizedState(QA_quantizer quantizer, int var, double *q, double *x,
    double *lqu);

#endif  /* QSS_QUANTIZER_H_ */
