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
#include <qss/qss_bdf.h>

typedef struct QA_quantizerOps_ *QA_quantizerOps;
typedef struct QA_quantizerState_ *QA_quantizerState;
typedef struct QA_quantizer_ *QA_quantizer;

/**
 * The function recomputeNextTimes iterates over a set of influenced variables and calls the recomputeNextTime
 * function for each variable.
 * 
 * @param quantizer The `quantizer` parameter is of type `QA_quantizer` and represents a quantizer
 * object. It is used to access the state and other properties of the quantizer.
 * @param vars The variable "vars" represents the number of influenced variables.
 * @param inf An array of integers representing the indices of the variables to be recomputed. @see QSS_data_ SD matrix.
 * @param t The parameter "t" represents the current time.
 * @param nTime The `nTime` parameter is a pointer to a double variable that will store the next time
 * at which the state variables need to be updated.
 * @param x An array of current state values for the variables in the system.
 * @param lqu The array "lqu" represents the current DQ value for the i-th varaible.
 * @param q The parameter "q" is array containing the quantized state values.
 */
typedef void (*QA_recomputeNextTimesFn)(QA_quantizer, int, int *, double, double *, double *, double *, double *);

/**
 * The function `recomputeNextTime` calculates the next time step for a given quantizer and updates the
 * nTime array accordingly. It's called when the derivate of a state variable changes due to a change
 * in some variable that appear on the right hand side of it's definition.
 * 
 * @param quantizer The `quantizer` parameter is of type `QA_quantizer` and represents a quantizer
 * object.
 * @param i The parameter `i` represents the index of the variable for which the next time is being
 * recomputed.
 * @param t The parameter "t" represents the current simulation time.
 * @param nTime A pointer to a double variable that will store the next time value for the computation.
 * @param x An array containing the current values of the state variables.
 * @param lqu The array "lqu" represents the current DQ value for the i-th varaible.
 * @param q The parameter `q` is array containing the quantized state values.
 */

typedef void (*QA_recomputeNextTimeFn)(QA_quantizer, int, double, double *, double *, double *, double *);

/**
 * The function calculates the next time step for a specific variable based on the current time, state
 * variables, and a quantizer. It's called after the quantized state change, i.e. when the difference
 * between x and q reaches DQ. So no derivatives are updated at this point.  
 * 
 * @param quantizer The `quantizer` parameter is of type `QA_quantizer` and is used to perform
 * quantization operations.
 * @param i The parameter "i" represents the index of the variable for which we are calculating the
 * next time.
 * @param t The parameter "t" represents the current time.
 * @param nTime nTime is an array of doubles that stores the next time values for each element in the
 * system.
 * @param x An array of values representing the state variables of the system.
 * @param lqu The array "lqu" represents the current DQ value for the i-th varaible.
 */
typedef void (*QA_nextTimeFn)(QA_quantizer, int, double, double *, double *, double *);

/**
 * The function `updateQuantizedState` updates the quantized state variables. It's called from the engine
 * when the time computed for  $|q - x| < \deltaQ$ is reached.
 * 
 * @param quantizer The `quantizer` parameter is a pointer to a structure that contains various
 * variables and arrays used in the quantization process.
 * @param i The parameter "i" is an integer representing the index of the state variable being updated.
 * @param q An array of quantized variables.
 * @param x An array of state variables.
 * @param lqu The array "lqu" represents the current DQ value for the i-th varaible.
 */
typedef void (*QA_updateQuantizedStateFn)(QA_quantizer, int, double *, double *, double *);

/**
 * @struct QA_quantizerState_
 * 
 * @brief This struct represents the state of a quantizer in a simulation. 
 * 
 * It contains all the properties needed by different QSS quantizers.
 * 
 * @var QA_quantizerState_::order
 * @brief  "order" property represents the order of the QSS method used.
 * 
 * @var QA_quantizerState_::xOrder 
 * @brief The `xOrder` property represents the order of the state variables which is 
 * equal to $order + 1$.
 * 
 * @var QA_quantizerState_::flag2
 * @brief flag2 array of integers, should be deprectaded.
 * 
 * @var QA_quantizerState_::flag3
 * @brief The "flag3" array of integers, should be deprectaded.
 * 
 * @var QA_quantizerState_::flag4
 * @brief The "flag4" array of integers, should be deprectaded.
 * 
 * @var QA_quantizerState_::BDF
 * @brief BDF is a pointer to QSS_data BDF array that contains the BDF partition of the model in mixed mode methods. It's used
 * to select the influenced variables that are computed by LIQSS (the ones that are not assigned to BDF).
 * 
 * @var QA_quantizerState_::nSZ
 * @brief nSZ is a pointer to QSS_data nSZ array.
 * 
 * @var QA_quantizerState_::SZ
 * @brief SZ is a pointer is a pointer to QSS_data SZ matrix.
 * 
 * @var QA_quantizerState_:: dq
 * @brief dq is a pointer to an array of double values. Should be deprecated.
 * 
 * @var QA_quantizerState_::a
 * @brief a is a pointer to an array of double values used to store the Jacobian values in LIQSS methods.
 * 
 * @var QA_quantizerState_::oldDx
 * @brief The `oldDx` property is a pointer to an array of `double` values. It is
 * used to store the previous values of the derivative of the state variables during the quantization
 * process.
 * 
 * @var QA_quantizerState_::qAux
 * @brief qAux is a pointer to an array of double values. It is used as auxiliary
 * storage for the quantizer state. Should be deprecated.
 * 
 * @var QA_quantizerState_::u0
 * @brief u0 is a pointer to an array of double values. Used to store the affine coefficients in LIQSS methods. 
 * 
 * @var QA_quantizerState_::u1
 * @brief The property "u1" is a pointer to a double. Used to store the estimated derivatives of the affine coefficients 
 * in LIQSS methods.
 * 
 * @var QA_quantizerState_::u2
 * @brief The property "u2" is a pointer to a double. Used to store the estimated derivatives of u1 affine coefficients 
 * in LIQSS methods.
 * 
 * @var QA_quantizerState_::tx
 * @brief The "tx" property is a pointer to an array of type double. It is used to
 * store the values of time points in the simulation. Should be deprecated.
 * 
 * @var QA_quantizerState_::lt
 * @brief lt is a pointer to an array of type double. It is used to store the values
 * of the lt variable in the struct. Should be deprecated.
 * 
 * @var QA_quantizerState_::ltq
 * @brief ltq is a pointer to an array of double values. It points to QSS_data lqu array.
 * 
 * @var QA_quantizerState_::lquOld
 * @brief Array that contains the previous value of the last quantum update, it's only used in LIQSS3 method.
 * 
 * @var QA_quantizerState_::qinf
 * @brief qinf is a pointer to an array of double values. It represents the lower
 * bound for each state variable in the quantizer. Used in CQSS methods, it should be deprecated.
 * 
 * @var QA_quantizerState_::qsup
 * @brief qsup is a pointer to a double array. It is used to store the upper limit
 * values for the quantized state variables.  Used in CQSS methods, it should be deprecated
 * 
 * @var QA_quantizerState_::simTime
 * @brief The `simTime` property is a pointer 
 * 
 * @var QA_quantizerState_::minStep
 * @brief The minimum step size for the quantizer.
 * 
 * @var QA_quantizerState_::finTime
 * @brief The `finTime` property represents the final time of the simulation. It
 * is a double value that indicates the time at which the simulation should stop.
 * 
 * @var QA_quantizerState_::lSimTime
 * @brief lSimTime is a variable of type QSS_time, which represents the
 * simulation time. This struct handles the time during simulation, including quantized and state variables current time, 
 * event times, current simulation time, etc. @see QSS_time. 
 * 
 * @var QA_quantizerState_::qMap
 * @brief qMap is a variable of type QSS_idxMap, used in parallel simulations to keep track of the computed
 * variables on each Logical Process.
 * 
 * @var QA_quantizerState_::sts
 * @brief The "sts" property is an integer that represents the number of state variables in the system.
 * 
 * @var QA_quantizerState_::infs
 * @brief The "infs" property is an integer that represents the number of total influenced
 * equations. It should be deprecated.
 * 
 * @var QA_quantizerState_::nSD
 * @brief nSD is a pointer to QSS_data nSD array
 * 
 * @var QA_quantizerState_::SD
 * @brief SD is a pointer to QSS_data SD matrix.
 * 
 * @var QA_quantizerState_::S
 * @brief The "S" matrix is used to map indexes in LIQSS3 method, it should be deprecated and use the same strategy as in LIQSS2 method.
 * 
 * @var QA_quantizerState_::sim_step
 * @brief `sim_step` is a boolean array that represents whether a simultaneous step should be taken in mLIQSS2 methods given that the value j-th
 * quantized state was updated during the i-th quantized state updated to avoid oscilations.
 * 
 * @var QA_quantizerState_::A
 * @brief A is the Jacobian matrix used in mLIQSS methods.
 * 
 * @var QA_quantizerState_::U0
 * @brief U0 is the affine coefficient matrix used in mLIQSS methods.
 * 
 * @var QA_quantizerState_::U1
 * @brief U1 is the affine coefficient derivative numerical estimation matrix used in mLIQSS methods.
 * 
 * @var QA_quantizerState_::qj
 * @brief In mLIQSS2 methods, qj is an array that contains the value of the j-th variable updated due to a change in the i-th variable. This value is needed to 
 * interact with the QSS engine. When an oscilation is detected, `sim_step` property is set for j and when the derivatives are updated it will set a next state 
 * change for the current simulation time that will update the second state variable. 
 */
struct QA_quantizerState_ {
  int order;
  int xOrder;
  int *flag2;
  int *flag3;
  int *flag4;
  int *BDF;
  int *nSZ;
  int **SZ;
  double *dq;
  double *a;
  double *oldDx;
  double *qAux;
  double *u0;
  double *u1;
  double *u2;
  double *tx;
  double *lt;
  double *ltq;
  double *lquOld;
  double *qinf;
  double *qsup;
  double *simTime;
  double minStep;
  double finTime;
  QSS_time lSimTime;
  QSS_idxMap qMap;

  // Added state for mLIQSS methods.
  int sts;
  int infs;
  int *nSD;
  int **SD;
  int **S;
  bool *sim_step;
  double **A;
  double **U0;
  double **U1;
  double *qj;
};

/**
 * @struct QA_quantizerOps_
 * 
 * The above type defines a struct that contains function pointers for various operations related to
 * quantization.
 * 
 * @var QA_quantizerState_:: {QA_recomputeNextTimesFn} recomputeNextTimes - A function pointer that takes no arguments
 * and returns no value. It is used to recompute the next times for the quantizer.
 * @var QA_quantizerState_:: {QA_recomputeNextTimeFn} recomputeNextTime - A function that takes in a quantized state
 * and returns the next time at which the state should be recomputed.
 * @var QA_quantizerState_:: {QA_nextTimeFn} nextTime - The nextTime property is a function that calculates the next
 * time step for the quantizer.
 * @var QA_quantizerState_:: {QA_updateQuantizedStateFn} updateQuantizedState - The updateQuantizedState property is a
 * function that is responsible for updating the quantized state of the QA_quantizerOps_ struct.
 */
struct QA_quantizerOps_ {
  QA_recomputeNextTimesFn recomputeNextTimes;
  QA_recomputeNextTimeFn recomputeNextTime;
  QA_nextTimeFn nextTime;
  QA_updateQuantizedStateFn updateQuantizedState;
};

struct QA_quantizer_ {
  QA_quantizerState state;
  QA_quantizerOps ops;
};

QA_quantizerOps QA_QuantizerOps();

QA_quantizerState QA_QuantizerState();

QA_quantizer QA_Quantizer(QSS_data simData, QSS_time simTime);

void QA_freeQuantizerState(QA_quantizerState state);

void QA_freeQuantizerOps(QA_quantizerOps ops);

void QA_freeQuantizer(QA_quantizer quantizer);

void QA_recomputeNextTimes(QA_quantizer quantizer, int vars, int *inf, double t, double *nTime, double *x, double *lqu, double *q);

void QA_recomputeNextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu, double *q);

void QA_nextTime(QA_quantizer quantizer, int var, double t, double *nTime, double *x, double *lqu);

void QA_updateQuantizedState(QA_quantizer quantizer, int var, double *q, double *x, double *lqu);
