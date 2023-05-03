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

#define EPS 1e-40

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void mLIQSS2_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int states = simData->states;
  quantizer->state->sts = states;
  int i, j;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->change = (bool *)malloc(states * sizeof(bool));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(double));
  quantizer->state->qj = (double *)malloc(3 * states * sizeof(double));
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  int infs = quantizer->state->infs;
  infs = 0;
  for (i = 0; i < states; i++) {
    infs += quantizer->state->nSD[i];  // cantidad de influencias
  }
  quantizer->state->S = (int **)malloc(states * sizeof(int *));  // matriz de sparcity
  for (i = 0; i < infs; i++) {
    quantizer->state->S[i] = (int *)malloc(states * sizeof(int));
  }
  quantizer->state->a = (double *)malloc(infs * sizeof(double));   // vector jacobiano
  quantizer->state->u0 = (double *)malloc(infs * sizeof(double));  // vector restos orden cero
  quantizer->state->u1 = (double *)malloc(infs * sizeof(double));  // vector restos orden uno
  quantizer->state->infs = infs;
  for (i = 0; i < infs; i++) {
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
  }
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->change[i] = FALSE;
    quantizer->state->flag2[i] = 0;
    quantizer->state->tx[i] = 0;
    quantizer->state->qj[cf0] = 0;
    quantizer->state->qj[cf0 + 1] = 0;
    quantizer->state->qj[cf0 + 2] = 0;
    for (j = 0; j < states; j++) {
      quantizer->state->S[i][j] = -1;
    }
  }

  int m, s = 0;
  for (i = 0; i < states; i++) {
    for (m = 0; m < quantizer->state->nSD[i]; m++) {
      quantizer->state->S[i][quantizer->state->SD[i][m]] = s;
      s++;
    }
  }

#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
#endif
  quantizer->state->finTime = simData->ft;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;

  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
  quantizer->ops->recomputeNextTimes = mLIQSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = mLIQSS2_recomputeNextTime;
  quantizer->ops->nextTime = mLIQSS2_nextTime;
  quantizer->ops->updateQuantizedState = mLIQSS2_updateQuantizedState;
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#else
void mLIQSS2_recomputeNextTime(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
#endif
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1, k;
  int *flag2 = quantizer->state->flag2;
  double *tx = quantizer->state->tx;
  bool *change = quantizer->state->change;
  int stind = quantizer->state->lSimTime->minIndex;
  bool self = quantizer->state->lSimTime->minIndex == i && quantizer->state->lSimTime->type == ST_State;
  double diffQ, timeaux, coeff[3];
  double *A = quantizer->state->a;
  double *U0 = quantizer->state->u0;
  double *U1 = quantizer->state->u1;
  int **S = quantizer->state->S;
  int sts = quantizer->state->sts;

  // Jacobian estimation
  if (t > 0) {
    diffQ = q[3 * stind] - quantizer->state->qAux[stind];
    // if(diffQ) A[S[i][stind]] = (x[i1] - quantizer->state->oldDx[i])/diffQ;
    if (diffQ && stind <= sts) A[S[i][stind]] = (x[i1] - quantizer->state->oldDx[i]) / diffQ;
    if (self && A[S[stind][stind]] > 0) A[S[stind][stind]] = 0;
  } else
    A[S[i][i]] = 0;
  // affine parameters
  U0[S[i][i]] = x[i1] - q[i0] * A[S[i][i]];
  U1[S[i][i]] = 2 * x[i2] - q[i1] * A[S[i][i]];
  // store last x update
  tx[i] = t;

  if (change[i] == TRUE)
    nTime[i] = t;
  else {
    coeff[1] = q[i1] - x[i1];
    coeff[2] = -x[i2];
    if (flag2[i] != 1) {
      if (quantizer->state->lSimTime->type == ST_Event && A[S[i][i]] < 0 &&
          quantizer->state->nSZ[i] >
              0) {  // we check if variable i is involved in the zero crossing function that produced the current event
        for (k = 0; k < quantizer->state->nSZ[i]; k++) {
          if (quantizer->state->SZ[i][k] == quantizer->state->lSimTime->minIndex) {
            nTime[i] = t;
            flag2[i] = 1;  // it does, so we restart the quantized state q[i]
          }
        }
      }
      if (flag2[i] != 1) {
        if (A[S[i][i]] < 0 && self && x[i2] != 0)
          nTime[i] = t + fabs((q[i1] - x[i1]) / x[i2]) / 2;
        else
          nTime[i] = INF;
        coeff[0] = q[i0] + 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[i]) nTime[i] = timeaux;
        coeff[0] = q[i0] - 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 2);
        if (timeaux < nTime[i]) nTime[i] = timeaux;
        if (q[i0] * q[i1] < 0 && fabs(q[i0]) > 10 * lqu[i]) {
          timeaux = -q[i0] / q[i1] - 2 * fabs(lqu[i] / q[i1]);
          if (nTime[i] > t + timeaux) nTime[i] = t + timeaux;
        }
        if (flag2[i] == 2 && self) flag2[i] = 0;
        double err1 = q[i0] - x[i0] + coeff[1] * (nTime[i] - t) / 2 + coeff[2] * pow((nTime[i] - t) / 2, 2);
        if (fabs(err1) > 2 * fabs(lqu[i])) nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
      }
    } else {
      if (self) {
        flag2[i] = 2;
        nTime[i] = t;
      }
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
void mLIQSS2_PAR_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
#else
void mLIQSS2_solver2x2_h(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0)
#endif
{
  int i0 = 3 * i, i1 = i0 + 1;
  int j0 = 3 * j, j1 = j0 + 1;
  double *a = quantizer->state->a;
  double *U0 = quantizer->state->u0;
  double *U1 = quantizer->state->u1;
  int **S = quantizer->state->S;
  double A[2][2] = {{a[S[i][i]], a[S[i][j]]}, {a[S[j][i]], a[S[j][j]]}};
  double u0[2] = {U0[S[i][j]], U0[S[j][i]]};
  double u1[2] = {U1[S[i][j]], U1[S[j][i]]};

  // B = I - h*A
  double B[2][2] = {{1 - h * A[0][0], -h * A[0][1]}, {-h * A[1][0], 1 - h * A[1][1]}};
  // c = u0 + h*u1
  double c[2] = {u0[0] + h * u1[0], u0[1] + h * u1[1]};
  double D[2][2];
  // D = inv(B)
  mLIQSS2_invA(B, D);
  double AD[2][2], DA[2][2], ADA[2][2];
  mLIQSS2_AxB(A, D, AD);
  mLIQSS2_AxB(D, A, DA);
  mLIQSS2_AxB(A, DA, ADA);
  // E = I + h*(DA-A) - h^2/2*ADA
  double E[2][2] = {{1 + h * (DA[0][0] - A[0][0]) - h * h / 2 * ADA[0][0], h * (DA[0][1] - A[0][1]) - h * h / 2 * ADA[0][1]},
                    {h * (DA[1][0] - A[1][0]) - h * h / 2 * ADA[1][0], 1 + h * (DA[1][1] - A[1][1]) - h * h / 2 * ADA[1][1]}};
  // f = h*(u0-D*c) + h^2/2*(AD*c+u1)
  double f[2] = {h * (u0[0] - D[0][0] * c[0] - D[0][1] * c[1]) + h * h / 2 * (AD[0][0] * c[0] + AD[0][1] * c[1] + u1[0]),
                 h * (u0[1] - D[1][0] * c[0] - D[1][1] * c[1]) + h * h / 2 * (AD[1][0] * c[0] + AD[1][1] * c[1] + u1[1])};
  double G[2][2];
  // G = inv(E)
  mLIQSS2_invA(E, G);

  q[i0] = G[0][0] * (x[i0] + f[0]) + G[0][1] * (xj0 + f[1]);
  qj[j0] = G[1][0] * (x[i0] + f[0]) + G[1][1] * (xj0 + f[1]);
  q[i1] = D[0][0] * (A[0][0] * q[i0] + A[0][1] * qj[j0] + c[0]) + D[0][1] * (A[1][0] * q[i0] + A[1][1] * qj[j0] + c[1]);
  qj[j1] = D[1][0] * (A[0][0] * q[i0] + A[0][1] * qj[j0] + c[0]) + D[1][1] * (A[1][0] * q[i0] + A[1][1] * qj[j0] + c[1]);
}

#ifdef QSS_PARALLEL
void mLIQSS2_PAR_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#else
void mLIQSS2_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
#endif
{
  int i0 = i * 3, i1 = i0 + 1, i2 = i1 + 1;
  int m, j, j0, j1, j2;
  int nSD = quantizer->state->nSD[i];
  bool *change = quantizer->state->change;
  double *qj = quantizer->state->qj;
  double t = quantizer->state->lSimTime->time;
  double *U0 = quantizer->state->u0;
  double *U1 = quantizer->state->u1;
  double *A = quantizer->state->a;
  double *tx = quantizer->state->tx;
  int **S = quantizer->state->S;
  double elapsed;

  elapsed = t - quantizer->state->lSimTime->tq[i];
  quantizer->state->qAux[i] = q[i0] + elapsed * q[i1];
  quantizer->state->oldDx[i] = x[i1];
  elapsed = t - tx[i];
  U0[S[i][i]] = U0[S[i][i]] + elapsed * U1[S[i][i]];
  tx[i] = t;

  if (change[i] == TRUE) {  // second variable update
    q[i0] = qj[i0];
    q[i1] = qj[i1];
    change[i] = FALSE;
  } else {  // normal update
    int *flag2 = quantizer->state->flag2;
    double h = 0, oldq1 = q[i1], ddx = x[i2] * 2;

    // LImLIQSS2
    if (A[S[i][i]] < 0) {
      if (ddx == 0) {
        ddx = A[S[i][i]] * A[S[i][i]] * q[i0] + A[S[i][i]] * U0[S[i][i]] + U1[S[i][i]];
        if (ddx == 0) ddx = 1e-40;
      }
      h = (quantizer->state->finTime - t);
      q[i0] = ((x[i0] + h * U0[S[i][i]] + h * h / 2 * U1[S[i][i]]) * (1 - h * A[S[i][i]]) +
               (h * h / 2 * A[S[i][i]] - h) * (U0[S[i][i]] + h * U1[S[i][i]])) /
              (1 - h * A[S[i][i]] + h * h * A[S[i][i]] * A[S[i][i]] / 2);
      if (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
        h = sqrt(fabs(2 * lqu[i] / ddx));
        q[i0] = ((x[i0] + h * U0[S[i][i]] + h * h / 2 * U1[S[i][i]]) * (1 - h * A[S[i][i]]) +
                 (h * h / 2 * A[S[i][i]] - h) * (U0[S[i][i]] + h * U1[S[i][i]])) /
                (1 - h * A[S[i][i]] + h * h * A[S[i][i]] * A[S[i][i]] / 2);
      }
      while (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
        h = h * sqrt(lqu[i] / fabs(q[i0] - x[i0]));
        q[i0] = ((x[i0] + h * U0[S[i][i]] + h * h / 2 * U1[S[i][i]]) * (1 - h * A[S[i][i]]) +
                 (h * h / 2 * A[S[i][i]] - h) * (U0[S[i][i]] + h * U1[S[i][i]])) /
                (1 - h * A[S[i][i]] + h * h * A[S[i][i]] * A[S[i][i]] / 2);
      }
      q[i1] = (A[S[i][i]] * q[i0] + U0[S[i][i]] + h * U1[S[i][i]]) / (1 - h * A[S[i][i]]);
    } else {
      ddx = U1[S[i][i]];
      if (ddx > 0)
        q[i0] = x[i0] - lqu[i];
      else
        q[i0] = x[i0] + lqu[i];
      if (ddx != 0) {
        h = sqrt(2 * lqu[i] / fabs(ddx));
        q[i1] = U0[S[i][i]] + h * U1[S[i][i]];
      } else
        q[i1] = U0[S[i][i]];
    }
    if (fabs(q[i0] - x[i0]) > 1.9 * lqu[i]) q[i0] = x[i0];
    if (flag2[i] >= 1) {
      if (lqu[i] > 1e-4) {
        if (oldq1 > 0)
          q[i0] = quantizer->state->qAux[i] + lqu[i] / 1e2;
        else
          q[i0] = quantizer->state->qAux[i] - lqu[i] / 1e2;
      } else {
        if (oldq1 > 0)
          q[i0] = quantizer->state->qAux[i] + lqu[i] / 1e1;
        else
          q[i0] = quantizer->state->qAux[i] - lqu[i] / 1e1;
      }

      /*
      if (oldq1>0) q[i0] = quantizer->state->qAux[i] + lqu[i]/1e2;
      else q[i0] = quantizer->state->qAux[i] - lqu[i]/1e2;
      */
    }

    // mLImLIQSS2 - 2x2
    if (t > 0 && flag2[i] == 0 && 1) {
      double u1ji, ddxj, u0jj, xj1, qj_proy, u0ji, dxj;
      double qi_proy = quantizer->state->qAux[i];
      int jmax = -1;
      // find 2x2 variable
      for (m = 0; m < nSD; m++) {
        j = quantizer->state->SD[i][m];
        j0 = 3 * j;
        j1 = 3 * j + 1;
        j2 = j1 + 1;
        if (j != i && A[S[i][j]] * A[S[j][i]] != 0) {
          u1ji = U1[S[j][j]] - A[S[j][i]] * oldq1;
          ddxj = A[S[j][i]] * q[i1] + A[S[j][j]] * q[j1] + u1ji;
          elapsed = t - tx[j];
          u0jj = U0[S[j][j]] + elapsed * U1[S[j][j]];
          xj1 = x[j1] + 2 * elapsed * x[j2];
          elapsed = t - quantizer->state->lSimTime->tq[j];
          qj_proy = q[j0] + elapsed * q[j1];
          u0ji = u0jj - A[S[j][i]] * qi_proy;
          dxj = A[S[j][i]] * q[i0] + A[S[j][j]] * qj_proy + u0ji;
          if (fabs(2 * x[j2] - ddxj) > fabs(2 * x[j2] + ddxj) / 2 || fabs(xj1 - dxj) > fabs(xj1 + dxj) / 2) {
            jmax = j;
            break;
          }
        }
      }
      // solve 2x2
      if (jmax != -1) {
        j0 = 3 * j;
        j1 = 3 * j + 1;
        j2 = j1 + 1;
        double qsingle[2] = {q[i0], q[i1]};
        U0[S[j][j]] = u0jj;
        U1[S[j][i]] = u1ji;
        double xj0 = x[j0] + (t - tx[j]) * x[j1] + (t - tx[j]) * (t - tx[j]) * x[j2];
        double qj0, qj1;
        U0[S[j][i]] = u0ji;
        U1[S[i][j]] = U1[S[i][i]] - A[S[i][j]] * q[j1];
        if (ddxj > 0)
          qj0 = xj0 - lqu[j];
        else
          qj0 = xj0 + lqu[j];
        qj1 = (A[S[j][i]] * (q[i0] + h * q[i1]) + A[S[j][j]] * qj0 + U0[S[j][i]] + h * U1[S[j][i]]) / (1 - h * A[S[j][j]]);
        double ddxi = A[S[i][i]] * q[i1] + A[S[i][j]] * qj1 + U1[S[i][j]];
        double a1 = A[S[i][i]] * A[S[j][j]];
        double a2 = A[S[i][j]] * A[S[j][i]];
        bool dA = FALSE;
        double tol = 1e-3;
        if (a2 != 0 && fabs(a1 / a2) < 1 + tol && fabs(a1 / a2) > 1 - tol) dA = TRUE;
        double u0ij = U0[S[i][i]] - A[S[i][j]] * qj_proy;
        double dxi = A[S[i][i]] * q[i0] + A[S[i][j]] * qj_proy + u0ij;
        if (fabs(2 * x[i2] - ddxi) > fabs(2 * x[i2] + ddxi) / 2 || fabs(x[i1] - dxi) > fabs(x[i1] + dxi) / 2 || dA) {
          U0[S[i][j]] = U0[S[i][i]] - A[S[i][j]] * qj_proy;
          h = (quantizer->state->finTime - t);
          mLIQSS2_solver2x2_h(quantizer, x, q, qj, i, j, h, xj0);
          if (fabs(q[i0] - x[i0]) > 2 * lqu[i] || fabs(qj[j0] - xj0) > 2 * lqu[j]) {
            ddxi = A[S[i][i]] * q[i1] + A[S[i][j]] * qj[j1] + U1[S[i][j]];
            ddxj = A[S[j][i]] * q[i1] + A[S[j][j]] * qj[j1] + U1[S[j][i]];
            if (ddxi != 0 && ddxj != 0) {
              if (fabs(lqu[i] / ddxi) < fabs(lqu[j] / ddxj))
                h = sqrt(2 * fabs(lqu[i] / ddxi));
              else
                h = sqrt(2 * fabs(lqu[j] / ddxj));
            } else if (ddxi != 0)
              h = sqrt(2 * fabs(lqu[i] / ddxi));
            else if (ddxj != 0)
              h = sqrt(2 * fabs(lqu[j] / ddxj));
            mLIQSS2_solver2x2_h(quantizer, x, q, qj, i, j, h, xj0);
            int it = 0;
            while ((fabs(q[i0] - x[i0]) > 1.9 * lqu[i] || fabs(qj[j0] - xj0) > 1.9 * lqu[j]) && it < 9) {
              it++;
              if (lqu[i] / fabs(q[i0] - x[i0] + EPS) < lqu[j] / fabs(qj[j0] - xj0 + EPS))
                h = h * sqrt(lqu[i] / fabs(q[i0] - x[i0] + EPS));
              else
                h = h * sqrt(lqu[j] / fabs(qj[j0] - xj0 + EPS));
              mLIQSS2_solver2x2_h(quantizer, x, q, qj, i, j, h, xj0);
            }
            if (it != 9)
              change[j] = TRUE;
            else {
              q[i0] = qsingle[0];
              q[i1] = qsingle[1];
            }
          } else
            change[j] = TRUE;
        }
      }
    }
  }
  // store values of dx/dt for every influenced variable
  if (t > 0) {
    for (m = 0; m < nSD; m++) {
      j = quantizer->state->SD[i][m];
      j0 = j * 3;
      j1 = j0 + 1;
      j2 = j1 + 1;
      quantizer->state->oldDx[j] = x[j1] + 2 * (t - tx[j]) * x[j2];
    }
  }
}
