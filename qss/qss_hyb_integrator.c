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

#include "qss_hyb_integrator.h"

#include <math.h>

#include <cvode/cvode.h>             /* prototypes for CVODE fcts., consts. */
#include <nvector/nvector_serial.h>  /* serial N_Vector types, fcts., macros */
#include <sundials/sundials_types.h> /* definition of type realtype */

#include <cvode/cvode_dense.h>        /* prototype for CVDense */
#include <cvode/cvode_klu.h>          /* prototype for CVSUPERLUMT */
#include <sundials/sundials_dense.h>  /* definitions DlsMat DENSE_ELEM */
#include <sundials/sundials_sparse.h> /* definitions SlsMat */

#include "../common/data.h"
#include "../common/simulator.h"
#include "../common/utils.h"
#include "qss_bdf.h"
#include "qss_data.h"
#include "qss_frw.h"
#include "qss_integrator.h"
#include "qss_output.h"
#include "qss_quantizer.h"
#include "qss_scheduler.h"
#include "qss_simulator.h"


#define Ith(v,i)    NV_Ith_S(v,i)       /* Ith numbers components 1..NEQ */
#define IJth(A,i,j) DENSE_ELEM(A,i,j) /* IJth numbers rows,cols 1..NEQ */

static QSS_data clcData = NULL;

static QSS_model clcModel = NULL;

static int Jac(realtype t, N_Vector y, N_Vector fy, SlsMat JacMat,
               void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3) {
  int n = 0;
  int jacIt, i, j;
  int *colptrs = *JacMat->colptrs;
  int *rowvals = *JacMat->rowvals;
  int nBDF = clcData->nBDF;
  int *BDF = clcData->BDF;
  int *BDFMap = clcData->BDFMap;
  int *nSD = clcData->nSD;
  int **SD = clcData->SD;
  int *JacIt = clcData->JacIt;
  const int coeffs = clcData->order + 1;
  double *q = clcData->q;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    q[cf0] = Ith(y,i);
  }
  SparseSetMatToZero(JacMat);
  clcModel->jac(q, clcData->d, clcData->alg, t, clcData->jac);
  int jval = 0, jcol = 0; 
  for (i = 0, jacIt = 0; i < nBDF; i++) {
    colptrs[i] = n;
    int row = BDFMap[i];
    int nsd = nSD[row];
    jcol = 0;
    for (j = 0; j < nsd; j++, jacIt++) {
      int sd = SD[row][j];
      int col = BDF[sd];
      if (col != NOT_ASSIGNED) {
        JacMat->data[jval++] =  clcData->jac[JacIt[jacIt]];
        rowvals[jcol + n] = col;
        jcol++;
      }
    }
    n += jcol;
  }
  colptrs[i] = n;
  return 0;
}

static int check_flag(void *flagvalue, const char *funcname, int opt,
                      QSS_simulator simulator) {
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    SD_print(simulator->simulationLog,
             "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
             funcname);
    return 1;
  }

  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *)flagvalue;
    if (*errflag < 0) {
      SD_print(simulator->simulationLog,
               "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n", funcname,
               *errflag);
      return 1;
    }
  }

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    SD_print(simulator->simulationLog,
             "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
             funcname);
    return 1;
  }

  return 0;
}

int QSS_HYB_BDF_model(realtype t, N_Vector y, N_Vector ydot, void *user_data) {
  int nBDF = clcData->nBDF, i;
  int *BDFMap = clcData->BDFMap;
  double *q = clcData->q;
  double *d = clcData->d;
  double *a = clcData->alg;
  double *x = clcData->x;
  const int coeffs = clcData->order + 1;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    q[cf0] = Ith(y,i);
  }
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    clcModel->f(bdfVar, q, d, a, t, &(x[cf0]));
    Ith(ydot,i) = x[cf0 + 1];
  }
  return 0;
}

void QSS_HYB_initialize(SIM_simulator simulate) {
  QSS_simulator simulator = (QSS_simulator)simulate->state->sim;
  // Local mappings.
  QSS_data qssData = simulator->data;
  double *x = qssData->x;
  double t = simulator->time->time;
  QSS_model qssModel = simulator->model;
  if (qssModel->jac != NULL) {
    qssModel->jac(x, qssData->d, qssData->alg, t, qssData->jac);
  }
  QSS_BDF_partition(qssData, simulator->output->name);
  QSS_SEQ_initialize(simulate);
}

void QSS_HYB_integrate(SIM_simulator simulate) {
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
  clcData = qssData;
  clcModel = qssModel;
#ifdef DEBUG
  SD_simulationSettings settings = simulator->settings;
  SD_simulationLog simulationLog = simulator->simulationLog;
#endif
  unsigned long totalSteps = 0;
  unsigned long reinits = 0;
  double t = qssTime->time;
  int index = qssTime->minIndex, flag;
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
  int *BDF = qssData->BDF;
  int *BDFMap = qssData->BDFMap;
  int nBDF = qssData->nBDF;
  int nBDFOutputs = qssData->nBDFOutputs;
  int *BDFOutputs = qssData->BDFOutputs;
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
  realtype reltol = qssData->dQRel[0], ct, tout;
  N_Vector y, abstol;
  void *cvode_mem;
  cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
  y = N_VNew_Serial(nBDF);
  if (check_flag((void *)y, "N_VNew_Serial", 0, simulator)) return;

  abstol = N_VNew_Serial(nBDF);
  if (check_flag((void *)abstol, "N_VNew_Serial", 0, simulator)) return;
  
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs; 
    Ith(y, i) = x[cf0];
    Ith(abstol, i) = qssData->dQMin[0];
  }

  bool reinit = FALSE;
  flag = CVodeInit(cvode_mem, QSS_HYB_BDF_model, t, y);
  if (check_flag((void *)cvode_mem, "CVodeCreate", 0, simulator)) return;

  flag = CVodeSVtolerances(cvode_mem, reltol, abstol);
  if (check_flag(&flag, "CVodeInit", 1, simulator)) return;

  int nnz = 0;
  for (i = 0; i < nBDF; i++) {
    int row = BDFMap[i];
    int nsd = qssData->nSD[row];
    for (j = 0; j < nsd; j++) {
      int sd = SD[row][j];
      int col = BDF[sd];
      if (col != NOT_ASSIGNED) {
        nnz++;
      }
    }
  }

  flag = CVKLU(cvode_mem, nBDF, nnz, CSC_MAT);
  if (check_flag(&flag, "CVKLU", 1, simulator)) return;
  /* Set the Jacobian routine to Jac (user-supplied) */
  flag = CVSlsSetSparseJacFn(cvode_mem, Jac);
  if (check_flag(&flag, "CVSlsSetSparseJacFn", 1, simulator)) return;

#ifdef DEBUG
  if (settings->debug & SD_DBG_StepInfo) {
    SD_print(simulator->simulationLog, "Begin Simulation:");
  }
#endif
  tout = ft;
  ct = t;
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
        if (BDF[index] != NOT_ASSIGNED) {
          ct = t;
          if (reinit) {
            for (i = 0; i < nBDF; i++) {
              int bdfVar = BDFMap[i];
              cf0 = bdfVar * coeffs;
              Ith(y, i) = q[cf0];
              tq[bdfVar] = ct;
              tx[bdfVar] = ct;
            }
            CVodeReInit(cvode_mem, ct, y);
            reinit = FALSE;
          }
          flag = CVode(cvode_mem, tout, y, &ct, CV_ONE_STEP);
          for (i = 0; i < nBDF; i++) {
            int bdfVar = BDFMap[i];
            cf0 = bdfVar * coeffs;
            nextStateTime[bdfVar] = ct;
            q[cf0] =  Ith(y, i);
          }          
          for (i = 0; i < nBDFOutputs; i++) {
            int bdfVar = BDFOutputs[i], inf;
            nSD = qssData->nSD[bdfVar];
            for (inf = 0; inf < nSD; inf++) {
              j = SD[bdfVar][inf];
              elapsed = t - tx[j];
              infCf0 = j * coeffs;
              if (elapsed > 0) {
                x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
                tx[j] = t;
              }
            }
            FRW_recomputeDerivatives(frw, qssModel, qssData, qssTime, bdfVar);
            if (qssData->events > 0) {
              int inf;
              nSZ = qssData->nSZ[bdfVar];
              for (inf = 0; inf < nSZ; inf++) {
                j = SZ[bdfVar][inf];
                FRW_nextEventTime(frw, qssModel, qssData, qssTime, j);
              }
            }
            QA_recomputeNextTimes(quantizer, nSD, SD[bdfVar], t, nextStateTime,
                                  x, lqu, q);
          }
          if (nOutputs) {
            for (i = 0; i < nBDF; i++) {
              int bdfVar = BDFMap[i];
              if (output->nSO[bdfVar]) {
                qssTime->minIndex = bdfVar;
                OUT_write(log, qssData, qssTime, output);
              }
            }
            qssTime->minIndex = index;
          }
        } else {
          reinit = TRUE;
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
          QA_recomputeNextTimes(quantizer, nSD, SD[index], t, nextStateTime, x,
                                lqu, q);
          if (nOutputs) {
            if (output->nSO[index]) {
              OUT_write(log, qssData, qssTime, output);
            }
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
              QA_recomputeNextTimes(quantizer, nSD, SD[j], t, nextStateTime, x,
                                    lqu, q);
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
              FRW_recomputeDerivative(frw, simulator->model, qssData, qssTime,
                                      j);
            }
            if (nHD) {
              QA_recomputeNextTimes(quantizer, nHD, HD[index], t, nextStateTime,
                                    x, lqu, q);
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
