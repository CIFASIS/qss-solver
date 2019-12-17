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

#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>

#include "utils.h"

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void (*SD_eq)(int, double *, double *, double *, double, double *);

/**
 *
 */
#define LPS 0

/**
 *
 */
#define ALL -1

/**
 *
 */
#define MAX_NAME_SIZE 256

/**
 *
 */
#define DEFAULT_PERIOD 1e-2

/**
 *
 */
#define DER_DELTA 1e-8

/**
 *
 */
#define ZC_HYST 1e-12

/**
 *
 */
#define MIN_STEP 1e-14

/**
 * @brief DELTA definition.
 */
#define DELTA 1e-3

#define ROOT_SIMULATOR -1

/**
 *
 */
typedef enum {
  SD_File,   //!< SD_File
  SD_Memory  //!< SD_Memory
} SD_StoreData;

/**
 *
 */
typedef enum {
  CI_Step,    //!< CI_Step
  CI_Dense,   //!< CI_Dense
  CI_Sampled  //!< CI_Sampled
} SD_CommInterval;

/**
 *
 */
typedef enum {
  SD_QSS,        //!< SD_QSS
  SD_CQSS,       //!< SD_CQSS
  SD_LIQSS,      //!< SD_LIQSS
  SD_QSS2,       //!< SD_QSS2
  SD_LIQSS2,     //!< SD_LIQSS2
  SD_LIQSS_BDF,  //!< SD_LIQSS2
  SD_QSS3,       //!< SD_QSS3
  SD_LIQSS3,     //!< SD_LIQSS3
  SD_QSS4,       //!< SD_QSS4
  SD_DASSL,      //!< SD_DASSL
  SD_DOPRI,      //!< SD_DOPRI
  SD_CVODE_BDF,  //!< SD_CVODE_BDF
  SD_CVODE_AM,   //!< SD_CVODE_
  SD_IDA         //!< SD_IDA
} SD_Solver;

/**
 *
 */
typedef enum {
  SD_DBG_NoDebug = 0,             //!< SD_DBG_NoDebug
  SD_DBG_VarChanges = 1 << 0,     //!< SD_DBG_VarSteps
  SD_DBG_InitValues = 1 << 1,     //!< SD_DBG_InitValues
  SD_DBG_StepInfo = 1 << 2,       //!< SD_DBG_StepInfo
  SD_DBG_Weights = 1 << 3,        //!< SD_DBG_Weights
  SD_DBG_Memory = 1 << 4,         //!< SD_DBG_Memory
  SD_DBG_ExternalEvent = 1 << 5,  //!< SD_DBG_ExternalEvent
  SD_DBG_Synchronize = 1 << 6,    //!< SD_DBG_Synchronize
  SD_DBG_WaitFor = 1 << 7,        //!< SD_DBG_WaitFor
  SD_DBG_Dt = 1 << 8              //!< SD_DBG_Dt
} SD_Debug;

/**
 *
 */
typedef enum {
  SD_Metis,   //!< SD_Metis
  SD_HMetis,  //!< SD_HMetis
  SD_Scotch,  //!< SD_Scotch
  SD_Patoh,   //!< SD_Patoh
  SD_Manual
} SD_PartitionMethod;

/**
 *
 */
typedef enum {
  SD_DT_Fixed,     //!<
  SD_DT_Adaptive,  //!<
  SD_DT_AdaptiveDiscrete
} SD_DtSynch;

typedef struct {
  char lvalue[128];
  char value[128];
} SD_partitionerOption;

typedef struct {
  SD_partitionerOption patoh[50];
  SD_partitionerOption scotch[50];
  SD_partitionerOption metis[50];
  int nPatoh;
  int nScotch;
  int nMetis;
} SD_partitionerOptions;

/**
 *
 */
typedef struct SD_simulationSettings_ *SD_simulationSettings;

/**
 *
 */
struct SD_simulationSettings_ {
  SD_Debug debug;    //!<
  SD_Solver method;  //!<
  bool parallel;     //!<
  bool hybrid;       //!<
  int jacobian;
};

/**
 *
 * @return
 */
SD_simulationSettings SD_SimulationSettings();

/**
 *
 * @return
 */
SD_simulationSettings SD_copySimulationSettings(SD_simulationSettings settings);

/**
 *
 * @param settings
 */
void SD_freeSimulationSettings(SD_simulationSettings settings);

/**
 *
 */
typedef struct SD_simulationLog_ *SD_simulationLog;

/**
 *
 */
struct SD_simulationLog_ {
  int *states;        //!<
  int *handlers;      //!<
  bool hasVariables;  //!<
  FILE *log;          //!<
};

/**
 *
 * @param states
 * @param events
 * @return
 */
SD_simulationLog SD_SimulationLog(char *name);

/**
 *
 * @param log
 * @param states
 * @param events
 */
void SD_setSimulationLogVariables(SD_simulationLog log, int states, int events);

/**
 *
 * @param log
 * @param orig
 */
void SD_copySimulationLogVariables(SD_simulationLog log, SD_simulationLog orig);

/**
 *
 * @param log
 * @param message
 */
void SD_print(SD_simulationLog log, const char *message, ...);

/**
 *
 * @param log
 */
void SD_freeSimulationLog(SD_simulationLog log);

/**
 *
 * @param algebraics
 * @param order
 * @return
 */
/**
 *
 */
typedef struct SD_eventData_ *SD_eventData;

/**
 *
 */
struct SD_eventData_ {
  int nLHSSt;     //!<
  int nLHSDsc;    //!<
  int nRHSSt;     //!<
  int zcSign;     //!<
  int direction;  //!<
  int relation;   //!<
  int *LHSSt;     //!<
  int *LHSDsc;    //!<
  int *RHSSt;     //!<
  double zcHyst;  //!<
};

/**
 *
 * @param size
 * @return
 */
SD_eventData SD_EventData(int size);

/**
 *
 * @param size
 * @param events
 */
SD_eventData SD_copyEventData(int size, SD_eventData events);

/**
 *
 * @param events
 * @param size
 */
void SD_freeEventData(SD_eventData events, int size);

void SD_cleanEventData(SD_eventData events, int size);

/**
 *
 */
typedef struct SD_parameters_ *SD_parameters;

/**
 *
 */
struct SD_parameters_ {
  double derDelta;  //!<
  double zcHyst;    //!<
  double minStep;   //!<
  double dt;        //!< For parallel simulations, minimum \f $ \delta t $\f value allowed.
  int symDiff;      //!< Use symbolic derivatives.
  int lps;          //!< Number of LPs defined for parallel simulations.
  int nodeSize;     //!< Node size used in the memory list for output simulation values.
  int jacobian;
  SD_PartitionMethod pm;  //!< Partition method used to obtain a model partition for parallel simulations.
  SD_DtSynch dtSynch;     //!< \f $ \delta t $ \f synchronization policy.
  SD_partitionerOptions partitionerOptions;
};

/**
 *
 * @param derDelta
 * @param zcHyst
 * @param minStep
 * @param symDiff
 * @return
 */
SD_parameters SD_Parameters(double derDelta, double zcHyst, double minStep, int symDiff, int lps, int nodeSize, SD_PartitionMethod pm,
                            double dt, SD_DtSynch synch, SD_partitionerOptions partitionerOptions, int jacobian);

/**
 *
 * @param parameters
 * @return
 */
SD_parameters SD_copyParameters(SD_parameters parameters);

/**
 *
 * @param params
 */
void SD_freeParameters(SD_parameters params);

/**
 *
 */
typedef struct SD_data_ *SD_data;

/**
 *
 */
typedef struct SD_sampledOutput_ *SD_sampledOutput;

/**
 *
 */
struct SD_sampledOutput_ {
  double *nextTime;  //!<
  double *period;    //!<
};

/**
 *
 * @param outputs
 * @param it
 * @param period
 * @param nperiod
 * @return
 */
SD_sampledOutput SD_SampledOutput(int outputs, double it, double *period, int nperiod);

/**
 *
 * @param output
 */
void SD_freeSampledOutput(SD_sampledOutput output);

/**
 *
 */
typedef struct SD_outputVariable_ *SD_outputVariable;

/**
 *
 */
struct SD_outputVariable_ {
  char name[MAX_NAME_SIZE];  //!<
};

/**
 *
 * @param outputs
 * @return
 */
SD_outputVariable SD_OutputVariable(int outputs);

/**
 *
 * @param variable
 */
void SD_freeOutputVariable(SD_outputVariable variable);

/**
 *
 */
typedef struct SD_output_ *SD_output;

/**
 *
 */
struct SD_output_ {
  char *name;                    //!<
  int outputs;                   //!<
  int *nDO;                      //!<
  int *nOD;                      //!<
  int *nSO;                      //!<
  int *nOS;                      //!<
  int **DO;                      //!<
  int **OD;                      //!<
  int **SO;                      //!<
  int **OS;                      //!<
  SD_CommInterval commInterval;  //!<
  SD_StoreData store;            //!<
  SD_sampledOutput sampled;      //!<
  SD_outputVariable variable;    //!<
  SD_eq value;                   //!<
};

/**
 *
 * @param name
 * @param outputs
 * @param discretes
 * @param states
 * @param period
 * @param nPeriod
 * @param it
 * @param commInterval
 * @param store
 * @param value
 * @return
 */
SD_output SD_Output(string name, int outputs, int discretes, int states, double *period, int nPeriod, double it,
                    SD_CommInterval commInterval, SD_StoreData store, SD_eq value);

/**
 *
 * @param output
 * @param states
 * @param discretes
 */
void SD_allocOutputMatrix(SD_output output, int states, int discretes);

/**
 *
 * @param output
 * @param states
 * @param discretes
 */
void SD_freeOutput(SD_output output, int states, int discretes);

/**
 *
 */
typedef struct SD_statistics_ *SD_statistics;

/**
 *
 */
struct SD_statistics_ {
  double *simulationTimes;
  double partitioningTime;
  double initializeLPS;
  int *steps;
  unsigned long *simulationMessages;
  unsigned long *simulationExternalEvents;
  unsigned long modelEvaluations;
  unsigned long totalSteps;
  unsigned long reinits;
  unsigned long memory;
  double initTime;
  double simulationTime;
  double saveTime;
  double sequentialMemory;
  unsigned long messages;
  unsigned long extTrans;
  unsigned int pastEvents;
#ifdef _WIN32
  struct timeval *iTime;
  struct timeval *sTime;
  struct timeval *sdTime;
#else
  struct timespec *iTime;
  struct timespec *sTime;
  struct timespec *sdTime;
#endif
};

/**
 *
 * @param lps
 * @return
 */
SD_statistics SD_Statistics();

/**
 *
 * @param stats
 */
void SD_freeStatistics(SD_statistics stats);

void SD_setStatisticsLPS(SD_statistics stats, int lps);

#endif /* DATA_H_ */
