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

// Macros definition for variable: C
#define _C __PAR__C

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: R
#define _R __PAR__R

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 10
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 10
#define _eval_out_exp_2 10

// Macros definition for variable: a1
#define _idx_a1(d1,coeff) ((d1-1))
#define _state_idx_a1(d1,coeff) ((d1-1))*3 + coeff
#define _a1(d1,coeff) a[_state_idx_a1(d1,coeff)]
#define _is_var_a1(idx) idx >= 0 && idx < 10
#define _get_a1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: x1
#define _idx_x1(d1,coeff) ((d1-1))
#define _state_idx_x1(d1,coeff) ((d1-1))*3 + coeff
#define _x1(d1,coeff) x[_state_idx_x1(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x1(d1,coeff) x[_state_idx_x1(d1,coeff)]
#define _q_x1(d1,coeff) q[_state_idx_x1(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_x1(d1,coeff) ((d1-1))
#define _is_var_x1(idx) idx >= 0 && idx < 10
#define _get_x1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_x1(d1,coeff) dx[_state_idx_x1(d1,coeff)]

// Macros definition for variable: x2
#define _idx_x2(d1,coeff) (10+0)
#define _state_idx_x2(d1,coeff) (10+0)*3 + coeff
#define _x2(d1,coeff) x[_state_idx_x2(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x2(d1,coeff) x[_state_idx_x2(d1,coeff)]
#define _q_x2(d1,coeff) q[_state_idx_x2(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_x2(d1,coeff) (10+(d1-1))
#define _is_var_x2(idx) idx >= 10 && idx < 11
#define _get_x2_idxs(idx)\
		_d1 = (idx-10)+ 1;
#define _eval_dep_x2(d1,coeff) dx[_state_idx_x2(d1,coeff)]


// Model Parameters Declaration

// Macro for parameter: C
double __PAR__C;
// Macro for parameter: L
double __PAR__L;
// Macro for parameter: R
double __PAR__R;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_x1(_rg_d1,0);
// Macros for equation: 2

// Macros for equation: 3


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_a1(_rg_d1,0);

// Output Equations Macros

// Macros for output equation: 1
#define _apply_usage_out_exp_1(_d1) \
	_d1 = _d1;
#define _get_out_exp_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_out_exp_1(_rg_d1);
// Macros for output equation: 2

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: x1
#define _der_x1(d1,coeff) dx[coeff+1]
// Derivative definition for variable: x2
#define _der_x2(d1,coeff) dx[coeff+1]
