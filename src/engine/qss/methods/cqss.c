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

#include <qss/methods/cqss.h>

#include <math.h>
#include <stdlib.h>

#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

void QSS_FUNC_DECL(CQSS, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int i;
  int states = simData->states;

  quantizer->state->qsup = (double *)malloc(states * sizeof(double));
  quantizer->state->qinf = (double *)malloc(states * sizeof(double));
  quantizer->state->lt = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(sizeof(int));
  for (i = 0; i < states; i++) {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
    quantizer->state->qsup[i] = simData->q[cf0] + simData->dQMin[i];
    quantizer->state->qinf[i] = simData->q[cf0] - simData->dQMin[i];
    quantizer->state->lt[i] = simData->it;
  }
  quantizer->state->flag2[0] = 0;
  quantizer->state->simTime = &simTime->time;
  QSS_ASSIGN_QUANTIZER_OPS(CQSS)
}

void QSS_FUNC_DECL(CQSS, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  if (quantizer->state->lt[var] == t) {
    if ((x[cf1] * (q[cf0] - x[cf0])) < 0 && (fabs(x[cf1]) > EPSILON)) {
      quantizer->state->flag2[0] = 1;
      quantizer->state->qinf[var] += lqu[var];
      quantizer->state->qsup[var] -= lqu[var];
    }
  }
  if (x[cf1] > 0) {
    nTime[var] = t + fabs((quantizer->state->qsup[var] - x[cf0]) / x[cf1]);
  } else if (x[cf1] < 0) {
    nTime[var] = t + fabs((quantizer->state->qinf[var] - x[cf0]) / x[cf1]);
  } else {
    nTime[var] = INF;
  }
}

void QSS_FUNC_DECL(CQSS, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                             double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(CQSS, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(CQSS, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  if (x[cf1] > 0) {
    nTime[var] = t + fabs((quantizer->state->qsup[var] - x[cf0]) / x[cf1]);
  } else if (x[cf1] < 0) {
    nTime[var] = t + fabs((quantizer->state->qinf[var] - x[cf0]) / x[cf1]);
  } else {
    nTime[var] = INF;
  }
}

void QSS_FUNC_DECL(CQSS, updateQuantizedState)(QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  quantizer->state->lt[var] = *(quantizer->state->simTime);
  double *qsup = quantizer->state->qsup;
  double *qinf = quantizer->state->qinf;
  if (x[cf1] > 0) {
    qsup[var] = qsup[var] + lqu[var];
    q[cf0] = qsup[var] - lqu[var] / 2;
    qinf[var] = qsup[var] - 2 * lqu[var];
  } else if (x[cf1] < 0) {
    qinf[var] = qinf[var] - lqu[var];
    q[cf0] = qinf[var] + lqu[var] / 2;
    qsup[var] = qinf[var] + 2 * lqu[var];
  }
  if (quantizer->state->flag2[0]) {
    q[cf0] = x[cf0];
    qsup[var] = q[var] + lqu[var];
    qinf[var] = q[var] - lqu[var];
    quantizer->state->flag2[0] = 0;
  }
}
