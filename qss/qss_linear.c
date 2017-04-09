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

#include <qss/qss_linear.h>

#include <stdio.h>

#include "../common/utils.h"
#include "qss_data.h"
#include "qss_scheduler.h"

#ifdef QSS_PARALLEL
void
LN_PAR_init (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
LN_init (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int j, minVarIdx = 0, w = ST_State;
  double minVar = INF;
  int minEventIdx = 0;
  double minEvent = INF;
  int states = simData->states;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
#endif
  for (j = 0; j < states; j++)
    {
#ifdef QSS_PARALLEL
      if (lp->qMap[j] > NOT_ASSIGNED)
	{
#endif
      if (simTime->nextStateTime[j] < minVar)
	{
	  minVar = simTime->nextStateTime[j];
	  minVarIdx = j;
	}
#ifdef QSS_PARALLEL
    }
#endif
    }
  int events = simData->events;
  for (j = 0; j < events; j++)
    {
#ifdef QSS_PARALLEL
      if (lp->eMap[j] > NOT_ASSIGNED)
	{
#endif
      if (simTime->nextEventTime[j] < minEvent)
	{
	  minEvent = simTime->nextEventTime[j];
	  minEventIdx = j;
	}
#ifdef QSS_PARALLEL
    }
#endif
    }
  scheduler->state->minEventValue = minEvent;
  scheduler->state->minEventIndex = minEventIdx;
  if (minEvent < minVar)
    {
      minVar = minEvent;
      minVarIdx = minEventIdx;
      w = ST_Event;
    }
  minEvent = INF;
  minEventIdx = 0;
  int inputs = simData->inputs;
  for (j = 0; j < inputs; j++)
    {
#ifdef QSS_PARALLEL
      if (lp->iMap[j] > NOT_ASSIGNED)
	{
#endif
      if (simTime->nextInputTime[j] < minEvent)
	{
	  minEvent = simTime->nextInputTime[j];
	  minEventIdx = j;
	}
#ifdef QSS_PARALLEL
    }
#endif
    }
  scheduler->state->minInputValue = minEvent;
  scheduler->state->minInputIndex = minEventIdx;
  if (minEvent < minVar)
    {
      minVar = minEvent;
      minVarIdx = minEventIdx;
      w = ST_Input;
    }
  simTime->minValue = minVar;
  simTime->minIndex = minVarIdx;
  simTime->type = w;
  simTime->time += simTime->minValue;
  scheduler->state->tUnit = (simData->ft - simData->it) / 100.0;
  scheduler->state->tOut = 1;
  scheduler->ops->update = LN_update;
}

#ifdef QSS_PARALLEL
void
LN_PAR_update (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#else
void
LN_update (SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
#endif
{
  int j, minVarIdx = 0, w = ST_State;
  double minVar = INF;
  int minEventIdx = 0;
  double minEvent = INF;
  int states = simData->states;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = simData->lp;
#endif
  for (j = 0; j < states; j++)
    {
#ifdef QSS_PARALLEL
      if (lp->qMap[j] > NOT_ASSIGNED)
	{
#endif
      if (simTime->nextStateTime[j] < minVar)
	{
	  minVar = simTime->nextStateTime[j];
	  minVarIdx = j;
	}
#ifdef QSS_PARALLEL
    }
#endif
    }
  int events = simData->events;
  if (simTime->type == ST_State && events)
    {
      if (simData->nSZ[simTime->minIndex])
	{
	  for (j = 0; j < events; j++)
	    {
#ifdef QSS_PARALLEL
	      if (lp->eMap[j] > NOT_ASSIGNED)
		{
#endif
	      if (simTime->nextEventTime[j] < minEvent)
		{
		  minEvent = simTime->nextEventTime[j];
		  minEventIdx = j;
		}
#ifdef QSS_PARALLEL
	    }
#endif
	    }
	  scheduler->state->minEventValue = minEvent;
	  scheduler->state->minEventIndex = minEventIdx;
	}
    }
  if (simTime->type == ST_Event)
    {
      for (j = 0; j < events; j++)
	{
#ifdef QSS_PARALLEL
	  if (lp->eMap[j] > NOT_ASSIGNED)
	    {
#endif
	  if (simTime->nextEventTime[j] < minEvent)
	    {
	      minEvent = simTime->nextEventTime[j];
	      minEventIdx = j;
	    }
#ifdef QSS_PARALLEL
	}
#endif
	}
      scheduler->state->minEventValue = minEvent;
      scheduler->state->minEventIndex = minEventIdx;
    }
  if (simTime->type == ST_Input)
    {
      int inputs = simData->inputs;
      for (j = 0; j < inputs; j++)
	{
#ifdef QSS_PARALLEL
	  if (lp->iMap[j] > NOT_ASSIGNED)
	    {
#endif
	  if (simTime->nextInputTime[j] < minEvent)
	    {
	      minEvent = simTime->nextInputTime[j];
	      minEventIdx = j;
	    }
#ifdef QSS_PARALLEL
	}
#endif
	}
      scheduler->state->minInputValue = minEvent;
      scheduler->state->minInputIndex = minEventIdx;
    }
  if (scheduler->state->minEventValue < minVar)
    {
      minVar = scheduler->state->minEventValue;
      minVarIdx = scheduler->state->minEventIndex;
      w = ST_Event;
    }
  if (scheduler->state->minInputValue < minVar)
    {
      minVar = scheduler->state->minInputValue;
      minVarIdx = scheduler->state->minInputIndex;
      w = ST_Input;
    }
  simTime->minValue = minVar;
  simTime->minIndex = minVarIdx;
  simTime->type = w;
  simTime->time = simTime->minValue;
#ifdef QSS_PARALLEL
  if (simTime->time == INF)
    {
      simTime->time = simData->ft;
    }
#endif
  double tElapsed = (simTime->time - simData->it);
  if (tElapsed >= scheduler->state->tOut * scheduler->state->tUnit)
    {
      scheduler->state->tOut++;
      fprintf (stderr, "*%.0f", tElapsed);
    }
}
