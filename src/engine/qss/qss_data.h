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

/**
 * @brief This header provides all the data structures used by the simulation
 * engine.
 *
 * All the structures defined here, all visible from all the layers in the
 * system architecture.
 *
 */

#pragma once

#include <common/data.h>
#include <common/utils.h>

#define QSS_REINIT_BUFFER 10000

typedef int *QSS_idxMap;

typedef void *(QSS_sim)(void *);

typedef struct QSS_reinit_ *QSS_reinit;

struct QSS_reinit_ {
  int variable[QSS_REINIT_BUFFER];
  double time;
  int counter;
};

QSS_reinit QSS_Reinit();

void QSS_freeReinit(QSS_reinit reinit);

typedef void (*QSS_fp)(void);

typedef int (*QSS_fpr)(int);

typedef void (*QSS_fpa)(int);

typedef void (*QSS_eq)(int, double *, double *, double *, double, double *);

typedef void (*QSS_dep)(int, double *, double *, double *, double, double *, int *);

typedef void (*QSS_zc)(int, double *, double *, double *, double, double *);

typedef void (*QSS_hnd)(int, double *, double *, double *, double *, double);

typedef void (*QSS_input)(double, double *);

typedef void (*QSS_jac)(double *, double *, double *, double, SD_jacMatrices, double *);

typedef void (*QSS_fmodel)(double *, double *, double *, double, double *, int *, int);

/**
 * @enum QSS_SchedulerType
 * @brief QSS simulator scheduler type definition.
 *
 * @var QSS_SchedulerType::ST_Linear
 * @brief Linear search scheduler.
 *
 * @var QSS_SchedulerType::ST_Binary
 * @brief
 *
 * @var QSS_SchedulerType::ST_Random
 * @brief
 *
 */
typedef enum { ST_Linear, ST_Binary, ST_Random } QSS_SchedulerType;

/**
 * @enum QSS_StepType
 * @brief QSS simulation step type definition.
 *
 * @var QSS_StepType::ST_State
 * @brief
 *
 * @var QSS_StepType::ST_Event
 * @brief
 *
 * @var QSS_StepType::ST_Input
 * @brief
 *
 */
typedef enum { ST_State, ST_Event, ST_Input } QSS_StepType;

typedef struct QSS_LP_data_ *QSS_LP_data;

typedef struct QSS_LP_dataArray_ *QSS_LP_dataArray;

struct QSS_LP_dataArray_ {
  QSS_LP_data *lp;
  int size;
};

struct QSS_LP_data_ {
  int lpsCount;
  int nLPSCount;
  int states;
  int events;
  int inputs;
  int outputs;
  int outStates;  //!< Number of state variables that communicates with another
                  //! LPs.
  int inStates;
  int inEvents;
  int totalStates;
  int totalEvents;
  int totalOutputs;
  int dscInf;
  int id;
  double initDt;  //!< dt initial value.
  bool externalEvent;
  QSS_idxMap nLPS;
  QSS_idxMap lps;
  QSS_idxMap qMap;
  QSS_idxMap qInMap;
  QSS_idxMap qOutMap;
  QSS_idxMap eMap;
  QSS_idxMap eInMap;
  QSS_idxMap eOutMap;
  QSS_idxMap iMap;
  QSS_idxMap oMap;
  QSS_idxMap dscMap;
};

QSS_LP_data QSS_LP_Data(int states, int events, int inputs, int outputs, int inStates, int inEvents, int totalStates, int totalEvents,
                        int totalOutputs, int nLPS, int lps, int id);

QSS_LP_dataArray QSS_LP_DataArray(int size);

void QSS_LP_freeDataArray(QSS_LP_dataArray array);

QSS_LP_data QSS_LP_copyData(QSS_LP_data data);

void QSS_LP_clean(QSS_LP_data data);

void QSS_LP_freeData(QSS_LP_data data);

typedef struct QSS_event_ *QSS_event;

struct QSS_event_ {
  QSS_zc zeroCrossing;
  QSS_hnd handlerPos;
  QSS_hnd handlerNeg;
};

QSS_event SD_Event(QSS_zc zeroCrossing, QSS_hnd handlerPos, QSS_hnd handlerNeg);

void QSS_freeEvent(QSS_event events);

typedef struct QSS_data_ *QSS_data;

/**
 * `QSS_data_` struct contains all the information needed to run QSS simulations.
 *
 * @property {double} dQMin - dQMin is a pointer to an array of double values. It represents the
 * minimum change in the state variables for the QSS (Quantized State System) simulation.
 * @property {double} dQRel - dQRel is a pointer to an array of double values. It represents the
 * relative tolerance for the change in the state variables during the simulation.
 * @property {double} lqu - The property "lqu" is a pointer to a double. It represents the last quantum update value
 * which is computed from dQMin and dQRel.
 * @property {double} d - `d` is a pointer to an array of double values. It contains the values of the discrete variables
 * defined in the system.
 * @property {double} q - The `q` property is a pointer to an array of `double` values. It is used to
 * store the values of the quantized variables in the QSS (Quantized State System) simulation.
 * @property {double} x - The `x` property is a pointer to an array of `double` values. It is used to
 * store the state variables of the system being modeled.
 * @property {double} alg - The `alg` property is a pointer to an array of doubles. It is used to store
 * the values of algebraic variables in the model.
 * @property {double} jac - The `jac` property is a pointer to a double array. It contains the jacobian evaluation values.
 * @property {double} it - The variable "it" represents the initial simulation time.
 * @property {double} ft - The property "ft" represents the final simulation time.
 * @property {double} BDFMaxStep - BDFMaxStep is a variable that represents the maximum BDF
 * step size for the mixed LIQSS-BDF (Backward Differentiation Formula) solver.
 * @property {int} nSD - nSD represents the number of state derivatives
 * influenced by a state variable in the system.
 * @property {int} nDS - The property "nDS" represents the number of state variables that appears on the right hand side of
 * each derivate definition.
 * @property {int} nSZ - The property "nSZ" represents the number of
 * zero-crossing functions influenced by a state variable in the system.
 * @property {int} nZS - The property "nZS" represents the number of state variables that appears in the definition of each
 * zero-crossing function.
 * @property {int} nHD - The property "nHD" represents the number of derivatives influenced by discrete changes in a handler definition.
 * @property {int} nHZ - The property "nHZ" represents the number of zero-crossing functions affected by a handler execution.
 * @property {int} nSH - The property "nSH" represents the number of handlers that needs to be updated when a state variable change, 
 * needed for parallel simulations.
 * @property {int} nHH - The property "nHH" represents the number of handlers that needs to be updated due to a handler execution, 
 * needed for parallel simulations.
 * @property {int} nDH - The property "nDH" represents the number discrete variables that appears on each handler, needed
 * for parallel simulations.
 * @property {int} IT - IT represents if an equation defined in the model is influenced by changes in time, i.e. the 
 * equation is not autonomuous.
 * @property {int} BDF - in mixed LIQSS_BDF method, it represents the partition of the model that is computed using
 * BDF (Backward Differentiation Formula). It is a numerical method used
 * for solving ordinary differential equations (ODEs).
 * @property {int} BDFMap - The `BDFMap` property is used to
 * map the indices of the state variables in the system computed by BDF solver to their corresponding indices in the global state
 * variables definition.
 * @property {int} BDFOutputs - BDFOutputs represents the
 * indices of the variables that are outputs of the BDF (Backward Differentiation Formula) solver.
 * These variables are the ones whose values need to be communicated to the LIQSS solver.
 * @property {int} BDFInputs - BDFInputs represents the
 * indices of the variables that are used as inputs in the BDF (Backward Differentiation Formula) solver, these values
 * are computed by the LIQSS solver.
 * @property {int} JacIt - JacIt is an iterator that it's used to compute the Jacobian of the model in LIQSS-BDF simulations.
 * @property {bool} BDFInputsFirstStep - BDFInputsFirstStep is a boolean variable that indicates
 * whether the BDFInputs are being evaluated for the first step of the simulation.
 * @property {int} nBDFOutputVars - The property "nBDFOutputVars" represents the number of output
 * variables for the Backward Differentiation Formula (BDF) method .
 * @property {int} BDFOutputVars - BDFOutputVars is an array that stores the indices of the output variables 
 * for LIQSS-BDF method that are computed with BDF.
 * @property {int} QSSOutputs - QSSOutputs is a pointer to an array of integers. It represents the
 * indices of the output variables for LIQSS-BDF method that are computed with LIQSS.
 * @property {int} QSSOutputJac - QSSOutputJac represents the variables computed by LIQSS method in the mixed LIQSS-BDF
 * method that are needed to compute the Jacobian in the CV_ODE part.   
 * @property {int} QSSOutputJacId - QSSOutputJacId represents
 * the  vector entry on the original Jacobian vector for LIQSS-BDF method.
 * @property {int} SD - The SD property represents the state
 * derivatives sparse matrix, where each row corresponds to a state and each column corresponds to a
 * derivative.
 * @property {int} DS - DS represents the sparse dependency matrix
 * between derivatives and state variables. Each row of the matrix represents a derivative, and
 * each column represents a state. The value at DS[i][j] indicates whether the derivative i
 * depends on the state j.
 * @property {int} SZ - The `SZ` property is sparse matrix that contains the zero-crossing functions
 * affected by a state variable.
 * @property {int} ZS - ZS represents the sparse matrix of state variables that appear on each
 * zero-crossing function definition.
 * @property {int} HD - HD represents the matrix of derivatives affected by the exection of a handler.
 * @property {int} HZ - HZ represents the matrix of zero-crossing functions affected by the execution of the handler.
 * @property {int} SH - SH represents the matrix with the handlers that needs to be updated after a state variable changes.
 * @property {int} HH - HH represents the matrix of handlers that need to be updated after the execution of a handler.
 * @property {int} DH - DH represents the dependency between discrete variables and handlers. Each row of the array represents a
 * discrete variable, and each column represents a handler. The value at DH[i][j] indicates the
 * dependency of the i-th discrete variable and the j-th handler.
 * @property {int} states - The "states" property represents the number of state variables in the
 * system. These variables represent the dynamic behavior of the system and their values change over
 * time.
 * @property {int} discretes - The "discretes" property represents the number of discrete variables in
 * the system. Discrete variables are variables that change their values instantaneously at
 * specific points in time, rather than continuously changing over time.
 * @property {int} algs - The "algs" property represents the number of algebraic variables in the
 * system.
 * @property {int} events - The "events" property represents the number of events in the system.
 * @property {int} inputs - The "inputs" property represents the number of input variables in the
 * struct.
 * @property {int} order - The "order" property represents the order of integration methodbeing
 * used.
 * @property {int} maxRHS - The property "maxRHS" represents the maximum number of right-hand side
 * (RHS) state variables that appears in zero-crossing handlers.
 * @property {int} nBDF - The property "nBDF" represents the number of equations computed with 
 * Backward Differentiation Formula (BDF) solver in LIQSS_BDF method.
 * @property {int} nBDFOutputs - The property "nBDFOutputs" represents the number of outputs for the
 * Backward Differentiation Formula (BDF) method in the QSS_data_ struct.
 * @property {int} nBDFInputs - The property "nBDFInputs" represents the number of inputs for the
 * Backward Differentiation Formula (BDF) solver.
 * @property {int} BDFPartitionDepth - The property "BDFPartitionDepth" represents the depth of the
 * partitioning scheme used in the Backward Differentiation Formula (BDF) method. When using LIQSS-BDF 
 * mixed method and automatic partition generation selected, this propterty tells how deep to look for
 * state variables that changes in handler executions. Note that these variables are selected to use
 * LIQSS. 
 * @property {int} BDFPart - The property "BDFPart" is a boolean value that indicates if the BDF
 * partition for LIQSS-BDF method is present or not. 
 * @property {SD_Solver} solver - The `solver` property is of type `SD_Solver` and is used to store
 * information related to the solver used for solving the system of equations.
 * @property {SD_eventData} event - The "event" property is of type SD_eventData and represents the
 * event data for the QSS simulation. It contains information about the events that occur during the
 * simulation, such as the event times and event types.
 * @property {SD_parameters} params - The `params` property is of type `SD_parameters` and is used to
 * store various parameters related to the simulation.
 * @property {QSS_LP_data} lp - The "lp" property is of type QSS_LP_data and represents the data
 * related to the Logical Process in QSS Parallel Simulations.
 * @property {SD_jacMatrices} jac_matrices - The `jac_matrices` property is of type `SD_jacMatrices`
 * and is a struct that contains matrices used for Jacobian calculations in the QSS solver.
 */
struct QSS_data_ {
  double *dQMin;
  double *dQRel;
  double *lqu;
  double *d;
  double *q;
  double *x;
  double *alg;
  double *jac;
  double it;
  double ft;
  double BDFMaxStep;
  int *nSD;
  int *nDS;
  int *nSZ;
  int *nZS;
  int *nHD;
  int *nHZ;
  int *nSH;
  int *nHH;
  int *nDH;
  int *IT;
  int *BDF;
  int *BDFMap;
  int *BDFOutputs;
  int *BDFInputs;
  int *JacIt;
  bool *BDFInputsFirstStep;
  int nBDFOutputVars;
  int *BDFOutputVars;
  int *QSSOutputs;
  int *QSSOutputJac;
  int *QSSOutputJacId;
  int **SD;
  int **DS;
  int **SZ;
  int **ZS;
  int **HD;
  int **HZ;
  int **SH;
  int **HH;
  int **DH;
  int states;
  int discretes;
  int algs;
  int events;
  int inputs;
  int order;
  int maxRHS;
  int nBDF;
  int nBDFOutputs;
  int nBDFInputs;
  int BDFPartitionDepth;
  int BDFPart;
  SD_Solver solver;
  SD_eventData event;
  SD_parameters params;
  QSS_LP_data lp;
  SD_jacMatrices jac_matrices;
};


/**
 * `QSS_Data` initializes and returns a `QSS_data` struct with the specified parameters @see `QSS_data_`.
 * 
 * @param states The number of states in the system.
 * @param discretes The parameter "discretes" represents the number of discrete variables in the
 * system.
 * @param events The parameter "events" represents the number of events in the system.
 * @param inputs The parameter "inputs" represents the number of input variables in the system.
 * @param algs The parameter "algs" represents the number of algebraic variables in the system.
 * @param state_eqs The parameter "state_eqs" represents the number of state equations in the system.
 * @param alg_eqs The parameter "alg_eqs" represents the number of algebraic equations in the system.
 * @param name The name of the name of the model for logging.
 * 
 * @return An initialized QSS_data structure.
 */
QSS_data QSS_Data(int states, int discretes, int events, int inputs, int algs, int state_eqs, int alg_eqs, string name);

QSS_data QSS_copyData(QSS_data data);

void QSS_orderDataMatrix(QSS_data data);

void QSS_computeReinitAssign(QSS_data data);

void QSS_freeData(QSS_data data);

void QSS_cleanData(QSS_data data);

void QSS_allocDataMatrix(QSS_data data);

typedef struct QSS_time_ *QSS_time;

/**
 * @struct QSS_time_
 *
 * @brief Time management data structure for QSS simulations.
 *
 * This structure stores the next time of the state variables,
 * events and inputs defined in the model.
 *
 * @var QSS_time_::nextStateTime
 * @brief Array that contains the next change time of all the state variables
 * defined in the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref
 * QA_quantizer_ routines
 * @ref QA_recomputeNextTimes, @ref QA_recomputeNextTime and @ref QA_nextTime
 * with the corresponding
 * QSS method and after the array is updated the scheduler selects the next
 * simulation time.
 * @see SC_update
 *
 * @var QSS_time_::nextEventTime
 * @brief  Array that contains the next change time of all the events defined in
 * the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref
 * FRW_framework_ routine
 * @ref FRW_nextEventTime according to the order of the QSS method selected for
 * the simulation
 * and after the array is updated the scheduler selects the next simulation
 * time.
 * @see SC_update
 *
 * @var QSS_time_::nextInputTime
 * @brief  Array that contains the next change time of all the inputs defined in
 * the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref
 * FRW_framework_ routine
 * @ref FRW_nextInputTime according to the order of the QSS method selected for
 * the simulation
 * and after the array is updated the scheduler selects the next simulation
 * time.
 * @see SC_update
 *
 * @var QSS_time_::tx
 * @brief Array that contains the current time @f$ x(t) @f$ polynomial in the
 * simulation.
 *
 * @var QSS_time_::tq
 * @brief Array that contains the current time @f$ q(t) @f$ polynomial in the
 * simulation.
 *
 * @var QSS_time_::weights
 * @brief Array that contains the weights associated with each event defined in
 * the model.
 *
 * The @a weight of an event defines the probability of that event in case of a
 * tie.
 *
 */
struct QSS_time_ {
  double *nextStateTime;
  double *nextEventTime;
  double *nextInputTime;
  double *tx;
  double *tq;
  double *weights;
  double time;
  double previousTime;
  double minValue;
  int minIndex;
  int noReinit;
  QSS_SchedulerType scheduler;
  QSS_StepType type;
  QSS_reinit reinits;
};

QSS_time QSS_Time(int states, int events, int inputs, double it, QSS_SchedulerType scheduler, double *weights);

void QSS_freeTime(QSS_time simTime, int events, int inputs);

typedef struct QSS_model_ *QSS_model;

struct QSS_model_ {
  QSS_eq f;
  QSS_dep deps;
  QSS_event events;
  QSS_jac jac;
  QSS_fmodel F;
};

QSS_model QSS_Model(QSS_eq f, QSS_dep deps, QSS_zc zeroCrossing, QSS_hnd handlerPos, QSS_hnd handlerNeg, QSS_jac jac, QSS_fmodel);

void QSS_freeModel(QSS_model model);

typedef void (*QSS_setData)(QSS_data, QSS_time, SD_output, QSS_model, SD_simulationSettings);

bool QSS_influenced(QSS_data data, int variable, int inf, QSS_StepType type);
