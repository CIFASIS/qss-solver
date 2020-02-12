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

#include <qss/qss_file.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_log.h"

#ifdef QSS_PARALLEL
void F_PAR_init(LG_log log, QSS_data sim_data, SD_output sim_output)
#else
void F_init(LG_log log, QSS_data sim_data, SD_output sim_output)
#endif
{
  int i, j, size;
  log->state->data = sim_data;
  log->state->output = sim_output;
#ifdef QSS_PARALLEL
  QSS_LP_data lp = sim_data->lp;
  log->state->size = lp->outputs;
#else
  log->state->size = sim_output->outputs;
#endif
  int init = log->state->batch_init;
  int end = log->state->batch_end;
  int total_outputs = log->state->output->outputs; 
  size = end - init;
  if (size > 0) {
    log->state->files = (FILE **)malloc(size * sizeof(FILE *));
    if (sim_output->commInterval == CI_Dense || sim_output->commInterval == CI_Sampled) {
      log->state->values = (int *)checkedMalloc(size * sizeof(int));
    }
    j = 0;
    for (i = init; i < total_outputs; i++) {
      char ext[128] = "";
#ifdef QSS_PARALLEL
      if (lp->oMap[i] > NOT_ASSIGNED) {
        if (sim_output->nOD[i] != 0) {
          sprintf(ext, "-discrete-%d", sim_data->lp->id);
        }
#endif
        sprintf(log->state->fileName, "%s%s.dat", sim_output->variable[i].name, ext);
        log->state->files[j] = fopen(log->state->fileName, "w+");
        if (sim_output->commInterval == CI_Dense || sim_output->commInterval == CI_Sampled) {
          log->state->values[j] = sim_output->nOS[i] * sim_data->order + sim_output->nOD[i];
        }
        j++;
#ifdef QSS_PARALLEL
      }
#endif
      if (j >= size) {
          break;
      } 
    }
#ifdef QSS_PARALLEL
    log->ops->write = F_PAR_write;
    log->ops->writeLine = F_PAR_writeLine;
    log->ops->toFile = F_PAR_toFile;
#else
    log->ops->write = F_write;
    log->ops->writeLine = F_writeLine;
    log->ops->toFile = F_toFile;
#endif
  }
}

#ifdef QSS_PARALLEL
void F_PAR_write(LG_log log, int i, double time, double value)
#else
void F_write(LG_log log, int i, double time, double value)
#endif
{
  int index = i - log->state->batch_init;
  fprintf(log->state->files[index], "%.16lf\t%.16lf\n", time, value);
}

#ifdef QSS_PARALLEL
void F_PAR_writeLine(LG_log log, int i, double time, double *value)
#else
void F_writeLine(LG_log log, int i, double time, double *value)
#endif
{
  int index = i - log->state->batch_init;
  int j, values = log->state->values[index];
  fprintf(log->state->files[index], "%16lf\t", time);
  for (j = 0; j < values - 1; j++) {
    fprintf(log->state->files[index], "%16lf\t", value[j]);
  }
  fprintf(log->state->files[index], "%16lf\n", value[log->state->values[index] - 1]);
}

#ifdef QSS_PARALLEL
void F_PAR_toFile(LG_log log)
#else
void F_toFile(LG_log log)
#endif
{
  return;
}
