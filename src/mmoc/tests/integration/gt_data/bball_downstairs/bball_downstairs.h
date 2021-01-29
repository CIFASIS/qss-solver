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

// Model Variables and Parameters Macros
#define _idx_event_1 0
#define _eval_event_1 0
#define _idx_event_2 1
#define _eval_event_2 1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_contact 1
#define _contact d[_idx_contact]
#define _idx_stair 0
#define _stair d[_idx_stair]
#define _idx_vx(coeff) 3
#define _state_idx_vx(coeff) 3*4 + coeff
#define _vx(coeff) x[_state_idx_vx(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_vx(coeff) x[_state_idx_vx(coeff)]
#define _eval_vx(coeff) 3
#define _eval_dep_vx(coeff) dx[_state_idx_vx(coeff)]
#define _idx_vy(coeff) 1
#define _state_idx_vy(coeff) 1*4 + coeff
#define _vy(coeff) x[_state_idx_vy(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_vy(coeff) x[_state_idx_vy(coeff)]
#define _eval_vy(coeff) 1
#define _eval_dep_vy(coeff) dx[_state_idx_vy(coeff)]
#define _idx_x(coeff) 2
#define _state_idx_x(coeff) 2*4 + coeff
#define _x(coeff) x[_state_idx_x(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x(coeff) x[_state_idx_x(coeff)]
#define _eval_x(coeff) 2
#define _eval_dep_x(coeff) dx[_state_idx_x(coeff)]
#define _idx_y(coeff) 0
#define _state_idx_y(coeff) 0*4 + coeff
#define _y(coeff) x[_state_idx_y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_y(coeff) x[_state_idx_y(coeff)]
#define _eval_y(coeff) 0
#define _eval_dep_y(coeff) dx[_state_idx_y(coeff)]

// Derivative Equations Macros

// Algebraic Equations Macros

// Event Macros
#define _zc(coeff) zc[coeff]

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
#define _der_vx(coeff) dx[coeff+1]
#define _der_vy(coeff) dx[coeff+1]
#define _der_x(coeff) dx[coeff+1]
#define _der_y(coeff) dx[coeff+1]
