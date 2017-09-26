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

#include "simulator.h"

#include <stdlib.h>

#include "../classic/classic_simulator.h"
#include "../qss/qss_simulator.h"
#include "commands.h"
#include "utils.h"

SIM_simulator
SIM_Simulator(SD_simulationSettings settings)
{
  SIM_simulator p = checkedMalloc(sizeof(*p));
  p->ops = SIM_SimulatorOps();
  p->state = SIM_SimulatorState();
  p->state->settings = settings;
  if(settings->method == SD_DASSL ||
     settings->method == SD_DOPRI ||
     settings->method == SD_CVODE_BDF ||
     settings->method == SD_CVODE_AM ||
     settings->method == SD_IDA)
  {
    CLC_initSimulator(p);
  }
  else
  {
    QSS_initSimulator(p);
  }
  CMD_init(settings);
  return p;
}

void
SIM_freeSimulator(SIM_simulator simulator)
{
  simulator->ops->freeSimulator(simulator);
  SIM_freeSimulatorState(simulator->state);
  SIM_freeSimulatorOps(simulator->ops);
  free(simulator);
}

SIM_simulatorState
SIM_SimulatorState()
{
  SIM_simulatorState p = checkedMalloc(sizeof(*p));
  p->sim = NULL;
  p->settings = NULL;
  return p;
}

void
SIM_freeSimulatorState(SIM_simulatorState state)
{
  free(state);
}

SIM_simulatorOps
SIM_SimulatorOps()
{
  SIM_simulatorOps p = checkedMalloc(sizeof(*p));
  p->freeSimulator = NULL;
  p->simulate = NULL;
  return p;
}

void
SIM_freeSimulatorOps(SIM_simulatorOps ops)
{
  free(ops);
}

void
SIM_simulate(SIM_simulator simulator)
{
  simulator->ops->simulate(simulator);
}
