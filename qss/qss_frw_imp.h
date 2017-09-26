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

#ifndef QSS_FRW_IMP_H_
#define QSS_FRW_IMP_H_

#include "qss_data.h"
#include "qss_frw.h"

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_PAR_recomputeDerivatives(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_PAR_recomputeDerivatives(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_PAR_recomputeDerivatives(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_PAR_recomputeDerivatives(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_recomputeDerivatives(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_PAR_recomputeDerivative(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_PAR_recomputeDerivative(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_PAR_recomputeDerivative(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_recomputeDerivative(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_PAR_recomputeDerivative(FRW_framework f, QSS_model simModel,
    QSS_data simData, QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SYM_PAR_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
FO_PAR_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
SO_PAR_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param index
 */
void
TO_PAR_nextEventTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
FO_PAR_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
FO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
SO_PAR_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
SO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
TO_PAR_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

/**
 *
 * @param f
 * @param simModel
 * @param simData
 * @param simTime
 * @param elapsed
 * @param var
 * @param index
 */
void
TO_nextInputTime(FRW_framework f, QSS_model simModel, QSS_data simData,
    QSS_time simTime, double elapsed, int var, int index);

#endif /* QSS_FRW_IMP_H_ */
