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

#include "data.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

SD_simulationSettings SD_SimulationSettings()
{
  SD_simulationSettings p = checkedMalloc(sizeof(*p));
  p->debug = SD_DBG_NoDebug;
  p->parallel = FALSE;
  p->hybrid = FALSE;
  p->method = SD_QSS3;
  return p;
}

SD_simulationSettings SD_copySimulationSettings(SD_simulationSettings settings)
{
  SD_simulationSettings p = SD_SimulationSettings();
  p->debug = settings->debug;
  p->parallel = settings->parallel;
  p->hybrid = settings->hybrid;
  p->method = settings->method;
  p->jacobian = settings->jacobian;
  return p;
}

void SD_freeSimulationSettings(SD_simulationSettings settings) { free(settings); }

SD_simulationLog SD_SimulationLog(char *name)
{
  char logFile[128];
  sprintf(logFile, "%s.log", name);
  SD_simulationLog p = checkedMalloc(sizeof(*p));
  p->states = NULL;
  p->handlers = NULL;
  p->hasVariables = FALSE;
  p->log = fopen(logFile, "w");
  if (!p->log) {
    fprintf(stderr, "Error: Can't open log file.");
  }
  return p;
}

void SD_freeSimulationLog(SD_simulationLog log)
{
  if (log->states != NULL && log->hasVariables == TRUE) {
    free(log->states);
  }
  if (log->handlers && log->hasVariables == TRUE) {
    free(log->handlers);
  }
  fclose(log->log);
  free(log);
}

void SD_copySimulationLogVariables(SD_simulationLog log, SD_simulationLog orig)
{
  log->states = orig->states;
  log->handlers = orig->handlers;
}

void SD_setSimulationLogVariables(SD_simulationLog log, int states, int events)
{
  log->states = (int *)malloc(states * sizeof(int));
  cleanVector(log->states, 0, states);
  log->handlers = NULL;
  if (events) {
    log->handlers = (int *)malloc(events * sizeof(int));
    cleanVector(log->handlers, 0, events);
  }
  log->hasVariables = TRUE;
}

void SD_print(SD_simulationLog log, const char *message, ...)
{
  va_list ap;
  va_start(ap, message);
  vfprintf(log->log, message, ap);
  va_end(ap);
  fprintf(log->log, "\n");
}

SD_eventData SD_EventData(int size)
{
  SD_eventData p = checkedMalloc(size * sizeof(*p));
  int i;
  for (i = 0; i < size; i++) {
    p[i].nLHSSt = 0;
    p[i].nLHSDsc = 0;
    p[i].nRHSSt = 0;
    p[i].zcSign = 0;
    p[i].direction = 0;
    p[i].LHSSt = NULL;
    p[i].LHSDsc = NULL;
    p[i].RHSSt = NULL;
    p[i].zcHyst = 0;
  }
  return p;
}

SD_eventData SD_copyEventData(int size, SD_eventData events)
{
  SD_eventData p = checkedMalloc(size * sizeof(*p));
  int i;
  for (i = 0; i < size; i++) {
    p[i].nLHSSt = events[i].nLHSSt;
    p[i].nLHSDsc = events[i].nLHSDsc;
    p[i].nRHSSt = events[i].nRHSSt;
    p[i].zcSign = events[i].zcSign;
    p[i].direction = events[i].direction;
    p[i].LHSSt = NULL;
    p[i].LHSDsc = NULL;
    p[i].RHSSt = NULL;
    p[i].zcHyst = events[i].zcHyst;
  }
  return p;
}

void SD_freeEventData(SD_eventData events, int size)
{
  if (events == NULL) {
    return;
  }
  int i;
  for (i = 0; i < size; i++) {
    if (events[i].nLHSSt) {
      free(events[i].LHSSt);
    }
    if (events[i].nLHSDsc) {
      free(events[i].LHSDsc);
    }
    if (events[i].nRHSSt) {
      free(events[i].RHSSt);
    }
  }
  free(events);
}

void SD_cleanEventData(SD_eventData events, int size)
{
  if (events == NULL) {
    return;
  }
  free(events);
}

SD_parameters SD_Parameters(double derDelta, double zcHyst, double minStep, int symDiff, int lps, int nodeSize, SD_PartitionMethod pm,
                            double dt, SD_DtSynch synch, SD_partitionerOptions partitionerOptions, int jacobian)
{
  SD_parameters p = checkedMalloc(sizeof(*p));
  p->derDelta = derDelta;
  p->zcHyst = zcHyst;
  p->minStep = minStep;
  p->symDiff = symDiff;
  p->lps = lps;
  p->nodeSize = nodeSize;
  p->pm = pm;
  p->dt = dt;
  p->dtSynch = synch;
  p->partitionerOptions = partitionerOptions;
  p->jacobian = jacobian;
  return p;
}

SD_parameters SD_copyParameters(SD_parameters parameters)
{
  SD_parameters p = checkedMalloc(sizeof(*p));
  p->derDelta = parameters->derDelta;
  p->zcHyst = parameters->zcHyst;
  p->minStep = parameters->minStep;
  p->symDiff = parameters->symDiff;
  p->lps = parameters->lps;
  p->nodeSize = parameters->nodeSize;
  p->pm = parameters->pm;
  p->dt = parameters->dt;
  return p;
}

void SD_freeParameters(SD_parameters params) { free(params); }

SD_sampledOutput SD_SampledOutput(int outputs, double it, double *period, int nPeriod)
{
  int i;
  SD_sampledOutput p = checkedMalloc(sizeof(*p));
  p->nextTime = (double *)malloc(outputs * sizeof(double));
  p->period = (double *)malloc(outputs * sizeof(double));
  if (nPeriod == outputs) {
    for (i = 0; i < outputs; i++) {
      p->nextTime[i] = it;
      p->period[i] = period[i];
    }
  } else {
    for (i = 0; i < outputs; i++) {
      p->nextTime[i] = it;
      p->period[i] = period[0];
    }
  }
  return p;
}

void SD_freeSampledOutput(SD_sampledOutput output)
{
  free(output->nextTime);
  free(output->period);
  free(output);
}

SD_outputVariable SD_OutputVariable(int outputs)
{
  SD_outputVariable p = checkedMalloc(outputs * sizeof(*p));
  return p;
}

void SD_freeOutputVariable(SD_outputVariable variable) { free(variable); }

SD_output SD_Output(string name, int outputs, int discretes, int states, double *period, int nPeriod, double it,
                    SD_CommInterval commInterval, SD_StoreData store, SD_eq value)
{
  SD_output p = checkedMalloc(sizeof(*p));
  p->name = name;
  p->outputs = outputs;
  p->commInterval = commInterval;
  p->store = store;
  p->sampled = NULL;
  p->nSO = NULL;
  p->SO = NULL;
  p->nDO = NULL;
  p->DO = NULL;
  p->nOS = NULL;
  p->OS = NULL;
  p->nOD = NULL;
  p->OD = NULL;
  if (outputs) {
    if (discretes) {
      p->nDO = (int *)malloc(discretes * sizeof(int));
      cleanVector(p->nDO, 0, discretes);
      p->DO = (int **)malloc(discretes * sizeof(int *));
    }
    if (states) {
      p->nSO = (int *)malloc(states * sizeof(int));
      cleanVector(p->nSO, 0, states);
      p->SO = (int **)malloc(states * sizeof(int *));
    }
    if (outputs) {
      p->nOS = (int *)malloc(outputs * sizeof(int));
      cleanVector(p->nOS, 0, outputs);
      p->OS = (int **)malloc(outputs * sizeof(int *));
      p->nOD = (int *)malloc(outputs * sizeof(int));
      cleanVector(p->nOD, 0, outputs);
      p->OD = (int **)malloc(outputs * sizeof(int *));
    }
    p->commInterval = commInterval;
    p->store = store;
    switch (commInterval) {
    case CI_Step:
      p->sampled = NULL;
      break;
    case CI_Sampled:
      p->sampled = SD_SampledOutput(outputs, it, period, nPeriod);
      break;
    case CI_Dense:
      break;
    }
    p->variable = SD_OutputVariable(outputs);
    p->value = value;
  }
  return p;
}

void SD_allocOutputMatrix(SD_output output, int states, int discretes)
{
  if (states) {
    assert(output->nSO != NULL);
  }
  if (discretes) {
    assert(output->nDO != NULL);
  }
  int i, outputs = output->outputs;
  if (outputs) {
    assert(output->nOS != NULL);
    assert(output->nOD != NULL);
  }
  for (i = 0; i < states; i++) {
    output->SO[i] = (output->nSO[i] > 0) ? (int *)malloc(output->nSO[i] * sizeof(int)) : NULL;
  }
  for (i = 0; i < discretes; i++) {
    output->DO[i] = (output->nDO[i] > 0) ? (int *)malloc(output->nDO[i] * sizeof(int)) : NULL;
  }
  for (i = 0; i < outputs; i++) {
    output->OS[i] = (output->nOS[i] > 0) ? (int *)malloc(output->nOS[i] * sizeof(int)) : NULL;
    output->OD[i] = (output->nOD[i] > 0) ? (int *)malloc(output->nOD[i] * sizeof(int)) : NULL;
  }
}

void SD_freeOutput(SD_output output, int states, int discretes)
{
  int i, outputs = output->outputs;
  if (outputs) {
    if (discretes) {
      free(output->nDO);
    }
    if (states) {
      free(output->nSO);
    }
    if (outputs) {
      free(output->nOS);
      free(output->nOD);
    }
    for (i = 0; i < discretes; i++) {
      if (output->DO[i] != NULL) {
        free(output->DO[i]);
      }
    }
    for (i = 0; i < states; i++) {
      if (output->SO[i] != NULL) {
        free(output->SO[i]);
      }
    }
    for (i = 0; i < outputs; i++) {
      if (output->OS[i] != NULL) {
        free(output->OS[i]);
      }
      if (output->OD[i] != NULL) {
        free(output->OD[i]);
      }
    }
    if (discretes) {
      free(output->DO);
    }
    if (states) {
      free(output->SO);
    }
    if (outputs) {
      free(output->OS);
      free(output->OD);
    }
    if (output->sampled) {
      SD_freeSampledOutput(output->sampled);
    }
    SD_freeOutputVariable(output->variable);
  }
  free(output);
}

SD_statistics SD_Statistics()
{
  SD_statistics p = checkedMalloc(sizeof(*p));
  p->initializeLPS = 0;
  p->partitioningTime = 0;
  p->simulationTimes = NULL;
  p->simulationMessages = NULL;
  p->simulationExternalEvents = NULL;
  p->steps = NULL;
  p->totalSteps = 1;
  p->memory = 0;
  p->reinits = 0;
  p->modelEvaluations = 0;
  p->iTime = checkedMalloc(sizeof(*(p->iTime)));
  p->sTime = checkedMalloc(sizeof(*(p->sTime)));
  p->sdTime = checkedMalloc(sizeof(*(p->sdTime)));
  p->initTime = 0;
  p->simulationTime = 0;
  p->saveTime = 0;
  p->extTrans = 0;
  p->pastEvents = 0;
  p->messages = 0;
  return p;
}

void SD_freeStatistics(SD_statistics stats)
{
  if (stats->simulationTimes != NULL) {
    free(stats->simulationTimes);
  }
  if (stats->simulationExternalEvents != NULL) {
    free(stats->simulationExternalEvents);
  }
  if (stats->simulationMessages != NULL) {
    free(stats->simulationMessages);
  }
  if (stats->steps != NULL) {
    free(stats->steps);
  }
  free(stats->iTime);
  free(stats->sTime);
  free(stats->sdTime);
  free(stats);
}

void SD_setStatisticsLPS(SD_statistics stats, int lps)
{
  stats->simulationTimes = checkedMalloc(lps * sizeof(double));
  cleanDoubleVector(stats->simulationTimes, 0, lps);
  stats->simulationMessages = checkedMalloc(lps * sizeof(unsigned long));
  stats->simulationExternalEvents = checkedMalloc(lps * sizeof(unsigned long));
  stats->steps = checkedMalloc(lps * sizeof(int));
  cleanVector(stats->steps, 0, lps);
}

SD_jacMatrix SD_JacMatrix(int variables)
{
  SD_jacMatrix p = checkedMalloc(sizeof(*p));
  p->variables = variables;
  p->size = (int *)malloc(variables * sizeof(int));
  cleanDoubleVector(p->size, 0, variables);
  p->index = (int **)malloc(variables * sizeof(int *));
  p->value = (double **)malloc(variables * sizeof(double *));
  return p;
}

void SD_allocJacMatrix(SD_jacMatrix jac_matrix)
{
  int i, variables = jac_matrix->variables;
  for (i = 0; i < variables; i++) {
    jac_matrix->index[i] = (jac_matrix->size[i] > 0) ? (int *)malloc(jac_matrix->size[i] * sizeof(int)) : NULL;
    jac_matrix->value[i] = (jac_matrix->size[i] > 0) ? (double *)malloc(jac_matrix->size[i] * sizeof(int)) : NULL;
  }
}

void SD_freeJacMatrix(SD_jacMatrix jac_matrix)
{
  int i, variables = jac_matrix->variables;
  free(jac_matrix->size);
  for (i = 0; i < variables; i++) {
    free(jac_matrix->index[i]);
  }
  free(jac_matrix->index);
  for (i = 0; i < variables; i++) {
    free(jac_matrix->value[i]);
  }
  free(jac_matrix->value);
  free(jac_matrix);
}

SD_jacMatrices SD_JacMatrices(int states, int algs)
{
  SD_jacMatrices p = checkedMalloc(sizeof(*p));
  p->df_dx = SD_JacMatrix(states);
  p->dg_dx = SD_JacMatrix(algs);
  return p;
}

void SD_allocJacMatrices(SD_jacMatrices jac_matrices)
{
  SD_allocJacMatrix(jac_matrices->df_dx);
  SD_allocJacMatrix(jac_matrices->dg_dx);
}

void SD_freeJacMatrices(SD_jacMatrices jac_matrices)
{
  SD_freeJacMatrix(jac_matrices->df_dx);
  SD_freeJacMatrix(jac_matrices->df_dx);
  free(jac_matrices);
}