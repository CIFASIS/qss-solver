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

#include <qss/methods/liqss.h>

#include <math.h>
#include <stdlib.h>

#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

void QSS_FUNC_DECL(LIQSS, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int i;
  int states = simData->states;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->a = (double *)malloc(states * sizeof(double));
  quantizer->state->u0 = (double *)malloc(states * sizeof(double));
  quantizer->state->lt = (double *)malloc(states * sizeof(double));
  for (i = 0; i < states; i++) {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->lt[i] = simData->it;
  }
  quantizer->state->simTime = &simTime->time;
  QSS_ASSIGN_QUANTIZER_OPS(LIQSS)
}

void QSS_FUNC_DECL(LIQSS, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                              double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(LIQSS, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(LIQSS, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double *u = quantizer->state->u0;
  double *a = quantizer->state->a;
  if (quantizer->state->lt[var] == t) {
    double diffQ;
    diffQ = q[cf0] - quantizer->state->qAux[var];
    if (diffQ) {
      a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
    } else {
      a[var] = 0;
    }
  }
  u[var] = x[cf1] - q[cf0] * a[var];
  double dt1 = 0;
  if (x[cf1] != 0) {
    dt1 = (q[cf0] - x[cf0]) / x[cf1];
    if (dt1 > 0) {
      nTime[var] = t + dt1;
    } else {
      if (x[cf1] > 0) {
        nTime[var] = t + (q[cf0] + 2 * lqu[var] - x[cf0]) / x[cf1];
      } else {
        nTime[var] = t + (q[cf0] - 2 * lqu[var] - x[cf0]) / x[cf1];
      }
      if (nTime[var] < t) {
        nTime[var] = t + MIN_STEP;
      }
    }
  } else {
    nTime[var] = INF;
  }
}

void QSS_FUNC_DECL(LIQSS, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf1 = var * 2 + 1;
  if (x[cf1] == 0) {
    nTime[var] = INF;
  } else {
    nTime[var] = t + fabs(lqu[var] / x[cf1]);
  }
}

void QSS_FUNC_DECL(LIQSS, updateQuantizedState)(QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double dx;
  double *u = quantizer->state->u0;
  double *a = quantizer->state->a;
  double dq;
  quantizer->state->qAux[var] = q[cf0];
  quantizer->state->oldDx[var] = x[cf1];
  quantizer->state->lt[var] = *(quantizer->state->simTime);
  q[cf0] = x[cf0];
  if (x[cf1] > 0) {
    dx = u[var] + (q[cf0] + lqu[var]) * a[var];
    if (dx >= 0) {
      dq = lqu[var];
    } else if (dx < 0) {
      dq = -u[var] / a[var] - q[cf0];
    }
  } else {
    dx = u[var] + (q[cf0] - lqu[var]) * a[var];
    if (dx <= 0) {
      dq = -lqu[var];
    } else if (dx > 0) {
      dq = -u[var] / a[var] - q[cf0];
    }
  }
  if (dq > 2 * lqu[var]) dq = 2 * lqu[var];
  if (dq < -2 * lqu[var]) dq = -2 * lqu[var];
  q[cf0] += dq;
}
