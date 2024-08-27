// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1

// Model Variables Macros

// Macros definition for variable: _event_1
#define _idx_event_1 0
#define _eval_event_1 0

// Macros definition for variable: d
#define _idx_d 0
#define _d d[_idx_d]

// Macros definition for variable: u
#define _idx_u(d1,coeff) ((d1-1))
#define _state_idx_u(d1,coeff) ((d1-1))*2 + coeff
#define _u(d1,coeff) x[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(d1,coeff) x[_state_idx_u(d1,coeff)]
#define _q_u(d1,coeff) q[_state_idx_u(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_u(d1,coeff) ((d1-1))
#define _is_var_u(idx) idx >= 0 && idx < 1000
#define _get_u_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_u(d1,coeff) dx[_state_idx_u(d1,coeff)]


// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_u(_rg_d1,0);

// Event Macros

// Macros for event: 1

#define _zc(coeff) zc[coeff]

// Input Matrix Macros

#define _input_1(i) ((i-1))

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
