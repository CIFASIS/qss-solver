// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Coeff multipliers definition.
#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables and Parameters Macros
#define _C __PAR__C
#define _L __PAR__L
#define _R __PAR__R
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 9
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _idx_out_exp_2 9
#define _eval_out_exp_2 9
#define _idx_a1(d1,coeff) ((d1-1))
#define _state_idx_a1(d1,coeff) ((d1-1))*3 + coeff
#define _a1(d1,coeff) a[_state_idx_a1(d1,coeff)]
#define _is_var_a1(idx) idx >= 0 && idx < 10
#define _get_a1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _idx_x1(d1,coeff) ((d1-1))
#define _state_idx_x1(d1,coeff) ((d1-1))*3 + coeff
#define _x1(d1,coeff) x[_state_idx_x1(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x1(d1,coeff) x[_state_idx_x1(d1,coeff)]
#define _eval_x1(d1,coeff) ((d1-1))
#define _is_var_x1(idx) idx >= 0 && idx < 10
#define _get_x1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_x1(d1,coeff) dx[_state_idx_x1(d1,coeff)]
#define _idx_x2(d1,coeff) (10+0)
#define _state_idx_x2(d1,coeff) (10+0)*3 + coeff
#define _x2(d1,coeff) x[_state_idx_x2(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x2(d1,coeff) x[_state_idx_x2(d1,coeff)]
#define _eval_x2(d1,coeff) (10+(d1-1))
#define _is_var_x2(idx) idx >= 10 && idx < 11
#define _get_x2_idxs(idx)\
		_d1 = (idx-10)+ 1;
#define _eval_dep_x2(d1,coeff) dx[_state_idx_x2(d1,coeff)]

// Model Parameters Declaration
double __PAR__C;
double __PAR__L;
double __PAR__R;

// Derivative Equations Macros
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_x1(_rg_d1,0);

// Algebraic Equations Macros
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_a1(_rg_d1,0);

// Event Macros

// Output Equations Macros
#define _apply_usage_out_exp_1(_d1) \
	_d1 = _d1;
#define _get_out_exp_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_out_exp_1(_rg_d1);
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
#define _der_x1(d1,coeff) dx[coeff+1]
#define _der_x2(d1,coeff) dx[coeff+1]