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
#define _G __PAR__G
#define _UOP __PAR__UOP
#define _UTH __PAR__UTH
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 501
#define _get_event_1_idxs(idx) \
		_d1 = (idx-0)+ 1;
#define _idx_event_2(d1) (501+(d1-1))
#define _eval_event_2(d1) (501+(d1-1))
#define _is_var_event_2(idx) idx >= 501 && idx < 1002
#define _get_event_2_idxs(idx) \
		_d1 = (idx-501)+ 1;
#define _idx_event_3 1002
#define _eval_event_3 1002
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _idx_nextchange 0
#define _nextchange d[_idx_nextchange]
#define _idx_satdx(d1) (503+(d1-1))
#define _satdx(d1) d[_idx_satdx(d1)]
#define _idx_satx(d1) (2+(d1-1))
#define _satx(d1) d[_idx_satx(d1)]
#define _idx_uslope 1
#define _uslope d[_idx_uslope]
#define _idx_x(d1,coeff) ((d1-1))
#define _state_idx_x(d1,coeff) ((d1-1))*3 + coeff
#define _x(d1,coeff) x[_state_idx_x(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x(d1,coeff) x[_state_idx_x(d1,coeff)]
#define _eval_x(d1,coeff) ((d1-1))
#define _is_var_x(idx) idx >= 0 && idx < 502
#define _get_x_idxs(idx) \
		_d1 = (idx-0)+ 1;
#define _eval_dep_x(d1,coeff) dx[_state_idx_x(d1,coeff)]

// Model Parameters Declaration
double __PAR__G;
double __PAR__UOP;
double __PAR__UTH;

// Derivative Equations Macros
#define _apply_usage_eq_2(_d1) \
	i = _d1;

// Event Macros
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _apply_usage_event_2(_d1) \
	i = _d1;
#define _zc(coeff) zc[coeff]

// Output Equations Macros
#define _out out[0]

// Input Matrix Macros

// Jacobian Macros definition. 
#define _jac(i) jac[i++]

#define _time t

// Derivative Macros definition. 
#define _der_x(d1,coeff) dx[coeff+1]
