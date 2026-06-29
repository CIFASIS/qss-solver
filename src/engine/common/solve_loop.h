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

#ifndef SOLVE_LOOP_H_
#define SOLVE_LOOP_H_

#include "data.h"

typedef void (*SVL_iter)(double *, double *, double *, double *, double *);
typedef void (*SVL_eval)(double *, double *, double *, double *, double *, double *);

/**
 *
 */
void SVL_solve_loop(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_iter assign_loop_vars,
                    SVL_eval eval_tearing_vars);

#endif /* SOLVE_LOOP_H_ */
