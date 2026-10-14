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

/**
 * Evaluates the residuals of an algebraic loop: assigns the tearing variables
 * from the guess vector (second to last argument), solves the iteration
 * equations and stores the residuals in the last argument.
 */
typedef void (*SVL_eval)(double *, double *, double *, double *, double *, const double *, double *);

/**
 * Solves an algebraic loop with tearing_vars tearing variables using GSL.
 */
void SVL_SolveLoop(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_eval eval_tearing_vars);

#endif /* SOLVE_LOOP_H_ */
