// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: C2
#define _idx_C2(coeff) 0
#define _state_idx_C2(coeff) 0*3 + coeff
#define _C2(coeff) x[_state_idx_C2(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_C2(coeff) x[_state_idx_C2(coeff)]
#define _q_C2(coeff) q[_state_idx_C2(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_C2(coeff) 0
#define _eval_dep_C2(coeff) dx[_state_idx_C2(coeff)]

// Macros definition for variable: CP
#define _idx_CP(coeff) 1
#define _state_idx_CP(coeff) 1*3 + coeff
#define _CP(coeff) x[_state_idx_CP(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_CP(coeff) x[_state_idx_CP(coeff)]
#define _q_CP(coeff) q[_state_idx_CP(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_CP(coeff) 1
#define _eval_dep_CP(coeff) dx[_state_idx_CP(coeff)]

// Macros definition for variable: CT
#define _idx_CT(coeff) 0
#define _state_idx_CT(coeff) 0*3 + coeff
#define _CT(coeff) a[_state_idx_CT(coeff)]

// Macros definition for variable: F
#define _idx_F(coeff) 2
#define _state_idx_F(coeff) 2*3 + coeff
#define _F(coeff) a[_state_idx_F(coeff)]

// Macros definition for variable: M
#define _idx_M(coeff) 3
#define _state_idx_M(coeff) 3*3 + coeff
#define _M(coeff) x[_state_idx_M(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_M(coeff) x[_state_idx_M(coeff)]
#define _q_M(coeff) q[_state_idx_M(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_M(coeff) 3
#define _eval_dep_M(coeff) dx[_state_idx_M(coeff)]

// Macros definition for variable: Y
#define _idx_Y(coeff) 4
#define _state_idx_Y(coeff) 4*3 + coeff
#define _Y(coeff) x[_state_idx_Y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_Y(coeff) x[_state_idx_Y(coeff)]
#define _q_Y(coeff) q[_state_idx_Y(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_Y(coeff) 4
#define _eval_dep_Y(coeff) dx[_state_idx_Y(coeff)]

// Macros definition for variable: YT
#define _idx_YT(coeff) 1
#define _state_idx_YT(coeff) 1*3 + coeff
#define _YT(coeff) a[_state_idx_YT(coeff)]

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

// Macros definition for variable: _out_exp_5
#define _idx_out_exp_5 4
#define _eval_out_exp_5 4

// Macros definition for variable: _out_exp_6
#define _idx_out_exp_6 5
#define _eval_out_exp_6 5

// Macros definition for variable: k1
#define _k1 __PAR__k1

// Macros definition for variable: k2
#define _k2 __PAR__k2

// Macros definition for variable: k3
#define _k3 __PAR__k3

// Macros definition for variable: k4
#define _k4 __PAR__k4

// Macros definition for variable: k4prim
#define _k4prim __PAR__k4prim

// Macros definition for variable: k5
#define _k5 __PAR__k5

// Macros definition for variable: k6
#define _k6 __PAR__k6

// Macros definition for variable: k7
#define _k7 __PAR__k7

// Macros definition for variable: k8
#define _k8 __PAR__k8

// Macros definition for variable: k9
#define _k9 __PAR__k9

// Macros definition for variable: pM
#define _idx_pM(coeff) 2
#define _state_idx_pM(coeff) 2*3 + coeff
#define _pM(coeff) x[_state_idx_pM(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_pM(coeff) x[_state_idx_pM(coeff)]
#define _q_pM(coeff) q[_state_idx_pM(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_pM(coeff) 2
#define _eval_dep_pM(coeff) dx[_state_idx_pM(coeff)]

// Macros definition for variable: yP
#define _idx_yP(coeff) 5
#define _state_idx_yP(coeff) 5*3 + coeff
#define _yP(coeff) x[_state_idx_yP(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_yP(coeff) x[_state_idx_yP(coeff)]
#define _q_yP(coeff) q[_state_idx_yP(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_yP(coeff) 5
#define _eval_dep_yP(coeff) dx[_state_idx_yP(coeff)]

// Macros definition for variable: yt
#define _idx_yt(coeff) 3
#define _state_idx_yt(coeff) 3*3 + coeff
#define _yt(coeff) a[_state_idx_yt(coeff)]


// Model Parameters Declaration

// Macro for parameter: k1
double __PAR__k1;
// Macro for parameter: k2
double __PAR__k2;
// Macro for parameter: k3
double __PAR__k3;
// Macro for parameter: k4
double __PAR__k4;
// Macro for parameter: k4prim
double __PAR__k4prim;
// Macro for parameter: k5
double __PAR__k5;
// Macro for parameter: k6
double __PAR__k6;
// Macro for parameter: k7
double __PAR__k7;
// Macro for parameter: k8
double __PAR__k8;
// Macro for parameter: k9
double __PAR__k9;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3

// Macros for equation: 4

// Macros for equation: 5

// Macros for equation: 6


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2

// Macros for algebraic equation: 3

// Macros for algebraic equation: 4


// Output Equations Macros

// Macros for output equation: 1

// Macros for output equation: 2

// Macros for output equation: 3

// Macros for output equation: 4

// Macros for output equation: 5

// Macros for output equation: 6

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: C2
#define _der_C2(coeff) dx[coeff+1]
// Derivative definition for variable: CP
#define _der_CP(coeff) dx[coeff+1]
// Derivative definition for variable: M
#define _der_M(coeff) dx[coeff+1]
// Derivative definition for variable: Y
#define _der_Y(coeff) dx[coeff+1]
// Derivative definition for variable: pM
#define _der_pM(coeff) dx[coeff+1]
// Derivative definition for variable: yP
#define _der_yP(coeff) dx[coeff+1]
