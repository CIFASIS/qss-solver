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

// Model Variables and Parameters Macros
#define _C __PAR__C
#define _DC __PAR__DC
#define _L __PAR__L
#define _R __PAR__R
#define _ROff __PAR__ROff
#define _ROn __PAR__ROn
#define _idx_Rd 0
#define _Rd d[_idx_Rd]
#define _idx_Rs 1
#define _Rs d[_idx_Rs]
#define _T __PAR__T
#define _U __PAR__U
#define _idx_event_1 0
#define _eval_event_1 0
#define _idx_event_2 1
#define _eval_event_2 1
#define _idx_event_3 2
#define _eval_event_3 2
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _idx_diodeon 4
#define _diodeon d[_idx_diodeon]
#define _idx_iD(coeff) 0
#define _state_idx_iD(coeff) 0*3 + coeff
#define _iD(coeff) a[_state_idx_iD(coeff)]
#define _idx_iL(coeff) 0
#define _state_idx_iL(coeff) 0*3 + coeff
#define _iL(coeff) x[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(coeff) x[_state_idx_iL(coeff)]
#define _eval_iL(coeff) 0
#define _eval_dep_iL(coeff) dx[_state_idx_iL(coeff)]
#define _idx_lastT 3
#define _lastT d[_idx_lastT]
#define _idx_nextT 2
#define _nextT d[_idx_nextT]
#define _idx_s(coeff) 1
#define _state_idx_s(coeff) 1*3 + coeff
#define _s(coeff) a[_state_idx_s(coeff)]
#define _idx_uC(coeff) 1
#define _state_idx_uC(coeff) 1*3 + coeff
#define _uC(coeff) x[_state_idx_uC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_uC(coeff) x[_state_idx_uC(coeff)]
#define _eval_uC(coeff) 1
#define _eval_dep_uC(coeff) dx[_state_idx_uC(coeff)]

// Model Parameters Declaration
double __PAR__C;
double __PAR__DC;
double __PAR__L;
double __PAR__R;
double __PAR__ROff;
double __PAR__ROn;
double __PAR__T;
double __PAR__U;

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
#define _der_iL(coeff) dx[coeff+1]
#define _der_uC(coeff) dx[coeff+1]
