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

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "data.h"

/**
 *
 */
typedef struct SET_settings_ *SET_settings;

/**
 *
 */
struct SET_settings_
{
    double minstep; //!<
    double zchyst; //!<
    double derdelta; //!<
    double it; //!<
    double ft; //!<
    double dt; //!<
    double *dqmin; //!<
    double *dqrel; //!<
    int symdiff; //!<
    int lps; //!<
    int nodesize; //!<
    int order; //!<
    SD_Solver solver; //!<
    int nDQMin; //!<
    int nDQRel; //!<
    int jacobian;
    SD_PartitionMethod pm; //!<
    SD_DtSynch dtSynch; //!< \f $ \delta t $ \f synchronization policy.
    SD_partitionerOptions partitionerOptions;
};
/**
 *
 * @param fname
 * @return
 */
SET_settings
SET_Settings(char *fname);
/**
 *
 * @param settings
 */
void
freeSettings(SET_settings settings);

#endif  /* SETTINGS_H_ */
