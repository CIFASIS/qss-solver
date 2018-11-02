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

#include <qss/qss2.h>

#include <math.h>

#include "../common/utils.h"
#include "qss_data.h"
#include "qss_quantizer.h"

#ifdef QSS_PARALLEL
void
QSS2_PAR_init (QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#else
void
QSS2_init(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
#endif
{
  int i;
  int states = simData->states;
  for(i = 0; i < states; i++)
  {
    int cf0 = i * 3;
    simData->x[cf0 + 2] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
  }
#ifdef QSS_PARALLEL
  quantizer->state->qMap = simData->lp->qMap;
  quantizer->ops->recomputeNextTimes = QSS2_PAR_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = QSS2_PAR_recomputeNextTime;
  quantizer->ops->nextTime = QSS2_PAR_nextTime;
  quantizer->ops->updateQuantizedState = QSS2_PAR_updateQuantizedState;
#else
  quantizer->ops->recomputeNextTimes = QSS2_recomputeNextTimes;
  quantizer->ops->recomputeNextTime = QSS2_recomputeNextTime;
  quantizer->ops->nextTime = QSS2_nextTime;
  quantizer->ops->updateQuantizedState = QSS2_updateQuantizedState;
#endif
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_nextTime (QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#else
void
QSS2_nextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu)
#endif
{
  int cf2 = var * 3 + 2;
  if(x[cf2])
  {
    nTime[var] = t + sqrt(lqu[var] / fabs(x[cf2]));
  }
  else
  {
    nTime[var] = INF;
  }
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_recomputeNextTime (QA_quantizer quantizer, int var, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
QSS2_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q)
#endif
{
  int cf0 = var * 3, cf1 = cf0 + 1, cf2 = cf1 + 1;
  double coeff[3];
  if(fabs(q[cf0] - x[cf0]) >= lqu[var] * 0.999999999)
  {
    nTime[var] = t;
  }
  else
  {
    coeff[0] = q[cf0] + lqu[var] - x[cf0];
    coeff[1] = q[cf1] - x[cf1];
    coeff[2] = -x[cf2];
    nTime[var] = t + minPosRoot(coeff, 2);
    coeff[0] = q[cf0] - lqu[var] - x[cf0];
    double timeaux = t + minPosRoot(coeff, 2);
    if(timeaux < nTime[var])
    {
      nTime[var] = timeaux;
    }
  }
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_recomputeNextTimes (QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double *lqu, double *q)
#else
void
QSS2_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
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
    QSS2_recomputeNextTime(quantizer, inf[i], t, nTime, x, lqu, q);
#ifdef QSS_PARALLEL
  }
#endif
  }
}

#ifdef QSS_PARALLEL
void
QSS2_PAR_updateQuantizedState (QA_quantizer quantizer, int i, double *q, double *x,
    double *lqu)
#else
void
QSS2_updateQuantizedState(QA_quantizer quantizer, int i, double *q, double *x,
    double *lqu)
#endif
{
  int cf0 = i * 3, cf1 = cf0 + 1;
  q[cf0] = x[cf0];
  q[cf1] = x[cf1];
}
