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

#include <time.h>

#include "../common/integrator.h"

#ifdef __linux__
#define _GNU_SOURCE
#define __USE_GNU
#include <fenv.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void fpe_handler(int dummy)
{
  printf("Floating point exception\n");
  abort();
}
#else

#include <stdio.h>
#include <stdlib.h>
#endif

#include <common/random.h>
#include <qss/qss_commands.h>
#include <qss/qss_simulator.h>
#include <qss/qss_model.h>

QSS_simulator QSS_Simulator()
{
  QSS_simulator p = checkedMalloc(sizeof(*p));
  p->quantizer = NULL;
  p->log = NULL;
  p->scheduler = NULL;
  p->frw = NULL;
  p->data = NULL;
  p->time = NULL;
  p->model = NULL;
  p->output = NULL;
  p->settings = NULL;
  p->simulationLog = NULL;
  p->dt = NULL;
  p->id = 0;
  p->lpTime = NULL;
  p->lpDtMin = NULL;
  p->mailbox = NULL;
  p->inbox = NULL;
  p->ack = NULL;
  p->stats = NULL;
  p->lps = NULL;
  p->dtSynch = NULL;
  p->simSteps = NULL;
  return p;
}

void QSS_freeSimulator(QSS_simulator simulator)
{
  SD_freeSimulationLog(simulator->simulationLog);
  QSS_freeTime(simulator->time, simulator->data->events, simulator->data->inputs);
  if (simulator->settings->parallel) {
    if (simulator->id == ROOT_SIMULATOR) {
      SD_freeOutput(simulator->output, simulator->data->states, simulator->data->discretes);
      QSS_freeModel(simulator->model);
      SD_freeSimulationSettings(simulator->settings);
      free(simulator->lpTime);
      free(simulator->lpDtMin);
      QSS_LP_freeDataArray(simulator->lps);
      MLB_freeMailbox(simulator->mailbox);
      QSS_freeDtSynch(simulator->dtSynch);
      QSS_freeData(simulator->data);
    } else {
      QA_freeQuantizer(simulator->quantizer);
      OUT_freeOutput(simulator->log);
      SC_freeScheduler(simulator->scheduler);
      FRW_freeFramework(simulator->frw);
      QSS_freeDt(simulator->dt);
      QSS_cleanData(simulator->data);
      QSS_freeSimSteps(simulator->simSteps);
    }
    SD_freeStatistics(simulator->stats);
  } else {
    QA_freeQuantizer(simulator->quantizer);
    OUT_freeOutput(simulator->log);
    SC_freeScheduler(simulator->scheduler);
    FRW_freeFramework(simulator->frw);
    SD_freeOutput(simulator->output, simulator->data->states, simulator->data->discretes);
    QSS_freeData(simulator->data);
    QSS_freeModel(simulator->model);
    SD_freeSimulationSettings(simulator->settings);
    SD_freeStatistics(simulator->stats);
  }
  free(simulator);
}

void QSS_simulatorEnd(SIM_simulator simulate)
{
  QSS_simulator simulator = (QSS_simulator)simulate->state->sim;
  QSS_freeSimulator(simulator);
  freeRandom();
}

void QSS_simulate(SIM_simulator simulate)
{
  QSS_simulator simulator = (QSS_simulator)simulate->state->sim;
  Random(simulator->settings->random_seed);
  INT_integrator integrator = INT_Integrator(simulate);
  /*#ifdef __linux__
   signal (SIGFPE, fpe_handler);
   feenableexcept (FE_DIVBYZERO);
   #endif*/
  getTime(simulator->stats->iTime);
  QSS_initializeDataStructs(simulator);
  // QSS_orderDataMatrix(simulator->data);
  getTime(simulator->stats->sdTime);
  subTime(simulator->stats->sdTime, simulator->stats->iTime);
  simulator->stats->initTime = getTimeValue(simulator->stats->sdTime);
  QSS_CMD_alloc(simulator);
  INT_initialize(integrator, simulate);
  INT_integrate(integrator, simulate);
  INT_freeIntegrator(integrator);
  QSS_CMD_free();
}

void QSS_initSimulator(SIM_simulator simulator)
{
  simulator->state->sim = (void*)QSS_Simulator();
  ((QSS_simulator)simulator->state->sim)->settings = simulator->state->settings;
  ((QSS_simulator)simulator->state->sim)->stats = SD_Statistics();
  simulator->ops->simulate = QSS_simulate;
  simulator->ops->freeSimulator = QSS_simulatorEnd;
}
