/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHQSS_BDF ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#ifndef QSS_BDF_H_
#define QSS_BDF_H_

#include "../common/data.h"
#include "qss_data.h"

/**
 *
 */
typedef struct QSS_BDF_hybrid_ *QSS_BDF_hybrid;

/**
 *
 */
struct QSS_BDF_hybrid_
{
	int smaller;
	int bigger;
	int nBE;
	int infs;
  int *nSD;
	int *BES;
  int **SD;
  int **S;
	double *xprev;
	double hmin;
	double h;
  bool band;
  bool *change;
	bool *BE;
};

/**
 *
 * @param simData
 * @param simTime
 * @param simOutput
 * @return
 */
QSS_BDF_hybrid
QSS_BDF_Hybrid(QSS_data simData);

/**
 *
 * @param o
 */
void
QSS_BDF_freeHybrid(QSS_BDF_hybrid o);

/**
 *
 * @param hybrid
 * @param simData
 * @param simTime
 * @param simOutput
 */
void
QSS_BDF_partition(QSS_BDF_hybrid hybrid, QSS_data simData);

#endif  /* QSS_BDF_H_ */
