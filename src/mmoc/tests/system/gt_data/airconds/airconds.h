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

// Model Variables Macros

// Macros definition for variable: CAP
#define _idx_CAP(d1) ((d1-1))
#define _CAP(d1) __PAR__CAP[_idx_CAP(d1)]

// Macros definition for variable: POT
#define _idx_POT(d1) ((d1-1))
#define _POT(d1) __PAR__POT[_idx_POT(d1)]

// Macros definition for variable: RES
#define _idx_RES(d1) ((d1-1))
#define _RES(d1) __PAR__RES[_idx_RES(d1)]

// Macros definition for variable: THA
#define _THA __PAR__THA

// Macros definition for variable: _event_1
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 20000
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _event_2
#define _idx_event_2(d1) (20000+(d1-1))
#define _eval_event_2(d1) (20000+(d1-1))
#define _is_var_event_2(idx) idx >= 20000 && idx < 40000
#define _get_event_2_idxs(idx)\
		_d1 = (idx-20000)+ 1;

// Macros definition for variable: _event_3
#define _idx_event_3(d1) (40000+(d1-1))
#define _eval_event_3(d1) (40000+(d1-1))
#define _is_var_event_3(idx) idx >= 40000 && idx < 60000
#define _get_event_3_idxs(idx)\
		_d1 = (idx-40000)+ 1;

// Macros definition for variable: _event_4
#define _idx_event_4(d1) (60000+(d1-1))
#define _eval_event_4(d1) (60000+(d1-1))
#define _is_var_event_4(idx) idx >= 60000 && idx < 80000
#define _get_event_4_idxs(idx)\
		_d1 = (idx-60000)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: nextSample
#define _idx_nextSample(d1) (40001+(d1-1))
#define _nextSample(d1) d[_idx_nextSample(d1)]

// Macros definition for variable: noise
#define _idx_noise(d1) (60001+(d1-1))
#define _noise(d1) d[_idx_noise(d1)]

// Macros definition for variable: on
#define _idx_on(d1) (1+(d1-1))
#define _on(d1) d[_idx_on(d1)]

// Macros definition for variable: pmax
#define _pmax __PAR__pmax

// Macros definition for variable: ptotal
#define _idx_ptotal 0
#define _ptotal d[_idx_ptotal]

// Macros definition for variable: th
#define _idx_th(d1,coeff) ((d1-1))
#define _state_idx_th(d1,coeff) ((d1-1))*4 + coeff
#define _th(d1,coeff) x[_state_idx_th(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_th(d1,coeff) x[_state_idx_th(d1,coeff)]
#define _eval_th(d1,coeff) ((d1-1))
#define _is_var_th(idx) idx >= 0 && idx < 20000
#define _get_th_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_th(d1,coeff) dx[_state_idx_th(d1,coeff)]

// Macros definition for variable: tref
#define _idx_tref(d1) (20001+(d1-1))
#define _tref(d1) d[_idx_tref(d1)]


// Model Parameters Declaration

// Macro for parameter: CAP
double __PAR__CAP[20000];
// Macro for parameter: POT
double __PAR__POT[20000];
// Macro for parameter: RES
double __PAR__RES[20000];
// Macro for parameter: THA
double __PAR__THA;
// Macro for parameter: pmax
double __PAR__pmax;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_th(_rg_d1,0);

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
#define _apply_usage_event_3(_d1) \
	i = _d1;
#define _get_event_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_3(_rg_d1);
// Macros for event: 4
#define _apply_usage_event_4(_d1) \
	i = _d1;
#define _get_event_4_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_4(_rg_d1);
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
// Derivative definition for variable: th
#define _der_th(d1,coeff) dx[coeff+1]
