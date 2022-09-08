/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHQSS_BDF ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "qss_bdf.h"

#include <stdlib.h>

#include <common/utils.h>

/**
 * @brief      Initialize Jacobian vector used by CV_ODE
 *
 * ** First count the number of SD variables of all the states computed
 * with CV_ODE.
 * ** Allocate JacIt vector with the corresponding number of entries.
 * ** Finally initialize JacIt vector with the original Jacobian values.
 *
 * The vector allocated here is used by the CV_ODE Jacobian function
 * provided by our integrator.
 *
 * @param[in]  simData  The simulation data structures.
 */

void QSS_BDF_initJacobianVector(QSS_data simData)
{
  int jac = 0, fullJac = 0, i, j;
  int states = simData->states;
  int *BDF = simData->BDF;
  for (i = 0; i < states; i++) {
    if (BDF[i] != NOT_ASSIGNED) {
      jac += simData->nSD[i];
    }
  }
  simData->JacIt = (int *)malloc(jac * sizeof(int));
  jac = 0;
  for (i = 0; i < states; i++) {
    if (BDF[i] == NOT_ASSIGNED) {
      fullJac += simData->nSD[i];
      continue;
    }
    for (j = 0; j < simData->nSD[i]; j++) {
      simData->JacIt[jac++] = fullJac++;
    }
  }
}

/**
 * @brief Allocate and initialize Jacobian vectors used in the computation of
 *        the maximum error algorithm.
 *
 * ** Count the number of variables computed by LIQSS2 that influences a
 *    variable computed by CV_ODE. We count ALL the SD variables, even if
 *    some of them are not computed by CV_ODE.
 * ** Allocate QSSOutputJac vector that contains all the affected CV_ODE variables
 *    where we need the Jacobian entry to compute the maximum error allowed. Also,
 *    allocate QSSOutputJacId that contains the vector entry on the original
 *    Jacobian vector.
 *
 * @param[in]  simData  The simulation data
 */

void QSS_BDF_initQSSJacInputs(QSS_data simData)
{
  int i, j, k;
  int states = simData->states, globalSD = 0;
  int *BDF = simData->BDF;
  for (i = 0; i < states; i++) {
    if (BDF[i] != NOT_ASSIGNED) {
      continue;
    }
    int nSD = simData->nSD[i];
    for (j = 0; j < nSD; j++) {
      k = simData->SD[i][j];
      if (BDF[k] != NOT_ASSIGNED) {
        globalSD += nSD;
        break;
      }
    }
  }
  simData->QSSOutputJac = (int *)malloc(globalSD * sizeof(int));
  simData->QSSOutputJacId = (int *)malloc(simData->nBDFInputs * sizeof(int));
  globalSD = 0;
  int localSD = 0;
  int inputs = 0;
  for (i = 0; i < states; i++) {
    int nSD = simData->nSD[i];
    // Each SD influence implies a Jacobian entry.
    // So if the variable is computed by QSS just increment the global index.
    if (BDF[i] != NOT_ASSIGNED) {
      globalSD += nSD;
      continue;
    }
    bool update = FALSE;
    for (j = 0; j < nSD; j++) {
      k = simData->SD[i][j];
      if (BDF[k] != NOT_ASSIGNED) {
        simData->QSSOutputJac[localSD + j] = globalSD + j;
        update = TRUE;
      }
    }
    globalSD += nSD;
    if (update) {
      simData->QSSOutputs[i] = inputs;
      simData->QSSOutputJacId[inputs++] = localSD;
      localSD += nSD;
    }
  }
}

void QSS_BDF_initBDFOutputVars(QSS_data simData, SD_output output)
{
  int nBDF = simData->nBDF, i;
  int *BDFMap = simData->BDFMap;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    if (output->nSO[bdfVar]) {
      simData->nBDFOutputVars++;
    }
  }
  simData->BDFOutputVars = (int *)malloc(simData->nBDFOutputVars * sizeof(int));
  int BDFOut = 0;
  for (i = 0; i < nBDF; i++) {
    int bdfVar = BDFMap[i];
    if (output->nSO[bdfVar]) {
      simData->BDFOutputVars[BDFOut++] = bdfVar;
    }
  }
}

int QSS_BDF_getOutput(int var, QSS_data simData)
{
  int nSD = simData->nSD[var];
  int *BDF = simData->BDF;
  int i, j, out = 0;
  for (i = 0; i < nSD; i++) {
    j = simData->SD[var][i];
    if (BDF[j] == NOT_ASSIGNED) {
      out++;
    }
  }
  return out;
}

int QSS_BDF_getInput(int var, QSS_data simData)
{
  int nSD = simData->nSD[var];
  int *BDF = simData->BDF;
  int i, j, in = 0;
  for (i = 0; i < nSD; i++) {
    j = simData->SD[var][i];
    if (BDF[j] != NOT_ASSIGNED) {
      in++;
    }
  }
  return in;
}

void QSS_BDF_allocBDFInput(QSS_data simData)
{
  simData->BDFInputs = (int *)malloc(simData->nBDFInputs * sizeof(int));
  simData->BDFInputsFirstStep = (bool *)malloc(simData->nBDFInputs * sizeof(bool));
  int nBDF = simData->nBDF;
  int *BDF = simData->BDF;
  int i, j, in = 0;
  for (i = 0; i < nBDF; i++) {
    int bdf = simData->BDFMap[i];
    int nDS = simData->nDS[bdf];
    for (j = 0; j < nDS; j++) {
      int v = simData->DS[bdf][j];
      if (BDF[v] == NOT_ASSIGNED) {
        simData->BDFInputs[in++] = v;
        simData->BDFInputsFirstStep[in++] = TRUE;
      }
    }
  }
}

void QSS_BDF_allocBDFOutput(QSS_data simData)
{
  simData->BDFOutputs = (int *)malloc(simData->nBDFOutputs * sizeof(int));
  int nBDF = simData->nBDF;
  int *BDF = simData->BDF;
  int i, j, out = 0;
  for (i = 0; i < nBDF; i++) {
    int bdf = simData->BDFMap[i];
    int nSD = simData->nSD[bdf];
    for (j = 0; j < nSD; j++) {
      int v = simData->SD[bdf][j];
      if (BDF[v] == NOT_ASSIGNED) {
        simData->BDFOutputs[out++] = bdf;
      }
    }
  }
}

void QSS_BDF_computeLIQSSVariables(QSS_data simData, int ifr, int level)
{
  int i, ifes = simData->nSD[ifr];
  for (i = 0; i < ifes; i++) {
    if (simData->BDF[simData->SD[ifr][i]] != NOT_ASSIGNED) {
      simData->BDF[simData->SD[ifr][i]] = NOT_ASSIGNED;
      simData->nBDF--;
#ifdef DEBUG
      printf("%d ", simData->SD[ifr][i]);
#endif
      if (level < simData->BDFPartitionDepth) {
        QSS_BDF_computeLIQSSVariables(simData, simData->SD[ifr][i], level + 1);
      }
    }
  }
}

void QSS_BDF_partition(QSS_data simData, SD_output output)
{
  int i, j;
  FILE *file;
  char fileName[256];
  sprintf(fileName, "%s_BDF.part", output->name);
  file = fopen(fileName, "r");
#ifdef DEBUG
  printf("BDF Partition file: %s\n", fileName);
#endif
  bool wrongFile = FALSE;
  simData->BDF = (int *)malloc(simData->states * sizeof(int));
  simData->QSSOutputs = (int *)malloc(simData->states * sizeof(int));
  int states = simData->states;
  if (file != NULL && simData->BDFPart) {
    for (i = 0; i < states; i++) {
      simData->BDF[i] = NOT_ASSIGNED;
      simData->QSSOutputs[i] = NOT_ASSIGNED;
    }
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int val;
    i = 0;
    read = getline(&line, &len, file);
    if (read != -1) {
      int vars;
      sscanf(line, "%d", &vars);
      if (vars > 0) simData->nBDF = vars;
    }
    if (simData->nBDF > 0) {
      simData->BDFMap = (int *)malloc(simData->nBDF * sizeof(int));
      while ((read = getline(&line, &len, file)) != -1) {
        sscanf(line, "%d", &val);
        if (val < 0) {
          fprintf(stderr, "Wrong partition file.\n");
          wrongFile = TRUE;
          break;
        }
        simData->BDF[val] = i;
        simData->BDFMap[i++] = val;
      }
    }
    fclose(file);
    if (line != NULL) {
      free(line);
    }
    if (wrongFile == TRUE) {
      abort();
    }
    for (i = 0; i < states; i++) {
      if (simData->BDF[i] != NOT_ASSIGNED) {
        simData->nBDFOutputs += QSS_BDF_getOutput(i, simData);
      } else {
        simData->nBDFInputs += QSS_BDF_getInput(i, simData);
      }
    }
    QSS_BDF_allocBDFOutput(simData);
    QSS_BDF_allocBDFInput(simData);
#ifdef DEBUG
    printf("Variables computed with BDF: ");
    printf("%d ", simData->nBDF);
    for (i = 0; i < simData->nBDF; i++) {
      printf("%d ", simData->BDFMap[i]);
    }
    printf("\n");
    printf("BDF outputs: %d \n", simData->nBDFOutputs);
    printf("BDF inputs: %d \n", simData->nBDFInputs);
#endif
  } else {
    simData->nBDF = states;
    for (i = 0; i < states; i++) {
      simData->BDF[i] = i;
      simData->QSSOutputs[i] = NOT_ASSIGNED;
    }
    // Compute LIQSS variables
    if (simData->HD) {
#ifdef DEBUG
      printf("Variables computed with LIQSS: ");
#endif
      for (i = 0; i < simData->events; i++) {
        for (j = 0; j < simData->nHD[i]; j++) {
          int ife = simData->HD[i][j];
          if (simData->BDF[ife] != NOT_ASSIGNED) {
            simData->BDF[ife] = NOT_ASSIGNED;
            simData->nBDF--;
#ifdef DEBUG
            printf("%d ", ife);
#endif
          }
          if (simData->BDFPartitionDepth > 1) {
            const int INIT_LEVEL = 2;
            QSS_BDF_computeLIQSSVariables(simData, ife, INIT_LEVEL);
          }
        }
      }
      j = 0;
      for (i = 0; i < states; i++) {
        if (simData->BDF[i] != NOT_ASSIGNED) {
          simData->BDF[i] = j++;
        }
      }
    }
#ifdef DEBUG
    printf("\n");
#endif
    if (simData->nBDF) {
      simData->BDFMap = (int *)malloc(simData->nBDF * sizeof(int));
#ifdef DEBUG
      printf("Variables computed with BDF: %d \n", simData->nBDF);
#endif
      j = 0;
      for (i = 0; i < states; i++) {
        if (simData->BDF[i] != NOT_ASSIGNED) {
          simData->BDFMap[j] = i;
          simData->nBDFOutputs += QSS_BDF_getOutput(i, simData);
#ifdef DEBUG
          printf("%d ", i);
#endif
          j++;
        } else {
          simData->nBDFInputs += QSS_BDF_getInput(i, simData);
        }
      }
      QSS_BDF_allocBDFOutput(simData);
      QSS_BDF_allocBDFInput(simData);
#ifdef DEBUG
      printf("\n");
      printf("BDF outputs: %d \n", simData->nBDFOutputs);
      printf("BDF inputs: %d \n", simData->nBDFInputs);
#endif
    }
  }
  QSS_BDF_initJacobianVector(simData);
  QSS_BDF_initQSSJacInputs(simData);
  QSS_BDF_initBDFOutputVars(simData, output);
}
