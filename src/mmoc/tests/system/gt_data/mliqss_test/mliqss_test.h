// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1

// Model Variables Macros

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: x1
#define _idx_x1(coeff) 0
#define _state_idx_x1(coeff) 0*2 + coeff
#define _x1(coeff) x[_state_idx_x1(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x1(coeff) x[_state_idx_x1(coeff)]
#define _q_x1(coeff) q[_state_idx_x1(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_x1(coeff) 0
#define _eval_dep_x1(coeff) dx[_state_idx_x1(coeff)]

// Macros definition for variable: x2
#define _idx_x2(coeff) 1
#define _state_idx_x2(coeff) 1*2 + coeff
#define _x2(coeff) x[_state_idx_x2(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x2(coeff) x[_state_idx_x2(coeff)]
#define _q_x2(coeff) q[_state_idx_x2(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_x2(coeff) 1
#define _eval_dep_x2(coeff) dx[_state_idx_x2(coeff)]


// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2


// Output Equations Macros

// Macros for output equation: 1

// Macros for output equation: 2

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: x1
#define _der_x1(coeff) dx[coeff+1]
// Derivative definition for variable: x2
#define _der_x2(coeff) dx[coeff+1]
