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

#include <gsl/gsl_multiroots.h>

#include "solve_loop.h"

// Eval and argument function callbacks.
SVL_eval _eval_callback;
double *_x = NULL;
double *_d = NULL;
double *_a = NULL;
double *_t = NULL;
double *_dx = NULL;
int _tearing_vars;
double *_tearing_var_values = NULL;

int SVL_SolveEval(const gsl_vector *__x, void *__p, gsl_vector *__f)
{
  _eval_callback(_x, _d, _a, _t, _dx, _tearing_var_values);
  for (int i = 0; i < _tearing_vars; i++) {
    gsl_vector_set(__f, i, _tearing_var_values[i]);
  }
  return GSL_SUCCESS;
}

void SVL_AssignCallback(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_eval eval_tearing_vars)
{
  if (_tearing_var_values == NULL) {
    _eval_callback = eval_tearing_vars;
    _x = x;
    _d = d;
    _a = a;
    _t = t;
    _dx = dx;
    _tearing_vars = tearing_vars;
    _tearing_var_values = (double *)malloc(sizeof(double) * _tearing_vars);
  }
}

void SVL_SolveLoop(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_iter assign_loop_vars,
                   SVL_eval eval_tearing_vars)
{
  size_t __iter = 0;
  int __status;

  SVL_AssignCallback(x, d, a, t, dx, tearing_vars, eval_tearing_vars);

  const gsl_multiroot_fsolver_type *__T = gsl_multiroot_fsolver_hybrid;
  gsl_multiroot_fsolver *__s = gsl_multiroot_fsolver_alloc(__T, tearing_vars);
  gsl_multiroot_function __F;
  static gsl_vector *__x = NULL;

  if (__x == NULL) {
    __x = gsl_vector_alloc(tearing_vars);
    for (int i = 0; i < tearing_vars; i++) {
      gsl_vector_set(__x, i, 0);
    }
  }
  __F.n = tearing_vars;
  __F.f = SVL_SolveEval;
  gsl_vector *__f = gsl_vector_alloc(tearing_vars);
  // Try if we are already in the solution from the start (useful for discrete dependendt loops)
  SVL_SolveEval(__x, NULL, __f);
  if (gsl_multiroot_test_residual(__f, 1e-7) == GSL_SUCCESS) {
    gsl_vector_free(__f);
    gsl_multiroot_fsolver_free(__s);
    return;
  }
  gsl_vector_free(__f);
  gsl_multiroot_fsolver_set(__s, &__F, __x);

  do {
    __iter++;
    __status = gsl_multiroot_fsolver_iterate(__s);
    if (__status) { /* check if solver is stuck */
      break;
    }
    __status = gsl_multiroot_test_residual(__s->f, 1e-7);
    assign_loop_vars(x, d, a, t, dx);
  } while (__status == GSL_CONTINUE && __iter < 100);

  if (__iter == 100) {
    printf("Warning: GSL could not solve an algebraic loop after %d iterations\n", (int)__iter);
  }

  for (int i = 0; i < tearing_vars; i++) {
    double ret = gsl_vector_get(__s->x, i);
    gsl_vector_set(__x, i, ret);
  }

  gsl_multiroot_fsolver_free(__s);
}
