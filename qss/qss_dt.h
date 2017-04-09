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

#ifndef QSS_DT_H_
#define QSS_DT_H_

#ifdef __linux__
#include <pthread.h>
#endif

#include <common/data.h>
#include <common/utils.h>
#include <qss/qss_data.h>

/**
 * @brief \f $ \delta t $ \f synchronization object.
 */
typedef struct QSS_dtSynch_ *QSS_dtSynch;

/**
 * @brief \f $ \delta t $ \f synchronization data structure.
 */
struct QSS_dtSynch_
{
  int synch; //!< Synchronization flag for \f $ \delta t $ \f update.
  int activeLPS;  //!< Number of active LPs in the simulation.
  double t;
  double elapsed;
#ifdef __linux__
  pthread_mutex_t activeMutex; //!< Active LPS mutex.
  pthread_barrier_t b; //!< \f $ \delta t $ \f synchronization barrier.
#endif
};

/**
 * @brief \f $ \delta t $ \f Synchronization constructor.
 *
 * @param lps Number of LPs defined in the simulation.
 * @return New \f $ \delta t $ \f synchronization object.
 */
QSS_dtSynch
QSS_DtSynch(int lps);

/**
 * @brief \f $ \delta t $ \f Synchronization destructor.
 *
 * @param dtSynch Object to destroy.
 */
void QSS_freeDtSynch(QSS_dtSynch dtSynch);

/**
 *
 */
typedef struct QSS_dtOps_ *QSS_dtOps;

/**
 *
 */
typedef struct QSS_dtState_ *QSS_dtState;

/**
 *
 */
typedef struct QSS_dt_ *QSS_dt;

/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef bool
(*QSS_dtLogOutputFn) (QSS_dt, double, double, double, int);

typedef bool
(*QSS_dtLogStepFn) (QSS_dt, double, double, double);

/**
 *
 */
struct QSS_dtOps_
{
  QSS_dtLogStepFn logStep; //!<
  QSS_dtLogOutputFn logOutput; //!<
};

/**
 *
 */
struct QSS_dtState_
{
  double *dtOpt; //!< Optimum dt calculated for each LP output variable.
  double *gblDtMin; //!< Global vector with the values of the average step time of all lps defiend.
  double dtMin; //!< Local dt minimum.
  double dt; //!< \f $\delta t $ \f current global value.
  double alpha;
  double dtUpperBound; //!< Upper bound for asynchronous \f $ \delta t $ \f update strategy.
  double dtLowerBound; //!< Lower bound for asynchronous \f $ \delta t $ \f update strategy.
  int *synch; //!< Synchronization flag for the \f $ \delta t $ \f update.
  int id; //!< LP number used to set the bit vector.
  int lps; //!< Total number of LPs defined.
  int outputs; //!< Number of output variables in the LP.
  int dtMinIndex; //!< Index of the output variable that contains the local minimum for the LP.
  int dtGlobalLP;
  int simSteps;
  int dtChanges;
  double avgDt;
  double lastChange;
  double *t; //!< Current simulation time.
  double *elapsed; //!< Elapsed time.
  double simTime; //!< Total simulation time.
  SD_DtSynch strategy;
  QSS_idxMap dscMap;
  QSS_idxMap qMap;
  SD_simulationLog log;
  SD_Debug debug;
  QSS_time time;
#ifdef __linux__
  pthread_barrier_t *b; //!< Barrier used for synchronization.
#endif
};

/**
 *
 */
struct QSS_dt_
{
  QSS_dtOps ops; //!<
  QSS_dtState state; //!<
};

/**
 *
 * @param synch
 * @param alpha
 * @param outputs
 * @param gblDtMin
 * @param id
 * @param dtSynch
 * @return
 */
QSS_dt
QSS_Dt (double *gblDtMin, int id,
	QSS_dtSynch dtSynch, char *file, SD_Debug debug,
	QSS_data data, QSS_time time);

/**
 *
 * @param dt
 */
void
QSS_freeDt (QSS_dt dt);

/**
 *
 * @return
 */
QSS_dtOps
QSS_DtOps ();

/**
 *
 * @param ops
 */
void
QSS_freeDtOps (QSS_dtOps ops);

/**
 *
 * @return
 */
QSS_dtState
QSS_DtState ();

/**
 *
 * @param state
 */
void
QSS_freeDtState (QSS_dtState state);

bool
QSS_dtLogOutput (QSS_dt dt, double Dq, double Dx, double Dt, int variable);

/**
 * @brief \f $ \delta t $ \f value wrapper.
 *
 * @param \f $ \delta t $ \f data structure.
 * @return \f $ \delta t $ \f value.
 */
double
QSS_dtValue (QSS_dt dt);

/**
 *
 * @param dt
 */
void
QSS_dtFinish (QSS_dt dt);

void
QSS_dtUpdate (QSS_dt dt);

/**
 *
 * @param dt
 * @return
 */
bool
QSS_dtLogStep (QSS_dt dt, double Dq, double Dx, double Dt);

void
QSS_dtCheck (QSS_dt dt);

#endif /* QSS_DT_H_ */
