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
#define _idx_C(d1,d2) ((d1-1)*5+(d2-1))
#define _C(d1,d2) __PAR__C[_idx_C(d1,d2)]
#define _Cm __PAR__Cm
#define _idx_Cn(d1) ((d1-1))
#define _Cn(d1) __PAR__Cn[_idx_Cn(d1)]
#define _EL __PAR__EL
#define _idx_Is(d1,coeff) (10+(d1-1))
#define _state_idx_Is(d1,coeff) (10+(d1-1))*3 + coeff
#define _Is(d1,coeff) x[_state_idx_Is(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_Is(d1,coeff) x[_state_idx_Is(d1,coeff)]
#define _eval_Is(d1,coeff) (10+(d1-1))
#define _is_var_Is(idx) idx >= 10 && idx < 20
#define _get_Is_idxs(idx)\
		_d1 = (idx-10)+ 1;
#define _eval_dep_Is(d1,coeff) dx[_state_idx_Is(d1,coeff)]
#define _idx_J(d1) ((d1-1))
#define _J(d1) __PAR__J[_idx_J(d1)]
#define _Jmean __PAR__Jmean
#define _idx_V(d1,coeff) ((d1-1))
#define _state_idx_V(d1,coeff) ((d1-1))*3 + coeff
#define _V(d1,coeff) x[_state_idx_V(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_V(d1,coeff) x[_state_idx_V(d1,coeff)]
#define _eval_V(d1,coeff) ((d1-1))
#define _is_var_V(idx) idx >= 0 && idx < 10
#define _get_V_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_V(d1,coeff) dx[_state_idx_V(d1,coeff)]
#define _Vr __PAR__Vr
#define _idx_event_1 0
#define _eval_event_1 0
#define _idx_event_2(d1) (1+(d1-1))
#define _eval_event_2(d1) (1+(d1-1))
#define _is_var_event_2(idx) idx >= 1 && idx < 11
#define _get_event_2_idxs(idx)\
		_d1 = (idx-1)+ 1;
#define _idx_event_3(d1) (11+(d1-1))
#define _eval_event_3(d1) (11+(d1-1))
#define _is_var_event_3(idx) idx >= 11 && idx < 21
#define _get_event_3_idxs(idx)\
		_d1 = (idx-11)+ 1;
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1
#define _idx_active(d1) (11+(d1-1))
#define _active(d1) d[_idx_active(d1)]
#define _dt __PAR__dt
#define _kext __PAR__kext
#define _mu __PAR__mu
#define _sigma __PAR__sigma
#define _taum __PAR__taum
#define _taur __PAR__taur
#define _taus __PAR__taus
#define _theta __PAR__theta
#define _idx_tnext 0
#define _tnext d[_idx_tnext]
#define _idx_tnextr(d1) (1+(d1-1))
#define _tnextr(d1) d[_idx_tnextr(d1)]
#define _vbg __PAR__vbg
#define _vext __PAR__vext
#define _idx_xi(d1) (21+(d1-1))
#define _xi(d1) d[_idx_xi(d1)]

// Model Parameters Declaration
int __PAR__C[50];
double __PAR__Cm;
int __PAR__Cn[10];
double __PAR__EL;
double __PAR__J[10];
double __PAR__Jmean;
double __PAR__Vr;
double __PAR__dt;
double __PAR__kext;
double __PAR__mu;
double __PAR__sigma;
double __PAR__taum;
double __PAR__taur;
double __PAR__taus;
double __PAR__theta;
double __PAR__vbg;
double __PAR__vext;

// Derivative Equations Macros
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_V(_rg_d1,0);
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_Is(_rg_d1,0);

// Algebraic Equations Macros

// Event Macros
#define _apply_usage_event_2(_d1) \
	i = _d1;
#define _get_event_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_2(_rg_d1);
#define _apply_usage_event_3(_d1) \
	i = _d1;
#define _get_event_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_event_3(_rg_d1);
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
#define _der_Is(d1,coeff) dx[coeff+1]
#define _der_V(d1,coeff) dx[coeff+1]
