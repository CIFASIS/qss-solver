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
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 2
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _idx_x(d1,coeff) ((d1-1))
#define _state_idx_x(d1,coeff) ((d1-1))*4 + coeff
#define _x(d1,coeff) x[_state_idx_x(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x(d1,coeff) x[_state_idx_x(d1,coeff)]
#define _eval_x(d1,coeff) ((d1-1))
#define _is_var_x(idx) idx >= 0 && idx < 2
#define _get_x_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_x(d1,coeff) dx[_state_idx_x(d1,coeff)]

// Derivative Equations Macros

// Algebraic Equations Macros

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
#define _der_x(d1,coeff) dx[coeff+1]
