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
  int states = simData->states, i, j;
  p->band = FALSE;
  p->bigger = 0;
  p->smaller = 0;
  p->hmin = simData->ft * 1e-4;
  p->h = p->hmin;
  p->nBE = 0;
  p->nSD = simData->nSD;
  p->SD = simData->SD;
  p->xprev = (double *)malloc(states * sizeof(double));
  p->change = (bool *)malloc(states * sizeof(bool));
  p->BE = (bool *)malloc(states * sizeof(bool));
  p->invAd = NULL;
  int infs = 0;
  for (i = 0; i < states; i++) {
    infs += simData->nSD[i]; // cantidad de influencias
  }
  p->S = (int **)malloc(states * sizeof(int *)); // matriz de sparcity
  for (i = 0; i < infs; i++) {
    p->S[i] = (int *)malloc(states * sizeof(int));
  }
  p->infs = infs;
  for (i = 0; i < states; i++) {
    int cf0 = i * 3;
    p->xprev[i] = simData->x[cf0];
    p->change[i] = FALSE;
    p->BE[i] = FALSE;
    for (j = 0; j < states; j++) {
      p->S[i][j] = -1;
    }
  }
  return p;
}

void QSS_BDF_freeHybrid(QSS_BDF_hybrid h) {
  if (h != NULL) {
    free(h);
  }
}

void QSS_BDF_partition(QSS_BDF_hybrid hybrid, QSS_data simData) {
  int i;
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
    ssize_t read;
    int val;
    i = 0;
    read = getline(&line, &len, file);
    if(read != -1)
    {
      unsigned long vars;
      sscanf(line, "%lu", &vars);
      if(vars > 0) hybrid->nBE = vars;
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
        hybrid->BE[val] = TRUE;
        hybrid->BES[i++] = val;
      }
    }
    fclose(file);
    if (line != NULL) {
      free(line);
    }
    if (wrongFile == TRUE) {
      abort();
    }
  }
  else {
    int j, states = simData->states;
    hybrid->nBE = states;
    for (i = 0; i < states; i++) {
      hybrid->BE[i] = TRUE;
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
              hybrid->BE[simData->SD[handlerInfDerivative][m]] = FALSE;
              hybrid->nBE--;
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
    if (hybrid->nBE) {
      hybrid->BES = (int *)malloc(hybrid->nBE * sizeof(int));
#ifdef DEBUG
      printf("Variables computed with BDF: ");
#endif
      j = 0;
      for (i = 0; i < states; i++) {
        if (hybrid->BE[i]) {
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
  if (hybrid->nBE) {
    hybrid->invAd = (double **)malloc(hybrid->nBE * sizeof(double*));
    for (i = 0; i < hybrid->nBE; i++) {
      hybrid->invAd[i] = (double *)malloc(hybrid->nBE * sizeof(double));
    }
  }
}


