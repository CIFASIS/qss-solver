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

#include "qss/qss_commands.h"

#include <stdlib.h>

#include <common/data.h>
#include <common/utils.h>
#include <qss/qss_data.h>
#include <qss/qss_scheduler.h>
#include "qss_simulator.h"

static QSS_simulator *_simulator = NULL;

void QSS_terminate(SC_scheduler scheduler, QSS_data simData, QSS_time simTime)
{
  simData->ft = simTime->time;
  simTime->time = INF;
}

void QSS_CMD_alloc(QSS_simulator simulator)
{
  int lps = simulator->data->params->lps;
  if (lps == 0) {
    lps = 1;
  }
  _simulator = checkedMalloc(lps * sizeof(QSS_simulator *));
}

void QSS_CMD_terminate()
{
  int i, lps = _simulator[0]->data->params->lps;
  if (lps == 0) {
    lps = 1;
  }
  for (i = 0; i < lps; i++) {
    SC_setUpdate(_simulator[i]->scheduler, QSS_terminate);
  }
}

void QSS_CMD_init(QSS_simulator simulator) { _simulator[simulator->id] = simulator; }

void QSS_CMD_free()
{
  if (_simulator != NULL) {
    free(_simulator);
  }
}
