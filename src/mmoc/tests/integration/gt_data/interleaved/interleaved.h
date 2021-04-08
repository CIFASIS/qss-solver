// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d; \
  double* a = m->alg;

// Coeff multipliers definition.
#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables and Parameters Macros
#define _C __PAR__C
#define _DC __PAR__DC
#define _L __PAR__L
#define _R __PAR__R
#define _ROff __PAR__ROff
#define _ROn __PAR__ROn
#define _idx_Rd(d1) ((d1-1))
#define _Rd(d1) d[_idx_Rd(d1)]
#define _idx_Rs(d1) (4+(d1-1))
#define _Rs(d1) d[_idx_Rs(d1)]
#define _T __PAR__T
#define _U __PAR__U
#define _idx_event_1 0
#define _eval_event_1 0
#define _idx_event_2(d1) (1+(d1-1))
#define _eval_event_2(d1) (1+(d1-1))
#define _is_var_event_2(idx) idx >= 1 && idx < 5
#define _get_event_2_idxs(idx)\
		_d1 = (idx-1)+ 1;
#define _idx_event_3(d1) (5+(d1-1))
#define _eval_event_3(d1) (5+(d1-1))
#define _is_var_event_3(idx) idx >= 5 && idx < 9
#define _get_event_3_idxs(idx)\
		_d1 = (idx-5)+ 1;
#define _idx_event_4(d1) (9+(d1-1))
#define _eval_event_4(d1) (9+(d1-1))
#define _is_var_event_4(idx) idx >= 9 && idx < 13
#define _get_event_4_idxs(idx)\
		_d1 = (idx-9)+ 1;
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2(d1) (1+(d1-1))
#define _eval_out_exp_2(d1) (1+(d1-1))
#define _is_var_out_exp_2(idx) idx >= 1 && idx < 5
#define _get_out_exp_2_idxs(idx)\
		_d1 = (idx-1)+ 1;
#define _idx_sum_0(d1,coeff) ((d1-1))
#define _state_idx_sum_0(d1,coeff) ((d1-1))*3 + coeff
#define _sum_0(d1,coeff) a[_state_idx_sum_0(d1,coeff)]
#define _is_var_sum_0(idx) idx >= 0 && idx < 4
#define _get_sum_0_idxs(idx)\
		_d1 = (idx)+ 1;
#define _idx_iL(d1,coeff) (1+(d1-1))
#define _state_idx_iL(d1,coeff) (1+(d1-1))*3 + coeff
#define _iL(d1,coeff) x[_state_idx_iL(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(d1,coeff) x[_state_idx_iL(d1,coeff)]
#define _eval_iL(d1,coeff) (1+(d1-1))
#define _is_var_iL(idx) idx >= 1 && idx < 5
#define _get_iL_idxs(idx)\
		_d1 = (idx-1)+ 1;
#define _eval_dep_iL(d1,coeff) dx[_state_idx_iL(d1,coeff)]
#define _idx_lastT 9
#define _lastT d[_idx_lastT]
#define _idx_nextT 8
#define _nextT d[_idx_nextT]
#define _idx_uC(coeff) 0
#define _state_idx_uC(coeff) 0*3 + coeff
#define _uC(coeff) x[_state_idx_uC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_uC(coeff) x[_state_idx_uC(coeff)]
#define _eval_uC(coeff) 0
#define _eval_dep_uC(coeff) dx[_state_idx_uC(coeff)]

// Model Parameters Declaration
double __PAR__C;
double __PAR__DC;
double __PAR__L;
double __PAR__R;
double __PAR__ROff;
double __PAR__ROn;
double __PAR__T;
double __PAR__U;

// Derivative Equations Macros
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1,0);

// Algebraic Equations Macros
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_sum_0(_rg_d1,0);

// Event Macros
#define _apply_usage_event_2(_d1) \
	i = _d1;
#define _get_event_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_2(_rg_d1);
#define _apply_usage_event_3(_d1) \
	i = _d1;
#define _get_event_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_3(_rg_d1);
#define _apply_usage_event_4(_d1) \
	i = _d1;
#define _get_event_4_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_4(_rg_d1);
#define _zc(coeff) zc[coeff]

// Output Equations Macros
#define _apply_usage_out_exp_2(_d1) \
	_d1 = _d1;
#define _get_out_exp_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_out_exp_2(_rg_d1);
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
#define _der_iL(d1,coeff) dx[coeff+1]
#define _der_uC(coeff) dx[coeff+1]
