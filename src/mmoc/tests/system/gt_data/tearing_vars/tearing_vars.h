
// Fordward declare tearing functions. 
void MOD_EvalTearingVars(double *x, double *d, double *a, double *t, double *dx, const double *guess, double *ret);
void MOD_AssignLoop(double *x, double *d, double *a, double *t, double *dx);

// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Model Variables Macros

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: a
#define _idx_a 0
#define _a a[_idx_a]

// Macros definition for variable: tearing_var_guess_1
#define _tearing_var_guess_1 __PAR__tearing_var_guess_1

// Macros definition for variable: tearing_var_res_1
#define _idx_tearing_var_res_1 1
#define _tearing_var_res_1 a[_idx_tearing_var_res_1]

// Macros definition for variable: x
#define _idx_x 0
#define _x x[_idx_x]
#define _init_x x[_idx_x]
#define _eval_x 0


// Model Parameters Declaration

// Macro for parameter: tearing_var_guess_1
int __PAR__tearing_var_guess_1;

// Derivative Equations Macros

// Macros for equation: 1


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2

// Macros for algebraic equation: 3


// Output Equations Macros

// Macros for output equation: 1

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: x
#define _der_x dx[0]
