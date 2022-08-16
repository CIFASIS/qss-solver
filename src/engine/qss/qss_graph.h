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

#include <stdio.h>

#include <qss/qss_data.h>

typedef long int grp_t;

typedef enum { GRP_Success = 0x00, GRP_ReadError = 0x01, GRP_WrongFile = 0x02, GRP_WriteError = 0x03 } GRP_Status;

GRP_Status GRP_readGraph(char *name, QSS_data data, grp_t **xadj, grp_t **adjncy, grp_t *edges, grp_t **vwgt, grp_t **ewgt, int rhe,
                         grp_t **hevars);

GRP_Status GRP_createGraph(char *name, QSS_data data, grp_t **xadj, grp_t **adjncy, grp_t *edges, grp_t **vwgt, grp_t **ewgt, int rhe,
                           grp_t **hevars);
