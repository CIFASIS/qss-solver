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

// Macros definition for variable: C
#define _C __PAR__C

// Macros definition for variable: DC
#define _idx_DC 1
#define _DC d[_idx_DC]

// Macros definition for variable: K
#define _idx_K(coeff) 0
#define _state_idx_K(coeff) 0*3 + coeff
#define _K(coeff) a[_state_idx_K(coeff)]

// Macros definition for variable: Kadj
#define _idx_Kadj 8
#define _Kadj d[_idx_Kadj]

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: R
#define _idx_R 0
#define _R d[_idx_R]

// Macros definition for variable: ROff
#define _ROff __PAR__ROff

// Macros definition for variable: ROn
#define _ROn __PAR__ROn

// Macros definition for variable: Rd
#define _idx_Rd 2
#define _Rd d[_idx_Rd]

// Macros definition for variable: Rs
#define _idx_Rs 3
#define _Rs d[_idx_Rs]

// Macros definition for variable: T
#define _T __PAR__T

// Macros definition for variable: U
#define _U __PAR__U

// Macros definition for variable: Vd
#define _idx_Vd(coeff) 3
#define _state_idx_Vd(coeff) 3*3 + coeff
#define _Vd(coeff) a[_state_idx_Vd(coeff)]

// Macros definition for variable: Vsw
#define _idx_Vsw(coeff) 4
#define _state_idx_Vsw(coeff) 4*3 + coeff
#define _Vsw(coeff) a[_state_idx_Vsw(coeff)]

// Macros definition for variable: _event_1
#define _idx_event_1 0
#define _eval_event_1 0

// Macros definition for variable: _event_10
#define _idx_event_10 9
#define _eval_event_10 9

// Macros definition for variable: _event_11
#define _idx_event_11 10
#define _eval_event_11 10

// Macros definition for variable: _event_12
#define _idx_event_12 11
#define _eval_event_12 11

// Macros definition for variable: _event_13
#define _idx_event_13 12
#define _eval_event_13 12

// Macros definition for variable: _event_14
#define _idx_event_14 13
#define _eval_event_14 13

// Macros definition for variable: _event_2
#define _idx_event_2 1
#define _eval_event_2 1

// Macros definition for variable: _event_3
#define _idx_event_3 2
#define _eval_event_3 2

// Macros definition for variable: _event_4
#define _idx_event_4 3
#define _eval_event_4 3

// Macros definition for variable: _event_5
#define _idx_event_5 4
#define _eval_event_5 4

// Macros definition for variable: _event_6
#define _idx_event_6 5
#define _eval_event_6 5

// Macros definition for variable: _event_7
#define _idx_event_7 6
#define _eval_event_7 6

// Macros definition for variable: _event_8
#define _idx_event_8 7
#define _eval_event_8 7

// Macros definition for variable: _event_9
#define _idx_event_9 8
#define _eval_event_9 8

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: _out_exp_3
#define _idx_out_exp_3 2
#define _eval_out_exp_3 2

// Macros definition for variable: alpha
#define _alpha __PAR__alpha

// Macros definition for variable: avDiVoltage
#define _idx_avDiVoltage(coeff) 6
#define _state_idx_avDiVoltage(coeff) 6*3 + coeff
#define _avDiVoltage(coeff) x[_state_idx_avDiVoltage(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_avDiVoltage(coeff) x[_state_idx_avDiVoltage(coeff)]
#define _q_avDiVoltage(coeff) q[_state_idx_avDiVoltage(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_avDiVoltage(coeff) 6
#define _eval_dep_avDiVoltage(coeff) dx[_state_idx_avDiVoltage(coeff)]

// Macros definition for variable: avDiVoltageDisc
#define _idx_avDiVoltageDisc 26
#define _avDiVoltageDisc d[_idx_avDiVoltageDisc]

// Macros definition for variable: avSwVoltage
#define _idx_avSwVoltage(coeff) 5
#define _state_idx_avSwVoltage(coeff) 5*3 + coeff
#define _avSwVoltage(coeff) x[_state_idx_avSwVoltage(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_avSwVoltage(coeff) x[_state_idx_avSwVoltage(coeff)]
#define _q_avSwVoltage(coeff) q[_state_idx_avSwVoltage(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_avSwVoltage(coeff) 5
#define _eval_dep_avSwVoltage(coeff) dx[_state_idx_avSwVoltage(coeff)]

// Macros definition for variable: avSwVoltageDisc
#define _idx_avSwVoltageDisc 25
#define _avSwVoltageDisc d[_idx_avSwVoltageDisc]

// Macros definition for variable: dc
#define _idx_dc(coeff) 0
#define _state_idx_dc(coeff) 0*3 + coeff
#define _dc(coeff) x[_state_idx_dc(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_dc(coeff) x[_state_idx_dc(coeff)]
#define _q_dc(coeff) q[_state_idx_dc(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_dc(coeff) 0
#define _eval_dep_dc(coeff) dx[_state_idx_dc(coeff)]

// Macros definition for variable: deltaDC
#define _deltaDC __PAR__deltaDC

// Macros definition for variable: diodeon
#define _idx_diodeon 6
#define _diodeon d[_idx_diodeon]

// Macros definition for variable: dutyCycle
#define _dutyCycle __PAR__dutyCycle

// Macros definition for variable: iD
#define _idx_iD(coeff) 1
#define _state_idx_iD(coeff) 1*3 + coeff
#define _iD(coeff) a[_state_idx_iD(coeff)]

// Macros definition for variable: iL
#define _idx_iL(coeff) 2
#define _state_idx_iL(coeff) 2*3 + coeff
#define _iL(coeff) x[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_iL(coeff) x[_state_idx_iL(coeff)]
#define _q_iL(coeff) q[_state_idx_iL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_iL(coeff) 2
#define _eval_dep_iL(coeff) dx[_state_idx_iL(coeff)]

// Macros definition for variable: lastDC
#define _idx_lastDC 27
#define _lastDC d[_idx_lastDC]

// Macros definition for variable: lastIL
#define _idx_lastIL 19
#define _lastIL d[_idx_lastIL]

// Macros definition for variable: lastMeanILdisc
#define _idx_lastMeanILdisc 18
#define _lastMeanILdisc d[_idx_lastMeanILdisc]

// Macros definition for variable: lastMeanUCdisc
#define _idx_lastMeanUCdisc 22
#define _lastMeanUCdisc d[_idx_lastMeanUCdisc]

// Macros definition for variable: lastT
#define _idx_lastT 5
#define _lastT d[_idx_lastT]

// Macros definition for variable: lastTaverage
#define _idx_lastTaverage 16
#define _lastTaverage d[_idx_lastTaverage]

// Macros definition for variable: lastUC
#define _idx_lastUC 23
#define _lastUC d[_idx_lastUC]

// Macros definition for variable: meanIL
#define _idx_meanIL(coeff) 3
#define _state_idx_meanIL(coeff) 3*3 + coeff
#define _meanIL(coeff) x[_state_idx_meanIL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_meanIL(coeff) x[_state_idx_meanIL(coeff)]
#define _q_meanIL(coeff) q[_state_idx_meanIL(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_meanIL(coeff) 3
#define _eval_dep_meanIL(coeff) dx[_state_idx_meanIL(coeff)]

// Macros definition for variable: meanILdiff
#define _idx_meanILdiff 20
#define _meanILdiff d[_idx_meanILdiff]

// Macros definition for variable: meanILdisc
#define _idx_meanILdisc 17
#define _meanILdisc d[_idx_meanILdisc]

// Macros definition for variable: meanUC
#define _idx_meanUC(coeff) 4
#define _state_idx_meanUC(coeff) 4*3 + coeff
#define _meanUC(coeff) x[_state_idx_meanUC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_meanUC(coeff) x[_state_idx_meanUC(coeff)]
#define _q_meanUC(coeff) q[_state_idx_meanUC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_meanUC(coeff) 4
#define _eval_dep_meanUC(coeff) dx[_state_idx_meanUC(coeff)]

// Macros definition for variable: meanUCdiff
#define _idx_meanUCdiff 24
#define _meanUCdiff d[_idx_meanUCdiff]

// Macros definition for variable: meanUCdisc
#define _idx_meanUCdisc 21
#define _meanUCdisc d[_idx_meanUCdisc]

// Macros definition for variable: modSEL
#define _idx_modSEL 11
#define _modSEL d[_idx_modSEL]

// Macros definition for variable: nextT
#define _idx_nextT 4
#define _nextT d[_idx_nextT]

// Macros definition for variable: ptedc
#define _idx_ptedc 7
#define _ptedc d[_idx_ptedc]

// Macros definition for variable: rUc
#define _idx_rUc 10
#define _rUc d[_idx_rUc]

// Macros definition for variable: riL
#define _idx_riL 9
#define _riL d[_idx_riL]

// Macros definition for variable: s
#define _idx_s(coeff) 2
#define _state_idx_s(coeff) 2*3 + coeff
#define _s(coeff) a[_state_idx_s(coeff)]

// Macros definition for variable: sigma
#define _sigma __PAR__sigma

// Macros definition for variable: toAvEnableI
#define _idx_toAvEnableI 12
#define _toAvEnableI d[_idx_toAvEnableI]

// Macros definition for variable: toAvEnableU
#define _idx_toAvEnableU 13
#define _toAvEnableU d[_idx_toAvEnableU]

// Macros definition for variable: toAverageEnabled
#define _idx_toAverageEnabled 14
#define _toAverageEnabled d[_idx_toAverageEnabled]

// Macros definition for variable: toSwEnabled
#define _idx_toSwEnabled 15
#define _toSwEnabled d[_idx_toSwEnabled]

// Macros definition for variable: toSwNextT
#define _idx_toSwNextT 28
#define _toSwNextT d[_idx_toSwNextT]

// Macros definition for variable: uC
#define _idx_uC(coeff) 1
#define _state_idx_uC(coeff) 1*3 + coeff
#define _uC(coeff) x[_state_idx_uC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _init_uC(coeff) x[_state_idx_uC(coeff)]
#define _q_uC(coeff) q[_state_idx_uC(coeff)] * COEFF_MULTIPLIER(coeff)
#define _eval_uC(coeff) 1
#define _eval_dep_uC(coeff) dx[_state_idx_uC(coeff)]


// Model Parameters Declaration

// Macro for parameter: C
double __PAR__C;
// Macro for parameter: L
double __PAR__L;
// Macro for parameter: ROff
double __PAR__ROff;
// Macro for parameter: ROn
double __PAR__ROn;
// Macro for parameter: T
double __PAR__T;
// Macro for parameter: U
double __PAR__U;
// Macro for parameter: alpha
double __PAR__alpha;
// Macro for parameter: deltaDC
double __PAR__deltaDC;
// Macro for parameter: dutyCycle
double __PAR__dutyCycle;
// Macro for parameter: sigma
double __PAR__sigma;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3

// Macros for equation: 4

// Macros for equation: 5

// Macros for equation: 6

// Macros for equation: 7


// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2

// Macros for algebraic equation: 3

// Macros for algebraic equation: 4

// Macros for algebraic equation: 5


// Event Macros

// Macros for event: 1

// Macros for event: 2

// Macros for event: 3

// Macros for event: 4

// Macros for event: 5

// Macros for event: 6

// Macros for event: 7

// Macros for event: 8

// Macros for event: 9

// Macros for event: 10

// Macros for event: 11

// Macros for event: 12

// Macros for event: 13

// Macros for event: 14

#define _zc(coeff) zc[coeff]

// Output Equations Macros

// Macros for output equation: 1

// Macros for output equation: 2

// Macros for output equation: 3

#define _out out[0]

// Jacobian Macros definition. 
#define _assign_jac(r, val) \
    col_t = dvdx->df_dx_t->size[r] + dvdx->df_dx_t->index[r][0]; \
    dvdx->df_dx_t->index[r][0]++; \
    jac[col_t] = val;
#define _c_index(i) (i-1)

#define _time t

// Derivative Macros definition. 
// Derivative definition for variable: avDiVoltage
#define _der_avDiVoltage(coeff) dx[coeff+1]
// Derivative definition for variable: avSwVoltage
#define _der_avSwVoltage(coeff) dx[coeff+1]
// Derivative definition for variable: dc
#define _der_dc(coeff) dx[coeff+1]
// Derivative definition for variable: iL
#define _der_iL(coeff) dx[coeff+1]
// Derivative definition for variable: meanIL
#define _der_meanIL(coeff) dx[coeff+1]
// Derivative definition for variable: meanUC
#define _der_meanUC(coeff) dx[coeff+1]
// Derivative definition for variable: uC
#define _der_uC(coeff) dx[coeff+1]
