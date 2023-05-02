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
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <common/utils.h>
#include <qss/methods/mliqss.h>

#define TOL 2

#ifdef QSS_PARALLEL
void mLIQSS_PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void mLIQSS_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int i, j;
  int states = simData->states;
  quantizer->state->dq = (double *)malloc(states * sizeof(double));
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->a = (double *)malloc(states * sizeof(double));
  quantizer->state->u0 = (double *)malloc(states * sizeof(double));
  quantizer->state->lt = (double *)malloc(states * sizeof(double));
  quantizer->state->A = (double **)malloc(states * sizeof(double *));
  quantizer->state->U0 = (double **)malloc(states * sizeof(double *));
  for (i = 0; i < states; i++) {
    quantizer->state->A[i] = (double *)malloc(states * sizeof(double));
    quantizer->state->U0[i] = (double *)malloc(states * sizeof(double));
  }
  quantizer->state->change = (bool *)malloc(states * sizeof(bool));
  quantizer->state->qj = (double *)malloc(2 * states * sizeof(double));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));
  quantizer->state->finTime = simData->ft;
  quantizer->state->flag2 = (int *)malloc(states * sizeof(int));
  for (i = 0; i < states; i++) {
    int i0 = i * 2;
    // simData->tmp1[i0] = simData->x[i0];
    simData->q[i0] = simData->x[i0];
    quantizer->state->qAux[i] = simData->x[i0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->dq[i] = 0;
    quantizer->state->change[i] = FALSE;
    quantizer->state->qj[i] = 0;
    quantizer->state->flag2[i] = 0;
    for (j = 0; j < states; j++) {
      quantizer->state->A[i][j] = 0;
      quantizer->state->U0[i][j] = 0;
    }
  }
  quantizer->state->simTime = &simTime->time;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  quantizer->state->minStep = simData->params->minStep;
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = mLIQSS_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = mLIQSS_PAR_recomputeNextTime;
  quantizer->ops->nextTime = mLIQSS_PAR_nextTime;
  quantizer->ops->updateQuantizedState = mLIQSS_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = mLIQSS_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = mLIQSS_recomputeNextTime;
  quantizer->ops->nextTime = mLIQSS_nextTime;
  quantizer->ops->updateQuantizedState = mLIQSS_updateQuantizedState;
#endif
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu, double *q)
#else
void mLIQSS_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu, double *q)
#endif
{
  int i;
#ifdef QSS_PARALLEL
  int *map = quantizer->state->qMap;
#endif
  for (i = 0; i < vars; i++) {
#ifdef QSS_PARALLEL
    if (map[inf[i]] != NOT_ASSIGNED) {
#endif
      mLIQSS_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
    }
#endif
  }
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#else
void mLIQSS_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#endif
{
  int i0 = i * 2, i1 = i0 + 1;
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int stind = quantizer->state->lSimTime->minIndex;
  double diffQ;
  bool *change = quantizer->state->change;

  if (t > 0) {
    diffQ = q[2 * stind] - quantizer->state->qAux[stind];
    if (diffQ)
      A[i][stind] = (x[i1] - quantizer->state->oldDx[i]) / diffQ;
    else
      A[i][stind] = 0;
    U[i][i] = x[i1] - q[i0] * A[i][i];
  } else
    U[i][i] = x[i1];

  if (change[i] == TRUE)
    nTime[i] = t;
  else {
    double dt1 = 0;
    if (x[i1] != 0) {
      dt1 = (q[i0] - x[i0]) / x[i1];
      if (dt1 > 0)
        nTime[i] = t + dt1;
      else {
        if (x[i1] > 0)
          nTime[i] = t + (q[i0] + 2 * lqu[i] - x[i0]) / x[i1];
        else
          nTime[i] = t + (q[i0] - 2 * lqu[i] - x[i0]) / x[i1];
      }
    } else
      nTime[i] = INF;
    if (fabs(q[i0] - x[i0]) > 2 * lqu[i]) nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
  }
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_nextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
#else
void mLIQSS_nextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
#endif
{
  int i1 = i * 2 + 1;
  if (x[i1] == 0)
    nTime[i] = INF;
  else
    nTime[i] = t + fabs(lqu[i] / x[i1]);
}

#ifdef QSS_PARALLEL
double mLIQSS_PAR_solver2x2(QA_quantizer quantizer, double *x, double *q, int i, int j, double *h)
#else
double mLIQSS_solver2x2(QA_quantizer quantizer, double *x, double *q, int i, int j, double *h)
#endif
{
  double qBE;
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int i0 = 2 * i;
  int j0 = 2 * j;
  double K1, K2, K3, coef[3];

  K1 = A[j][i] * q[i0] + U[j][i];
  K2 = A[i][i] * q[i0] + U[i][j];
  K3 = x[i0] - q[i0];
  coef[2] = -A[i][j] * K1 - A[j][j] * K2;
  coef[1] = -A[j][j] * K3 + A[i][j] * x[j0] + K2;
  coef[0] = K3;
  *h = minPosRoot(coef, 3);
  qBE = (x[j0] + *h * K1) / (1 - *h * A[j][j]);
  return qBE;
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
#else
void mLIQSS_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
#endif
{
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int i0 = 2 * i;
  int j0 = 2 * j;
  double Ad[2][2] = {{1 - h * A[i][i], -h * A[i][j]}, {-h * A[j][i], 1 - h * A[j][j]}};
  double detAd = Ad[0][0] * Ad[1][1] - Ad[0][1] * Ad[1][0];
  if (detAd == 0) detAd = 1e-10;
  double invAd[2][2] = {{Ad[1][1] / detAd, -Ad[0][1] / detAd}, {-Ad[1][0] / detAd, Ad[0][0] / detAd}};
  q[i0] = invAd[0][0] * (x[i0] + h * U[i][j]) + invAd[0][1] * (xj0 + h * U[j][i]);
  qj[j0] = invAd[1][0] * (x[i0] + h * U[i][j]) + invAd[1][1] * (xj0 + h * U[j][i]);
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
#else
void mLIQSS_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
#endif
{
  int m, j, j0, j1;
  if (t > 0) {
    for (m = 0; m < nSD; m++) {
      j = quantizer->state->SD[i][m];
      j0 = j * 2;
      j1 = j0 + 1;
      quantizer->state->oldDx[j] = x[j1];
      tx[j] = t;
    }
  }
}

#ifdef QSS_PARALLEL
void mLIQSS_PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#else
void mLIQSS_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#endif
{
  bool *change = quantizer->state->change;
  double *qj = quantizer->state->qj;
  double t = quantizer->state->lSimTime->time;
  double **U = quantizer->state->U0;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  int i0 = i * 2, i1 = i0 + 1;
  int m;
  int nSD = quantizer->state->nSD[i];
  double dx;
  double *dq = quantizer->state->dq;
  double h = 1e30;

  quantizer->state->qAux[i] = q[i0];
  quantizer->state->oldDx[i] = x[i1];
  tx[i] = t;
  if (change[i] == TRUE) {
    q[i0] = qj[i0];
    change[i] = FALSE;
  } else {
    double qiold = q[i0];

    // LIQSS1
    q[i0] = x[i0];
    if (x[i1] > 0) {
      dx = U[i][i] + (q[i0] + lqu[i]) * A[i][i];
      if (dx > 0)
        dq[i] = lqu[i];
      else
        dq[i] = -U[i][i] / A[i][i] - q[i0];
    } else {
      dx = U[i][i] + (q[i0] - lqu[i]) * A[i][i];
      if (dx < 0)
        dq[i] = -lqu[i];
      else
        dq[i] = -U[i][i] / A[i][i] - q[i0];
    }
    if (dq[i] > 2 * lqu[i]) dq[i] = 2 * lqu[i];
    if (dq[i] < -2 * lqu[i]) dq[i] = -2 * lqu[i];
    q[i0] += dq[i];

    if (t > 0) {
      for (m = 0; m < nSD; m++) {
        int j = quantizer->state->SD[i][m];
        int j0 = j * 2;
        int j1 = j0 + 1;
        if (j != i && A[i][j] * A[j][i] != 0) {
          U[j][i] = U[j][j] - A[j][i] * quantizer->state->qAux[i];
          double dxj = A[j][i] * q[i0] + A[j][j] * q[j0] + U[j][i];
          // if (x[j1]*dxj<=0 || fabs(x[j1]*dxj)<1e-30)
          if (fabs(x[j1] - dxj) > fabs(x[j1] + dxj) / 2) {
            double xj0 = x[j0] + (t - tx[j]) * x[j1];
            double qjold = q[j0];  // guardado qj
            U[i][j] = U[i][i] - A[i][j] * q[j0];
            if (dxj > 0)
              q[j0] = xj0 + 2 * lqu[j];
            else
              q[j0] = xj0 - 2 * lqu[j];
            double dxi = A[i][i] * q[i0] + A[i][j] * q[j0] + U[i][j];
            if (x[i1] * dxi > 0 && fabs(x[i1] * dxi) > 1e-30) {
              if (dxj > 0)
                q[j0] = xj0 - 2 * lqu[j];
              else
                q[j0] = xj0 + 2 * lqu[j];
              dxi = A[i][i] * q[i0] + A[i][j] * q[j0] + U[i][j];
            }
            q[j0] = qjold;
            double a1 = A[i][i] * A[j][j];
            double a2 = A[i][j] * A[j][i];
            bool dA = FALSE;
            double tol = 1e-1;
            if (a2 != 0 && a1 / a2 < 1 + tol && a1 / a2 > 1 - tol) dA = TRUE;
            // if (x[i1]*dxi<=0 || fabs(x[i1]*dxi)<1e-30 || dA)
            if (fabs(x[i1] - dxi) > fabs(x[i1] + dxi) / 2 || dA) {
              qiold = q[i0];
              qjold = q[j0];
              qj[j0] = 1e20;
              if (!dA) {
                if (fabs(A[j][j]) > 1e-30) {
                  q[i0] = (A[i][j] * U[j][i] - A[j][j] * U[i][j]) / (A[i][i] * A[j][j] - A[i][j] * A[j][i]);
                  qj[j0] = (-A[j][i] * q[i0] - U[j][i]) / A[j][j];
                  if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                    change[j] = TRUE;
                    m = nSD;
                    break;
                  } else {
                    q[i0] = qiold;
                    q[j0] = qjold;
                    qj[j0] = 1e20;
                  }
                }
              }
              h = (quantizer->state->finTime - t);
              mLIQSS_PAR_solver2x2_h(quantizer, x, q, qj, i, j, h, xj0);
              if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                change[j] = TRUE;
                m = nSD;
                break;
              } else {
                if (fabs(q[i0] - x[i0]) / lqu[i] > fabs(qj[j0] - xj0) / lqu[j])
                  h = h / (fabs(q[i0] - x[i0]) / lqu[i]) * 0.9;
                else
                  h = h / (fabs(qj[j0] - xj0) / lqu[j]) * 0.9;
                if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                  change[j] = TRUE;
                  m = nSD;
                  break;
                } else {
                  if (x[i1] != 0 && x[j1] != 0) {
                    if (fabs(TOL * lqu[i] / x[i1]) < fabs(TOL * lqu[j] / x[j1]))
                      h = fabs(TOL * lqu[i] / x[i1]);
                    else
                      h = fabs(TOL * lqu[j] / x[j1]);
                  } else if (x[i1] != 0)
                    h = fabs(TOL * lqu[i] / x[i1]);
                  else if (x[j1] != 0)
                    h = fabs(TOL * lqu[j] / x[j1]);
                  mLIQSS_PAR_solver2x2_h(quantizer, x, q, qj, i, j, h, xj0);
                  if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                    change[j] = TRUE;
                    m = nSD;
                    break;
                  } else {
                    q[i0] = qiold;
                    q[j0] = qjold;
                    qj[j0] = 1e20;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  mLIQSS_old_dx(quantizer, i, t, nSD, x, tx);
}
