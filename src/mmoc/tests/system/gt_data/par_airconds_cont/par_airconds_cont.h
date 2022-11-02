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

// Macros definition for variable: Ki
#define _Ki __PAR__Ki

// Macros definition for variable: Kp
#define _Kp __PAR__Kp

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
#define _is_var_event_1(idx) idx >= 0 && idx < 4
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _event_2
#define _idx_event_2(d1) (4+(d1-1))
#define _eval_event_2(d1) (4+(d1-1))
#define _is_var_event_2(idx) idx >= 4 && idx < 8
#define _get_event_2_idxs(idx)\
		_d1 = (idx-4)+ 1;

// Macros definition for variable: _event_3
#define _idx_event_3 8
#define _eval_event_3 8

// Macros definition for variable: _event_4
#define _idx_event_4 9
#define _eval_event_4 9

// Macros definition for variable: _event_5
#define _idx_event_5 10
#define _eval_event_5 10

// Macros definition for variable: _event_6
#define _idx_event_6(d1) (11+(d1-1))
#define _eval_event_6(d1) (11+(d1-1))
#define _is_var_event_6(idx) idx >= 11 && idx < 4011
#define _get_event_6_idxs(idx)\
		_d1 = (idx-11)+ 1;

// Macros definition for variable: _event_7
#define _idx_event_7(d1) (4011+(d1-1))
#define _eval_event_7(d1) (4011+(d1-1))
#define _is_var_event_7(idx) idx >= 4011 && idx < 8011
#define _get_event_7_idxs(idx)\
		_d1 = (idx-4011)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: dtref
#define _idx_dtref 4009
#define _dtref d[_idx_dtref]

// Macros definition for variable: ierr
#define _idx_ierr(coeff) 4000
#define _state_idx_ierr(coeff) 4000*4 + coeff
#define _ierr(coeff) x[_state_idx_ierr(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_ierr(coeff) x[_state_idx_ierr(coeff)]
#define _q_ierr(coeff) q[_state_idx_ierr(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_ierr(coeff) 4000
#define _eval_dep_ierr(coeff) dx[_state_idx_ierr(coeff)]

// Macros definition for variable: inputs
#define _idx_inputs(d1) (12020+(d1-1))
#define _inputs(d1) d[_idx_inputs(d1)]

// Macros definition for variable: nextSample
#define _idx_nextSample 4011
#define _nextSample d[_idx_nextSample]

// Macros definition for variable: noise
#define _idx_noise(d1) (4016+(d1-1))
#define _noise(d1) d[_idx_noise(d1)]

// Macros definition for variable: on
#define _idx_on(d1) (9+(d1-1))
#define _on(d1) d[_idx_on(d1)]

// Macros definition for variable: partSample
#define _idx_partSample(d1) (4012+(d1-1))
#define _partSample(d1) d[_idx_partSample(d1)]

// Macros definition for variable: partTotal
#define _idx_partTotal(d1) (1+(d1-1))
#define _partTotal(d1) d[_idx_partTotal(d1)]

// Macros definition for variable: pmax
#define _pmax __PAR__pmax

// Macros definition for variable: pref
#define _idx_pref 4010
#define _pref d[_idx_pref]

// Macros definition for variable: ptotal
#define _idx_ptotal(coeff) 4001
#define _state_idx_ptotal(coeff) 4001*4 + coeff
#define _ptotal(coeff) x[_state_idx_ptotal(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_ptotal(coeff) x[_state_idx_ptotal(coeff)]
#define _q_ptotal(coeff) q[_state_idx_ptotal(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_ptotal(coeff) 4001
#define _eval_dep_ptotal(coeff) dx[_state_idx_ptotal(coeff)]

// Macros definition for variable: ptotals
#define _idx_ptotals 0
#define _ptotals d[_idx_ptotals]

// Macros definition for variable: sampleNoise
#define _idx_sampleNoise(d1) (8020+(d1-1))
#define _sampleNoise(d1) d[_idx_sampleNoise(d1)]

// Macros definition for variable: sections
#define _idx_sections(d1) ((d1-1))
#define _sections(d1) __PAR__sections[_idx_sections(d1)]

// Macros definition for variable: sections_end
#define _idx_sections_end(d1) ((d1-1))
#define _sections_end(d1) __PAR__sections_end[_idx_sections_end(d1)]

// Macros definition for variable: sections_init
#define _idx_sections_init(d1) ((d1-1))
#define _sections_init(d1) __PAR__sections_init[_idx_sections_init(d1)]

// Macros definition for variable: sendPartTotal
#define _idx_sendPartTotal(d1) (5+(d1-1))
#define _sendPartTotal(d1) d[_idx_sendPartTotal(d1)]

// Macros definition for variable: th
#define _idx_th(d1,coeff) ((d1-1))
#define _state_idx_th(d1,coeff) ((d1-1))*4 + coeff
#define _th(d1,coeff) x[_state_idx_th(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_th(d1,coeff) x[_state_idx_th(d1,coeff)]
#define _q_th(d1,coeff) q[_state_idx_th(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_th(d1,coeff) ((d1-1))
#define _is_var_th(idx) idx >= 0 && idx < 4000
#define _get_th_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_th(d1,coeff) dx[_state_idx_th(d1,coeff)]

// Macros definition for variable: tref
#define _tref __PAR__tref

// Macros definition for variable: update
#define _idx_update(d1) (8016+(d1-1))
#define _update(d1) d[_idx_update(d1)]


// Model Parameters Declaration

// Macro for parameter: CAP
double __PAR__CAP[4000];
// Macro for parameter: Ki
double __PAR__Ki;
// Macro for parameter: Kp
double __PAR__Kp;
// Macro for parameter: POT
double __PAR__POT[4000];
// Macro for parameter: RES
double __PAR__RES[4000];
// Macro for parameter: THA
double __PAR__THA;
// Macro for parameter: pmax
double __PAR__pmax;
// Macro for parameter: sections
int __PAR__sections[4000];
// Macro for parameter: sections_end
int __PAR__sections_end[4];
// Macro for parameter: sections_init
int __PAR__sections_init[4];
// Macro for parameter: tref
double __PAR__tref;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_th(_rg_d1,0);
// Macros for equation: 2

// Macros for equation: 3


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

// Macros for event: 4

// Macros for event: 5

// Macros for event: 6
#define _apply_usage_event_6(_d1) \
	i = _d1;
#define _get_event_6_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_6(_rg_d1);
// Macros for event: 7
#define _apply_usage_event_7(_d1) \
	i = _d1;
#define _get_event_7_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_7(_rg_d1);
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
// Derivative definition for variable: ierr
#define _der_ierr(coeff) dx[coeff+1]
// Derivative definition for variable: ptotal
#define _der_ptotal(coeff) dx[coeff+1]
// Derivative definition for variable: th
#define _der_th(d1,coeff) dx[coeff+1]
