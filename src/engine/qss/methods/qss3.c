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

#include <qss/methods/qss3.h>

#include <math.h>

#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

void QSS_FUNC_DECL(QSS3, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int i;
  int states = simData->states;
  for (i = 0; i < states; i++) {
    int cf0 = i * 4;
    simData->x[cf0 + 2] = 0;
    simData->x[cf0 + 3] = 0;
    simData->q[cf0] = simData->x[cf0];
    simData->q[cf0 + 1] = 0;
    simData->q[cf0 + 2] = 0;
  }
  QSS_ASSIGN_QUANTIZER_OPS(QSS3)
}

void QSS_FUNC_DECL(QSS3, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                             double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(QSS3, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(QSS3, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 4, cf1 = cf0 + 1, cf2 = cf1 + 1, cf3 = cf2 + 1;
  double coeff[4];
  if (fabs(q[cf0] - x[cf0]) >= lqu[var] * 0.999999999) {
    nTime[var] = t;
  } else {
    coeff[0] = q[cf0] + lqu[var] - x[cf0];
    coeff[1] = q[cf1] - x[cf1];
    coeff[2] = q[cf2] - x[cf2];
    coeff[3] = -x[cf3];
    nTime[var] = t + minPosRoot(coeff, 3);
    coeff[0] = q[cf0] - lqu[var] - x[cf0];
    double timeaux = t + minPosRoot(coeff, 3);
    if (timeaux < nTime[var]) {
      nTime[var] = timeaux;
    }
  }
}

void QSS_FUNC_DECL(QSS3, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf3 = var * 4 + 3;
  if (x[cf3]) {
    nTime[var] = t + pow(lqu[var] / fabs(x[cf3]), 1.0 / 3);
  } else {
    nTime[var] = INF;
  }
}

void QSS_FUNC_DECL(QSS3, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  int cf0 = i * 4, cf1 = cf0 + 1, cf2 = cf1 + 1;
  q[cf0] = x[cf0];
  q[cf1] = x[cf1];
  q[cf2] = x[cf2];
}
