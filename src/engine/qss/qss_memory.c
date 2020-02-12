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

#include <qss/qss_memory.h>

#include <stdio.h>
#include <stdlib.h>

#include "../common/data.h"
#include "../common/utils.h"
#include "qss_data.h"
#include "qss_log.h"

#ifdef QSS_PARALLEL
void M_PAR_init(LG_log log, QSS_data sim_data, SD_output sim_output)
#else
void M_init(LG_log log, QSS_data sim_data, SD_output sim_output)
#endif
{
  int i, size, out_idx;
#ifdef QSS_PARALLEL
  int local_idx = 0;
  log->state->size = sim_data->lp->outputs;
#else
  log->state->size = sim_output->outputs;
#endif
  if (sim_data->params->lps > 1) {
    log->state->max_open_files = MAX_OPEN_FILES / sim_data->params->lps;
  }
  size = sim_output->outputs;
  if (size > 0) {
    log->state->states = (list *)checkedMalloc(log->state->size * sizeof(struct list_));
    for (i = 0; i < size; i++) {
#ifdef QSS_PARALLEL
      if (sim_data->lp->oMap[i] <= NOT_ASSIGNED) {
        continue;
      } else {
        out_idx = local_idx;
        local_idx++;
      }
#else
      out_idx = i;
#endif
      if (sim_output->commInterval == CI_Dense || sim_output->commInterval == CI_Sampled) {
        log->state->states[out_idx] = List(sim_data->params->nodeSize, (sim_data->order + 1) * sim_output->nOS[i] + sim_output->nOD[i]);
      } else {
        log->state->states[out_idx] = List(sim_data->params->nodeSize, 1);
      }
    }
#ifdef QSS_PARALLEL
    log->ops->write = M_PAR_write;
    log->ops->writeLine = M_PAR_writeLine;
    log->ops->toFile = M_PAR_toFile;
#else
    log->ops->write = M_write;
    log->ops->writeLine = M_writeLine;
    log->ops->toFile = M_toFile;
#endif
    log->state->data = sim_data;
    log->state->output = sim_output;
  }
}

#ifdef QSS_PARALLEL
void M_PAR_write(LG_log log, int i, double time, double value)
#else
void M_write(LG_log log, int i, double time, double value)
#endif
{
  append(log->state->states[i], time, &value);
}

#ifdef QSS_PARALLEL
void M_PAR_writeLine(LG_log log, int i, double time, double *value)
#else
void M_writeLine(LG_log log, int i, double time, double *value)
#endif
{
  append(log->state->states[i], time, value);
}

#ifdef QSS_PARALLEL
void M_PAR_writeFile(LG_log mem_log, LG_log file_log)
#else
void M_writeFile(LG_log mem_log, LG_log file_log)
#endif
{
  int i, j;
  int output_init = mem_log->state->batch_init;
  int output_end = mem_log->state->batch_end;
  if (mem_log->state->output->commInterval == CI_Step) {
    for (i = output_init; i < output_end; i++) {
      node n = mem_log->state->states[i]->begin;
      while (n) {
        for (j = 0; j < n->used; j++) {
          LG_write(file_log, i, n->timeVal[j], n->val[j][0]);
        }
        n = n->next;
      }
    }
  }
  if (mem_log->state->output->commInterval == CI_Dense) {
    for (i = output_init; i < output_end; i++) {
      node n = mem_log->state->states[i]->begin;
      while (n) {
        for (j = 0; j < n->used; j++) {
          LG_writeLine(file_log, i, n->timeVal[j], n->val[j]);
        }
        n = n->next;
      }
    }
  }
  if (mem_log->state->output->commInterval == CI_Sampled) {
    int order = mem_log->state->data->order;
    int coeffs = order + 1;
    int out_idx;
#ifdef QSS_PARALLEL
    int local_idx = 0;
#endif
    int outputs = mem_log->state->output->outputs;
    double *tmp1 = (double *)malloc(mem_log->state->data->states * (mem_log->state->data->order + 1) * sizeof(double));
    for (i = 0; i < outputs; i++) {
#ifdef QSS_PARALLEL
      if (mem_log->state->data->lp->oMap[i] <= NOT_ASSIGNED) {
        continue;
      } else {
        out_idx = local_idx;
        local_idx++;
      }
#else
      out_idx = i;
#endif
      int index = 1;
      double dt = mem_log->state->output->sampled->period[i];
      if (dt == 0) {
        dt = 1e-2;
      }
      node n = mem_log->state->states[out_idx]->begin;
      while (n) {
        for (j = 0; j < n->used; j++) {
          double nextStep;
          if (j + 1 == n->used) {
            if (n->next == NULL) {
              nextStep = mem_log->state->data->ft + dt / 10;
            } else {
              nextStep = n->next->timeVal[0];
            }
          } else {
            nextStep = n->timeVal[j + 1];
          }
          if (n->timeVal[j] == nextStep) {
            continue;
          }
          double nt = mem_log->state->output->sampled->nextTime[i];
          double e = nt - n->timeVal[j];
          while (nt <= nextStep) {
            int g;
            double tmpval = 0;
            double *d = mem_log->state->data->d;
            double *alg = mem_log->state->data->alg;
            if (mem_log->state->output->nOS[i]) {
              int nOS = mem_log->state->output->nOS[i];
              for (g = 0; g < nOS; g++) {
                int stVar = mem_log->state->output->OS[i][g];
                tmp1[stVar * coeffs] = evaluateVectorPoly(e, &(n->val[j][g * coeffs]), order);
              }
            }
            if (mem_log->state->output->nOD[i]) {
              int nOD = mem_log->state->output->nOD[i];
              int nStates = mem_log->state->output->nOS[i] * coeffs;
              for (g = 0; g < nOD; g++) {
                int dscVar = mem_log->state->output->OD[i][g];
                d[dscVar] = n->val[j][nStates + g];
              }
            }
            mem_log->state->output->value(i, tmp1, d, alg, nt, &tmpval);
            LG_write(file_log, out_idx, nt, tmpval);
            e += dt;
            mem_log->state->output->sampled->nextTime[i] = dt * index++;
            nt = mem_log->state->output->sampled->nextTime[i];
          }
        }
        n = n->next;
      }
    }
    free(tmp1);
  }
}

#ifdef QSS_PARALLEL
void M_PAR_toFile(LG_log log)
#else
void M_toFile(LG_log log)
#endif
{
  int i;
  int outputs = log->state->size;
  int max_open_files = log->state->max_open_files;
  int output_iter = (outputs / max_open_files) + 1;
  int remaining = outputs - log->state->batch_end;
  log->state->batch_end = (remaining > max_open_files) ? max_open_files : remaining; 
  for (i = 0; i < output_iter; i++) {
    LG_log file_log = LG_copy(log, SD_File); 
#ifdef QSS_PARALLEL
    M_PAR_writeFile(log, file_log);
#else
    M_writeFile(log, file_log);
#endif
    LG_freeLog(file_log);
    log->state->batch_init += max_open_files;
    remaining = outputs - log->state->batch_end;
    log->state->batch_end += (remaining > max_open_files) ? max_open_files : remaining; 
  }
}
