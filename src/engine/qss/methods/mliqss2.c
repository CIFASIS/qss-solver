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
#include <stdio.h>
#include <stdlib.h>

#include <qss/methods/mliqss2.h>
#include <common/utils.h>

void QSS_FUNC_DECL(mLIQSS2, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int states = simData->states;
  int i, j;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->ltq = (double *)malloc(states * sizeof(double));
  quantizer->state->qinf = (double *)malloc(states * sizeof(double));
  quantizer->state->qsup = (double *)malloc(states * sizeof(double));
  quantizer->state->A = (double **)malloc(states * sizeof(double *));
  quantizer->state->U0 = (double **)malloc(states * sizeof(double *));
  quantizer->state->U1 = (double **)malloc(states * sizeof(double *));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(double));
  quantizer->state->flag3 = (int *)malloc(states * sizeof(double));
  for (i = 0; i < states; i++) {
    quantizer->state->A[i] = (double *)malloc(states * sizeof(double));
    quantizer->state->U0[i] = (double *)malloc(states * sizeof(double));
    quantizer->state->U1[i] = (double *)malloc(states * sizeof(double));
  }
  quantizer->state->sim_step = (bool *)malloc(states * sizeof(bool));
  quantizer->state->qj = (double *)malloc(3 * states * sizeof(double));
  quantizer->state->finTime = simData->ft;
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    simData->x[cf0 + 1] = 0;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->ltq[i] = simData->it;
    quantizer->state->sim_step[i] = FALSE;
    quantizer->state->qj[cf0] = 0;
    quantizer->state->qj[cf0 + 1] = 0;
    quantizer->state->qj[cf0 + 2] = 0;
    for (j = 0; j < states; j++) {
      quantizer->state->A[i][j] = 0;
      quantizer->state->U0[i][j] = 0;
      quantizer->state->U1[i][j] = 0;
    }
    quantizer->state->flag2[i] = 0;
    quantizer->state->flag3[i] = 0;
    quantizer->state->qinf[i] = 0;
    quantizer->state->qsup[i] = 0;
  }
  QSS_ASSIGN_QUANTIZER_OPS(mLIQSS2)
  quantizer->state->tx = simTime->tx;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
}

void QSS_FUNC_DECL(mLIQSS2, recomputeNextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  double coeff[3], diff_Q, timeaux;
  double **A = quantizer->state->A;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double *tx = quantizer->state->lSimTime->tx;
  double *tq = quantizer->state->lSimTime->tq;
  int ifr_state = quantizer->state->lSimTime->minIndex;
  bool state_step = quantizer->state->lSimTime->type == ST_State;
  bool *sim_step = quantizer->state->sim_step;
  bool self = quantizer->state->lSimTime->minIndex == i && state_step;

  if (state_step && ifr_state >= 0) {
    double elapsed = t - tq[ifr_state];
    double q_prev_proy = quantizer->state->qAux[ifr_state] + elapsed * quantizer->state->qinf[ifr_state];
    diff_Q = q[3 * ifr_state] - q_prev_proy;

    if (diff_Q) {
      double elapsed = t - quantizer->state->ltq[i];
      if (self) {
        elapsed = 0;
      }
      double x_prev_proy = quantizer->state->oldDx[i] + elapsed * quantizer->state->qsup[i] * 2;
      A[i][ifr_state] = (x[i1] - x_prev_proy) / diff_Q;
    } else {
      A[i][ifr_state] = 0;
    }
  }
  /*if (quantizer->state->lSimTime->type == ST_Event) {
    quantizer->state->oldDx[i] = x[i1];
    A[i][i] = 0;
  }*/
  U0[i][i] = x[i1] - q[i0] * A[i][i];
  U1[i][i] = 2 * x[i2] - q[i1] * A[i][i];
  if (sim_step[i] == TRUE) {
    nTime[i] = t;
  } else {
    if (quantizer->state->flag2[i] != 1) {
      if ((quantizer->state->lSimTime->type == ST_Event) && (A[i][i] < 0) &&
          (quantizer->state->nSZ[i] > 0)) {  // we check if var is involved in the zero crossing function that produced the current event
        int inf;
        for (inf = 0; inf < quantizer->state->nSZ[i]; inf++) {
          if (quantizer->state->SZ[i][inf] == quantizer->state->lSimTime->minIndex) {
            nTime[i] = t;
            quantizer->state->flag2[i] = 1;  // it does, so we restart the quantized state q[var]
            return;
          }
        }
      }
      coeff[1] = q[i1] - x[i1];
      coeff[2] = -x[i2];

      if (A[i][i] < 0 && self && x[i2] != 0 && quantizer->state->flag3[i] != 1) {
        nTime[i] = t + fabs((q[i1] - x[i1]) / x[i2]);
        double err1 = q[i0] - x[i0] + coeff[1] * (nTime[i] - t) / 2 + coeff[2] * pow((nTime[i] - t) / 2, 2);
        if (fabs(err1) > 3 * fabs(lqu[i])) {
          nTime[i] = INF;
        }
      } else {
        nTime[i] = INF;
      }
      quantizer->state->flag3[i] = 0;
      if (nTime[i] == INF) {
        coeff[0] = q[i0] + 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[i]) {
          nTime[i] = timeaux;
        }
        coeff[0] = q[i0] - 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[i]) {
          nTime[i] = timeaux;
        }
        if (q[i0] * q[i1] < 0 && fabs(q[i0]) > 10 * lqu[i]) {
          timeaux = -q[i0] / q[i1] - 2 * fabs(lqu[i] / q[i1]);
          if (nTime[i] > t + timeaux) {
            nTime[i] = t + timeaux;
          }
        }
        if (quantizer->state->flag2[i] == 2 && self) {
          quantizer->state->flag2[i] = 0;
        }
      }
      double err1 = q[i0] - x[i0] + coeff[1] * (nTime[i] - t) / 2 + coeff[2] * pow((nTime[i] - t) / 2, 2);
      if (fabs(err1) > 3 * fabs(lqu[i])) {
        nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
      }
    } else if (self) {
      quantizer->state->flag2[i] = 2;
      nTime[i] = t;
    }
  }
}

void QSS_FUNC_DECL(mLIQSS2, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                                double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
    QSS_FUNC_INVK(mLIQSS2, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
  QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(mLIQSS2, nextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
{
  int i2 = i * 3 + 2;
  if (x[i2] == 0) {
    nTime[i] = INF;
  } else {
    nTime[i] = t + sqrt(fabs(lqu[i] / x[i2]));
  }
}

void QSS_FUNC_DECL(mLIQSS2, BEStepSingle)(QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *step_size)
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  double t = quantizer->state->lSimTime->time;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double h = INF, ddx;

  if (A[i][i] < 0) {
    ddx = x[i2];
    if (ddx == 0) {
      ddx = A[i][i] * A[i][i] * q[i0] + A[i][i] * U0[i][i] + U1[i][i];
      if (ddx == 0) {
        ddx = 1e-40;
      }
    }
    h = (quantizer->state->finTime - t);
    q[i0] = ((x[i0] + h * U0[i][i] + h * h / 2 * U1[i][i]) * (1 - h * A[i][i]) + (h * h / 2 * A[i][i] - h) * (U0[i][i] + h * U1[i][i])) /
            (1 - h * A[i][i] + h * h * A[i][i] * A[i][i] / 2);

    if (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
      h = sqrt(fabs(lqu[i] / ddx));
      q[i0] = ((x[i0] + h * U0[i][i] + h * h / 2 * U1[i][i]) * (1 - h * A[i][i]) + (h * h / 2 * A[i][i] - h) * (U0[i][i] + h * U1[i][i])) /
              (1 - h * A[i][i] + h * h * A[i][i] * A[i][i] / 2);
    }
    while (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
      h = h * sqrt(lqu[i] / fabs(q[i0] - x[i0]));
      q[i0] = ((x[i0] + h * U0[i][i] + h * h / 2 * U1[i][i]) * (1 - h * A[i][i]) + (h * h / 2 * A[i][i] - h) * (U0[i][i] + h * U1[i][i])) /
              (1 - h * A[i][i] + h * h * A[i][i] * A[i][i] / 2);
    }
    q[i1] = (A[i][i] * q[i0] + U0[i][i] + h * U1[i][i]) / (1 - h * A[i][i]);
  } else {
    if (x[i2] != 0) {
      q[i0] = x[i0] - sign(x[i2]) * lqu[i];
      h = sqrt(2 * (x[i0] - q[i0]) / x[i2]);
      h *= 2;
      q[i1] = x[i1] + x[i2] * h;
    } else {
      q[i0] = x[i0];
      q[i1] = x[i1];
    }
  }
  *step_size = h;
}

void QSS_FUNC_DECL(mLIQSS2, AxB)(double A[2][2], double B[2][2], double AB[2][2])
{
  AB[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
  AB[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
  AB[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
  AB[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
}

void QSS_FUNC_DECL(mLIQSS2, invA)(double A[2][2], double inv_A[2][2])
{
  double det_A = A[0][0] * A[1][1] - A[0][1] * A[1][0];
  if (det_A == 0) {
    det_A = 1e-10;
  }
  inv_A[0][0] = A[1][1] / det_A;
  inv_A[0][1] = -A[0][1] / det_A;
  inv_A[1][0] = -A[1][0] / det_A;
  inv_A[1][1] = A[0][0] / det_A;
}

void QSS_FUNC_DECL(mLIQSS2, BEStep)(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
{
  int i0 = 3 * i, i1 = i0 + 1;
  int j0 = 3 * j, j1 = j0 + 1;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double Ad[2][2] = {{1 - h * A[i][i], -h * A[i][j]}, {-h * A[j][i], 1 - h * A[j][j]}};
  double An[2][2] = {{A[i][i], A[i][j]}, {A[j][i], A[j][j]}};
  double B[2][2], BA[2][2], AB[2][2], ABA[2][2], inv_C[2][2];
  QSS_FUNC_INVK(mLIQSS2, invA)(Ad, B);
  QSS_FUNC_INVK(mLIQSS2, AxB)(B, An, BA);
  QSS_FUNC_INVK(mLIQSS2, AxB)(An, B, AB);
  QSS_FUNC_INVK(mLIQSS2, AxB)(An, BA, ABA);
  // C = I + h(BA - A - h/2*ABA)
  double C[2][2] = {{1 + h * (BA[0][0] - A[i][i] - h / 2 * ABA[0][0]), h * (BA[0][1] - A[i][j] - h / 2 * ABA[0][1])},
                    {h * (BA[1][0] - A[j][i] - h / 2 * ABA[1][0]), 1 + h * (BA[1][1] - A[j][j] - h / 2 * ABA[1][1])}};
  // D = I - B + h/2*AB
  double D[2][2] = {{1 - B[0][0] + h / 2 * AB[0][0], -B[0][1] + h / 2 * AB[0][1]},
                    {-B[1][0] + h / 2 * AB[1][0], 1 - B[1][1] + h / 2 * AB[1][1]}};
  // E = (1/2*(h*AB + I) - B)
  double E[2][2] = {{1 / 2 * (h * AB[0][0] + 1) - B[0][0], h / 2 * AB[0][1] - B[0][1]},
                    {h / 2 * AB[1][0] - B[1][0], 1 / 2 * (h * AB[1][1] + 1) - B[1][1]}};
  double Du0[2] = {D[0][0] * U0[i][j] + D[0][1] * U0[j][i], D[1][0] * U0[i][j] + D[1][1] * U0[j][i]};
  double Eu1[2] = {E[0][0] * U1[i][j] + E[0][1] * U1[j][i], E[1][0] * U1[i][j] + E[1][1] * U1[j][i]};
  QSS_FUNC_INVK(mLIQSS2, invA)(C, inv_C);
  q[i0] = inv_C[0][0] * (x[i0] + h * Du0[0] + h * h * Eu1[0]) + inv_C[0][1] * (xj0 + h * Du0[1] + h * h * Eu1[1]);
  qj[j0] = inv_C[1][0] * (x[i0] + h * Du0[0] + h * h * Eu1[0]) + inv_C[1][1] * (xj0 + h * Du0[1] + h * h * Eu1[1]);
  q[i1] = B[0][0] * (A[i][i] * q[i0] + A[i][j] * qj[j0] + U0[i][j] + h * U1[i][j]) +
          B[0][1] * (A[j][i] * q[i0] + A[j][j] * qj[j0] + U0[j][i] + h * U1[j][i]);
  qj[j1] = B[1][0] * (A[i][i] * q[i0] + A[i][j] * qj[j0] + U0[i][j] + h * U1[i][j]) +
           B[1][1] * (A[j][i] * q[i0] + A[j][j] * qj[j0] + U0[j][i] + h * U1[j][i]);
}

void QSS_FUNC_DECL(mLIQSS2, dxPrev)(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
{
  int m, j, j0, j1, j2;
  double *dx_prev = quantizer->state->oldDx;
  int **SD = quantizer->state->SD;
  for (m = 0; m < nSD; m++) {
    j = SD[i][m];
    j0 = j * 3;
    j1 = j0 + 1;
    j2 = j1 + 1;
    dx_prev[j] = x[j1];  // + (t - tx[j]) * x[j2] * 2;
    quantizer->state->qsup[j] = x[j2];
    quantizer->state->ltq[j] = quantizer->state->lSimTime->tx[j];
  }
}

int QSS_FUNC_DECL(mLIQSS2, findMaxInf)(QA_quantizer quantizer, double *q, double *x, int i)
{
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double *tq = quantizer->state->lSimTime->tq;
  double *tx = quantizer->state->lSimTime->tx;
  double t = quantizer->state->lSimTime->time;
  double *qi_proy = quantizer->state->qAux;

  int nSD = quantizer->state->nSD[i];
  int inf_var, i0 = 3 * i, i1 = i0 + 1, j, j0, j1, j2, j_max = -1;
  double elapsed = 0;
  double max = 0;

  for (inf_var = 0; inf_var < nSD; inf_var++) {
    j = quantizer->state->SD[i][inf_var];
    if (j != i && A[i][j] * A[j][i] != 0) {
      j0 = 3 * j;
      j1 = j0 + 1;
      j2 = j1 + 1;
      elapsed = t - tx[j];
      U0[j][j] = U0[j][j] + elapsed * U1[j][j];
      elapsed = t - tq[j];
      double qj_proy = q[j0] + elapsed * q[j1];
      U0[j][i] = U0[j][j] - A[j][i] * qi_proy[i];
      double dx_j = A[j][i] * q[i0] + A[j][j] * qj_proy + U0[j][i];
      U1[j][i] = U1[j][j] - A[j][i] * quantizer->state->qinf[i];
      double ddx_j = A[j][i] * q[i1] + A[j][j] * q[j1] + U1[j][i];
      double dx_j_change = fabs(x[j1] - dx_j);
      double dx_j_mean = fabs(x[j1] + dx_j) / 2;
      double ddx_j_change = fabs(2 * x[j2] - ddx_j);
      double ddx_j_mean = fabs(2 * x[j2] + ddx_j) / 2;
      if ((dx_j_change > dx_j_mean) || (ddx_j_change > ddx_j_mean)) {
        double max_change = (dx_j_change > ddx_j_change) ? dx_j_change : ddx_j_change;
        if (max_change > max) {
          max = max_change;
          j_max = j;
        }
      }
    }
  }
  return j_max;
}

void QSS_FUNC_DECL(mLIQSS2, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  bool *sim_step = quantizer->state->sim_step;
  double *qj = quantizer->state->qj;
  double t = quantizer->state->lSimTime->time;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  double *tq = quantizer->state->lSimTime->tq;
  double *qi_proy = quantizer->state->qAux;
  double *dx_prev = quantizer->state->oldDx;
  int *flag2 = quantizer->state->flag2;
  int nSD = quantizer->state->nSD[i];
  double elapsed;
  double h;

  elapsed = t - tq[i];
  qi_proy[i] = q[i0] + elapsed * q[i1];
  quantizer->state->qinf[i] = q[i1];
  dx_prev[i] = x[i1];
  quantizer->state->qsup[i] = x[i2];
  elapsed = t - tq[i];
  U0[i][i] = U0[i][i] + elapsed * U1[i][i];

  if (sim_step[i] == TRUE) {
    q[i0] = qj[i0];
    q[i1] = qj[i1];
    sim_step[i] = FALSE;
  } else if (flag2[i] >= 1) {
    if (q[i1] > 0) {
      q[i0] = qi_proy[i] + lqu[i] / 100;
    } else {
      q[i0] = qi_proy[i] - lqu[i] / 100;
    }
    q[i1] = 0;
  } else {
    int j, j0, j1, j2;
    QSS_FUNC_INVK(mLIQSS2, BEStepSingle)(quantizer, i, x, q, lqu, &h);
    j = QSS_FUNC_INVK(mLIQSS2, findMaxInf)(quantizer, q, x, i);
    if (j != -1) {
      double qj_proy[2], dx_i, ddx_i, qi_prev[2], qj_prev[2];
      j0 = j * 3;
      j1 = j0 + 1;
      j2 = j1 + 1;

      double ddx_j = A[j][i] * q[i1] + A[j][j] * q[j1] + U1[j][i];
      elapsed = t - tx[j];
      double x_j0 = x[j0] + elapsed * x[j1] + elapsed * elapsed * x[j2];
      qj_proy[0] = x_j0 - sign(ddx_j) * 2 * lqu[j];
      qj_proy[1] = (A[j][i] * (q[i0] + h * q[i1]) + A[j][j] * q[j0] + U0[j][i] + h * U1[j][i]) / (1 - h * A[j][j]);

      elapsed = t - tq[j];
      double qj_prev_proy = q[j0] + elapsed * q[j1];
      U0[i][j] = U0[i][i] - A[i][j] * qj_prev_proy;
      dx_i = A[i][i] * q[i0] + A[i][j] * qj_proy[0] + U0[i][j];
      U1[i][j] = U1[i][i] - A[i][j] * q[j1];
      ddx_i = A[i][i] * q[i1] + A[i][j] * qj_proy[1] + U1[i][j];
      double dx_i_change = fabs(x[i1] - dx_i);
      double dx_i_mean = fabs(x[i1] + dx_i) / 2;
      double ddx_i_change = fabs(2 * x[i2] - ddx_i);
      double ddx_i_mean = fabs(2 * x[i2] + ddx_i) / 2;
      if ((dx_i_change > dx_i_mean) || (ddx_i_change > ddx_i_mean)) {
        qi_prev[0] = q[i0];
        qi_prev[1] = q[i1];
        qj_prev[0] = q[j0];
        qj_prev[1] = q[j1];
        qj[j0] = INF;
        qj[j1] = INF;
        h = quantizer->state->finTime - t;
        QSS_FUNC_INVK(mLIQSS2, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
        if (fabs(q[i0] - x[i0]) < 3 * lqu[i] && fabs(qj[j0] - x_j0) < 3 * lqu[j]) {
          sim_step[j] = TRUE;
        } else {
          if (fabs(q[i0] - x[i0]) / lqu[i] > fabs(qj[j0] - x_j0) / lqu[j]) {
            h = h / sqrt(2 * fabs(q[i0] - x[i0]) / lqu[i]);
          } else {
            h = h / sqrt(2 * fabs(qj[j0] - x_j0) / lqu[j]);
          }
          h /= 2;
          QSS_FUNC_INVK(mLIQSS2, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
          if (fabs(q[i0] - x[i0]) < 3 * lqu[i] && fabs(qj[j0] - x_j0) < 3 * lqu[j]) {
            sim_step[j] = TRUE;
          } else {
            if (ddx_i != 0 && ddx_j != 0) {
              if (fabs(lqu[i] / ddx_i) < fabs(lqu[j] / ddx_j)) {
                h = sqrt(2 * fabs(lqu[i] / ddx_i));
              } else {
                h = sqrt(2 * fabs(lqu[j] / ddx_j));
              }
            } else if (ddx_i != 0) {
              h = sqrt(2 * fabs(lqu[i] / ddx_i));
            } else if (ddx_j != 0) {
              h = sqrt(2 * fabs(lqu[j] / ddx_j));
            }
            h /= 2;
            QSS_FUNC_INVK(mLIQSS2, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
            if (fabs(q[i0] - x[i0]) < 3 * lqu[i] && fabs(qj[j0] - x_j0) < 3 * lqu[j]) {
              sim_step[j] = TRUE;

            } else {
              h /= 10.0;
              QSS_FUNC_INVK(mLIQSS2, BEStep)(quantizer, x, q, qj, i, j, h, x_j0);
              if (fabs(q[i0] - x[i0]) < 3 * lqu[i] && fabs(qj[j0] - x_j0) < 3 * lqu[j]) {
                sim_step[j] = TRUE;
              } else {
                q[i0] = qi_prev[0];
                q[i1] = qi_prev[1];
                q[j0] = qj_prev[0];
                q[j1] = qj_prev[1];
                qj[j0] = INF;
                qj[j1] = INF;
              }
            }
          }
        }
      }
    }
  }
  QSS_FUNC_INVK(mLIQSS2, dxPrev)(quantizer, i, t, nSD, x, tx);
}
