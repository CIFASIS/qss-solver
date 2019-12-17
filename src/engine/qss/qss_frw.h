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

#ifndef QSS_FRW_H_
#define QSS_FRW_H_

#include "qss_data.h"

/**
 *
 */
typedef struct FRW_frameworkState_ *FRW_frameworkState;

/**
 *
 */
typedef struct FRW_frameworkOps_ *FRW_frameworkOps;

/**
 *
 */
typedef struct FRW_framework_ *FRW_framework;

/**
 *
 * @param QSS_model
 * @param QSS_data
 * @param QSS_time
 * @param
 * @param
 */
typedef void (*FRW_recomputeDerivativesFn)(FRW_framework, QSS_model, QSS_data, QSS_time, int);

/**
 *
 * @param QSS_model
 * @param QSS_data
 * @param QSS_time
 * @param
 * @param
 */
typedef void (*FRW_recomputeDerivativeFn)(FRW_framework, QSS_model, QSS_data, QSS_time, int);

/**
 *
 * @param QSS_model
 * @param QSS_data
 * @param QSS_time
 * @param
 */
typedef void (*FRW_nextEventTimeFn)(FRW_framework, QSS_model, QSS_data, QSS_time, int);

/**
 *
 * @param QSS_model
 * @param QSS_data
 * @param QSS_time
 * @param
 * @param
 * @param
 */
typedef void (*FRW_nextInputTimeFn)(FRW_framework, QSS_model, QSS_data, QSS_time, double, int, int);

/**
 *
 */
struct FRW_frameworkState_ {
  double delta;   //!<
  double dxnOld;  //!<
};

/**
 *
 */
struct FRW_frameworkOps_ {
  FRW_recomputeDerivativesFn recomputeDerivatives;  //!<
  FRW_recomputeDerivativeFn recomputeDerivative;    //!<
  FRW_nextEventTimeFn nextEventTime;                //!<
  FRW_nextInputTimeFn nextInputTime;                //!<
};

/**
 *
 */
struct FRW_framework_ {
  FRW_frameworkState state;  //!<
  FRW_frameworkOps ops;      //!<
};

/**
 *
 * @param simData
 * @return
 */
FRW_framework FRW_Framework(QSS_data simData);

/**
 *
 * @param f
 */
void FRW_freeFramework(FRW_framework f);

/**
 *
 * @return
 */
FRW_frameworkState FRW_FrameworkState();

/**
 *
 * @param f
 */
void FRW_freeFrameworkState(FRW_frameworkState f);

/**
 *
 * @return
 */
FRW_frameworkOps FRW_FrameworkOps();

/**
 *
 * @param f
 */
void FRW_freeFrameworkOps(FRW_frameworkOps f);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void FRW_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void FRW_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void FRW_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param framework
 * @param model
 * @param data
 * @param time
 * @param elapsed
 * @param var
 * @param index
 */
void FRW_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData, QSS_time simTime, double elapsed, int var, int index);

#endif /* QSS_FRW_H_ */
