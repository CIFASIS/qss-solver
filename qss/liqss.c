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

#include <qss/liqss.h>

#include <math.h>
#include <stdlib.h>

#include "../common/utils.h"
#include "qss_data.h"
#include "qss_quantizer.h"

#ifdef QSS_PARALLEL
void
LIQSS_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
LIQSS_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int i;
  int states = simData->states;
  quantizer->state->dq = (double *) malloc(states * sizeof(double));
  quantizer->state->oldDx = (double *) malloc(states * sizeof(double));
  quantizer->state->qAux = (double *) malloc(states * sizeof(double));
  quantizer->state->a = (double *) malloc(states * sizeof(double));
  quantizer->state->u0 = (double *) malloc(states * sizeof(double));
  quantizer->state->lt = (double *) malloc(states * sizeof(double));
  for(i = 0; i < states; i++)
  {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->dq[i] = 0;
    quantizer->state->lt[i] = simData->it;
  }
  quantizer->state->simTime = &simTime->time;
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = LIQSS_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS_PAR_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS_PAR_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = LIQSS_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS_updateQuantizedState;
#endif
}

#ifdef QSS_PARALLEL
void
LIQSS_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#endif
{
  int i;
#ifdef QSS_PARALLEL
  int *map = quantizer->state->qMap;
#endif
  for(i = 0; i < vars; i++)
  {
#ifdef QSS_PARALLEL
    if (map[inf[i]] > NOT_ASSIGNED)
    {
#endif
    LIQSS_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
LIQSS_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS_recomputeNextTime(QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#endif
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double *u = quantizer->state->u0;
  double *a = quantizer->state->a;
  if(quantizer->state->lt[var] == t)
  {
    double diffQ;
    diffQ = q[cf0] - quantizer->state->qAux[var];
    if(diffQ)
    {
      a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
    }
    else
    {
      a[var] = 0;
    }
  }
  u[var] = x[cf1] - q[cf0] * a[var];
  double dt1 = 0;
  if(x[cf1] != 0)
  {
    dt1 = (q[cf0] - x[cf0]) / x[cf1];
    if(dt1 > 0)
    {
      nTime[var] = t + dt1;
    }
    else
    {
      if(x[cf1] > 0)
      {
        nTime[var] = t + (q[cf0] + 2 * lqu[var] - x[cf0]) / x[cf1];
      }
      else
      {
        nTime[var] = t + (q[cf0] - 2 * lqu[var] - x[cf0]) / x[cf1];
      }
      if(nTime[var] < t)
      {
        nTime[var] = t + MIN_STEP;
      }
    }
  }
  else
  {
    nTime[var] = INF;
  }
}

#ifdef QSS_PARALLEL
void
LIQSS_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#else
void
LIQSS_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#endif
{
  int cf1 = var * 2 + 1;
  if(x[cf1] == 0)
  {
    nTime[var] = INF;
  }
  else
  {
    nTime[var] = t + fabs(lqu[var] / x[cf1]);
  }
}

#ifdef QSS_PARALLEL
void
LIQSS_PAR_updateQuantizedState (QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu)
#else
void
LIQSS_updateQuantizedState(QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu)
#endif
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double dx;
  double *u = quantizer->state->u0;
  double *a = quantizer->state->a;
  double *dq = quantizer->state->dq;
  quantizer->state->qAux[var] = q[cf0];
  quantizer->state->oldDx[var] = x[cf1];
  quantizer->state->lt[var] = *(quantizer->state->simTime);
  q[cf0] = x[cf0];
  if(x[cf1] > 0)
  {
    dx = u[var] + (q[cf0] + lqu[var]) * a[var];
    if(dx >= 0)
    {
      dq[var] = lqu[var];
    }
    else if(dx < 0)
    {
      dq[var] = -u[var] / a[var] - q[cf0];
    }
  }
  else
  {
    dx = u[var] + (q[cf0] - lqu[var]) * a[var];
    if(dx <= 0)
    {
      dq[var] = -lqu[var];
    }
    else if(dx > 0)
    {
      dq[var] = -u[var] / a[var] - q[cf0];
    }
  }
  if(dq[var] > 2 * lqu[var])
    dq[var] = 2 * lqu[var];
  if(dq[var] < -2 * lqu[var])
    dq[var] = -2 * lqu[var];
  q[cf0] += dq[var];
}
