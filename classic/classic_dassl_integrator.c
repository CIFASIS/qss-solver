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

#include "classic_dassl_integrator.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "classic_data.h"
#include "classic_integrator.h"
#include "classic_simulator.h"

static CLC_data clcData = NULL;

static CLC_model clcModel = NULL;

static SD_output simOutput = NULL;

void
DASSL_update_x(const double *x)
{
  abort();
  int i;
  for(i = 0; i < clcData->states; i++)
  {
#ifdef DEBUG
    //printf("x[%d]=%g ",i,x[i]);
#endif
    clcData->x[i] = x[i];
  }
#ifdef DEBUG
  //printf("\n");
#endif
}

void
DASSL_reset_x(double *x)
{
  abort();
  int i;
  for(i = 0; i < clcData->states; i++)
  {
    x[i] = clcData->x[i];
  }
}

#define HIST 1e-12
void
DASSL_events(const int *n, const double *t, const double *x, const double *dx,
    const int *nrt, double *res, const double *rPar, const int *iPar)
{
  int i;
  double out;
  //DASSL_update_x(x);
  for(i = 0; i < clcData->events; i++)
  {
    clcModel->events->zeroCrossing(i, x, clcData->d, clcData->alg, t[0], &out);
    res[i] = out + clcData->event[i].zcSign * HIST;
#ifdef DEBUG
    //printf("Evaluating zc %d at %g res=%g. Sign = %d\n",i,t[0],res[i],clcData->events[i].zc_sign);
#endif
  }
}
void
DASSL_model(const double *t, const double *x, const double *dx,
    const double *CJ, double *res, int *iRes, const double *rPar,
    const int *iPar)
{
  int i;
  clcModel->f(x, clcData->d, clcData->alg, t[0], res);
  for(i = 0; i < clcData->states; i++)
    res[i] -= dx[i];
  clcData->funEvaluations++;
}

void
DASSL_integrate(SIM_simulator simulate)
{
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  clcData = simulator->data;
  clcModel = simulator->model;
  simOutput = simulator->output;
  int i;
  double t = clcData->it;
  double tout;
  const double _ft = clcData->ft;
  double dQRel = clcData->dQRel[0];
  double dQMin = clcData->dQMin[0];
  double *_x = clcData->x;
  double *rwork;
  int is_sampled = simOutput->commInterval != CI_Step;
  double step_size;
  if(is_sampled)
  {
    step_size = simOutput->sampled->period[0];
  }
  const int num_steps = (
      is_sampled ? ceil(_ft / step_size) + 2 : MAX_OUTPUT_POINTS);
  double **solution = checkedMalloc(sizeof(double*) * simOutput->outputs);
  double *solution_time = checkedMalloc(sizeof(double) * num_steps);
  double **outvar = checkedMalloc(sizeof(double) * simOutput->outputs);

  int info[20], lrw, liw, *iwork;
  double *x, *dx, rel_tol = dQRel, abs_tol = dQMin;
  int numofconstr = clcData->events, method_info = 0;
  int *root_output;
  int size = clcData->states;
  int event_detected = 0;

  x = checkedMalloc(sizeof(double) * clcData->states);
  dx = checkedMalloc(sizeof(double) * clcData->states);
  root_output = checkedMalloc(sizeof(int) * clcData->events);
  lrw = 5000 + 15000 * clcData->states
      + /*clcData->states * clcData->states +*/8 * clcData->events;
  rwork = checkedMalloc(sizeof(double) * lrw);
  CLC_compute_outputs(simOutput, solution, num_steps);
  for(i = 0; i < clcData->states; i++)
    x[i] = _x[i];
  cleanDoubleVector(dx, 0, clcData->states);
  cleanVector(root_output, 0, clcData->events);
  cleanVector(info, 0, 20);
  if(!is_sampled)
  {
    info[2] = 1;
  }
  liw = 60040;
  iwork = checkedMalloc(sizeof(int) * liw);
  int percentage = 0;
  // Save first step
  CLC_save_step(simOutput, solution, solution_time, t,
      clcData->totalOutputSteps, x, clcData->d, clcData->alg);
  clcData->totalOutputSteps++;
  getTime(simulator->stats->sTime);
#ifdef SYNC_RT
  setInitRealTime();
#endif
  while(t < _ft)
  {
    if(!is_sampled)
    {
      tout = _ft;
    }
    else
    {
      if(!event_detected)
      {
        tout = t + step_size;
      }
      else
      {
        if(fabs(tout - t) < 1e-12)
        {
          CLC_save_step(simOutput, solution, solution_time, tout,
              clcData->totalOutputSteps, x, clcData->d, clcData->alg);
          clcData->totalOutputSteps++;
          tout = t + step_size;
        }
      }
      event_detected = 0;
    }
    if(tout > _ft)
      tout = _ft;
    ddaskr_(DASSL_model, &size, &t, x, dx, &tout, info, &rel_tol, &abs_tol,
        &method_info, rwork, &lrw, iwork, &liw,
        NULL,
        NULL, NULL, NULL, DASSL_events, &numofconstr, root_output);
    if(method_info < 0)
    {
      printf("Error: DASSL returned IDID = %d. Check DASSL documentation\n",
          method_info);
      exit(-1);
    }
#ifdef SYNC_RT
    /* Sync */
    waitUntil(t);
#endif

    if(method_info == 5)
    {
      CLC_handle_event(clcData, clcModel, x, root_output, t, iwork);
      if(is_sampled)
        event_detected = 1;
      info[0] = 0;
    }
    if(!is_sampled)
    {
      CLC_save_step(simOutput, solution, solution_time, t,
          clcData->totalOutputSteps, x, clcData->d, clcData->alg);
      clcData->totalOutputSteps++;
    }
    else
    {
      if(!event_detected)
      {
        if(fabs(tout - solution_time[clcData->totalOutputSteps - 1])
            > step_size / 10)
        {
          CLC_save_step(simOutput, solution, solution_time, tout,
              clcData->totalOutputSteps, x, clcData->d, clcData->alg);
          clcData->totalOutputSteps++;
        }
      }
      else
      {
      }
    }

    if((int) (t * 100 / _ft) > percentage)
    {
      percentage = 100 * t / _ft;
      fprintf(stderr, "*%g", t);
      fflush(stderr);
    }
  }
  /*
   if (!event_detected && is_sampled) {
   if (solution_time[totalOutputSteps]<t) {
   CLC_save_step(simOutput,solution,solution_time,t,totalOutputSteps,x, clcData->d);
   totalOutputSteps++;
   }
   }
   */
  clcData->totalSteps += iwork[10];
  clcData->totalStepsDASSL += iwork[11];
  clcData->totalJacobians += iwork[12];
  clcData->totalCrossingEvaluations += iwork[35];
  getTime(simulator->stats->sTime);
  subTime(simulator->stats->sTime, simulator->stats->iTime);
  if(simulator->settings->debug == 0 || simulator->settings->debug > 1)
  {
    SD_print(simulator->simulationLog, "Simulation time (DASSL):");
    SD_print(simulator->simulationLog, "----------------");
    SD_print(simulator->simulationLog, "Miliseconds: %g",
        getTimeValue(simulator->stats->sTime));
    SD_print(simulator->simulationLog, "Function evaluations: %llu",
        clcData->funEvaluations);
    //SD_print (simulator->simulationLog, "Scalar function evaluations: %d", clcData->scalarEvaluations);
    //SD_print (simulator->simulationLog, "Zero Crossings : %d", clcData->zeroCrossings);
    SD_print(simulator->simulationLog,
        "Function evaluations (reported by DASSL): %d",
        clcData->totalStepsDASSL);
    SD_print(simulator->simulationLog, "Jacobian evaluations : %d",
        clcData->totalJacobians);
    SD_print(simulator->simulationLog, "Zero crossing evaluations : %d",
        clcData->totalCrossingEvaluations);
    SD_print(simulator->simulationLog, "Output steps: %d",
        clcData->totalOutputSteps);
    SD_print(simulator->simulationLog, "Simulation steps: %d",
        clcData->totalSteps);
    SD_print(simulator->simulationLog, "Events detected : %d",
        clcData->totalEvents);
  }
  CLC_write_output(simOutput, solution, solution_time,
      clcData->totalOutputSteps);
  // To avoid QSS output
  free(x);
  free(dx);
  free(outvar);
  free(root_output);
  free(solution_time);
  free(rwork);
  free(iwork);
  for(i = 0; i < simOutput->outputs; i++)
  {
    free(solution[i]);
  }
  free(solution);
}
