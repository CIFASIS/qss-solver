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

#include "qss_scheduler.h"

#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_binary.h"
#include "qss_binary_random.h"
#include "qss_linear.h"

SC_schedulerState
SC_SchedulerState()
{
  SC_schedulerState p = checkedMalloc(sizeof(*p));
  p->BTR_events = NULL;
  p->BTR_inputs = NULL;
  p->BTR_states = NULL;
  p->update = NULL;
  p->visit = NULL;
  p->events = NULL;
  p->info = NULL;
  p->inputs = NULL;
  p->states = NULL;
  p->minEventValue = 0;
  p->minInputValue = 0;
  p->tOut = 0;
  p->tUnit = 0;
  p->minEventIndex = 0;
  p->minInputIndex = 0;
  p->stepType = ST_State;
  p->lp = NULL;
  return p;
}

void
SC_freeSchedulerState(SC_schedulerState state)
{
  if(state->states != NULL)
  {
    BT_freeTree(state->states);
  }
  if(state->info != NULL)
  {
    BT_freeInfo(state->info);
  }
  if(state->events != NULL)
  {
    BT_freeTree(state->events);
  }
  if(state->inputs != NULL)
  {
    BT_freeTree(state->inputs);
  }
  if(state->BTR_states != NULL)
  {
    free(state->BTR_states);
  }
  if(state->visit != NULL)
  {
    BT_freeInfo(state->visit);
  }
  if(state->BTR_events != NULL)
  {
    free(state->BTR_events);
  }
  if(state->BTR_inputs != NULL)
  {
    free(state->BTR_inputs);
  }
  free(state);
}

SC_schedulerOps
SC_SchedulerOps()
{
  SC_schedulerOps p = checkedMalloc(sizeof(*p));
  p->update = NULL;
  return p;
}

void
SC_freeSchedulerOps(SC_schedulerOps ops)
{
  free(ops);
}

SC_scheduler
SC_Scheduler(QSS_data simData, QSS_time simTime)
{
  SC_scheduler p = checkedMalloc(sizeof(*p));
  p->state = SC_SchedulerState();
  p->ops = SC_SchedulerOps();
  switch(simTime->scheduler)
  {
    case ST_Linear:
      if(simData->params->lps > 0)
      {
        LN_PAR_init(p, simData, simTime);
      }
      else
      {
        LN_init(p, simData, simTime);
      }
      break;
    case ST_Binary:
      if(simData->params->lps > 0)
      {
        BT_PAR_init(p, simData, simTime);
      }
      else
      {
        BT_init(p, simData, simTime);
      }
      break;
    case ST_Random:
      BTR_init(p, simData, simTime);
      break;
    default:
      return NULL;
  }
  return p;
}

void
SC_freeScheduler(SC_scheduler t)
{
  SC_freeSchedulerState(t->state);
  SC_freeSchedulerOps(t->ops);
  free(t);
}

void
SC_update(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  scheduler->ops->update(scheduler, simData, simTime);
}

void
SC_setUpdate(SC_scheduler scheduler, BT_upd updFunction)
{
  scheduler->ops->update = updFunction;
}
