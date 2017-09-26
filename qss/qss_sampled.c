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

#include <qss/qss_sampled.h>

#include <stdlib.h>

#include "../common/data.h"
#include "qss_data.h"
#include "qss_log.h"
#include "qss_output.h"

#ifdef QSS_PARALLEL
void
SAM_PAR_writeOutvar (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput, int index, int variable)
#else
void
SAM_writeOutvar(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput, int index, int variable)
#endif
{
  int j, k, h, nOS = simOutput->nOS[index], nOD = simOutput->nOD[index], order =
      simData->order, coeffs = order + 1, nStates = nOS * coeffs;
  for(j = 0; j < nOS; j++)
  {
    double *q = simData->q;
    k = simOutput->OS[index][j];
    for(h = 0; h < order; h++)
    {
      output->state->values[j * coeffs + h] = q[k * coeffs + h];
    }
  }
  for(j = 0; j < nOD; j++)
  {
    k = simOutput->OD[index][j];
    output->state->values[nStates + j] = simData->d[k];
  }
  LG_writeLine(output->state->log, variable, simTime->time,
      output->state->values);
}

#ifdef QSS_PARALLEL
void
SAM_PAR_init (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#else
void
SAM_init(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#endif
{
  int i, j, max = 0, maxDiscrete = 0, outputs = simOutput->outputs, size =
      outputs;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
  size = lp->outputs;
#endif
  if(size > 0)
  {
    output->state->log = LG_Log(simData, simOutput);
    output->state->steps = (int*) malloc(outputs * sizeof(int));
    for(j = 0; j < outputs; j++)
    {
      if(max < simOutput->nOS[j])
      {
        max = simOutput->nOS[j];
      }
      if(maxDiscrete < simOutput->nOD[j])
      {
        maxDiscrete = simOutput->nOD[j];
      }
    }
    output->state->values = (double*) malloc(
        (((simData->order + 1) * max) + maxDiscrete) * sizeof(double));
    i = 0;
    for(j = 0; j < outputs; j++)
    {
#ifdef QSS_PARALLEL
      if (lp->oMap[j] > NOT_ASSIGNED)
      {
        SAM_PAR_writeOutvar (output, simData, simTime, simOutput, j, i);
#else
      SAM_writeOutvar(output, simData, simTime, simOutput, j, i);
#endif
      output->state->steps[i++] = 1;
#ifdef QSS_PARALLEL
    }
#endif
    }
#ifdef QSS_PARALLEL
    output->ops->write = SAM_PAR_write;
    output->ops->getSteps = SAM_PAR_getSteps;
    output->ops->save = SAM_PAR_save;
#else
    output->ops->write = SAM_write;
    output->ops->getSteps = SAM_getSteps;
    output->ops->save = SAM_save;
#endif

  }
}

#ifdef QSS_PARALLEL
void
SAM_PAR_write (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#else
void
SAM_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#endif
{
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
#endif
  int i, k, h, j;
  if(simTime->type == ST_State)
  {
    int nSO = simOutput->nSO[simTime->minIndex];
    for(i = 0; i < nSO; i++)
    {
      j = simOutput->SO[simTime->minIndex][i];
      int variable = j;
#ifdef QSS_PARALLEL
      if (lp->oMap[j] > NOT_ASSIGNED)
      {
        variable = lp->oMap[j];
        SAM_PAR_writeOutvar (output, simData, simTime, simOutput, j, variable);
#else
      SAM_writeOutvar(output, simData, simTime, simOutput, j, variable);
#endif
      output->state->steps[variable]++;
#ifdef QSS_PARALLEL
    }
#endif
    }
  }
  else if(simTime->type == ST_Event)
  {
    int nLHSSt = simData->event[simTime->minIndex].nLHSSt;
    for(k = 0; k < nLHSSt; k++)
    {
      h = simData->event[simTime->minIndex].LHSSt[k];
      int nSO = simOutput->nSO[h];
      for(i = 0; i < nSO; i++)
      {
        j = simOutput->SO[h][i];
        int variable = j;
#ifdef QSS_PARALLEL
        if (lp->oMap[j] > NOT_ASSIGNED)
        {
          variable = lp->oMap[j];
          SAM_PAR_writeOutvar (output, simData, simTime, simOutput, j,
              variable);
#else
        SAM_writeOutvar(output, simData, simTime, simOutput, j, variable);
#endif
        output->state->steps[variable]++;
#ifdef QSS_PARALLEL
      }
#endif
      }
    }
    int nLHSDsc = simData->event[simTime->minIndex].nLHSDsc;
    for(k = 0; k < nLHSDsc; k++)
    {
      h = simData->event[simTime->minIndex].LHSDsc[k];
      int nDO = simOutput->nDO[h];
      for(i = 0; i < nDO; i++)
      {
        j = simOutput->DO[h][i];
        int variable = j;
#ifdef QSS_PARALLEL
        if (lp->oMap[j] > NOT_ASSIGNED)
        {
          variable = lp->oMap[j];
          SAM_PAR_writeOutvar (output, simData, simTime, simOutput, j, variable);
#else
        SAM_writeOutvar(output, simData, simTime, simOutput, j, variable);
#endif
        output->state->steps[variable]++;
#ifdef QSS_PARALLEL
      }
#endif
      }
    }
  }
}

#ifdef QSS_PARALLEL
int
SAM_PAR_getSteps (OUT_output output, int var)
#else
int
SAM_getSteps(OUT_output output, int var)
#endif
{
  return output->state->steps[var];
}

#ifdef QSS_PARALLEL
void
SAM_PAR_save (OUT_output output)
#else
void
SAM_save(OUT_output output)
#endif
{
  LG_toFile(output->state->log);
}
