// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1

// Model Variables Macros

// Macros definition for variable: I
#define _idx_I(coeff) 0
#define _state_idx_I(coeff) 0*2 + coeff
#define _I(coeff) x[_state_idx_I(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_I(coeff) x[_state_idx_I(coeff)]
#define _q_I(coeff) q[_state_idx_I(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_I(coeff) 0
#define _eval_dep_I(coeff) dx[_state_idx_I(coeff)]

// Macros definition for variable: TEvent
#define _idx_TEvent(d1) ((d1-1))
#define _TEvent(d1) __PAR__TEvent[_idx_TEvent(d1)]

// Macros definition for variable: W
#define _idx_W(d1) ((d1-1))
#define _W(d1) __PAR__W[_idx_W(d1)]

// Macros definition for variable: X
#define _idx_X(d1,coeff) (1+(d1-1))
#define _state_idx_X(d1,coeff) (1+(d1-1))*2 + coeff
#define _X(d1,coeff) x[_state_idx_X(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_X(d1,coeff) x[_state_idx_X(d1,coeff)]
#define _q_X(d1,coeff) q[_state_idx_X(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_X(d1,coeff) (1+(d1-1))
#define _is_var_X(idx) idx >= 1 && idx < 11
#define _get_X_idxs(idx)\
		_d1 = (idx-1)+ 1;
#define _eval_dep_X(d1,coeff) dx[_state_idx_X(d1,coeff)]

// Macros definition for variable: _event_1
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 10000
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 10
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: dW
#define _idx_dW 0
#define _dW d[_idx_dW]

// Macros definition for variable: dWval
#define _idx_dWval(d1) ((d1-1))
#define _dWval(d1) __PAR__dWval[_idx_dWval(d1)]

// Macros definition for variable: di
#define _di __PAR__di

// Macros definition for variable: h
#define _h __PAR__h

// Macros definition for variable: hmin
#define _hmin __PAR__hmin

// Macros definition for variable: k
#define _k __PAR__k

// Macros definition for variable: mu
#define _mu __PAR__mu

// Macros definition for variable: omega
#define _omega __PAR__omega

// Macros definition for variable: sigma
#define _sigma __PAR__sigma

// Macros definition for variable: tf
#define _tf __PAR__tf


// Model Parameters Declaration

// Macro for parameter: TEvent
double __PAR__TEvent[10000];
// Macro for parameter: W
double __PAR__W[10000001];
// Macro for parameter: dWval
double __PAR__dWval[10000];
// Macro for parameter: di
int __PAR__di;
// Macro for parameter: h
double __PAR__h;
// Macro for parameter: hmin
double __PAR__hmin;
// Macro for parameter: k
double __PAR__k;
// Macro for parameter: mu
double __PAR__mu;
// Macro for parameter: omega
double __PAR__omega;
// Macro for parameter: sigma
double __PAR__sigma;
// Macro for parameter: tf
double __PAR__tf;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3
#define _apply_usage_eq_3(_d1) \
	i = _d1;
#define _get_eq_3_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_X(_rg_d1,0);

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
// Derivative definition for variable: I
#define _der_I(coeff) dx[coeff+1]
// Derivative definition for variable: X
#define _der_X(d1,coeff) dx[coeff+1]
