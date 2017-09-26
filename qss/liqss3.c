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

#include <qss/liqss3.h>

#include <math.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_quantizer.h"

#ifdef QSS_PARALLEL
void
LIQSS3_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
LIQSS3_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int states = simData->states;
  int i;
  quantizer->state->dq = (double *) malloc(states * sizeof(double));
  quantizer->state->oldDx = (double *) malloc(states * sizeof(double));
  quantizer->state->qAux = (double *) malloc(states * sizeof(double));
  quantizer->state->a = (double *) malloc(states * sizeof(double));
  quantizer->state->u0 = (double *) malloc(states * sizeof(double));
  quantizer->state->u1 = (double *) malloc(states * sizeof(double));
  quantizer->state->u2 = (double *) malloc(states * sizeof(double));
  quantizer->state->lt = (double *) malloc(states * sizeof(double));
  quantizer->state->ltq = (double *) malloc(states * sizeof(double));
  quantizer->state->lquOld = (double *) malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *) malloc(states * sizeof(int));
  quantizer->state->flag3 = (int *) malloc(states * sizeof(int));
  quantizer->state->flag4 = (int *) malloc(states * sizeof(int));
  quantizer->state->finTime = simData->ft;
  for(i = 0; i < states; i++)
  {
    int cf0 = i * 4;
    simData->x[cf0 + 2] = 0;
    simData->x[cf0 + 3] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    simData->q[cf0 + 2] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
    quantizer->state->u2[i] = 0;
    quantizer->state->dq[i] = 0;
    quantizer->state->lt[i] = simData->it;
    quantizer->state->ltq[i] = simData->it;
    quantizer->state->lquOld[i] = simData->lqu[i];
    quantizer->state->flag2[i] = 0; //this flag becomes true when a future situation dddx=0 is detected.
    quantizer->state->flag3[i] = 0; //this flag becomes true after trying to provoke dddx=0.
    quantizer->state->flag4[i] = 0; //this flag becomes true after detecting a sign change in ddx.
  }
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = LIQSS3_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS3_PAR_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS3_PAR_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS3_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = LIQSS3_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS3_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS3_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS3_updateQuantizedState;
#endif
}

#ifdef QSS_PARALLEL
void
LIQSS3_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS3_recomputeNextTime(QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#endif
{
  int cf0 = var * 4, cf1 = cf0 + 1, cf2 = cf1 + 1, cf3 = cf2 + 1;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  double *a = quantizer->state->a;
  double *dq = quantizer->state->dq;
  if(quantizer->state->ltq[var] == t)
  {
    double diffQ;
    diffQ = q[cf0] - quantizer->state->qAux[var];
    if(fabs(diffQ) > lqu[var] * 1e-6)
    {
      a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
      if(a[var] > 0)
      {
        a[var] = 0;
      }
    }
  }
  else
  {
    quantizer->state->flag3[var] = 0;
  }
  u0[var] = x[cf1] - q[cf0] * a[var];
  u1[var] = 2 * x[cf2] - q[cf1] * a[var];
  u2[var] = 3 * x[cf3] - q[cf2] * a[var];
  quantizer->state->lt[var] = t;
  if(quantizer->state->flag4[var])
  {
    nTime[var] = t;
  }
  else
  {
    double diffxq[4];
    double timeaux;
    diffxq[1] = q[cf1] - x[cf1];
    diffxq[2] = q[cf2] - x[cf2];
    diffxq[3] = -x[cf3];
    diffxq[0] = q[cf0] - dq[var] + lqu[var] - x[cf0];
    nTime[var] = t + minPosRoot(diffxq, 3);
    diffxq[0] = q[cf0] - dq[var] - lqu[var] - x[cf0];
    timeaux = t + minPosRoot(diffxq, 3);
    if(timeaux < nTime[var])
    {
      nTime[var] = timeaux;
    }
    else
    {
      diffxq[0] = q[cf0] - dq[var] + lqu[var] - x[cf0];
    }
    if(a[var] != 0 && (fabs(x[cf3]) > 1e-10) && !quantizer->state->flag3[var]
        && !quantizer->state->flag2[var])
    {
      double coeff[3];
      coeff[0] = a[var] * a[var] * a[var] * q[cf0] + a[var] * a[var] * u0[var]
          + a[var] * u1[var] + 2 * u2[var];
      coeff[1] = a[var] * a[var] * a[var] * q[cf1] + a[var] * a[var] * u1[var]
          + a[var] * 2 * u2[var];
      coeff[2] = a[var] * a[var] * a[var] * q[cf2] + a[var] * a[var] * u2[var];
      timeaux = t + minPosRoot(coeff, 2);
      if(timeaux < nTime[var])
      {
        quantizer->state->flag2[var] = 1;
        nTime[var] = timeaux;
        quantizer->state->lquOld[var] = lqu[var];
      }
    }
    else
    {
      quantizer->state->flag2[var] = 0;
    }
    if(nTime[var] > quantizer->state->finTime)
    {
      nTime[var] = quantizer->state->finTime;
    }
    double err1 = q[cf0] - x[cf0] + diffxq[1] * (nTime[var] - t) / 2
        + diffxq[2] * pow((nTime[var] - t) / 2, 2)
        + diffxq[3] * pow((nTime[var] - t) / 2, 3);

    if(fabs(err1) > 3 * fabs(lqu[var]))
    {
      nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep;
    }
  }
}

#ifdef QSS_PARALLEL
void
LIQSS3_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS3_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
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
    LIQSS3_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
LIQSS3_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#else
void
LIQSS3_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#endif
{
  int cf3 = var * 4 + 3;
  if(x[cf3] == 0)
  {
    nTime[var] = INF;
  }
  else
  {
    nTime[var] = t + pow(fabs(lqu[var] / x[cf3]), 1.0 / 3);
  }
}

#ifdef QSS_PARALLEL
void
LIQSS3_PAR_updateQuantizedState (QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu)
#else
void
LIQSS3_updateQuantizedState(QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu)
#endif
{
  int cf0 = var * 4, cf1 = cf0 + 1, cf2 = cf1 + 1, cf3 = cf2 + 1;
  double dx, elapsed;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *u2 = quantizer->state->u2;
  double *a = quantizer->state->a;
  double *dq = quantizer->state->dq;
  quantizer->state->flag3[var] = 0;
  elapsed = quantizer->state->lSimTime->time
      - quantizer->state->lSimTime->tq[var];
  quantizer->state->qAux[var] = q[cf0] + elapsed * q[cf1]
      + elapsed * elapsed * q[cf2];
  quantizer->state->oldDx[var] = x[cf1];
  quantizer->state->ltq[var] = quantizer->state->lSimTime->time;
  elapsed = quantizer->state->lSimTime->time - quantizer->state->lt[var];
  u0[var] = u0[var] + elapsed * u1[var] + elapsed * elapsed * u2[var];
  u1[var] = u1[var] + 2 * elapsed * u2[var];
  quantizer->state->lt[var] = quantizer->state->lSimTime->time;
  if(quantizer->state->flag2[var])
  {
    lqu[var] = quantizer->state->lquOld[var];
    quantizer->state->flag2[var] = 0;
    q[cf0] = quantizer->state->qAux[var];
  }
  else
  {
    q[cf0] = x[cf0];
  }
  if(quantizer->state->a[var] < -1e-30)
  {
    if(x[cf3] > 0)
    {
      dx = a[var] * a[var] * a[var] * (q[cf0] + lqu[var])
          + a[var] * a[var] * u0[var] + a[var] * u1[var] + 2 * u2[var];
      if(dx >= 0)
      {
        dq[var] = lqu[var];
      }
      else
      {
        dq[var] = -2 * u2[var] / a[var] / a[var] / a[var]
            - u1[var] / a[var] / a[var] - u0[var] / a[var] - q[cf0];
        quantizer->state->flag3[var] = 1;
        if(fabs(dq[var]) > lqu[var])
        {
          dq[var] = lqu[var];
        }
      }
    }
    else
    {
      dx = a[var] * a[var] * a[var] * (q[cf0] - lqu[var])
          + a[var] * a[var] * u0[var] + a[var] * u1[var] + 2 * u2[var];
      if(dx <= 0)
      {
        dq[var] = -lqu[var];
      }
      else
      {
        dq[var] = -2 * u2[var] / a[var] / a[var] / a[var]
            - u1[var] / a[var] / a[var] - u0[var] / a[var] - q[cf0];
        quantizer->state->flag3[var] = 1;
        if(fabs(dq[var]) > lqu[var])
        {
          dq[var] = -lqu[var];
        }
      }
    }
    if(q[cf1] * x[cf1] < 0 && !quantizer->state->flag4[var]
        && !quantizer->state->flag2[var] && !quantizer->state->flag3[var])
    {
      if(q[cf1] < 0)
      {
        dq[var] = quantizer->state->qAux[var] - q[cf0]
            - fabs(quantizer->state->lquOld[var]) * 0.1;
      }
      else
      {
        dq[var] = quantizer->state->qAux[var] - q[cf0]
            + fabs(quantizer->state->lquOld[var]) * 0.1;
      }
      quantizer->state->flag4[var] = 1;
    }
    else if(quantizer->state->flag4[var])
    {
      quantizer->state->flag4[var] = 0;
      if(fabs(
          -2 * u2[var] / a[var] / a[var] / a[var] - u1[var] / a[var] / a[var]
              - u0[var] / a[var] - q[cf0]) < 3 * lqu[var])
      {
        dq[var] = -2 * u2[var] / a[var] / a[var] / a[var]
            - u1[var] / a[var] / a[var] - u0[var] / a[var] - q[cf0];
        quantizer->state->flag3[var] = 1;
      }
    }
  }
  else
  {
    quantizer->state->flag4[var] = 0;
    if(x[cf3] > 0)
    {
      dq[var] = -lqu[var];
    }
    else
    {
      dq[var] = lqu[var];
    }
  }
  q[cf0] = q[cf0] + dq[var];
  if(quantizer->state->flag3[var])
  {
    q[cf1] = a[var] * q[cf0] + u0[var];
    q[cf2] = a[var] * q[cf1] / 2 + u1[var] / 2;
  }
  else
  {
    q[cf1] = x[cf1];
    q[cf2] = x[cf2];
  }
}
