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
#define _alpha __PAR__alpha
#define _mu __PAR__mu
#define _idx_u(d1,coeff) ((d1-1))
#define _state_idx_u(d1,coeff) ((d1-1))*3 + coeff
#define _u(d1,coeff) x[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,coeff) x[_state_idx_u(d1,coeff)]
#define _eval_u(d1,coeff) ((d1-1))
#define _is_var_u(idx) idx >= 0 && idx < 20000
#define _get_u_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_u(d1,coeff) dx[_state_idx_u(d1,coeff)]

// Model Parameters Declaration
double __PAR__alpha;
double __PAR__mu;

// Derivative Equations Macros
#define _apply_usage_eq_2(_d1) \
	j = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_u(_rg_d1,0);

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
#define _der_u(d1,coeff) dx[coeff+1]
