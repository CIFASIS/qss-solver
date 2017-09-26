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

#include <qss/qss_memory.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_log.h"

#ifdef QSS_PARALLEL
void
M_PAR_init (LG_log log, QSS_data simData, SD_output simOutput)
#else
void
M_init(LG_log log, QSS_data simData, SD_output simOutput)
#endif
{
  int i, size, outIdx;
#ifdef QSS_PARALLEL
  int localIdx = 0;
  log->state->size = simData->lp->outputs;
#else
  log->state->size = simOutput->outputs;
#endif
  size = simOutput->outputs;
  if(size > 0)
  {
    log->state->states = (list*) checkedMalloc(
        log->state->size * sizeof(struct list_));
    for(i = 0; i < size; i++)
    {
#ifdef QSS_PARALLEL
      if (simData->lp->oMap[i] <= NOT_ASSIGNED)
      {
        continue;
      }
      else
      {
        outIdx = localIdx;
        localIdx++;
      }
#else
      outIdx = i;
#endif
      if(simOutput->commInterval == CI_Dense
          || simOutput->commInterval == CI_Sampled)
      {
        log->state->states[outIdx] = List(simData->params->nodeSize,
            (simData->order + 1) * simOutput->nOS[i] + simOutput->nOD[i]);
      }
      else
      {
        log->state->states[outIdx] = List(simData->params->nodeSize, 1);
      }
    }
#ifdef QSS_PARALLEL
    log->ops->write = M_PAR_write;
    log->ops->writeLine = M_PAR_writeLine;
    log->ops->toFile = M_PAR_toFile;
#else
    log->ops->write = M_write;
    log->ops->writeLine = M_writeLine;
    log->ops->toFile = M_toFile;
#endif
    log->state->data = simData;
    log->state->output = simOutput;
  }
}

#ifdef QSS_PARALLEL
void
M_PAR_write (LG_log log, int i, double time, double value)
#else
void
M_write(LG_log log, int i, double time, double value)
#endif
{
  append(log->state->states[i], time, &value);
}

#ifdef QSS_PARALLEL
void
M_PAR_writeLine (LG_log log, int i, double time, double *value)
#else
void
M_writeLine(LG_log log, int i, double time, double *value)
#endif
{
  append(log->state->states[i], time, value);
}

#ifdef QSS_PARALLEL
void
M_PAR_toFile (LG_log log)
#else
void
M_toFile(LG_log log)
#endif
{
  int i, j;
  log->state->output->store = SD_File;
  LG_log fileLog = LG_Log(log->state->data, log->state->output);
  int outputs = log->state->size;
  if(log->state->output->commInterval == CI_Step)
  {
    for(i = 0; i < outputs; i++)
    {
      node n = log->state->states[i]->begin;
      while(n)
      {
        for(j = 0; j < n->used; j++)
        {
          LG_write(fileLog, i, n->timeVal[j], n->val[j][0]);
        }
        n = n->next;
      }
    }
  }
  if(log->state->output->commInterval == CI_Dense)
  {
    for(i = 0; i < outputs; i++)
    {
      node n = log->state->states[i]->begin;
      while(n)
      {
        for(j = 0; j < n->used; j++)
        {
          LG_writeLine(fileLog, i, n->timeVal[j], n->val[j]);
        }
        n = n->next;
      }
    }
  }
  if(log->state->output->commInterval == CI_Sampled)
  {
    int order = log->state->data->order;
    int coeffs = order + 1;
    int outIdx;
#ifdef QSS_PARALLEL
    int localIdx = 0;
#endif
    outputs = log->state->output->outputs;
    double *tmp1 = (double*) malloc(
        log->state->data->states * (log->state->data->order + 1)
            * sizeof(double));
    for(i = 0; i < outputs; i++)
    {
#ifdef QSS_PARALLEL
      if (log->state->data->lp->oMap[i] <= NOT_ASSIGNED)
      {
        continue;
      }
      else
      {
        outIdx = localIdx;
        localIdx++;
      }
#else
      outIdx = i;
#endif
      int index = 1;
      double dt = log->state->output->sampled->period[i];
      if(dt == 0)
      {
        dt = 1e-2;
      }
      node n = log->state->states[outIdx]->begin;
      while(n)
      {
        for(j = 0; j < n->used; j++)
        {
          double nextStep;
          if(j + 1 == n->used)
          {
            if(n->next == NULL)
            {
              nextStep = log->state->data->ft + dt / 10;
            }
            else
            {
              nextStep = n->next->timeVal[0];
            }
          }
          else
          {
            nextStep = n->timeVal[j + 1];
          }
          if(n->timeVal[j] == nextStep)
          {
            continue;
          }
          double nt = log->state->output->sampled->nextTime[i];
          double e = nt - n->timeVal[j];
          while(nt <= nextStep)
          {
            int g;
            double tmpval = 0;
            double *d = log->state->data->d;
            double *alg = log->state->data->alg;
            if(log->state->output->nOS[i])
            {
              int nOS = log->state->output->nOS[i];
              for(g = 0; g < nOS; g++)
              {
                int stVar = log->state->output->OS[i][g];
                tmp1[stVar * coeffs] = evaluateVectorPoly(e,
                    &(n->val[j][g * coeffs]), order);
              }
            }
            if(log->state->output->nOD[i])
            {
              int nOD = log->state->output->nOD[i];
              int nStates = log->state->output->nOS[i] * coeffs;
              for(g = 0; g < nOD; g++)
              {
                int dscVar = log->state->output->OD[i][g];
                d[dscVar] = n->val[j][nStates + g];
              }
            }
            log->state->output->value(i, tmp1, d, alg, nt, &tmpval);
            LG_write(fileLog, outIdx, nt, tmpval);
            e += dt;
            log->state->output->sampled->nextTime[i] = dt * index++;
            nt = log->state->output->sampled->nextTime[i];
          }
        }
        n = n->next;
      }
    }
    free(tmp1);
  }
  LG_freeLog(fileLog);
  log->state->output->store = SD_Memory;
}
