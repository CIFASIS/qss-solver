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
 * @brief This header provides all the data structures used by the simulation engine.
 *
 * All the structures defined here, all visible from all the layers in the
 * system architecture.
 *
 * @file qss_data.h
 * @author Joaquin Fernandez
 * @version 3.1
 * @date 2010-2015
 * @copyright  GNU General Public License
 *
 */

#ifndef QSS_DATA_H_
#define QSS_DATA_H_

#include "../common/data.h"
#include "../common/utils.h"

#define QSS_REINIT_BUFFER 10000 //!<

/**
 *
 */
typedef int *QSS_idxMap;

/**
 *
 */
typedef void *
(QSS_sim) (void *);

/**
 *
 */
typedef struct QSS_reinit_ *QSS_reinit;

/**
 *
 */
struct QSS_reinit_
{
    int variable[QSS_REINIT_BUFFER]; //!<
    double time; //!<
    int counter; //!<
};

/**
 *
 * @return
 */
QSS_reinit
QSS_Reinit ();

/**
 *
 * @param reinit
 */
void
QSS_freeReinit (QSS_reinit reinit);

/**
 *
 */
typedef void
(*QSS_fp) (void);

/**
 *
 * @param
 * @return
 */
typedef int
(*QSS_fpr) (int);

/**
 *
 * @param
 */
typedef void
(*QSS_fpa) (int);

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QSS_eq) (int, double*, double*, double*, double, double*);

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QSS_dep) (int, double*, double*, double*, double, double*, int*);

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QSS_zc) (int, double*, double*, double*, double, double*);

/**
 *
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*QSS_hnd) (int, double*, double*, double*, double);

/**
 *
 * @param
 * @param
 */
typedef void
(*QSS_input) (double, double*);

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
typedef enum
{
    ST_Linear, ST_Binary, ST_Random
} QSS_SchedulerType;

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
typedef enum
{
    ST_State, ST_Event, ST_Input
} QSS_StepType;

/**
 *
 */
typedef struct QSS_LP_data_ *QSS_LP_data;

/**
 *
 */
typedef struct QSS_LP_dataArray_ *QSS_LP_dataArray;

/**
 *
 */
struct QSS_LP_dataArray_
{
    QSS_LP_data *lp;
    int size;
};

/**
 *
 */
struct QSS_LP_data_
{
    int lpsCount; //!<
    int nLPSCount; //!<
    int states; //!<
    int events; //!<
    int inputs; //!<
    int outputs; //!<
    int outStates; //!< Number of state variables that communicates with another LPs.
    int inStates; //!<
    int inEvents; //!<
    int totalStates; //!<
    int totalEvents; //!<
    int totalOutputs; //!<
    int dscInf;
    int id;
    double initDt; //!< dt initial value.
    bool externalEvent; //!<
    QSS_idxMap nLPS; //!<
    QSS_idxMap lps;  //!<
    QSS_idxMap qMap; //!<
    QSS_idxMap qInMap; //!<
    QSS_idxMap qOutMap; //!<
    QSS_idxMap eMap; //!<
    QSS_idxMap eInMap; //!<
    QSS_idxMap eOutMap; //!<
    QSS_idxMap iMap; //!<
    QSS_idxMap oMap; //!<
    QSS_idxMap dscMap;
};

/**
 *
 */
QSS_LP_data
QSS_LP_Data (int states, int events, int inputs, int outputs, int inStates, int inEvents, int totalStates, int totalEvents, int totalOutputs, int nLPS, int lps,
             int id);

/**
 *
 */
QSS_LP_dataArray
QSS_LP_DataArray (int size);

/**
 *
 * @param array
 */
void
QSS_LP_freeDataArray (QSS_LP_dataArray array);

/**
 *
 */
QSS_LP_data
QSS_LP_copyData (QSS_LP_data data);

/**
 *
 */
void
QSS_LP_clean (QSS_LP_data data);

/**
 *
 */
void
QSS_LP_freeData (QSS_LP_data data);

/**
 *
 */
typedef struct QSS_event_ *QSS_event;

/**
 *
 */
struct QSS_event_
{
    QSS_zc zeroCrossing; //!<
    QSS_hnd handlerPos; //!<
    QSS_hnd handlerNeg; //!<
};

/**
 *
 * @param zeroCrossing
 * @param handlerPos
 * @param handlerNeg
 * @return
 */
QSS_event
SD_Event (QSS_zc zeroCrossing, QSS_hnd handlerPos, QSS_hnd handlerNeg);

/**
 *
 * @param events
 */
void
QSS_freeEvent (QSS_event events);

/**
 *
 */
typedef struct QSS_data_ *QSS_data;

/**
 *
 */
struct QSS_data_
{
    double *dQMin; //!<
    double *dQRel;  //!<
    double *lqu; //!<
    double *d;  //!<
    double *q;  //!<
    double *x; //!<
    double *alg; //!<
    double it;  //!<
    double ft;  //!<
    int *nSD;  //!<
    int *nDS; //!<
    int *nSZ; //!<
    int *nZS; //!<
    int *nHD; //!<
    int *nHZ; //!<
    int *nSH;  //!<
    int *nDD; //!<
    int *nDH; //!<
    int *TD;  //!<
    int **SD; //!<
    int **DS; //!<
    int **SZ; //!<
    int **ZS; //!<
    int **HD; //!<
    int **HZ; //!<
    int **SH; //!<
    int **DD; //!<
    int **DH; //!<
    int states;  //!<
    int discretes;  //!<
    int algs; //!<
    int events;  //!<
    int inputs;  //!<
    int order;  //!<
    int maxRHS; //!<
    SD_Solver solver;  //!<
    SD_eventData event;  //!<
    SD_parameters params; //!<
    QSS_LP_data lp; //!<
};

/**
 *
 * @param states
 * @param discretes
 * @param events
 * @param inputs
 * @param name
 * @return
 */
QSS_data
QSS_Data (int states, int discretes, int events, int inputs, int algs, string name);

/**
 *
 * @return
 */
QSS_data
QSS_copyData (QSS_data data);

/**
 *
 * @param data
 */
void
QSS_orderDataMatrix (QSS_data data);

/**
 *
 * @param data
 */
void
QSS_freeData (QSS_data data);

void
QSS_cleanData (QSS_data data);

/**
 *
 * @param data
 */
void
QSS_allocDataMatrix (QSS_data data);

/**
 * 
 */
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
 * @brief Array that contains the next change time of all the state variables defined in the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref QA_quantizer_ routines
 * @ref QA_recomputeNextTimes, @ref QA_recomputeNextTime and @ref QA_nextTime with the corresponding
 * QSS method and after the array is updated the scheduler selects the next simulation time.
 * @see SC_update
 *
 * @var QSS_time_::nextEventTime
 * @brief  Array that contains the next change time of all the events defined in the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref FRW_framework_ routine
 * @ref FRW_nextEventTime according to the order of the QSS method selected for the simulation
 * and after the array is updated the scheduler selects the next simulation time.
 * @see SC_update
 *
 * @var QSS_time_::nextInputTime
 * @brief  Array that contains the next change time of all the inputs defined in the model.
 *
 * On each step taken by the simulator, this array is updated by the \ref FRW_framework_ routine
 * @ref FRW_nextInputTime according to the order of the QSS method selected for the simulation
 * and after the array is updated the scheduler selects the next simulation time.
 * @see SC_update
 *
 * @var QSS_time_::tx
 * @brief Array that contains the current time @f$ x(t) @f$ polynomial in the simulation.
 *
 * @var QSS_time_::tq
 * @brief Array that contains the current time @f$ q(t) @f$ polynomial in the simulation.
 *
 * @var QSS_time_::weights
 * @brief Array that contains the weights associated with each event defined in the model.
 *
 * The @a weight of an event defines the probability of that event in case of a tie.
 *
 */
struct QSS_time_
{
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

/**
 *
 * @param states
 * @param events
 * @param inputs
 * @param it
 * @param scheduler
 * @param weights
 * @return
 */
QSS_time
QSS_Time (int states, int events, int inputs, double it, QSS_SchedulerType scheduler, double *weights);

/**
 *
 * @param simTime
 * @param events
 * @param inputs
 */
void
QSS_freeTime (QSS_time simTime, int events, int inputs);

/**
 *
 */
typedef struct QSS_model_ *QSS_model;

/**
 *
 */
struct QSS_model_
{
    QSS_eq f; //!<
    QSS_dep deps; //!<
    QSS_event events; //!<
};

/**
 *
 * @param f
 * @param deps
 * @param zeroCrossing
 * @param handlerPos
 * @param handlerNeg
 * @return
 */
QSS_model
QSS_Model (QSS_eq f, QSS_dep deps, QSS_zc zeroCrossing, QSS_hnd handlerPos, QSS_hnd handlerNeg);

/**
 *
 * @param model
 */
void
QSS_freeModel (QSS_model model);

/*! \brief Callback function prototype, used to initialize data structures.	*/
/**
 *
 * @param
 * @param
 * @param SD_output
 * @param
 * @param SD_simulationSettings
 */
typedef void
(*QSS_setData) (QSS_data, QSS_time, SD_output, QSS_model, SD_simulationSettings);

/**
 *
 * @param simulator
 * @param variable
 * @param inf
 * @param type
 * @return
 */
bool
QSS_influenced (QSS_data data, int variable, int inf, QSS_StepType type);

#endif /* QSS_DATA_H_ */
