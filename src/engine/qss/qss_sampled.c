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

#include <stdlib.h>

#include <common/data.h>
#include <qss/qss_data.h>
#include <qss/qss_log.h>
#include <qss/qss_output.h>
#include <qss/qss_sampled.h>

void QSS_FUNC_DECL(SAM, writeOutvar)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output,
                                     int index, int variable)
{
  int nOS = sim_output->nOS[index];
  int nOD = sim_output->nOD[index];
  int order = output->state->order;
  int coeffs = sim_data->order + 1;
  int nStates = nOS * coeffs;
  const double *output_vars = output->state->output_var;
  for (int j = 0; j < nOS; j++) {
    int k = sim_output->OS[index][j];
    for (int h = 0; h < order; h++) {
      output->state->values[j * coeffs + h] = output_vars[k * coeffs + h];
    }
  }
  for (int j = 0; j < nOD; j++) {
    int k = sim_output->OD[index][j];
    output->state->values[nStates + j] = sim_data->d[k];
  }
  LG_writeLine(output->state->log, variable, sim_time->time, output->state->values);
}

void QSS_FUNC_DECL(SAM, init)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output)
{
  int i;
  int max = 0;
  int max_discrete = 0;
  int outputs = sim_output->outputs;
  int size = outputs;
  QSS_PARALLEL_EXP(QSS_LP_data lp = sim_data->lp;)
  QSS_PARALLEL_EXP(size = lp->outputs;)
  if (size > 0) {
    if (sim_data->params->x_output == 1) {
      output->state->output_var = sim_data->x;
      output->state->order = sim_data->order + 1;
    } else {
      output->state->output_var = sim_data->q;
      output->state->order = sim_data->order;
    }
    output->state->log = LG_Log(sim_data, sim_output);
    output->state->steps = (int *)malloc(outputs * sizeof(int));
    for (int j = 0; j < outputs; j++) {
      if (max < sim_output->nOS[j]) {
        max = sim_output->nOS[j];
      }
      if (max_discrete < sim_output->nOD[j]) {
        max_discrete = sim_output->nOD[j];
      }
    }
    size_t values_nbr = (((sim_data->order + 1) * max) + max_discrete);
    if (values_nbr > 0) {
      output->state->values = (double *)malloc(values_nbr * sizeof(double));
    }
    i = 0;
    for (int j = 0; j < outputs; j++) {
      QSS_PARALLEL_IF(lp->oMap[j])
      QSS_FUNC_INVK(SAM, writeOutvar)(output, sim_data, sim_time, sim_output, j, i);
      output->state->steps[i++] = 1;
      QSS_PARALLEL_IF_END
    }
    QSS_ASSIGN_OUTPUT_OPS(SAM)
  }
}

void QSS_FUNC_DECL(SAM, initOutputVar)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output,
                                       int output_index)
{
  QSS_PARALLEL_EXP(QSS_LP_data lp = sim_data->lp;)
  int variable = output_index;
  QSS_PARALLEL_IF(lp->oMap[output_index])
  QSS_PARALLEL_EXP(variable = lp->oMap[output_index];)
  QSS_FUNC_INVK(SAM, writeOutvar)(output, sim_data, sim_time, sim_output, output_index, variable);
  output->state->steps[variable]++;
  QSS_PARALLEL_IF_END
}

void QSS_FUNC_DECL(SAM, write)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output)
{
  if (sim_time->type == ST_State) {
    int nSO = sim_output->nSO[sim_time->minIndex];
    for (int i = 0; i < nSO; i++) {
      int j = sim_output->SO[sim_time->minIndex][i];
      QSS_FUNC_INVK(SAM, initOutputVar)(output, sim_data, sim_time, sim_output, j);
    }
  } else if (sim_time->type == ST_Event) {
    int nLHSSt = sim_data->event[sim_time->minIndex].nLHSSt;
    for (int k = 0; k < nLHSSt; k++) {
      int h = sim_data->event[sim_time->minIndex].LHSSt[k];
      int nSO = sim_output->nSO[h];
      for (int i = 0; i < nSO; i++) {
        int j = sim_output->SO[h][i];
        QSS_FUNC_INVK(SAM, initOutputVar)(output, sim_data, sim_time, sim_output, j);
      }
    }
    int nLHSDsc = sim_data->event[sim_time->minIndex].nLHSDsc;
    for (int k = 0; k < nLHSDsc; k++) {
      int h = sim_data->event[sim_time->minIndex].LHSDsc[k];
      int nDO = sim_output->nDO[h];
      for (int i = 0; i < nDO; i++) {
        int j = sim_output->DO[h][i];
        QSS_FUNC_INVK(SAM, initOutputVar)(output, sim_data, sim_time, sim_output, j);
      }
    }
  }
}

int QSS_FUNC_DECL(SAM, getSteps)(const_OUT_output output, int var) { return output->state->steps[var]; }

void QSS_FUNC_DECL(SAM, save)(OUT_output output) { LG_toFile(output->state->log); }
