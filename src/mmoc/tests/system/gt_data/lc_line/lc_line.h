// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2
#define COEFF_MULTIPLIER_3 6

// Model Variables Macros

// Macros definition for variable: _event_1
#define _idx_event_1 0
#define _eval_event_1 0

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: _out_exp_3
#define _idx_out_exp_3 2
#define _eval_out_exp_3 2

// Macros definition for variable: _out_exp_4
#define _idx_out_exp_4 3
#define _eval_out_exp_4 3

// Macros definition for variable: d
#define _idx_d 0
#define _d d[_idx_d]

// Macros definition for variable: x
#define _idx_x(d1,coeff) ((d1-1))
#define _state_idx_x(d1,coeff) ((d1-1))*4 + coeff
#define _x(d1,coeff) x[_state_idx_x(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x(d1,coeff) x[_state_idx_x(d1,coeff)]
#define _eval_x(d1,coeff) ((d1-1))
#define _is_var_x(idx) idx >= 0 && idx < 200
#define _get_x_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_x(d1,coeff) dx[_state_idx_x(d1,coeff)]


// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_x(_rg_d1,0);
// Macros for equation: 2

// Macros for equation: 3


// Event Macros

// Macros for event: 1

#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1

// Macros for output equation: 2

// Macros for output equation: 3

// Macros for output equation: 4

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
#define _der_x(d1,coeff) dx[coeff+1]
