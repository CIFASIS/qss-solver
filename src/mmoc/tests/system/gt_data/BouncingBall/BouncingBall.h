// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d; \
  double* a = m->alg;

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

// Macros definition for variable: _event_2
#define _idx_event_2 1
#define _eval_event_2 1

// Macros definition for variable: _event_3
#define _idx_event_3 2
#define _eval_event_3 2

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: ball1_flange1_f
#define _idx_ball1_flange1_f(coeff) 12
#define _state_idx_ball1_flange1_f(coeff) 12*4 + coeff
#define _ball1_flange1_f(coeff) a[_state_idx_ball1_flange1_f(coeff)]

// Macros definition for variable: ball1_flange1_s
#define _idx_ball1_flange1_s(coeff) 3
#define _state_idx_ball1_flange1_s(coeff) 3*4 + coeff
#define _ball1_flange1_s(coeff) a[_state_idx_ball1_flange1_s(coeff)]

// Macros definition for variable: ball1_flange1_v
#define _idx_ball1_flange1_v(coeff) 2
#define _state_idx_ball1_flange1_v(coeff) 2*4 + coeff
#define _ball1_flange1_v(coeff) a[_state_idx_ball1_flange1_v(coeff)]

// Macros definition for variable: ball1_g
#define _ball1_g __PAR__ball1_g

// Macros definition for variable: ball1_m
#define _ball1_m __PAR__ball1_m

// Macros definition for variable: ball1_v
#define _idx_ball1_v(coeff) 1
#define _state_idx_ball1_v(coeff) 1*4 + coeff
#define _ball1_v(coeff) x[_state_idx_ball1_v(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_ball1_v(coeff) x[_state_idx_ball1_v(coeff)]
#define _q_ball1_v(coeff) q[_state_idx_ball1_v(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_ball1_v(coeff) 1
#define _eval_dep_ball1_v(coeff) dx[_state_idx_ball1_v(coeff)]

// Macros definition for variable: ball1_y
#define _idx_ball1_y(coeff) 0
#define _state_idx_ball1_y(coeff) 0*4 + coeff
#define _ball1_y(coeff) x[_state_idx_ball1_y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_ball1_y(coeff) x[_state_idx_ball1_y(coeff)]
#define _q_ball1_y(coeff) q[_state_idx_ball1_y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_ball1_y(coeff) 0
#define _eval_dep_ball1_y(coeff) dx[_state_idx_ball1_y(coeff)]

// Macros definition for variable: d0
#define _idx_d0 0
#define _d0 d[_idx_d0]

// Macros definition for variable: d1
#define _idx_d1 1
#define _d1 d[_idx_d1]

// Macros definition for variable: fixed1_flange1_f
#define _idx_fixed1_flange1_f(coeff) 13
#define _state_idx_fixed1_flange1_f(coeff) 13*4 + coeff
#define _fixed1_flange1_f(coeff) a[_state_idx_fixed1_flange1_f(coeff)]

// Macros definition for variable: fixed1_flange1_s
#define _idx_fixed1_flange1_s(coeff) 0
#define _state_idx_fixed1_flange1_s(coeff) 0*4 + coeff
#define _fixed1_flange1_s(coeff) a[_state_idx_fixed1_flange1_s(coeff)]

// Macros definition for variable: fixed1_flange1_v
#define _idx_fixed1_flange1_v(coeff) 1
#define _state_idx_fixed1_flange1_v(coeff) 1*4 + coeff
#define _fixed1_flange1_v(coeff) a[_state_idx_fixed1_flange1_v(coeff)]

// Macros definition for variable: fixed1_s0
#define _fixed1_s0 __PAR__fixed1_s0

// Macros definition for variable: spring1_b
#define _spring1_b __PAR__spring1_b

// Macros definition for variable: spring1_dv
#define _idx_spring1_dv(coeff) 8
#define _state_idx_spring1_dv(coeff) 8*4 + coeff
#define _spring1_dv(coeff) a[_state_idx_spring1_dv(coeff)]

// Macros definition for variable: spring1_dy
#define _idx_spring1_dy(coeff) 9
#define _state_idx_spring1_dy(coeff) 9*4 + coeff
#define _spring1_dy(coeff) a[_state_idx_spring1_dy(coeff)]

// Macros definition for variable: spring1_flange1_f
#define _idx_spring1_flange1_f(coeff) 10
#define _state_idx_spring1_flange1_f(coeff) 10*4 + coeff
#define _spring1_flange1_f(coeff) a[_state_idx_spring1_flange1_f(coeff)]

// Macros definition for variable: spring1_flange1_s
#define _idx_spring1_flange1_s(coeff) 6
#define _state_idx_spring1_flange1_s(coeff) 6*4 + coeff
#define _spring1_flange1_s(coeff) a[_state_idx_spring1_flange1_s(coeff)]

// Macros definition for variable: spring1_flange1_v
#define _idx_spring1_flange1_v(coeff) 7
#define _state_idx_spring1_flange1_v(coeff) 7*4 + coeff
#define _spring1_flange1_v(coeff) a[_state_idx_spring1_flange1_v(coeff)]

// Macros definition for variable: spring1_flange2_f
#define _idx_spring1_flange2_f(coeff) 11
#define _state_idx_spring1_flange2_f(coeff) 11*4 + coeff
#define _spring1_flange2_f(coeff) a[_state_idx_spring1_flange2_f(coeff)]

// Macros definition for variable: spring1_flange2_s
#define _idx_spring1_flange2_s(coeff) 4
#define _state_idx_spring1_flange2_s(coeff) 4*4 + coeff
#define _spring1_flange2_s(coeff) a[_state_idx_spring1_flange2_s(coeff)]

// Macros definition for variable: spring1_flange2_v
#define _idx_spring1_flange2_v(coeff) 5
#define _state_idx_spring1_flange2_v(coeff) 5*4 + coeff
#define _spring1_flange2_v(coeff) a[_state_idx_spring1_flange2_v(coeff)]

// Macros definition for variable: spring1_k
#define _spring1_k __PAR__spring1_k


// Model Parameters Declaration

// Macro for parameter: ball1_g
double __PAR__ball1_g;
// Macro for parameter: ball1_m
double __PAR__ball1_m;
// Macro for parameter: fixed1_s0
double __PAR__fixed1_s0;
// Macro for parameter: spring1_b
double __PAR__spring1_b;
// Macro for parameter: spring1_k
double __PAR__spring1_k;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2

// Macros for algebraic equation: 3

// Macros for algebraic equation: 4

// Macros for algebraic equation: 5

// Macros for algebraic equation: 6

// Macros for algebraic equation: 7

// Macros for algebraic equation: 8

// Macros for algebraic equation: 9

// Macros for algebraic equation: 10

// Macros for algebraic equation: 11

// Macros for algebraic equation: 12

// Macros for algebraic equation: 13

// Macros for algebraic equation: 14


// Event Macros

// Macros for event: 1

// Macros for event: 2

// Macros for event: 3

#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: ball1_v
#define _der_ball1_v(coeff) dx[coeff+1]
// Derivative definition for variable: ball1_y
#define _der_ball1_y(coeff) dx[coeff+1]
