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

#include <qss/qss.h>

#include <math.h>

#include "../common/utils.h"
#include "qss_data.h"
#include "qss_quantizer.h"

#ifdef QSS_PARALLEL
void
QSS_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
QSS_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int i;
  int states = simData->states;
  for(i = 0; i < states; i++)
  {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
  }
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = QSS_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = QSS_PAR_recomputeNextTime;
  quantizer->ops->nextTime = QSS_PAR_nextTime;
  quantizer->ops->updateQuantizedState = QSS_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = QSS_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = QSS_recomputeNextTime;
  quantizer->ops->nextTime = QSS_nextTime;
  quantizer->ops->updateQuantizedState = QSS_updateQuantizedState;
#endif
}

#ifdef QSS_PARALLEL
void
QSS_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q)
#else
void
QSS_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q)
#endif
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double coeff[2];
  coeff[0] = q[cf0] - x[cf0] - lqu[var];
  coeff[1] = -x[cf1];
  nTime[var] = t + minPosRoot(coeff, 1);
  coeff[0] = q[cf0] - x[cf0] + lqu[var];
  double timeaux = t + minPosRoot(coeff, 1);
  if(timeaux < nTime[var])
  {
    nTime[var] = timeaux;
  }
}

#ifdef QSS_PARALLEL
void
QSS_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
QSS_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
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
    QSS_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
QSS_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#else
void
QSS_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#endif
{
  int cf1 = var * 2 + 1;
  if(x[cf1])
  {
    nTime[var] = t + fabs(lqu[var] / x[cf1]);
  }
  else
  {
    nTime[var] = INF;
  }
}

#ifdef QSS_PARALLEL
void
QSS_PAR_updateQuantizedState (QA_quantizer quantizer, int i, double *q, double *x,
    double *lqu)
#else
void
QSS_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x,
    double *lqu)
#endif
{
  int cf0 = i * 2;
  q[cf0] = x[cf0];
}
