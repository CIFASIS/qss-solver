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

#include <common/data.h>
#include <common/macros.h>
#include <common/utils.h>
#include <qss/qss_data.h>

void QSS_FUNC_DECL(LIQSS, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int states = simData->states;
  int i;
  quantizer->state->oldDx = (double *)malloc(states * sizeof(double));
  quantizer->state->qAux = (double *)malloc(states * sizeof(double));
  quantizer->state->a = (double *)malloc(states * sizeof(double));
  quantizer->state->u0 = (double *)malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *)malloc(states * sizeof(double));
  quantizer->state->finTime = simData->ft;
  for (i = 0; i < states; i++) {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->flag2[i] = 0;
  }
  QSS_ASSIGN_QUANTIZER_OPS(LIQSS)
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSZ = simData->nSZ;
  quantizer->state->SZ = simData->SZ;
}

void QSS_FUNC_DECL(LIQSS, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  bool self = quantizer->state->lSimTime->minIndex == var && quantizer->state->lSimTime->type == ST_State;
  bool stateEvent = (quantizer->state->lSimTime->type == ST_Event) && (quantizer->state->nSZ[var] > 0);
  double diffQ;
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
  if (quantizer->state->flag2[var] == 1) {
    // the quantized state will be restarted at its current value to avoid that the zero crossing function changes its sign back
    nTime[var] = t;
  } else {
    double dt1 = 0;
    int sign = (x[cf1] > 0) ? 1 : ((x[cf1] < 0) ? -1 : 0);
    if (x[cf1] != 0) {
      if ((q[cf0] - x[cf0]) * x[cf1] >= 0) {
        // x moves towards q
        dt1 = (q[cf0] + 0.001 * sign * lqu[var] - x[cf0]) / x[cf1];
      } else {
        // x must go to q +- dq
        dt1 = fabs((q[cf0] - x[cf0] + 1.001 * sign * lqu[var]) / x[cf1]);
      }
      if (dt1 > quantizer->state->finTime - t) dt1 = quantizer->state->finTime - t;
      double err1 = q[cf0] - x[cf0] - dt1 * x[cf1];
      if (fabs(err1) < 1.1 * lqu[var]) {
        nTime[var] = t + dt1;
      } else {
        nTime[var] = t + quantizer->state->minStep;
      }
    } else {
      nTime[var] = INF;
    }
  }
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

void QSS_FUNC_DECL(LIQSS, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf1 = var * 2 + 1;
  if (x[cf1] == 0) {
    nTime[var] = INF;
  } else {
    nTime[var] = t + fabs(2 * lqu[var] / x[cf1]);
  }
}

void QSS_FUNC_DECL(LIQSS, updateQuantizedState)(QA_quantizer quantizer, int var, double *q, double *x, double *lqu)
{
  double t = quantizer->state->lSimTime->time;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  int cf0 = var * 2, cf1 = cf0 + 1;
  quantizer->state->qAux[var] = q[cf0];
  quantizer->state->oldDx[var] = x[cf1];
  if (quantizer->state->flag2[var] == 1 && t > 0) {
    // Restart quantized state from a nearby value in case there was an event.
    int signDx = (x[cf1] > 0) ? 1 : ((x[cf1] < 0) ? -1 : 0);
    q[cf0] = quantizer->state->qAux[var] + 1e-6 * lqu[var] * signDx;
    if (fabs(q[cf0] - x[cf0]) > 1.001 * lqu[var]) {
      x[cf0] = q[cf0];  // we reinit the state value at the quantized state.
    }
    quantizer->state->flag2[var] = 0;
  } else {
    if (fabs(a[var] * x[cf0] + u0[var]) < fabs(a[var] * lqu[var])) {
      q[cf0] = -u0[var] / a[var];
    } else {
      if (a[var] * x[cf0] + u0[var] > -a[var] * lqu[var]) {
        q[cf0] = x[cf0] + lqu[var];
      } else {
        q[cf0] = x[cf0] - lqu[var];
      }
    }
  }
}
