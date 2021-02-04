// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.
#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2
#define COEFF_MULTIPLIER_3 6

// Model Variables and Parameters Macros
#define _idx_I(d1) ((d1-1))
#define _I(d1) __PAR__I[_idx_I(d1)]
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 1000
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _a __PAR__a
#define _b __PAR__b
#define _idx_c(d1) ((d1-1))
#define _c(d1) __PAR__c[_idx_c(d1)]
#define _idx_d(d1) ((d1-1))
#define _d(d1) __PAR__d[_idx_d(d1)]
#define _idx_r(d1) ((d1-1))
#define _r(d1) __PAR__r[_idx_r(d1)]
#define _idx_u(d1,coeff) (1000+(d1-1))
#define _state_idx_u(d1,coeff) (1000+(d1-1))*4 + coeff
#define _u(d1,coeff) x[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,coeff) x[_state_idx_u(d1,coeff)]
#define _eval_u(d1,coeff) (1000+(d1-1))
#define _is_var_u(idx) idx >= 1000 && idx < 2000
#define _get_u_idxs(idx)\
		_d1 = (idx-1000)+ 1;
#define _eval_dep_u(d1,coeff) dx[_state_idx_u(d1,coeff)]
#define _idx_v(d1,coeff) ((d1-1))
#define _state_idx_v(d1,coeff) ((d1-1))*4 + coeff
#define _v(d1,coeff) x[_state_idx_v(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_v(d1,coeff) x[_state_idx_v(d1,coeff)]
#define _eval_v(d1,coeff) ((d1-1))
#define _is_var_v(idx) idx >= 0 && idx < 1000
#define _get_v_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_v(d1,coeff) dx[_state_idx_v(d1,coeff)]

// Model Parameters Declaration
double __PAR__I[1000];
double __PAR__a;
double __PAR__b;
double __PAR__c[1000];
double __PAR__d[1000];
double __PAR__r[1000];

// Derivative Equations Macros
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_v(_rg_d1,0);
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_u(_rg_d1,0);

// Algebraic Equations Macros

// Event Macros
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _get_event_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_1(_rg_d1);
#define _zc(coeff) zc[coeff]

// Output Equations Macros
#define _out out[0]

// Input Matrix Macros

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
#define _der_u(d1,coeff) dx[coeff+1]
#define _der_v(d1,coeff) dx[coeff+1]
