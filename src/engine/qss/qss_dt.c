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

#include <stdio.h>

#ifdef __linux__

#define _GNU_SOURCE
#define __USE_GNU
#include <sys/mman.h>
#include <sched.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>

#include <qss/qss_dt.h>
#include <common/utils.h>

#define DT_LOWER_BOUND 0.5
#define DT_UPPER_BOUND 2

QSS_dtSynch QSS_DtSynch(int lps)
{
  QSS_dtSynch p = checkedMalloc(sizeof(*p));
  p->activeLPS = lps;
  p->synch = 0;
  p->t = 0;
  p->elapsed = 0;
#ifdef __linux__
  pthread_mutex_init(&(p->activeMutex), NULL);
  pthread_barrier_init(&(p->b), NULL, lps);
#endif
  return p;
}

void QSS_freeDtSynch(QSS_dtSynch dtSynch)
{
#ifdef __linux__
  pthread_mutex_destroy(&(dtSynch->activeMutex));
  pthread_barrier_destroy(&(dtSynch->b));
#endif
  free(dtSynch);
}

static void QSS_updateLocalDt(QSS_dt dt)
{
  double lclMin = dt->state->dtOpt[0];
  int ub = dt->state->outputs, lclMinIndex = 0, i;
  for (i = 1; i < ub; i++) {
    if (dt->state->dtOpt[i] < lclMin) {
      lclMin = dt->state->dtOpt[i];
      lclMinIndex = i;
    }
  }
  dt->state->dtMin = lclMin;
  dt->state->dtMinIndex = lclMinIndex;
}

/**
 * @brief Update the global \f $ \delta t$ \f value.
 *
 *  The procedure works as follows:
 *    - Update the global average step time array.
 *    - Wait until all the LPs have updated the array.
 *    - Calculate the new average step time of all LPs and the corresponding new \f $ \delta t$ \f value.
 *    - Turn off the synchronization flag.
 *    - Wait until all the LPs finished the update process.
 *    - Update the new \f $ \delta t$ \f.
 *
 * @param dt \f $ \delta t$ \f data structure.
 */
void QSS_dtUpdate(QSS_dt dt)
{
  int id = dt->state->id;
  double *gblDtMin = dt->state->gblDtMin;
  QSS_updateLocalDt(dt);
  gblDtMin[id] = dt->state->dtMin;
  double oldDt = dt->state->dt;
  pthread_barrier_wait(dt->state->b);
  double dtMin = gblDtMin[0];
  dt->state->dtGlobalLP = 0;
  int i, lps = dt->state->lps;
  for (i = 1; i < lps; i++) {
    if (gblDtMin[i] < dtMin) {
      dtMin = gblDtMin[i];
      dt->state->dtGlobalLP = i;
    }
  }
  printf("%g \n", dtMin);
  dt->state->synch[0] = 0;
  dt->state->dt = dtMin;
  dt->state->dtLowerBound = dtMin * DT_LOWER_BOUND;
  dt->state->dtUpperBound = dtMin * DT_UPPER_BOUND;
  pthread_barrier_wait(dt->state->b);
  if (oldDt < INF) {
    dt->state->avgDt += oldDt * dt->state->elapsed[0];
    dt->state->dtChanges++;
  }
#ifdef DEBUG
  if (dt->state->debug & SD_DBG_Dt && dt->state->id == 0) {
    SD_print(dt->state->log, "%g %g", dt->state->t[0], dt->state->dt);
  }
#endif
}

/**
 * @brief Dummy implementation for fixed \f $ \delta $ \f.
 *
 * @param dt
 * @param t
 * @param maxAdvanceTime
 * @param synch
 */
static inline bool DT_FIXED_logOutput(QSS_dt dt, double Dq, double Dx, double Dt, int variable) { return FALSE; }

static inline bool DT_FIXED_logStep(QSS_dt dt, double Dq, double Dx, double Dt) { return FALSE; }

/**
 * @brief
 *
 * @param dt
 * @param t0.0507148
 *
 * @param maxAdvanceTime
 * @param synch
 */
static bool DT_ADAPTIVE_logOutput(QSS_dt dt, double Dq, double Dx, double Dt, int variable)
{
  if (variable >= dt->state->outputs) {
    return FALSE;
  }
  double dtOpt = INF;
  if (Dx != 0) {
    dtOpt = (Dq * Dt * dt->state->alpha) / fabs(Dx);
  }
  dt->state->dtOpt[variable] = dtOpt;
  if (variable == dt->state->dtMinIndex) {
    QSS_updateLocalDt(dt);
  } else if (dtOpt < dt->state->dtMin) {
    dt->state->dtMin = dtOpt;
    dt->state->dtMinIndex = variable;
  }
  if ((dt->state->dtGlobalLP == dt->state->id && dt->state->dtMin > dt->state->dtUpperBound) ||
      dt->state->dtMin < dt->state->dtLowerBound) {
    dt->state->elapsed[0] = (dt->state->time->time - dt->state->t[0]) / dt->state->simTime;
    dt->state->t[0] = dt->state->time->time;
    dt->state->synch[0] = 1;
    QSS_dtUpdate(dt);
    return TRUE;
  }
  return FALSE;
}

static bool DT_ADAPTIVE_logStep(QSS_dt dt, double Dq, double Dx, double Dt)
{
  if (dt->state->synch[0] == 1) {
    QSS_dtUpdate(dt);
    return TRUE;
  }
  return FALSE;
}

static bool DT_ADAPTIVE_DISCRETE_LogStep(QSS_dt dt, double Dq, double Dx, double Dt)
{
  if (dt->state->time->type == ST_State && dt->state->time->noReinit) {
    int index = dt->state->time->minIndex;
    if (dt->state->qMap[index] > NOT_ASSIGNED && dt->state->dscMap[index] >= 0) {
      DT_ADAPTIVE_logOutput(dt, Dq, Dx, Dt, dt->state->dscMap[index]);
    }
  } else if (dt->state->synch[0] == 1) {
    QSS_dtUpdate(dt);
    return TRUE;
  }
  return FALSE;
}

QSS_dt QSS_Dt(double *gblDtMin, int id, QSS_dtSynch dtSynch, char *file, SD_Debug debug, QSS_data data, QSS_time time)
{
  QSS_dt p = checkedMalloc(sizeof(*p));
  int i, lclOutputs = data->lp->outStates;
  char logFile[128];
  double alpha = data->params->dt;
  double initDt = data->lp->initDt;
  double it = data->it;
  double ft = data->ft;
  sprintf(logFile, "%s-dt", file);
  p->ops = QSS_DtOps();
  p->state = QSS_DtState();
  p->state->strategy = data->params->dtSynch;
  p->state->dt = initDt;
  p->state->dtUpperBound = 0;
  p->state->dtLowerBound = INF;
  p->state->alpha = alpha;
  p->state->dtMin = INF;
  p->state->dtMinIndex = -1;
  p->state->gblDtMin = gblDtMin;
  p->state->synch = &(dtSynch->synch);
  p->state->id = id;
  p->state->lps = dtSynch->activeLPS;
  p->state->outputs = lclOutputs;
  p->state->dtChanges = 0;
  p->state->dtGlobalLP = id;
  p->state->avgDt = 0;
  p->state->time = time;
  p->state->t = &(dtSynch->t);
  p->state->elapsed = &(dtSynch->elapsed);
  p->state->simTime = ft - it;
  p->state->lastChange = p->state->t[0];
  p->state->simSteps = 0;
  if (p->state->strategy == SD_DT_Fixed) {
    p->ops->logStep = DT_FIXED_logStep;
    p->ops->logOutput = DT_FIXED_logOutput;
  } else if (p->state->strategy == SD_DT_Adaptive) {
    p->ops->logStep = DT_ADAPTIVE_logStep;
    p->ops->logOutput = DT_ADAPTIVE_logOutput;
  } else if (p->state->strategy == SD_DT_AdaptiveDiscrete) {
    p->ops->logStep = DT_ADAPTIVE_DISCRETE_LogStep;
    p->ops->logOutput = DT_FIXED_logOutput;
    p->state->dscMap = data->lp->dscMap;
    p->state->qMap = data->lp->qMap;
    p->state->outputs = data->lp->dscInf;
    lclOutputs = data->lp->dscInf;
  }
  if (lclOutputs == 0) {
    lclOutputs++;
  }
  p->state->dtOpt = checkedMalloc(lclOutputs * sizeof(double));
  for (i = 0; i < lclOutputs; i++) {
    p->state->dtOpt[i] = INF;
  }
  if (id == 0) {
    p->state->log = SD_SimulationLog(logFile);
  }
  p->state->debug = debug;
#ifdef __linux__
  p->state->b = &(dtSynch->b);
#endif
  return p;
}

void QSS_freeDt(QSS_dt dt)
{
  QSS_freeDtOps(dt->ops);
  QSS_freeDtState(dt->state);
  free(dt);
}

QSS_dtOps QSS_DtOps()
{
  QSS_dtOps p = checkedMalloc(sizeof(*p));
  p->logStep = NULL;
  return p;
}

void QSS_freeDtOps(QSS_dtOps ops) { free(ops); }

QSS_dtState QSS_DtState()
{
  QSS_dtState p = checkedMalloc(sizeof(*p));
  p->alpha = 0;
  p->dt = 0;
  p->dtLowerBound = 0;
  p->dtMin = 0;
  p->dtMinIndex = 0;
  p->dtOpt = NULL;
  p->dtUpperBound = 0;
  p->gblDtMin = NULL;
  p->dtGlobalLP = 0;
  p->id = 0;
  p->lps = 0;
  p->outputs = 0;
  p->synch = NULL;
  p->avgDt = 0;
  p->t = 0;
  p->elapsed = 0;
  p->simTime = 0;
  p->dtChanges = 0;
  p->lastChange = 0;
  p->simSteps = 0;
  p->log = NULL;
  p->dscMap = NULL;
  p->qMap = NULL;
  p->strategy = SD_DT_Fixed;
  return p;
}

void QSS_freeDtState(QSS_dtState state)
{
  if (state->id == 0) {
    SD_freeSimulationLog(state->log);
  }
  free(state->dtOpt);
  free(state);
}

void QSS_dtFinish(QSS_dt dt)
{
  if (dt->state->strategy == SD_DT_Fixed) {
    return;
  }
  int ub = dt->state->outputs, i;
  for (i = 0; i < ub; i++) {
    dt->state->dtOpt[i] = INF;
  }
  dt->state->dtMin = INF;
  dt->state->dtMinIndex = 0;
  if (dt->state->dtGlobalLP == dt->state->id) {
    dt->state->synch[0] = 1;
    QSS_dtUpdate(dt);
  }
}

bool QSS_dtLogOutput(QSS_dt dt, double Dq, double Dx, double Dt, int variable) { return dt->ops->logOutput(dt, Dq, Dx, Dt, variable); }

double QSS_dtValue(QSS_dt dt) { return dt->state->dt; }

bool QSS_dtLogStep(QSS_dt dt, double Dq, double Dx, double Dt) { return dt->ops->logStep(dt, Dq, Dx, Dt); }

void QSS_dtCheck(QSS_dt dt)
{
  if (dt->state->synch[0] == 1) {
    QSS_dtUpdate(dt);
  }
}

#else

/* Windows and MAC wrappers */

#include <qss/qss_dt.h>

QSS_dtSynch QSS_DtSynch(int lps) { return NULL; }

void QSS_freeDtSynch(QSS_dtSynch dtSynch) { return; }

QSS_dt QSS_Dt(double *gblDtMin, int id, QSS_dtSynch dtSynch, char *file, SD_Debug debug, QSS_data data, QSS_time time) { return NULL; }

void QSS_freeDt(QSS_dt dt) { return; }

QSS_dtOps QSS_DtOps() { return NULL; }

void QSS_freeDtOps(QSS_dtOps ops) { return; }

QSS_dtState QSS_DtState() { return NULL; }

void QSS_freeDtState(QSS_dtState state) { return; }

void QSS_dtCheck(QSS_dt dt) { return; }

double QSS_dtValue(QSS_dt dt) { return 0; }

bool QSS_dtLogOutput(QSS_dt dt, double Dq, double Dx, double Dt, int variable) { return TRUE; }

bool QSS_dtLogStep(QSS_dt dt, double Dq, double Dx, double Dt) { return TRUE; }

void QSS_dtFinish(QSS_dt dt) { return; }

void QSS_dtUpdate(QSS_dt dt) { return; }

#endif
