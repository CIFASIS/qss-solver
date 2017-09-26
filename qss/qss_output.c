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

#include "qss_output.h"

#include <stdlib.h>

#include "../common/utils.h"
#include "qss_sampled.h"
#include "qss_step.h"

OUT_outputState
OUT_OutputState()
{
  OUT_outputState p = checkedMalloc(sizeof(*p));
  p->log = NULL;
  p->steps = NULL;
  p->values = NULL;
  return p;
}

OUT_outputOps
OUT_OutputOps()
{
  OUT_outputOps p = checkedMalloc(sizeof(*p));
  p->getSteps = NULL;
  p->save = NULL;
  p->write = NULL;
  return p;
}

OUT_output
OUT_Output(QSS_data simData, QSS_time simTime, SD_output simOutput)
{
  OUT_output p = checkedMalloc(sizeof(*p));
  p->state = OUT_OutputState();
  p->ops = OUT_OutputOps();
  switch(simOutput->commInterval)
  {
    case CI_Step:
      if(simData->params->lps > 0)
      {
        ST_PAR_init(p, simData, simTime, simOutput);
      }
      else
      {
        ST_init(p, simData, simTime, simOutput);
      }
      break;
    case CI_Dense:
      case CI_Sampled:
      if(simData->params->lps > 0)
      {
        SAM_PAR_init(p, simData, simTime, simOutput);
      }
      else
      {
        SAM_init(p, simData, simTime, simOutput);
      }
      break;
    default:
      return NULL;
  }
  return p;
}

void
OUT_freeOutputOps(OUT_outputOps o)
{
  free(o);
}

void
OUT_freeOutputState(OUT_outputState o)
{
  LG_freeLog(o->log);
  if(o->steps != NULL)
  {
    free(o->steps);
  }
  if(o->values != NULL)
  {
    free(o->values);
  }
  free(o);
}

void
OUT_freeOutput(OUT_output o)
{
  if(o != NULL)
  {
    OUT_freeOutputState(o->state);
    OUT_freeOutputOps(o->ops);
    free(o);
  }
}

void
OUT_write(OUT_output output, QSS_data simData, QSS_time simTime,
    SD_output simOutput)
{
  output->ops->write(output, simData, simTime, simOutput);
}

int
OUT_getSteps(OUT_output output, int var)
{
  return output->ops->getSteps(output, var);
}

void
OUT_save(OUT_output output)
{
  output->ops->save(output);
}
