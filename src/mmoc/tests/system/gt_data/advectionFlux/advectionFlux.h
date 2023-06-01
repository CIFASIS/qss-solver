// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: F
#define _idx_F(d1,coeff) ((d1-1))
#define _state_idx_F(d1,coeff) ((d1-1))*3 + coeff
#define _F(d1,coeff) a[_state_idx_F(d1,coeff)]
#define _is_var_F(idx) idx >= 0 && idx < 101
#define _get_F_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: beta
#define _beta __PAR__beta

// Macros definition for variable: diff
#define _diff __PAR__diff

// Macros definition for variable: gam
#define _gam __PAR__gam

// Macros definition for variable: inVal
#define _inVal __PAR__inVal

// Macros definition for variable: outVal
#define _outVal __PAR__outVal

// Macros definition for variable: u
#define _idx_u(d1,coeff) ((d1-1))
#define _state_idx_u(d1,coeff) ((d1-1))*3 + coeff
#define _u(d1,coeff) x[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,coeff) x[_state_idx_u(d1,coeff)]
#define _q_u(d1,coeff) q[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_u(d1,coeff) ((d1-1))
#define _is_var_u(idx) idx >= 0 && idx < 100
#define _get_u_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_u(d1,coeff) dx[_state_idx_u(d1,coeff)]

// Macros definition for variable: v
#define _v __PAR__v

// Macros definition for variable: x
#define _idx_x(d1) ((d1-1))
#define _x(d1) __PAR__x[_idx_x(d1)]

// Macros definition for variable: x0
#define _x0 __PAR__x0


// Model Parameters Declaration

// Macro for parameter: L
double __PAR__L;
// Macro for parameter: beta
double __PAR__beta;
// Macro for parameter: diff
double __PAR__diff;
// Macro for parameter: gam
double __PAR__gam;
// Macro for parameter: inVal
double __PAR__inVal;
// Macro for parameter: outVal
double __PAR__outVal;
// Macro for parameter: v
double __PAR__v;
// Macro for parameter: x
double __PAR__x[100];
// Macro for parameter: x0
double __PAR__x0;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	j = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_u(_rg_d1,0);

// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2
#define _apply_usage_alg_eq_2(_d1) \
	j = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_F(_rg_d1,0);

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
// Derivative definition for variable: u
#define _der_u(d1,coeff) dx[coeff+1]
