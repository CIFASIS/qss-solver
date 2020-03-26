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

#include "qss_log.h"

#include <stdlib.h>

#include "qss_file.h"
#include "qss_memory.h"

LG_logState LG_LogState()
{
  int i;
  LG_logState s = checkedMalloc(sizeof(*s));
  s->states = NULL;
  s->data = NULL;
  s->output = NULL;
  s->files = NULL;
  s->size = 0;
  s->batch_init = 0;
  s->batch_end = 0;
  s->max_open_files = MAX_OPEN_FILES;
  for (i = 0; i < NAME_SIZE; i++) {
    s->fileName[i] = '\0';
  }
  s->values = NULL;
  return s;
}

LG_logOps LG_LogOps()
{
  LG_logOps s = checkedMalloc(sizeof(*s));
  s->write = NULL;
  s->writeLine = NULL;
  s->toFile = NULL;
  return s;
}

LG_log LG_Log(QSS_data sim_data, SD_output sim_output)
{
  LG_log s = checkedMalloc(sizeof(*s));
  s->state = LG_LogState();
  s->ops = LG_LogOps();
  switch (sim_output->store) {
  case SD_Memory:
    if (sim_data->params->lps > 0) {
      M_PAR_init(s, sim_data, sim_output);
    } else {
      M_init(s, sim_data, sim_output);
    }
    break;
  case SD_File:
    if (sim_data->params->lps > 0) {
      F_PAR_init(s, sim_data, sim_output);
    } else {
      F_init(s, sim_data, sim_output);
    }
    break;
  }
  return s;
}

LG_log LG_copy(LG_log log, SD_StoreData store_data)
{
  LG_log s = checkedMalloc(sizeof(*s));
  s->state = LG_LogState();
  s->ops = LG_LogOps();
  QSS_data sim_data = log->state->data;
  SD_output sim_output = log->state->output;
  s->state->batch_init = log->state->batch_init;
  s->state->batch_end = log->state->batch_end;
  switch (store_data) {
  case SD_Memory:
    if (sim_data->params->lps > 0) {
      M_PAR_init(s, sim_data, sim_output);
    } else {
      M_init(s, sim_data, sim_output);
    }
    break;
  case SD_File:
    if (sim_data->params->lps > 0) {
      F_PAR_init(s, sim_data, sim_output);
    } else {
      F_init(s, sim_data, sim_output);
    }
    break;
  }
  return s;
}

void LG_freeLogOps(LG_logOps ops)
{
  if (ops != NULL) {
    free(ops);
  }
}

void LG_freeLogState(LG_logState state)
{
  if (state == NULL) {
    return;
  }
  int i, j, outputs = state->size;
  if (state->files != NULL) {
    int size = state->batch_end - state->batch_init;
    for (i = 0; i < size; i++) {
      fclose(state->files[i]);
    }
    free(state->files);
  }
  if (state->values != NULL) {
    free(state->values);
  }
  if (state->states != NULL) {
    for (i = 0; i < outputs; i++) {
      node n = state->states[i]->begin;
      while (n) {
        int size = state->states[i]->nodeSize;
        for (j = 0; j < size; j++) {
          free(n->val[j]);
        }
        free(n->val);
        free(n->timeVal);
        node tmp = n->next;
        free(n);
        n = tmp;
      }
      free(state->states[i]);
    }
    free(state->states);
  }
  free(state);
}

void LG_freeLog(LG_log v)
{
  if (v == NULL) {
    return;
  }
  LG_freeLogState(v->state);
  LG_freeLogOps(v->ops);
  free(v);
}

void LG_write(LG_log log, int i, double time, double value) { log->ops->write(log, i, time, value); }

void LG_writeLine(LG_log log, int i, double time, double *value) { log->ops->writeLine(log, i, time, value); }

void LG_toFile(LG_log log) { log->ops->toFile(log); }
