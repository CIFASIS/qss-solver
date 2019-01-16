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

#include "qss_data.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/settings.h"

static bool QSS_allocBuffer = FALSE;
static bool QSS_hardCopyStruct = FALSE;
static bool QSS_sharedState = FALSE;

void QSS_setReinitBuffer(bool b) { QSS_allocBuffer = b; }

QSS_reinit QSS_Reinit() {
  if (QSS_allocBuffer == FALSE) {
    return NULL;
  }
  QSS_reinit p = checkedMalloc(sizeof(*p));
  int i;
  for (i = 0; i < QSS_REINIT_BUFFER; i++) {
    p->variable[i] = NOT_ASSIGNED;
  }
  p->time = INF;
  p->counter = 0;
  return p;
}

void QSS_freeReinit(QSS_reinit reinit) { free(reinit); }

QSS_LP_data QSS_LP_Data(int states, int events, int inputs, int outputs,
                        int inStates, int inEvents, int totalStates,
                        int totalEvents, int totalOutputs, int nLPS, int lps,
                        int id) {
  QSS_LP_data p = checkedMalloc(sizeof(*p));
  p->nLPSCount = nLPS;
  p->lpsCount = lps;
  p->totalStates = totalStates;
  p->totalEvents = totalEvents;
  p->totalOutputs = totalOutputs;
  p->states = states;
  p->events = events;
  p->inputs = inputs;
  p->outputs = outputs;
  p->outStates = 0;
  p->inStates = inStates;
  p->inEvents = inEvents;
  p->initDt = 0;
  p->dscInf = 0;
  p->id = id;
  p->nLPS = (int *)malloc((nLPS + 1) * sizeof(int));
  p->lps = (int *)malloc(lps * sizeof(int));
  p->qMap = (QSS_idxMap)malloc(totalStates * sizeof(int));
  p->qInMap = (QSS_idxMap)malloc(inStates * sizeof(int));
  p->qOutMap = (QSS_idxMap)malloc(states * sizeof(int));
  p->dscMap = NULL;
  p->externalEvent = FALSE;
  if (events + inEvents > 0) {
    p->eMap = (QSS_idxMap)malloc(totalEvents * sizeof(*p->eMap));
    p->eInMap = (QSS_idxMap)malloc(inEvents * sizeof(*p->eInMap));
    p->eOutMap = (QSS_idxMap)malloc(events * sizeof(*p->eInMap));
    cleanVector(p->eMap, NOT_ASSIGNED, totalEvents);
  } else {
    p->eMap = NULL;
    p->eInMap = NULL;
    p->eOutMap = NULL;
  }
  p->iMap = NULL;
  cleanVector(p->qMap, NOT_ASSIGNED, totalStates);
  cleanVector(p->nLPS, 0, nLPS + 1);
  cleanVector(p->lps, 0, lps);
  if (outputs) {
    p->oMap = (QSS_idxMap)malloc(totalOutputs * sizeof(int));
    cleanVector(p->oMap, NOT_ASSIGNED, totalOutputs);
  } else {
    p->oMap = NULL;
  }
  return p;
}

void QSS_LP_copyStructure(QSS_LP_data data, QSS_LP_data p) {
  int totalStates = data->totalStates;
  int totalEvents = data->totalEvents;
  int totalOutputs = data->totalOutputs;
  int states = data->states;
  int events = data->events;
  int inStates = data->inStates;
  int outputs = data->outputs;
  int inEvents = data->inEvents;
  int nLPS = data->nLPSCount;
  int lps = data->lpsCount;
  if (QSS_hardCopyStruct) {
    p->nLPS = (int *)malloc((nLPS + 1) * sizeof(int));
    p->lps = (int *)malloc(lps * sizeof(int));
    p->qMap = (QSS_idxMap)malloc(totalStates * sizeof(int));
    p->qInMap = (QSS_idxMap)malloc(inStates * sizeof(int));
    p->qOutMap = (QSS_idxMap)malloc(states * sizeof(int));
    int i;
    for (i = 0; i < nLPS + 1; i++) {
      p->nLPS[i] = data->nLPS[i];
    }
    for (i = 0; i < lps; i++) {
      p->lps[i] = data->lps[i];
    }
    for (i = 0; i < totalStates; i++) {
      p->qMap[i] = data->qMap[i];
    }
    for (i = 0; i < inStates; i++) {
      p->qInMap[i] = data->qInMap[i];
    }
    for (i = 0; i < states; i++) {
      p->qOutMap[i] = data->qOutMap[i];
    }
    if (events + inEvents > 0) {
      p->eMap = (QSS_idxMap)malloc(totalEvents * sizeof(*p->eMap));
      p->eInMap = (QSS_idxMap)malloc(inEvents * sizeof(*p->eInMap));
      p->eOutMap = (QSS_idxMap)malloc(events * sizeof(*p->eInMap));
      for (i = 0; i < totalEvents; i++) {
        p->eMap[i] = data->eMap[i];
      }
      for (i = 0; i < inEvents; i++) {
        p->eInMap[i] = data->eInMap[i];
      }
      for (i = 0; i < events; i++) {
        p->eOutMap[i] = data->eOutMap[i];
      }
    } else {
      p->eMap = NULL;
      p->eInMap = NULL;
      p->eOutMap = NULL;
    }
    if (outputs) {
      p->oMap = (QSS_idxMap)malloc(totalOutputs * sizeof(int));
      for (i = 0; i < totalOutputs; i++) {
        p->oMap[i] = data->oMap[i];
      }
    } else {
      p->oMap = NULL;
    }
  } else {
    p->nLPS = data->nLPS;
    p->lps = data->lps;
    p->qMap = data->qMap;
    p->qInMap = data->qInMap;
    p->qOutMap = data->qOutMap;
    p->dscMap = data->dscMap;
    p->externalEvent = data->externalEvent;
    if (events + inEvents > 0) {
      p->eMap = data->eMap;
      p->eInMap = data->eInMap;
      p->eOutMap = data->eOutMap;
    } else {
      p->eMap = NULL;
      p->eInMap = NULL;
      p->eOutMap = NULL;
    }
    if (outputs) {
      p->oMap = data->oMap;
    } else {
      p->oMap = NULL;
    }
  }
  p->iMap = NULL;
}

QSS_LP_data QSS_LP_copyData(QSS_LP_data data) {
  QSS_LP_data p = checkedMalloc(sizeof(*p));
  int totalStates = data->totalStates;
  int totalEvents = data->totalEvents;
  int totalOutputs = data->totalOutputs;
  int states = data->states;
  int events = data->events;
  int inputs = data->inputs;
  int inStates = data->inStates;
  int outputs = data->outputs;
  int inEvents = data->inEvents;
  p->totalStates = totalStates;
  p->totalEvents = totalEvents;
  p->totalOutputs = totalOutputs;
  p->states = states;
  p->events = events;
  p->inputs = inputs;
  p->outputs = outputs;
  p->outStates = data->outStates;
  p->inStates = inStates;
  p->inEvents = inEvents;
  p->initDt = data->initDt;
  p->externalEvent = data->externalEvent;
  p->nLPSCount = data->nLPSCount;
  p->lpsCount = data->lpsCount;
  p->dscInf = data->dscInf;
  p->id = data->id;
  QSS_LP_copyStructure(data, p);
  return p;
}

QSS_LP_dataArray QSS_LP_DataArray(int size) {
  QSS_LP_dataArray p = (QSS_LP_dataArray)checkedMalloc(sizeof(*p));
  p->lp = (QSS_LP_data *)checkedMalloc(sizeof(QSS_LP_data) * size);
  p->size = size;
  int i;
  for (i = 0; i < size; i++) {
    p->lp[i] = NULL;
  }
  return p;
}

void QSS_LP_freeDataArray(QSS_LP_dataArray array) {
  if (!QSS_hardCopyStruct) {
    int i, size = array->size;
    for (i = 0; i < size; i++) {
      if (array->lp[i] != NULL) {
        QSS_LP_freeData(array->lp[i]);
      }
    }
  }
  free(array->lp);
  free(array);
}

void QSS_LP_clean(QSS_LP_data data) {
  if (QSS_hardCopyStruct) {
    QSS_LP_freeData(data);
  }
}

void QSS_LP_freeData(QSS_LP_data data) {
  if (data == NULL) {
    return;
  }
  if (data->nLPS != NULL) {
    free(data->nLPS);
  }
  if (data->lps != NULL) {
    free(data->lps);
  }
  if (data->qOutMap != NULL) {
    free(data->qOutMap);
  }
  if (data->eOutMap != NULL) {
    free(data->eOutMap);
  }
  if (data->qMap != NULL) {
    free(data->qMap);
  }
  if (data->qInMap != NULL) {
    free(data->qInMap);
  }
  if (data->eMap != NULL) {
    free(data->eMap);
  }
  if (data->eInMap != NULL) {
    free(data->eInMap);
  }
  if (data->iMap != NULL) {
    free(data->iMap);
  }
  if (data->dscMap != NULL) {
    free(data->dscMap);
  }
  free(data);
}

QSS_event QSS_Event(QSS_zc zeroCrossing, QSS_hnd handlerPos,
                    QSS_hnd handlerNeg) {
  QSS_event p = checkedMalloc(sizeof(*p));
  p->zeroCrossing = zeroCrossing;
  p->handlerPos = handlerPos;
  p->handlerNeg = handlerNeg;
  return p;
}

void QSS_freeEvent(QSS_event events) { free(events); }

QSS_data QSS_Data(int states, int discretes, int events, int inputs, int algs,
                  string name) {
  char fileName[256];
  strcpy(fileName, name);
  strcat(fileName, ".ini");
  SET_settings settings = SET_Settings(fileName);
  if (settings == NULL) {
    printf("Error: Could not open ini file\n");
    exit(-1);
  }
  QSS_data p = checkedMalloc(sizeof(*p));
  int i, order = settings->order;
  int xOrder = order + 1;
  p->states = states;
  p->discretes = discretes;
  p->algs = algs;
  p->events = events;
  p->inputs = inputs;
  p->order = order;
  p->maxRHS = 0;
  p->solver = settings->solver;
  p->dQMin = (double *)malloc(states * sizeof(double));
  p->dQRel = (double *)malloc(states * sizeof(double));
  p->lqu = (double *)malloc(states * sizeof(double));
  p->BDF = NULL;
  p->BDFMap = NULL;
  p->BDFOutputs = NULL;
  p->BDFInputs = NULL;
  p->BDFOutputVars = NULL;
  p->JacIt = NULL;
  p->nBDF = 0;
  p->nBDFOutputs = 0;
  p->nBDFInputs = 0;
  p->nBDFOutputVars = 0;
  p->QSSOutputJac = NULL;
  p->QSSOutputJacId = NULL;
  p->QSSOutputs = NULL;
  if (discretes) {
    p->d = (double *)malloc(discretes * sizeof(double));
  } else {
    p->d = NULL;
  }
  p->q = (double *)malloc(states * xOrder * sizeof(double));
  p->x = (double *)malloc(states * xOrder * sizeof(double));
  if (algs) {
    p->alg = (double *)malloc(algs * xOrder * sizeof(double));
  } else {
    p->alg = NULL;
  }
  p->jac = NULL;
  p->it = settings->it;
  p->ft = settings->ft;
  p->nSD = (int *)malloc(states * sizeof(int));
  p->nDS = (int *)malloc(states * sizeof(int));
  p->SD = (int **)malloc(states * sizeof(int *));
  p->DS = (int **)malloc(states * sizeof(int *));
  if (events) {
    p->nSZ = (int *)malloc(states * sizeof(int));
    p->nZS = (int *)malloc(events * sizeof(int));
    p->nHD = (int *)malloc(events * sizeof(int));
    p->nHZ = (int *)malloc(events * sizeof(int));
    p->SZ = (int **)malloc(states * sizeof(int *));
    p->ZS = (int **)malloc(events * sizeof(int *));
    p->HD = (int **)malloc(events * sizeof(int *));
    p->HZ = (int **)malloc(events * sizeof(int *));
    if (settings->lps > 0) {
      p->nSH = (int *)malloc(states * sizeof(int));
      p->SH = (int **)malloc(states * sizeof(int *));
      p->nDD = (int *)malloc(events * sizeof(int));
      p->DD = (int **)malloc(events * sizeof(int *));
      p->nDH = (int *)malloc(discretes * sizeof(int));
      p->DH = (int **)malloc(discretes * sizeof(int *));
    } else {
      p->nSH = NULL;
      p->SH = NULL;
      p->nDD = NULL;
      p->DD = NULL;
      p->nDH = NULL;
      p->DH = NULL;
    }
  } else {
    p->nSZ = NULL;
    p->nZS = NULL;
    p->nHD = NULL;
    p->nHZ = NULL;
    p->SZ = NULL;
    p->ZS = NULL;
    p->HD = NULL;
    p->HZ = NULL;
    p->nSH = NULL;
    p->SH = NULL;
    p->nDD = NULL;
    p->DD = NULL;
    p->nDH = NULL;
    p->DH = NULL;
  }
  if (inputs) {
    p->TD = (int *)malloc(inputs * sizeof(int));
  } else {
    p->TD = NULL;
  }
  if (discretes) {
    cleanDoubleVector(p->d, 0, discretes);
  }
  double sameDQMin = (settings->nDQMin == 1) ? settings->dqmin[0] : 0;
  double sameDQRel = (settings->nDQRel == 1) ? settings->dqrel[0] : 0;
  cleanDoubleVector(p->q, 0, states * xOrder);
  cleanDoubleVector(p->x, 0, states * xOrder);
  cleanDoubleVector(p->alg, 0, algs * xOrder);
  cleanDoubleVector(p->lqu, 0, states);
  cleanVector(p->nSD, 0, states);
  cleanVector(p->nDS, 0, states);
  if (events) {
    cleanVector(p->nSZ, 0, states);
    if (settings->lps > 0) {
      cleanVector(p->nSH, 0, states);
    }
  }
  if (sameDQMin == 0 || sameDQRel == 0) {
    for (i = 0; i < states; i++) {
      p->dQMin[i] = (sameDQMin) ? settings->dqmin[0] : settings->dqmin[i];
      p->dQRel[i] = (sameDQRel) ? settings->dqrel[0] : settings->dqrel[i];
    }
  } else {
    for (i = 0; i < states; i++) {
      p->dQMin[i] = settings->dqmin[0];
      p->dQRel[i] = settings->dqrel[0];
    }
  }
  if (events) {
    cleanVector(p->nZS, 0, events);
    cleanVector(p->nHD, 0, events);
    cleanVector(p->nHZ, 0, events);
    if (settings->lps > 0) {
      cleanVector(p->nDD, 0, events);
      cleanVector(p->nDH, 0, discretes);
    }
    p->event = SD_EventData(events);
  } else {
    p->event = NULL;
  }
  p->params = SD_Parameters(settings->derdelta, settings->zchyst,
                            settings->minstep, settings->symdiff, settings->lps,
                            settings->nodesize, settings->pm, settings->dt,
                            settings->dtSynch, settings->partitionerOptions, 0);
  p->lp = NULL;
  if (settings->lps > 0) {
    QSS_setReinitBuffer(TRUE);
  }
  freeSettings(settings);
  return p;
}

void QSS_dataCopyStructure(QSS_data data, QSS_data p) {
  int i;
  int states = data->states;
  int events = data->events;
  int inputs = data->inputs;
  if (QSS_hardCopyStruct) {
    int j, iter;
    p->dQMin = (double *)malloc(states * sizeof(double));
    p->dQRel = (double *)malloc(states * sizeof(double));
    p->nSD = (int *)malloc(states * sizeof(int));
    p->nDS = (int *)malloc(states * sizeof(int));
    p->SD = (int **)malloc(states * sizeof(int *));
    p->DS = (int **)malloc(states * sizeof(int *));
    if (events) {
      p->nSZ = (int *)malloc(states * sizeof(int));
      p->SZ = (int **)malloc(states * sizeof(int *));
    }
    for (i = 0; i < states; i++) {
      p->dQMin[i] = data->dQMin[i];
      p->dQRel[i] = data->dQRel[i];
      p->nSD[i] = data->nSD[i];
      p->nDS[i] = data->nDS[i];
      p->SD[i] =
          (p->nSD[i] > 0) ? (int *)malloc(p->nSD[i] * sizeof(int)) : NULL;
      if (p->SD[i] != NULL) {
        iter = p->nSD[i];
        for (j = 0; j < iter; j++) {
          p->SD[i][j] = data->SD[i][j];
        }
      }
      p->DS[i] =
          (p->nDS[i] > 0) ? (int *)malloc(p->nDS[i] * sizeof(int)) : NULL;
      if (p->DS[i] != NULL) {
        iter = p->nDS[i];
        for (j = 0; j < iter; j++) {
          p->DS[i][j] = data->DS[i][j];
        }
      }
      if (events) {
        p->nSZ[i] = data->nSZ[i];
        p->SZ[i] =
            (p->nSZ[i] > 0) ? (int *)malloc(p->nSZ[i] * sizeof(int)) : NULL;
        if (p->SZ[i] != NULL) {
          iter = p->nSZ[i];
          for (j = 0; j < iter; j++) {
            p->SZ[i][j] = data->SZ[i][j];
          }
        }
      }
    }
    if (events) {
      p->nZS = (int *)malloc(events * sizeof(int));
      p->nHD = (int *)malloc(events * sizeof(int));
      p->nHZ = (int *)malloc(events * sizeof(int));
      p->ZS = (int **)malloc(events * sizeof(int *));
      p->HD = (int **)malloc(events * sizeof(int *));
      p->HZ = (int **)malloc(events * sizeof(int *));
      for (i = 0; i < events; i++) {
        p->nZS[i] = data->nZS[i];
        p->nHD[i] = data->nHD[i];
        p->nHZ[i] = data->nHZ[i];
        p->ZS[i] =
            (p->nZS[i] > 0) ? (int *)malloc(p->nZS[i] * sizeof(int)) : NULL;
        if (p->ZS[i] != NULL) {
          iter = p->nZS[i];
          for (j = 0; j < iter; j++) {
            p->ZS[i][j] = data->ZS[i][j];
          }
        }
        p->HD[i] =
            (p->nHD[i] > 0) ? (int *)malloc(p->nHD[i] * sizeof(int)) : NULL;
        if (p->HD[i] != NULL) {
          iter = p->nHD[i];
          for (j = 0; j < iter; j++) {
            p->HD[i][j] = data->HD[i][j];
          }
        }
        p->HZ[i] =
            (p->nHZ[i] > 0) ? (int *)malloc(p->nHZ[i] * sizeof(int)) : NULL;
        if (p->HZ[i] != NULL) {
          iter = p->nHZ[i];
          for (j = 0; j < iter; j++) {
            p->HZ[i][j] = data->HZ[i][j];
          }
        }
        p->event[i].LHSSt =
            (p->event[i].nLHSSt > 0)
                ? (int *)malloc(p->event[i].nLHSSt * sizeof(int))
                : NULL;
        if (p->event[i].LHSSt != NULL) {
          iter = p->event[i].nLHSSt;
          for (j = 0; j < iter; j++) {
            p->event[i].LHSSt[j] = data->event[i].LHSSt[j];
          }
        }
        p->event[i].LHSDsc =
            (p->event[i].nLHSDsc > 0)
                ? (int *)malloc(p->event[i].nLHSDsc * sizeof(int))
                : NULL;
        if (p->event[i].LHSDsc != NULL) {
          iter = p->event[i].nLHSDsc;
          for (j = 0; j < iter; j++) {
            p->event[i].LHSDsc[j] = data->event[i].LHSDsc[j];
          }
        }
        p->event[i].RHSSt =
            (p->event[i].nRHSSt > 0)
                ? (int *)malloc(p->event[i].nRHSSt * sizeof(int))
                : NULL;
        if (p->event[i].RHSSt != NULL) {
          iter = p->event[i].nRHSSt;
          for (j = 0; j < iter; j++) {
            p->event[i].RHSSt[j] = data->event[i].RHSSt[j];
          }
        }
      }
    } else {
      p->nSZ = NULL;
      p->nZS = NULL;
      p->nHD = NULL;
      p->nHZ = NULL;
      p->SZ = NULL;
      p->ZS = NULL;
      p->HD = NULL;
      p->HZ = NULL;
    }
    if (inputs) {
      p->TD = (int *)malloc(inputs * sizeof(int));
      for (i = 0; i < inputs; i++) {
        p->TD[i] = data->TD[i];
      }
    } else {
      p->TD = NULL;
    }
    p->params = SD_copyParameters(data->params);
  } else {
    p->dQMin = data->dQMin;
    p->dQRel = data->dQRel;
    p->nSD = data->nSD;
    p->nDS = data->nDS;
    p->SD = data->SD;
    p->DS = data->DS;
    if (events) {
      p->nSZ = data->nSZ;
      p->nZS = data->nZS;
      p->nHD = data->nHD;
      p->nHZ = data->nHZ;
      p->SZ = data->SZ;
      p->ZS = data->ZS;
      p->HD = data->HD;
      p->HZ = data->HZ;
      for (i = 0; i < events; i++) {
        p->event[i].LHSSt = data->event[i].LHSSt;
        p->event[i].LHSDsc = data->event[i].LHSDsc;
        p->event[i].RHSSt = data->event[i].RHSSt;
      }
    } else {
      p->nSZ = NULL;
      p->nZS = NULL;
      p->nHD = NULL;
      p->nHZ = NULL;
      p->SZ = NULL;
      p->ZS = NULL;
      p->HD = NULL;
      p->HZ = NULL;
    }
    if (inputs) {
      p->TD = data->TD;
    } else {
      p->TD = NULL;
    }
    p->params = data->params;
  }
}

QSS_data QSS_copyData(QSS_data data) {
  QSS_data p = checkedMalloc(sizeof(*p));
  int i;
  int order = data->order;
  int states = data->states;
  int discretes = data->discretes;
  int events = data->events;
  int inputs = data->inputs;
  int algs = data->algs;
  int xOrder = order + 1;
  p->states = states;
  p->discretes = discretes;
  p->algs = algs;
  p->events = events;
  p->inputs = inputs;
  p->order = order;
  p->solver = data->solver;
  p->it = data->it;
  p->ft = data->ft;
  p->maxRHS = data->maxRHS;
  if (discretes) {
    p->d = (double *)malloc(discretes * sizeof(double));
    for (i = 0; i < discretes; i++) {
      p->d[i] = data->d[i];
    }
  } else {
    p->d = NULL;
  }
  if (QSS_sharedState == TRUE) {
    p->lqu = data->lqu;
    p->x = data->x;
    p->q = data->q;
  } else {
    p->lqu = (double *)malloc(states * sizeof(double));
    cleanDoubleVector(p->lqu, 0, states);
    p->x = (double *)malloc(states * xOrder * sizeof(double));
    p->q = (double *)malloc(states * xOrder * sizeof(double));
    for (i = 0; i < states; i++) {
      p->x[i * xOrder] = data->x[i * xOrder];
      p->q[i * xOrder] = data->q[i * xOrder];
    }
  }
  if (algs) {
    p->alg = (double *)malloc(algs * xOrder * sizeof(double));
    cleanDoubleVector(p->alg, 0, algs * xOrder);
  } else {
    p->alg = NULL;
  }
  if (events) {
    p->event = SD_copyEventData(events, data->event);
  }
  p->lp = NULL;
  p->nSH = NULL;
  p->SH = NULL;
  p->nDD = NULL;
  p->DD = NULL;
  p->nDH = NULL;
  p->DH = NULL;
  QSS_dataCopyStructure(data, p);
  return p;
}

void QSS_allocDataMatrix(QSS_data data) {
  int i, states = data->states, events = data->events, mRHS = 0, deps = 0;
  for (i = 0; i < states; i++) {
    data->SD[i] =
        (data->nSD[i] > 0) ? (int *)malloc(data->nSD[i] * sizeof(int)) : NULL;
    data->DS[i] =
        (data->nDS[i] > 0) ? (int *)malloc(data->nDS[i] * sizeof(int)) : NULL;
    if (data->nDS[i] > mRHS) {
      mRHS = data->nDS[i];
    }
    if (events) {
      data->SZ[i] =
          (data->nSZ[i] > 0) ? (int *)malloc(data->nSZ[i] * sizeof(int)) : NULL;
      if (data->nSH != NULL) {
        data->SH[i] = (data->nSH[i] > 0)
                          ? (int *)malloc(data->nSH[i] * sizeof(int))
                          : NULL;
      }
    }
    deps += data->nSD[i];
  }
  if (deps > 0) {
    data->jac = (double *)malloc(deps * sizeof(double));
  }
  for (i = 0; i < events; i++) {
    data->ZS[i] =
        (data->nZS[i] > 0) ? (int *)malloc(data->nZS[i] * sizeof(int)) : NULL;
    if (data->nZS[i] > mRHS) {
      mRHS = data->nZS[i];
    }
    data->HD[i] =
        (data->nHD[i] > 0) ? (int *)malloc(data->nHD[i] * sizeof(int)) : NULL;
    data->HZ[i] =
        (data->nHZ[i] > 0) ? (int *)malloc(data->nHZ[i] * sizeof(int)) : NULL;
    data->event[i].LHSSt =
        (data->event[i].nLHSSt > 0)
            ? (int *)malloc(data->event[i].nLHSSt * sizeof(int))
            : NULL;
    data->event[i].LHSDsc =
        (data->event[i].nLHSDsc > 0)
            ? (int *)malloc(data->event[i].nLHSDsc * sizeof(int))
            : NULL;
    data->event[i].RHSSt =
        (data->event[i].nRHSSt > 0)
            ? (int *)malloc(data->event[i].nRHSSt * sizeof(int))
            : NULL;
    if (data->event[i].nRHSSt + data->event[i].nLHSSt > mRHS) {
      mRHS = data->event[i].nRHSSt + data->event[i].nLHSSt;
    }
    if (data->nDD != NULL) {
      data->DD[i] =
          (data->nDD[i] > 0) ? (int *)malloc(data->nDD[i] * sizeof(int)) : NULL;
    }
  }
  if (data->nDH != NULL) {
    int discretes = data->discretes;
    for (i = 0; i < discretes; i++) {
      data->DH[i] =
          (data->nDH[i] > 0) ? (int *)malloc(data->nDH[i] * sizeof(int)) : NULL;
    }
  }
  data->maxRHS = mRHS;
}

int QSS_intCmp(const void *x, const void *y) {
  int xx = *(int *)x, yy = *(int *)y;
  if (xx < yy)
    return -1;
  if (xx > yy)
    return 1;
  return 0;
}

void QSS_orderDataMatrix(QSS_data data) {
  int i, states = data->states, events = data->events;
  for (i = 0; i < states; i++) {
    if (data->SD[i] != NULL) {
      qsort(data->SD[i], data->nSD[i], sizeof(int), QSS_intCmp);
    }
    if (data->DS[i] != NULL) {
      qsort(data->DS[i], data->nDS[i], sizeof(int), QSS_intCmp);
    }
    if (events) {
      if (data->SZ[i] != NULL) {
        qsort(data->SZ[i], data->nSZ[i], sizeof(int), QSS_intCmp);
      }
    }
  }
  for (i = 0; i < events; i++) {
    if (data->ZS[i] != NULL) {
      qsort(data->ZS[i], data->nZS[i], sizeof(int), QSS_intCmp);
    }
    if (data->HD[i] != NULL) {
      qsort(data->HD[i], data->nHD[i], sizeof(int), QSS_intCmp);
    }
    if (data->HZ[i] != NULL) {
      qsort(data->HZ[i], data->nHZ[i], sizeof(int), QSS_intCmp);
    }
    if (data->event[i].LHSSt != NULL) {
      qsort(data->event[i].LHSSt, data->event[i].nLHSSt, sizeof(int),
            QSS_intCmp);
    }
    if (data->event[i].LHSDsc != NULL) {
      qsort(data->event[i].LHSDsc, data->event[i].nLHSDsc, sizeof(int),
            QSS_intCmp);
    }
    if (data->event[i].RHSSt != NULL) {
      qsort(data->event[i].RHSSt, data->event[i].nRHSSt, sizeof(int),
            QSS_intCmp);
    }
  }
}

void QSS_freeData(QSS_data data) {
  int i, states = data->states, events = data->events;
  free(data->dQMin);
  free(data->dQRel);
  free(data->lqu);
  if (data->discretes) {
    free(data->d);
  }
  if (data->algs) {
    free(data->alg);
  }
  free(data->x);
  free(data->q);
  free(data->nSD);
  free(data->nDS);
  if (data->events) {
    free(data->nSZ);
    free(data->nZS);
    free(data->nHD);
    free(data->nHZ);
  }
  if (data->inputs) {
    free(data->TD);
  }
  for (i = 0; i < states; i++) {
    if (data->SD[i] != NULL) {
      free(data->SD[i]);
    }
    if (data->DS[i] != NULL) {
      free(data->DS[i]);
    }
    if (events) {
      if (data->SZ[i] != NULL) {
        free(data->SZ[i]);
      }
    }
  }
  free(data->SD);
  free(data->DS);
  for (i = 0; i < events; i++) {
    if (data->ZS[i] != NULL) {
      free(data->ZS[i]);
    }
    if (data->HD[i] != NULL) {
      free(data->HD[i]);
    }
    if (data->HZ[i] != NULL) {
      free(data->HZ[i]);
    }
  }
  if (events) {
    free(data->SZ);
    free(data->ZS);
    free(data->HD);
    free(data->HZ);
  }
  if (events) {
    if (data->SH != NULL) {
      for (i = 0; i < states; i++) {
        if (data->SH[i] != NULL) {
          free(data->SH[i]);
        }
      }
      free(data->SH);
    }
    if (data->nSH != NULL) {
      free(data->nSH);
    }
    if (data->DD != NULL) {
      for (i = 0; i < events; i++) {
        if (data->DD[i] != NULL) {
          free(data->DD[i]);
        }
      }
      free(data->DD);
    }
    if (data->nDD != NULL) {
      free(data->nDD);
    }
    if (data->DH != NULL) {
      int discretes = data->discretes;
      for (i = 0; i < discretes; i++) {
        if (data->DH[i] != NULL) {
          free(data->DH[i]);
        }
      }
      free(data->DH);
    }
    if (data->nDH != NULL) {
      free(data->nDH);
    }
  }
  if (data->lp != NULL) {
    QSS_LP_freeData(data->lp);
  }
  SD_freeEventData(data->event, data->events);
  SD_freeParameters(data->params);
  free(data);
}

void QSS_cleanData(QSS_data data) {
  if (QSS_hardCopyStruct) {
    QSS_freeData(data);
  } else {
    if (data->discretes) {
      free(data->d);
    }
    if (data->algs) {
      free(data->alg);
    }
    if (QSS_sharedState == FALSE) {
      free(data->lqu);
      free(data->x);
      free(data->q);
    }
    SD_cleanEventData(data->event, data->events);
    free(data->lp);
    free(data);
  }
}

QSS_time QSS_Time(int states, int events, int inputs, double it,
                  QSS_SchedulerType scheduler, double *weights) {
  int i;
  QSS_time p = checkedMalloc(sizeof(*p));
  if (states) {
    p->nextStateTime = (double *)malloc(states * sizeof(double));
  } else {
    p->nextStateTime = NULL;
  }
  if (events) {
    p->nextEventTime = (double *)malloc(events * sizeof(double));
    if (weights != NULL) {
      p->weights = (double *)malloc(events * sizeof(double));
    } else {
      p->weights = NULL;
    }
    p->reinits = QSS_Reinit();
  } else {
    p->nextEventTime = NULL;
    p->weights = NULL;
  }
  if (inputs) {
    p->nextInputTime = (double *)malloc(inputs * sizeof(double));
  } else {
    p->nextInputTime = NULL;
  }
  p->tx = (double *)malloc(states * sizeof(double));
  p->tq = (double *)malloc(states * sizeof(double));
  p->time = it;
  p->minValue = 0;
  p->minIndex = NULL_INDEX;
  p->scheduler = scheduler;
  p->type = ST_State;
  p->noReinit = 1;
  p->previousTime = it;
  for (i = 0; i < states; i++) {
    p->tx[i] = it;
    p->tq[i] = it;
    p->nextStateTime[i] = it;
  }
  for (i = 0; i < events; i++) {
    p->nextEventTime[i] = it;
  }
  if (weights != NULL) {
    for (i = 0; i < events; i++) {
      p->weights[i] = weights[i];
    }
  }
  for (i = 0; i < inputs; i++) {
    p->nextInputTime[i] = it;
  }
  return p;
}

void QSS_freeTime(QSS_time simTime, int events, int inputs) {
  free(simTime->nextStateTime);
  free(simTime->tq);
  free(simTime->tx);
  if (events) {
    free(simTime->nextEventTime);
    if (simTime->reinits != NULL) {
      QSS_freeReinit(simTime->reinits);
    }
  }
  if (inputs) {
    free(simTime->nextInputTime);
  }
  free(simTime);
}

QSS_model QSS_Model(QSS_eq f, QSS_dep deps, QSS_zc zeroCrossing,
                    QSS_hnd handlerPos, QSS_hnd handlerNeg, QSS_jac jac, QSS_fmodel F) {
  QSS_model p = checkedMalloc(sizeof(*p));
  p->f = f;
  p->deps = deps;
  p->events = QSS_Event(zeroCrossing, handlerPos, handlerNeg);
  p->jac = jac;
  p->F = F;
  return p;
}

void QSS_freeModel(QSS_model model) {
  free(model->events);
  free(model);
}

/**
 * This should be deprecated
 *
 */

bool QSS_influenced(QSS_data data, int variable, int inf, QSS_StepType type) {
  if (type == ST_State) {
    int nSD = data->nSD[variable], iter;
    for (iter = 0; iter < nSD; iter++) {
      if (inf == data->SD[variable][iter]) {
        return TRUE;
      }
    }
  } else if (type == ST_Event) {
    int nHZ = data->nHZ[variable], iter;
    for (iter = 0; iter < nHZ; iter++) {
      if (inf == data->HZ[variable][iter]) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
