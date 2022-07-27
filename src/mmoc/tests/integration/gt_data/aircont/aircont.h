// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

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
#define _is_var_event_1(idx) idx >= 0 && idx < 200
#define _get_event_1_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: _event_2
#define _idx_event_2 200
#define _eval_event_2 200

// Macros definition for variable: _event_3
#define _idx_event_3 201
#define _eval_event_3 201

// Macros definition for variable: _event_4
#define _idx_event_4 202
#define _eval_event_4 202

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: dtref
#define _idx_dtref 202
#define _dtref d[_idx_dtref]

// Macros definition for variable: ierr
#define _idx_ierr 200
#define _ierr x[_idx_ierr]
#define _init_ierr x[_idx_ierr]
#define _eval_ierr 200

// Macros definition for variable: nextSample
#define _idx_nextSample 204
#define _nextSample d[_idx_nextSample]

// Macros definition for variable: noise
#define _idx_noise 205
#define _noise d[_idx_noise]

// Macros definition for variable: on
#define _idx_on(d1) (2+(d1-1))
#define _on(d1) d[_idx_on(d1)]

// Macros definition for variable: pmax
#define _pmax __PAR__pmax

// Macros definition for variable: pref
#define _idx_pref 203
#define _pref d[_idx_pref]

// Macros definition for variable: ptotal
#define _idx_ptotal 0
#define _ptotal d[_idx_ptotal]

// Macros definition for variable: ptotals
#define _idx_ptotals 1
#define _ptotals d[_idx_ptotals]

// Macros definition for variable: th
#define _idx_th(d1) ((d1-1))
#define _th(d1) x[_idx_th(d1)]
#define _init_th(d1) x[_idx_th(d1)]
#define _eval_th(d1) ((d1-1))
#define _is_var_th(idx) idx >= 0 && idx < 200
#define _get_th_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: tref
#define _tref __PAR__tref


// Model Parameters Declaration

// Macro for parameter: CAP
double __PAR__CAP[200];
// Macro for parameter: Ki
double __PAR__Ki;
// Macro for parameter: Kp
double __PAR__Kp;
// Macro for parameter: POT
double __PAR__POT[200];
// Macro for parameter: RES
double __PAR__RES[200];
// Macro for parameter: THA
double __PAR__THA;
// Macro for parameter: pmax
double __PAR__pmax;
// Macro for parameter: tref
double __PAR__tref;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_th(_rg_d1);
// Macros for equation: 2


// Event Macros

// Macros for event: 1
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _get_event_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_1(_rg_d1);
// Macros for event: 2

// Macros for event: 3

// Macros for event: 4

#define _zc zc[0]

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
#define _der_ierr dx[200]
// Derivative definition for variable: th
#define _der_th(d1) dx[((d1-1))]
