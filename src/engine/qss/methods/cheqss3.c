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

#include <qss/methods/cheqss3.h>

#include <math.h>
#include <stdlib.h>

#include <common/data.h>
#include <common/macros.h>
#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

void QSS_FUNC_DECL(CHEQSS3, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int states = simData->states;
  int i;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->a = (double *)malloc(states * sizeof(double));
  quantizer->state->u0 = (double *)malloc(states * sizeof(double));
  quantizer->state->u1 = (double *)malloc(states * sizeof(double));
  quantizer->state->u2 = (double *)malloc(states * sizeof(double));
  quantizer->state->tx = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(double));
  quantizer->state->finTime = simData->ft;
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
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
    quantizer->state->u2[i] = 0;
    quantizer->state->flag2[i] = 0;
  }
  QSS_ASSIGN_QUANTIZER_OPS(CHEQSS3)
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
}

void QSS_FUNC_DECL(CHEQSS3, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 4, cf1 = cf0 + 1, cf2 = cf1 + 1, cf3 = cf2 + 1;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  bool self = quantizer->state->lSimTime->minIndex == var && quantizer->state->lSimTime->type == ST_State;
  bool stateEvent = (quantizer->state->lSimTime->type == ST_Event) && (quantizer->state->nSZ[var] > 0) && 1;

  double diffQ;
  double coeff[4];
  double *tu = quantizer->state->tx;
  if (stateEvent) {
    // we check if the state event can be triggered by this state variable.
    for (int i = 0; i < quantizer->state->nSZ[var]; i++) {
      if (quantizer->state->SZ[var][i] == quantizer->state->lSimTime->minIndex) {
        quantizer->state->flag2[var] = 1;
      }
    }
  }
  if (self) {
    if (t > 0) {
      diffQ = q[cf0] - quantizer->state->qAux[var];
      if (diffQ) {
        a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
        if (a[var] > 0) a[var] = 0;
      }
    } else {
      a[var] = 0;
    }
  }

  u0[var] = x[cf1] - q[cf0] * a[var];
  u1[var] = 2 * x[cf2] - q[cf1] * a[var];
  u2[var] = 3 * x[cf3] - q[cf2] * a[var];
  tu[var] = t;
  if (quantizer->state->flag2[var] == 1) {
    // the quantized state will be restarted at its current value to avoid that the zero crossing function changes its sign back
    nTime[var] = t;
  } else {
    double timeaux, timeaux2;
    coeff[1] = q[cf1] - x[cf1];
    coeff[2] = q[cf2] - x[cf2];
    coeff[3] = -x[cf3];
    coeff[0] = q[cf0] + 1.001 * lqu[var] - x[cf0];
    timeaux = t + minPosRoot(coeff, 3);
    coeff[0] = q[cf0] - 1.001 * lqu[var] - x[cf0];
    timeaux2 = t + minPosRoot(coeff, 3);
    if (timeaux < timeaux2) {
      nTime[var] = timeaux;
    } else {
      nTime[var] = timeaux2;
    }
    // we verify next that the difference between x and q at the middle of the step is less than de quantum (within certain margin).
    double dt = (nTime[var] - t) / 2;
    if (dt > quantizer->state->finTime - t) dt = quantizer->state->finTime - t;
    double err1 = q[cf0] - x[cf0] + coeff[1] * dt + coeff[2] * dt * dt + coeff[3] * dt * dt * dt;
    if (fabs(err1) > 1.1 * lqu[var]) {
      nTime[var] = t + quantizer->state->minStep;
    }
  }
}

void QSS_FUNC_DECL(CHEQSS3, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                                double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(CHEQSS3, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(CHEQSS3, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf3 = var * 4 + 3;
  if (x[cf3] == 0) {
    nTime[var] = INF;
  } else {
    nTime[var] = t + pow(fabs(32 * lqu[var] / x[cf3]), 1.0 / 3);
  }
}

void QSS_FUNC_DECL(CHEQSS3, updateQuantizedState)(QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
{
  double t = quantizer->state->lSimTime->time;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  double *tu = quantizer->state->tx;
  int cf0 = var * 4, cf1 = cf0 + 1, cf2 = cf1 + 1, cf3 = cf2 + 1;
  double elapsed;
  elapsed = t - quantizer->state->lSimTime->tq[var];
  quantizer->state->qAux[var] = q[cf0] + elapsed * q[cf1] + elapsed * elapsed * q[cf2];
  quantizer->state->oldDx[var] = x[cf1];
  elapsed = t - tu[var];
  u0[var] = u0[var] + elapsed * u1[var] + elapsed * elapsed * u2[var];
  u1[var] = u1[var] + 2 * elapsed * u2[var];
  tu[var] = t;
  if (quantizer->state->flag2[var] == 1) {
    // Restart quantized state from a nearby value in case there was an event.
    quantizer->state->flag2[var] = 0;
    int signDx = (q[cf1] > 0) ? 1 : ((q[cf1] < 0) ? -1 : 0);
    q[cf0] = quantizer->state->qAux[var] + 1e-6 * lqu[var] * signDx;
    q[cf1] = 0;
    q[cf2] = 0;
    if (fabs(q[cf0] - x[cf0]) > 1.001 * lqu[var]) {
      x[cf0] = q[cf0];  // we reinit the state value at the quantized state.
    }
  } else {
    double disc = a[var] * a[var] * a[var] * x[cf0] + a[var] * a[var] * u0[var] + a[var] * u1[var] + 2 * u2[var];
    int sign = (disc > 0) ? 1 : ((disc < 0) ? -1 : 0);
    double coeff[4];
    double tm3 = 1e20;
    if (fabs(disc) < fabs(a[var] * a[var] * a[var]) * lqu[var]) {
      q[cf2] = (-u2[var] / a[var]);
      q[cf1] = (2 * q[cf2] - u1[var]) / a[var];
      q[cf0] = (q[cf1] - u0[var]) / a[var];
    } else {
      if (a[var] == 0) {
        if (disc) tm3 = pow(192 * lqu[var] / fabs(disc), 1.0 / 3);
      } else {
        coeff[0] = -192;
        coeff[1] = 96 * a[var];
        coeff[2] = -18 * a[var] * a[var];
        coeff[3] = fabs(disc) / lqu[var] + a[var] * a[var] * a[var];
        tm3 = minPosRoot(coeff, 3);
      }
      q[cf0] = x[cf0] + sign * lqu[var];
      q[cf1] = a[var] * q[cf0] + u0[var] - 18 / tm3 * sign * lqu[var];
      q[cf2] = (a[var] * q[cf1] + u1[var] + 96 / (tm3 * tm3) * sign * lqu[var]) / 2;
    }
  }
}
