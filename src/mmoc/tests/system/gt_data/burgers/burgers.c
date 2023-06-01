#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "burgers.h"
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
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_der_u(1,0) = _ap(1)*(_u(1,0)+_u(1000,0))/(double)2*(-_u(1,0)+_u(1000,0))*1000+(1-_ap(1))*(_u(2,0)+_u(1,0))/2*(-_u(2,0)+_u(1,0))*1000;
	
	
			return;
		}
		case _eval_u(1000,0): {
			_der_u(1000,0) = _ap(1000)*(_u(1000,0)+_u(999,0))/(double)2*(-_u(1000,0)+_u(999,0))*1000+(1-_ap(1000))*(_u(1,0)+_u(1000,0))*(-_u(1,0)+_u(1000,0))*1000;
	
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 999)) {
			_der_u(j,0) = _ap(j)*(_u(j,0)+_u(j-1,0))/(double)2*(-_u(j,0)+_u(j-1,0))*1000+(1-_ap(j))*(_u(j+1,0)+_u(j,0))*(-_u(j+1,0)+_u(j,0))*1000;
	
	
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
		if ((i >= 1 && i <= 1000)) {
			_zc(0) = _u(i,0)-(0);
	
	
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
		if ((i >= 1 && i <= 1000)) {
			_ap(i) = 1;
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_ap(i) = 0;
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	int _d1;
	if (_is_var_out_exp_1(idx)) {
		_get_out_exp_1_idxs(idx);
		_apply_usage_out_exp_1(_d1);
		if ((_d1 >= 1 && _d1 <= 5)) {
			_out = _u(200*_d1-199,0);
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
	int j;
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1,0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_u(1000,0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_u(2,0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 998; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			x_ind = _idx_u(_d1+1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 999)) {
			x_ind = _idx_u(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			x_ind = _idx_u(_d1-1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1,0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_u(1000,0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_u(999,0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 1
	for (row = 0; row < 998; row++) {
	  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
	    row_t = dvdx->df_dx[1]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_eval_dep_u(1,1) = _ap(1)*(_u(1,0)+_u(1000,0))/(double)2*(-_u(1,0)+_u(1000,0))*1000+(1-_ap(1))*(_u(2,0)+_u(1,0))/2*(-_u(2,0)+_u(1,0))*1000;
			_eval_dep_u(1000,1) = _ap(1000)*(_u(1000,0)+_u(999,0))/(double)2*(-_u(1000,0)+_u(999,0))*1000+(1-_ap(1000))*(_u(1,0)+_u(1000,0))*(-_u(1,0)+_u(1000,0))*1000;
			break;
		}
		case _eval_u(1000,0): {
			_eval_dep_u(1,1) = _ap(1)*(_u(1,0)+_u(1000,0))/(double)2*(-_u(1,0)+_u(1000,0))*1000+(1-_ap(1))*(_u(2,0)+_u(1,0))/2*(-_u(2,0)+_u(1,0))*1000;
			_eval_dep_u(1000,1) = _ap(1000)*(_u(1000,0)+_u(999,0))/(double)2*(-_u(1000,0)+_u(999,0))*1000+(1-_ap(1000))*(_u(1,0)+_u(1000,0))*(-_u(1,0)+_u(1000,0))*1000;
			break;
		}
		case _eval_u(2,0): {
			_eval_dep_u(1,1) = _ap(1)*(_u(1,0)+_u(1000,0))/(double)2*(-_u(1,0)+_u(1000,0))*1000+(1-_ap(1))*(_u(2,0)+_u(1,0))/2*(-_u(2,0)+_u(1,0))*1000;
			break;
		}
		case _eval_u(999,0): {
			_eval_dep_u(1000,1) = _ap(1000)*(_u(1000,0)+_u(999,0))/(double)2*(-_u(1000,0)+_u(999,0))*1000+(1-_ap(1000))*(_u(1,0)+_u(1000,0))*(-_u(1,0)+_u(1000,0))*1000;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1-1);
		if ((j >= 2 && j <= 999)) {
			_eval_dep_u(j,1) = _ap(j)*(_u(j,0)+_u(j-1,0))/(double)2*(-_u(j,0)+_u(j-1,0))*1000+(1-_ap(j))*(_u(j+1,0)+_u(j,0))*(-_u(j+1,0)+_u(j,0))*1000;
	
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 999)) {
			_eval_dep_u(j,1) = _ap(j)*(_u(j,0)+_u(j-1,0))/(double)2*(-_u(j,0)+_u(j-1,0))*1000+(1-_ap(j))*(_u(j+1,0)+_u(j,0))*(-_u(j+1,0)+_u(j,0))*1000;
	
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1+1);
		if ((j >= 2 && j <= 999)) {
			_eval_dep_u(j,1) = _ap(j)*(_u(j,0)+_u(j-1,0))/(double)2*(-_u(j,0)+_u(j-1,0))*1000+(1-_ap(j))*(_u(j+1,0)+_u(j,0))*(-_u(j+1,0)+_u(j,0))*1000;
	
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
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_eval_dep_u(1,1) = _ap(1)*(_u(1,0)+_u(1000,0))/(double)2*(-_u(1,0)+_u(1000,0))*1000+(1-_ap(1))*(_u(2,0)+_u(1,0))/2*(-_u(2,0)+_u(1,0))*1000;
	
	
			continue;
		}
		case _eval_u(1000,0): {
			_eval_dep_u(1000,1) = _ap(1000)*(_u(1000,0)+_u(999,0))/(double)2*(-_u(1000,0)+_u(999,0))*1000+(1-_ap(1000))*(_u(1,0)+_u(1000,0))*(-_u(1,0)+_u(1000,0))*1000;
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 999)) {
			_eval_dep_u(j,1) = _ap(j)*(_u(j,0)+_u(j-1,0))/(double)2*(-_u(j,0)+_u(j-1,0))*1000+(1-_ap(j))*(_u(j+1,0)+_u(j,0))*(-_u(j+1,0)+_u(j,0))*1000;
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(1000,1000,1000,0,0,3,0,"burgers");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(1000*sizeof(int));
	int* discretes = (int*) malloc(1000*sizeof(int));
	int* events = (int*) malloc(1000*sizeof(int));
	int* outputs = (int*) malloc(5*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	int j;
	_ap(_d1) = 0;
	_beta = 100;
	_inVal = 1;
	_outVal = 1;
	_pi = 3.141590e+00;
	_x0 = 5.000000e-01;
	for(i = 1; i<=1000; i+=1) {
		_x(i) = 1.000000e+00*i/(double)1000;
		_init_u(i,0) = sin(_pi*2*_x(i));
		if(_init_u(i,0)>0) {
			_ap(i) = 1;
		}	else {
			_ap(i) = 0;
		}
	}
	modelData->nSD[_idx_u(1,0)]++;
	modelData->nSD[_idx_u(1,0)]++;
	modelData->nSD[_idx_u(1000,0)]++;
	modelData->nSD[_idx_u(1000,0)]++;
	modelData->nSD[_idx_u(2,0)]++;
	modelData->nSD[_idx_u(999,0)]++;
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,0)]++;
	}
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(1000,0)]++;
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(1000,0)]++;
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(1000,0)]++;
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 998; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 999)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSZ[_idx_u(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nZS[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 1000);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1000,0);
	modelData->SD[_idx_u(1000,0)][states[_idx_u(1000,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(1000,0)][states[_idx_u(1000,0)]++] = _idx_u(1000,0);
	modelData->SD[_idx_u(2,0)][states[_idx_u(2,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(999,0)][states[_idx_u(999,0)]++] = _idx_u(1000,0);
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,0)][states[_idx_u(_d1+1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,0)][states[_idx_u(_d1-1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 1000);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->DS[_idx_u(1000,0)][states[_idx_u(1000,0)]++] = _idx_u(1,0);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1000,0);
	modelData->DS[_idx_u(1000,0)][states[_idx_u(1000,0)]++] = _idx_u(1000,0);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(2,0);
	modelData->DS[_idx_u(1000,0)][states[_idx_u(1000,0)]++] = _idx_u(999,0);
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1+1,0);
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1-1,0);
	}
	cleanVector(states, 0, 1000);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(1000,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(2,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 1000);
	for(row = 1; row <= 998; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			x_ind = _idx_u(_d1+1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 999)) {
			x_ind = _idx_u(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			x_ind = _idx_u(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 1000);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(1000,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(999,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SZ[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_event_1(_d1);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->ZS[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_u(_d1,0);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_u(1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_u(1000,0);
	}
	for(_d1 = 2; _d1<=999; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_u(i,0);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_ap(_d1);
	}
	for(i = 1; i<=1000; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 0;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(1000,1000,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.01;
		simulator->output = SD_Output("burgers",5,1000,1000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	for(_d1 = 1; _d1<=5; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=5; _d1+=1) {
		modelOutput->nSO[_idx_u(200*_d1-199,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 1000, 1000);
	for(_d1 = 1; _d1<=5; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_1(_d1)].name, "u[%d]",200*_d1-199);
	}
	cleanVector(outputs, 0, 5);
	for(_d1 = 1; _d1<=5; _d1+=1) {
		modelOutput->OS[_idx_out_exp_1(_d1)][outputs[_idx_out_exp_1(_d1)]++] = _idx_u(200*_d1-199,0);
	}
	cleanVector(states, 0, 1000);
	for(_d1 = 1; _d1<=5; _d1+=1) {
		modelOutput->SO[_idx_u(200*_d1-199,0)][states[_idx_u(200*_d1-199,0)]++] = _idx_out_exp_1(_d1);
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

