// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2
#define COEFF_MULTIPLIER_3 6

// Model Variables Macros

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: y
#define _idx_y(coeff) 0
#define _state_idx_y(coeff) 0*4 + coeff
#define _y(coeff) x[_state_idx_y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_y(coeff) x[_state_idx_y(coeff)]
#define _q_y(coeff) q[_state_idx_y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_y(coeff) 0
#define _eval_dep_y(coeff) dx[_state_idx_y(coeff)]


// Derivative Equations Macros

// Macros for equation: 1


// Output Equations Macros

// Macros for output equation: 1

#define _out out[0]

// Input Matrix Macros

#define _input_1 0

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: y
#define _der_y(coeff) dx[coeff+1]
