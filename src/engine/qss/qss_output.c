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

#include <common/macros.h>
#include <common/utils.h>
#include <qss/qss_output.h>
#include "qss_sampled.h"
#include "qss_step.h"

OUT_outputState OUT_OutputState()
{
  OUT_outputState p = checkedMalloc(sizeof(*p));
  p->log = NULL;
  p->steps = NULL;
  p->values = NULL;
  p->output_var = NULL;
  p->order = 0;
  return p;
}

OUT_outputOps OUT_OutputOps()
{
  OUT_outputOps p = checkedMalloc(sizeof(*p));
  p->getSteps = NULL;
  p->save = NULL;
  p->write = NULL;
  return p;
}

OUT_output OUT_Output(QSS_data sim_data, QSS_time sim_time, SD_output sim_output)
{
  OUT_output p = checkedMalloc(sizeof(*p));
  p->state = OUT_OutputState();
  p->ops = OUT_OutputOps();
  switch (sim_output->commInterval) {
  case CI_Step:
    QSS_MODULE_INIT(ST, p, sim_data, sim_time, sim_output)
    break;
  case CI_Dense:
  case CI_Sampled:
    QSS_MODULE_INIT(SAM, p, sim_data, sim_time, sim_output)
    break;
  default:
    return NULL;
  }
  return p;
}

void OUT_freeOutputOps(OUT_outputOps o) { free(o); }

void OUT_freeOutputState(OUT_outputState o)
{
  LG_freeLog(o->log);
  if (o->steps != NULL) {
    free(o->steps);
  }
  if (o->values != NULL) {
    free(o->values);
  }
  free(o);
}

void OUT_freeOutput(OUT_output o)
{
  if (o != NULL) {
    OUT_freeOutputState(o->state);
    OUT_freeOutputOps(o->ops);
    free(o);
  }
}

void OUT_write(OUT_output output, QSS_data sim_data, QSS_time sim_time, SD_output sim_output)
{
  output->ops->write(output, sim_data, sim_time, sim_output);
}

int OUT_getSteps(const_OUT_output output, int var) { return output->ops->getSteps(output, var); }

void OUT_save(OUT_output output) { output->ops->save(output); }
