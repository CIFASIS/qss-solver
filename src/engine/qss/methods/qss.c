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

#include <qss/methods/qss.h>

#include <math.h>

#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_quantizer.h>

void QSS_FUNC_DECL(QSS, init)(QA_quantizer quantizer, QSS_data simData, QSS_time simTime)
{
  int i;
  int states = simData->states;
  for (i = 0; i < states; i++) {
    int cf0 = i * 2;
    simData->q[cf0] = simData->x[cf0];
  }
  QSS_ASSIGN_QUANTIZER_OPS(QSS)
}

void QSS_FUNC_DECL(QSS, recomputeNextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q)
{
  int cf0 = var * 2, cf1 = cf0 + 1;
  double coeff[2];
  coeff[0] = q[cf0] - x[cf0] - lqu[var];
  coeff[1] = -x[cf1];
  nTime[var] = t + minPosRoot(coeff, 1);
  coeff[0] = q[cf0] - x[cf0] + lqu[var];
  double timeaux = t + minPosRoot(coeff, 1);
  if (timeaux < nTime[var]) {
    nTime[var] = timeaux;
  }
}

void QSS_FUNC_DECL(QSS, recomputeNextTimes)(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu,
                                            double *q)
{
  int i;
  QSS_PARALLEL_EXP(int *map = quantizer->state->qMap;)
  for (i = 0; i < vars; i++) {
    QSS_PARALLEL_EXP(if (map[inf[i]] != NOT_ASSIGNED) {)
      QSS_FUNC_INVK(QSS, recomputeNextTime)(quantizer, inf[i], t, nTime, x, lqu, q);
    QSS_PARALLEL_EXP(
    })
  }
}

void QSS_FUNC_DECL(QSS, nextTime)(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu)
{
  int cf1 = var * 2 + 1;
  if (x[cf1]) {
    nTime[var] = t + fabs(lqu[var] / x[cf1]);
  } else {
    nTime[var] = INF;
  }
}

void QSS_FUNC_DECL(QSS, updateQuantizedState)(QA_quantizer quantizer, int i, double *q, double *x, double *lqu)
{
  int cf0 = i * 2;
  q[cf0] = x[cf0];
}
