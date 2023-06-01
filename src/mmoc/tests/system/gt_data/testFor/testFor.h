// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1(d1) ((d1-1))
#define _eval_out_exp_1(d1) ((d1-1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 2
#define _get_out_exp_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: ax
#define _ax __PAR__ax

// Macros definition for variable: ay
#define _ay __PAR__ay

// Macros definition for variable: dx
#define _dx __PAR__dx

// Macros definition for variable: dy
#define _dy __PAR__dy

// Macros definition for variable: r
#define _r __PAR__r

// Macros definition for variable: u1
#define _idx_u1(d1,coeff) ((d1-1))
#define _state_idx_u1(d1,coeff) ((d1-1))*3 + coeff
#define _u1(d1,coeff) x[_state_idx_u1(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u1(d1,coeff) x[_state_idx_u1(d1,coeff)]
#define _q_u1(d1,coeff) q[_state_idx_u1(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_u1(d1,coeff) ((d1-1))
#define _is_var_u1(idx) idx >= 0 && idx < 2
#define _get_u1_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_u1(d1,coeff) dx[_state_idx_u1(d1,coeff)]


// Model Parameters Declaration

// Macro for parameter: ax
double __PAR__ax;
// Macro for parameter: ay
double __PAR__ay;
// Macro for parameter: dx
double __PAR__dx;
// Macro for parameter: dy
double __PAR__dy;
// Macro for parameter: r
double __PAR__r;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_u1(_rg_d1,0);

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
// Derivative definition for variable: u1
#define _der_u1(d1,coeff) dx[coeff+1]
