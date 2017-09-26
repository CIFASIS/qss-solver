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

#include <classic/classic_dopri_integrator.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "classic_data.h"
#include "classic_integrator.h"
#include "classic_simulator.h"
#include "dopri5/dopri5.h"

static CLC_data clcData = NULL;

static CLC_model clcModel = NULL;

static SD_output simOutput = NULL;

int is_sampled;

int dopri_percentage = 0;

struct simData_DOPRI
{
  double **solution;
  double *solution_time;
  double **outvar;
  double *x;
  double *t;
  unsigned long *totalOutputSteps;
  double last_step;
  double step_size;
  double *temp_x;
  double final_time;
  int size;
};

struct simData_DOPRI simDataDopri;

void
DOPRI_events(int n, double x, const double *y, double *g)
{
  double out;
  clcModel->events->zeroCrossing(n, y, clcData->d, clcData->alg, x, &out);
  g[0] = out + clcData->event[n].zcSign * HIST;
}

void
DOPRI_model(unsigned n, double x, double *y, double *f)
{
  clcData->funEvaluations++;
  clcModel->f(y, clcData->d, clcData->alg, x, f);
}

void
DOPRI_solout(long nr, double xold, double x, double* y, unsigned n, int* irtrn,
    int *jroot)
{
  const double _ft = clcData->ft;
#ifdef SYNC_RT
  /* Sync */
  waitUntil(x);
#endif
  if(irtrn[0] == 3)
    CLC_handle_event(clcData, clcModel, y, jroot, x, NULL);
  if(!is_sampled)
  {
    clcData->totalSteps++;
    CLC_save_step(simOutput, simDataDopri.solution, simDataDopri.solution_time,
        x, simDataDopri.totalOutputSteps[0], y, clcData->d, clcData->alg);
    simDataDopri.totalOutputSteps[0]++;
  }
  else
  { // Do sample
    while(simDataDopri.last_step + simDataDopri.step_size < x)
    {
      // Skip last step
      if(fabs(
          simDataDopri.last_step + simDataDopri.step_size
              - simDataDopri.final_time) / simDataDopri.step_size < 1)
        break;
      clcData->totalSteps++;
      int i;
      for(i = 0; i < simDataDopri.size; i++)
        simDataDopri.temp_x[i] = contd5(i,
            simDataDopri.last_step + simDataDopri.step_size);
      CLC_save_step(simOutput, simDataDopri.solution,
          simDataDopri.solution_time,
          simDataDopri.last_step + simDataDopri.step_size,
          simDataDopri.totalOutputSteps[0], simDataDopri.temp_x, clcData->d,
          clcData->alg);
      simDataDopri.totalOutputSteps[0]++;
      simDataDopri.last_step += simDataDopri.step_size;
    }
  }
  if((int) (x * 100 / _ft) > dopri_percentage)
  {
    dopri_percentage = 100 * x / _ft;
    fprintf(stderr, "*%g", x);
    fflush(stderr);
  }
}

void
DOPRI_integrate(SIM_simulator simulate)
{
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  clcData = simulator->data;
  clcModel = simulator->model;
  simOutput = simulator->output;
  int i;
  unsigned long totalOutputSteps = 0;
  double t = clcData->it;
  const double _ft = clcData->ft;
  double dQRel = clcData->dQRel[0];
  double dQMin = clcData->dQMin[0];
  double *_x = clcData->x;
  double step_size = 1;
  is_sampled = simOutput->commInterval != CI_Step;
  if(is_sampled)
  {
    step_size = simOutput->sampled->period[0];
  }
  const int num_steps = (is_sampled ? _ft / step_size + 1 : MAX_OUTPUT_POINTS);
  double **solution = malloc(sizeof(double*) * simOutput->outputs);
  double *solution_time = malloc(sizeof(double) * num_steps);
  double **outvar = malloc(sizeof(double) * simOutput->outputs);
  int *jroot = malloc(sizeof(int) * clcData->events);
  double troot = 0;
  int size = clcData->states, res;
  double *x, *temp_x, rel_tol = dQRel, abs_tol = dQMin;
  x = checkedMalloc(sizeof(double) * clcData->states);
  temp_x = checkedMalloc(sizeof(double) * clcData->states);
  CLC_compute_outputs(simOutput, solution, num_steps);
  for(i = 0; i < clcData->states; i++)
    x[i] = _x[i];
  simDataDopri.solution = solution;
  simDataDopri.solution_time = solution_time;
  simDataDopri.x = x;
  simDataDopri.temp_x = temp_x;
  simDataDopri.outvar = outvar;
  simDataDopri.totalOutputSteps = &totalOutputSteps;
  simDataDopri.step_size = step_size;
  simDataDopri.final_time = _ft;
  simDataDopri.size = clcData->states;
  dopri_percentage = 0;
  getTime(simulator->stats->sTime);
  if(is_sampled)
  {
    CLC_save_step(simOutput, solution, solution_time, t, totalOutputSteps, x,
        clcData->d, clcData->alg);
    totalOutputSteps++;
  }
#ifdef SYNC_RT
  setInitRealTime();
#endif
  res = dopri5(size, DOPRI_model, t, x, _ft, &rel_tol, &abs_tol, 0,
      DOPRI_solout, 2,
      stdout,
      1e-30, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 2000000000, 0, -1, size,
      NULL,
      0, DOPRI_events, clcData->events, jroot, &troot);
  if(res < 0)
  {
    SD_print(simulator->simulationLog, "DOPRI failed with res=%d at %g\n", res,
        t);
    printf("DOPRI failed with res=%d at %g\n", res, t);
  }
#ifdef SYNC_RT
  /* Sync */
  waitUntil(t);
#endif

  if(is_sampled)
  {
    if(totalOutputSteps < num_steps)
    {
      CLC_save_step(simOutput, solution, solution_time, _ft, totalOutputSteps,
          x, clcData->d, clcData->alg);
      totalOutputSteps++;
    }
  }
  getTime(simulator->stats->sTime);
  subTime(simulator->stats->sTime, simulator->stats->iTime);
  if(simulator->settings->debug == 0 || simulator->settings->debug > 1)
  {
    SD_print(simulator->simulationLog, "Simulation time (DOPRI):");
    SD_print(simulator->simulationLog, "----------------");
    SD_print(simulator->simulationLog, "Miliseconds: %g",
        getTimeValue(simulator->stats->sTime));
    SD_print(simulator->simulationLog, "Scalar function evaluations: %d",
        clcData->scalarEvaluations);
    SD_print(simulator->simulationLog, "Individual Zero Crossings : %d",
        clcData->zeroCrossings);
    SD_print(simulator->simulationLog, "Function evaluations: %llu",
        clcData->funEvaluations);
    //printLog("Function evaluations (reported by DASSL): %d",totalStepsDASSL);
    //printLog("Zero crossing evaluations : %d", totalCrossingEvaluations );
    SD_print(simulator->simulationLog, "Output steps: %d", totalOutputSteps);
    SD_print(simulator->simulationLog, "Simulation steps: %d", nstepRead());
    SD_print(simulator->simulationLog, "Simulation steps (accepted) : %d",
        naccptRead());
    SD_print(simulator->simulationLog, "Simulation steps (rejected) : %d",
        nrejctRead());
    SD_print(simulator->simulationLog,
        "Function evaluations (reported by DOPRI): %d", nfcnRead());
    SD_print(simulator->simulationLog, "Events detected : %d",
        clcData->totalEvents);
  }
  CLC_write_output(simOutput, solution, solution_time, totalOutputSteps);
  free(outvar);
  free(x);
  free(temp_x);
  free(solution_time);
  free(jroot);
  for(i = 0; i < simOutput->outputs; i++)
  {
    free(solution[i]);
  }
  free(solution);
}
