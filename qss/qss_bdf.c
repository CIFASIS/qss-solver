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

#include "../common/utils.h"

void QSS_BDF_initGSL(QSS_BDF_hybrid hybrid) {
  // gsl
  hybrid->T = gsl_splinalg_itersolve_gmres;
  hybrid->work = gsl_splinalg_itersolve_alloc(hybrid->T, hybrid->nBDF, 0);
  hybrid->As =
      gsl_spmatrix_alloc(hybrid->nBDF, hybrid->nBDF);  // sparse left side matrix
  hybrid->fs = gsl_vector_alloc(hybrid->nBDF);        // right side vector
  hybrid->us = gsl_vector_alloc(hybrid->nBDF);        // solution vector
}

void QSS_BDF_initJacobianVector(QSS_BDF_hybrid hybrid, QSS_data simData) {
  int jac = 0, fullJac = 0, i, j;
  int states = simData->states;
  int *BDF = hybrid->BDF;
  for (i = 0; i < states; i++) {
    if (BDF[i] != NOT_ASSIGNED) {
      jac += hybrid->nSD[i];
      continue;
    }
  }
  hybrid->Jac = (int *)malloc(jac * sizeof(int));
  jac = 0;
  for (i = 0; i < states; i++) {
    if (BDF[i] == NOT_ASSIGNED) {
      fullJac += hybrid->nSD[i];
      continue;
    }
    for (j = 0; j < hybrid->nSD[i]; j++) {
      hybrid->Jac[jac++] = fullJac++;
    }
  }
}

QSS_BDF_hybrid QSS_BDF_Hybrid(QSS_data simData) {
  QSS_BDF_hybrid p = checkedMalloc(sizeof(*p));
  int states = simData->states, i;
  p->band = FALSE;
  p->inc_step = 0;
  p->dec_step = 0;
  p->hmin = simData->ft / 1.5e4;
  p->hmax = simData->ft / 100.0;
  p->h = p->hmin;
  p->hprev = 0;
  p->nBDF = simData->nBDF;
  p->nSD = simData->nSD;
  p->SD = simData->SD;
  p->xprev = (double *)malloc(states * sizeof(double));
  p->BDF = simData->BDF;
  p->BDFMap = simData->BDFMap;
  p->fs = NULL;
  p->us = NULL;
  p->work = NULL;
  p->jac = simData->jac;
  p->As = NULL;
  p->Cs = NULL;
  p->work = NULL;
  p->T = NULL;
  p->Jac = NULL;
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    p->xprev[i] = simData->x[cf0];
  }
  QSS_BDF_initGSL(p);
  QSS_BDF_initJacobianVector(p, simData);
  return p;
}

void QSS_BDF_freeHybrid(QSS_BDF_hybrid h) {
  if (h->Jac != NULL) {
    free(h->Jac);
  }
  if (h->xprev != NULL) {
    free(h->xprev);
  }
  if (h->As != NULL) {
    gsl_spmatrix_free(h->As);
  }
  if (h->Cs != NULL) {
    gsl_spmatrix_free(h->Cs);
  }
  if (h->fs != NULL) {
    gsl_vector_free(h->fs);
  }
  if (h->us != NULL) {
    gsl_vector_free(h->us);
  }
  if (h->work != NULL) {
    gsl_splinalg_itersolve_free(h->work);
  }
  free(h);
}

int QSS_BDF_getOutput(int var, QSS_data simData) {
  int nSD = simData->nSD[var];
  int *BDF = simData->BDF;
  int i,j, out = 0;
  for(i = 0; i < nSD; i++) {
    j = simData->SD[var][i];
    if (BDF[j] == NOT_ASSIGNED) { out++; }
  }
  return out;
}

void QSS_BDF_allocBDFOutput(QSS_data simData) {
  simData->BDFOutputs = (int*) malloc(simData->nBDFOutputs * sizeof(int));
  int nBDF = simData->nBDF;
  int *BDF = simData->BDF;
  int i, j, out = 0;
  for(i = 0; i < nBDF; i++) {
    int bdf = simData->BDFMap[i];
    int nSD = simData->nSD[bdf];
    for(j = 0; j < nSD; j++) {
      int v = simData->SD[bdf][j];
      if(BDF[v] == NOT_ASSIGNED) {
        simData->BDFOutputs[out++] = bdf;
      }
    }
  } 
}

void QSS_BDF_partition(QSS_data simData, char* name) {
  int i, j;
  FILE *file;
  char fileName[256];
  sprintf(fileName, "%s_BDF.part", name);
  file = fopen(fileName, "r");
#ifdef DEBUG
  printf("BDF Partition file: %s\n", fileName);
#endif
  bool wrongFile = FALSE;
  simData->BDF = (int *)malloc(simData->states * sizeof(int));
  int states = simData->states;
  if (file != NULL) {
    for (i = 0; i < simData->states; i++) {
      simData->BDF[i] = NOT_ASSIGNED;
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
      }
    }
    QSS_BDF_allocBDFOutput(simData);
#ifdef DEBUG
    printf("Variables computed with BDF: ");
    printf("%d ", simData->nBDF);
    for (i = 0; i < simData->nBDF; i++) {
      printf("%d ", simData->BDFMap[i]);
    }
#endif
  } else {
    simData->nBDF = states;
    for (i = 0; i < states; i++) {
      simData->BDF[i] = i;
    }
    // Compute LIQSS variables
    if (simData->HD) {
#ifdef DEBUG
      printf("Variables computed with LIQSS: ");
#endif
      int m;
      for (i = 0; i < simData->events; i++) {
        for (j = 0; j < simData->nHD[i]; j++) {
          int handlerInfDerivative = simData->HD[i][j];
          int stateInfDerivatives = simData->nSD[handlerInfDerivative];
          for (m = 0; m < stateInfDerivatives; m++) {
            if (simData->BDF[simData->SD[handlerInfDerivative][m]] != NOT_ASSIGNED) {
              simData->BDF[simData->SD[handlerInfDerivative][m]] = NOT_ASSIGNED;
              simData->nBDF--;
#ifdef DEBUG
              printf("%d ", simData->SD[handlerInfDerivative][m]);
#endif
            }
          }
        }
      }
    }
#ifdef DEBUG
    printf("\n");
#endif
    if (simData->nBDF) {
      simData->BDFMap = (int *)malloc(simData->nBDF * sizeof(int));
#ifdef DEBUG
      printf("Variables computed with BDF: ");
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
        }
      }
      QSS_BDF_allocBDFOutput(simData);
#ifdef DEBUG
      printf("\n");
#endif
    }
  }
  if(simData->nBDF) {

  }
}
