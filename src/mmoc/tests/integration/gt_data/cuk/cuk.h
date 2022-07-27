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

// Model Variables Macros

// Macros definition for variable: C
#define _C __PAR__C

// Macros definition for variable: C1
#define _C1 __PAR__C1

// Macros definition for variable: DC
#define _DC __PAR__DC

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: L1
#define _L1 __PAR__L1

// Macros definition for variable: R
#define _R __PAR__R

// Macros definition for variable: ROff
#define _ROff __PAR__ROff

// Macros definition for variable: ROn
#define _ROn __PAR__ROn

// Macros definition for variable: Rd
#define _idx_Rd 0
#define _Rd d[_idx_Rd]

// Macros definition for variable: Rs
#define _idx_Rs 1
#define _Rs d[_idx_Rs]

// Macros definition for variable: T
#define _T __PAR__T

// Macros definition for variable: U
#define _U __PAR__U

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

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: _out_exp_3
#define _idx_out_exp_3 2
#define _eval_out_exp_3 2

// Macros definition for variable: _out_exp_4
#define _idx_out_exp_4 3
#define _eval_out_exp_4 3

// Macros definition for variable: diodeon
#define _idx_diodeon 4
#define _diodeon d[_idx_diodeon]

// Macros definition for variable: iD
#define _idx_iD(coeff) 0
#define _state_idx_iD(coeff) 0*3 + coeff
#define _iD(coeff) a[_state_idx_iD(coeff)]

// Macros definition for variable: iL
#define _idx_iL(coeff) 3
#define _state_idx_iL(coeff) 3*3 + coeff
#define _iL(coeff) x[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(coeff) x[_state_idx_iL(coeff)]
#define _eval_iL(coeff) 3
#define _eval_dep_iL(coeff) dx[_state_idx_iL(coeff)]

// Macros definition for variable: iL1
#define _idx_iL1(coeff) 1
#define _state_idx_iL1(coeff) 1*3 + coeff
#define _iL1(coeff) x[_state_idx_iL1(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL1(coeff) x[_state_idx_iL1(coeff)]
#define _eval_iL1(coeff) 1
#define _eval_dep_iL1(coeff) dx[_state_idx_iL1(coeff)]

// Macros definition for variable: lastT
#define _idx_lastT 3
#define _lastT d[_idx_lastT]

// Macros definition for variable: nextT
#define _idx_nextT 2
#define _nextT d[_idx_nextT]

// Macros definition for variable: s
#define _idx_s(coeff) 1
#define _state_idx_s(coeff) 1*3 + coeff
#define _s(coeff) a[_state_idx_s(coeff)]

// Macros definition for variable: uC
#define _idx_uC(coeff) 2
#define _state_idx_uC(coeff) 2*3 + coeff
#define _uC(coeff) x[_state_idx_uC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_uC(coeff) x[_state_idx_uC(coeff)]
#define _eval_uC(coeff) 2
#define _eval_dep_uC(coeff) dx[_state_idx_uC(coeff)]

// Macros definition for variable: uC1
#define _idx_uC1(coeff) 0
#define _state_idx_uC1(coeff) 0*3 + coeff
#define _uC1(coeff) x[_state_idx_uC1(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_uC1(coeff) x[_state_idx_uC1(coeff)]
#define _eval_uC1(coeff) 0
#define _eval_dep_uC1(coeff) dx[_state_idx_uC1(coeff)]


// Model Parameters Declaration

// Macro for parameter: C
double __PAR__C;
// Macro for parameter: C1
double __PAR__C1;
// Macro for parameter: DC
double __PAR__DC;
// Macro for parameter: L
double __PAR__L;
// Macro for parameter: L1
double __PAR__L1;
// Macro for parameter: R
double __PAR__R;
// Macro for parameter: ROff
double __PAR__ROff;
// Macro for parameter: ROn
double __PAR__ROn;
// Macro for parameter: T
double __PAR__T;
// Macro for parameter: U
double __PAR__U;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3

// Macros for equation: 4


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2


// Event Macros

// Macros for event: 1

// Macros for event: 2

// Macros for event: 3

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
// Derivative definition for variable: iL
#define _der_iL(coeff) dx[coeff+1]
// Derivative definition for variable: iL1
#define _der_iL1(coeff) dx[coeff+1]
// Derivative definition for variable: uC
#define _der_uC(coeff) dx[coeff+1]
// Derivative definition for variable: uC1
#define _der_uC1(coeff) dx[coeff+1]
