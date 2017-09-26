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

/* Header files with a description of contents used */

//#define USE_JACOBIAN
//#ifdef USE_JACOBIAN
#include <ida/ida_klu.h>   /* prototype for CVSUPERLUMT */
#include <sundials/sundials_sparse.h> /* definitions SlsMat */
//#endif

#include <ida/ida.h>
#include <ida/ida_dense.h>
#include <nvector/nvector_serial.h>
#include <sundials/sundials_math.h>
#include <sundials/sundials_types.h>

#include <classic/classic_ida_integrator.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "classic_data.h"
#include "classic_integrator.h"
#include "classic_simulator.h"

#define Ith(v,i)    NV_Ith_S(v,i)       /* Ith numbers components 1..NEQ */
#define IJth(A,i,j) DENSE_ELEM(A,i,j) /* IJth numbers rows,cols 1..NEQ */

static CLC_data clcData = NULL;

static CLC_model clcModel = NULL;

static SD_output simOutput = NULL;

int is_sampled;

//#ifdef USE_JACOBIAN
/* Test jacobian */
static int
IDA_Jac(realtype t, realtype cj, N_Vector y, N_Vector fy, N_Vector resvec,
    SlsMat JacMat, void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
  int n = 0;
  int size = clcData->states, i, m;
  int *colptrs = *JacMat->colptrs;
  int *rowvals = *JacMat->rowvals;

  SparseSetMatToZero(JacMat);
  clcModel->jac(NV_DATA_S(y), clcData->d, clcData->alg, t, JacMat->data);
  for(i = 0; i < size; i++)
  {
    colptrs[i] = n;
    for(m = 0; m < clcData->nSD[i]; m++)
    {
      if(i == clcData->SD[i][m]) // The diagonal derivtive of df/dyp is 1*cj
        JacMat->data[n + m] += cj;
      rowvals[m + n] = clcData->SD[i][m];
    }
    n += clcData->nSD[i];
  }
  colptrs[i] = n;

//  SparsePrintMat(JacMat,stdout);
//  printf("%g\n", cj);
//  abort();

  return 0;
}
//#endif

/* Test jacobian */
static int
check_flag(void *flagvalue, const char *funcname, int opt,
    CLC_simulator simulator)
{
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if(opt == 0 && flagvalue == NULL)
  {
    SD_print(simulator->simulationLog,
        "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
    return 1;
  }

  /* Check if flag < 0 */
  else if(opt == 1)
  {
    errflag = (int *) flagvalue;
    if(*errflag < 0)
    {
      SD_print(simulator->simulationLog,
          "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n", funcname,
          *errflag);
      return 1;
    }
  }

  /* Check if function returned NULL pointer - no memory allocated */
  else if(opt == 2 && flagvalue == NULL)
  {
    SD_print(simulator->simulationLog,
        "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
    return 1;
  }

  return 0;
}

int
IDA_model(realtype t, N_Vector y, N_Vector ydot, N_Vector resval,
    void *user_data)
{
  int i;
  double *res = N_VGetArrayPointer_Serial(resval), *yd =
      N_VGetArrayPointer_Serial(ydot);
  ;
  double *f = (double*) user_data; //malloc(sizeof(double)*clcData->states);
  clcData->funEvaluations++;
  clcModel->f(NV_DATA_S(y), clcData->d, clcData->alg, t, f);
  for(i = 0; i < clcData->states; i++)
  {
    res[i] = yd[i] - f[i];
  }
  //free(f);
  return 0;
}

int
IDA_events(realtype t, N_Vector y, N_Vector yp, realtype *gout, void *user_data)
{
  double out;
  int i;
  for(i = 0; i < clcData->events; i++)
  {
    clcModel->events->zeroCrossing(i, NV_DATA_S(y), clcData->d, clcData->alg, t,
        &out);
    gout[i] = out + clcData->event[i].zcSign * HIST;
  }
  return 0;
}

void
IDA_integrate(SIM_simulator simulate)
{
  CLC_simulator simulator = (CLC_simulator) simulate->state->sim;
  clcData = simulator->data;
  clcModel = simulator->model;
  simOutput = simulator->output;

  N_Vector y, yp, abstol, temp_y = NULL;
  void *mem;
  int i, j, nnz;
  unsigned long totalOutputSteps = 0;
  const double _ft = clcData->ft;
  double dQRel = clcData->dQRel[0];
  double dQMin = clcData->dQMin[0];
  double *_x = clcData->x;
  double step_size = 1;
  int size = clcData->states;
  is_sampled = simOutput->commInterval != CI_Step;
  if(is_sampled)
  {
    temp_y = N_VNew_Serial(size);
    step_size = simOutput->sampled->period[0];
  }
  const int num_steps = (is_sampled ? _ft / step_size + 1 : MAX_OUTPUT_POINTS);
  double **solution = malloc(sizeof(double*) * simOutput->outputs);
  double *solution_time = malloc(sizeof(double) * (num_steps + 1));
  double **outvar = malloc(sizeof(double) * simOutput->outputs);
  int *jroot = malloc(sizeof(int) * clcData->events), flag;
  int event_detected = 0;
  double rel_tol = dQRel, abs_tol = dQMin;
  realtype reltol = rel_tol, t = clcData->it, tout;
  y = abstol = NULL;
  int percentage = 0;
  long int nst = 0, nfe = 0, nni = 0, netf = 0, val;
  CLC_compute_outputs(simOutput, solution, num_steps);

  mem = IDACreate();

  y = N_VNew_Serial(size);
  if(check_flag((void *) y, "N_VNew_Serial", 0, simulator))
    return;

  yp = N_VNew_Serial(size);
  if(check_flag((void *) yp, "N_VNew_Serial", 0, simulator))
    return;

  abstol = N_VNew_Serial(size);
  if(check_flag((void *) abstol, "N_VNew_Serial", 0, simulator))
    return;

  for(i = 0; i < clcData->states; i++)
  {
    Ith(y,i) = _x[i];
    Ith(abstol,i) = abs_tol;
  }
  clcModel->f(NV_DATA_S(y), clcData->d, clcData->alg, t, NV_DATA_S(yp));

  flag = IDAInit(mem, IDA_model, t, y, yp);
  if(check_flag(&flag, "IDAInit", 1, simulator))
    return;
  flag = IDASetErrFile(mem, stdout);

  flag = IDASVtolerances(mem, reltol, abstol);
  if(check_flag(&flag, "IDASVtolerances", 1, simulator))
    return;
  N_VDestroy_Serial(abstol);

  flag = IDARootInit(mem, clcData->events, IDA_events);
  if(check_flag(&flag, "IDARootInit", 1, simulator))
    return;

//#ifndef USE_JACOBIAN
  if(simulator->data->params->jacobian == 1)
  {
    flag = IDADense(mem, size);
    if(check_flag(&flag, "IDADense", 1, simulator))
      return;
  }
  else
  {
    nnz = 0;
    for(i = 0; i < size; i++)
    {
      nnz += clcData->nSD[i];
    }
    //flag = IDASuperLUMT(mem, 1, size, nnz);
    //if(check_flag(&flag, "IDASuperLUMT", 1, simulator)) return;

    flag = IDAKLU(mem, size, nnz, CSC_MAT);
    if(check_flag(&flag, "IDAKLU", 1, simulator))
      return;

    flag = IDASlsSetSparseJacFn(mem, IDA_Jac);
    if(check_flag(&flag, "IDASlsSetSparseJacFn", 1, simulator))
      return;
  }

//#endif
  double *res = malloc(sizeof(double) * size);
  IDASetUserData(mem, res);
  getTime(simulator->stats->sTime);
  if(is_sampled)
  {
    CLC_save_step(simOutput, solution, solution_time, t, totalOutputSteps,
        NV_DATA_S(y), clcData->d, clcData->alg);
    totalOutputSteps++;
  }
#ifdef SYNC_RT
  setInitRealTime();
#endif
  double last_step = t;
  tout = _ft;
  while(t < _ft)
  {
    flag = IDASolve(mem, tout, &t, y, yp, IDA_ONE_STEP);
    if(flag == IDA_SUCCESS)
    {
      if(is_sampled)
      {
        while(last_step + step_size < t)
        {
          if(fabs(last_step + step_size - _ft) / step_size < 1)
            break;
          flag = IDAGetDky(mem, last_step + step_size, 0, temp_y);
          check_flag(&flag, "IDAGetDky", 1, simulator);
          if(IDA_BAD_T == flag)
          {
            printf("Interpolation failed at %g. Last_step = %g t=%g\n",
                last_step + step_size, last_step, t);
            abort();
          }
          CLC_save_step(simOutput, solution, solution_time,
              last_step + step_size, totalOutputSteps, NV_DATA_S(temp_y),
              clcData->d,
              clcData->alg);
          totalOutputSteps++;
          last_step += step_size;
        }
      }
      else
      {
        CLC_save_step(simOutput, solution, solution_time, t, totalOutputSteps,
            NV_DATA_S(y), clcData->d, clcData->alg);
        totalOutputSteps++;
      }
    }
    else if(flag == IDA_ROOT_RETURN)
    {
      flag = IDAGetRootInfo(mem, jroot);
      if(check_flag(&flag, "IDAGetRootInfo", 1, simulator))
        return;
      CLC_handle_event(clcData, clcModel, NV_DATA_S(y), jroot, t, NULL);
      flag = IDAGetNumSteps(mem, &val);
      check_flag(&flag, "IDAGetNumSteps", 1, simulator);
      nst += val;
      flag = IDAGetNumResEvals(mem, &val);
      check_flag(&flag, "IDAGetNumResEvals", 1, simulator);
      nfe += val;
      flag = IDAGetNumErrTestFails(mem, &val);
      check_flag(&flag, "IDAGetNumErrTestFails", 1, simulator);
      netf += val;
      flag = IDAGetNumNonlinSolvIters(mem, &val);
      check_flag(&flag, "IDAGetNumNonlinSolvIters", 1, simulator);
      nni += val;
      clcModel->f(NV_DATA_S(y), clcData->d, clcData->alg, t, NV_DATA_S(yp));
      if(is_sampled)
      {
        while(last_step + step_size < t)
        {
          if(fabs(last_step + step_size - _ft) / step_size < 1)
            break;
          flag = IDAGetDky(mem, last_step + step_size, 0, temp_y);
          check_flag(&flag, "IDAGetDky", 1, simulator);
          if(IDA_BAD_T == flag)
          {
            printf("Interpolation failed at %g. Last_step = %g t=%g\n",
                last_step + step_size, last_step, t);
            abort();
          }
          CLC_save_step(simOutput, solution, solution_time,
              last_step + step_size, totalOutputSteps, NV_DATA_S(temp_y),
              clcData->d,
              clcData->alg);
          totalOutputSteps++;
          last_step += step_size;
        }
      }
      else
      { // When a root is found and the per-step output is selected, take roots as outputs
        CLC_save_step(simOutput, solution, solution_time, t, totalOutputSteps,
            NV_DATA_S(y), clcData->d, clcData->alg);
        totalOutputSteps++;
      }
      IDAReInit(mem, t, y, yp);
    }
    else
    {
      SD_print(simulator->simulationLog, "IDA failed at t=%g\n", t);
      break;
    }
    if((int) (t * 100 / _ft) > percentage)
    {
      percentage = 100 * t / _ft;
      fprintf(stderr, "*%g", t);
      fflush(stderr);
    }
#ifdef SYNC_RT
    waitUntil(t);
#endif
  }
  getTime(simulator->stats->sTime);
  if(is_sampled)
  {
    if(totalOutputSteps < num_steps)
    {
      CLC_save_step(simOutput, solution, solution_time, _ft, totalOutputSteps,
          NV_DATA_S(y), clcData->d, clcData->alg);
      totalOutputSteps++;
    }
  }
  subTime(simulator->stats->sTime, simulator->stats->iTime);
  if(simulator->settings->debug == 0 || simulator->settings->debug > 1)
  {
    flag = IDAGetNumSteps(mem, &val);
    check_flag(&flag, "IDAGetNumSteps", 1, simulator);
    nst += val;
    flag = IDAGetNumResEvals(mem, &val);
    check_flag(&flag, "IDAGetNumResEvals", 1, simulator);
    nfe += val;
    flag = IDAGetNumErrTestFails(mem, &val);
    check_flag(&flag, "IDAGetNumErrTestFails", 1, simulator);
    netf += val;
    flag = IDAGetNumNonlinSolvIters(mem, &val);
    check_flag(&flag, "IDAGetNumNonlinSolvIters", 1, simulator);
    nni += val;

    SD_print(simulator->simulationLog, "Simulation time (IDA)");
    SD_print(simulator->simulationLog, "----------------");
    SD_print(simulator->simulationLog, "Miliseconds: %g",
        getTimeValue(simulator->stats->sTime));
    SD_print(simulator->simulationLog, "Scalar function evaluations: %d",
        clcData->scalarEvaluations);
    SD_print(simulator->simulationLog, "Individual Zero Crossings : %d",
        clcData->zeroCrossings);
    SD_print(simulator->simulationLog, "Function evaluations: %llu",
        clcData->funEvaluations);
    SD_print(simulator->simulationLog,
        "Residual evaluations (reported by IDA): %ld", nfe);
    SD_print(simulator->simulationLog, "Output steps: %d", totalOutputSteps);
    SD_print(simulator->simulationLog, "Simulation steps: %ld", nst);
    SD_print(simulator->simulationLog, "Simulation steps (rejected) : %ld",
        netf);
    SD_print(simulator->simulationLog,
        "Nonlinear solver iterations performed: (reported by IDA): %ld", nni);
    SD_print(simulator->simulationLog, "Events detected : %d",
        clcData->totalEvents);
  }
  CLC_write_output(simOutput, solution, solution_time, totalOutputSteps);
  /* Free y and abstol vectors */
  N_VDestroy_Serial(y);
  N_VDestroy_Serial(yp);
  if(temp_y)
    N_VDestroy_Serial(temp_y);

  /* Free integrator memory */
  IDAFree(&mem);
  free(res);
  free(outvar);
  free(solution_time);
  free(jroot);
  for(i = 0; i < simOutput->outputs; i++)
    free(solution[i]);
  free(solution);

}
