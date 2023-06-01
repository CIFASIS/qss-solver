#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "VIRplanoS.h"
#include "pkg_math.h"
#include <common/utils.h>
#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

void MOD_settings(SD_simulationSettings settings)
{
	settings->debug = 0;
	settings->parallel = FALSE;
	settings->hybrid = TRUE;
	settings->method = 4;
	settings->random_seed = 1;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int _rg_d1;
	int i;
	switch(idx) {
		case _eval_Vr(0): {
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
			_sum_0(1,0) = _Vpar(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 9; _rg_d1<=11; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_4(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_Vpar(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_der_Vr(0) = _sum_0(4,0)*100/(double)4-_cr*_Vr(0);
			_der_Vr(1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_V(i,0) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_der_V(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_H(i,0) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_der_H(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_I(i,0) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
			_der_I(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_M(i,0) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_der_M(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_F(idx)) {
		_get_F_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_F(i,0) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
			_der_F(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_R(idx)) {
		_get_R_idxs(idx);
		_apply_usage_eq_6(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_R(i,0) = _bhf(i)*_F(i,0)*_H(i,0)-_ar(i)*_R(i,0);
			_der_R(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_E(idx)) {
		_get_E_idxs(idx);
		_apply_usage_eq_7(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_E(i,0) = _bem(i)*_M(i,0)*_E(i,0)-_bei(i)*_I(i,0)*_E(i,0)+_ae(i)*(1-_E(i,0));
			_der_E(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_P(idx)) {
		_get_P_idxs(idx);
		_apply_usage_eq_8(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_P(i,0) = _bpm(i)*_M(i,0)*_P(i,0)+_ap(i)*(1-_P(i,0));
			_der_P(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_A(idx)) {
		_get_A_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_A(i,0) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
			_der_A(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_S(idx)) {
		_get_S_idxs(idx);
		_apply_usage_eq_10(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_S(i,0) = _r(i)*_P(i,0)*(1-_S(i,0));
			_der_S(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _time-(_inst(i));
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _V(i,0)-(1);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _time-(_day(i));
			_zc(1) = (0)/1;
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 4)) {
			if(_inside(i)==0) {
		_inside(i) = 1-_inside(i);
		_inst(i) = _inst(i)+__math__rand(8.330000e-02);
	}else {
		_inside(i) = 1-_inside(i);
		_inst(i) = _inst(i)+__math__rand(2.075000e-01)+8.330000e-01;
	}
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Su = _Su-1;
			_In1 = _In1+1;
			_day(i) = _time+10;
			}
			return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_In1 = _In1-1;
			_Rec = _Rec+1;
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _In1;
			return;
		}
		case _eval_out_exp_2: {
			_out = _Rec;
			return;
		}
		case _eval_out_exp_3: {
			_out = _Su;
			return;
		}
		case _eval_out_exp_4: {
			_out = _Vr(0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
	int row, row_t, eq_var, c_row, c_row_g;
	int col, col_g, col_t;
	int x_ind;
	double aux;
	int _d1;
	int _rg_d1;
	int i;
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_A(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		x_ind = _idx_Vr(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		_apply_usage_eq_1(_d1);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_1_var_idxs(row, eq_var);
		_get_D_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
			_apply_usage_alg_eq_1(_d1);
			aux = 0;
			dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
			_apply_usage_alg_eq_1(_d1);
			aux = 0;
			dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
			_apply_usage_alg_eq_1(_d1);
			aux = 0;
			dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_H_idxs(eq_var);
		_apply_usage_eq_2(_d1);
		aux = 0;
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_I_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_E(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_M_idxs(eq_var);
		_apply_usage_eq_4(_d1);
		aux = 0;
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			c_row_g = _idx_D(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			_apply_usage_eq_4(_d1);
			aux = 0;
			dvdx->df_dx[3]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			_apply_usage_eq_4(_d1);
			aux = 0;
			dvdx->df_dx[3]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_F_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_R_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(_d1);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(_d1);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(_d1);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_E_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_E(_d1,0);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(_d1);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(_d1);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(_d1);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_P_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_A_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_A(_d1,0);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_10_var_idxs(row, eq_var);
		_get_S_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			col = pos(dvdx->df_dx[9]->index[c_row], dvdx->df_dx[9]->size[c_row], x_ind);
			_apply_usage_eq_10(_d1);
			aux = 0;
			dvdx->df_dx[9]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			col = pos(dvdx->df_dx[9]->index[c_row], dvdx->df_dx[9]->size[c_row], x_ind);
			_apply_usage_eq_10(_d1);
			aux = 0;
			dvdx->df_dx[9]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_Vpar_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
			_apply_usage_alg_eq_2(_d1);
			aux = 0;
			dvdx->dg_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_V(1,0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_Vpar(1,0) - 4;
		col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
		dvdx->dg_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_4_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		_apply_usage_alg_eq_4(_d1);
		aux = 0;
		if((2 <= _d1 && _d1 <= 4) && (2 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->dg_dx[3]->index[c_row], dvdx->dg_dx[3]->size[c_row], x_ind);
			c_row_g = _idx_Vpar(_rg_d1,0) - 4;
			col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
			dvdx->dg_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_V(1,0);
		col = pos(dvdx->df_dx[10]->index[c_row], dvdx->df_dx[10]->size[c_row], x_ind);
		c_row_g = _idx_sum_0(1,0) - 8;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[10]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_4_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 4)) {
				x_ind = _idx_V(_d1,0);
				col = pos(dvdx->df_dx[10]->index[c_row], dvdx->df_dx[10]->size[c_row], x_ind);
				c_row_g = _idx_sum_0(_rg_d1,0) - 9;
				col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
				dvdx->df_dx[10]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
			}
	}		x_ind = _idx_Vr(0);
			col = pos(dvdx->df_dx[10]->index[c_row], dvdx->df_dx[10]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[10]->value[c_row][col] +=  aux;
	}
	// Assign Jacobian Matrix values for equation: 0
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
		    row_t = dvdx->df_dx[0]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 1
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
		    row_t = dvdx->df_dx[1]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 2
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
		    row_t = dvdx->df_dx[2]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 3
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[3]->size[row]; col++) {
		    row_t = dvdx->df_dx[3]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[3]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 4
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[4]->size[row]; col++) {
		    row_t = dvdx->df_dx[4]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[4]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 5
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[5]->size[row]; col++) {
		    row_t = dvdx->df_dx[5]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[5]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 6
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[6]->size[row]; col++) {
		    row_t = dvdx->df_dx[6]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[6]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 7
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[7]->size[row]; col++) {
		    row_t = dvdx->df_dx[7]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[7]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 8
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[8]->size[row]; col++) {
		    row_t = dvdx->df_dx[8]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[8]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 9
		for (row = 0; row < 4; row++) {
		  for (col = 0; col < dvdx->df_dx[9]->size[row]; col++) {
		    row_t = dvdx->df_dx[9]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[9]->value[row][col]);
		  }
		}
	// Assign Jacobian Matrix values for equation: 10
		for (row = 0; row < 1; row++) {
		  for (col = 0; col < dvdx->df_dx[10]->size[row]; col++) {
		    row_t = dvdx->df_dx[10]->index[row][col];
		    _assign_jac(row_t, dvdx->df_dx[10]->value[row][col]);
		  }
		}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int _rg_d1;
	int i;
	switch(idx) {
		case _eval_Vr(0): {
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
			_sum_0(1,0) = _Vpar(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 9; _rg_d1<=11; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_4(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_Vpar(i,0);
		_sum_0(i,1) = 0;
	}
		}
	for(i = 1; i<=4; i+=1) {
		_apply_usage_eq_1(i);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
			_eval_dep_Vr(1) = _sum_0(4,0)*100/(double)4-_cr*_Vr(0);
			_eval_dep_Vr(2) = (0)/2;
			break;
		}
	}
	if (_is_var_A(idx)) {
		_get_A_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_A(idx)) {
		_get_A_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_A(i,1) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
			_eval_dep_A(i,2) = (0)/2;
		}
	}
	if (_is_var_E(idx)) {
		_get_E_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_I(i,1) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
			_eval_dep_I(i,2) = (0)/2;
		}
	}
	if (_is_var_E(idx)) {
		_get_E_idxs(idx);
		_apply_usage_eq_7(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_E(i,1) = _bem(i)*_M(i,0)*_E(i,0)-_bei(i)*_I(i,0)*_E(i,0)+_ae(i)*(1-_E(i,0));
			_eval_dep_E(i,2) = (0)/2;
		}
	}
	if (_is_var_F(idx)) {
		_get_F_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_eval_dep_H(i,2) = (0)/2;
		}
	}
	if (_is_var_F(idx)) {
		_get_F_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_F(i,1) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
			_eval_dep_F(i,2) = (0)/2;
		}
	}
	if (_is_var_F(idx)) {
		_get_F_idxs(idx);
		_apply_usage_eq_6(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_R(i,1) = _bhf(i)*_F(i,0)*_H(i,0)-_ar(i)*_R(i,0);
			_eval_dep_R(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_eval_dep_H(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_I(i,1) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
			_eval_dep_I(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_eval_dep_M(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_F(i,1) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
			_eval_dep_F(i,2) = (0)/2;
		}
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_eq_6(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_R(i,1) = _bhf(i)*_F(i,0)*_H(i,0)-_ar(i)*_R(i,0);
			_eval_dep_R(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_eval_dep_H(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_I(i,1) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
			_eval_dep_I(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_eval_dep_M(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_F(i,1) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
			_eval_dep_F(i,2) = (0)/2;
		}
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_7(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_E(i,1) = _bem(i)*_M(i,0)*_E(i,0)-_bei(i)*_I(i,0)*_E(i,0)+_ae(i)*(1-_E(i,0));
			_eval_dep_E(i,2) = (0)/2;
		}
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_eval_dep_M(i,2) = (0)/2;
		}
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_F(i,1) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
			_eval_dep_F(i,2) = (0)/2;
		}
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_eq_7(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_E(i,1) = _bem(i)*_M(i,0)*_E(i,0)-_bei(i)*_I(i,0)*_E(i,0)+_ae(i)*(1-_E(i,0));
			_eval_dep_E(i,2) = (0)/2;
		}
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_eq_8(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_P(i,1) = _bpm(i)*_M(i,0)*_P(i,0)+_ap(i)*(1-_P(i,0));
			_eval_dep_P(i,2) = (0)/2;
		}
	}
	if (_is_var_P(idx)) {
		_get_P_idxs(idx);
		_apply_usage_eq_8(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_P(i,1) = _bpm(i)*_M(i,0)*_P(i,0)+_ap(i)*(1-_P(i,0));
			_eval_dep_P(i,2) = (0)/2;
		}
	}
	if (_is_var_P(idx)) {
		_get_P_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_A(i,1) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
			_eval_dep_A(i,2) = (0)/2;
		}
	}
	if (_is_var_P(idx)) {
		_get_P_idxs(idx);
		_apply_usage_eq_10(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_S(i,1) = _r(i)*_P(i,0)*(1-_S(i,0));
			_eval_dep_S(i,2) = (0)/2;
		}
	}
	if (_is_var_R(idx)) {
		_get_R_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_eval_dep_H(i,2) = (0)/2;
		}
	}
	if (_is_var_R(idx)) {
		_get_R_idxs(idx);
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_eval_dep_M(i,2) = (0)/2;
		}
	}
	if (_is_var_R(idx)) {
		_get_R_idxs(idx);
		_apply_usage_eq_6(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_R(i,1) = _bhf(i)*_F(i,0)*_H(i,0)-_ar(i)*_R(i,0);
			_eval_dep_R(i,2) = (0)/2;
		}
	}
	if (_is_var_S(idx)) {
		_get_S_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_S(idx)) {
		_get_S_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_A(i,1) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
			_eval_dep_A(i,2) = (0)/2;
		}
	}
	if (_is_var_S(idx)) {
		_get_S_idxs(idx);
		_apply_usage_eq_10(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_S(i,1) = _r(i)*_P(i,0)*(1-_S(i,0));
			_eval_dep_S(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
		_D(i,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
			_eval_dep_H(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_I(i,1) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
			_eval_dep_I(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
			_eval_dep_M(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_A(i,1) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
			_eval_dep_A(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
		_Vpar(i,1) = 0;
	}
		}
			_sum_0(1,0) = _Vpar(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 9; _rg_d1<=11; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_4(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_Vpar(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_eval_dep_Vr(1) = _sum_0(4,0)*100/(double)4-_cr*_Vr(0);
			_eval_dep_Vr(2) = (0)/2;
		}
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	int _d1;
	int _rg_d1;
	int i;
	switch(idx) {
		case _eval_Vr(0): {
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
	
	}
		}
	for(_rg_d1 = 4; _rg_d1<=7; _rg_d1+=1) {
	_get_Vpar_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Vpar(i,0) = _V(i,0)*_tr(i)*_inside(i);
	
	}
		}
			_sum_0(1,0) = _Vpar(1,0);
	
	for(_rg_d1 = 9; _rg_d1<=11; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_4(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_Vpar(i,0);
	
	}
		}
			_eval_dep_Vr(1) = _sum_0(4,0)*100/(double)4-_cr*_Vr(0);
	
	
			continue;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_V(i,1) = _gv(i)*_I(i,0)-_gva(i)*_S(i,0)*_A(i,0)*_V(i,0)-_gvh(i)*_H(i,0)*_V(i,0)-_av(i)*_V(i,0)-_av1(i)*_V(i,0)/(1+_av2(i)*_V(i,0))+_inside(i)*_re(i)*_Vr(0);
	
	
		}
		continue;
	}
	if (_is_var_H(idx)) {
		_get_H_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
	
	}
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_H(i,1) = _bhd(i)*_D(i,0)*(_H(i,0)+_R(i,0))+_ar(i)*_R(i,0)-_ghv(i)*_V(i,0)*_H(i,0)-_bhf(i)*_F(i,0)*_H(i,0);
	
	
		}
		continue;
	}
	if (_is_var_I(idx)) {
		_get_I_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_I(i,1) = _ghv(i)*_V(i,0)*_H(i,0)-_bie(i)*_E(i,0)*_I(i,0)-_ai(i)*_I(i,0);
	
	
		}
		continue;
	}
	if (_is_var_M(idx)) {
		_get_M_idxs(idx);
		_apply_usage_alg_eq_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_D(i,0) = 1-_H(i,0)-_R(i,0)-_I(i,0);
	
	}
		_apply_usage_eq_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_M(i,1) = (_bmd(i)*_D(i,0)+_bmv(i)*_V(i,0))*(1-_M(i,0))-_am(i)*_M(i,0);
	
	
		}
		continue;
	}
	if (_is_var_F(idx)) {
		_get_F_idxs(idx);
		_apply_usage_eq_5(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_F(i,1) = _bf(i)*_M(i,0)+_cf(i)*_I(i,0)-_bfh(i)*_H(i,0)*_F(i,0)-_af(i)*_F(i,0);
	
	
		}
		continue;
	}
	if (_is_var_R(idx)) {
		_get_R_idxs(idx);
		_apply_usage_eq_6(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_R(i,1) = _bhf(i)*_F(i,0)*_H(i,0)-_ar(i)*_R(i,0);
	
	
		}
		continue;
	}
	if (_is_var_E(idx)) {
		_get_E_idxs(idx);
		_apply_usage_eq_7(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_E(i,1) = _bem(i)*_M(i,0)*_E(i,0)-_bei(i)*_I(i,0)*_E(i,0)+_ae(i)*(1-_E(i,0));
	
	
		}
		continue;
	}
	if (_is_var_P(idx)) {
		_get_P_idxs(idx);
		_apply_usage_eq_8(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_P(i,1) = _bpm(i)*_M(i,0)*_P(i,0)+_ap(i)*(1-_P(i,0));
	
	
		}
		continue;
	}
	if (_is_var_A(idx)) {
		_get_A_idxs(idx);
		_apply_usage_eq_9(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_A(i,1) = _ba(i)*_P(i,0)-_gav(i)*_S(i,0)*_A(i,0)*_V(i,0)-_aa(i)*_A(i,0);
	
	
		}
		continue;
	}
	if (_is_var_S(idx)) {
		_get_S_idxs(idx);
		_apply_usage_eq_10(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_S(i,1) = _r(i)*_P(i,0)*(1-_S(i,0));
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(41,16,12,0,12,11,4,"VIRplanoS");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(12*sizeof(int));
	int* states = (int*) malloc(41*sizeof(int));
	int* discretes = (int*) malloc(16*sizeof(int));
	int* events = (int*) malloc(12*sizeof(int));
	int* outputs = (int*) malloc(4*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_init_A(_d1,0) = 1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_init_E(_d1,0) = 1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_init_H(_d1,0) = 1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_init_P(_d1,0) = 1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_init_S(_d1,0) = 1.000000e-01;
	}
	_cr = 100;
	_u = __math__rand(1);
	for(i = 1; i<=4; i+=1) {
		_gv(i) = 510;
		_gva(i) = 6.192000e+02;
		_gvh(i) = 1.020000e+00;
		_av(i) = 1.700000e+00;
		_av1(i) = 100;
		_av2(i) = 23000;
		_bhd(i) = 4;
		_ar(i) = 1;
		_ghv(i) = 3.400000e-01;
		_bhf(i) = 1.000000e-02;
		_bie(i) = 6.600000e-02;
		_ai(i) = 1.500000e+00;
		_bmd(i) = 1;
		_bmv(i) = 3.700000e-03;
		_am(i) = 1;
		_bf(i) = 250000;
		_cf(i) = 2000;
		_bfh(i) = 17;
		_af(i) = 8;
		_bem(i) = 8.300000e+00;
		_bei(i) = 2.720000e+00;
		_ae(i) = 4.000000e-01;
		_bpm(i) = 1.150000e+01;
		_ap(i) = 4.000000e-01;
		_ba(i) = 4.300000e-02;
		_gav(i) = 1.462000e+02;
		_aa(i) = 4.300000e-02;
		_r(i) = 3.000000e-05;
	}
	for(i = 1; i<=2; i+=1) {
		_re(i) = __math__rand(2.200000e-01)+1.800000e-01;
		_tr(i) = _re(i);
	}
	for(i = 3; i<=4; i+=1) {
		_re(i) = __math__rand(6.000000e-02)+4.000000e-02;
		_tr(i) = _re(i);
	}
	_init_V(1,0) = 10;
	_inside(1) = 1;
	for(i = 1; i<=4; i+=1) {
		_inst(i) = __math__rand(2);
		_day(i) = 1000;
	}
	for(i = 1; i<=2; i+=1) {
		_init_V(i,0) = 10;
		_inside(i) = 1;
	}
	_Su = 4;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_E(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_E(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_P(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_P(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_P(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_S(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_S(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_S(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_Vr(0)]++;
	}
	modelData->nSD[_idx_Vr(0)]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_E(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_E(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_F(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_E(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_P(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_P(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_S(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_R(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_S(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_H(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_I(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_M(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_A(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_Vr(0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	modelData->nDS[_idx_Vr(0)]++;
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_1_var_idxs(row, eq_var);
		_get_D_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_H_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_I_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_M_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_F_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_R_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_E_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_P_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_A_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_10_var_idxs(row, eq_var);
		_get_S_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[9]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[9]->size[c_row]++;
		}
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_Vpar_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_4_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 4) && (2 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[3]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[10]->size[c_row]++;
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_4_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[10]->size[c_row]++;
			}
	}		modelData->jac_matrices->df_dx[10]->size[c_row]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSZ[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nZS[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 41);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_E(_d1,0)][states[_idx_E(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_E(_d1,0)][states[_idx_E(_d1,0)]++] = _idx_E(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_E(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_E(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_P(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_P(_d1,0)][states[_idx_P(_d1,0)]++] = _idx_P(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_P(_d1,0)][states[_idx_P(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_P(_d1,0)][states[_idx_P(_d1,0)]++] = _idx_S(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_S(_d1,0)][states[_idx_S(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_S(_d1,0)][states[_idx_S(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_S(_d1,0)][states[_idx_S(_d1,0)]++] = _idx_S(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_Vr(0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_Vr(0)][states[_idx_Vr(0)]++] = _idx_V(_d1,0);
	}
	modelData->SD[_idx_Vr(0)][states[_idx_Vr(0)]++] = _idx_Vr(0);
	cleanVector(states, 0, 41);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_A(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_E(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_E(_d1,0)][states[_idx_E(_d1,0)]++] = _idx_E(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_F(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_H(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_E(_d1,0)][states[_idx_E(_d1,0)]++] = _idx_I(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_F(_d1,0)][states[_idx_F(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_E(_d1,0)][states[_idx_E(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_P(_d1,0)][states[_idx_P(_d1,0)]++] = _idx_M(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_P(_d1,0)][states[_idx_P(_d1,0)]++] = _idx_P(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_P(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_S(_d1,0)][states[_idx_S(_d1,0)]++] = _idx_P(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_R(_d1,0)][states[_idx_R(_d1,0)]++] = _idx_R(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_S(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_S(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_S(_d1,0)][states[_idx_S(_d1,0)]++] = _idx_S(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_H(_d1,0)][states[_idx_H(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_I(_d1,0)][states[_idx_I(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_M(_d1,0)][states[_idx_M(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_A(_d1,0)][states[_idx_A(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_Vr(0)][states[_idx_Vr(0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_Vr(0);
	}
	modelData->DS[_idx_Vr(0)][states[_idx_Vr(0)]++] = _idx_Vr(0);
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_A(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
			x_ind = _idx_Vr(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 12);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_1_var_idxs(row, eq_var);
		_get_D_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_H_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_I_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_E(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_M_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4) && (1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_F_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_R_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_F(_d1,0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_H(_d1,0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_R(_d1,0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_E_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_E(_d1,0);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_I(_d1,0);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_P_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_M(_d1,0);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_A_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_A(_d1,0);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_10_var_idxs(row, eq_var);
		_get_S_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_P(_d1,0);
			if(in(modelData->jac_matrices->df_dx[9]->index[c_row],modelData->jac_matrices->df_dx[9]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[9]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[9]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_S(_d1,0);
			if(in(modelData->jac_matrices->df_dx[9]->index[c_row],modelData->jac_matrices->df_dx[9]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[9]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[9]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(algebraics, 0, 12);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_Vpar_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][algebraics[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(algebraics, 0, 12);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_V(1,0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 12);
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_4_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 4) && (2 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[3]->index[c_row],modelData->jac_matrices->dg_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[3]->index[c_row][algebraics[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 41);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_V(1,0);
			if(in(modelData->jac_matrices->df_dx[10]->index[c_row],modelData->jac_matrices->df_dx[10]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[10]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[10]->index[c_row][states[c_row]++] = x_ind;
			}
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_4_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[10]->index[c_row],modelData->jac_matrices->df_dx[10]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[10]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[10]->index[c_row][states[c_row]++] = x_ind;
			}
			}
	}		x_ind = _idx_Vr(0);
			if(in(modelData->jac_matrices->df_dx[10]->index[c_row],modelData->jac_matrices->df_dx[10]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[10]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[10]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 41);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SZ[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 12);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->ZS[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_V(_d1,0);
	}
	cleanVector(events, 0, 12);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_3(_d1);
	}
	cleanVector(events, 0, 12);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_V(_d1,0);
	}
	cleanVector(events, 0, 12);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_day(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_In1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_In1;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_Rec;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_Su;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_inside(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_inst(_d1);
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 1;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_3(i)].direction = 1;
		modelData->event[_idx_event_3(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(41,12,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.01;
		simulator->output = SD_Output("VIRplanoS",4,16,41,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nOD[_idx_out_exp_1]++;
	modelOutput->nOD[_idx_out_exp_2]++;
	modelOutput->nOD[_idx_out_exp_3]++;
	modelOutput->nSO[_idx_Vr(0)]++;
	modelOutput->nDO[_idx_In1]++;
	modelOutput->nDO[_idx_Rec]++;
	modelOutput->nDO[_idx_Su]++;
	SD_allocOutputMatrix(modelOutput, 41, 16);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "In1");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "Rec");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "Su");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "Vr");
	cleanVector(outputs, 0, 4);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_Vr(0);
	cleanVector(outputs, 0, 4);
	modelOutput->OD[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_In1;
	modelOutput->OD[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_Rec;
	modelOutput->OD[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_Su;
	cleanVector(states, 0, 41);
	modelOutput->SO[_idx_Vr(0)][states[_idx_Vr(0)]++] = _idx_out_exp_4;
	cleanVector(discretes, 0, 16);
	modelOutput->DO[_idx_In1][discretes[_idx_In1]++] = _idx_out_exp_1;
	modelOutput->DO[_idx_Rec][discretes[_idx_Rec]++] = _idx_out_exp_2;
	modelOutput->DO[_idx_Su][discretes[_idx_Su]++] = _idx_out_exp_3;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

