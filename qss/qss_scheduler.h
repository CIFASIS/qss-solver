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

#ifndef QSS_SCHEDULER_H_
#define QSS_SCHEDULER_H_

#include "../common/tree.h"
#include "qss_data.h"

/**
 *
 */
typedef struct SC_schedulerState_ *SC_schedulerState;

/**
 *
 */
typedef struct SC_schedulerOps_ *SC_schedulerOps;

/**
 *
 */
typedef struct SC_scheduler_ *SC_scheduler;

/**
 *
 * @param QSS_data
 * @param QSS_time
 */
typedef void
(*SC_updateFn)(SC_scheduler, QSS_data, QSS_time);

/**
 *
 * @param QSS_data
 * @param QSS_time
 */
typedef void
(*BT_upd)(SC_scheduler, QSS_data, QSS_time);

/**
 *
 */
struct SC_schedulerState_
{
    BT_upd update; //!<
    BT_tree states; //!<
    BT_tree events; //!<
    BT_tree inputs; //!<
    BT_info info; //!<
    BTR_tree BTR_states; //!<
    BTR_tree BTR_events; //!<
    BTR_tree BTR_inputs; //!<
    BT_info visit; //!<
    double minInputValue; //!<
    double minEventValue; //!<
    double tUnit; //!<
    double tOut; //!<
    int minInputIndex; //!<
    int minEventIndex; //!<
    QSS_StepType stepType; //!<
    QSS_LP_data lp; //!<
};

/**
 *
 */
struct SC_schedulerOps_
{
    SC_updateFn update; //!<
};

/**
 *
 */
struct SC_scheduler_
{
    SC_schedulerState state; //!<
    SC_schedulerOps ops; //!<
};

/**
 *
 * @return
 */
SC_schedulerState
SC_SchedulerState();

/**
 *
 * @param scheduler
 */
void
SC_freeSchedulerState(SC_schedulerState scheduler);

/**
 *
 * @return
 */
SC_schedulerOps
SC_SchedulerOps();

/**
 *
 * @param scheduler
 */
void
SC_freeSchedulerOps(SC_schedulerOps scheduler);

/**
 *
 * @param simData
 * @param simTime
 * @return
 */
SC_scheduler
SC_Scheduler(QSS_data simData, QSS_time simTime);

/**
 *
 * @param scheduler
 */
void
SC_freeScheduler(SC_scheduler scheduler);

/**
 *
 * @param scheduler
 * @param simData
 * @param simTime
 */
void
SC_update(SC_scheduler scheduler, QSS_data simData, QSS_time simTime);

/**
 *
 * @param scheduler
 * @param simData
 * @param simTime
 */
void
SC_setUpdate(SC_scheduler scheduler, BT_upd updFunction);

#endif  /* QSS_SCHEDULER_H_ */
