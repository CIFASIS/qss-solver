// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Coeff multipliers definition.
#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables and Parameters Macros
#define _L __PAR__L
#define _R __PAR__R
#define _idx_Rd 0
#define _Rd d[_idx_Rd]
#define _Roff __PAR__Roff
#define _Ron __PAR__Ron
#define _U __PAR__U
#define _idx_event_1 0
#define _eval_event_1 0
#define _idx_event_2 1
#define _eval_event_2 1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _idx_iL(coeff) 1
#define _state_idx_iL(coeff) 1*3 + coeff
#define _iL(coeff) x[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(coeff) x[_state_idx_iL(coeff)]
#define _eval_iL(coeff) 1
#define _eval_dep_iL(coeff) dx[_state_idx_iL(coeff)]
#define _idx_u(coeff) 0
#define _state_idx_u(coeff) 0*3 + coeff
#define _u(coeff) x[_state_idx_u(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_u(coeff) x[_state_idx_u(coeff)]
#define _eval_u(coeff) 0
#define _eval_dep_u(coeff) dx[_state_idx_u(coeff)]
#define _w __PAR__w

// Model Parameters Declaration
double __PAR__L;
double __PAR__R;
double __PAR__Roff;
double __PAR__Ron;
double __PAR__U;
double __PAR__w;

// Derivative Equations Macros

// Event Macros
#define _zc(coeff) zc[coeff]

// Output Equations Macros
#define _out out[0]

// Input Matrix Macros

// Jacobian Macros definition. 
#define _jac(i) jac[i++]

#define _time t

// Derivative Macros definition. 
#define _der_iL(coeff) dx[coeff+1]
#define _der_u(coeff) dx[coeff+1]
