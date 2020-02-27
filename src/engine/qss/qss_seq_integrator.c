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

#include "qss_seq_integrator.h"

#include <math.h>

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_frw.h"
#include "qss_integrator.h"
#include "qss_output.h"
#include "qss_quantizer.h"
#include "qss_scheduler.h"
#include "qss_simulator.h"

void QSS_SEQ_integrate(SIM_simulator simulate)
{
  QSS_simulator simulator = simulate->state->sim;
  int i, j;
  double elapsed;
  // Local data structure mappings.
  QSS_data qssData = simulator->data;
  QSS_time qssTime = simulator->time;
  FRW_framework frw = simulator->frw;
  SC_scheduler scheduler = simulator->scheduler;
  OUT_output log = simulator->log;
  QSS_model qssModel = simulator->model;
  QA_quantizer quantizer = simulator->quantizer;
  SD_output output = simulator->output;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
#endif
  unsigned long totalSteps = 0;
  unsigned long reinits = 0;
  double t = qssTime->time;
  int index = qssTime->minIndex;
  QSS_StepType type = qssTime->type;
  int nSD, nOutputs = output->outputs;
  const int xOrder = qssData->order;
  const int coeffs = xOrder + 1;
  const double ft = qssData->ft;
  double *tq = qssTime->tq;
  double *tx = qssTime->tx;
  double *nextStateTime = qssTime->nextStateTime;
  double *dQRel = qssData->dQRel;
  double *dQMin = qssData->dQMin;
  double *lqu = qssData->lqu;
  double *x = qssData->x;
  double *q = qssData->q;
  double *a = qssData->alg;
  int **SD = qssData->SD;
  int *TD = qssData->TD;
  int cf0, infCf0;
  const int qOrder = xOrder - 1;
  int nSZ, nLHSSt, nRHSSt, nHD, nHZ;
  SD_eventData event = qssData->event;
  double *d = qssData->d;
  double tmp1[qssData->maxRHS];
  int **SZ = qssData->SZ;
  int **ZS = qssData->ZS;
  int **HD = qssData->HD;
  int **HZ = qssData->HZ;
#ifdef SYNC_RT
  setInitRealTime();
#endif
#ifdef DEBUG
  if (settings->debug & SD_DBG_StepInfo) {
    SD_print(simulator->simulationLog, "Begin Simulation:");
  }
#endif
  while (t < ft) {
#ifdef SYNC_RT
    /* Sync */
    waitUntil(t);
#endif
#ifdef DEBUG
    if (settings->debug & SD_DBG_StepInfo) {
      SD_print(simulationLog, "Simulation Time: %g", t);
    }
#endif
    switch (type) {
    case ST_State: {
#ifdef DEBUG
      if (settings->debug & SD_DBG_StepInfo) {
        SD_print(simulationLog, "State Variable: %d", index);
      }
      if (settings->debug & SD_DBG_VarChanges) {
        simulationLog->states[index]++;
      }
#endif
      cf0 = index * coeffs;
      elapsed = t - tx[index];
      integrateState(cf0, elapsed, x, xOrder);
      tx[index] = t;
      lqu[index] = dQRel[index] * fabs(x[cf0]);
      if (lqu[index] < dQMin[index]) {
        lqu[index] = dQMin[index];
      }
      QA_updateQuantizedState(quantizer, index, q, x, lqu);
      tq[index] = t;
      QA_nextTime(quantizer, index, t, nextStateTime, x, lqu);
      nSD = qssData->nSD[index];
      for (i = 0; i < nSD; i++) {
        j = SD[index][i];
        elapsed = t - tx[j];
        infCf0 = j * coeffs;
        if (elapsed > 0) {
          x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
          tx[j] = t;
        }
      }
      FRW_recomputeDerivatives(frw, qssModel, qssData, qssTime, index);
      if (qssData->events > 0) {
        nSZ = qssData->nSZ[index];
        for (i = 0; i < nSZ; i++) {
          j = SZ[index][i];
          FRW_nextEventTime(frw, qssModel, qssData, qssTime, j);
        }
      }
      QA_recomputeNextTimes(quantizer, nSD, SD[index], t, nextStateTime, x, lqu, q);
      if (nOutputs) {
        if (output->nSO[index]) {
          OUT_write(log, qssData, qssTime, output);
        }
      }
    } break;
    case ST_Input: {
#ifdef DEBUG
      if (settings->debug & SD_DBG_StepInfo) {
        SD_print(simulationLog, "Input: %d", index);
      }
#endif
      j = TD[index];
      elapsed = t - tx[j];
      infCf0 = j * coeffs;
      if (elapsed > 0) {
        x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
        tx[j] = t;
      }
      FRW_recomputeDerivative(frw, qssModel, qssData, qssTime, j);
      FRW_nextInputTime(frw, qssModel, qssData, qssTime, elapsed, j, index);
      QA_recomputeNextTime(quantizer, j, t, nextStateTime, x, lqu, q);
    } break;
    case ST_Event: {
#ifdef DEBUG
      if (settings->debug & SD_DBG_StepInfo) {
        SD_print(simulationLog, "Event: %d", index);
      }
#endif
      double zc[4];
      int s;
      int nZS = qssData->nZS[index];
      for (i = 0; i < nZS; i++) {
        j = ZS[index][i];
        elapsed = t - tq[j];
        if (elapsed > 0) {
          infCf0 = j * coeffs;
          integrateState(infCf0, elapsed, q, qOrder);
          tq[j] = t;
        }
      }
      qssModel->events->zeroCrossing(index, q, d, a, t, zc);
      s = sign(zc[0]);
      double et = INF;
      if (event[index].zcSign == s) {
        FRW_nextEventTime(frw, qssModel, qssData, qssTime, index);
        et = qssTime->nextEventTime[index];
      }
      if (event[index].zcSign != s || xOrder == 1 || et == t) {
        if (event[index].direction == 0 || event[index].direction == s) {
#ifdef DEBUG
          if (settings->debug & SD_DBG_VarChanges) {
            simulationLog->handlers[index]++;
          }
#endif
          nRHSSt = event[index].nRHSSt;
          for (i = 0; i < nRHSSt; i++) {
            j = event[index].RHSSt[i];
            infCf0 = j * coeffs;
            elapsed = t - tq[j];
            if (elapsed > 0) {
              integrateState(infCf0, elapsed, q, qOrder);
            }
            tq[j] = t;
          }
          nLHSSt = event[index].nLHSSt;
          for (i = 0; i < nLHSSt; i++) {
            j = event[index].LHSSt[i];
            infCf0 = j * coeffs;
            elapsed = t - tq[j];
            if (elapsed > 0) {
              tmp1[i] = q[infCf0];
              q[infCf0] = evaluatePoly(infCf0, elapsed, q, qOrder);
            } else {
              tmp1[i] = q[infCf0];
            }
          }
          if (s >= 0) {
            qssModel->events->handlerPos(index, q, d, a, t);
          } else {
            qssModel->events->handlerNeg(index, q, d, a, t);
          }
          for (i = 0; i < nLHSSt; i++) {
            j = event[index].LHSSt[i];
            infCf0 = j * coeffs;
            x[infCf0] = q[infCf0];
            q[infCf0] = tmp1[i];
            tx[j] = t;
            lqu[j] = dQRel[j] * fabs(x[infCf0]);
            if (lqu[j] < dQMin[j]) {
              lqu[j] = dQMin[j];
            }
            QA_updateQuantizedState(quantizer, j, q, x, lqu);
            tq[j] = t;
#ifdef DEBUG
            if (settings->debug & SD_DBG_VarChanges) {
              simulationLog->states[j]++;
            }
#endif
          }
          qssModel->events->zeroCrossing(index, q, d, a, t, zc);
          event[index].zcSign = sign(zc[0]);
          for (i = 0; i < nLHSSt; i++) {
            j = event[index].LHSSt[i];
            QA_nextTime(quantizer, j, t, nextStateTime, x, lqu);
            int k, h;
            nSD = qssData->nSD[j];
            for (h = 0; h < nSD; h++) {
              k = SD[j][h];
              elapsed = t - tx[k];
              infCf0 = k * coeffs;
              if (elapsed > 0) {
                x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
                tx[k] = t;
              }
            }
            FRW_recomputeDerivatives(frw, qssModel, qssData, qssTime, j);
            nSZ = qssData->nSZ[j];
            for (h = 0; h < nSZ; h++) {
              k = SZ[j][h];
              if (k != index) {
                FRW_nextEventTime(frw, qssModel, qssData, qssTime, k);
              }
            }
            QA_recomputeNextTimes(quantizer, nSD, SD[j], t, nextStateTime, x, lqu, q);
            reinits++;
          }
          nHZ = qssData->nHZ[index];
          for (i = 0; i < nHZ; i++) {
            j = HZ[index][i];
            if (j != index) {
              FRW_nextEventTime(frw, qssModel, qssData, qssTime, j);
            }
          }
          nHD = qssData->nHD[index];
          for (i = 0; i < nHD; i++) {
            j = HD[index][i];
            elapsed = t - tx[j];
            if (elapsed > 0) {
              infCf0 = j * coeffs;
              x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
              tx[j] = t;
            }
            FRW_recomputeDerivative(frw, simulator->model, qssData, qssTime, j);
          }
          if (nHD) {
            QA_recomputeNextTimes(quantizer, nHD, HD[index], t, nextStateTime, x, lqu, q);
          }
          if (nOutputs) {
            if (event[index].nLHSDsc || event[index].nLHSSt) {
              OUT_write(log, qssData, qssTime, output);
            }
          }
        } else {
          event[index].zcSign = sign(zc[0]);
        }
      }
      if (et == t) {
        qssTime->nextEventTime[index] = INF;
      } else {
        FRW_nextEventTime(frw, qssModel, qssData, qssTime, index);
      }
    } break;
    default:
      break;
    }
    SC_update(scheduler, qssData, qssTime);
    t = qssTime->time;
    type = qssTime->type;
    index = qssTime->minIndex;
    totalSteps++;
#ifdef DEBUG
    if (settings->debug & SD_DBG_StepInfo) {
      SD_print(simulationLog, "");
    }
#endif
  }
  getTime(simulator->stats->sTime);
  subTime(simulator->stats->sTime, simulator->stats->iTime);
  simulator->stats->simulationTime = getTimeValue(simulator->stats->sTime);
  QSS_SEQ_saveLog(simulator);
  simulator->stats->totalSteps = totalSteps;
  QSS_SEQ_printSimulationLog(simulator);
}