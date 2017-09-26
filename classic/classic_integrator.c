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

#include "classic_integrator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/utils.h"
#include "classic_simulator.h"

void
CLC_initialize(SIM_simulator simulate)
{
  int i, s, forUL;
  double e, zc[4];
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  CLC_data clcData = simulator->data;
  CLC_model clcModel = simulator->model;
  double t = 0;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
  if (settings->debug > 2)
  {
    SD_print (simulationLog, "Initialize simulation\n");
  }
#endif
  forUL = clcData->states;
  for(i = 0; i < forUL; i++)
  {
#ifdef DEBUG
    if (settings->debug > 2)
    {
      SD_print (simulationLog, "\nInitial value: x[%d][0] = %g", i, clcData->x[i]);
    }
#endif
  }
#ifdef DEBUG
  if (settings->debug > 2)
  {
    SD_print (simulationLog, "\nInitialize events...");
  }
#endif
  forUL = clcData->events;
  for(i = 0; i < forUL; i++)
  {
    e = clcData->ft * clcData->params->zcHyst;
    clcModel->events->zeroCrossing(i, clcData->x, clcData->d, clcData->alg, t,
        zc);
    s = sign(zc[0]);
    clcData->event[i].zcSign = s;
    clcData->event[i].zcHyst = e / 10.0;
  }
#ifdef DEBUG
  if (settings->debug > 2)
  {
    SD_print (simulationLog, "done.");
  }
#endif
  getTime(simulator->stats->iTime);
#ifdef DEBUG
  if (settings->debug > 0)
  {
    SD_setSimulationLogVariables(simulationLog, clcData->states, clcData->events);
  }
#endif
}

void
CLC_write_output(SD_output simOutput, double **solution, double *solution_time,
    int totalOutputSteps)
{
  int i, j;
  char name[128];
  for(i = 0; i < simOutput->outputs; i++)
  {
    sprintf(name, "%s.dat", simOutput->variable[i].name);
    FILE *out = fopen(name, "w");
    for(j = 0; j < totalOutputSteps; j++)
      fprintf(out, "%13.13g\t\t%13.13g\n", solution_time[j], solution[i][j]);
    fclose(out);
  }

}

void
CLC_compute_outputs(SD_output simOutput, double **solution, int num_steps)
{
  int i;
  for(i = 0; i < simOutput->outputs; i++)
  {
    solution[i] = checkedMalloc(sizeof(double) * num_steps);
  }
}

void
CLC_save_step(SD_output simOutput, double **solution, double *solution_time,
    const double t, const int totalOutputSteps, double *x, double *d, double *a)
{
  int i;
  if(totalOutputSteps == MAX_OUTPUT_POINTS)
  {
    CLC_write_output(simOutput, solution, solution_time, totalOutputSteps);
    printf("Maximum number of output steps reached\n");
    abort();
  }
  solution_time[totalOutputSteps] = t;
  for(i = 0; i < simOutput->outputs; i++)
  {
    double out;
    simOutput->value(i, x, d, a, t, &out);
    solution[i][totalOutputSteps] = out;
  }
}

void
CLC_handle_event(CLC_data clcData, CLC_model clcModel, double *x,
    int *root_output, double t, int *iwork)
{
  int i, j;
  // Event Detected
  //update_x(x);
  int recheck = 0;
  for(i = 0; i < clcData->events; i++)
  {
    if(root_output[i] == 1 && clcData->event[i].direction >= 0)
    {
#ifdef DEBUG
      printf ("Execution handler pos for zc %d at %g\n", i, t);
#endif
      clcModel->events->handlerPos(i, x, clcData->d, clcData->alg, t);
      recheck = 1;
      clcData->event[i].zcSign = 1;
#ifdef DEBUG
      printf ("Changing sign of %d to %d at %g\n", i,
          clcData->event[i].zcSign, t);
#endif
      //reset_x(x);
      clcData->totalEvents++;
      break;
    }
    else if(root_output[i] == -1 && clcData->event[i].direction <= 0)
    {
#ifdef DEBUG
      printf ("Execution handler neg for zc %d at %g\n", i, t);
#endif
      clcModel->events->handlerNeg(i, x, clcData->d, clcData->alg, t);
      clcData->event[i].zcSign = -1;
#ifdef DEBUG
      printf ("Changing sign of %d to %d at %g\n", i,
          clcData->event[i].zcSign, t);
#endif
      recheck = 1;
      //reset_x(x);
      clcData->totalEvents++;
      break;
    }
    else if(root_output[i] != 0)
    {
      clcData->event[i].zcSign *= -1;
#ifdef DEBUG
      printf ("Changing sign of %d to %d at %g\n", i,
          clcData->event[i].zcSign, t);
#endif
    }
  }
  int iter = 0;
  memset(clcData->fired, 0, clcData->events);
  while(recheck && iter++ < 100)
  {
    recheck = 0;
    //update_x(x);
    double out;
    for(j = 0; j < clcData->events; j++)
    {
      clcModel->events->zeroCrossing(j, x, clcData->d, clcData->alg, t, &out);
#ifdef DEBUG
      printf (
          "Re-checking handler. Evaluating zc %d at %g res=%g. Sign = %d\n",
          j, t, out, clcData->event[j].zcSign);
#endif
      if(out * clcData->event[j].zcSign < 0)
      {
#ifdef DEBUG
        printf (
            "Must execute linked handler %d at %g. out=%g sign=%d dir=%d\n",
            j, t, out, clcData->event[j].zcSign,
            clcData->event[j].direction);
#endif
        if(/*out>HIST &&*/clcData->event[j].zcSign < 0
            && clcData->event[j].direction >= 0 && clcData->fired[j] < 3)
        {
#ifdef DEBUG
          printf ("Execution handler pos for zc %d at %g\n", j, t);
#endif
          clcModel->events->handlerPos(j, x, clcData->d, clcData->alg, t);
          recheck = 1;
          clcData->fired[j]++;
          //reset_x(x);
          clcData->totalEvents++;
        }
        else if(/*out<-HIST &&*/clcData->event[j].zcSign > 0
            && clcData->event[j].direction <= 0 && clcData->fired[j] < 3)
        {
#ifdef DEBUG
          printf ("Execution handler neg for zc %d at %g\n", j, t);
#endif
          clcModel->events->handlerNeg(j, x, clcData->d, clcData->alg, t);
          recheck = 1;
          clcData->fired[j]++;
          //reset_x(x);
          clcData->totalEvents++;
        }
        clcData->event[j].zcSign *= -1;
#ifdef DEBUG
        printf ("Changing sign of %d to %d at %g\n", j,
            clcData->event[j].zcSign, t);
#endif
      }
    }
  }
  if(iter >= 100)
  {
    printf(
        "Maximum number of event iterations. Probably there is a loop dependency in the events\n");
  }
  if(iwork != NULL)
  {
    clcData->totalSteps += iwork[10];
    clcData->totalStepsDASSL += iwork[11];
    clcData->totalJacobians += iwork[12];
    clcData->totalCrossingEvaluations += iwork[35];
  }
}
