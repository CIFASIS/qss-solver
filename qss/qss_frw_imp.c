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

#include <qss/qss_frw_imp.h>

#include <math.h>
#include <stdio.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_frw.h"

/* First order */

#ifdef QSS_PARALLEL
void
FO_PAR_nextEventTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
FO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  int s;
  double coeff[5], t = simTime->time;
  double new_zc;
  double *q = simData->q;
  if(simData->nZS[index] > 0)
  {
    simModel->events->zeroCrossing(index, q, simData->d, simData->alg, t,
        &coeff[0]);
    s = sign(coeff[0]);
    if(simData->event[index].zcSign != s)
    {
      simTime->nextEventTime[index] = t;
    }
    else
    {
      simTime->nextEventTime[index] = INF;
    }
  }
  else
  {
    simModel->events->zeroCrossing(index, q, simData->d, simData->alg, t,
        &coeff[0]);
    s = sign(coeff[0]);
    if(simData->event[index].zcSign != s)
    {
      simTime->nextEventTime[index] = t;
      simData->event[index].zcSign = s;
    }
    else
    {
      simModel->events->zeroCrossing(index, q, simData->d, simData->alg,
          t + f->state->delta, &new_zc);
      coeff[1] = (new_zc - coeff[0]) / f->state->delta;
      coeff[0] += simData->event[index].zcSign * simData->event[index].zcHyst;
      simTime->nextEventTime[index] = t + minPosRoot(coeff, 1);
    }
  }
}

#ifdef QSS_PARALLEL
void
FO_PAR_nextInputTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#else
void
FO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#endif
{
  double df;
  double *x = simData->x;
  if(elapsed > 0)
  {
    df = (x[var * 2 + 1] - f->state->dxnOld) / elapsed;
  }
  else
  {
    df = simData->lqu[var] * 1e6;
  }
  if(df != 0)
  {
    simTime->nextInputTime[index] = simTime->time
        + sqrt(simData->lqu[var] / fabs(df));
  }
  else
  {
    simTime->nextInputTime[index] = INF;
  }
}

#ifdef QSS_PARALLEL
void
FO_PAR_recomputeDerivatives (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
FO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg, simTime->time, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg, simTime->time, x, NULL);
#endif
}

#ifdef QSS_PARALLEL
void
FO_PAR_recomputeDerivative (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int stateVar)
#else
void
FO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int stateVar)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int cf0 = stateVar * 2;
  f->state->dxnOld = x[cf0 + 1];
  simModel->f(stateVar, q, simData->d, simData->alg, simTime->time, &(x[cf0]));
}

/* Second order */

#ifdef QSS_PARALLEL
void
SO_PAR_nextEventTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double tmp1[simData->maxRHS];
  int i, j, s;
  double coeff[5], t = simTime->time;
  double dNew;
  int nZS = simData->nZS[index];
  for(i = 0; i < nZS; i++)
  {
    j = simData->ZS[index][i];
    tmp1[i] = q[j * 3];
    q[j * 3] = evaluatePoly(j * 3, t - simTime->tq[j], q, 1);
  }
  simModel->events->zeroCrossing(index, q, simData->d, simData->alg, t,
      &coeff[0]);
  for(i = 0; i < nZS; i++)
  {
    j = simData->ZS[index][i];
    q[j * 3] = tmp1[i];
  }
  s = sign(coeff[0]);
  if(simData->event[index].zcSign != s)
  {
    simTime->nextEventTime[index] = t;
  }
  else
  {
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      tmp1[i] = q[j * 3];
      q[j * 3] = evaluatePoly(j * 3, t - simTime->tq[j] + f->state->delta, q,
          1);
    }
    simModel->events->zeroCrossing(index, q, simData->d, simData->alg,
        t + f->state->delta, &dNew);
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      q[j * 3] = tmp1[i];
    }
    coeff[1] = (dNew - coeff[0]) / (f->state->delta);
    coeff[0] += simData->event[index].zcSign * simData->event[index].zcHyst;
    simTime->nextEventTime[index] = t + minPosRoot(coeff, 1);
  }
}

#ifdef QSS_PARALLEL
void
SO_PAR_nextInputTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#else
void
SO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#endif
{
  double *x = simData->x;
  double df;
  if(elapsed > 0)
  {
    df = (x[var * 3 + 2] - f->state->dxnOld) / elapsed / 2;
  }
  else
  {
    df = 2 * simData->lqu[var] * 1e12;
  }
  if(df != 0)
  {
    simTime->nextInputTime[index] = simTime->time
        + pow(simData->lqu[var] / fabs(df), 1.0 / 3);
  }
  else
  {
    simTime->nextInputTime[index] = INF;
  }
}

#ifdef QSS_PARALLEL
void
SO_PAR_recomputeDerivatives (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int nSD = simData->nSD[index];
  int size = simData->maxRHS * nSD;
  double tmp1[size], tmp2[nSD], tmp3[nSD];
  int k, j, i, w, it;
  double e;
  for(i = 0, it = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    e = simTime->time - simTime->tq[w];
    if(e > 0)
    {
      integrateState(w * 3, e, q, 1);
      simTime->tq[w] = simTime->time;
    }
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++, it++)
    {
      j = simData->DS[w][k];
      e = simTime->time - simTime->tq[j];
      int infCf0 = j * 3;
      if(e > 0)
      {
        integrateState(infCf0, e, q, 1);
      }
      simTime->tq[j] = simTime->time;
    }
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg, simTime->time, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg, simTime->time, x, NULL);
#endif
  for(i = 0, it = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++, it++)
    {
      j = simData->DS[w][k];
      int infCf0 = j * 3;
      if(simTime->tq[j] == simTime->time)
      {
        tmp1[it] = q[infCf0];
        q[infCf0] = evaluatePoly(infCf0, f->state->delta, q, 1);
        simTime->tq[j] = simTime->time + f->state->delta;
      }
    }
    tmp2[i] = x[w * 3 + 1];
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg, simTime->time+ f->state->delta, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg,
      simTime->time + f->state->delta, x, NULL);
#endif
  for(i = 0, it = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++, it++)
    {
      j = simData->DS[w][k];
      if(simTime->tq[j] == simTime->time + f->state->delta)
      {
        int infCf0 = j * 3;
        q[infCf0] = tmp1[it];
        simTime->tq[j] = simTime->time;
      }
    }
    tmp3[i] = x[w * 3 + 1];
    x[w * 3 + 1] = tmp2[i];
#ifdef QSS_PARALLEL
  }
#endif
  }
  for(k = 0; k < nSD; k++)
  {
    j = simData->SD[index][k];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[j] > NOT_ASSIGNED)
    {
#endif
    if(f->state->delta > 0)
    {
      int cf0 = j * 3;
      x[cf0 + 2] = (tmp3[k] - x[cf0 + 1]) / (f->state->delta * 2);
    }
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
SO_PAR_recomputeDerivative (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int nDS = simData->nDS[index];
  double tmp1[simData->maxRHS], tmp2, tmp3;
  int cf0 = index * 3;
  int k, j;
  double e;
  e = simTime->time - simTime->tq[index];
  if(e > 0)
  {
    integrateState(index * 3, e, q, 1);
    simTime->tq[index] = simTime->time;
  }
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    e = simTime->time - simTime->tq[j];
    int infCf0 = j * 3;
    if(e > 0)
    {
      integrateState(infCf0, e, q, 1);
    }
    simTime->tq[j] = simTime->time;
  }
  simModel->f(index, q, simData->d, simData->alg, simTime->time, &(x[cf0]));
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    int infCf0 = j * 3;
    tmp1[k] = q[infCf0];
    q[infCf0] = evaluatePoly(infCf0, f->state->delta, q, 1);
  }
  tmp2 = x[cf0 + 1];
  simModel->f(index, q, simData->d, simData->alg,
      simTime->time + f->state->delta, &(x[cf0]));
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    int infCf0 = j * 3;
    q[infCf0] = tmp1[k];
  }
  tmp3 = x[cf0 + 1];
  x[cf0 + 1] = tmp2;
  f->state->dxnOld = x[cf0 + 2];
  if(f->state->delta > 0)
  {
    x[cf0 + 2] = (tmp3 - x[cf0 + 1]) / (f->state->delta * 2);
  }
}

/* Third order . */

#ifdef QSS_PARALLEL
void
TO_PAR_nextEventTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
TO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double tmp1[simData->maxRHS];
  int i, j, s;
  double coeff[5], t = simTime->time;
  double dNew, dOld;
  int nZS = simData->nZS[index];
  for(i = 0; i < nZS; i++)
  {
    j = simData->ZS[index][i];
    tmp1[i] = q[j * 4];
    q[j * 4] = evaluatePoly(j * 4, t - simTime->tq[j], q, 2);
  }
  simModel->events->zeroCrossing(index, q, simData->d, simData->alg, t,
      &coeff[0]);
  for(i = 0; i < nZS; i++)
  {
    j = simData->ZS[index][i];
    q[j * 4] = tmp1[i];
  }
  s = sign(coeff[0]);
  if(simData->event[index].zcSign != s)
  {
    simTime->nextEventTime[index] = t;
  }
  else
  {
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      tmp1[i] = q[j * 4];
      q[j * 4] = evaluatePoly(j * 4, t - simTime->tq[j] + f->state->delta, q,
          2);
    }
    simModel->events->zeroCrossing(index, q, simData->d, simData->alg,
        t + f->state->delta, &dNew);
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      q[j * 4] = tmp1[i];
    }
    coeff[1] = (dNew - coeff[0]) / (f->state->delta);
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      tmp1[i] = q[j * 4];
      q[j * 4] = evaluatePoly(j * 4, t - simTime->tq[j] - f->state->delta, q,
          2);
    }
    simModel->events->zeroCrossing(index, q, simData->d, simData->alg,
        t - f->state->delta, &dOld);
    for(i = 0; i < nZS; i++)
    {
      j = simData->ZS[index][i];
      q[j * 4] = tmp1[i];
    }
    coeff[2] = (dNew - 2 * coeff[0] + dOld)
        / (f->state->delta * f->state->delta * 2);
    coeff[0] += simData->event[index].zcSign * simData->event[index].zcHyst;
    simTime->nextEventTime[index] = t + minPosRoot(coeff, 2);
  }
}

#ifdef QSS_PARALLEL
void
TO_PAR_nextInputTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#else
void
TO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index)
#endif
{
  double *x = simData->x;
  double df;
  if(elapsed > 0)
  {
    df = (x[var * 4 + 3] - f->state->dxnOld) / elapsed / 3;
  }
  else
  {
    df = 6 * simData->lqu[var] * 1e18;
  }
  if(df != 0)
  {
    simTime->nextInputTime[index] = simTime->time
        + pow(simData->lqu[var] / fabs(df), 1.0 / 4);
  }
  else
  {
    simTime->nextInputTime[index] = INF;
  }
}

#ifdef QSS_PARALLEL
void
TO_PAR_recomputeDerivatives (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
TO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int nSD = simData->nSD[index];
  int size = nSD * simData->maxRHS;
  double tmp1[size], tmp2[nSD], tmp3[nSD], tmp4[nSD];
  int k, i, w, it, j;
  double e;
  for(i = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    e = simTime->time - simTime->tq[w];
    if(e > 0)
    {
      integrateState(w * 4, e, q, 2);
      simTime->tq[w] = simTime->time;
    }
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++)
    {
      j = simData->DS[w][k];
      e = simTime->time - simTime->tq[j];
      if(e > 0)
      {
        integrateState(j * 4, e, q, 2);
      }
      simTime->tq[j] = simTime->time;
    }
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg, simTime->time, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg, simTime->time, x, NULL);
#endif
  for(i = 0, it = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++, it++)
    {
      j = simData->DS[w][k];
      if(simTime->tq[j] == simTime->time)
      {
        tmp1[it] = q[j * 4];
        q[j * 4] = evaluatePoly(j * 4, f->state->delta, q, 2);
        simTime->tq[j] = simTime->time + f->state->delta;
      }
    }
    tmp3[i] = x[w * 4 + 1];
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg,
      simTime->time + f->state->delta, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg,
      simTime->time + f->state->delta, x, NULL);
#endif
  for(i = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++)
    {
      j = simData->DS[w][k];
      q[j * 4] = evaluatePoly(j * 4, -f->state->delta, q, 2);
    }
    tmp4[i] = x[w * 4 + 1];
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg,
      simTime->time - f->state->delta, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg,
      simTime->time - f->state->delta, x, NULL);
#endif
  for(i = 0, it = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++, it++)
    {
      j = simData->DS[w][k];
      if(simTime->tq[j] == simTime->time + f->state->delta)
      {
        q[j * 4] = tmp1[it];
        simTime->tq[j] = simTime->time;
      }
    }
    tmp2[i] = x[w * 4 + 1];
    x[w * 4 + 1] = tmp3[i];
#ifdef QSS_PARALLEL
  }
#endif
  }
  for(k = 0; k < nSD; k++)
  {
    j = simData->SD[index][k];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[j] > NOT_ASSIGNED)
    {
#endif
    if(f->state->delta > 0)
    {
      int cf0 = j * 4;
      x[cf0 + 2] = (tmp4[k] - tmp2[k]) / (f->state->delta * 4);
      x[cf0 + 3] = (tmp4[k] - 2 * x[cf0 + 1] + tmp2[k])
          / (f->state->delta * f->state->delta * 6);
    }
    //     printf("Variable %d influeciada %d %g %g %g\n",index,j,x[j*4+1],x[j*4+2],x[j*4+3]);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
TO_PAR_recomputeDerivative (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
TO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int nDS = simData->nDS[index];
  double tmp1[simData->maxRHS], tmp2, tmp3, tmp4;
  int k, j;
  double e;
  e = simTime->time - simTime->tq[index];
  if(e > 0)
  {
    integrateState(index * 4, e, q, 2);
    simTime->tq[index] = simTime->time;
  }
  int cf0 = index * 4;
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    e = simTime->time - simTime->tq[j];
    if(e > 0)
    {
      integrateState(j * 4, e, q, 2);
    }
    simTime->tq[j] = simTime->time;
  }
  simModel->f(index, q, simData->d, simData->alg, simTime->time, &(x[cf0]));
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    tmp1[k] = q[j * 4];
    q[j * 4] = evaluatePoly(j * 4, f->state->delta, q, 2);
  }
  tmp2 = x[cf0 + 1];
  simModel->f(index, q, simData->d, simData->alg,
      simTime->time + f->state->delta, &(x[cf0]));
  tmp3 = x[cf0 + 1];
  if(f->state->delta > 0)
  {
    x[cf0 + 2] = (tmp3 - tmp2) / (f->state->delta * 2);
  }
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    q[j * 4] = evaluatePoly(j * 4, -(f->state->delta), q, 2);
  }
  simModel->f(index, q, simData->d, simData->alg,
      simTime->time - f->state->delta, &(x[cf0]));
  tmp4 = x[cf0 + 1];
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    q[j * 4] = tmp1[k];
  }
  x[cf0 + 1] = tmp2;
  f->state->dxnOld = x[cf0 + 3];
  if(f->state->delta > 0)
  {
    x[cf0 + 3] = (tmp3 - 2 * x[cf0 + 1] + tmp4)
        / (f->state->delta * f->state->delta * 6);
  }
}

/* Symbolic derivatives. */

#ifdef QSS_PARALLEL
void
SYM_PAR_recomputeDerivative (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SYM_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int k, j;
  double e;
  int derOrder = simData->order;
  int qOrder = derOrder - 1;
  int coeffs = derOrder + 1;
  int cf0 = index * coeffs;
  e = simTime->time - simTime->tq[index];
  if(e > 0)
  {
    integrateState(cf0, e, q, qOrder);
    simTime->tq[index] = simTime->time;
  }
  int nDS = simData->nDS[index];
  for(k = 0; k < nDS; k++)
  {
    j = simData->DS[index][k];
    e = simTime->time - simTime->tq[j];
    if(e > 0)
    {
      integrateState(j * coeffs, e, q, qOrder);
    }
    simTime->tq[j] = simTime->time;
  }
  f->state->dxnOld = x[cf0 + derOrder];
  simModel->f(index, q, simData->d, simData->alg, simTime->time, &(x[cf0]));
}

#ifdef QSS_PARALLEL
void
SYM_PAR_recomputeDerivatives (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SYM_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double *x = simData->x;
  int k, j, i, w;
  double e;
  int nSD = simData->nSD[index];
  int order = simData->order - 1;
  int coeffs = order + 2;
  for(i = 0; i < nSD; i++)
  {
    w = simData->SD[index][i];
#ifdef QSS_PARALLEL
    if (simData->lp->qMap[w] > NOT_ASSIGNED)
    {
#endif
    e = simTime->time - simTime->tq[w];
    if(e > 0)
    {
      integrateState(w * coeffs, e, q, order);
      simTime->tq[w] = simTime->time;
    }
    int nDS = simData->nDS[w];
    for(k = 0; k < nDS; k++)
    {
      j = simData->DS[w][k];
      e = simTime->time - simTime->tq[j];
      if(e > 0)
      {
        integrateState(j * coeffs, e, q, order);
      }
      simTime->tq[j] = simTime->time;
    }
#ifdef QSS_PARALLEL
  }
#endif
  }
#ifdef QSS_PARALLEL
  simModel->deps (index, q, simData->d, simData->alg, simTime->time, x, simData->lp->qMap);
#else
  simModel->deps(index, q, simData->d, simData->alg, simTime->time, x, NULL);
#endif
}

#ifdef QSS_PARALLEL
void
SYM_PAR_nextEventTime (FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#else
void
SYM_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index)
#endif
{
  double *q = simData->q;
  double coeff[4], e = 0;
  int i, j, s;
  int order = simData->order - 1;
  int coeffs = order + 2;
  int nZS = simData->nZS[index];
  for(i = 0; i < nZS; i++)
  {
    j = simData->ZS[index][i];
    e = simTime->time - simTime->tq[j];
    if(e > 0)
    {
      integrateState(j * coeffs, e, q, order);
    }
    simTime->tq[j] = simTime->time;
  }
  simModel->events->zeroCrossing(index, q, simData->d, simData->alg,
      simTime->time, coeff);
  s = sign(coeff[0]);
  if(simData->event[index].zcSign != s)
  {
    simTime->nextEventTime[index] = simTime->time;
  }
  else
  {
    coeff[0] += simData->event[index].zcSign * simData->event[index].zcHyst;
    simTime->nextEventTime[index] = simTime->time + minPosRoot(coeff, order);
  }
}
