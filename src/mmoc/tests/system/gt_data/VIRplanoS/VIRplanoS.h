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

// Model Variables Macros

// Macros definition for variable: A
#define _idx_A(d1,coeff) (32+(d1-1))
#define _state_idx_A(d1,coeff) (32+(d1-1))*3 + coeff
#define _A(d1,coeff) x[_state_idx_A(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_A(d1,coeff) x[_state_idx_A(d1,coeff)]
#define _q_A(d1,coeff) q[_state_idx_A(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_A(d1,coeff) (32+(d1-1))
#define _is_var_A(idx) idx >= 32 && idx < 36
#define _get_A_idxs(idx)\
		_d1 = (idx-32)+ 1;
#define _eval_dep_A(d1,coeff) dx[_state_idx_A(d1,coeff)]

// Macros definition for variable: D
#define _idx_D(d1,coeff) ((d1-1))
#define _state_idx_D(d1,coeff) ((d1-1))*3 + coeff
#define _D(d1,coeff) a[_state_idx_D(d1,coeff)]
#define _is_var_D(idx) idx >= 0 && idx < 4
#define _get_D_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: E
#define _idx_E(d1,coeff) (24+(d1-1))
#define _state_idx_E(d1,coeff) (24+(d1-1))*3 + coeff
#define _E(d1,coeff) x[_state_idx_E(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_E(d1,coeff) x[_state_idx_E(d1,coeff)]
#define _q_E(d1,coeff) q[_state_idx_E(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_E(d1,coeff) (24+(d1-1))
#define _is_var_E(idx) idx >= 24 && idx < 28
#define _get_E_idxs(idx)\
		_d1 = (idx-24)+ 1;
#define _eval_dep_E(d1,coeff) dx[_state_idx_E(d1,coeff)]

// Macros definition for variable: F
#define _idx_F(d1,coeff) (16+(d1-1))
#define _state_idx_F(d1,coeff) (16+(d1-1))*3 + coeff
#define _F(d1,coeff) x[_state_idx_F(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_F(d1,coeff) x[_state_idx_F(d1,coeff)]
#define _q_F(d1,coeff) q[_state_idx_F(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_F(d1,coeff) (16+(d1-1))
#define _is_var_F(idx) idx >= 16 && idx < 20
#define _get_F_idxs(idx)\
		_d1 = (idx-16)+ 1;
#define _eval_dep_F(d1,coeff) dx[_state_idx_F(d1,coeff)]

// Macros definition for variable: H
#define _idx_H(d1,coeff) (4+(d1-1))
#define _state_idx_H(d1,coeff) (4+(d1-1))*3 + coeff
#define _H(d1,coeff) x[_state_idx_H(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_H(d1,coeff) x[_state_idx_H(d1,coeff)]
#define _q_H(d1,coeff) q[_state_idx_H(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_H(d1,coeff) (4+(d1-1))
#define _is_var_H(idx) idx >= 4 && idx < 8
#define _get_H_idxs(idx)\
		_d1 = (idx-4)+ 1;
#define _eval_dep_H(d1,coeff) dx[_state_idx_H(d1,coeff)]

// Macros definition for variable: I
#define _idx_I(d1,coeff) (8+(d1-1))
#define _state_idx_I(d1,coeff) (8+(d1-1))*3 + coeff
#define _I(d1,coeff) x[_state_idx_I(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_I(d1,coeff) x[_state_idx_I(d1,coeff)]
#define _q_I(d1,coeff) q[_state_idx_I(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_I(d1,coeff) (8+(d1-1))
#define _is_var_I(idx) idx >= 8 && idx < 12
#define _get_I_idxs(idx)\
		_d1 = (idx-8)+ 1;
#define _eval_dep_I(d1,coeff) dx[_state_idx_I(d1,coeff)]

// Macros definition for variable: In1
#define _idx_In1 9
#define _In1 d[_idx_In1]

// Macros definition for variable: M
#define _idx_M(d1,coeff) (12+(d1-1))
#define _state_idx_M(d1,coeff) (12+(d1-1))*3 + coeff
#define _M(d1,coeff) x[_state_idx_M(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_M(d1,coeff) x[_state_idx_M(d1,coeff)]
#define _q_M(d1,coeff) q[_state_idx_M(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_M(d1,coeff) (12+(d1-1))
#define _is_var_M(idx) idx >= 12 && idx < 16
#define _get_M_idxs(idx)\
		_d1 = (idx-12)+ 1;
#define _eval_dep_M(d1,coeff) dx[_state_idx_M(d1,coeff)]

// Macros definition for variable: P
#define _idx_P(d1,coeff) (28+(d1-1))
#define _state_idx_P(d1,coeff) (28+(d1-1))*3 + coeff
#define _P(d1,coeff) x[_state_idx_P(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_P(d1,coeff) x[_state_idx_P(d1,coeff)]
#define _q_P(d1,coeff) q[_state_idx_P(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_P(d1,coeff) (28+(d1-1))
#define _is_var_P(idx) idx >= 28 && idx < 32
#define _get_P_idxs(idx)\
		_d1 = (idx-28)+ 1;
#define _eval_dep_P(d1,coeff) dx[_state_idx_P(d1,coeff)]

// Macros definition for variable: R
#define _idx_R(d1,coeff) (20+(d1-1))
#define _state_idx_R(d1,coeff) (20+(d1-1))*3 + coeff
#define _R(d1,coeff) x[_state_idx_R(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_R(d1,coeff) x[_state_idx_R(d1,coeff)]
#define _q_R(d1,coeff) q[_state_idx_R(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_R(d1,coeff) (20+(d1-1))
#define _is_var_R(idx) idx >= 20 && idx < 24
#define _get_R_idxs(idx)\
		_d1 = (idx-20)+ 1;
#define _eval_dep_R(d1,coeff) dx[_state_idx_R(d1,coeff)]

// Macros definition for variable: Rec
#define _idx_Rec 10
#define _Rec d[_idx_Rec]

// Macros definition for variable: S
#define _idx_S(d1,coeff) (36+(d1-1))
#define _state_idx_S(d1,coeff) (36+(d1-1))*3 + coeff
#define _S(d1,coeff) x[_state_idx_S(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_S(d1,coeff) x[_state_idx_S(d1,coeff)]
#define _q_S(d1,coeff) q[_state_idx_S(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_S(d1,coeff) (36+(d1-1))
#define _is_var_S(idx) idx >= 36 && idx < 40
#define _get_S_idxs(idx)\
		_d1 = (idx-36)+ 1;
#define _eval_dep_S(d1,coeff) dx[_state_idx_S(d1,coeff)]

// Macros definition for variable: Su
#define _idx_Su 8
#define _Su d[_idx_Su]

// Macros definition for variable: V
#define _idx_V(d1,coeff) ((d1-1))
#define _state_idx_V(d1,coeff) ((d1-1))*3 + coeff
#define _V(d1,coeff) x[_state_idx_V(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_V(d1,coeff) x[_state_idx_V(d1,coeff)]
#define _q_V(d1,coeff) q[_state_idx_V(d1,coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_V(d1,coeff) ((d1-1))
#define _is_var_V(idx) idx >= 0 && idx < 4
#define _get_V_idxs(idx)\
		_d1 = (idx)+ 1;
#define _eval_dep_V(d1,coeff) dx[_state_idx_V(d1,coeff)]

// Macros definition for variable: Vpar
#define _idx_Vpar(d1,coeff) (4+(d1-1))
#define _state_idx_Vpar(d1,coeff) (4+(d1-1))*3 + coeff
#define _Vpar(d1,coeff) a[_state_idx_Vpar(d1,coeff)]
#define _is_var_Vpar(idx) idx >= 4 && idx < 8
#define _get_Vpar_idxs(idx)\
		_d1 = (idx-4)+ 1;

// Macros definition for variable: Vr
#define _idx_Vr(coeff) 40
#define _state_idx_Vr(coeff) 40*3 + coeff
#define _Vr(coeff) x[_state_idx_Vr(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_Vr(coeff) x[_state_idx_Vr(coeff)]
#define _q_Vr(coeff) q[_state_idx_Vr(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_Vr(coeff) 40
#define _eval_dep_Vr(coeff) dx[_state_idx_Vr(coeff)]

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
#define _idx_event_3(d1) (8+(d1-1))
#define _eval_event_3(d1) (8+(d1-1))
#define _is_var_event_3(idx) idx >= 8 && idx < 12
#define _get_event_3_idxs(idx)\
		_d1 = (idx-8)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: _out_exp_3
#define _idx_out_exp_3 2
#define _eval_out_exp_3 2

// Macros definition for variable: _out_exp_4
#define _idx_out_exp_4 3
#define _eval_out_exp_4 3

// Macros definition for variable: _sum_0
#define _idx_sum_0(d1,coeff) (8+(d1-1))
#define _state_idx_sum_0(d1,coeff) (8+(d1-1))*3 + coeff
#define _sum_0(d1,coeff) a[_state_idx_sum_0(d1,coeff)]
#define _is_var_sum_0(idx) idx >= 8 && idx < 12
#define _get_sum_0_idxs(idx)\
		_d1 = (idx-8)+ 1;

// Macros definition for variable: aa
#define _idx_aa(d1) ((d1-1))
#define _aa(d1) __PAR__aa[_idx_aa(d1)]

// Macros definition for variable: ae
#define _idx_ae(d1) ((d1-1))
#define _ae(d1) __PAR__ae[_idx_ae(d1)]

// Macros definition for variable: af
#define _idx_af(d1) ((d1-1))
#define _af(d1) __PAR__af[_idx_af(d1)]

// Macros definition for variable: ai
#define _idx_ai(d1) ((d1-1))
#define _ai(d1) __PAR__ai[_idx_ai(d1)]

// Macros definition for variable: am
#define _idx_am(d1) ((d1-1))
#define _am(d1) __PAR__am[_idx_am(d1)]

// Macros definition for variable: ap
#define _idx_ap(d1) ((d1-1))
#define _ap(d1) __PAR__ap[_idx_ap(d1)]

// Macros definition for variable: ar
#define _idx_ar(d1) ((d1-1))
#define _ar(d1) __PAR__ar[_idx_ar(d1)]

// Macros definition for variable: av
#define _idx_av(d1) ((d1-1))
#define _av(d1) __PAR__av[_idx_av(d1)]

// Macros definition for variable: av1
#define _idx_av1(d1) ((d1-1))
#define _av1(d1) __PAR__av1[_idx_av1(d1)]

// Macros definition for variable: av2
#define _idx_av2(d1) ((d1-1))
#define _av2(d1) __PAR__av2[_idx_av2(d1)]

// Macros definition for variable: ba
#define _idx_ba(d1) ((d1-1))
#define _ba(d1) __PAR__ba[_idx_ba(d1)]

// Macros definition for variable: bei
#define _idx_bei(d1) ((d1-1))
#define _bei(d1) __PAR__bei[_idx_bei(d1)]

// Macros definition for variable: bem
#define _idx_bem(d1) ((d1-1))
#define _bem(d1) __PAR__bem[_idx_bem(d1)]

// Macros definition for variable: bf
#define _idx_bf(d1) ((d1-1))
#define _bf(d1) __PAR__bf[_idx_bf(d1)]

// Macros definition for variable: bfh
#define _idx_bfh(d1) ((d1-1))
#define _bfh(d1) __PAR__bfh[_idx_bfh(d1)]

// Macros definition for variable: bhd
#define _idx_bhd(d1) ((d1-1))
#define _bhd(d1) __PAR__bhd[_idx_bhd(d1)]

// Macros definition for variable: bhf
#define _idx_bhf(d1) ((d1-1))
#define _bhf(d1) __PAR__bhf[_idx_bhf(d1)]

// Macros definition for variable: bie
#define _idx_bie(d1) ((d1-1))
#define _bie(d1) __PAR__bie[_idx_bie(d1)]

// Macros definition for variable: bmd
#define _idx_bmd(d1) ((d1-1))
#define _bmd(d1) __PAR__bmd[_idx_bmd(d1)]

// Macros definition for variable: bmv
#define _idx_bmv(d1) ((d1-1))
#define _bmv(d1) __PAR__bmv[_idx_bmv(d1)]

// Macros definition for variable: bpm
#define _idx_bpm(d1) ((d1-1))
#define _bpm(d1) __PAR__bpm[_idx_bpm(d1)]

// Macros definition for variable: cf
#define _idx_cf(d1) ((d1-1))
#define _cf(d1) __PAR__cf[_idx_cf(d1)]

// Macros definition for variable: cr
#define _cr __PAR__cr

// Macros definition for variable: day
#define _idx_day(d1) (12+(d1-1))
#define _day(d1) d[_idx_day(d1)]

// Macros definition for variable: gav
#define _idx_gav(d1) ((d1-1))
#define _gav(d1) __PAR__gav[_idx_gav(d1)]

// Macros definition for variable: ghv
#define _idx_ghv(d1) ((d1-1))
#define _ghv(d1) __PAR__ghv[_idx_ghv(d1)]

// Macros definition for variable: gv
#define _idx_gv(d1) ((d1-1))
#define _gv(d1) __PAR__gv[_idx_gv(d1)]

// Macros definition for variable: gva
#define _idx_gva(d1) ((d1-1))
#define _gva(d1) __PAR__gva[_idx_gva(d1)]

// Macros definition for variable: gvh
#define _idx_gvh(d1) ((d1-1))
#define _gvh(d1) __PAR__gvh[_idx_gvh(d1)]

// Macros definition for variable: inside
#define _idx_inside(d1) ((d1-1))
#define _inside(d1) d[_idx_inside(d1)]

// Macros definition for variable: inst
#define _idx_inst(d1) (4+(d1-1))
#define _inst(d1) d[_idx_inst(d1)]

// Macros definition for variable: r
#define _idx_r(d1) ((d1-1))
#define _r(d1) __PAR__r[_idx_r(d1)]

// Macros definition for variable: re
#define _idx_re(d1) ((d1-1))
#define _re(d1) __PAR__re[_idx_re(d1)]

// Macros definition for variable: tr
#define _idx_tr(d1) ((d1-1))
#define _tr(d1) __PAR__tr[_idx_tr(d1)]

// Macros definition for variable: u
#define _idx_u 11
#define _u d[_idx_u]


// Model Parameters Declaration

// Macro for parameter: aa
double __PAR__aa[4];
// Macro for parameter: ae
double __PAR__ae[4];
// Macro for parameter: af
double __PAR__af[4];
// Macro for parameter: ai
double __PAR__ai[4];
// Macro for parameter: am
double __PAR__am[4];
// Macro for parameter: ap
double __PAR__ap[4];
// Macro for parameter: ar
double __PAR__ar[4];
// Macro for parameter: av
double __PAR__av[4];
// Macro for parameter: av1
double __PAR__av1[4];
// Macro for parameter: av2
double __PAR__av2[4];
// Macro for parameter: ba
double __PAR__ba[4];
// Macro for parameter: bei
double __PAR__bei[4];
// Macro for parameter: bem
double __PAR__bem[4];
// Macro for parameter: bf
double __PAR__bf[4];
// Macro for parameter: bfh
double __PAR__bfh[4];
// Macro for parameter: bhd
double __PAR__bhd[4];
// Macro for parameter: bhf
double __PAR__bhf[4];
// Macro for parameter: bie
double __PAR__bie[4];
// Macro for parameter: bmd
double __PAR__bmd[4];
// Macro for parameter: bmv
double __PAR__bmv[4];
// Macro for parameter: bpm
double __PAR__bpm[4];
// Macro for parameter: cf
double __PAR__cf[4];
// Macro for parameter: cr
double __PAR__cr;
// Macro for parameter: gav
double __PAR__gav[4];
// Macro for parameter: ghv
double __PAR__ghv[4];
// Macro for parameter: gv
double __PAR__gv[4];
// Macro for parameter: gva
double __PAR__gva[4];
// Macro for parameter: gvh
double __PAR__gvh[4];
// Macro for parameter: r
double __PAR__r[4];
// Macro for parameter: re
double __PAR__re[4];
// Macro for parameter: tr
double __PAR__tr[4];

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_V(_rg_d1,0);
// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_H(_rg_d1,0);
// Macros for equation: 3
#define _apply_usage_eq_3(_d1) \
	i = _d1;
#define _get_eq_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_I(_rg_d1,0);
// Macros for equation: 4
#define _apply_usage_eq_4(_d1) \
	i = _d1;
#define _get_eq_4_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_M(_rg_d1,0);
// Macros for equation: 5
#define _apply_usage_eq_5(_d1) \
	i = _d1;
#define _get_eq_5_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_F(_rg_d1,0);
// Macros for equation: 6
#define _apply_usage_eq_6(_d1) \
	i = _d1;
#define _get_eq_6_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_R(_rg_d1,0);
// Macros for equation: 7
#define _apply_usage_eq_7(_d1) \
	i = _d1;
#define _get_eq_7_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_E(_rg_d1,0);
// Macros for equation: 8
#define _apply_usage_eq_8(_d1) \
	i = _d1;
#define _get_eq_8_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_P(_rg_d1,0);
// Macros for equation: 9
#define _apply_usage_eq_9(_d1) \
	i = _d1;
#define _get_eq_9_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_A(_rg_d1,0);
// Macros for equation: 10
#define _apply_usage_eq_10(_d1) \
	i = _d1;
#define _get_eq_10_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_S(_rg_d1,0);
// Macros for equation: 11


// Algebraic Equations Macros

// Macros for algebraic equation: 1
#define _apply_usage_alg_eq_1(_d1) \
	i = _d1;
#define _get_alg_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_D(_rg_d1,0);
// Macros for algebraic equation: 2
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_Vpar(_rg_d1,0);
// Macros for algebraic equation: 3

// Macros for algebraic equation: 4
#define _apply_usage_alg_eq_4(_d1) \
	i = _d1;
#define _get_alg_eq_4_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_sum_0(_rg_d1,0);

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
#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1

// Macros for output equation: 2

// Macros for output equation: 3

// Macros for output equation: 4

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: A
#define _der_A(d1,coeff) dx[coeff+1]
// Derivative definition for variable: E
#define _der_E(d1,coeff) dx[coeff+1]
// Derivative definition for variable: F
#define _der_F(d1,coeff) dx[coeff+1]
// Derivative definition for variable: H
#define _der_H(d1,coeff) dx[coeff+1]
// Derivative definition for variable: I
#define _der_I(d1,coeff) dx[coeff+1]
// Derivative definition for variable: M
#define _der_M(d1,coeff) dx[coeff+1]
// Derivative definition for variable: P
#define _der_P(d1,coeff) dx[coeff+1]
// Derivative definition for variable: R
#define _der_R(d1,coeff) dx[coeff+1]
// Derivative definition for variable: S
#define _der_S(d1,coeff) dx[coeff+1]
// Derivative definition for variable: V
#define _der_V(d1,coeff) dx[coeff+1]
// Derivative definition for variable: Vr
#define _der_Vr(coeff) dx[coeff+1]
