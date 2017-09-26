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

#include "commands.h"

#include <stddef.h>

#include "../classic/classic_commands.h"
#include "../qss/qss_commands.h"

static CMD_terminateFn _terminate = NULL;

void
CMD_init(SD_simulationSettings settings)
{
  if(settings->method == SD_DOPRI || settings->method == SD_DASSL)
  {
    _terminate = CLC_CMD_terminate;
  }
  else
  {
    _terminate = QSS_CMD_terminate;
  }
}

void
CMD_terminate()
{
  _terminate();
}
