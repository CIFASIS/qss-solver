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

#include <stdio.h>
#include <stdlib.h>

#include <gsl/gsl_multiroots.h>

#include "solve_loop.h"

#define SVL_MAX_ITER 100
#define SVL_TOL 1e-7

typedef struct {
  double *x;
  double *d;
  double *a;
  double *t;
  double *dx;
  int tearing_vars;
  double *guess;
  double *res;
  SVL_eval eval_tearing_vars;
} SVL_context;

// Evaluates the loop residuals for the candidate tearing values given by GSL.
// The generated callback assigns the guesses to the tearing variables, solves
// the iteration equations and computes the residuals.
static int SVL_SolveEval(const gsl_vector *tearing_values, void *params, gsl_vector *f)
{
  SVL_context *ctx = (SVL_context *)params;
  for (int i = 0; i < ctx->tearing_vars; i++) {
    ctx->guess[i] = gsl_vector_get(tearing_values, i);
  }
  ctx->eval_tearing_vars(ctx->x, ctx->d, ctx->a, ctx->t, ctx->dx, ctx->guess, ctx->res);
  for (int i = 0; i < ctx->tearing_vars; i++) {
    gsl_vector_set(f, i, ctx->res[i]);
  }
  return GSL_SUCCESS;
}

void SVL_SolveLoop(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_eval eval_tearing_vars)
{
  // Last solution found, used as the initial guess for the next call.
  static gsl_vector *solution = NULL;
  static gsl_vector *f = NULL;
  static double *guess = NULL;
  static double *res = NULL;

  if (solution == NULL) {
    solution = gsl_vector_calloc(tearing_vars);
    f = gsl_vector_alloc(tearing_vars);
    guess = (double *)malloc(sizeof(double) * tearing_vars);
    res = (double *)malloc(sizeof(double) * tearing_vars);
  }

  SVL_context ctx = {x, d, a, t, dx, tearing_vars, guess, res, eval_tearing_vars};
  gsl_multiroot_function F = {SVL_SolveEval, (size_t)tearing_vars, &ctx};

  // Try if we are already in the solution from the start (useful for discrete dependendt loops)
  SVL_SolveEval(solution, &ctx, f);
  if (gsl_multiroot_test_residual(f, SVL_TOL) == GSL_SUCCESS) {
    return;
  }

  gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrid, tearing_vars);
  gsl_multiroot_fsolver_set(s, &F, solution);

  size_t iter = 0;
  int status;
  do {
    iter++;
    status = gsl_multiroot_fsolver_iterate(s);
    if (status) { /* check if solver is stuck */
      break;
    }
    status = gsl_multiroot_test_residual(s->f, SVL_TOL);
  } while (status == GSL_CONTINUE && iter < SVL_MAX_ITER);

  if (status != GSL_SUCCESS) {
    printf("Warning: GSL could not solve an algebraic loop after %d iterations: %s\n", (int)iter, gsl_strerror(status));
  }

  gsl_vector_memcpy(solution, s->x);
  gsl_multiroot_fsolver_free(s);

  // The last evaluation done by the solver may correspond to a trial point,
  // evaluate again so the model variables are consistent with the solution.
  SVL_SolveEval(solution, &ctx, f);
}
