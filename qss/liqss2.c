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
#include <qss/liqss2.h>
#include <math.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_quantizer.h"

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
LIQSS2_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int states = simData->states;
  int i;
  quantizer->state->oldDx = (double *) malloc(states * sizeof(double));
  quantizer->state->qAux = (double *) malloc(states * sizeof(double));
  quantizer->state->a = (double *) malloc(states * sizeof(double));
  quantizer->state->u0 = (double *) malloc(states * sizeof(double));
  quantizer->state->u1 = (double *) malloc(states * sizeof(double));
  quantizer->state->tx = (double *) malloc(states * sizeof(double));
  quantizer->state->flag2 = (int *) malloc(states * sizeof(double));
  quantizer->state->finTime = simData->ft;

  for(i = 0; i < states; i++)
  {
    int cf0 = i * 3;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    quantizer->state->qAux[i] = simData->x[cf0];
    quantizer->state->oldDx[i] = 0;
    quantizer->state->tx[i] = 0;
    quantizer->state->a[i] = 0;
    quantizer->state->u0[i] = 0;
    quantizer->state->u1[i] = 0;
    quantizer->state->flag2[i] = 0;
  }
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
#endif
  quantizer->state->minStep = simData->params->minStep;
  quantizer->state->lSimTime = simTime;
  quantizer->state->nSD = simData->nSZ;
  quantizer->state->SD = simData->SZ;
  quantizer->ops->recomputeNextTimes = LIQSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = LIQSS2_recomputeNextTime;
  quantizer->ops->nextTime = LIQSS2_nextTime;
  quantizer->ops->updateQuantizedState = LIQSS2_updateQuantizedState;
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q)
#else
void
LIQSS2_recomputeNextTime(QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#endif
{
  int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
  double diffxq[3];
  double dt1, nT_prev;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  bool self = quantizer->state->lSimTime->minIndex == var
      && quantizer->state->lSimTime->type == ST_State;
  double diffQ, timeaux;
  double coeff[3];
  double *tu = quantizer->state->tx;
  double aold = a[var];
  int *flag2 = quantizer->state->flag2;

  if(self)
  {
    if(t > 0)
    {
      diffQ = q[cf0] - quantizer->state->qAux[var];
      if(diffQ)
        a[var] = (x[cf1] - quantizer->state->oldDx[var]) / diffQ;
      if(a[var] > 0)
        a[var] = 0;
    }
    else
    {
      a[var] = 0;
    }
  }
  u0[var] = x[cf1] - q[cf0] * a[var];
  u1[var] = 2 * x[cf2] - q[cf1] * a[var];
  tu[var] = t;

  coeff[1] = q[cf1] - x[cf1];
  coeff[2] = -x[cf2];
  if(flag2[var] != 1)
  {
    if((quantizer->state->lSimTime->type == ST_Event) && (a[var] < 0)
        && (quantizer->state->nSD[var] > 0))
    { //we check if var is involved in the zero crossing function that produced the current event
      for(int i = 0; i < quantizer->state->nSD[var]; i++)
      {
        if(quantizer->state->SD[var][i] == quantizer->state->lSimTime->minIndex)
        {
          nTime[var] = t;
          flag2[var] = 1; //it does, so we restart the quantized state q[var]
        }
      }
    }
    if(flag2[var] != 1)
    {
      if(a[var] < 0 && self && x[cf2] != 0)
      {
        nTime[var] = t + fabs((q[cf1] - x[cf1]) / x[cf2]);
      }
      else
      {
        nTime[var] = INF;
      }
      coeff[0] = q[cf0] + 2 * lqu[var] - x[cf0];
      timeaux = t + minPosRoot(coeff, 2);
      if(timeaux < nTime[var])
      {
        nTime[var] = timeaux;
      }
      coeff[0] = q[cf0] - 2 * lqu[var] - x[cf0];
      timeaux = t + minPosRoot(coeff, 2);
      if(timeaux < nTime[var])
      {
        nTime[var] = timeaux;
      }

      if(q[cf0] * q[cf1] < 0 && fabs(q[cf0]) > 10 * lqu[var])
      {
        timeaux = -q[cf0] / q[cf1] - 2 * fabs(lqu[var] / q[cf1]);
        if(nTime[var] > t + timeaux)
          nTime[var] = t + timeaux;
      }
      if(flag2[var] == 2 && self)
        flag2[var] = 0;
      double err1 = q[cf0] - x[cf0] + coeff[1] * (nTime[var] - t) / 2
          + coeff[2] * pow((nTime[var] - t) / 2, 2);
      if(fabs(err1) > 3 * fabs(lqu[var]))
        nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep;
    }
  }
  else
  {
    // if (fabs(aold-a[var])>fabs(a[var])/2) nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep*rand()/RAND_MAX;;
//      nTime[var] = t + quantizer->state->finTime * quantizer->state->minStep*0;
    if(self)
    {
      flag2[var] = 2;
      nTime[var] = t;
    }
  }
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
LIQSS2_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
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
    if (map[inf[i]] != NOT_ASSIGNED)
    {
#endif
    LIQSS2_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#else
void
LIQSS2_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#endif
{
  int cf2 = var * 3 + 2;
  if(x[cf2] == 0)
    nTime[var] = INF;
  else
    nTime[var] = t + sqrt(fabs(lqu[var] / x[cf2]));
}

#ifdef QSS_PARALLEL
void
LIQSS2_PAR_updateQuantizedState (QA_quantizer quantizer, int var, double *q, double *x,
    double *lqu)
#else
void
LIQSS2_updateQuantizedState(QA_quantizer quantizer, int var, double *q,
    double *x, double *lqu)
#endif
{
  double t = quantizer->state->lSimTime->time;
  double *a = quantizer->state->a;
  double *u0 = quantizer->state->u0;
  double *u1 = quantizer->state->u1;
  double *tu = quantizer->state->tx;
  int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
  double elapsed;
  double h;
  int *flag2 = quantizer->state->flag2;

  elapsed = t - quantizer->state->lSimTime->tq[var];
  quantizer->state->qAux[var] = q[cf0] + elapsed * q[cf1];
  quantizer->state->oldDx[var] = x[cf1];
  elapsed = t - tu[var];
  u0[var] = u0[var] + elapsed * u1[var];
  tu[var] = t;
  double ddx = x[cf2] * 2;
  double oldq1 = q[cf1];

  if(a[var] < 0)
  {
    if(ddx == 0)
    {
      ddx = a[var] * a[var] * q[cf0] + a[var] * u0[var] + u1[var];
      if(ddx == 0)
        ddx = 1e-40;
    }

    h = (quantizer->state->finTime - t);
    q[cf0] = ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var])
        + (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var]))
        / (1 - h * a[var] + h * h * a[var] * a[var] / 2);
    if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
    {
      h = sqrt(fabs(2 * lqu[var] / ddx));
      q[cf0] = ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var])
          + (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var]))
          / (1 - h * a[var] + h * h * a[var] * a[var] / 2);
    }
    while(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
    {
      h = h * sqrt(lqu[var] / fabs(q[cf0] - x[cf0]));
      q[cf0] = ((x[cf0] + h * u0[var] + h * h / 2 * u1[var]) * (1 - h * a[var])
          + (h * h / 2 * a[var] - h) * (u0[var] + h * u1[var]))
          / (1 - h * a[var] + h * h * a[var] * a[var] / 2);
    }
    q[cf1] = (a[var] * q[cf0] + u0[var] + h * u1[var]) / (1 - h * a[var]);

    if(x[cf2] * ddx < 0)	// no hace falta
    {
      q[cf1] = -u1[var] / a[var];
      q[cf0] = (q[cf1] - u0[var]) / a[var];
      if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
      {
        if(q[cf0] > x[cf0])
          q[cf0] = x[cf0] + lqu[var];
        else
          q[cf0] = x[cf0] - lqu[var];
      }
    }
  }
  else
  {
    ddx = u1[var];
    if(ddx > 0)
      q[cf0] = x[cf0] - lqu[var];
    else
      q[cf0] = x[cf0] + lqu[var];
    if(ddx != 0)
    {
      h = sqrt(2 * lqu[var] / fabs(ddx));
      q[cf1] = u0[var] + h * u1[var];
    }
    else
    {
      q[cf1] = u0[var];
    }
  }
  if(fabs(q[cf0] - x[cf0]) > 2 * lqu[var])
    q[cf0] = x[cf0];
  if(flag2[var] >= 1)
  {
    if(oldq1 > 0)
      q[cf0] = quantizer->state->qAux[var] + lqu[var] / 100;
    else
      q[cf0] = quantizer->state->qAux[var] - lqu[var] / 100;
    q[cf1] = 0;

  }

}
