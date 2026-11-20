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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <gsl/gsl_math.h>
#include <gsl/gsl_multiroots.h>

#include "solve_loop.h"

#define SVL_MAX_ITER 100
#define SVL_TOL 1e-7
#define SVL_MAX_HYBRID_FAILURES 5

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

// Forward difference Jacobian of the loop residuals, used only by the Newton fallback. The step is
// relative to the tearing value but bounded from below: GSL's internal Jacobian uses a purely relative
// step, which on a tiny tearing value is lost in the residual and leads to a singular Jacobian.
static int SVL_SolveEvalJac(const gsl_vector *tearing_values, void *params, gsl_matrix *J)
{
  SVL_context *ctx = (SVL_context *)params;
  int n = ctx->tearing_vars;
  gsl_vector *x = gsl_vector_alloc(n);
  gsl_vector *f = gsl_vector_alloc(n);
  gsl_vector *f_h = gsl_vector_alloc(n);
  gsl_vector_memcpy(x, tearing_values);
  SVL_SolveEval(x, ctx, f);
  for (int j = 0; j < n; j++) {
    double x_j = gsl_vector_get(x, j);
    double h = GSL_SQRT_DBL_EPSILON * GSL_MAX_DBL(fabs(x_j), 1.0);
    gsl_vector_set(x, j, x_j + h);
    SVL_SolveEval(x, ctx, f_h);
    gsl_vector_set(x, j, x_j);
    for (int i = 0; i < n; i++) {
      gsl_matrix_set(J, i, j, (gsl_vector_get(f_h, i) - gsl_vector_get(f, i)) / h);
    }
  }
  gsl_vector_free(x);
  gsl_vector_free(f);
  gsl_vector_free(f_h);
  return GSL_SUCCESS;
}

static int SVL_SolveEvalFdf(const gsl_vector *tearing_values, void *params, gsl_vector *f, gsl_matrix *J)
{
  SVL_SolveEval(tearing_values, params, f);
  return SVL_SolveEvalJac(tearing_values, params, J);
}

// Copies the last iterate into solution, only if it is finite so that a diverging
// solver does not corrupt the initial guess used in the next call.
static int SVL_StoreSolution(const gsl_vector *last, gsl_vector *solution, int status)
{
  for (size_t i = 0; i < last->size; i++) {
    if (!isfinite(gsl_vector_get(last, i))) {
      return (status == GSL_SUCCESS) ? GSL_EBADFUNC : status;
    }
  }
  gsl_vector_memcpy(solution, last);
  return status;
}

// Default solver: hybrid method using the GSL internal finite difference Jacobian.
static int SVL_RunHybrid(gsl_multiroot_function *F, gsl_vector *solution, size_t *iter)
{
  gsl_multiroot_fsolver *s = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrid, F->n);
  gsl_multiroot_fsolver_set(s, F, solution);

  int status;
  *iter = 0;
  do {
    (*iter)++;
    status = gsl_multiroot_fsolver_iterate(s);
    if (status) { /* check if solver is stuck */
      break;
    }
    status = gsl_multiroot_test_residual(s->f, SVL_TOL);
  } while (status == GSL_CONTINUE && *iter < SVL_MAX_ITER);

  status = SVL_StoreSolution(s->x, solution, status);
  gsl_multiroot_fsolver_free(s);
  return status;
}

// Fallback solver: Newton with line search using SVL_SolveEvalJac. Newton has no trust region,
// so it is not limited by a tiny initial guess as the hybrid method is.
static int SVL_RunNewton(gsl_multiroot_function_fdf *F, gsl_vector *solution, size_t *iter)
{
  gsl_multiroot_fdfsolver *s = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_gnewton, F->n);
  gsl_multiroot_fdfsolver_set(s, F, solution);

  int status;
  *iter = 0;
  do {
    (*iter)++;
    status = gsl_multiroot_fdfsolver_iterate(s);
    if (status) { /* check if solver is stuck */
      break;
    }
    status = gsl_multiroot_test_residual(s->f, SVL_TOL);
  } while (status == GSL_CONTINUE && *iter < SVL_MAX_ITER);

  status = SVL_StoreSolution(s->x, solution, status);
  gsl_multiroot_fdfsolver_free(s);
  return status;
}

// Per loop solver state. Loops are identified by their residual evaluation function.
typedef struct {
  SVL_eval eval_tearing_vars;
  int tearing_vars;
  // Last solution found, used as the initial guess for the next call.
  gsl_vector *solution;
  gsl_vector *initial_guess;
  gsl_vector *f;
  double *guess;
  double *res;
  // Number of consecutive calls where the hybrid solver failed. Once it reaches
  // SVL_MAX_HYBRID_FAILURES, Newton is used directly for the rest of the simulation.
  int hybrid_failures;
  bool use_newton;
} SVL_loop;

static SVL_loop *SVL_GetLoop(SVL_eval eval_tearing_vars, int tearing_vars)
{
  static SVL_loop *loops = NULL;
  static int n_loops = 0;

  for (int i = 0; i < n_loops; i++) {
    if (loops[i].eval_tearing_vars == eval_tearing_vars) {
      return &loops[i];
    }
  }

  loops = (SVL_loop *)realloc(loops, sizeof(SVL_loop) * (n_loops + 1));
  SVL_loop *loop = &loops[n_loops++];
  loop->eval_tearing_vars = eval_tearing_vars;
  loop->tearing_vars = tearing_vars;
  loop->solution = gsl_vector_calloc(tearing_vars);
  loop->initial_guess = gsl_vector_alloc(tearing_vars);
  loop->f = gsl_vector_alloc(tearing_vars);
  loop->guess = (double *)malloc(sizeof(double) * tearing_vars);
  loop->res = (double *)malloc(sizeof(double) * tearing_vars);
  loop->hybrid_failures = 0;
  loop->use_newton = FALSE;
  return loop;
}

void SVL_SolveLoop(double *x, double *d, double *a, double *t, double *dx, int tearing_vars, SVL_eval eval_tearing_vars)
{
  SVL_loop *loop = SVL_GetLoop(eval_tearing_vars, tearing_vars);
  gsl_vector *solution = loop->solution;

  SVL_context ctx = {x, d, a, t, dx, tearing_vars, loop->guess, loop->res, eval_tearing_vars};
  gsl_multiroot_function F = {SVL_SolveEval, (size_t)tearing_vars, &ctx};

  // Try if we are already in the solution from the start (useful for discrete dependendt loops)
  SVL_SolveEval(solution, &ctx, loop->f);
  if (gsl_multiroot_test_residual(loop->f, SVL_TOL) == GSL_SUCCESS) {
    return;
  }

  gsl_multiroot_function_fdf F_fdf = {SVL_SolveEval, SVL_SolveEvalJac, SVL_SolveEvalFdf, (size_t)tearing_vars, &ctx};
  size_t iter = 0;
  int status;

  if (loop->use_newton) {
    status = SVL_RunNewton(&F_fdf, solution, &iter);
  } else {
    gsl_vector_memcpy(loop->initial_guess, solution);
    status = SVL_RunHybrid(&F, solution, &iter);

    // The hybrid solver sizes its initial trust region relative to the initial guess, so when the
    // previous solution is tiny (but not zero) it can not take steps large enough to reach the new
    // solution. In that case retry from the same initial guess with Newton.
    if (status == GSL_SUCCESS) {
      loop->hybrid_failures = 0;
    } else {
      loop->use_newton = (++loop->hybrid_failures >= SVL_MAX_HYBRID_FAILURES);
      size_t newton_iter = 0;
      gsl_vector_memcpy(solution, loop->initial_guess);
      status = SVL_RunNewton(&F_fdf, solution, &newton_iter);
      iter += newton_iter;
    }
  }

  if (status != GSL_SUCCESS) {
    printf("Warning: GSL could not solve an algebraic loop after %d iterations: %s\n", (int)iter, gsl_strerror(status));
  }

  // The last evaluation done by the solver may correspond to a trial point,
  // evaluate again so the model variables are consistent with the solution.
  SVL_SolveEval(solution, &ctx, loop->f);
}
