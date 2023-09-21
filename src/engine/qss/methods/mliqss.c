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
  quantizer->state->sim_step = (bool *)malloc(states * sizeof(bool));
  quantizer->state->qj = (double *)malloc(2 * states * sizeof(double));
  quantizer->state->finTime = simData->ft;
  quantizer->state->flag2 = (int *)malloc(states * sizeof(int));
  for (i = 0; i < states; i++) {
    int i0 = i * 2;
    simData->q[i0] = simData->x[i0];
    quantizer->state->qAux[i] = simData->x[i0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->sim_step[i] = FALSE;
    quantizer->state->qj[i] = 0;
    quantizer->state->flag2[i] = 0;
    for (j = 0; j < states; j++) {
      quantizer->state->A[i][j] = 0;
      quantizer->state->U0[i][j] = 0;
    }
  }
  quantizer->state->tx = simTime->tx;
  quantizer->state->simTime = &simTime->time;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
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
  bool *sim_step = quantizer->state->sim_step;
  int *flag2 = quantizer->state->flag2;

  if (t > 0 && state_step) {
    diff_Q = q[2 * ifr_state] - quantizer->state->qAux[ifr_state];
    if (diff_Q) {
      A[i][ifr_state] = (x[i1] - quantizer->state->oldDx[i]) / diff_Q;
    } else {
      A[i][ifr_state] = 0;
    }
  }

  U[i][i] = x[i1] - q[i0] * A[i][i];

  if (sim_step[i] == TRUE) {
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
    if (nTime[i] < t) {
      nTime[i] = t + MIN_STEP;
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

void QSS_FUNC_DECL(mLIQSS, BEStep)(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
{
  double **A = quantizer->state->A;
  double **U = quantizer->state->U0;
  int i0 = 2 * i;
  int j0 = 2 * j;
  // Compute G = I - h * A
  double G[2][2] = {{1 - h * A[i][i], -h * A[i][j]}, {-h * A[j][i], 1 - h * A[j][j]}};
  double det_G = G[0][0] * G[1][1] - G[0][1] * G[1][0];
  if (det_G == 0) {
    det_G = 1e-10;
  }
  double adj_G[2][2] = {{G[1][1], -G[0][1]}, {-G[1][0], G[0][0]}};
  double inv_G[2][2] = {{adj_G[0][0] / det_G, adj_G[0][1] / det_G}, {adj_G[1][0] / det_G, adj_G[1][1] / det_G}};
  q[i0] = inv_G[0][0] * (x[i0] + h * U[i][j]) + inv_G[0][1] * (xj0 + h * U[j][i]);
  qj[j0] = inv_G[1][0] * (x[i0] + h * U[i][j]) + inv_G[1][1] * (xj0 + h * U[j][i]);
}

void QSS_FUNC_DECL(mLIQSS, prevDx)(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
{
  int inf_var, j, j1;
  if (t > 0) {
    for (inf_var = 0; inf_var < nSD; inf_var++) {
      j = quantizer->state->SD[i][inf_var];
      j1 = j * 2 + 1;
      quantizer->state->oldDx[j] = x[j1];
    }
  }
}

int QSS_FUNC_DECL(mLIQSS, findMaxInf)(QA_quantizer quantizer, double *q, double *x, int i, double qi_prev)
{
  double **U = quantizer->state->U0;
  double **A = quantizer->state->A;
  int nSD = quantizer->state->nSD[i];
  int m, j, i0 = 2 * i, j0, j1, j_max = -1;
  double max = 0;

  for (m = 0; m < nSD; m++) {
    j = quantizer->state->SD[i][m];
    j0 = 2 * j;
    j1 = j0 + 1;
    if (j != i && A[i][j] * A[j][i] != 0) {
      U[j][i] = U[j][j] - A[j][i] * qi_prev;
      double dx_j = A[j][i] * q[i0] + A[j][j] * q[j0] + U[j][i];
      double dx_j_change = fabs(x[j1] - dx_j);
      double dx_j_mean = fabs(x[j1] + dx_j) / 2;
      /// Change in x_i triggers a significant change in dx_j. This is different from
      /// the algorithm presented in https://doi.org/10.1177/0037549718766689 for mLIQSS1.
      /// We take the same approach here as the one defined for mLIQSS2 method, also
      /// the same is done when looking for furute changes in dx_i.
      if (dx_j_change > dx_j_mean) {
        if (dx_j_change > max) {
          max = dx_j_change;
          j_max = j;
        }
      }
    }
  }
  return j_max;
}

void QSS_FUNC_DECL(mLIQSS, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  bool *sim_step = quantizer->state->sim_step;
  double *qj = quantizer->state->qj;
  double t = quantizer->state->lSimTime->time;
  double **U = quantizer->state->U0;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  double *q_prev = quantizer->state->qAux;
  double *dx_prev = quantizer->state->oldDx;
  int i0 = i * 2, i1 = i0 + 1, j;

  // Number of influenced state derivatives for x[i]
  int nSD = quantizer->state->nSD[i];
  double dx;
  double dq;
  // Set h_max as INF.
  double h = INF;

  // Store old derivative and quantized variables.
  q_prev[i] = q[i0];
  dx_prev[i] = x[i1];
  // If the sim_step for q[j] was already computed by the previous mLIQSS step,
  // just update the value and return.
  if (sim_step[i] == TRUE) {
    q[i0] = qj[i0];
    sim_step[i] = FALSE;
  } else {
    // LIQSS1
    q[i0] = x[i0];
    // Compute the future value of dx and check for sign changes.
    int der_sign = sign(x[i1]);
    dx = (q[i0] + der_sign * lqu[i]) * A[i][i] + U[i][i];
    if (der_sign * dx >= 0) {
      dq = der_sign * lqu[i];
    } else {
      dq = -U[i][i] / A[i][i] - q[i0];
    }
    if (dq > 2 * der_sign * lqu[i]) {
      dq = 2 * der_sign * lqu[i];
    }
    q[i0] += dq;

    // mLIQSS control algorithm
    j = QSS_FUNC_INVK(mLIQSS, findMaxInf)(quantizer, q, x, i, q_prev[i]);
    if (j != -1) {
      int j0 = j * 2, j1 = j0 + 1;
      double dx_j = A[j][i] * q[i0] + A[j][j] * q[j0] + U[j][i];
      double elapsed = t - tx[j];
      double x_j0 = x[j0] + elapsed * x[j1];
      U[i][j] = U[i][i] - A[i][j] * q[j0];
      int dx_j_sign = sign(dx_j);
      double qj_next = x_j0 + dx_j_sign * 2 * lqu[j];
      double dx_i = A[i][i] * q[i0] + A[i][j] * qj_next + U[i][j];
      double d1 = A[i][i] * A[j][j];
      double d2 = A[i][j] * A[j][i];
      bool dA = FALSE;
      double tol = 1e-1;
      if (d2 != 0 && d1 / d2 < 1 + tol && d1 / d2 > 1 - tol) {
        dA = TRUE;
      }
      // Change in xj triggers a significant change in xi.
      if (fabs(x[i1] - dx_i) > fabs(x[i1] + dx_i) / 2 || dA) {
        double qi_prev = q[i0];
        double qj_prev = q[j0];
        qj[j0] = INF;
        if (!dA) {
          if (A[j][j] != 0) {
            q[i0] = (A[i][j] * U[j][i] - A[j][j] * U[i][j]) / (A[i][i] * A[j][j] - A[i][j] * A[j][i]);
            qj[j0] = (-A[j][i] * q[i0] - U[j][i]) / A[j][j];
            if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - x_j0) < TOL * lqu[j]) {
              sim_step[j] = TRUE;
            }
          }
        }
        if (!sim_step[j]) {
          h = (quantizer->state->finTime - t);
          QSS_FUNC_INVK(mLIQSS, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
          if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - x_j0) < TOL * lqu[j]) {
            sim_step[j] = TRUE;
          } else {
            if (fabs(q[i0] - x[i0]) / lqu[i] > fabs(qj[j0] - x_j0) / lqu[j]) {
              h = h / (fabs(q[i0] - x[i0]) / lqu[i]) * 0.9;
            } else {
              h = h / (fabs(qj[j0] - x_j0) / lqu[j]) * 0.9;
            }
            QSS_FUNC_INVK(mLIQSS, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
            if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - x_j0) < TOL * lqu[j]) {
              sim_step[j] = TRUE;
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
              QSS_FUNC_INVK(mLIQSS, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
              if (fabs(q[i0] - x[i0]) < TOL * lqu[i] && fabs(qj[j0] - x_j0) < TOL * lqu[j]) {
                sim_step[j] = TRUE;
              }
            }
          }
          if (!sim_step[j]) {
            q[i0] = qi_prev;
            q[j0] = qj_prev;
            qj[j0] = INF;
          }
        }
      }
    }
  }
  QSS_FUNC_INVK(mLIQSS, prevDx)(quantizer, i, t, nSD, x, tx);
}
