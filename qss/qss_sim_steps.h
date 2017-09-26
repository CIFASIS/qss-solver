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

#ifndef QSS_SIM_STEPS_H_
#define QSS_SIM_STEPS_H_

#include "../common/utils.h"

#define SIM_STEPS_BUFFER 2000

typedef struct QSS_simSteps_ *QSS_simSteps;

typedef struct QSS_simStepsState_ *QSS_simStepsState;

typedef struct QSS_simStepsOps_ *QSS_simStepsOps;

typedef struct QSS_stepInfo_ QSS_stepInfo;

typedef void
(*QSS_simStepsAddFn)(QSS_simSteps, IBX_message, int);

typedef bool
(*QSS_simStepsEmptyFn)(QSS_simSteps);

typedef QSS_stepInfo
(*QSS_simStepsItFn)(QSS_simSteps);

typedef void
(*QSS_simStepsResetFn)(QSS_simSteps);

struct QSS_stepInfo_
{
    IBX_message msg;
    int synch;
};

struct QSS_simStepsState_
{
    QSS_stepInfo steps[SIM_STEPS_BUFFER];
    int size;
    unsigned int current;
    unsigned int it;
};

QSS_simStepsState
QSS_SimStepsState();

void
QSS_freeSimStepsState(QSS_simStepsState state);

struct QSS_simStepsOps_
{
    QSS_simStepsAddFn add;
    QSS_simStepsResetFn reset;
    QSS_simStepsEmptyFn empty;
    QSS_simStepsItFn first;
    QSS_simStepsItFn next;
};

QSS_simStepsOps
QSS_SimStepsOps();

void
QSS_freeSimStepsOps(QSS_simStepsOps ops);

struct QSS_simSteps_
{
    QSS_simStepsState state;
    QSS_simStepsOps ops;
};

QSS_simSteps
QSS_SimSteps();

void
QSS_freeSimSteps(QSS_simSteps s);

void
QSS_SIS_add(QSS_simSteps steps, IBX_message msg, int synch);

void
QSS_SIS_reset(QSS_simSteps steps);

bool
QSS_SIS_empty(QSS_simSteps steps);

QSS_stepInfo
QSS_SIS_first(QSS_simSteps steps);

QSS_stepInfo
QSS_SIS_next(QSS_simSteps steps);

#endif /* QSS_SIM_STEPS_H_ */
