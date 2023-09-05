#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mliqss_adr.h"
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
	settings->method = 15;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_u(1,0): {
			_der_u(1,0) = -_a*(_u(1,0)-1)/_dx+_d*(_u(2,0)-2*_u(1,0)+1)/(pow(_dx,2))+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_der_u(1,1) = (0)/2;
	
			return;
		}
		case _eval_u(100,0): {
			_der_u(100,0) = -_a*(_u(100,0)-_u(99,0))/_dx+_d*(_u(99,0)-2*_u(100,0)+_u(99,0))/(pow(_dx,2))+_r*(pow(_u(100,0),2))*(1-_u(100,0));
			_der_u(100,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_der_u(i,0) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/(pow(_dx,2))+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_der_u(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	int _d1;
	if (_is_var_out_exp_1(idx)) {
		_get_out_exp_1_idxs(idx);
		_apply_usage_out_exp_1(_d1);
		if ((_d1 >= 1 && _d1 <= 10)) {
			_out = _u(10*_d1-9,0);
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
		x_ind = _idx_u(1,0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_u(2,0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 98; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 100)) {
			x_ind = _idx_u(_d1+1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 99)) {
			x_ind = _idx_u(_d1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 98)) {
			x_ind = _idx_u(_d1-1,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(_d1);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(100,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_u(99,0);
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
	for (row = 0; row < 98; row++) {
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
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d*(_u(2,0)-2*_u(1,0)+1)/(pow(_dx,2))+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(100,0): {
			_eval_dep_u(100,1) = -_a*(_u(100,0)-_u(99,0))/_dx+_d*(_u(99,0)-2*_u(100,0)+_u(99,0))/(pow(_dx,2))+_r*(pow(_u(100,0),2))*(1-_u(100,0));
			_eval_dep_u(100,2) = (0)/2;
			break;
		}
		case _eval_u(2,0): {
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d*(_u(2,0)-2*_u(1,0)+1)/(pow(_dx,2))+_r*(pow(_u(1,0),2))*(1-_u(1,0));
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(99,0): {
			_eval_dep_u(100,1) = -_a*(_u(100,0)-_u(99,0))/_dx+_d*(_u(99,0)-2*_u(100,0)+_u(99,0))/(pow(_dx,2))+_r*(pow(_u(100,0),2))*(1-_u(100,0));
			_eval_dep_u(100,2) = (0)/2;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1-1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/(pow(_dx,2))+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/(pow(_dx,2))+_r*(pow(_u(i,0),2))*(1-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1+1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/(pow(_dx,2))+_r*(pow(_u(i,0),2))*(1-_u(i,0));
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
			_eval_dep_u(1,1) = -_a*(_u(1,0)-1)/_dx+_d*(_u(2,0)-2*_u(1,0)+1)/(pow(_dx,2))+_r*(pow(_u(1,0),2))*(1-_u(1,0));
	
	
			continue;
		}
		case _eval_u(100,0): {
			_eval_dep_u(100,1) = -_a*(_u(100,0)-_u(99,0))/_dx+_d*(_u(99,0)-2*_u(100,0)+_u(99,0))/(pow(_dx,2))+_r*(pow(_u(100,0),2))*(1-_u(100,0));
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = -_a*(_u(i,0)-_u(i-1,0))/_dx+_d*(_u(i+1,0)-2*_u(i,0)+_u(i-1,0))/(pow(_dx,2))+_r*(pow(_u(i,0),2))*(1-_u(i,0));
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(100,0,0,0,0,3,0,"mliqss_adr");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(10*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_a = 1;
	_d = 1.000000e-03;
	_dx = 10/(double)100;
	_r = 1000;
	for(i = 1; i<=20; i+=1) {
		_init_u(i,0) = 1;
	}
	modelData->nSD[_idx_u(1,0)]++;
	modelData->nSD[_idx_u(100,0)]++;
	modelData->nSD[_idx_u(2,0)]++;
	modelData->nSD[_idx_u(99,0)]++;
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,0)]++;
	}
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(100,0)]++;
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(100,0)]++;
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 98; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 100)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 99)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 98)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 100);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(100,0);
	modelData->SD[_idx_u(2,0)][states[_idx_u(2,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(99,0)][states[_idx_u(99,0)]++] = _idx_u(100,0);
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,0)][states[_idx_u(_d1+1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,0)][states[_idx_u(_d1-1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 100);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->DS[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(100,0);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(2,0);
	modelData->DS[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(99,0);
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1+1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1-1,0);
	}
	cleanVector(states, 0, 100);
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
	cleanVector(states, 0, 100);
	for(row = 1; row <= 98; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 100)) {
			x_ind = _idx_u(_d1+1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 99)) {
			x_ind = _idx_u(_d1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 98)) {
			x_ind = _idx_u(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(100,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(99,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(100,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("mliqss_adr",10,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nSO[_idx_u(10*_d1-9,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 100, 0);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_1(_d1)].name, "u[%d]",10*_d1-9);
	}
	cleanVector(outputs, 0, 10);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->OS[_idx_out_exp_1(_d1)][outputs[_idx_out_exp_1(_d1)]++] = _idx_u(10*_d1-9,0);
	}
	cleanVector(states, 0, 100);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->SO[_idx_u(10*_d1-9,0)][states[_idx_u(10*_d1-9,0)]++] = _idx_out_exp_1(_d1);
	}
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}
