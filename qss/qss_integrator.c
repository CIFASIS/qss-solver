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

#include "qss_integrator.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../common/data.h"
#include "qss_commands.h"
#include "qss_data.h"
#include "qss_dt.h"
#include "qss_frw.h"
#include "qss_graph.h"
#include "qss_lp.h"
#include "qss_output.h"
#include "qss_parallel.h"
#include "qss_partition.h"
#include "qss_quantizer.h"
#include "qss_scheduler.h"
#include "qss_sim_steps.h"

void
QSS_SEQ_saveLog (QSS_simulator simulator)
{
  int outputs = simulator->output->outputs;
  if (outputs)
    {
      getTime (simulator->stats->sTime);
      OUT_save (simulator->log);
      getTime (simulator->stats->sdTime);
      subTime (simulator->stats->sdTime, simulator->stats->sTime);
      simulator->stats->saveTime = getTimeValue (simulator->stats->sdTime);
    }
}

grp_t
getSteps (QSS_simulator simulator, grp_t infVar)
{
  grp_t states = simulator->data->states;
  if (infVar < states)
    {
      return (simulator->simulationLog->states[infVar]);
    }
  else
    {
      return (simulator->simulationLog->handlers[infVar - states]);
    }
}

void
QSS_generateWeights (QSS_simulator simulator)
{
  if (simulator->settings->debug & SD_DBG_VarChanges)
    {
      FILE* vweights = NULL;
      FILE* eweights = NULL;
      FILE* heweights = NULL;
      grp_t *vwgt = NULL, *ewgt = NULL;
      if (simulator->settings->debug & SD_DBG_Weights)
	{
	  grp_t *xadj = NULL, *adjncy = NULL, *hevars = NULL, edges = 0;
	  char fileName[256];
	  sprintf (fileName, "%s.vwgts", simulator->output->name);
	  vweights = fopen (fileName, "wb");
	  sprintf (fileName, "%s.ewgts", simulator->output->name);
	  eweights = fopen (fileName, "wb");
	  sprintf (fileName, "%s.hewgts", simulator->output->name);
	  heweights = fopen (fileName, "wb");
	  int states = simulator->data->states;
	  int events = simulator->data->events;
	  int vsize = states + events, eiter;
	  double normSteps = 0;
	  grp_t maxVSteps = 0;
	  grp_t maxESteps = 0;
	  simulator->data->params->pm = SD_Metis;
	  if (GRP_readGraph (simulator->output->name, simulator->data, &xadj,
			     &adjncy, &edges, 1, &vwgt, &ewgt, 0, NULL)
	      == GRP_ReadError)
	    {
	      fprintf (stderr, "Could not read generated graph files.\n");
	      abort ();
	    }
	  for (eiter = 0; eiter < vsize; eiter++)
	    {
	      grp_t steps = getSteps (simulator, eiter);
	      if (steps > maxVSteps)
		{
		  maxVSteps = steps;
		}
	    }
	  for (eiter = 0; eiter < vsize; eiter++)
	    {
	      grp_t steps = getSteps (simulator, eiter);
	      normSteps = steps / (double) maxVSteps;
	      vwgt[eiter] = vwgt[eiter] + (int) (100 * normSteps);
	      fwrite (&(vwgt[eiter]), sizeof(grp_t), 1, vweights);
	    }
	  for (eiter = 0; eiter < vsize; eiter++)
	    {
	      grp_t steps = getSteps (simulator, eiter);
	      int init = xadj[eiter], end = xadj[eiter + 1], iter;
	      for (iter = init; iter < end; iter++)
		{
		  grp_t infVar = adjncy[iter];
		  if (steps + getSteps (simulator, infVar) > maxESteps)
		    {
		      maxESteps = steps + getSteps (simulator, infVar);
		    }
		}
	    }
	  for (eiter = 0; eiter < vsize; eiter++)
	    {
	      grp_t steps = getSteps (simulator, eiter);
	      int init = xadj[eiter], end = xadj[eiter + 1], iter;
	      for (iter = init; iter < end; iter++)
		{
		  if (ewgt[iter] == 1)
		    {
		      fwrite (&(ewgt[iter]), sizeof(grp_t), 1, eweights);
		    }
		  else
		    {
		      grp_t infVar = adjncy[iter];
		      grp_t infVarSteps = getSteps (simulator, infVar);
		      normSteps = (steps + infVarSteps) / (double) maxESteps;
		      grp_t totalWgt = ewgt[iter] + (int) 100 * normSteps;
		      fwrite (&(totalWgt), sizeof(grp_t), 1, eweights);
		    }
		}
	    }
	  free (xadj);
	  free (adjncy);
	  free (vwgt);
	  free (ewgt);
	  xadj = NULL;
	  adjncy = NULL;
	  vwgt = NULL;
	  ewgt = NULL;
	  simulator->data->params->pm = SD_Patoh;
	  if (GRP_readGraph (simulator->output->name, simulator->data, &xadj,
			     &adjncy, &edges, 1, &vwgt, &ewgt, 1, &hevars)
	      == GRP_ReadError)
	    {
	      fprintf (stderr, "Could not read generated graph files.\n");
	      abort ();
	    }
	  for (eiter = 0; eiter < edges; eiter++)
	    {
	      int var = hevars[eiter];
	      grp_t steps = getSteps (simulator, var);
	      normSteps = steps / (double) maxVSteps;
	      ewgt[eiter] = ewgt[eiter] + (int)(100*normSteps);
	      fwrite (&(ewgt[eiter]), sizeof(grp_t), 1, heweights);
	    }
	  free (xadj);
	  free (adjncy);
	  free (vwgt);
	  free (ewgt);
	  free (hevars);
	}
      SD_print (simulator->simulationLog, "State Variables:");
      int forUL = simulator->data->states, j;
      for (j = 0; j < forUL; j++)
	{
	  SD_print (simulator->simulationLog, "Variable %d changes: %d", j,
		    simulator->simulationLog->states[j]);
	}
      if (simulator->data->events > 0)
	{
	  SD_print (simulator->simulationLog, "Handler execution:");
	  forUL = simulator->data->events;
	  for (j = 0; j < forUL; j++)
	    {
	      SD_print (simulator->simulationLog, "Handler %d:  %d", j,
			simulator->simulationLog->handlers[j]);
	    }
	}
      if (simulator->settings->debug & SD_DBG_Weights)
	{
	  fclose (vweights);
	  fclose (eweights);
	  fclose (heweights);
	}
    }
}

void
QSS_SEQ_printSimulationLog (QSS_simulator simulator)
{
  SD_print (simulator->simulationLog, "Simulation time: %g ms",
	    simulator->stats->simulationTime);
  SD_print (simulator->simulationLog, "CPU time per transition: %g",
	    simulator->stats->simulationTime / simulator->stats->totalSteps);
  int nOutputs = simulator->output->outputs;
  if (nOutputs > 0)
    {
      SD_print (simulator->simulationLog, "Simulation output:");
      int j;
      for (j = 0; j < nOutputs; j++)
	{
	  SD_print (simulator->simulationLog, "Variable %s changes: %d",
		    simulator->output->variable[j].name,
		    OUT_getSteps (simulator->log, j));
	}
    }
  SD_print (simulator->simulationLog, "Simulation transitions: %lu",
	    simulator->stats->totalSteps);
  if (simulator->stats->reinits > 0)
    {
      SD_print (simulator->simulationLog, "State variable reinits: %lu",
		simulator->stats->reinits);
    }
  SD_print (simulator->simulationLog, "Initialization time: %g ms",
	    simulator->stats->initTime);
  SD_print (simulator->simulationLog, "Save data time: %g ms",
	    simulator->stats->saveTime);
  SD_print (
      simulator->simulationLog,
      "Total time: %g ms",
      simulator->stats->initTime + simulator->stats->simulationTime
	  + simulator->stats->saveTime);
#ifdef DEBUG
  QSS_generateWeights (simulator);
#endif
  SD_print (simulator->simulationLog, "");
}

void
QSS_SEQ_logMemory (QSS_simulator simulator)
{
  QSS_data data = simulator->data;
  SD_output output = simulator->output;
  int xOrder = data->order + 1, states = data->states, events = data->events,
      sDouble = sizeof(double), sInt = sizeof(int), discretes = data->discretes,
      outputs = output->outputs, algs = data->algs, inputs = data->inputs;
  simulator->stats->memory += (3 * states + 4 * states * xOrder + algs * xOrder
      + discretes) * sDouble + (4 * states + inputs) * sInt;
  if (events)
    {
      simulator->stats->memory += (6 * events + 2 * states) * sInt;
      simulator->stats->memory += events * sizeof(struct SD_eventData_);
    }
  int i;
  for (i = 0; i < states; i++)
    {
      simulator->stats->memory += data->nSD[i] * sInt;
      simulator->stats->memory += data->nDS[i] * sInt;
      if (events)
	{
	  simulator->stats->memory += data->nSZ[i] * sInt;
	}
    }
  for (i = 0; i < events; i++)
    {
      simulator->stats->memory += data->nZS[i] * sInt;
      simulator->stats->memory += data->nHD[i] * sInt;
      simulator->stats->memory += data->nHZ[i] * sInt;
      simulator->stats->memory += data->event[i].nLHSSt * sInt;
      simulator->stats->memory += data->event[i].nLHSDsc * sInt;
      simulator->stats->memory += data->event[i].nRHSSt * sInt;
    }
  simulator->stats->memory += (3 * states + events + inputs) * sDouble;
  if (simulator->time->weights != NULL)
    {
      simulator->stats->memory += events * sDouble;
    }
  if (discretes)
    {
      simulator->stats->memory += 2 * discretes * sInt;
    }
  if (states)
    {
      simulator->stats->memory += 2 * states * sInt;
    }
  if (outputs)
    {
      simulator->stats->memory += 4 * outputs * sInt;
      for (i = 0; i < states; i++)
	{
	  simulator->stats->memory += output->nSO[i] * sInt;
	}
      for (i = 0; i < discretes; i++)
	{
	  simulator->stats->memory += output->nDO[i] * sInt;
	}
      for (i = 0; i < outputs; i++)
	{
	  simulator->stats->memory += output->nOS[i] * sInt;
	  simulator->stats->memory += output->nOD[i] * sInt;
	}
      int nOutputs = simulator->output->outputs;
      if (nOutputs > 0)
	{
	  int j = 0;
	  for (j = 0; j < nOutputs; j++)
	    {
	      int steps = OUT_getSteps (simulator->log, j);
	      int nodes = (int) (steps / data->params->nodeSize) + 1;
	      simulator->stats->memory += (2 * data->params->nodeSize
		  + nodes * data->params->nodeSize * 2) * sDouble;
	    }
	}
    }
  int treeH = 0, treeS = 0, treeE = 0, treeI = 0, max = states, treeT = 0;
  if (states)
    {
      treeH = (int) LOG2(states);
      treeS = (pow (2, treeH + 1) - 1 + states) * sDouble;
      treeT = treeS * sDouble;
    }
  if (events)
    {
      treeH = (int) LOG2(events);
      treeE = (pow (2, treeH + 1) - 1 + events) * sDouble;
      treeT += treeE;
      if (events > max)
	{
	  max = events;
	}
    }
  if (inputs)
    {
      treeH = (int) LOG2(inputs);
      treeI = (pow (2, treeH + 1) - 1 + inputs) * sDouble;
      treeT += treeI;
      if (inputs > max)
	{
	  max = inputs;
	}
    }
  if (max == states)
    {
      treeT += treeS;
    }
  else if (max == events)
    {
      treeT += treeE;
    }
  else
    {
      treeT += treeI;
    }
  int solverM = 0;
  switch (simulator->data->solver)
    {
    case SD_CQSS:
      solverM = 3 * states * sDouble;
      break;
    case SD_LIQSS:
      solverM = 6 * states * sDouble;
      break;
    case SD_LIQSS2:
      solverM = 12 * states * sDouble;
      break;
    case SD_LIQSS3:
      solverM = 12 * states * sDouble;
      break;
    default:
      break;
    }
  simulator->stats->memory += treeT + solverM;
}

void
QSS_SEQ_initialize (SIM_simulator simulate)
{
  QSS_simulator simulator = (QSS_simulator) simulate->state->sim;
  QSS_CMD_init (simulator);
  getTime (simulator->stats->iTime);
  int i, j, k, s, forUL;
  double e, zc[4];
  simulator->frw = FRW_Framework (simulator->data);
  double t = simulator->time->time;
  char logFile[128];
  sprintf (logFile, "%s", simulator->output->name);
  simulator->simulationLog = SD_SimulationLog (logFile);
  // Local mappings.
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  FRW_framework frw = simulator->frw;
  double *q = qssData->q;
  double *x = qssData->x;
  const int order = qssData->order;
  const int coeffs = order + 1;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
#endif
  QSS_model qssModel = simulator->model;
  QA_quantizer quantizer;
  SD_output output = simulator->output;
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "Initialize simulation\n");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      qssData->lqu[i] = qssData->dQRel[i] * fabs (x[i * coeffs]);
      if (qssData->lqu[i] < qssData->dQMin[i])
	{
	  qssData->lqu[i] = qssData->dQMin[i];
	}
#ifdef DEBUG
      if (settings->debug & SD_DBG_InitValues)
	{
	  SD_print (simulationLog, "Initial value: x[%d][0] = %g", i, x[i * coeffs]);
	}
#endif
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "Initialize solver...");
    }
#endif
  simulator->quantizer = QA_Quantizer (qssData, qssTime);
  quantizer = simulator->quantizer;
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize state derivatives...");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      FRW_recomputeDerivative (frw, qssModel, qssData, qssTime, i);
      QA_nextTime (quantizer, i, t, qssTime->nextStateTime, x, qssData->lqu);
#ifdef DEBUG
      if (settings->debug & SD_DBG_InitValues)
	{
	  SD_print (simulationLog, "Initial derivative: x[%d][1] = %g", i,
	      x[i * coeffs + 1]);
	}
#endif
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize input...");
    }
#endif
  forUL = qssData->inputs;
  for (i = 0; i < forUL; i++)
    {
      j = qssData->TD[i];
      FRW_nextInputTime (frw, qssModel, qssData, qssTime, 0, j, i);
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize events...");
    }
#endif
  forUL = qssData->events;
  for (i = 0; i < forUL; i++)
    {
      if (qssData->nZS[i])
	{
	  int nZS = qssData->nZS[i];
	  e = INF;
	  for (j = 0; j < nZS; j++)
	    {
	      k = qssData->ZS[i][j];
	      if (qssData->dQMin[k] < e)
		{
		  e = qssData->dQMin[k];
		}
	    }
	}
      else
	{
	  e = qssData->ft * qssData->params->zcHyst;
	}
      qssModel->events->zeroCrossing (i, q, qssData->d, qssData->alg,
				      qssTime->time, zc);
      s = sign (zc[0]);
      qssData->event[i].zcSign = s;
      qssData->event[i].zcHyst = e / 10.0;
      FRW_nextEventTime (frw, qssModel, qssData, qssTime, i);
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize state variables time...");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      QA_recomputeNextTime (quantizer, i, qssTime->time, qssTime->nextStateTime,
			    x, qssData->lqu, q);
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize output...");
    }
#endif
  simulator->log = OUT_Output (qssData, qssTime, output);
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize time...");
    }
#endif
  simulator->scheduler = SC_Scheduler (qssData, qssTime);
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
    }
  if (settings->debug & SD_DBG_VarChanges)
    {
      SD_setSimulationLogVariables(simulationLog, qssData->states, qssData->events);
    }
#endif
  QSS_SEQ_logMemory (simulator);
  getTime (simulator->stats->sTime);
  subTime (simulator->stats->sTime, simulator->stats->iTime);
  simulator->stats->initTime += getTimeValue (simulator->stats->sTime);
}

//// PARALLEL  INTERFACE ///////////////////////////////////////////////////////////

#ifdef __linux__

#include <pthread.h>

void
QSS_PAR_saveLog (QSS_simulator simulator)
{
  int outputs = simulator->data->lp->outputs;
  if (outputs)
    {
      getTime (simulator->stats->sTime);
      OUT_save (simulator->log);
      getTime (simulator->stats->sdTime);
      subTime (simulator->stats->sdTime, simulator->stats->sTime);
      simulator->stats->saveTime = getTimeValue (simulator->stats->sdTime);
    }
}

void
QSS_PAR_printSimulationLog (QSS_simulator simulator)
{
  SD_print (simulator->simulationLog, "Simulation Results for LP: %d",
	    simulator->id);
  SD_print (simulator->simulationLog, "");
  SD_print (simulator->simulationLog, "Simulation time: %g ms",
	    simulator->stats->simulationTime);
  SD_print (simulator->simulationLog, "CPU time per transition: %g",
	    simulator->stats->simulationTime / simulator->stats->totalSteps);
  if (simulator->data->lp->outputs > 0)
    {
      int nOutputs = simulator->output->outputs;
      if (nOutputs > 0)
	{
	  SD_print (simulator->simulationLog, "Simulation output:");
	  int k = 0;
	  int j;
	  for (j = 0; j < nOutputs; j++)
	    {
	      if (simulator->data->lp->oMap[j] > NOT_ASSIGNED)
		{
		  SD_print (simulator->simulationLog, "Variable %s changes: %d",
			    simulator->output->variable[j].name,
			    OUT_getSteps (simulator->log, k++));
		}
	    }
	}
    }
  SD_print (simulator->simulationLog, "Simulation transitions: %lu",
	    simulator->stats->totalSteps);
  if (simulator->stats->reinits > 0)
    {
      SD_print (simulator->simulationLog, "State variable reinits: %lu",
		simulator->stats->reinits);
    }
  SD_print (simulator->simulationLog, "Simulation messages sent: %lu",
	    simulator->stats->messages);
  SD_print (simulator->simulationLog, "Simulation external events: %lu",
	    simulator->stats->extTrans);
  SD_print (
      simulator->simulationLog,
      "Simulation past events: %lu (%.2f %%)",
      simulator->stats->pastEvents,
      (simulator->stats->extTrans > 0) ?
	  ((double) simulator->stats->pastEvents
	      / (double) simulator->stats->extTrans) * 100 :
	  0);
  SD_print (simulator->simulationLog, "Dt sum: %g",
	    simulator->dt->state->avgDt);
  SD_print (simulator->simulationLog, "Dt changes: %d",
	    simulator->dt->state->dtChanges);
  if (simulator->dt->state->dtChanges > 0)
    {
      SD_print (simulator->simulationLog, "Average dt: %g",
		simulator->dt->state->avgDt / simulator->dt->state->dtChanges);
    }
  else
    {
      SD_print (simulator->simulationLog, "Average dt: 0");
    }
  SD_print (simulator->simulationLog, "Initialization time: %g ms",
	    simulator->stats->initTime);
  SD_print (simulator->simulationLog, "Save data time: %g ms",
	    simulator->stats->saveTime);
  SD_print (
      simulator->simulationLog,
      "Total time: %g ms",
      simulator->stats->initTime + simulator->stats->simulationTime
	  + simulator->stats->saveTime);
  SD_print (simulator->simulationLog, "");
}

void
QSS_PAR_statistics (QSS_simulator simulator)
{
  unsigned long totalSimSteps = 0, totalLPSteps = 0, maxSteps =
      simulator->stats->steps[0], minSteps = maxSteps, totalExternalEvents = 0,
      totalMessages = 0;
  double maxSimulationTime = 0, totalTime = 0, memoryIncrement = 0, pm = 0, sm =
      0;
  double avgTime = 0, avgSteps = 0, avgStepCost = 0;
  int i, lps = simulator->data->params->lps;
  for (i = 0; i < lps; i++)
    {
      int simSteps = simulator->stats->steps[i]
	  + simulator->stats->simulationExternalEvents[i];
      if (maxSimulationTime < simulator->stats->simulationTimes[i])
	{
	  maxSimulationTime = simulator->stats->simulationTimes[i];
	}
      if (maxSteps < simSteps)
	{
	  maxSteps = simSteps;
	}
      if (minSteps > simSteps)
	{
	  minSteps = simSteps;
	}
      totalLPSteps += simulator->stats->steps[i];
      totalSimSteps += simSteps;
      totalTime += simulator->stats->simulationTimes[i];
      totalMessages += simulator->stats->simulationMessages[i];
      totalExternalEvents += simulator->stats->simulationExternalEvents[i];
    }
  avgSteps = totalSimSteps / lps;
  avgTime = totalTime / lps;
  avgStepCost = avgTime / avgSteps;
  pm = (simulator->stats->memory / 1024) / 1024;
  sm = (simulator->stats->sequentialMemory / 1024) / 1024;
  memoryIncrement = (pm - sm) / sm;
  SD_print (simulator->simulationLog, "Parallel Simulation Statistics:");
  SD_print (simulator->simulationLog, "");
  SD_print (simulator->simulationLog, "Simulation time: %g ms",
	    maxSimulationTime);
  SD_print (simulator->simulationLog, "Total Simulation transitions: %lu",
	    totalLPSteps);
  SD_print (simulator->simulationLog, "Initialization time: %g ms",
	    simulator->stats->initTime);
  SD_print (simulator->simulationLog, "Partitioning time: %g ms",
	    simulator->stats->partitioningTime);
  SD_print (simulator->simulationLog, "Initialize LPS time: %g ms",
	    simulator->stats->initializeLPS);
  SD_print (simulator->simulationLog, "Messages sent: %lu", totalMessages);
  SD_print (simulator->simulationLog, "External events: %lu",
	    totalExternalEvents);
  SD_print (
      simulator->simulationLog,
      "Processed messages: %.2f %%",
      (totalMessages > 0) ?
	  ((double) totalExternalEvents / (double) totalMessages) * 100 : 0);
  SD_print (simulator->simulationLog, "Estimated load (time): %g",
	    maxSimulationTime / avgTime);
  SD_print (simulator->simulationLog, "Estimated max load (steps): %g",
	    maxSteps / avgSteps);
  SD_print (simulator->simulationLog, "Estimated min load (steps): %g",
	    minSteps / avgSteps);
  SD_print (simulator->simulationLog, "Average step cost: %g", avgStepCost);
  SD_print (simulator->simulationLog, "Allocated memory: %.2lf MBytes", pm);
  SD_print (simulator->simulationLog,
	    "Additional memory allocated: %.2lf MBytes (%.2f %%)",
	    memoryIncrement * sm, (memoryIncrement / (double) lps) * 100);
#ifdef DEBUG
  QSS_generateWeights (simulator);
#endif
  SD_print (simulator->simulationLog, "");
}

void
QSS_PAR_printParallelLog (QSS_simulator simulator, int code)
{
  switch (code)
    {
    case PAR_ERR_CREATE_THREAD:
      SD_print (simulator->simulationLog, "LP %d: Error creating thread\n",
		simulator->id);
      break;
    case PAR_ERR_SET_AFFINITY_MASK:
      SD_print (simulator->simulationLog,
		"LP %d: WARNING: can not set affinity mask\n", simulator->id);
      break;
    case PAR_ERR_GET_AFFINITY_MASK:
      SD_print (simulator->simulationLog,
		"LP %d: WARNING: can not get affinity mask\n", simulator->id);
      break;
    default:
      break;
    }
}

void
QSS_PAR_allocRootSimulatorData (QSS_simulator simulator)
{
  QSS_data data = simulator->data;
  int lps = data->params->lps;
  simulator->lpTime = (double*) checkedMalloc (lps * sizeof(double));
  simulator->lpDtMin = (double*) checkedMalloc (lps * sizeof(double));
  simulator->mailbox = MLB_Mailbox (lps);
  SD_setStatisticsLPS (simulator->stats, lps);
  simulator->lps = QSS_LP_DataArray (lps);
  simulator->simulationLog = SD_SimulationLog (simulator->output->name);
#ifdef DEBUG
  if (simulator->settings->debug & SD_DBG_VarChanges)
    {
      SD_setSimulationLogVariables(simulator->simulationLog, data->states, data->events);
    }
#endif
  simulator->dtSynch = QSS_DtSynch (lps);
}

QSS_simulator
QSS_PAR_copySimulator (QSS_simulatorInstance *instance)
{
  QSS_simulator p = QSS_Simulator ();
  QSS_simulator root = instance->root;
  int id = instance->id;
  QSS_data data = root->data;
  char logFile[128];
  char logId[128];
  sprintf (logId, "%s-%d", root->output->name, id);
  sprintf (logFile, "%s", root->output->name);
  p->data = QSS_copyData (data);
  p->data->lp = QSS_LP_copyData (root->lps->lp[id]);
  QSS_LP_clean (root->lps->lp[id]);
  p->lpTime = root->lpTime;
  p->model = root->model;
  p->output = root->output;
  p->settings = root->settings;
  p->stats = SD_Statistics ();
  p->simulationLog = SD_SimulationLog (logId);
  SD_copySimulationLogVariables (p->simulationLog, root->simulationLog);
  p->time = QSS_Time (data->states, data->events, data->inputs, data->it,
		      root->time->scheduler, root->time->weights);
  p->dt = QSS_Dt (root->lpDtMin, id, root->dtSynch, logFile,
		  root->settings->debug, p->data, p->time);
  p->dtSynch = root->dtSynch;
  p->id = id;
  p->inbox = IBX_Inbox (p->data->lp->inStates, 0, p->data->lp->qMap);
  p->ack = IBX_Inbox (0, 1, NULL);
  root->mailbox->inbox[MSG_EVENT][id] = p->inbox;
  root->mailbox->inbox[MSG_ACK][id] = p->ack;
  p->mailbox = root->mailbox;
  return (p);
}

double
QSS_PAR_GVT (QSS_simulator simulator)
{
  int i;
  int lps = simulator->data->params->lps;
  double min = simulator->lpTime[0];
  for (i = 1; i < lps; i++)
    {
      if (simulator->lpTime[i] < min)
	{
	  min = simulator->lpTime[i];
	}
    }
  return (min);
}

void
QSS_PAR_removePendingMessages (QSS_simulator simulator)
{
  MLB_mailbox mailbox = simulator->mailbox;
  int id = simulator->id;
  QSS_LP_data lp = simulator->data->lp;
  int i, to, lps = lp->inStates;
  for (i = 0; i < lps; i++)
    {
      to = lp->qInMap[i];
      MLB_close (mailbox, to, id);
    }
  lps = lp->inEvents;
  for (i = 0; i < lps; i++)
    {
      to = lp->eInMap[i];
      MLB_close (mailbox, to, id);
    }
}

void
QSS_PAR_controlPassiveLPS (QSS_simulator simulator)
{
  int i;
  int lps = simulator->data->params->lps;
  for (i = 0; i < lps; i++)
    {
      if (simulator->lpTime[i]
	  != simulator->data->ft&& simulator->lpTime[i] != INF)
	{
	  return;
	}
    }
  for (i = 0; i < lps; i++)
    {
      if (simulator->lpTime[i] == simulator->data->ft)
	{
	  simulator->lpTime[i] = INF;
	}
    }
}

void
QSS_PAR_initializeSimulation (QSS_simulator simulator)
{
  getTime (simulator->stats->iTime);
  int i, j, k, s, forUL;
  double e, zc[4];
  QSS_CMD_init (simulator);
  simulator->simSteps = QSS_SimSteps ();
  simulator->frw = FRW_Framework (simulator->data);
  double t = simulator->time->time;
  // Local mappings.
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  FRW_framework frw = simulator->frw;
  QSS_LP_data lp = qssData->lp;
  double *q = qssData->q;
  double *x = qssData->x;
  const int order = qssData->order;
  const int coeffs = order + 1;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
#endif
  QSS_model qssModel = simulator->model;
  QA_quantizer quantizer;
  SD_output output = simulator->output;
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "Initialize simulation\n");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      if (lp->qMap[i] > NOT_ASSIGNED)
	{
	  qssData->lqu[i] = qssData->dQRel[i] * fabs (x[i * coeffs]);
	  if (qssData->lqu[i] < qssData->dQMin[i])
	    {
	      qssData->lqu[i] = qssData->dQMin[i];
	    }
#ifdef DEBUG
	  if (settings->debug & SD_DBG_InitValues)
	    {
	      SD_print (simulationLog, "Initial value: x[%d][0] = %g", i, x[i * coeffs]);
	    }
#endif
	}
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "Initialize solver...");
    }
#endif
  simulator->quantizer = QA_Quantizer (qssData, qssTime);
  quantizer = simulator->quantizer;
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize state derivatives...");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      if (lp->qMap[i] > NOT_ASSIGNED)
	{
	  FRW_recomputeDerivative (frw, qssModel, qssData, qssTime, i);
	  QA_nextTime (quantizer, i, t, qssTime->nextStateTime, x,
		       qssData->lqu);
#ifdef DEBUG
	  if (settings->debug & SD_DBG_InitValues)
	    {
	      SD_print (simulationLog, "Initial derivative: x[%d][1] = %g", i,
		  x[i * coeffs + 1]);
	    }
#endif
	}
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize input...");
    }
#endif
  forUL = qssData->inputs;
  for (i = 0; i < forUL; i++)
    {
      j = qssData->TD[i];
      if (lp->qMap[j] > NOT_ASSIGNED)
	{
	  FRW_nextInputTime (frw, qssModel, qssData, qssTime, 0, j, i);
	}
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize events...");
    }
#endif
  if (lp->events + lp->inEvents > 0)
    {
      forUL = qssData->events;
      for (i = 0; i < forUL; i++)
	{
	  if (lp->eMap[i] > NOT_ASSIGNED)
	    {
	      if (qssData->nZS[i])
		{
		  int nZS = qssData->nZS[i];
		  e = INF;
		  for (j = 0; j < nZS; j++)
		    {
		      k = qssData->ZS[i][j];
		      if (qssData->dQMin[k] < e)
			{
			  e = qssData->dQMin[k];
			}
		    }
		}
	      else
		{
		  e = qssData->ft * qssData->params->zcHyst;
		}
	      qssModel->events->zeroCrossing (i, q, qssData->d, qssData->alg,
					      qssTime->time, zc);
	      s = sign (zc[0]);
	      qssData->event[i].zcSign = s;
	      qssData->event[i].zcHyst = e / 10.0;
	      FRW_nextEventTime (frw, qssModel, qssData, qssTime, i);
	    }
	}
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize state variables time...");
    }
#endif
  forUL = qssData->states;
  for (i = 0; i < forUL; i++)
    {
      if (lp->qMap[i] > NOT_ASSIGNED)
	{
	  QA_recomputeNextTime (quantizer, i, qssTime->time,
				qssTime->nextStateTime, x, qssData->lqu, q);
	}
    }
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize output...");
    }
#endif
  simulator->log = OUT_Output (qssData, qssTime, output);
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
      SD_print (simulationLog, "Initialize time...");
    }
#endif
  simulator->scheduler = SC_Scheduler (qssData, qssTime);
  simulator->lpTime[simulator->id] = qssTime->time;
#ifdef DEBUG
  if (settings->debug & SD_DBG_InitValues)
    {
      SD_print (simulationLog, "done.");
    }
#endif
  getTime (simulator->stats->sTime);
  subTime (simulator->stats->sTime, simulator->stats->iTime);
  simulator->stats->initTime += getTimeValue (simulator->stats->sTime);
}

void
QSS_PAR_logMemory (QSS_simulator simulator)
{
  QSS_data data = simulator->data;
  SD_output output = simulator->output;
  int xOrder = data->order + 1, states = data->states, events = data->events,
      sDouble = sizeof(double), sInt = sizeof(int), discretes = data->discretes,
      outputs = output->outputs, algs = data->algs, inputs = data->inputs;
  simulator->stats->memory += (3 * states + 4 * states * xOrder + algs * xOrder
      + discretes) * sDouble + (4 * states + inputs) * sInt;
  if (events)
    {
      simulator->stats->memory += (6 * events + 2 * states) * sInt;
      simulator->stats->memory += events * sizeof(struct SD_eventData_);
    }
  int i;
  for (i = 0; i < states; i++)
    {
      simulator->stats->memory += data->nSD[i] * sInt;
      simulator->stats->memory += data->nDS[i] * sInt;
      if (events)
	{
	  simulator->stats->memory += data->nSZ[i] * sInt;
	}
    }
  for (i = 0; i < events; i++)
    {
      simulator->stats->memory += data->nZS[i] * sInt;
      simulator->stats->memory += data->nHD[i] * sInt;
      simulator->stats->memory += data->nHZ[i] * sInt;
      simulator->stats->memory += data->event[i].nLHSSt * sInt;
      simulator->stats->memory += data->event[i].nLHSDsc * sInt;
      simulator->stats->memory += data->event[i].nRHSSt * sInt;
    }
  simulator->stats->memory += (3 * states + events + inputs) * sDouble;
  if (simulator->time->weights != NULL)
    {
      simulator->stats->memory += events * sDouble;
    }
  if (discretes)
    {
      simulator->stats->memory += 2 * discretes * sInt;
    }
  if (states)
    {
      simulator->stats->memory += 2 * states * sInt;
    }
  if (outputs)
    {
      simulator->stats->memory += 4 * outputs * sInt;
      for (i = 0; i < states; i++)
	{
	  simulator->stats->memory += output->nSO[i] * sInt;
	}
      for (i = 0; i < discretes; i++)
	{
	  simulator->stats->memory += output->nDO[i] * sInt;
	}
      for (i = 0; i < outputs; i++)
	{
	  simulator->stats->memory += output->nOS[i] * sInt;
	  simulator->stats->memory += output->nOD[i] * sInt;
	}
      int nOutputs = simulator->output->outputs;
      if (nOutputs > 0)
	{
	  int j = 0;
	  for (j = 0; j < nOutputs; j++)
	    {
	      int nodes = 1;
	      simulator->stats->memory += (2 * data->params->nodeSize
		  + nodes * data->params->nodeSize * 2) * sDouble;
	    }
	}
    }
  int treeH = 0, treeS = 0, treeE = 0, treeI = 0, max = states, treeT = 0;
  if (states)
    {
      treeH = (int) LOG2(states);
      treeS = (pow (2, treeH + 1) - 1 + states) * sDouble;
      treeT = treeS * sDouble;
    }
  if (events)
    {
      treeH = (int) LOG2(events);
      treeE = (pow (2, treeH + 1) - 1 + events) * sDouble;
      treeT += treeE;
      if (events > max)
	{
	  max = events;
	}
    }
  if (inputs)
    {
      treeH = (int) LOG2(inputs);
      treeI = (pow (2, treeH + 1) - 1 + inputs) * sDouble;
      treeT += treeI;
      if (inputs > max)
	{
	  max = inputs;
	}
    }
  if (max == states)
    {
      treeT += treeS;
    }
  else if (max == events)
    {
      treeT += treeE;
    }
  else
    {
      treeT += treeI;
    }
  int solverM = 0;
  switch (simulator->data->solver)
    {
    case SD_CQSS:
      solverM = 3 * states * sDouble;
      break;
    case SD_LIQSS:
      solverM = 6 * states * sDouble;
      break;
    case SD_LIQSS2:
      solverM = 12 * states * sDouble;
      break;
    case SD_LIQSS3:
      solverM = 12 * states * sDouble;
      break;
    default:
      break;
    }
  simulator->stats->sequentialMemory = simulator->stats->memory + treeT
      + solverM;
  int lps = simulator->data->params->lps;
  int sMessage = sizeof(IBX_message);
  for (i = 0; i < lps; i++)
    {
      QSS_LP_data lp = simulator->lps->lp[i];
      simulator->stats->memory += (lp->totalStates + lp->totalEvents
	  + lp->totalOutputs + lp->states + lp->events + lp->inputs
	  + lp->outputs + lp->inStates + lp->inEvents) * sInt;
      simulator->stats->memory += (4 * states * xOrder + states + algs * xOrder
	  + discretes) * sDouble;
      int treeH = 0, treeS = 0, treeE = 0, treeI = 0, max = states, treeT = 0;
      if (lp->states)
	{
	  treeH = (int) LOG2(lp->states);
	  treeS = (pow (2, treeH + 1) - 1 + states) * sDouble;
	  treeT = treeS;
	}
      if (lp->events)
	{
	  treeH = (int) LOG2(lp->events);
	  treeE = (pow (2, treeH + 1) - 1 + events) * sDouble;
	  treeT += treeE;
	  if (events > max)
	    {
	      max = events;
	    }
	}
      if (lp->inputs)
	{
	  treeH = (int) LOG2(lp->inputs);
	  treeI = (pow (2, treeH + 1) - 1 + inputs) * sDouble;
	  treeT += treeI;
	  if (inputs > max)
	    {
	      max = inputs;
	    }
	}
      if (max == states)
	{
	  treeT += treeS;
	}
      else if (max == events)
	{
	  treeT += treeE;
	}
      else
	{
	  treeT += treeI;
	}
      simulator->stats->memory += treeT + solverM;
    }
  simulator->stats->memory += (states * sDouble + 64 * sMessage
      + 10000 * sMessage + 8) * lps;
}

void
QSS_PAR_initialize (SIM_simulator simulate)
{
  QSS_simulator simulator = (QSS_simulator) simulate->state->sim;
  QSS_PAR_allocRootSimulatorData (simulator);
  getTime (simulator->stats->iTime);
  PRT_partition partition = PRT_Partition (simulator->data,
					   simulator->output->name);
  getTime (simulator->stats->sdTime);
  subTime (simulator->stats->sdTime, simulator->stats->iTime);
  simulator->stats->partitioningTime = getTimeValue (simulator->stats->sdTime);
  getTime (simulator->stats->iTime);
  LP_initializeDataStructs (simulator, partition);
  getTime (simulator->stats->sdTime);
  subTime (simulator->stats->sdTime, simulator->stats->iTime);
  PRT_freePartition (partition);
  QSS_PAR_logMemory (simulator);
  simulator->stats->initializeLPS = getTimeValue (simulator->stats->sdTime);
}

void
QSS_PAR_sendSimultaneousMessages (QSS_simulator simulator,
				  QSS_externaEventHandler externalEvent,
				  QSS_internalEventHandler internalEvent)
{
  QSS_simSteps simSteps = simulator->simSteps;
  QSS_stepInfo it = QSS_SIS_first (simSteps);
  QSS_LP_data lp = simulator->data->lp;
  MLB_mailbox mailbox = simulator->mailbox;
  int lpsBegin = lp->nLPS[it.synch];
  int lpsEnd = lp->nLPS[it.synch + 1];
  int i;
  for (i = lpsBegin; i < lpsEnd; i++)
    {
      MLB_send (mailbox, lp->lps[i], simulator->id, it.msg);
    }
  simulator->time->minIndex = it.msg.index;
  simulator->time->type = it.msg.type;
  simulator->time->time = it.msg.time;
  QSS_PAR_synchronize (simulator, it.synch, externalEvent, internalEvent);
}

void
QSS_PAR_synchronize (QSS_simulator simulator, int synchronize,
		     QSS_externaEventHandler externalEvent,
		     QSS_internalEventHandler internalEvent)
{
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  SC_scheduler scheduler = simulator->scheduler;
  QSS_LP_data lp = qssData->lp;
  word_t nLPS = lp->nLPS[synchronize + 1] - lp->nLPS[synchronize];
  IBX_inbox inbox = simulator->inbox;
  IBX_inbox ack = simulator->ack;
  MLB_mailbox mailbox = simulator->mailbox;
  QSS_dt dt = simulator->dt;
  int index = qssTime->minIndex;
  int type = qssTime->type;
  double stepTime = qssTime->time;
  int id = simulator->id;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
  if (settings->debug & SD_DBG_Synchronize)
    {
      SD_print (simulationLog, "LP %d synchronize variable = %d, type = %d, localTime = %.16lf, globalTime = %.16lf",id, qssTime->minIndex, qssTime->type, qssTime->time, simulator->lpTime[id]);
    }
#endif
  while (IBX_ackMessages (ack) < nLPS)
    {
      IBX_checkAckInbox (inbox, mailbox, id);
      double nextMessageTime = IBX_nextMessageTime (inbox);
      if (nextMessageTime <= qssTime->time)
	{
	  externalEvent (simulator, IBX_nextMessage (inbox));
	  SC_update (scheduler, qssData, qssTime);

	  while (qssTime->time == stepTime && qssTime->minIndex != index)
	    {
	      internalEvent (simulator);
	      SC_update (scheduler, qssData, qssTime);
	    }
	  qssTime->time = stepTime;
	  qssTime->minValue = stepTime;
	  qssTime->minIndex = index;
	  qssTime->type = type;
	}
      QSS_dtCheck (dt);
    }
  IBX_reset (ack);
  if (!QSS_SIS_empty (simulator->simSteps))
    {
      QSS_PAR_sendSimultaneousMessages (simulator, externalEvent,
					internalEvent);
      if (QSS_SIS_empty (simulator->simSteps))
	{
	  QSS_SIS_reset (simulator->simSteps);
	  qssTime->time = stepTime;
	  qssTime->minValue = stepTime;
	  qssTime->minIndex = index;
	  qssTime->type = type;
	}
    }
}

double
QSS_PAR_passiveInitialization (QSS_simulator simulator,
			       QSS_externaEventHandler externalEvent)
{
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  QSS_LP_data lp = qssData->lp;
  int id = simulator->id;
  double t = qssTime->time;
  const double ft = qssData->ft;
  if (t == ft && lp->inStates == 0 && lp->inEvents == 0)
    {
      simulator->lpTime[id] = INF;
      getTime (simulator->stats->sTime);
      subTime (simulator->stats->sTime, simulator->stats->iTime);
      simulator->stats->simulationTime = getTimeValue (simulator->stats->sTime);
      PAR_cleanLPTask (id);
      return (t);
    }
  return (QSS_PAR_passiveLP (simulator, externalEvent));
}

double
QSS_PAR_passiveLP (QSS_simulator simulator,
		   QSS_externaEventHandler externalEvent)
{
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  IBX_inbox inbox = simulator->inbox;
  SC_scheduler scheduler = simulator->scheduler;
  QSS_dt dt = simulator->dt;
  QSS_LP_data lp = qssData->lp;
  int id = simulator->id;
  unsigned long advanceControl = qssData->states + qssData->events, sample = 0;
  double t = qssTime->time;
  double nextMessageTime;
  const double ft = qssData->ft;
  double gvt = QSS_PAR_GVT (simulator);
  while (t == ft && gvt < ft)
    {
      IBX_checkInbox (inbox);
      nextMessageTime = IBX_nextMessageTime (inbox);
      if (nextMessageTime != INF)
	{
	  externalEvent (simulator, IBX_nextMessage (inbox));
	  SC_update (scheduler, qssData, qssTime);
	  nextMessageTime = IBX_nextMessageTime (inbox);
	  if (nextMessageTime < qssTime->time)
	    {
	      lp->externalEvent = TRUE;
	      t = nextMessageTime;
	    }
	  else
	    {
	      lp->externalEvent = FALSE;
	      t = qssTime->time;
	    }
	  simulator->lpTime[id] = t;

	  QSS_dtCheck (dt);
	  if (t != ft)
	    {
	      break;
	    }
	}
      sample++;
      if (sample == advanceControl)
	{
	  t = simulator->lpTime[id];
	  gvt = QSS_PAR_GVT (simulator);
	  sample = 0;
	}
      QSS_dtCheck (dt);
    }
  return (t);
}

void
QSS_PAR_waitFor (QSS_simulator simulator)
{
  QSS_dtFinish (simulator->dt);
  pthread_mutex_t *m = &(simulator->dtSynch->activeMutex);
  pthread_mutex_lock (m);
  simulator->dtSynch->activeLPS--;
  pthread_mutex_unlock (m);
  while (TRUE)
    {
      pthread_mutex_lock (m);
      if (simulator->dtSynch->activeLPS == 0)
	{
	  pthread_mutex_unlock (m);
	  return;
	}
      pthread_mutex_unlock (m);
      QSS_dtCheck (simulator->dt);
    }
}

#else
void
QSS_PAR_saveLog (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_printSimulationLog (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_printParallelLog (QSS_simulator simulator, int code)
  {
    return;
  }

void
QSS_PAR_allocRootSimulatorData (QSS_simulator simulator)
  {
    return;
  }

QSS_simulator
QSS_PAR_copySimulator (QSS_simulatorInstance *instance)
  {
    return (NULL);
  }

double
QSS_PAR_GVT (QSS_simulator simulator)
  {
    return (0);
  }

void
QSS_PAR_removePendingMessages (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_controlPassiveLPS (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_initializeSimulation (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_logMemory (QSS_simulator simulator)
  {
    return;
  }

void
QSS_PAR_initialize (SIM_simulator simulate)
  {
    return;
  }

void
QSS_PAR_synchronize (QSS_simulator simulator, int synchronize, QSS_externaEventHandler externalEvent, QSS_internalEventHandler internalEvent)
  {
    return;
  }

void
QSS_PAR_waitFor(QSS_simulator simulator)
  {

  }

#endif

