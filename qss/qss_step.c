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

#include <qss/qss_step.h>

#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_log.h"
#include "qss_output.h"

#ifdef QSS_PARALLEL
void
ST_PAR_writeOutvar (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput, int index, int variable)
#else
void
ST_writeOutvar(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput, int index, int variable)
#endif
{
  int j, k;
  double e;
  int nOS = simOutput->nOS[index], order = simData->order - 1, coeffs = order
      + 2;
  double *q = simData->q;
  double tmp[nOS];
  for(j = 0; j < nOS; j++)
  {
    k = simOutput->OS[index][j];
    e = simTime->time - simTime->tq[k];
    int cf0 = k * coeffs;
    tmp[j] = q[cf0];
    if(e)
    {
      q[cf0] = evaluatePoly(cf0, e, q, order);
    }
  }
  simOutput->value(index, q, simData->d, simData->alg, simTime->time, &e);
  LG_write(output->state->log, variable, simTime->time, e);
  for(j = 0; j < nOS; j++)
  {
    k = simOutput->OS[index][j];
    int cf0 = k * coeffs;
    q[cf0] = tmp[j];
  }
}

#ifdef QSS_PARALLEL
void
ST_PAR_init (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#else
void
ST_init(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#endif
{
  int j, i, outputs = simOutput->outputs, size = outputs;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
  size = lp->outputs;
#endif
  if(size)
  {
    output->state->steps = (int*) malloc(outputs * sizeof(int));
    output->state->log = LG_Log(simData, simOutput);
    j = 0;
    for(i = 0; i < outputs; i++)
    {
#ifdef QSS_PARALLEL
      if (lp->oMap[i] > NOT_ASSIGNED)
      {
        ST_PAR_writeOutvar (output, simData, simTime, simOutput, i, j);
#else
      ST_writeOutvar(output, simData, simTime, simOutput, i, j);
#endif
      output->state->steps[j++] = 1;
#ifdef QSS_PARALLEL
    }
#endif
    }
#ifdef QSS_PARALLEL
    output->ops->write = ST_PAR_write;
    output->ops->getSteps = ST_PAR_getSteps;
    output->ops->save = ST_PAR_save;
#else
    output->ops->write = ST_write;
    output->ops->getSteps = ST_getSteps;
    output->ops->save = ST_save;
#endif
  }
}

#ifdef QSS_PARALLEL
void
ST_PAR_write (OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#else
void
ST_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
#endif
{
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
#endif
  int i, j, k, h;
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
        ST_PAR_writeOutvar (output, simData, simTime, simOutput, j, variable);
#else
      ST_writeOutvar(output, simData, simTime, simOutput, j, variable);
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
          ST_PAR_writeOutvar (output, simData, simTime, simOutput, j, variable);
#else
        ST_writeOutvar(output, simData, simTime, simOutput, j, variable);
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
          ST_PAR_writeOutvar (output, simData, simTime, simOutput, j, variable);

#else
        ST_writeOutvar(output, simData, simTime, simOutput, j, variable);
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
ST_PAR_getSteps (OUT_output output, int var)
#else
int
ST_getSteps(OUT_output output, int var)
#endif
{
  return output->state->steps[var];
}

#ifdef QSS_PARALLEL
void
ST_PAR_save (OUT_output output)
#else
void
ST_save(OUT_output output)
#endif
{
  LG_toFile(output->state->log);
}
