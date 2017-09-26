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

#ifndef CLASSIC_DATA_H_
#define CLASSIC_DATA_H_

#include "../common/data.h"
#include "../common/utils.h"

/**
 *
 */
typedef void
(*CLC_fp)(void);
/**
 *
 * @param
 * @return
 */
typedef int
(*CLC_fpr)(int);
/**
 *
 * @param
 */
typedef void
(*CLC_fpa)(int);
/**
 *
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*CLC_eq)(double*, double*, double*, double, double*);
/**
 *
 * @param
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*CLC_zc)(int, double*, double*, double*, double, double*);
/**
 *
 * @param
 * @param
 * @param
 * @param
 */
typedef void
(*CLC_hnd)(int, double*, double*, double*, double);
/**
 *
 * @param
 * @param
 */
typedef void
(*CLC_input)(double, double*);
/**
 *
 */
typedef struct CLC_event_ *CLC_event;
/**
 *
 */
struct CLC_event_
{
    CLC_zc zeroCrossing; //!<
    CLC_hnd handlerPos; //!<
    CLC_hnd handlerNeg; //!<
};

typedef void
(*CLC_jac)(double *x, double *d, double *alg, double t, double *j);
/**
 *
 * @param zeroCrossing
 * @param handlerPos
 * @param handlerNeg
 * @return
 */
CLC_event
CLC_Event(CLC_zc zeroCrossing, CLC_hnd handlerPos, CLC_hnd handlerNeg);
/**
 *
 * @param events
 */
void
CLC_freeEvent(CLC_event events);
/**
 *
 */
typedef struct CLC_data_ *CLC_data;

/**
 *
 */
struct CLC_data_
{
    double *dQMin; //!<
    double *dQRel; //!<
    double *d; //!<
    double *x; //!<
    double *alg; //!<
    double it; //!<
    double ft; //!<
    int *nSD; //!<
    int *nDS; //!<
    int **SD; //!<
    int **DS; //!<
    int *IT; //!<
    int states; //!<
    int discretes; //!<
    int events; //!<
    int inputs; //!<
    char *fired;
    unsigned long scalarEvaluations; //!<
    unsigned long zeroCrossings; //!<
    unsigned long long funEvaluations; //!<
    unsigned long totalOutputSteps; //!<
    unsigned long totalJacobians; //!<
    unsigned long totalCrossingEvaluations; //!<
    unsigned long totalStepsDASSL; //!<
    unsigned long totalSteps; //!<
    unsigned long totalEvents; //!<
    SD_Solver solver; //!<
    SD_eventData event; //!<
    SD_parameters params; //!<
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
CLC_data
CLC_Data(int states, int discretes, int events, int inputs, int algebraics,
    string name);
/**
 *
 * @param data
 */
void
CLC_freeData(CLC_data data);
/**
 *
 * @param data
 */
void
CLC_allocDataMatrix(CLC_data data);
/**
 *
 */
typedef struct CLC_model_ *CLC_model;
/**
 *
 */
struct CLC_model_
{
    CLC_eq f; /**< Model definition \f$ \forall \imath \in [0,DIM] f(x_{i}(t),t)_{i} \f$ 		*/
    CLC_jac jac;
    CLC_event events; //!<
};
/**
 *
 * @param f
 * @param zeroCrossing
 * @param handlerPos
 * @param handlerNeg
 * @return
 */
CLC_model
CLC_Model(CLC_eq f, CLC_zc zeroCrossing, CLC_hnd handlerPos, CLC_hnd handlerNeg,
    CLC_jac);
/**
 *
 * @param model
 */
void
CLC_freeModel(CLC_model model);
/**
 *
 * @param
 * @param SD_output
 * @param
 * @param SD_simulationSettings
 */
typedef void
(*CLC_setData)(CLC_data, SD_output, CLC_model, SD_simulationSettings);

void
CLC_allocDataMatrix(CLC_data data);
#endif /* CLASSIC_DATA_H_ */
