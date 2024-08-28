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

#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_log.h>
#include <qss/qss_step.h>

void QSS_FUNC_DECL(ST, writeOutvar)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output,
                                    int index, int variable)
{
  double output_val;
  int nOS = sim_output->nOS[index];
  int order = sim_data->order - 1;
  int coeffs = order + 2;
  double *q = sim_data->q;
  double tmp[nOS];
  for (int j = 0; j < nOS; j++) {
    int k = sim_output->OS[index][j];
    double elapsed = sim_time->time - sim_time->tq[k];
    int cf0 = k * coeffs;
    tmp[j] = q[cf0];
    if (elapsed != 0) {
      q[cf0] = evaluatePoly(cf0, elapsed, q, order);
    }
  }
  sim_output->value(index, q, sim_data->d, sim_data->alg, sim_time->time, &output_val);
  LG_write(output->state->log, variable, sim_time->time, output_val);
  for (int j = 0; j < nOS; j++) {
    int k = sim_output->OS[index][j];
    int cf0 = k * coeffs;
    q[cf0] = tmp[j];
  }
}

void QSS_FUNC_DECL(ST, init)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output)
{
  int j;
  int outputs = sim_output->outputs;
  int size = outputs;
  QSS_PARALLEL_EXP(QSS_LP_data lp = sim_data->lp;)
  QSS_PARALLEL_EXP(size = lp->outputs;)
  if (size) {
    output->state->steps = (int *)malloc(outputs * sizeof(int));
    output->state->log = LG_Log(sim_data, sim_output);
    j = 0;
    for (int i = 0; i < outputs; i++) {
      QSS_PARALLEL_EXP(if (lp->oMap[i] > NOT_ASSIGNED) {)
      QSS_FUNC_INVK(ST, writeOutvar)(output, sim_data, sim_time, sim_output, i, j);
        output->state->steps[j++] = 1;
      QSS_PARALLEL_EXP(
      })
    }
    QSS_ASSIGN_OUTPUT_OPS(ST)
  }
}

void QSS_FUNC_DECL(ST, write)(OUT_output output, const_QSS_data sim_data, const_QSS_time sim_time, const_SD_output sim_output)
{
  QSS_PARALLEL_EXP(QSS_LP_data lp = sim_data->lp;)
  if (sim_time->type == ST_State) {
    int nSO = sim_output->nSO[sim_time->minIndex];
    for (int i = 0; i < nSO; i++) {
      int j = sim_output->SO[sim_time->minIndex][i];
      int variable = j;
      QSS_PARALLEL_EXP(if (lp->oMap[j] > NOT_ASSIGNED) {)
      QSS_PARALLEL_EXP(variable = lp->oMap[j];)
      QSS_FUNC_INVK(ST, writeOutvar)(output, sim_data, sim_time, sim_output, j, variable);
        output->state->steps[variable]++;
      QSS_PARALLEL_EXP(
      })
    }
  } else if (sim_time->type == ST_Event) {
    int nLHSSt = sim_data->event[sim_time->minIndex].nLHSSt;
    for (int k = 0; k < nLHSSt; k++) {
      int h = sim_data->event[sim_time->minIndex].LHSSt[k];
      int nSO = sim_output->nSO[h];
      for (int i = 0; i < nSO; i++) {
        int j = sim_output->SO[h][i];
        int variable = j;
        QSS_PARALLEL_EXP(if (lp->oMap[j] > NOT_ASSIGNED) {)
        QSS_PARALLEL_EXP(variable = lp->oMap[j];)
        QSS_FUNC_INVK(ST, writeOutvar)(output, sim_data, sim_time, sim_output, j, variable);
          output->state->steps[variable]++;
        QSS_PARALLEL_EXP(
        })
      }
    }
    int nLHSDsc = sim_data->event[sim_time->minIndex].nLHSDsc;
    for (int k = 0; k < nLHSDsc; k++) {
      int h = sim_data->event[sim_time->minIndex].LHSDsc[k];
      int nDO = sim_output->nDO[h];
      for (int i = 0; i < nDO; i++) {
        int j = sim_output->DO[h][i];
        int variable = j;
        QSS_PARALLEL_EXP(if (lp->oMap[j] > NOT_ASSIGNED) {)
        QSS_PARALLEL_EXP(variable = lp->oMap[j];)
        QSS_FUNC_INVK(ST, writeOutvar)(output, sim_data, sim_time, sim_output, j, variable);
          output->state->steps[variable]++;
        QSS_PARALLEL_EXP(
        })
      }
    }
  }
}

int QSS_FUNC_DECL(ST, getSteps)(const_OUT_output output, int var) { return output->state->steps[var]; }

void QSS_FUNC_DECL(ST, save)(OUT_output output) { LG_toFile(output->state->log); }
