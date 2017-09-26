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

#ifndef HMETIS_H_
#define HMETIS_H_

/* Wraper header for khmetis library */

void
HMETIS_PartRecursive(int nvtxs, int nhedges, int *vwgts, int *eptr, int *eind,
    int *hewgts, int nparts, int ubfactor, int *options, int *part,
    int *edgecut);

void
HMETIS_PartKway(int nvtxs, int nhedges, int *vwgts, int *eptr, int *eind,
    int *hewgts, int nparts, int ubfactor, int *options, int *part,
    int *edgecut);

#endif /* HMETIS_H_ */
