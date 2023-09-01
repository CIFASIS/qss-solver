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

#include <qss/methods/mliqss3.h>

#include <math.h>
#include <stdlib.h>

#include <common/data.h>
#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

#define EPS 1e-40

void QSS_FUNC_DECL(mLIQSS3, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int i, j;
  int states = simData->states;
  quantizer->state->sts = states;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(int));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));
  quantizer->state->sim_step = (bool *)malloc(states * sizeof(bool));
  quantizer->state->qj = (double *)malloc(4 * states * sizeof(double));
  quantizer->state->nSD = simData->nSD;
  quantizer->state->SD = simData->SD;
  quantizer->state->S = (int **)malloc(states * sizeof(int *));  // sparcity matrix
  for (i = 0; i < states; i++) quantizer->state->S[i] = (int *)malloc(states * sizeof(int));
  int infs = 0;
  for (i = 0; i < states; i++) infs += quantizer->state->nSD[i];  // number of influences
  quantizer->state->infs = infs;
  quantizer->state->a = (double *)malloc(infs * sizeof(double));   // jacobian vector
  quantizer->state->u0 = (double *)malloc(infs * sizeof(double));  // zero order affine parameters
  quantizer->state->u1 = (double *)malloc(infs * sizeof(double));  // first order affine parameters
  quantizer->state->u2 = (double *)malloc(infs * sizeof(double));  // second order affine parameters
  for (i = 0; i < infs; i++) {
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
    quantizer->state->u2[i] = 0;
  }
  for (i = 0; i < states; i++) {
    int cf0 = i * 4;
    simData->x[cf0 + 2] = 0;
    simData->x[cf0 + 3] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    simData->q[cf0 + 2] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->tx[i] = 0;
    quantizer->state->flag2[i] = 0;
    quantizer->state->sim_step[i] = FALSE;
    quantizer->state->qj[cf0] = 0;
    quantizer->state->qj[cf0 + 1] = 0;
    quantizer->state->qj[cf0 + 2] = 0;
    quantizer->state->qj[cf0 + 3] = 0;
    for (j = 0; j < states; j++) quantizer->state->S[i][j] = -1;
  }
  int m, s = 0;
  for (i = 0; i < states; i++) {
    for (m = 0; m < quantizer->state->nSD[i]; m++) {
      quantizer->state->S[i][quantizer->state->SD[i][m]] = s;
      s++;
    }
  }
  quantizer->state->finTime = simData->ft;
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
  QSS_ASSIGN_QUANTIZER_OPS(mLIQSS3)
}

void QSS_FUNC_DECL(mLIQSS3, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                                double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] > NOT_ASSIGNED) {)
      QSS_FUNC_INVK(mLIQSS3, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(mLIQSS3, recomputeNextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu, double *q)
{
  int i0 = i * 4, i1 = i0 + 1, i2 = i1 + 1, i3 = i2 + 1, k;
  int *flag2 = quantizer->state->flag2;
  int stind = quantizer->state->lSimTime->minIndex;
  int **S = quantizer->state->S;
  bool self = quantizer->state->lSimTime->minIndex == i && quantizer->state->lSimTime->type == ST_State;
  bool *sim_step = quantizer->state->sim_step;
  double coeff[4], diffQ, timeaux;
  double *tx = quantizer->state->tx;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  int sts = quantizer->state->sts;

  // Jacobian estimation
  if (t > 0) {
    diffQ = q[4 * stind] - quantizer->state->qAux[stind];
    if (diffQ && stind < sts) a[S[i][stind]] = (x[i1] - quantizer->state->oldDx[i]) / diffQ;
    if (self && a[S[stind][stind]] > 0) a[S[stind][stind]] = -EPS;
  } else
    a[S[i][i]] = 0;
  // affine parameters
  u0[S[i][i]] = x[i1] - q[i0] * a[S[i][i]];
  u1[S[i][i]] = 2 * x[i2] - q[i1] * a[S[i][i]];
  u2[S[i][i]] = 3 * x[i3] - q[i2] * a[S[i][i]];
  // store last x update
  tx[i] = t;

  if (sim_step[i] == TRUE)
    nTime[i] = t;
  else {
    coeff[1] = q[i1] - x[i1];
    coeff[2] = q[i2] - x[i2];
    coeff[3] = -x[i3];
    if (flag2[i] != 1) {
      if (quantizer->state->lSimTime->type == ST_Event && a[S[i][i]] < 0 &&
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
        // if(a[S[i][i]] < -EPS*2 && self) nTime[i] = t + fabs((q[i2]-x[i2])/(x[i3]+EPS));
        if (a[S[i][i]] < -EPS * 2 && self)
          nTime[i] = t + fabs((q[i2] - x[i2]) / (x[i3] + EPS)) * 6;
        else
          nTime[i] = INF;
        coeff[0] = q[i0] - 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 3);
        if (timeaux < nTime[i]) nTime[i] = timeaux;
        coeff[0] = q[i0] + 2 * lqu[i] - x[i0];
        timeaux = t + minPosRoot(coeff, 3);
        if (timeaux < nTime[i]) nTime[i] = timeaux;
        if (flag2[i] == 2 && self) flag2[i] = 0;
        coeff[0] = q[i0] - x[i0];
        double err1 =
            coeff[0] + coeff[1] * (nTime[i] - t) / 2 + coeff[2] * pow((nTime[i] - t) / 2, 2) + coeff[3] * pow((nTime[i] - t) / 3, 3) / 1;
        if (fabs(err1) > 3 * fabs(lqu[i])) nTime[i] = t + quantizer->state->finTime * quantizer->state->minStep;
      }
    } else {
      if (self) {
        flag2[i] = 2;
        nTime[i] = t;
      }
    }
  }
}

void QSS_FUNC_DECL(mLIQSS3, nextTime)(QA_quantizer quantizer, int i, double t, double *nTime, double *x, double *lqu)
{
  int i3 = i * 4 + 3;
  if (x[i3])
    nTime[i] = t + pow(lqu[i] / fabs(x[i3]), 1.0 / 3);
  else
    nTime[i] = INF;
}

void QSS_FUNC_DECL(mLIQSS3, AxB)(double A[2][2], double B[2][2], double AB[2][2])
{
  AB[0][0] = A[0][0] * B[0][0] + A[0][1] * B[1][0];
  AB[0][1] = A[0][0] * B[0][1] + A[0][1] * B[1][1];
  AB[1][0] = A[1][0] * B[0][0] + A[1][1] * B[1][0];
  AB[1][1] = A[1][0] * B[0][1] + A[1][1] * B[1][1];
}

void QSS_FUNC_DECL(mLIQSS3, invA)(double A[2][2], double invA[2][2])
{
  double detA = A[0][0] * A[1][1] - A[0][1] * A[1][0];
  if (detA == 0) detA = 1e-10;
  invA[0][0] = A[1][1] / detA;
  invA[0][1] = -A[0][1] / detA;
  invA[1][0] = -A[1][0] / detA;
  invA[1][1] = A[0][0] / detA;
}

void QSS_FUNC_DECL(mLIQSS3, solver2x2_h)(QA_quantizer quantizer, double *x, double *q, double *qj, int i, int j, double h, double xj0,
                                         double *lqu)
{
  int i0 = 4 * i, i1 = i0 + 1, i2 = i1 + 1;
  int j0 = 4 * j, j1 = j0 + 1, j2 = j1 + 1;
  int **S = quantizer->state->S;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  double A[2][2] = {{a[S[i][i]], a[S[i][j]]}, {a[S[j][i]], a[S[j][j]]}};
  double U0[2] = {u0[S[i][j]], u0[S[j][i]]};
  double U1[2] = {u1[S[i][j]], u1[S[j][i]]};
  double U2[2] = {u2[S[i][j]], u2[S[j][i]]};

  // B = I - h*A
  double B[2][2] = {{1 - h * A[0][0], -h * A[0][1]}, {-h * A[1][0], 1 - h * A[1][1]}};
  // c = U1 + h*U2
  double c[2] = {U1[0] + h * U2[0], U1[1] + h * U2[1]};
  double D[2][2];
  // D = inv(B)
  QSS_FUNC_INVK(mLIQSS3, invA)(B, D);
  double AD[2][2], DA[2][2], ADA[2][2];
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, D, AD);
  QSS_FUNC_INVK(mLIQSS3, AxB)(D, A, DA);
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, DA, ADA);
  // E = I + h*(DA-A) - h²/2*ADA
  double E[2][2] = {{1 + h * (DA[0][0] - A[0][0]) - h * h / 2 * ADA[0][0], h * (DA[0][1] - A[0][1]) - h * h / 2 * ADA[0][1]},
                    {h * (DA[1][0] - A[1][0]) - h * h / 2 * ADA[1][0], 1 + h * (DA[1][1] - A[1][1]) - h * h / 2 * ADA[1][1]}};
  // f = U0 + h*(U1-D*c) + h²/2*(AD*c+U2)
  double f[2] = {U0[0] + h * (U1[0] - D[0][0] * c[0] - D[0][1] * c[1]) + h * h / 2 * (AD[0][0] * c[0] + AD[0][1] * c[1] + U2[0]),
                 U0[1] + h * (U1[1] - D[1][0] * c[0] - D[1][1] * c[1]) + h * h / 2 * (AD[1][0] * c[0] + AD[1][1] * c[1] + U2[1])};
  double G[2][2];
  // G = inv(E)
  QSS_FUNC_INVK(mLIQSS3, invA)(E, G);
  double GA[2][2], AGA[2][2], DAGA[2][2], ADAGA[2][2], AG[2][2], DAG[2][2], ADAG[2][2];
  QSS_FUNC_INVK(mLIQSS3, AxB)(G, A, GA);
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, GA, AGA);
  QSS_FUNC_INVK(mLIQSS3, AxB)(D, AGA, DAGA);
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, DAGA, ADAGA);
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, G, AG);
  QSS_FUNC_INVK(mLIQSS3, AxB)(D, AG, DAG);
  QSS_FUNC_INVK(mLIQSS3, AxB)(A, DAG, ADAG);
  // J = I + h*(GA-A) + h²/2*(DAGA-AGA) - h³/6*ADAGA
  double J[2][2] = {{1 + h * (GA[0][0] - A[0][0]) + h * h / 2 * (DAGA[0][0] - AGA[0][0]) - h * h * h / 6 * ADAGA[0][0],
                     h * (GA[0][1] - A[0][1]) + h * h / 2 * (DAGA[0][1] - AGA[0][1]) - h * h * h / 6 * ADAGA[0][1]},
                    {h * (GA[1][0] - A[1][0]) + h * h / 2 * (DAGA[1][0] - AGA[1][0]) - h * h * h / 6 * ADAGA[1][0],
                     1 + h * (GA[1][1] - A[1][1]) + h * h / 2 * (DAGA[1][1] - AGA[1][1]) - h * h * h / 6 * ADAGA[1][1]}};
  // k = h*(U0-Gf) + h²/2*((AG-DAG)*f+U1-Dc) + h³/6*(ADAGf+ADc+U2)
  double k[2] = {h * (U0[0] - G[0][0] * f[0] - G[0][1] * f[1]) +
                     h * h / 2 * ((AG[0][0] - DAG[0][0]) * f[0] + (AG[0][1] - DAG[0][1]) * f[1] + U1[0] - D[0][0] * c[0] - D[0][1] * c[1]) +
                     h * h * h / 6 * (ADAG[0][0] * f[0] + ADAG[0][1] * f[1] + AD[0][0] * c[0] + AD[0][1] * c[1] + U2[0]),
                 h * (U0[1] - G[1][0] * f[0] - G[1][1] * f[1]) +
                     h * h / 2 * ((AG[1][0] - DAG[1][0]) * f[0] + (AG[1][1] - DAG[1][1]) * f[1] + U1[1] - D[1][0] * c[0] - D[1][1] * c[1]) +
                     h * h * h / 6 * (ADAG[1][0] * f[0] + ADAG[1][1] * f[1] + AD[1][0] * c[0] + AD[1][1] * c[1] + U2[1])};
  double M[2][2];
  // M = inv(J)
  QSS_FUNC_INVK(mLIQSS3, invA)(J, M);
  q[i0] = M[0][0] * (x[i0] + k[0]) + M[0][1] * (xj0 + k[1]);
  qj[j0] = M[1][0] * (x[i0] + k[0]) + M[1][1] * (xj0 + k[1]);
  q[i1] = G[0][0] * (A[0][0] * q[i0] + A[0][1] * qj[j0] + f[0]) + G[0][1] * (A[1][0] * q[i0] + A[1][1] * qj[j0] + f[1]);
  qj[j1] = G[1][0] * (A[0][0] * q[i0] + A[0][1] * qj[j0] + f[0]) + G[1][1] * (A[1][0] * q[i0] + A[1][1] * qj[j0] + f[1]);
  q[i2] = (D[0][0] * (A[0][0] * q[i1] + A[0][1] * qj[j1] + c[0]) + D[0][1] * (A[1][0] * q[i1] + A[1][1] * qj[j1] + c[1])) / 2;
  qj[j2] = (D[1][0] * (A[0][0] * q[i1] + A[0][1] * qj[j1] + c[0]) + D[1][1] * (A[1][0] * q[i1] + A[1][1] * qj[j1] + c[1])) / 2;
}

void QSS_FUNC_DECL(mLIQSS3, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  int i0 = i * 4, i1 = i0 + 1, i2 = i1 + 1, i3 = i2 + 1;
  int m, j, j0, j1, j2, j3;
  int nSD = quantizer->state->nSD[i];
  int **S = quantizer->state->S;
  bool *sim_step = quantizer->state->sim_step;
  double elapsed, oldq1, h = 0;
  double t = quantizer->state->lSimTime->time;
  double *qj = quantizer->state->qj;
  double *tu = quantizer->state->tx;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;

  elapsed = t - quantizer->state->lSimTime->tq[i];
  quantizer->state->qAux[i] = q[i0] + elapsed * q[i1] + elapsed * elapsed * q[i2];
  oldq1 = q[i1] + 2 * elapsed * q[i2];
  quantizer->state->oldDx[i] = x[i1];
  elapsed = t - tu[i];
  u0[S[i][i]] = u0[S[i][i]] + elapsed * u1[S[i][i]] + elapsed * elapsed * u2[S[i][i]];
  u1[S[i][i]] = u1[S[i][i]] + 2 * elapsed * u2[S[i][i]];
  tu[i] = t;

  if (sim_step[i] == TRUE) {  // second variable update
    q[i0] = qj[i0];
    q[i1] = qj[i1];
    q[i2] = qj[i2];
    sim_step[i] = FALSE;
  } else {  // normal update
    double dddx = x[i3] * 6;
    int *flag2 = quantizer->state->flag2;
    double qi_proy[3] = {quantizer->state->qAux[i], q[i1] + 2 * elapsed * q[i2], q[i2]};

    // LImLIQSS3
    if (a[S[i][i]] < 0) {
      if (dddx == 0) {
        dddx = a[S[i][i]] * a[S[i][i]] * a[S[i][i]] * q[i0] + a[S[i][i]] * a[S[i][i]] * u0[S[i][i]] + a[S[i][i]] * u1[S[i][i]] +
               2 * u2[S[i][i]];
        if (dddx == 0) dddx = 1e-40;
      }
      h = (quantizer->state->finTime - t);
      q[i0] = (-3 * x[i0] * (h * h * a[S[i][i]] * a[S[i][i]] - 2 * h * a[S[i][i]] + 2) - h * h * h * a[S[i][i]] * a[S[i][i]] * u0[S[i][i]] -
               h * h * h * a[S[i][i]] * u1[S[i][i]] - h * h * h * u2[S[i][i]] * 2) /
              (h * h * h * a[S[i][i]] * a[S[i][i]] * a[S[i][i]] - 3 * h * h * a[S[i][i]] * a[S[i][i]] + 6 * h * a[S[i][i]] - 6);
      if (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
        h = pow(fabs(6 * lqu[i] / dddx), 1.0 / 3);
        q[i0] = (-3 * x[i0] * (h * h * a[S[i][i]] * a[S[i][i]] - 2 * h * a[S[i][i]] + 2) -
                 h * h * h * a[S[i][i]] * a[S[i][i]] * u0[S[i][i]] - h * h * h * a[S[i][i]] * u1[S[i][i]] - h * h * h * u2[S[i][i]] * 2) /
                (h * h * h * a[S[i][i]] * a[S[i][i]] * a[S[i][i]] - 3 * h * h * a[S[i][i]] * a[S[i][i]] + 6 * h * a[S[i][i]] - 6);
      }

      while (fabs(q[i0] - x[i0]) > 2 * lqu[i]) {
        h = h * pow(lqu[i] / fabs(q[i0] - x[i0]), 1.0 / 3);
        q[i0] = (-3 * x[i0] * (h * h * a[S[i][i]] * a[S[i][i]] - 2 * h * a[S[i][i]] + 2) -
                 h * h * h * a[S[i][i]] * a[S[i][i]] * u0[S[i][i]] - h * h * h * a[S[i][i]] * u1[S[i][i]] - h * h * h * u2[S[i][i]] * 2) /
                (h * h * h * a[S[i][i]] * a[S[i][i]] * a[S[i][i]] - 3 * h * h * a[S[i][i]] * a[S[i][i]] + 6 * h * a[S[i][i]] - 6);
      }
      q[i1] =
          (-(2 * (a[S[i][i]] * q[i0] + u0[S[i][i]])) * (h * a[S[i][i]] - 1) - h * h * a[S[i][i]] * u1[S[i][i]] - h * h * u2[S[i][i]] * 2) /
          (h * h * a[S[i][i]] * a[S[i][i]] - 2 * h * a[S[i][i]] + 2);
      q[i2] = (h * u2[S[i][i]] * 2 + a[S[i][i]] * q[i1] + u1[S[i][i]]) / (-h * a[S[i][i]] + 1) / 2;
    } else {
      dddx = 2 * u2[S[i][i]];
      if (dddx > 0)
        q[i0] = x[i0] - lqu[i];
      else
        q[i0] = x[i0] + lqu[i];
      if (dddx != 0) {
        h = pow(6 * fabs(lqu[i] / dddx), 1.0 / 3);
        q[i1] = u0[S[i][i]] + h * u1[S[i][i]] + h * h * u2[S[i][i]];
        q[i2] = u1[S[i][i]] + 2 * h * u2[S[i][i]];
      } else {
        q[i1] = u0[S[i][i]];
        q[i2] = u1[S[i][i]];
      }
    }

    if (fabs(q[i0] - x[i0]) > 1.9 * lqu[i]) q[i0] = x[i0];
    if (flag2[i] >= 1) {
      if (oldq1 > 0)
        q[i0] = quantizer->state->qAux[i] + lqu[i] / 1e5;
      else
        q[i0] = quantizer->state->qAux[i] - lqu[i] / 1e5;
    }

    // mLImLIQSS3 - 2x2
    if (t > 0 && flag2[i] == 0 && 1) {
      double u2ji, dddxj, xj2, u1jj, u1ji, ddxj, xj1, u0jj, u0ji, dxj, qj_proy[2];
      int jmax = -1;
      // find 2x2 variable
      for (m = 0; m < nSD; m++) {
        j = quantizer->state->SD[i][m];
        j0 = 4 * j;
        j1 = j0 + 1;
        j2 = j1 + 1;
        j3 = j2 + 1;
        if (j != i && a[S[i][j]] * a[S[j][i]] != 0) {
          // third time derivative state j
          u2ji = u2[S[j][j]] - a[S[j][i]] * qi_proy[2];
          dddxj = 2 * (a[S[j][i]] * q[i2] + a[S[j][j]] * q[j2] + u2ji);
          // second time derivative state j
          elapsed = t - quantizer->state->lSimTime->tq[j];
          qj_proy[0] = q[j0] + elapsed * q[j1] + elapsed * elapsed * q[j2];
          qj_proy[1] = q[j1] + 2 * elapsed * q[j2];
          elapsed = t - tu[j];
          xj2 = x[j2] * 2 + 6 * elapsed * x[j3];  // actual ddxj
          u1jj = u1[S[j][j]] + 2 * elapsed * u2[S[j][j]];
          u1ji = u1jj - a[S[j][i]] * qi_proy[1];
          ddxj = a[S[j][i]] * q[i1] + a[S[j][j]] * qj_proy[1] + u1ji;
          // first time derivative state j
          xj1 = x[j1] + 2 * elapsed * x[j2] + 3 * elapsed * elapsed * x[j3];  // actual dxj
          u0jj = u0[S[j][j]] + elapsed * u1[S[j][j]] + elapsed * elapsed * u2[S[j][j]];
          u0ji = u0jj - a[S[j][i]] * qi_proy[0];
          dxj = a[S[j][i]] * q[i0] + a[S[j][j]] * qj_proy[0] + u0ji;
          if (fabs(6 * x[j3] - dddxj) > fabs(6 * x[j3] + dddxj) / 3 || fabs(xj2 - ddxj) > fabs(xj2 + ddxj) / 2 ||
              fabs(xj1 - dxj) > fabs(xj1 + dxj) / 2) {
            jmax = j;
            break;
          }
        }
      }
      // solve 2x2
      if (jmax != -1) {
        double qsingle[3] = {q[i0], q[i1], q[i2]};
        u0[S[j][j]] = u0jj;
        u0[S[j][i]] = u0ji;
        u1[S[j][j]] = u1jj;
        u1[S[j][i]] = u1ji;
        u2[S[j][i]] = u2ji;
        // project x[j0]
        double xj0 = x[j0] + elapsed * x[j1] + elapsed * elapsed * x[j2] + elapsed * elapsed * elapsed * x[j3];
        double qj0, qj1, qj2;
        // estimate q[j0] => q[j1] and q[j2]
        if (dddxj > 0)
          qj0 = xj0 - lqu[j];
        else
          qj0 = xj0 + lqu[j];
        qj1 = (-(qj0 * a[S[j][j]] + q[i0] * a[S[j][i]] + u0[S[j][i]]) * (2 * h * a[S[j][j]] - 1) - h * h * q[i1] * a[S[j][i]] * a[S[j][j]] -
               h * h * q[i2] * a[S[j][i]] - h * h * a[S[j][j]] * u1[S[j][i]] - h * h * u2[S[j][i]]) /
              (h * h * a[S[j][j]] * a[S[j][j]] - 2 * h * a[S[j][j]] + 1);
        qj2 = (2 * h * q[i2] * a[S[j][i]] + 2 * h * u2[S[j][i]] + qj1 * a[S[j][j]] + q[i1] * a[S[j][i]] + u1[S[j][i]]) /
              (-2 * h * a[S[j][j]] + 1) / 2;
        // third time derivative state i
        u2[S[i][j]] = u2[S[i][i]] - a[S[i][j]] * q[j2];
        double dddxi = 2 * (a[S[i][i]] * q[i2] + a[S[i][j]] * qj2 + u2[S[i][j]]);
        // second time derivative state i
        u1[S[i][j]] = u1[S[i][i]] - a[S[i][j]] * qj_proy[1];
        double ddxi = a[S[i][i]] * q[i1] + a[S[i][j]] * qj1 + u1[S[i][j]];
        // first time derivative state i
        u0[S[i][j]] = u0[S[i][i]] - a[S[i][j]] * qj_proy[0];
        double dxi = a[S[j][i]] * q[i0] + a[S[j][j]] * qj0 + u0[S[i][j]];
        // det A
        double a1 = a[S[i][i]] * a[S[j][j]];
        double a2 = a[S[i][j]] * a[S[j][i]];
        bool dA = FALSE;
        double tol = 1e-3;
        if (a2 != 0 && fabs(a1 / a2) < 1 + tol && fabs(a1 / a2) > 1 - tol) dA = TRUE;
        if (fabs(6 * x[i3] - dddxi) > fabs(6 * x[i3] + dddxi) / 3 || fabs(2 * x[i2] - ddxi) > fabs(2 * x[i2] + ddxi) / 2 ||
            fabs(x[i1] - dxi) > fabs(x[i1] + dxi) / 2 || dA) {
          h = (quantizer->state->finTime - t) * 100;
          QSS_FUNC_INVK(mLIQSS3, solver2x2_h)(quantizer, x, q, qj, i, j, h, xj0, lqu);
          if (fabs(q[i0] - x[i0]) > 2 * lqu[i] || fabs(qj[j0] - xj0) > 2 * lqu[j]) {
            int K = 12;
            if (dddxi != 0 && dddxj != 0) {
              if (fabs(lqu[i] / dddxi) > fabs(lqu[j] / dddxj))
                h = pow(K * fabs(lqu[i] / dddxi), 1.0 / 3);
              else
                h = pow(K * fabs(lqu[j] / dddxj), 1.0 / 3);
            } else if (dddxi != 0)
              h = pow(K * fabs(lqu[i] / dddxi), 1.0 / 3);
            else if (dddxj != 0)
              h = pow(K * fabs(lqu[j] / dddxj), 1.0 / 3);
            QSS_FUNC_INVK(mLIQSS3, solver2x2_h)(quantizer, x, q, qj, i, j, h, xj0, lqu);

            int it = 0;
            while ((fabs(q[i0] - x[i0]) > 1.98 * lqu[i] || fabs(qj[j0] - xj0) > 1.98 * lqu[j]) && it < 3) {
              it++;
              h *= 0.1;
              QSS_FUNC_INVK(mLIQSS3, solver2x2_h)(quantizer, x, q, qj, i, j, h, xj0, lqu);
            }
            if (it != 3)
              sim_step[j] = TRUE;
            else {
              q[i0] = qsingle[0];
              q[i1] = qsingle[1];
              q[i2] = qsingle[2];
            }
          } else
            sim_step[j] = TRUE;
        }
      }
    }
  }
  if (t > 0) {
    for (m = 0; m < nSD; m++) {
      j = quantizer->state->SD[i][m];
      j0 = j * 4;
      j1 = j0 + 1;
      j2 = j1 + 1;
      j3 = j2 + 1;
      quantizer->state->oldDx[j] = x[j1] + (t - tu[j]) * x[j2] * 2 + (t - tu[j]) * (t - tu[j]) * x[j3] * 3;
    }
  }
}
