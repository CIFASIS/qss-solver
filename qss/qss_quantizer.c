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

#include "qss_quantizer.h"

#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "cqss.h"
#include "liqss.h"
#include "liqss2.h"
#include "liqss3.h"
#include "qss.h"
#include "qss2.h"
#include "qss3.h"
#include "qss4.h"

QA_quantizerOps
QA_QuantizerOps()
{
  QA_quantizerOps p = checkedMalloc(sizeof(*p));
  p->recomputeNextTimes = NULL;
  p->recomputeNextTime = NULL;
  p->nextTime = NULL;
  p->updateQuantizedState = NULL;
  return p;
}

QA_quantizerState
QA_QuantizerState()
{
  QA_quantizerState p = checkedMalloc(sizeof(*p));
  p->order = 0;
  p->xOrder = 0;
  p->dq = NULL;
  p->a = NULL;
  p->oldDx = NULL;
  p->qAux = NULL;
  p->u0 = NULL;
  p->u1 = NULL;
  p->u2 = NULL;
  p->lt = NULL;
  p->ltq = NULL;
  p->lquOld = NULL;
  p->qinf = NULL;
  p->qsup = NULL;
  p->simTime = NULL;
  p->minStep = 0;
  p->finTime = 0;
  p->flag2 = NULL;
  p->flag3 = NULL;
  p->flag4 = NULL;
  p->lSimTime = NULL;
  p->qMap = NULL;
  return p;
}

QA_quantizer
QA_Quantizer(QSS_data simData, QSS_time simTime)
{
  QA_quantizer p = checkedMalloc(sizeof(*p));
  p->state = QA_QuantizerState();
  p->ops = QA_QuantizerOps();
  switch(simData->solver)
  {
    case SD_QSS:
      if(simData->params->lps > 0)
      {
        QSS_PAR_init(p, simData, simTime);
      }
      else
      {
        QSS_init(p, simData, simTime);
      }
      break;
    case SD_CQSS:
      if(simData->params->lps > 0)
      {
        CQSS_PAR_init(p, simData, simTime);
      }
      else
      {
        CQSS_init(p, simData, simTime);
      }
      break;
    case SD_LIQSS:
      if(simData->params->lps > 0)
      {
        LIQSS_PAR_init(p, simData, simTime);
      }
      else
      {
        LIQSS_init(p, simData, simTime);
      }
      break;
    case SD_QSS2:
      if(simData->params->lps > 0)
      {
        QSS2_PAR_init(p, simData, simTime);
      }
      else
      {
        QSS2_init(p, simData, simTime);
      }
      break;
    case SD_LIQSS2:
      if(simData->params->lps > 0)
      {
        LIQSS2_PAR_init(p, simData, simTime);
      }
      else
      {
        LIQSS2_init(p, simData, simTime);
      }
      break;
    case SD_QSS3:
      if(simData->params->lps > 0)
      {
        QSS3_PAR_init(p, simData, simTime);
      }
      else
      {
        QSS3_init(p, simData, simTime);
      }
      break;
    case SD_LIQSS3:
      if(simData->params->lps > 0)
      {
        LIQSS3_PAR_init(p, simData, simTime);
      }
      else
      {
        LIQSS3_init(p, simData, simTime);
      }
      break;
    case SD_QSS4:
      if(simData->params->lps > 0)
      {
        QSS4_PAR_init(p, simData, simTime);
      }
      else
      {
        QSS4_init(p, simData, simTime);
      }
      break;
    default:
      return NULL;
  }
  return p;
}

void
QA_freeQuantizerState(QA_quantizerState state)
{
  if(state->dq != NULL)
  {
    free(state->dq);
  }
  if(state->a != NULL)
  {
    free(state->a);
  }
  if(state->oldDx != NULL)
  {
    free(state->oldDx);
  }
  if(state->qAux != NULL)
  {
    free(state->qAux);
  }
  if(state->u0 != NULL)
  {
    free(state->u0);
  }
  if(state->u1 != NULL)
  {
    free(state->u1);
  }
  if(state->u2 != NULL)
  {
    free(state->u2);
  }
  if(state->lt != NULL)
  {
    free(state->lt);
  }
  if(state->ltq != NULL)
  {
    free(state->ltq);
  }
  if(state->lquOld != NULL)
  {
    free(state->lquOld);
  }
  if(state->qinf != NULL)
  {
    free(state->qinf);
  }
  if(state->qsup != NULL)
  {
    free(state->qsup);
  }
  if(state->flag2 != NULL)
  {
    free(state->flag2);
  }
  if(state->flag3 != NULL)
  {
    free(state->flag3);
  }
  if(state->flag4 != NULL)
  {
    free(state->flag4);
  }
  free(state);
}

void
QA_freeQuantizerOps(QA_quantizerOps ops)
{
  free(ops);
}

void
QA_freeQuantizer(QA_quantizer quantizer)
{
  QA_freeQuantizerState(quantizer->state);
  QA_freeQuantizerOps(quantizer->ops);
  free(quantizer);
}

void
QA_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t,
    double *nTime, double *x, double * lqu, double *q)
{
  quantizer->ops->recomputeNextTimes(quantizer, vars, inf, t, nTime, x, lqu, q);
}

void
QA_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime,
    double *x, double *lqu, double *q)
{
  quantizer->ops->recomputeNextTime(quantizer, var, t, nTime, x, lqu, q);
}

void
QA_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x,
    double *lqu)
{
  quantizer->ops->nextTime(quantizer, var, t, nTime, x, lqu);
}

void
QA_updateQuantizedState(QA_quantizer quantizer, int var, double *q, double *x,
    double *lqu)
{
  quantizer->ops->updateQuantizedState(quantizer, var, q, x, lqu);
}
