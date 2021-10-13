// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.
#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables and Parameters Macros
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _ax __PAR__ax
#define _ay __PAR__ay
#define _dx __PAR__dx
#define _dy __PAR__dy
#define _r __PAR__r
#define _idx_u(d1,d2,coeff) ((d1-1)*20+(d2-1))
#define _state_idx_u(d1,d2,coeff) ((d1-1)*20+(d2-1))*3 + coeff
#define _u(d1,d2,coeff) x[_state_idx_u(d1,d2,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,d2,coeff) x[_state_idx_u(d1,d2,coeff)]
#define _eval_u(d1,d2,coeff) ((d1-1)*20+(d2-1))
#define _is_var_u(idx) idx >= 0 && idx < 400
#define _get_u_idxs(idx)\
		_d1 = ((idx)/20)+ 1;\
	_d2 = ((idx)%20)+ 1;
#define _eval_dep_u(d1,d2,coeff) dx[_state_idx_u(d1,d2,coeff)]

// Model Parameters Declaration
double __PAR__ax;
double __PAR__ay;
double __PAR__dx;
double __PAR__dy;
double __PAR__r;

// Derivative Equations Macros
#define _apply_usage_eq_2(_d1,_d2) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	_rg_d2 = 1;\
	var = _idx_u(_rg_d1,_rg_d2,0);
#define _apply_usage_eq_3(_d1,_d2) \
	j = _d2;
#define _get_eq_3_var_idxs(row, var)\
	_rg_d2 = 1 + (row-1)+ 1;\
	_rg_d1 = 1;\
	var = _idx_u(_rg_d1,_rg_d2,0);
#define _apply_usage_eq_4(_d1,_d2) \
	i = _d1;\
	j = _d2;
#define _get_eq_4_var_idxs(row, var)\
	_rg_d1 = 1 + ((row-1)/19)+ 1;\
	_rg_d2 = 1 + ((row-1)%19)+ 1;\
	var = _idx_u(_rg_d1,_rg_d2,0);

// Algebraic Equations Macros

// Event Macros

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
#define _der_u(d1,d2,coeff) dx[coeff+1]
