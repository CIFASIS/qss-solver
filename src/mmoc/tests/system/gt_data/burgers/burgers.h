// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: _event_1
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 1000
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 5
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: ap
#define _idx_ap(d1) ((d1-1))
#define _ap(d1) d[_idx_ap(d1)]

// Macros definition for variable: beta
#define _beta __PAR__beta

// Macros definition for variable: inVal
#define _inVal __PAR__inVal

// Macros definition for variable: outVal
#define _outVal __PAR__outVal

// Macros definition for variable: pi
#define _pi __PAR__pi

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

// Macros definition for variable: x
#define _idx_x(d1) ((d1-1))
#define _x(d1) __PAR__x[_idx_x(d1)]

// Macros definition for variable: x0
#define _x0 __PAR__x0


// Model Parameters Declaration

// Macro for parameter: beta
double __PAR__beta;
// Macro for parameter: inVal
double __PAR__inVal;
// Macro for parameter: outVal
double __PAR__outVal;
// Macro for parameter: pi
double __PAR__pi;
// Macro for parameter: x
double __PAR__x[1000];
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
// Macros for equation: 3


// Event Macros

// Macros for event: 1
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _get_event_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_1(_rg_d1);
#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1
#define _apply_usage_out_exp_1(_d1) \
	_d1 = _d1;
#define _get_out_exp_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_out_exp_1(_rg_d1);
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
