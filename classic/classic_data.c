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

#include "classic_data.h"

#include <stdlib.h>
#include <string.h>

#include "../common/settings.h"

CLC_event
CLC_Event(CLC_zc zeroCrossing, CLC_hnd handlerPos, CLC_hnd handlerNeg)
{
  CLC_event p = checkedMalloc(sizeof(*p));
  p->zeroCrossing = zeroCrossing;
  p->handlerPos = handlerPos;
  p->handlerNeg = handlerNeg;
  return p;
}

void
CLC_freeEvent(CLC_event events)
{
  free(events);
}

CLC_data
CLC_Data(int states, int discretes, int events, int inputs, int algebraics,
    string name)
{
  char fileName[256];
  strcpy(fileName, name);
  strcat(fileName, ".ini");
  SET_settings settings = SET_Settings(fileName);
  CLC_data p = checkedMalloc(sizeof(*p));
  int i;
  p->states = states;
  p->discretes = discretes;
  p->events = events;
  p->inputs = inputs;
  p->solver = settings->solver;
  p->dQMin = (double*) malloc(states * sizeof(double));
  p->dQRel = (double*) malloc(states * sizeof(double));
  if(discretes > 0)
  {
    p->d = (double*) malloc(discretes * sizeof(double));
  }
  else
  {
    p->d = NULL;
  }
  p->x = (double*) malloc(states * sizeof(double));
  if(algebraics > 0)
  {
    p->alg = (double*) malloc(algebraics * sizeof(double));
  }
  else
  {
    p->alg = NULL;
  }
  p->it = settings->it;
  p->ft = settings->ft;
  p->nSD = (int*) malloc(states * sizeof(int));
  p->nDS = (int*) malloc(states * sizeof(int));
  p->SD = (int**) malloc(states * sizeof(int*));
  p->DS = (int**) malloc(states * sizeof(int*));
  if(inputs > 0)
  {
    p->IT = (int*) malloc(inputs * sizeof(int));
  }
  else
  {
    p->IT = NULL;
  }
  cleanDoubleVector(p->d, 0, discretes);
  cleanDoubleVector(p->x, 0, states);
  cleanVector(p->nSD, 0, states);
  cleanVector(p->nDS, 0, states);
  double sameDQMin = (settings->nDQMin == 1) ? settings->dqmin[0] : 0;
  double sameDQRel = (settings->nDQRel == 1) ? settings->dqrel[0] : 0;
  for(i = 0; i < states; i++)
  {
    p->nSD[i] = 0;
    p->dQMin[i] = (sameDQMin > 0) ? sameDQMin : settings->dqmin[i];
    p->dQRel[i] = (sameDQRel > 0) ? sameDQRel : settings->dqrel[i];
  }
  p->event = SD_EventData(events);
  p->params = SD_Parameters(settings->derdelta, settings->zchyst,
      settings->minstep, settings->symdiff, settings->lps, settings->nodesize,
      settings->pm,
      settings->dt, settings->dtSynch, settings->partitionerOptions,
      settings->jacobian);
  p->scalarEvaluations = 0;
  p->zeroCrossings = 0;
  p->funEvaluations = 0;
  p->totalOutputSteps = 0;
  p->totalJacobians = 0;
  p->totalCrossingEvaluations = 0;
  p->totalStepsDASSL = 0;
  p->totalSteps = 0;
  p->totalEvents = 0;
  freeSettings(settings);
  if(events)
  {
    p->fired = (char*) malloc(events);
  }
  return p;
}

void
CLC_freeData(CLC_data data)
{
  int i, states = data->states;
  free(data->dQMin);
  free(data->dQRel);
  if(data->alg != NULL)
  {
    free(data->alg);
  }
  if(data->discretes > 0)
  {
    free(data->d);
  }
  free(data->x);
  free(data->nSD);
  free(data->nDS);
  for(i = 0; i < states; i++)
  {
    if(data->SD[i] != NULL)
      free(data->SD[i]);
    if(data->DS[i] != NULL)
      free(data->DS[i]);
  }
  free(data->SD);
  free(data->DS);
  if(data->inputs > 0)
  {
    free(data->IT);
  }
  SD_freeEventData(data->event, data->events);
  SD_freeParameters(data->params);
  if(data->events)
  {
    free(data->fired);
  }
  free(data);
}

CLC_model
CLC_Model(CLC_eq f, CLC_zc zeroCrossing, CLC_hnd handlerPos, CLC_hnd handlerNeg,
    CLC_jac jac)
{
  CLC_model p = checkedMalloc(sizeof(*p));
  p->f = f;
  p->jac = jac;
  p->events = CLC_Event(zeroCrossing, handlerPos, handlerNeg);
  return p;
}

void
CLC_freeModel(CLC_model model)
{
  free(model->events);
  free(model);
}

void
CLC_allocDataMatrix(CLC_data data)
{
  int i, states = data->states;
  for(i = 0; i < states; i++)
  {
    data->SD[i] =
        (data->nSD[i] > 0) ? (int*) malloc(data->nSD[i] * sizeof(int)) : NULL;
    data->DS[i] =
        (data->nDS[i] > 0) ? (int*) malloc(data->nDS[i] * sizeof(int)) : NULL;
  }
}

