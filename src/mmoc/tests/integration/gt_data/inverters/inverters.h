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

// Macros definition for variable: G
#define _G __PAR__G

// Macros definition for variable: UOP
#define _UOP __PAR__UOP

// Macros definition for variable: UTH
#define _UTH __PAR__UTH

// Macros definition for variable: _event_1
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 501
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _event_2
#define _idx_event_2(d1) (501+(d1-1))
#define _eval_event_2(d1) (501+(d1-1))
#define _is_var_event_2(idx) idx >= 501 && idx < 1002
#define _get_event_2_idxs(idx)\
		_d1 = (idx-501)+ 1;

// Macros definition for variable: _event_3
#define _idx_event_3 1002
#define _eval_event_3 1002

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: nextchange
#define _idx_nextchange 0
#define _nextchange d[_idx_nextchange]

// Macros definition for variable: satdx
#define _idx_satdx(d1) (503+(d1-1))
#define _satdx(d1) d[_idx_satdx(d1)]

// Macros definition for variable: satx
#define _idx_satx(d1) (2+(d1-1))
#define _satx(d1) d[_idx_satx(d1)]

// Macros definition for variable: uslope
#define _idx_uslope 1
#define _uslope d[_idx_uslope]

// Macros definition for variable: x
#define _idx_x(d1,coeff) ((d1-1))
#define _state_idx_x(d1,coeff) ((d1-1))*3 + coeff
#define _x(d1,coeff) x[_state_idx_x(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_x(d1,coeff) x[_state_idx_x(d1,coeff)]
#define _eval_x(d1,coeff) ((d1-1))
#define _is_var_x(idx) idx >= 0 && idx < 502
#define _get_x_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_x(d1,coeff) dx[_state_idx_x(d1,coeff)]


// Model Parameters Declaration

// Macro for parameter: G
double __PAR__G;
// Macro for parameter: UOP
double __PAR__UOP;
// Macro for parameter: UTH
double __PAR__UTH;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_x(_rg_d1,0);

// Event Macros

// Macros for event: 1
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _get_event_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_1(_rg_d1);
// Macros for event: 2
#define _apply_usage_event_2(_d1) \
	i = _d1;
#define _get_event_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_2(_rg_d1);
// Macros for event: 3

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
// Derivative definition for variable: x
#define _der_x(d1,coeff) dx[coeff+1]
