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
#include <stdlib.h>

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

#define Ith(v, i) NV_Ith_S(v, i)          /* Ith numbers components 1..NEQ */
#define IJth(A, i, j) DENSE_ELEM(A, i, j) /* IJth numbers rows,cols 1..NEQ */

static QSS_data clcData = NULL;

static QSS_time clcTime = NULL;

static QSS_model clcModel = NULL;

static double *qBDF = NULL;

static double *qQSS = NULL;

static double *bdfQ = NULL;

static double BDFIntegrationTime = 0;

static double *Q = NULL;

static double *qssTQ = NULL;

static double NEXT_BDF_T = 0;

static int jac_struct_initialized = FALSE;

static SlsMat init_jac_matrix = NULL;

int HYB_cmpfunc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void HYB_order(SlsMat JacMat)
{
  int size = clcData->nBDF, row, cols;
  int *col_ptrs = *JacMat->colptrs;
  int *row_vals = *JacMat->rowvals;
  for (row = 0; row < size; row++) {
    cols = col_ptrs[row + 1] - col_ptrs[row];
    qsort(&(row_vals[col_ptrs[row]]), cols, sizeof(int), HYB_cmpfunc);
  }
}

void HYB_assignJacStruct(SlsMat JacMat)
{
  int size = clcData->states, row, col, eq, eqs;
  int *col_ptrs = *JacMat->colptrs;
  int *row_vals = *JacMat->rowvals;
  int nBDF = clcData->nBDF;
  int *BDF = clcData->BDF;
  int *BDFMap = clcData->BDFMap;

  eqs = clcData->jac_matrices->state_eqs;

  // Check that the initial pointer values are cleared.
  SD_cleanTransJacMatrices(clcData->jac_matrices);

  // Assign Jacobian transpose size values to BDF partition.
  col_ptrs[0] = 0;
  for (row = 0; row < nBDF; row++) {
    int glob_row = BDFMap[row];
    int nsd = clcData->nSD[glob_row];
    for (col = 0; col < nsd; col++) {
      int sd = clcData->SD[glob_row][col];
      if (BDF[sd] != NOT_ASSIGNED) {
        col_ptrs[row+1]++;
      }
    }
  }
  for (row = 1; row <= nBDF; row++) {
    col_ptrs[row] += col_ptrs[row-1];
  }

  SD_jacMatrix jac_t = clcData->jac_matrices->df_dx_t;
  for (eq = 0; eq < eqs; eq++) {
    SD_jacMatrix jac = clcData->jac_matrices->df_dx[eq];
    for (row = 0; row < size; row++) {
      for (col = 0; col < jac->size[row]; col++) {
        /// Get the variable column from the original Jac matrix.
        int row_t = jac->index[row][col];
        // If the row is computed by CV_ODE, then look at the columns.
        int bdf_row = BDF[row_t];
        if (bdf_row != NOT_ASSIGNED) {
          int col_t = clcData->SD[row_t][jac_t->index[row_t][0]];
          if (BDF[col_t] != NOT_ASSIGNED) {
            col_t = col_ptrs[bdf_row] + jac_t->index[row_t][0];
            row_vals[col_t] = clcData->SD[row_t][jac_t->index[row_t][0]];
            printf("%d, %d -> %d\n", row_t, col_t, row_vals[col_t]);
          }
          jac_t->index[row_t][0]++;
        }
      }
    }
  }
  
  HYB_order(JacMat);

  init_jac_matrix = SparseNewMat(JacMat->M, JacMat->N, JacMat->NNZ, CSC_MAT);
  SparseCopyMat(JacMat, init_jac_matrix);
}

static int Jac(realtype t, N_Vector y, N_Vector fy, SlsMat JacMat, void *user_data, N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
  int jac_col_nbr = 0, i, j;
  int nBDF = clcData->nBDF;
  int *BDF = clcData->BDF;
  int *nSD = clcData->nSD;
  int **SD = clcData->SD;
  int *JacIt = clcData->JacIt;
  int *BDFMap = clcData->BDFMap;
  const int coeffs = clcData->order + 1;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    bdfQ[cf0] = Ith(y, i);
  }
  
  clcModel->jac(bdfQ, clcData->d, clcData->alg, t, clcData->jac_matrices, clcData->jac);
  
  if (jac_struct_initialized == FALSE) {
    HYB_assignJacStruct(JacMat);
    jac_struct_initialized = TRUE;
  } else {
    SparseCopyMat(init_jac_matrix, JacMat);
  }

  /// Assign the BDF computed part of the Jacobian to the CV_ODE jac vector.
  int cv_ode_col = 0;
  for (i = 0, jac_col_nbr = 0; i < nBDF; i++) {
    int row = BDFMap[i];
    int nsd = nSD[row];
    for (j = 0; j < nsd; j++, jac_col_nbr++) {
      int sd = SD[row][j];
      int col = BDF[sd];
      if (col != NOT_ASSIGNED) {
        JacMat->data[cv_ode_col++] = clcData->jac[JacIt[jac_col_nbr]];
      } 
    }
  }
  //SparsePrintMat(JacMat,stdout);
  return 0;
}

static int check_flag(void *flagvalue, const char *funcname, int opt, QSS_simulator simulator)
{
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    SD_print(simulator->simulationLog, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
    return 1;
  }

  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *)flagvalue;
    if (*errflag < 0) {
      SD_print(simulator->simulationLog, "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n", funcname, *errflag);
      return 1;
    }
  }

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    SD_print(simulator->simulationLog, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
    return 1;
  }

  return 0;
}

int QSS_HYB_BDF_model(realtype t, N_Vector y, N_Vector ydot, void *user_data)
{
  int nBDF = clcData->nBDF, i;
  int *BDFMap = clcData->BDFMap;
  double H = clcTime->time - BDFIntegrationTime;
  int nBDFInputs = clcData->nBDFInputs;
  int *BDFInputs = clcData->BDFInputs;
  double e;
  double *d = clcData->d;
  double *a = clcData->alg;
  double *x = clcData->x;
  double *q = clcData->q;

  const int coeffs = clcData->order + 1;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    bdfQ[cf0] = Ith(y, i);
  }

  for (i = 0; i < nBDFInputs; i++) {
    int qssVar = BDFInputs[i];
    int cf0 = qssVar * coeffs;
    e = t - BDFIntegrationTime;
    if (clcTime->time == 0) {
      bdfQ[cf0] = q[cf0];
    } else {
      bdfQ[cf0] = (q[cf0] - qBDF[cf0]) * e / H + qBDF[cf0];
    }
  }
  clcModel->F(bdfQ, d, a, t, x, BDFMap, nBDF);
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    Ith(ydot, i) = x[cf0 + 1];
  }
  return 0;
}

void QSS_HYB_initialize(SIM_simulator simulate)
{
  QSS_simulator simulator = (QSS_simulator)simulate->state->sim;
  // Local mappings.
  QSS_data qssData = simulator->data;
  double *x = qssData->x;
  double t = simulator->time->time;
  qBDF = (double *)malloc(qssData->states * (qssData->order + 1) * sizeof(double));
  qQSS = (double *)malloc(qssData->states * (qssData->order + 1) * sizeof(double));
  qssTQ = (double *)malloc(qssData->states * sizeof(double));
  bdfQ = (double *)malloc(qssData->states * (qssData->order + 1) * sizeof(double));
  Q = (double *)malloc(qssData->states * sizeof(double));
  
  QSS_model qssModel = simulator->model;
  if (qssModel->jac != NULL) {
    qssModel->jac(x, qssData->d, qssData->alg, t, qssData->jac_matrices, qssData->jac);
  }
  QSS_BDF_partition(qssData, simulator->output);
  QSS_SEQ_initialize(simulate);
}

double QSS_HYB_BDFMaxInputError(int index)
{
  int i, j;
  double eMax = INF;
  const int coeffs = clcData->order + 1;
  int nBDFInputs = clcData->nBDFInputs;
  int *BDFInputs = clcData->BDFInputs;
  for (i = 0; i < nBDFInputs; i++) {
    int qssVar = BDFInputs[i];
    for (j = 0; j < clcData->nSD[qssVar]; j++) {
      int inf = clcData->SD[qssVar][j];
      if (clcData->BDF[inf] != NOT_ASSIGNED) {
        int jid = clcData->QSSOutputJac[clcData->QSSOutputJacId[i] + j];
        double e = (clcData->dQRel[qssVar] * fabs(clcData->q[qssVar * coeffs]) + clcData->dQMin[qssVar]) / fabs(clcData->jac[jid]);
        if (e < eMax) {
          eMax = e;
        }
      }
    }
  }
  return eMax;
}

bool QSS_HYB_updateBDFStep()
{
  bool ret = FALSE;
  double delta1, delta2, t, tk1, tjtk, eMax, H, h;
  double *q = clcData->q;
  int index = clcTime->minIndex;
  const int cf0 = (clcData->order + 1) * index;
  int bdfVar = clcData->BDFMap[0];
  eMax = QSS_HYB_BDFMaxInputError(index) * 2;
  t = clcTime->time;
  tk1 = NEXT_BDF_T;
  h = tk1 - BDFIntegrationTime;
  tjtk = t - BDFIntegrationTime;
  double tj1tj = t - qssTQ[index];
  Q[index] += qQSS[cf0] * tj1tj + qQSS[cf0 + 1] * tj1tj * tj1tj / 2;
  delta1 = (qBDF[cf0] - q[cf0] + q[cf0 + 1] * tjtk) / 2;
  delta2 = ((qBDF[cf0] + q[cf0]) / 2) * tjtk - Q[index];
  double h1;
  if (delta1 >= 0) {
    h1 = eMax - delta2;
  } else {
    h1 = -eMax - delta2;
  }
  H = tjtk + fabs(h1 / delta1);
  if (H < h) {
    clcTime->nextStateTime[bdfVar] = BDFIntegrationTime + H;
    ret = TRUE;
  } else {
    clcTime->nextStateTime[bdfVar] = NEXT_BDF_T;
  }
  return ret;
}

void QSS_HYB_integrate(SIM_simulator simulate)
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
  clcData = qssData;
  clcModel = qssModel;
  clcTime = qssTime;
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
  int nBDFOutputVars = qssData->nBDFOutputVars;
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
  int nBDFInputs = qssData->nBDFInputs;
  int *BDFInputs = qssData->BDFInputs;
  int *BDFOutputVars = qssData->BDFOutputVars;
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
  double BDFMaxStep = qssData->BDFMaxStep;
#ifdef SYNC_RT
  setInitRealTime();
#endif
  realtype reltol = qssData->dQRel[0], ct, tout, BDFH;
  N_Vector y, abstol, ycurrent, ydot;
  void *cvode_mem;
  cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
  y = N_VNew_Serial(nBDF);
  if (check_flag((void *)y, "N_VNew_Serial", 0, simulator)) return;

  ycurrent = N_VNew_Serial(nBDF);
  if (check_flag((void *)ycurrent, "N_VNew_Serial", 0, simulator)) return;

  ydot = N_VNew_Serial(nBDF);
  if (check_flag((void *)ydot, "N_VNew_Serial", 0, simulator)) return;

  abstol = N_VNew_Serial(nBDF);
  if (check_flag((void *)abstol, "N_VNew_Serial", 0, simulator)) return;

  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    int cf0 = bdfVar * coeffs;
    Ith(y, i) = x[cf0];
    Ith(abstol, i) = qssData->dQMin[0];
  }

  flag = CVodeInit(cvode_mem, QSS_HYB_BDF_model, t, y);
  if (check_flag((void *)cvode_mem, "CVodeCreate", 0, simulator)) return;

  flag = CVodeSVtolerances(cvode_mem, reltol, abstol);
  if (check_flag(&flag, "CVodeInit", 1, simulator)) return;

  if (simulator->data->params->jacobian == 0) {
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
    // Set the Jacobian routine to Jac (user-supplied)
    flag = CVSlsSetSparseJacFn(cvode_mem, Jac);
    if (check_flag(&flag, "CVSlsSetSparseJacFn", 1, simulator)) return;
  } else {
    flag = CVDense(cvode_mem, nBDF);
    if (check_flag(&flag, "CVDense", 1, simulator)) return;
  }
#ifdef DEBUG
  if (settings->debug & SD_DBG_StepInfo) {
    SD_print(simulator->simulationLog, "Begin Simulation:");
  }
#endif

  CVodeSetMaxOrd(cvode_mem, 2);
  tout = ft;
  ct = t;
  bool BDFReinit = FALSE;
  bool BDFRestore = FALSE;

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
        bool RESTORE_MAX_STEP = FALSE;
        if (BDFReinit) {
          BDFReinit = FALSE;
          if (BDFRestore) {
            flag = CVodeGetDky(cvode_mem, t, 0, ycurrent);
            CVodeSetMaxStep(cvode_mem, BDFMaxStep);
            CVodeReInit(cvode_mem, t, ycurrent);
            BDFRestore = FALSE;
          } else {
            BDFH = t - BDFIntegrationTime;
            double max_step = BDFH;
            if (max_step > BDFMaxStep) {
              max_step = BDFMaxStep;
            }
            CVodeSetMaxStep(cvode_mem, max_step);
            RESTORE_MAX_STEP = TRUE;
          }
        }
        for (i = 0; i < qssData->states; i++) {
          Q[i] = 0;
        }
        for (i = 0; i < nBDFInputs; i++) {
          int qssVar = BDFInputs[i];
          elapsed = t - tq[qssVar];
          infCf0 = qssVar * coeffs;
          if (elapsed > 0) {
            integrateState(infCf0, elapsed, q, qOrder);
          }
          tq[qssVar] = t;
        }

        flag = CVode(cvode_mem, tout, y, &ct, CV_ONE_STEP);
        while (ct < t) {
          BDFH = t - ct;
          double max_step = BDFH;
          if (max_step > BDFMaxStep) {
            max_step = BDFMaxStep;
          }
          CVodeSetMaxStep(cvode_mem, max_step);
          RESTORE_MAX_STEP = TRUE;
          flag = CVode(cvode_mem, tout, y, &ct, CV_ONE_STEP);
        }
        CVodeGetCurrentStep(cvode_mem, &BDFH);
        flag = CVodeGetDky(cvode_mem, t, 0, ycurrent);
        flag = CVodeGetDky(cvode_mem, t, 1, ydot);
        if (ct != t && t != 0) {
          CVodeReInit(cvode_mem, t, ycurrent);
          CVodeSetInitStep(cvode_mem, BDFH);
          ct = t;
        }
        if (RESTORE_MAX_STEP) {
          CVodeSetMaxStep(cvode_mem, BDFMaxStep);
        }
        for (i = 0; i < nBDFInputs; i++) {
          int qssVar = BDFInputs[i];
          infCf0 = qssVar * coeffs;
          qBDF[infCf0] = q[infCf0];
          qBDF[infCf0 + 1] = q[infCf0 + 1];
        }
        if (t == 0) {
          BDFIntegrationTime = ct;
        } else {
          BDFIntegrationTime = t;
        }
        for (i = 0; i < nBDF; i++) {
          int bdfVar = BDFMap[i];
          nextStateTime[bdfVar] = ct + BDFH;
          NEXT_BDF_T = ct + BDFH;
          cf0 = bdfVar * coeffs;
          q[cf0] = Ith(ycurrent, i);
          q[cf0 + 1] = Ith(ydot, i);
          tq[bdfVar] = t;
          tx[bdfVar] = t;
        }
        for (i = 0; i < nBDFOutputs; i++) {
          int bdfVar = BDFOutputs[i], inf;
          nSD = qssData->nSD[bdfVar];
          cf0 = bdfVar * coeffs;
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
          QA_recomputeNextTimes(quantizer, nSD, SD[bdfVar], t, nextStateTime, x, lqu, q);
          qssTime->minIndex = bdfVar;
          SC_update(scheduler, qssData, qssTime);
          qssTime->minIndex = index;
          qssTime->minValue = t;
          qssTime->time = t;
          qssTime->type = ST_State;
        }
        if (nOutputs && nBDFOutputVars) {
          for (i = 0; i < nBDFOutputVars; i++) {
            int bdfVar = BDFOutputVars[i];
            qssTime->minIndex = bdfVar;
            OUT_write(log, qssData, qssTime, output);
          }
          qssTime->minIndex = index;
        }
      } else {
        cf0 = index * coeffs;
        elapsed = t - tx[index];
        if (Q[index] == 0) {
          qssTQ[index] = BDFIntegrationTime;
        } else {
          qssTQ[index] = tq[index];
        }
        qQSS[cf0] = q[cf0];
        qQSS[cf0 + 1] = q[cf0 + 1];
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
        bool updateBDF = TRUE;
        for (i = 0; i < nSD; i++) {
          j = SD[index][i];
          elapsed = t - tx[j];
          infCf0 = j * coeffs;
          if ((BDF[j] != NOT_ASSIGNED) && updateBDF) {
            int bdfInput = qssData->QSSOutputs[index];
            if (qssData->BDFInputsFirstStep[bdfInput]) {
              qssData->BDFInputsFirstStep[bdfInput] = FALSE;
              nextStateTime[j] = t;
              BDFReinit = TRUE;
            } else if (BDFIntegrationTime > t) {
              nextStateTime[j] = t;
              BDFReinit = TRUE;
              BDFRestore = TRUE;
            } else if (QSS_HYB_updateBDFStep()) {
              BDFReinit = TRUE;
            }
            updateBDF = FALSE;
          }
          if (elapsed > 0) {
            x[infCf0] = evaluatePoly(infCf0, elapsed, x, xOrder);
            tx[j] = t;
          }
        }
        FRW_recomputeDerivatives(frw, qssModel, qssData, qssTime, index);
        for (i = 0; i < nSD; i++) {
          j = SD[index][i];
          infCf0 = j * coeffs;
          if (BDF[j] == NOT_ASSIGNED) {
            qssTQ[j] = t;
            qQSS[infCf0] = q[infCf0];
            qQSS[infCf0 + 1] = q[infCf0 + 1];
          }
        }
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
