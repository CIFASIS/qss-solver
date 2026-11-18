// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; \
  double* a = m->alg;

// Model Variables Macros

// Macros definition for variable: I
#define _I __PAR__I

// Macros definition for variable: L
#define _L __PAR__L

// Macros definition for variable: L1
#define _L1 __PAR__L1

// Macros definition for variable: R
#define _R __PAR__R

// Macros definition for variable: R0
#define _R0 __PAR__R0

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: iL
#define _idx_iL(d1) ((d1-1))
#define _iL(d1) x[_idx_iL(d1)]
#define _init_iL(d1) x[_idx_iL(d1)]
#define _eval_iL(d1) ((d1-1))
#define _is_var_iL(idx) idx >= 0 && idx < 100
#define _get_iL_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: iR
#define _idx_iR(d1) ((d1-1))
#define _iR(d1) a[_idx_iR(d1)]
#define _is_var_iR(idx) idx >= 0 && idx < 100
#define _get_iR_idxs(idx)\
		_d1 = (idx)+ 1;

// Macros definition for variable: uL
#define _idx_uL(d1) (100+(d1-1))
#define _uL(d1) a[_idx_uL(d1)]
#define _is_var_uL(idx) idx >= 100 && idx < 200
#define _get_uL_idxs(idx)\
		_d1 = (idx-100)+ 1;


// Model Parameters Declaration

// Macro for parameter: I
double __PAR__I;
// Macro for parameter: L
double __PAR__L;
// Macro for parameter: L1
double __PAR__L1;
// Macro for parameter: R
double __PAR__R;
// Macro for parameter: R0
double __PAR__R0;

// Derivative Equations Macros

// Macros for equation: 1
#define _apply_usage_eq_1(_d1) \
	i = _d1;
#define _get_eq_1_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1);
// Macros for equation: 2
#define _apply_usage_eq_2(_d1) \
	i = _d1;
#define _get_eq_2_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	var = _idx_iL(_rg_d1);

// Algebraic Equations Macros

// Macros for algebraic equation: 1

// Macros for algebraic equation: 2
#define _apply_usage_alg_eq_2(_d1) \
	i = _d1;
#define _get_alg_eq_2_var_idxs(row, var)\
	_rg_d1 = 98 + (row-1)+ 1;\
	var = _idx_iR(_rg_d1);
// Macros for algebraic equation: 3
#define _apply_usage_alg_eq_3(_d1) \
	i = _d1;
#define _get_alg_eq_3_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_iR(_rg_d1);
// Macros for algebraic equation: 4

// Macros for algebraic equation: 5
#define _apply_usage_alg_eq_5(_d1) \
	i = _d1;
#define _get_alg_eq_5_var_idxs(row, var)\
	_rg_d1 = 0 + (row-1)+ 1;\
	var = _idx_uL(_rg_d1);

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
// Derivative definition for variable: iL
#define _der_iL(d1) dx[((d1-1))]
