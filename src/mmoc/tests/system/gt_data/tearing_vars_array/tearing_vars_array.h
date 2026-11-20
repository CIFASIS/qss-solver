
// Fordward declare tearing functions. 
void MOD_EvalTearingVars(double *x, double *d, double *a, double *t, double *dx, const double *guess, double *ret);
void MOD_AssignLoop(double *x, double *d, double *a, double *t, double *dx);

// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Model Variables Macros

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: Ra
#define _Ra __PAR__Ra

// Macros definition for variable: Rb
#define _Rb __PAR__Rb

// Macros definition for variable: Rc
#define _Rc __PAR__Rc

// Macros definition for variable: U0
#define _U0 __PAR__U0

// Macros definition for variable: Ua
#define _idx_Ua(d1) ((d1-1))
#define _Ua(d1) a[_idx_Ua(d1)]
#define _is_var_Ua(idx) idx >= 0 && idx < 5
#define _get_Ua_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: Uc
#define _idx_Uc(d1) (5+(d1-1))
#define _Uc(d1) a[_idx_Uc(d1)]
#define _is_var_Uc(idx) idx >= 5 && idx < 10
#define _get_Uc_idxs(idx)\
		_d1 = (idx-5)+ 1;

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: _out_exp_2
#define _idx_out_exp_2 1
#define _eval_out_exp_2 1

// Macros definition for variable: iL
#define _idx_iL(d1) ((d1-1))
#define _iL(d1) x[_idx_iL(d1)]
#define _init_iL(d1) x[_idx_iL(d1)]
#define _eval_iL(d1) ((d1-1))
#define _is_var_iL(idx) idx >= 0 && idx < 5
#define _get_iL_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: tearing_var_guess_Ua
#define _idx_tearing_var_guess_Ua(d1) ((d1-1))
#define _tearing_var_guess_Ua(d1) __PAR__tearing_var_guess_Ua[_idx_tearing_var_guess_Ua(d1)]

// Macros definition for variable: tearing_var_guess_Uc_N
#define _tearing_var_guess_Uc_N __PAR__tearing_var_guess_Uc_N

// Macros definition for variable: tearing_var_res_Ua
#define _idx_tearing_var_res_Ua(d1) (10+(d1-1))
#define _tearing_var_res_Ua(d1) a[_idx_tearing_var_res_Ua(d1)]
#define _is_var_tearing_var_res_Ua(idx) idx >= 10 && idx < 14
#define _get_tearing_var_res_Ua_idxs(idx)\
		_d1 = (idx-10)+ 1;

// Macros definition for variable: tearing_var_res_Uc_N
#define _idx_tearing_var_res_Uc_N 14
#define _tearing_var_res_Uc_N a[_idx_tearing_var_res_Uc_N]


// Model Parameters Declaration

// Macro for parameter: L
double __PAR__L;
// Macro for parameter: Ra
double __PAR__Ra;
// Macro for parameter: Rb
double __PAR__Rb;
// Macro for parameter: Rc
double __PAR__Rc;
// Macro for parameter: U0
double __PAR__U0;
// Macro for parameter: tearing_var_guess_Ua
double __PAR__tearing_var_guess_Ua[4];
// Macro for parameter: tearing_var_guess_Uc_N
double __PAR__tearing_var_guess_Uc_N;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1);
// Macros for equation: 3
#define _apply_usage_eq_3(_d1) \
	i = _d1;
#define _get_eq_3_var_idxs(row, var)\
	_rg_d1 = 2 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1);
// Macros for equation: 4
#define _apply_usage_eq_4(_d1) \
	i = _d1;
#define _get_eq_4_var_idxs(row, var)\
	_rg_d1 = 4 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1);

// Algebraic Equations Macros

// Macros for algebraic equation: 1
#define _apply_usage_alg_eq_1(_d1) \
	i = _d1;
#define _get_alg_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_Ua(_rg_d1);
// Macros for algebraic equation: 2
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_Uc(_rg_d1);
// Macros for algebraic equation: 3
#define _apply_usage_alg_eq_3(_d1) \
	i = _d1;
#define _get_alg_eq_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_tearing_var_res_Ua(_rg_d1);
// Macros for algebraic equation: 4
#define _apply_usage_alg_eq_4(_d1) \
	i = _d1;
#define _get_alg_eq_4_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_tearing_var_res_Ua(_rg_d1);
// Macros for algebraic equation: 5
#define _apply_usage_alg_eq_5(_d1) \
	i = _d1;
#define _get_alg_eq_5_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_Ua(_rg_d1);
// Macros for algebraic equation: 6
#define _apply_usage_alg_eq_6(_d1) \
	i = _d1;
#define _get_alg_eq_6_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_Uc(_rg_d1);
// Macros for algebraic equation: 7
#define _apply_usage_alg_eq_7(_d1) \
	i = _d1;
#define _get_alg_eq_7_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_tearing_var_res_Ua(_rg_d1);
// Macros for algebraic equation: 8
#define _apply_usage_alg_eq_8(_d1) \
	i = _d1;
#define _get_alg_eq_8_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_tearing_var_res_Ua(_rg_d1);
// Macros for algebraic equation: 9

// Macros for algebraic equation: 10

// Macros for algebraic equation: 11

// Macros for algebraic equation: 12


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
// Derivative definition for variable: iL
#define _der_iL(d1) dx[((d1-1))]
