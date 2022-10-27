// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x;          \
  double* d = m->d;          \
  double* a = m->alg;

// Coeff multipliers definition.

#define COEFF_MULTIPLIER(c) COEFF_MULTIPLIER_##c
#define COEFF_MULTIPLIER_0 1
#define COEFF_MULTIPLIER_1 1
#define COEFF_MULTIPLIER_2 2

// Model Variables Macros

// Macros definition for variable: C
#define _idx_C(d1, d2) ((d1 - 1) * 20 + (d2 - 1))
#define _C(d1, d2) __PAR__C[_idx_C(d1, d2)]

// Macros definition for variable: Cm
#define _Cm __PAR__Cm

// Macros definition for variable: Cn
#define _idx_Cn(d1) ((d1 - 1))
#define _Cn(d1) __PAR__Cn[_idx_Cn(d1)]

// Macros definition for variable: EL
#define _EL __PAR__EL

// Macros definition for variable: Is
#define _idx_Is(d1, coeff) ((d1 - 1))
#define _state_idx_Is(d1, coeff) ((d1 - 1)) * 3 + coeff
#define _Is(d1, coeff) a[_state_idx_Is(d1, coeff)]
#define _is_var_Is(idx) idx >= 0 && idx < 100
#define _get_Is_idxs(idx) _d1 = (idx) + 1;

// Macros definition for variable: Islast
#define _idx_Islast(d1) (300 + (d1 - 1))
#define _Islast(d1) d[_idx_Islast(d1)]

// Macros definition for variable: J
#define _idx_J(d1) ((d1 - 1))
#define _J(d1) __PAR__J[_idx_J(d1)]

// Macros definition for variable: Jmean
#define _Jmean __PAR__Jmean

// Macros definition for variable: Ne
#define _Ne __PAR__Ne

// Macros definition for variable: Ni
#define _Ni __PAR__Ni

// Macros definition for variable: V
#define _idx_V(d1, coeff) ((d1 - 1))
#define _state_idx_V(d1, coeff) ((d1 - 1)) * 3 + coeff
#define _V(d1, coeff) x[_state_idx_V(d1, coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_V(d1, coeff) x[_state_idx_V(d1, coeff)]
#define _eval_V(d1, coeff) ((d1 - 1))
#define _is_var_V(idx) idx >= 0 && idx < 100
#define _get_V_idxs(idx) _d1 = (idx) + 1;
#define _eval_dep_V(d1, coeff) dx[_state_idx_V(d1, coeff)]

// Macros definition for variable: Vr
#define _Vr __PAR__Vr

// Macros definition for variable: _event_1
#define _idx_event_1(d1) ((d1 - 1))
#define _eval_event_1(d1) ((d1 - 1))
#define _is_var_event_1(idx) idx >= 0 && idx < 100
#define _get_event_1_idxs(idx) _d1 = (idx) + 1;

// Macros definition for variable: _event_2
#define _idx_event_2(d1) (100 + (d1 - 1))
#define _eval_event_2(d1) (100 + (d1 - 1))
#define _is_var_event_2(idx) idx >= 100 && idx < 200
#define _get_event_2_idxs(idx) _d1 = (idx - 100) + 1;

// Macros definition for variable: _event_3
#define _idx_event_3(d1) (200 + (d1 - 1))
#define _eval_event_3(d1) (200 + (d1 - 1))
#define _is_var_event_3(idx) idx >= 200 && idx < 300
#define _get_event_3_idxs(idx) _d1 = (idx - 200) + 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1(d1) ((d1 - 1))
#define _eval_out_exp_1(d1) ((d1 - 1))
#define _is_var_out_exp_1(idx) idx >= 0 && idx < 10
#define _get_out_exp_1_idxs(idx) _d1 = (idx) + 1;

// Macros definition for variable: active
#define _idx_active(d1) (200 + (d1 - 1))
#define _active(d1) d[_idx_active(d1)]

// Macros definition for variable: aux
#define _aux __PAR__aux

// Macros definition for variable: cI
#define _cI __PAR__cI

// Macros definition for variable: cV
#define _cV __PAR__cV

// Macros definition for variable: ce
#define _ce __PAR__ce

// Macros definition for variable: ci
#define _ci __PAR__ci

// Macros definition for variable: g
#define _g __PAR__g

// Macros definition for variable: kext
#define _kext __PAR__kext

// Macros definition for variable: taum
#define _taum __PAR__taum

// Macros definition for variable: taur
#define _taur __PAR__taur

// Macros definition for variable: taus
#define _taus __PAR__taus

// Macros definition for variable: theta
#define _theta __PAR__theta

// Macros definition for variable: tlast
#define _idx_tlast(d1) (400 + (d1 - 1))
#define _tlast(d1) d[_idx_tlast(d1)]

// Macros definition for variable: tnext
#define _idx_tnext(d1) ((d1 - 1))
#define _tnext(d1) d[_idx_tnext(d1)]

// Macros definition for variable: tnextr
#define _idx_tnextr(d1) (100 + (d1 - 1))
#define _tnextr(d1) d[_idx_tnextr(d1)]

// Macros definition for variable: vbg
#define _vbg __PAR__vbg

// Macros definition for variable: vext
#define _vext __PAR__vext

// Model Parameters Declaration

// Macro for parameter: C
int __PAR__C[2000];
// Macro for parameter: Cm
double __PAR__Cm;
// Macro for parameter: Cn
int __PAR__Cn[100];
// Macro for parameter: EL
double __PAR__EL;
// Macro for parameter: J
double __PAR__J[100];
// Macro for parameter: Jmean
double __PAR__Jmean;
// Macro for parameter: Ne
int __PAR__Ne;
// Macro for parameter: Ni
int __PAR__Ni;
// Macro for parameter: Vr
double __PAR__Vr;
// Macro for parameter: aux
int __PAR__aux;
// Macro for parameter: cI
double __PAR__cI;
// Macro for parameter: cV
double __PAR__cV;
// Macro for parameter: ce
int __PAR__ce;
// Macro for parameter: ci
int __PAR__ci;
// Macro for parameter: g
double __PAR__g;
// Macro for parameter: kext
double __PAR__kext;
// Macro for parameter: taum
double __PAR__taum;
// Macro for parameter: taur
double __PAR__taur;
// Macro for parameter: taus
double __PAR__taus;
// Macro for parameter: theta
double __PAR__theta;
// Macro for parameter: vbg
double __PAR__vbg;
// Macro for parameter: vext
double __PAR__vext;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) i = _d1;
#define _get_eq_1_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;        \
  var = _idx_V(_rg_d1, 0);

// Algebraic Equations Macros

// Macros for algebraic equation: 1
#define _apply_usage_alg_eq_1(_d1) i = _d1;
#define _get_alg_eq_1_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;            \
  var = _idx_Is(_rg_d1, 0);

// Event Macros

// Macros for event: 1
#define _apply_usage_event_1(_d1) i = _d1;
#define _get_event_1_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;           \
  var = _idx_event_1(_rg_d1);
// Macros for event: 2
#define _apply_usage_event_2(_d1) i = _d1;
#define _get_event_2_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;           \
  var = _idx_event_2(_rg_d1);
// Macros for event: 3
#define _apply_usage_event_3(_d1) i = _d1;
#define _get_event_3_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;           \
  var = _idx_event_3(_rg_d1);
#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1
#define _apply_usage_out_exp_1(_d1) _d1 = _d1;
#define _get_out_exp_1_var_idxs(row, var) \
  _rg_d1 = 0 + (row - 1) + 1;             \
  var = _idx_out_exp_1(_rg_d1);
#define _out out[0]

// Jacobian Macros definition.
#define _assign_jac(r, val)                                    \
  col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
  dvdx->df_dx_t->index[r][0]++;                                \
  jac[col_t] = val;
#define _c_index(i) (i - 1)

#define _time t

// Derivative Macros definition.
// Derivative definition for variable: V
#define _der_V(d1, coeff) dx[coeff + 1]
