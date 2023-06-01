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

#ifndef mLIQSS3_H_
#define mLIQSS3_H_

#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

extern void mLIQSS3_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                       double *q) __attribute__((hot));

extern void mLIQSS3_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
    __attribute__((hot));

extern void mLIQSS3_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu) __attribute__((hot));

extern void mLIQSS3_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu) __attribute__((hot));

extern void mLIQSS3_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);

extern void mLIQSS3_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                           double *q) __attribute__((hot));

extern void mLIQSS3_PAR_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
    __attribute__((hot));

extern void mLIQSS3_PAR_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu) __attribute__((hot));

double qsolver_liqss2(QA_quantizer quantizer, double *x, double *u, double A, double dQ, double *q);

void solve_2x2(QA_quantizer quantizer, int i, int j, double xi0, double xj0, double qi0, double *qj, double *h);

void old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx);

extern void mLIQSS3_PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu) __attribute__((hot));

extern void mLIQSS3_PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime);

extern void mLIQSS3_PAR_AxB(double A[2][2], double B[2][2], double AB[2][2]);

extern void mLIQSS3_AxB(double A[2][2], double B[2][2], double AB[2][2]);

extern void mLIQSS3_PAR_invA(double A[2][2], double invA[2][2]);

extern void mLIQSS3_invA(double A[2][2], double invA[2][2]);

extern void mLIQSS3_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0,
                                    double *lqu);

extern void mLIQSS3_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0, double *lqu);

extern void mLIQSS3_PAR_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx);

extern void mLIQSS3_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx);

extern void mLIQSS3_PAR_solve_single(QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2);

extern void mLIQSS3_solve_single(QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2);

#endif /* mLIQSS3_H_ */
