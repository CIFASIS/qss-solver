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

#include "data.h"
#include "model.h"
#include "simulator.h"

#ifdef __linux__
#include <signal.h> 
#include <stdlib.h> 
void
segfault_hdlr(int dummy)
{
  printf("Segmentation fault detected\n");
  exit(-1);
}
#endif

int
main()
{

#ifdef __linux__
  signal(SIGSEGV, segfault_hdlr);
#endif
  SD_simulationSettings settings = SD_SimulationSettings();
  MOD_settings(settings);
  SIM_simulator simulator = SIM_Simulator(settings);
  SIM_simulate(simulator);
  SIM_freeSimulator(simulator);
  return 0;
}
