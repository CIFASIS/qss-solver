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

#pragma once

#include <qss/qss_data.h>
#include <qss/qss_simulator.h>

#define PAR_NO_ERROR 0
#define PAR_ERR_CREATE_THREAD -1
#define PAR_ERR_SET_AFFINITY_MASK -2
#define PAR_ERR_GET_AFFINITY_MASK -3

int PAR_createLPTasks(QSS_sim simulate, QSS_simulator simulator);

int PAR_cleanLPTask(int lp);

int PAR_initLPTasks(int lp);
