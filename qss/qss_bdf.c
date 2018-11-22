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
  p->nBE = 0;
  p->nSD = simData->nSD;
  p->SD = simData->SD;
  p->xprev = (double *)malloc(states * sizeof(double));
  p->change = (bool *)malloc(states * sizeof(bool));
  p->BE = (int *)malloc(states * sizeof(int));
  p->BES = NULL;
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
    p->change[i] = FALSE;
    p->BE[i] = NOT_ASSIGNED;
  }
  return p;
}

void QSS_BDF_freeHybrid(QSS_BDF_hybrid h) {
  if (h->BE != NULL) {
    free(h->BE);
  }
  if (h->Jac != NULL) {
    free(h->Jac);
  }
  if (h->BES != NULL) {
    free(h->BES);
  }
  if (h->change != NULL) {
    free(h->change);
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

void QSS_BDF_initGSL(QSS_BDF_hybrid hybrid) {
  // gsl
  hybrid->T = gsl_splinalg_itersolve_gmres;
  hybrid->work = gsl_splinalg_itersolve_alloc(hybrid->T, hybrid->nBE, 0);
  hybrid->As =
      gsl_spmatrix_alloc(hybrid->nBE, hybrid->nBE);  // sparse left side matrix
  hybrid->fs = gsl_vector_alloc(hybrid->nBE);        // right side vector
  hybrid->us = gsl_vector_alloc(hybrid->nBE);        // solution vector
}

void QSS_BDF_initJacobianVector(QSS_BDF_hybrid hybrid, QSS_data simData) {
  int jac = 0, fullJac = 0, i, j;
  int states = simData->states;
  int *BE = hybrid->BE;
  for (i = 0; i < states; i++) {
    if (BE[i] != NOT_ASSIGNED) {
      jac += hybrid->nSD[i];
      continue;
    }
  }
  hybrid->Jac = (int *)malloc(jac * sizeof(int));
  jac = 0;
  for (i = 0; i < states; i++) {
    if (BE[i] == NOT_ASSIGNED) {
      fullJac += hybrid->nSD[i];
      continue;
    }
    for (j = 0; j < hybrid->nSD[i]; j++) {
      hybrid->Jac[jac++] = fullJac++;
    }
  }
}

void QSS_BDF_partition(QSS_BDF_hybrid hybrid, QSS_data simData) {
  int i, j;
  FILE *file;
  char fileName[256];
  sprintf(fileName, "%s", simData->bdfPartFile);
  file = fopen(fileName, "r");
#ifdef DEBUG
  printf("BDF Partition file: %s\n", fileName);
#endif
  bool wrongFile = FALSE;
  if (file != NULL) {
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int val;
    i = 0;
    read = getline(&line, &len, file);
    if (read != -1) {
      int vars;
      sscanf(line, "%d", &vars);
      if (vars > 0) hybrid->nBE = vars;
    }
    if (hybrid->nBE > 0) {
      hybrid->BES = (int *)malloc(hybrid->nBE * sizeof(int));
      while ((read = getline(&line, &len, file)) != -1) {
        sscanf(line, "%d", &val);
        if (val < 0) {
          fprintf(stderr, "Wrong partition file.\n");
          wrongFile = TRUE;
          break;
        }
        hybrid->BE[val] = i;
        hybrid->BES[i++] = val;
      }
      QSS_BDF_initGSL(hybrid);
      QSS_BDF_initJacobianVector(hybrid, simData);
    }
    fclose(file);
    if (line != NULL) {
      free(line);
    }
    if (wrongFile == TRUE) {
      abort();
    }
#ifdef DEBUG
    printf("Variables computed with BDF: ");
    printf("%d ", hybrid->nBE);
    for (i = 0; i < hybrid->nBE; i++) {
      printf("%d ", hybrid->BES[i]);
    }
#endif
  } else {
    int states = simData->states;
    hybrid->nBE = states;
    for (i = 0; i < states; i++) {
      hybrid->BE[i] = i;
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
            if (hybrid->BE[simData->SD[handlerInfDerivative][m]]) {
              hybrid->BE[simData->SD[handlerInfDerivative][m]] = NOT_ASSIGNED;
              hybrid->nBE--;
#ifdef DEBUG
              printf("%d ", simData->SD[handlerInfDerivative][m]);
#endif
            }
          }
        }
      }
    }
    QSS_BDF_initGSL(hybrid);
    QSS_BDF_initJacobianVector(hybrid, simData);
#ifdef DEBUG
    printf("\n");
#endif
    if (hybrid->nBE) {
      hybrid->BES = (int *)malloc(hybrid->nBE * sizeof(int));
#ifdef DEBUG
      printf("Variables computed with BDF: ");
#endif
      j = 0;
      for (i = 0; i < states; i++) {
        if (hybrid->BE[i] != NOT_ASSIGNED) {
          hybrid->BES[j] = i;
#ifdef DEBUG
          printf("%d ", i);
#endif
          j++;
        }
      }
#ifdef DEBUG
      printf("\n");
#endif
    }
  }
}
