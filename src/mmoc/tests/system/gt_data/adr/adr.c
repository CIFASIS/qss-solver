#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "adr.h"
#include <common/utils.h>
#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

void MOD_settings(SD_simulationSettings settings)
{
	settings->debug = 0;
	settings->parallel = FALSE;
	settings->hybrid = FALSE;
	settings->method = 5;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_u(1,0): {
			_der_u(1,0) = -_a*(_u(1,0)-1)/_dx+_d(1)*(_u(2,0)-2*_u(1,0)+1)/_dx2+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_der_u(1,1) = (0)/2;
	
			return;
		}
		case _eval_u(1000,0): {
			_der_u(1000,0) = -_a*(_u(1000,0)-_u(999,0))/_dx+_d(1000)*(_u(1000,0)-2*_u(1000,0)+_u(999,0))/_dx2+_r*(pow(_u(1000,0),2))*(1-_u(1000,0));
			_der_u(1000,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 999)) {
			_der_u(i,0) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d(i)*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/_dx2+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_der_u(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _u(750,0);
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
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1,0);
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
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			x_ind = _idx_u(_d1+1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 999)) {
			x_ind = _idx_u(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			x_ind = _idx_u(_d1-1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1000,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_u(999,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
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
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 998; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 1
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
	    row_t = dvdx->df_dx[1]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_u(1,0): {
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d(1)*(_u(2,0)-2*_u(1,0)+1)/_dx2+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(1000,0): {
			_eval_dep_u(1000,1) = -_a*(_u(1000,0)-_u(999,0))/_dx+_d(1000)*(_u(1000,0)-2*_u(1000,0)+_u(999,0))/_dx2+_r*(pow(_u(1000,0),2))*(1-_u(1000,0));
			_eval_dep_u(1000,2) = (0)/2;
			break;
		}
		case _eval_u(2,0): {
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d(1)*(_u(2,0)-2*_u(1,0)+1)/_dx2+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(999,0): {
			_eval_dep_u(1000,1) = -_a*(_u(1000,0)-_u(999,0))/_dx+_d(1000)*(_u(1000,0)-2*_u(1000,0)+_u(999,0))/_dx2+_r*(pow(_u(1000,0),2))*(1-_u(1000,0));
			_eval_dep_u(1000,2) = (0)/2;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1-1);
		if ((i >= 2 && i <= 999)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d(i)*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/_dx2+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 999)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d(i)*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/_dx2+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1+1);
		if ((i >= 2 && i <= 999)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d(i)*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/_dx2+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
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
	int i;
	switch(idx) {
		case _eval_u(1,0): {
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d(1)*(_u(2,0)-2*_u(1,0)+1)/_dx2+_r*(pow(_u(1,0),2))*(1-_u(1,0));
	
	
			continue;
		}
		case _eval_u(1000,0): {
			_eval_dep_u(1000,1) = -_a*(_u(1000,0)-_u(999,0))/_dx+_d(1000)*(_u(1000,0)-2*_u(1000,0)+_u(999,0))/_dx2+_r*(pow(_u(1000,0),2))*(1-_u(1000,0));
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 999)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d(i)*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/_dx2+_r*(pow(_u(i,0),2))*(1-_u(i,0));
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(1000,0,0,0,0,3,0,"adr");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(1000*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_L = 10;
	_a = 1;
	_dx = _L/1000;
	_dx2 = pow(_dx,2);
	_r = 1000;
	for(i = 500; i<=1000; i+=1) {
		_d(i) = 10;
	}
	for(i = 1; i<=499; i+=1) {
		_d(i) = 1e-06;
	}
	modelData->nSD[_idx_u(1,0)]++;
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
	}
	for(row = 1; row <= 998; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 999)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 1000);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
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
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 1000)) {
			x_ind = _idx_u(_d1+1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 999)) {
			x_ind = _idx_u(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 998)) {
			x_ind = _idx_u(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 1000);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1000,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(999,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 1000);
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(1000,0,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.002;
		simulator->output = SD_Output("adr",1,0,1000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(750,0)]++;
	SD_allocOutputMatrix(modelOutput, 1000, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[750]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(750,0);
	cleanVector(states, 0, 1000);
	modelOutput->SO[_idx_u(750,0)][states[_idx_u(750,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

