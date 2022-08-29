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

#include "qss_sim_steps.h"

#include <stdlib.h>

QSS_simStepsState QSS_SimStepsState()
{
  QSS_simStepsState p = checkedMalloc(sizeof(*p));
  p->current = 0;
  p->it = 0;
  p->size = 0;
  int i;
  for (i = 0; i < SIM_STEPS_BUFFER; i++) {
    p->steps[i].synch = NOT_ASSIGNED;
  }
  return p;
}

void QSS_freeSimStepsState(QSS_simStepsState state) { free(state); }

void QSS_SimStepsAdd(QSS_simSteps steps, IBX_message msg, int synch)
{
  steps->state->steps[steps->state->current].msg = msg;
  steps->state->steps[steps->state->current++].synch = synch;
  steps->state->size++;
}

bool QSS_SimStepsEmpty(QSS_simSteps steps) { return steps->state->size <= 0; }

void QSS_SimStepsReset(QSS_simSteps steps)
{
  steps->state->current = 0;
  steps->state->it = 0;
  steps->state->size = 0;
}

QSS_stepInfo QSS_SimStepsFirst(QSS_simSteps steps)
{
  steps->state->size--;
  return steps->state->steps[steps->state->it++];
}

QSS_stepInfo QSS_SimStepsNext(QSS_simSteps steps)
{
  steps->state->size--;
  return steps->state->steps[steps->state->it++];
}

QSS_simStepsOps QSS_SimStepsOps()
{
  QSS_simStepsOps p = checkedMalloc(sizeof(*p));
  p->add = QSS_SimStepsAdd;
  p->reset = QSS_SimStepsReset;
  p->empty = QSS_SimStepsEmpty;
  p->first = QSS_SimStepsFirst;
  p->next = QSS_SimStepsNext;
  return p;
}

void QSS_freeSimStepsOps(QSS_simStepsOps ops) { free(ops); }

QSS_simSteps QSS_SimSteps()
{
  QSS_simSteps p = checkedMalloc(sizeof(*p));
  p->ops = QSS_SimStepsOps();
  p->state = QSS_SimStepsState();
  return p;
}

void QSS_freeSimSteps(QSS_simSteps s)
{
  QSS_freeSimStepsOps(s->ops);
  QSS_freeSimStepsState(s->state);
  free(s);
}

void QSS_SIS_add(QSS_simSteps steps, IBX_message msg, int synch) { return steps->ops->add(steps, msg, synch); }

void QSS_SIS_reset(QSS_simSteps steps) { return steps->ops->reset(steps); }

bool QSS_SIS_empty(QSS_simSteps steps) { return steps->ops->empty(steps); }

QSS_stepInfo QSS_SIS_first(QSS_simSteps steps) { return steps->ops->first(steps); }

QSS_stepInfo QSS_SIS_next(QSS_simSteps steps) { return steps->ops->next(steps); }
