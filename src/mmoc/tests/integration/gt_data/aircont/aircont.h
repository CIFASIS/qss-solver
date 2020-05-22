// Model data access macro.
#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* d = m->d;

// Model Variables and Parameters Macros
#define _idx_CAP(d1) ((d1-1))
#define _CAP(d1) __PAR__CAP[_idx_CAP(d1)]
#define _Ki __PAR__Ki
#define _Kp __PAR__Kp
#define _idx_POT(d1) ((d1-1))
#define _POT(d1) __PAR__POT[_idx_POT(d1)]
#define _idx_RES(d1) ((d1-1))
#define _RES(d1) __PAR__RES[_idx_RES(d1)]
#define _THA __PAR__THA
#define _idx_event_1(d1) ((d1-1))
#define _eval_event_1(d1) ((d1-1))
#define _is_var_event_1(idx) idx >= 0 && idx < 200
#define _get_event_1_idxs(idx) \
		_d1 = (idx-0)+ 1;
#define _idx_event_2 200
#define _eval_event_2 200
#define _idx_event_3 201
#define _eval_event_3 201
#define _idx_event_4 202
#define _eval_event_4 202
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_dtref 202
#define _dtref d[_idx_dtref]
#define _idx_ierr 200
#define _ierr x[_idx_ierr]
#define _init_ierr x[_idx_ierr]
#define _eval_ierr 200
#define _idx_nextSample 204
#define _nextSample d[_idx_nextSample]
#define _idx_noise 205
#define _noise d[_idx_noise]
#define _idx_on(d1) (2+(d1-1))
#define _on(d1) d[_idx_on(d1)]
#define _pmax __PAR__pmax
#define _idx_pref 203
#define _pref d[_idx_pref]
#define _idx_ptotal 0
#define _ptotal d[_idx_ptotal]
#define _idx_ptotals 1
#define _ptotals d[_idx_ptotals]
#define _idx_th(d1) ((d1-1))
#define _th(d1) x[_idx_th(d1)]
#define _init_th(d1) x[_idx_th(d1)]
#define _eval_th(d1) ((d1-1))
#define _is_var_th(idx) idx >= 0 && idx < 200
#define _get_th_idxs(idx) \
		_d1 = (idx-0)+ 1;
#define _tref __PAR__tref

// Model Parameters Declaration
double __PAR__CAP[200];
double __PAR__Ki;
double __PAR__Kp;
double __PAR__POT[200];
double __PAR__RES[200];
double __PAR__THA;
double __PAR__pmax;
double __PAR__tref;
#define _apply_usage_eq_1(_d1) \
	i = _d1;

// Algebraic Equations Macros

// Event Macros
#define _apply_usage_event_1(_d1) \
	i = _d1;
#define _zc zc[0]

// Output Equations Macros
#define _out out[0]

// Input Matrix Macros

// Jacobian Macros definition. 
#define _jac(i) jac[i++]

#define _time t

// Derivative Macros definition. 
#define _der_ierr dx[200]
#define _der_th(d1) dx[((d1-1))]
