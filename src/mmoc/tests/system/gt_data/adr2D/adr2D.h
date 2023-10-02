// Model data access macro.

#define MODEL_DATA_ACCESS(m) \
  double* x = m->x; 

// Model Variables Macros

// Macros definition for variable: _out_exp_1
#define _idx_out_exp_1 0
#define _eval_out_exp_1 0

// Macros definition for variable: a
#define _a __PAR__a

// Macros definition for variable: d
#define _d __PAR__d

// Macros definition for variable: dx
#define _dx __PAR__dx

// Macros definition for variable: r
#define _r __PAR__r

// Macros definition for variable: u
#define _idx_u(d1,d2) ((d1-1)*10+(d2-1))
#define _u(d1,d2) x[_idx_u(d1,d2)]
#define _init_u(d1,d2) x[_idx_u(d1,d2)]
#define _eval_u(d1,d2) ((d1-1)*10+(d2-1))
#define _is_var_u(idx) idx >= 0 && idx < 100
#define _get_u_idxs(idx)\
		_d1 = ((idx)/10)+ 1;\
	_d2 = ((idx)%10)+ 1;


// Model Parameters Declaration

// Macro for parameter: a
double __PAR__a;
// Macro for parameter: d
double __PAR__d;
// Macro for parameter: dx
double __PAR__dx;
// Macro for parameter: r
double __PAR__r;

// Derivative Equations Macros

// Macros for equation: 1

// Macros for equation: 2

// Macros for equation: 3

// Macros for equation: 4

// Macros for equation: 5
#define _apply_usage_eq_5(_d1,_d2) \
	i = _d1;\
	j = _d2;
#define _get_eq_5_var_idxs(row, var)\
	_rg_d1 = 1 + ((row-1)/8)+ 1;\
	_rg_d2 = 1 + ((row-1)%8)+ 1;\
	var = _idx_u(_rg_d1,_rg_d2);
// Macros for equation: 6
#define _apply_usage_eq_6(_d1,_d2) \
	j = _d2;
#define _get_eq_6_var_idxs(row, var)\
	_rg_d2 = 1 + (row-1)+ 1;\
	_rg_d1 = 1;\
	var = _idx_u(_rg_d1,_rg_d2);
// Macros for equation: 7
#define _apply_usage_eq_7(_d1,_d2) \
	j = _d2;
#define _get_eq_7_var_idxs(row, var)\
	_rg_d2 = 1 + (row-1)+ 1;\
	_rg_d1 = 10;\
	var = _idx_u(_rg_d1,_rg_d2);
// Macros for equation: 8
#define _apply_usage_eq_8(_d1,_d2) \
	i = _d1;
#define _get_eq_8_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	_rg_d2 = 1;\
	var = _idx_u(_rg_d1,_rg_d2);
// Macros for equation: 9
#define _apply_usage_eq_9(_d1,_d2) \
	i = _d1;
#define _get_eq_9_var_idxs(row, var)\
	_rg_d1 = 1 + (row-1)+ 1;\
	_rg_d2 = 10;\
	var = _idx_u(_rg_d1,_rg_d2);

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
// Derivative definition for variable: u
#define _der_u(d1,d2) dx[((d1-1)*10+(d2-1))]
