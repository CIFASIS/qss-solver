#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "interleaved.h"
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
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int _rg_d1;
	int i;
	switch(idx) {
		case _eval_uC(0): {
			_sum_0(1,0) = _iL(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 1; _rg_d1<=3; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_iL(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_der_uC(0) = (_sum_0(4,0)-_uC(0)/_R)/_C;
			_der_uC(1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_iL(idx)) {
		_get_iL_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_iL(i,0) = (((_U/_Rs(i))-_iL(i,0))*(_Rs(i)*_Rd(i)/(_Rs(i)+_Rd(i)))-_uC(0))/_L;
			_der_iL(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_nextT);
			_zc(1) = (0)/1;
	
			return;
		}
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _time-_lastT-_T*(i-1)/4-0.01*_T-(0);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _time-_lastT-_T*(i-1)/4-_DC*_T/4-0.01*_T-(0);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_4(idx)) {
		_get_event_4_idxs(idx);
		_apply_usage_event_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _iL(i,0)-(0);
			_zc(1) = (0)/1;
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_1: {
			_lastT = _nextT;
			_nextT = _nextT+_T;
					return;
		}
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_Rs(i) = _ROn;
			_Rd(i) = _ROff;
			}
			return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4)) {
			_Rs(i) = _ROff;
			_Rd(i) = _ROn;
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_4(idx)) {
		_get_event_4_idxs(idx);
		_apply_usage_event_4(_d1);
		if ((i >= 1 && i <= 4)) {
			_Rd(i) = _ROff;
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	int _d1;
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _uC(0);
			return;
		}
	}
	if (_is_var_out_exp_2(idx)) {
		_get_out_exp_2_idxs(idx);
		_apply_usage_out_exp_2(_d1);
		if ((_d1 >= 1 && _d1 <= 4)) {
			_out = _iL(_d1,0);
		}
		return;
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
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_iL(1,0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_iL(_d1,0);
			col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
			_apply_usage_alg_eq_2(_d1);
			aux = 0;
			dvdx->dg_dx[1]->value[c_row][col] +=  aux;
		}
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 3)) {
				x_ind = _idx_iL(_d1,0);
				col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
				_apply_usage_alg_eq_2(_d1);
				aux = 0;
				dvdx->dg_dx[1]->value[c_row][col] +=  aux;
			}
	}		x_ind = _idx_iL(1,0);
			col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->dg_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			aux = 0;
			x_ind = _idx_iL(1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_sum_0(1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
			for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
				if((2 <= _d1 && _d1 <= 3)) {
					x_ind = _idx_iL(_d1,0);
					col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
					c_row_g = _idx_sum_0(_rg_d1,0) - 1;
					col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
					dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
				}
	}			x_ind = _idx_iL(4,0);
				col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
				c_row_g = _idx_sum_0(4,0) - 1;
				col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
				dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
				x_ind = _idx_uC(0);
				col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
				aux = 0;
				dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_iL_idxs(eq_var);
				if((1 <= _d1 && _d1 <= 4)) {
					x_ind = _idx_iL(_d1,0);
					col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
					_apply_usage_eq_2(_d1);
					aux = 0;
					dvdx->df_dx[1]->value[c_row][col] +=  aux;
				}
				x_ind = _idx_uC(0);
				col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
				_apply_usage_eq_2(_d1);
				aux = 0;
				dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	// Assign Jacobian Matrix values for equation: 0
			for (row = 0; row < 1; row++) {
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
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int _rg_d1;
	int i;
	switch(idx) {
		case _eval_iL(4,0): {
			_sum_0(1,0) = _iL(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 1; _rg_d1<=3; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_iL(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_eval_dep_uC(1) = (_sum_0(4,0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
			break;
		}
		case _eval_uC(0): {
			_sum_0(1,0) = _iL(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 1; _rg_d1<=3; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_iL(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_eval_dep_uC(1) = (_sum_0(4,0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
	for(i = 1; i<=4; i+=1) {
		_apply_usage_eq_2(i);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_iL(i,1) = (((_U/_Rs(i))-_iL(i,0))*(_Rs(i)*_Rd(i)/(_Rs(i)+_Rd(i)))-_uC(0))/_L;
			_eval_dep_iL(i,2) = (0)/2;
		}
	}
			break;
		}
	}
	if (_is_var_iL(idx)) {
		_get_iL_idxs(idx);
			_sum_0(1,0) = _iL(1,0);
			_sum_0(1,1) = 0;
	for(_rg_d1 = 1; _rg_d1<=3; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_iL(i,0);
		_sum_0(i,1) = 0;
	}
		}
			_eval_dep_uC(1) = (_sum_0(4,0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
		}
	if (_is_var_iL(idx)) {
		_get_iL_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_eval_dep_iL(i,1) = (((_U/_Rs(i))-_iL(i,0))*(_Rs(i)*_Rd(i)/(_Rs(i)+_Rd(i)))-_uC(0))/_L;
			_eval_dep_iL(i,2) = (0)/2;
		}
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
		case _eval_uC(0): {
			_sum_0(1,0) = _iL(1,0);
	
	for(_rg_d1 = 1; _rg_d1<=3; _rg_d1+=1) {
	_get_sum_0_idxs(_rg_d1);
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 2 && i <= 4)) {
			_sum_0(i,0) = _sum_0(i-1,0)+_iL(i,0);
	
	}
		}
			_der_uC(0) = (_sum_0(4,0)-_uC(0)/_R)/_C;
	
	
			return;
		}
	}
	if (_is_var_iL(idx)) {
		_get_iL_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_der_iL(i,0) = (((_U/_Rs(i))-_iL(i,0))*(_Rs(i)*_Rd(i)/(_Rs(i)+_Rd(i)))-_uC(0))/_L;
	
	
		}
		return;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(5,10,13,0,4,2,2,"interleaved");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(5*sizeof(int));
	int* discretes = (int*) malloc(10*sizeof(int));
	int* events = (int*) malloc(13*sizeof(int));
	int* outputs = (int*) malloc(5*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_C = 0.0001;
	_DC = 0.5;
	_L = 0.0001;
	_R = 10;
	_ROff = 100000;
	_ROn = 1e-05;
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_Rd(_d1) = 100000;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_Rs(_d1) = 100000;
	}
	_T = 0.0001;
	_U = 24;
	_nextT = 1e-08;
	modelData->nSD[_idx_iL(4,0)]++;
	modelData->nSD[_idx_uC(0)]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
	modelData->nSD[_idx_uC(0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_iL(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSD[_idx_iL(_d1,0)]++;
	}
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
	modelData->nDS[_idx_iL(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_uC(0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDS[_idx_iL(_d1,0)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
		}
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 3)) {
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
			}
	}		modelData->jac_matrices->dg_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 3)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			}
	}		modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_iL_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSZ[_idx_iL(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nZS[_idx_event_4(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_1]++;
	}
	modelData->nHZ[_idx_event_1]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_4(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHD[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 5);
	modelData->SD[_idx_iL(4,0)][states[_idx_iL(4,0)]++] = _idx_uC(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	for(_d1 = 1; _d1<=4; _d1+=1) {
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_uC(0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SD[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_iL(_d1,0);
	}
	cleanVector(states, 0, 5);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(4,0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	for(_d1 = 1; _d1<=4; _d1+=1) {
	modelData->DS[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_uC(0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DS[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_iL(_d1,0);
	}
	cleanVector(states, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(1,0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 5);
	for(row = 1; row <= 3; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_sum_0_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_iL(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 3)) {
			x_ind = _idx_iL(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			}
	}		x_ind = _idx_iL(1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		for(i = 2; i<=4; i+=1) {
			_get_alg_eq_2_var_idxs(_idx_sum_0(i,0), eq_var);
		_get_sum_0_idxs(eq_var);
			if((2 <= _d1 && _d1 <= 3)) {
			x_ind = _idx_iL(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			}
	}		x_ind = _idx_iL(4,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 5);
	for(row = 1; row <= 4; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_iL_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4)) {
			x_ind = _idx_iL(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 5);
	cleanVector(states, 0, 5);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SZ[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_event_4(_d1);
	}
	cleanVector(events, 0, 13);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->ZS[_idx_event_4(_d1)][events[_idx_event_4(_d1)]++] = _idx_iL(_d1,0);
	}
	cleanVector(events, 0, 13);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3(_d1);
	}
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	cleanVector(events, 0, 13);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_4(_d1)][events[_idx_event_4(_d1)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_iL(_d1,0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HD[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_iL(_d1,0);
	}
	cleanVector(events, 0, 13);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_Rd(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_Rd(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].LHSDsc[events[_idx_event_4(_d1)]++] = _idx_Rd(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_Rs(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_Rs(_d1);
	}
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_3(i)].direction = 1;
		modelData->event[_idx_event_3(i)].relation = 2;
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_4(i)].direction = -1;
		modelData->event[_idx_event_4(i)].relation = 0;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(5,13,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("interleaved",5,10,5,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_2(_d1)]++;
	}
	modelOutput->nSO[_idx_uC(0)]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelOutput->nSO[_idx_iL(_d1,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 5, 10);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "uC");
	for(_d1 = 1; _d1<=4; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_2(_d1)].name, "iL[%d]",_d1);
	}
	cleanVector(outputs, 0, 5);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_uC(0);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelOutput->OS[_idx_out_exp_2(_d1)][outputs[_idx_out_exp_2(_d1)]++] = _idx_iL(_d1,0);
	}
	cleanVector(states, 0, 5);
	modelOutput->SO[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_out_exp_1;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelOutput->SO[_idx_iL(_d1,0)][states[_idx_iL(_d1,0)]++] = _idx_out_exp_2(_d1);
	}
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

