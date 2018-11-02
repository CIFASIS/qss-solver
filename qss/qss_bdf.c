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
    p->BE[i] = TRUE;
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
  int i, j, m, states = simData->states;
  // Determinar variables que NO van con BE
  if (simData->HD) {
    printf("LIQSS_BDF: ");
    for (i = 0; i < simData->events; i++) {
      for (j = 0; j < simData->nHD[i]; j++) {
        for (m = 0; m < simData->nSD[simData->HD[i][j]]; m++) {
          if (hybrid->BE[simData->SD[simData->HD[i][j]][m]]) {
            hybrid->BE[simData->SD[simData->HD[i][j]][m]] = FALSE;
            hybrid->nBE--;
            printf("%d ", simData->SD[simData->HD[i][j]][m]);
          }
        }
      }
    }
  }
  printf("\n");
  // Guardar vector de estados que van con BE
  if (hybrid->nBE) {
    hybrid->BES = (int *)malloc(hybrid->nBE * sizeof(int));
    printf("LIDF2: ");
    j = 0;
    for (i = 0; i < states; i++) {
      if (hybrid->BE[i]) {
        hybrid->BES[j] = i;
        printf("%d ", i);
        j++;
      }
    }
    printf("\n");
  }

  // Forzar todo a LI
  if (0) {
    for (i = 0; i < states; i++)
      hybrid->BE[i] = FALSE;
    hybrid->nBE = 0;
  }
}
