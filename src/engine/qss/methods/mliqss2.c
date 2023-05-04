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

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void mLIQSS2_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int states = simData->states;
  int i, j;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->ltq = (double *)malloc(states * sizeof(double));
  quantizer->state->A = (double **)malloc(states * sizeof(double *));
  quantizer->state->U0 = (double **)malloc(states * sizeof(double *));
  quantizer->state->U1 = (double **)malloc(states * sizeof(double *));
  for (i = 0; i < states; i++) {
    quantizer->state->A[i] = (double *)malloc(states * sizeof(double));
    quantizer->state->U0[i] = (double *)malloc(states * sizeof(double));
    quantizer->state->U1[i] = (double *)malloc(states * sizeof(double));
  }
  quantizer->state->change = (bool *)malloc(states * sizeof(bool));
  quantizer->state->next = (double *)malloc(3 * states * sizeof(double));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));

  quantizer->state->finTime = simData->ft;
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->ltq[i] = simData->it;
    quantizer->state->change[i] = FALSE;
    quantizer->state->tx[i] = 0;
    quantizer->state->next[cf0] = 0;
    quantizer->state->next[cf0 + 1] = 0;
    quantizer->state->next[cf0 + 2] = 0;
    for (j = 0; j < states; j++) {
      quantizer->state->A[i][j] = 0;
      quantizer->state->U0[i][j] = 0;
      quantizer->state->U1[i][j] = 0;
    }
  }
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = mLIQSS2_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = mLIQSS2_PAR_recomputeNextTime;
  quantizer->ops->nextTime = mLIQSS2_PAR_nextTime;
  quantizer->ops->updateQuantizedState = mLIQSS2_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = mLIQSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = mLIQSS2_recomputeNextTime;
  quantizer->ops->nextTime = mLIQSS2_nextTime;
  quantizer->ops->updateQuantizedState = mLIQSS2_updateQuantizedState;
#endif
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#else
void mLIQSS2_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#endif
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  double coeff[3], diffQ, timeaux;
  double **A = quantizer->state->A;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  int stind = quantizer->state->lSimTime->minIndex;
  bool *change = quantizer->state->change;
  bool self = quantizer->state->lSimTime->minIndex == i && quantizer->state->lSimTime->type == ST_State;
  double *tx = quantizer->state->tx;

  if (t > 0) {
    diffQ = q[3 * stind] - quantizer->state->qAux[stind];
    if (diffQ)
      A[i][stind] = (x[i1] - quantizer->state->oldDx[i]) / diffQ;
    else
      A[i][stind] = 0;
  }
  U0[i][i] = x[i1] - q[i0] * A[i][i];
  U1[i][i] = 2 * x[i2] - q[i1] * A[i][i];
  tx[i] = t;

  if (change[i] == TRUE) {
    nTime[i] = t;
  } else {
    coeff[1] = q[i1] - x[i1];
    coeff[2] = -x[i2];
    if (A[i][i] < 0 && self && x[i2] != 0) {
      nTime[i] = t + fabs((q[i1] - x[i1]) / x[i2]);
    } else {
      nTime[i] = INF;
    }
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
      if (nTime[i] > t + timeaux) nTime[i] = t + timeaux;
    }
    double err1 = q[i0] - x[i0] + coeff[1] * (nTime[i] - t) / 2 + coeff[2] * pow((nTime[i] - t) / 2, 2);
    if (fabs(err1) > 3 * fabs(lqu[i])) {
      nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
    }
  }
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu, double *q)
#else
void mLIQSS2_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu, double *q)
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
      mLIQSS2_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
    }
#endif
  }
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_nextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
#else
void mLIQSS2_nextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
#endif
{
  int i2 = i * 3 + 2;
  if (x[i2] == 0)
    nTime[i] = INF;
  else
    nTime[i] = t + sqrt(fabs(lqu[i] / x[i2]));
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_solve_single(QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2)
#else
void mLIQSS2_solve_single(QA_quantizer quantizer, int i, double *x, double *q, double *lqu, double *h2)
#endif
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  double t = quantizer->state->lSimTime->time;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double h, ddx;

  if (A[i][i] < 0) {
    ddx = x[i2];
    if (ddx == 0) {
      ddx = A[i][i] * A[i][i] * q[i0] + A[i][i] * U0[i][i] + U1[i][i];
      if (ddx == 0) ddx = 1e-40;
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
      if (x[i2] > 0)
        q[i0] = x[i0] - lqu[i];
      else
        q[i0] = x[i0] + lqu[i];
      h = sqrt(2 * (x[i0] - q[i0]) / x[i2]);
      h *= 2;
      q[i1] = x[i1] + x[i2] * h;
    } else {
      q[i0] = x[i0];
      q[i1] = x[i1];
    }
  }
  *h2 = h;
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_AxB(double A[2][2], double B[2][2], double AB[2][2])
#else
void mLIQSS2_AxB(double A[2][2], double B[2][2], double AB[2][2])
#endif
{
  AB[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
  AB[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
  AB[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
  AB[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_invA(double A[2][2], double invA[2][2])
#else
void mLIQSS2_invA(double A[2][2], double invA[2][2])
#endif
{
  double detA = A[0][0] * A[1][1] - A[0][1] * A[1][0];
  if (detA == 0) detA = 1e-10;
  invA[0][0] = A[1][1] / detA;
  invA[0][1] = -A[0][1] / detA;
  invA[1][0] = -A[1][0] / detA;
  invA[1][1] = A[0][0] / detA;
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *next, int i, int j, double h, double xj0)
#else
void mLIQSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *next, int i, int j, double h, double xj0)
#endif
{
  int i0 = 3 * i, i1 = i0 + 1;
  int j0 = 3 * j, j1 = j0 + 1;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double Ad[2][2] = {{1 - h * A[i][i], -h * A[i][j]}, {-h * A[j][i], 1 - h * A[j][j]}};
  double An[2][2] = {{A[i][i], A[i][j]}, {A[j][i], A[j][j]}};
  double B[2][2], BA[2][2], AB[2][2], ABA[2][2], invC[2][2];
  mLIQSS2_invA(Ad, B);
  mLIQSS2_AxB(B, An, BA);
  mLIQSS2_AxB(An, B, AB);
  mLIQSS2_AxB(An, BA, ABA);
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
  mLIQSS2_invA(C, invC);
  q[i0] = invC[0][0] * (x[i0] + h * Du0[0] + h * h * Eu1[0]) + invC[0][1] * (xj0 + h * Du0[1] + h * h * Eu1[1]);
  next[j0] = invC[1][0] * (x[i0] + h * Du0[0] + h * h * Eu1[0]) + invC[1][1] * (xj0 + h * Du0[1] + h * h * Eu1[1]);
  q[i1] = B[0][0] * (A[i][i] * q[i0] + A[i][j] * next[j0] + U0[i][j] + h * U1[i][j]) +
          B[0][1] * (A[j][i] * q[i0] + A[j][j] * next[j0] + U0[j][i] + h * U1[j][i]);
  next[j1] = B[1][0] * (A[i][i] * q[i0] + A[i][j] * next[j0] + U0[i][j] + h * U1[i][j]) +
             B[1][1] * (A[j][i] * q[i0] + A[j][j] * next[j0] + U0[j][i] + h * U1[j][i]);
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
#else
void mLIQSS2_old_dx(QA_quantizer quantizer, int i, double t, int nSD, double *x, double *tx)
#endif
{
  int m, j, j0, j1, j2;
  if (t > 0) {
    for (m = 0; m < nSD; m++) {
      j = quantizer->state->SD[i][m];
      j0 = j * 3;
      j1 = j0 + 1;
      j2 = j1 + 1;
      quantizer->state->oldDx[j] = x[j1] + (t - tx[j]) * x[j2] * 2;
    }
  }
}

#ifdef QSS_PARALLEL
int mLIQSS2_PAR_find_max_inf(QA_quantizer quantizer, double *q, double *x, int i, double qi1_old)
#else
int mLIQSS2_find_max_inf(QA_quantizer quantizer, double *q, double *x, int i, double qi1_old)
#endif
{
  double t = quantizer->state->lSimTime->time;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  int nSD = quantizer->state->nSD[i];
  int m, j, i1 = 3 * i + 1, j1, j2, jmax = -1;
  double elapsed, u0, u1, ddxj, max = 0;

  if (t > 0) {
    for (m = 0; m < nSD; m++) {
      j = quantizer->state->SD[i][m];
      j1 = 3 * j + 1;
      j2 = j1 + 1;
      if (j != i && A[i][j] * A[j][i] != 0) {
        elapsed = t - tx[j];
        u0 = U0[j][j] + elapsed * U1[j][j];
        u1 = U1[j][j] - A[j][i] * qi1_old;
        ddxj = A[j][i] * q[i1] + A[j][j] * q[j1] + U1[j][i];
        if (fabs(2 * x[j2] - ddxj) > fabs(2 * x[j2] + ddxj) / 10) {
          if (fabs(2 * x[j2] - ddxj) / (fabs(2 * x[j2] + ddxj)) > max) {
            max = fabs(2 * x[j2] - ddxj) / (fabs(2 * x[j2] + ddxj));
            jmax = j;
          }
        }
      }
    }
  }
  return jmax;
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#else
void mLIQSS2_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#endif
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  bool *change = quantizer->state->change;
  double *next = quantizer->state->next;
  double t = quantizer->state->lSimTime->time;
  double **U0 = quantizer->state->U0;
  double **U1 = quantizer->state->U1;
  double **A = quantizer->state->A;
  double *tx = quantizer->state->tx;
  int nSD = quantizer->state->nSD[i];
  double elapsed;

  double c = quantizer->state->cont;
  double h;

  elapsed = t - quantizer->state->lSimTime->tq[i];
  quantizer->state->qAux[i] = q[i0] + elapsed * q[i1];
  quantizer->state->oldDx[i] = x[i1];
  elapsed = t - tx[i];
  U0[i][i] = U0[i][i] + elapsed * U1[i][i];
  tx[i] = t;

  if (change[i] == TRUE) {
    q[i0] = next[i0];
    q[i1] = next[i1];
    change[i] = FALSE;
  } else {
    double qi_proy, qj_proy, ddxi, qiold[2], qjold[2];
    int j, j0, j1, j2;

    qiold[0] = q[i0];
    qiold[1] = q[i1];
    mLIQSS2_solve_single(quantizer, i, x, q, lqu, &h);
#ifdef QSS_PARALLEL
    j = mLIQSS2_PAR_find_max_inf(quantizer, q, x, i, qiold[1]);
#else
    j = mLIQSS2_find_max_inf(quantizer, q, x, i, qiold[1]);
#endif
    if (t > 0 && j != -1) {
      j0 = j * 3;
      j1 = j0 + 1;
      j2 = j1 + 1;
      if (j != i && A[i][j] * A[j][i] != 0) {
        double ddxj;
        elapsed = t - quantizer->state->lSimTime->tq[j];
        qj_proy = q[j0] + elapsed * q[j1];
        elapsed = t - tx[j];
        U0[j][j] = U0[j][j] + elapsed * U1[j][j];
        U1[j][i] = U1[j][j] - A[j][i] * qiold[1];
        ddxj = A[j][i] * q[i1] + A[j][j] * q[j1] + U1[j][i];
        if (fabs(2 * x[j2] - ddxj) > fabs(2 * x[j2] + ddxj) / 2 || c == j) {
          double xj0 = x[j0] + (t - tx[j]) * x[j1] + (t - tx[j]) * (t - tx[j]) * x[j2];
          qi_proy = quantizer->state->qAux[i];
          qjold[0] = q[j0];
          qjold[1] = q[j1];
          U0[j][i] = U0[j][j] - A[j][i] * qi_proy;
          U1[i][j] = U1[i][i] - A[i][j] * q[j1];
          if (ddxj > 0)
            q[j0] = xj0 - 2 * lqu[j];
          else
            q[j0] = xj0 + 2 * lqu[j];
          q[j1] = (A[j][i] * (q[i0] + h * q[i1]) + A[j][j] * q[j0] + U0[j][i] + h * U1[j][i]) / (1 - h * A[j][j]);
          ddxi = A[i][i] * q[i1] + A[i][j] * q[j1] + U1[i][j];
          q[j0] = qjold[0];
          q[j1] = qjold[1];
          double a1 = A[i][i] * A[j][j];
          double a2 = A[i][j] * A[j][i];
          bool dA = FALSE;
          double tol = 1e-1;
          if (a2 != 0 && a1 / a2 < 1 + tol && a1 / a2 > 1 - tol) dA = TRUE;
          if (x[i2] * ddxi <= 0 || fabs(x[i2] * ddxi) < 1e-30 || dA) {
            U0[i][j] = U0[i][i] - A[i][j] * qj_proy;
            qiold[0] = q[i0];
            qiold[1] = q[i1];
            qjold[0] = q[j0];
            qjold[1] = q[j1];
            next[j0] = 1e20;
            next[j1] = 1e20;

            h = (quantizer->state->finTime - t) * 1;
            mLIQSS2_solver2x2_h(quantizer, x, q, next, i, j, h, xj0);
            if (fabs(q[i0] - x[i0]) < 2 * lqu[i] && fabs(next[j0] - xj0) < 2 * lqu[j])
              change[j] = TRUE;
            else {
              if (fabs(q[i0] - x[i0]) / lqu[i] > fabs(next[j0] - xj0) / lqu[j])
                h = h / sqrt(2 * fabs(q[i0] - x[i0]) / lqu[i]);
              else
                h = h / sqrt(2 * fabs(next[j0] - xj0) / lqu[j]);
              h /= 2;
              mLIQSS2_solver2x2_h(quantizer, x, q, next, i, j, h, xj0);
              if (fabs(q[i0] - x[i0]) < 2 * lqu[i] && fabs(next[j0] - xj0) < 2 * lqu[j])
                change[j] = TRUE;
              else {
                if (ddxi != 0 && ddxj != 0) {
                  if (fabs(lqu[i] / ddxi) < fabs(lqu[j] / ddxj))
                    h = sqrt(2 * fabs(lqu[i] / ddxi));
                  else
                    h = sqrt(2 * fabs(lqu[j] / ddxj));
                } else if (ddxi != 0)
                  h = sqrt(2 * fabs(lqu[i] / ddxi));
                else if (ddxj != 0)
                  h = sqrt(2 * fabs(lqu[j] / ddxj));
                h /= 2;
                mLIQSS2_solver2x2_h(quantizer, x, q, next, i, j, h, xj0);
                if (fabs(q[i0] - x[i0]) < 2 * lqu[i] && fabs(next[j0] - xj0) < 2 * lqu[j])
                  change[j] = TRUE;
                else {
                  h /= 10;
                  mLIQSS2_solver2x2_h(quantizer, x, q, next, i, j, h, xj0);
                  if (fabs(q[i0] - x[i0]) < 2 * lqu[i] && fabs(next[j0] - xj0) < 2 * lqu[j])
                    change[j] = TRUE;
                  else {
                    q[i0] = qiold[0];
                    q[i1] = qiold[1];
                    q[j0] = qjold[0];
                    q[j1] = qjold[1];
                    next[j0] = 1e20;
                    next[j1] = 1e20;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  mLIQSS2_old_dx(quantizer, i, t, nSD, x, tx);
  quantizer->state->cont = i;
}
