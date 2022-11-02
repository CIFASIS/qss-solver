// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: R
#define _R __PAR__R

// Macros definition for variable: Rd
#define _idx_Rd 0
#define _Rd d[_idx_Rd]

// Macros definition for variable: Roff
#define _Roff __PAR__Roff

// Macros definition for variable: Ron
#define _Ron __PAR__Ron

// Macros definition for variable: U
#define _U __PAR__U

// Macros definition for variable: _event_1
#define _idx_event_1 0
#define _eval_event_1 0

// Macros definition for variable: _event_2
#define _idx_event_2 1
#define _eval_event_2 1

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: iL
#define _idx_iL(coeff) 1
#define _state_idx_iL(coeff) 1*3 + coeff
#define _iL(coeff) x[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(coeff) x[_state_idx_iL(coeff)]
#define _q_iL(coeff) q[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_iL(coeff) 1
#define _eval_dep_iL(coeff) dx[_state_idx_iL(coeff)]

// Macros definition for variable: u
#define _idx_u(coeff) 0
#define _state_idx_u(coeff) 0*3 + coeff
#define _u(coeff) x[_state_idx_u(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(coeff) x[_state_idx_u(coeff)]
#define _q_u(coeff) q[_state_idx_u(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_u(coeff) 0
#define _eval_dep_u(coeff) dx[_state_idx_u(coeff)]

// Macros definition for variable: w
#define _w __PAR__w


// Model Parameters Declaration

// Macro for parameter: L
double __PAR__L;
// Macro for parameter: R
double __PAR__R;
// Macro for parameter: Roff
double __PAR__Roff;
// Macro for parameter: Ron
double __PAR__Ron;
// Macro for parameter: U
double __PAR__U;
// Macro for parameter: w
double __PAR__w;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2


// Event Macros

// Macros for event: 1

// Macros for event: 2

#define _zc(coeff) zc[coeff]

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
// Derivative definition for variable: iL
#define _der_iL(coeff) dx[coeff+1]
// Derivative definition for variable: u
#define _der_u(coeff) dx[coeff+1]
