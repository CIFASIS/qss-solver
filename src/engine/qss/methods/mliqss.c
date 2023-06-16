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

void QSS_FUNC_DECL(mLIQSS, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
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
  QSS_ASSIGN_QUANTIZER_OPS(mLIQSS)
}

void QSS_FUNC_DECL(mLIQSS, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                               double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(mLIQSS, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(mLIQSS, recomputeNextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
{
  int i0 = i * 2, i1 = i0 + 1;
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int ifr_state = quantizer->state->lSimTime->minIndex;
  bool state_step = quantizer->state->lSimTime->type == ST_State;
  double diff_Q;
  bool *change = quantizer->state->change;

  if (t > 0 && state_step) {
    diff_Q = q[2 * ifr_state] - quantizer->state->qAux[ifr_state];
    if (diff_Q) {
      A[i][ifr_state] = (x[i1] - quantizer->state->oldDx[i]) / diff_Q;
    } else {
      A[i][ifr_state] = 0;
    }
    U[i][i] = x[i1] - q[i0] * A[i][i];
  } else {
    U[i][i] = x[i1];
  }

  if (change[i] == TRUE) {
    nTime[i] = t;
  } else {
    double dt1 = 0;
    if (x[i1] != 0) {
      dt1 = (q[i0] - x[i0]) / x[i1];
      if (dt1 > 0) {
        nTime[i] = t + dt1;
      } else {
        if (x[i1] > 0) {
          nTime[i] = t + (q[i0] + 2 * lqu[i] - x[i0]) / x[i1];
        } else {
          nTime[i] = t + (q[i0] - 2 * lqu[i] - x[i0]) / x[i1];
        }
      }
    } else {
      nTime[i] = INF;
    }
    if (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
      nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
    }
  }
}

void QSS_FUNC_DECL(mLIQSS, nextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
{
  int i1 = i * 2 + 1;
  if (x[i1] == 0) {
    nTime[i] = INF;
  } else {
    nTime[i] = t + fabs(lqu[i] / x[i1]);
  }
}

void QSS_FUNC_DECL(mLIQSS, solver2x2_h)(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
{
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int i0 = 2 * i;
  int j0 = 2 * j;
  double Ad[2][2] = {{1 - h * A[i][i], -h * A[i][j]}, {-h * A[j][i], 1 - h * A[j][j]}};
  double det_Ad = Ad[0][0] * Ad[1][1] - Ad[0][1] * Ad[1][0];
  if (det_Ad == 0) {
    det_Ad = 1e-10;
  }
  double invAd[2][2] = {{Ad[1][1] / det_Ad, -Ad[0][1] / det_Ad}, {-Ad[1][0] / det_Ad, Ad[0][0] / det_Ad}};
  q[i0] = invAd[0][0] * (x[i0] + h * U[i][j]) + invAd[0][1] * (xj0 + h * U[j][i]);
  qj[j0] = invAd[1][0] * (x[i0] + h * U[i][j]) + invAd[1][1] * (xj0 + h * U[j][i]);
}

void QSS_FUNC_DECL(mLIQSS, old_dx)(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
{
  int inf_var, j, j1;
  if (t > 0) {
    for (inf_var = 0; inf_var < nSD; inf_var++) {
      j = quantizer->state->SD[i][inf_var];
      j1 = j * 2 + 1;
      quantizer->state->oldDx[j] = x[j1];
      tx[j] = t;
    }
  }
}

void QSS_FUNC_DECL(mLIQSS, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  bool *change = quantizer->state->change;
  double *qj = quantizer->state->qj;
  double t = quantizer->state->lSimTime->time;
  double **U = quantizer->state->U0;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  double *q_prev = quantizer->state->qAux;
  double *dx_prev = quantizer->state->oldDx;
  int i0 = i * 2, i1 = i0 + 1;
  int inf_var;
  // Number of influenced state derivatives for x[i]
  int nSD = quantizer->state->nSD[i];
  double dx;
  double *dq = quantizer->state->dq;
  // Set h_max as INF.
  double h = INF;

  // Store old state and quetized variables.
  q_prev[i] = q[i0];
  dx_prev[i] = x[i1];
  tx[i] = t;
  if (change[i] == TRUE) {
    q[i0] = qj[i0];
    change[i] = FALSE;
  } else {
    double qi_prev = q[i0];
    // LIQSS1
    q[i0] = x[i0];
    // Compute the future value of dx and check for sign changes.
    int der_sign = sign(x[i1]);
    dx = (q[i0] + der_sign * lqu[i]) * A[i][i] + U[i][i];
    if (der_sign * dx > 0) {
      dq[i] = der_sign * lqu[i];
    } else {
      dq[i] = -U[i][i] / A[i][i] - q[i0];
    }
    if (dq[i] > 2 * der_sign * lqu[i]) {
      dq[i] = 2 * der_sign * lqu[i];
    }
    q[i0] += dq[i];

    // mLIQSS control algorithm
    if (t > 0) {
      // For every state derivative influenced by x[i]
      for (inf_var = 0; inf_var < nSD; inf_var++) {
        int j = quantizer->state->SD[i][inf_var];
        int j0 = j * 2, j1 = j0 + 1;
        if (j != i && A[i][j] * A[j][i] != 0) {
          U[j][i] = U[j][j] - A[j][i] * q_prev[i];
          double dxj = A[j][i] * q[i0] + A[j][j] * q[j0] + U[j][i];
          if (fabs(x[j1] - dxj) > fabs(x[j1] + dxj) / 2) {
            double elapsed = t - tx[j];
            double xj0 = x[j0] + elapsed * x[j1];
            double qj_prev = q[j0];
            U[i][j] = U[i][i] - A[i][j] * q[j0];
            if (dxj > 0) {
              q[j0] = xj0 + 2 * lqu[j];
            } else {
              q[j0] = xj0 - 2 * lqu[j];
            }
            double dxi = A[i][i] * q[i0] + A[i][j] * q[j0] + U[i][j];
            if (x[i1] * dxi > 0 && fabs(x[i1] * dxi) > 1e-30) {
              if (dxj > 0) {
                q[j0] = xj0 - 2 * lqu[j];
              } else {
                q[j0] = xj0 + 2 * lqu[j];
              }
              dxi = A[i][i] * q[i0] + A[i][j] * q[j0] + U[i][j];
            }
            q[j0] = qj_prev;
            double a1 = A[i][i] * A[j][j];
            double a2 = A[i][j] * A[j][i];
            bool dA = FALSE;
            double tol = 1e-1;
            if (a2 != 0 && a1 / a2 < 1 + tol && a1 / a2 > 1 - tol) {
              dA = TRUE;
            }
            if (fabs(x[i1] - dxi) > fabs(x[i1] + dxi) / 2 || dA) {
              qi_prev = q[i0];
              qj_prev = q[j0];
              qj[j0] = 1e20;
              if (!dA) {
                if (fabs(A[j][j]) > 1e-30) {
                  q[i0] = (A[i][j] * U[j][i] - A[j][j] * U[i][j]) / (A[i][i] * A[j][j] - A[i][j] * A[j][i]);
                  qj[j0] = (-A[j][i] * q[i0] - U[j][i]) / A[j][j];
                  if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                    change[j] = TRUE;
                    break;
                  } else {
                    q[i0] = qi_prev;
                    q[j0] = qj_prev;
                    qj[j0] = 1e20;
                  }
                }
              }
              h = (quantizer->state->finTime - t);
              QSS_FUNC_INVK(mLIQSS, solver2x2_h)(quantizer, x, q, qj, i, j, h, xj0);
              if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                change[j] = TRUE;
                break;
              } else {
                if (fabs(q[i0] - x[i0]) / lqu[i] > fabs(qj[j0] - xj0) / lqu[j]) {
                  h = h / (fabs(q[i0] - x[i0]) / lqu[i]) * 0.9;
                } else {
                  h = h / (fabs(qj[j0] - xj0) / lqu[j]) * 0.9;
                }
                if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                  change[j] = TRUE;
                  break;
                } else {
                  if (x[i1] != 0 && x[j1] != 0) {
                    if (fabs(TOL * lqu[i] / x[i1]) < fabs(TOL * lqu[j] / x[j1])) {
                      h = fabs(TOL * lqu[i] / x[i1]);
                    } else {
                      h = fabs(TOL * lqu[j] / x[j1]);
                    }
                  } else if (x[i1] != 0) {
                    h = fabs(TOL * lqu[i] / x[i1]);
                  } else if (x[j1] != 0) {
                    h = fabs(TOL * lqu[j] / x[j1]);
                  }
                  QSS_FUNC_INVK(mLIQSS, solver2x2_h)(quantizer, x, q, qj, i, j, h, xj0);
                  if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - xj0) < TOL * lqu[j]) {
                    change[j] = TRUE;
                    break;
                  } else {
                    q[i0] = qi_prev;
                    q[j0] = qj_prev;
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
  QSS_FUNC_INVK(mLIQSS, old_dx)(quantizer, i, t, nSD, x, tx);
}
