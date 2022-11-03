// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: a
#define _a __PAR__a

// Macros definition for variable: d
#define _idx_d(d1) ((d1-1))
#define _d(d1) __PAR__d[_idx_d(d1)]

// Macros definition for variable: dx
#define _dx __PAR__dx

// Macros definition for variable: dx2
#define _dx2 __PAR__dx2

// Macros definition for variable: r
#define _r __PAR__r

// Macros definition for variable: u
#define _idx_u(d1,coeff) ((d1-1))
#define _state_idx_u(d1,coeff) ((d1-1))*3 + coeff
#define _u(d1,coeff) x[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,coeff) x[_state_idx_u(d1,coeff)]
#define _q_u(d1,coeff) q[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_u(d1,coeff) ((d1-1))
#define _is_var_u(idx) idx >= 0 && idx < 1000
#define _get_u_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_u(d1,coeff) dx[_state_idx_u(d1,coeff)]


// Model Parameters Declaration

// Macro for parameter: L
double __PAR__L;
// Macro for parameter: a
double __PAR__a;
// Macro for parameter: d
double __PAR__d[1000];
// Macro for parameter: dx
double __PAR__dx;
// Macro for parameter: dx2
double __PAR__dx2;
// Macro for parameter: r
double __PAR__r;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3
#define _apply_usage_eq_3(_d1) \
	i = _d1;
#define _get_eq_3_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_u(_rg_d1,0);

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
