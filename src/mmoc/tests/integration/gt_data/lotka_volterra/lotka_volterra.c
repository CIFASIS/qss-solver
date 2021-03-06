#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lotka_volterra.h"
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
	settings->method = 6;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	switch(idx) {
		case _eval_x(1,0): {
			_der_x(1,0) = 0.1*_x(1,0)-0.1*_x(1,0)*_x(2,0);
			_der_x(1,1) = (0)/2;
			_der_x(1,2) = (0)/6;
	
			return;
		}
		case _eval_x(2,0): {
			_der_x(2,0) = 0.1*_x(1,0)*_x(2,0)-0.1*_x(2,0);
			_der_x(2,1) = (0)/2;
			_der_x(2,2) = (0)/6;
	
			return;
		}
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
	int _d1;
	if (_is_var_out_exp_1(idx)) {
		_get_out_exp_1_idxs(idx);
		_apply_usage_out_exp_1(_d1);
		if ((_d1 >= 1 && _d1 <= 2)) {
			_out = _x(_d1,0);
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
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			x_ind = _idx_x(1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			x_ind = _idx_x(1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
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
	switch(idx) {
		case _eval_x(1,0): {
			_eval_dep_x(1,1) = 0.1*_x(1,0)-0.1*_x(1,0)*_x(2,0);
			_eval_dep_x(1,2) = (0)/2;
			_eval_dep_x(1,3) = (0)/6;	
			_eval_dep_x(2,1) = 0.1*_x(1,0)*_x(2,0)-0.1*_x(2,0);
			_eval_dep_x(2,2) = (0)/2;
			_eval_dep_x(2,3) = (0)/6;	
			break;
		}
		case _eval_x(2,0): {
			_eval_dep_x(1,1) = 0.1*_x(1,0)-0.1*_x(1,0)*_x(2,0);
			_eval_dep_x(1,2) = (0)/2;
			_eval_dep_x(1,3) = (0)/6;	
			_eval_dep_x(2,1) = 0.1*_x(1,0)*_x(2,0)-0.1*_x(2,0);
			_eval_dep_x(2,2) = (0)/2;
			_eval_dep_x(2,3) = (0)/6;	
			break;
		}
	}
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	switch(idx) {
		case _eval_x(1,0): {
			_der_x(1,0) = 0.1*_x(1,0)-0.1*_x(1,0)*_x(2,0);
	
	
			return;
		}
		case _eval_x(2,0): {
			_der_x(2,0) = 0.1*_x(1,0)*_x(2,0)-0.1*_x(2,0);
	
	
			return;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(2,0,0,0,0,2,0,"lotka_volterra");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(2*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	_init_x(1,0) = 0.5;
	_init_x(2,0) = 0.5;
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nSD[_idx_x(1,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nSD[_idx_x(2,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nSD[_idx_x(1,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nSD[_idx_x(2,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nDS[_idx_x(1,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nDS[_idx_x(1,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nDS[_idx_x(2,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nDS[_idx_x(2,0)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((2 <= 2 && 2 <= 2)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((2 <= 2 && 2 <= 2)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 2);
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->SD[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->SD[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->SD[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(2,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->SD[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(2,0);
	}
	cleanVector(states, 0, 2);
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->DS[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->DS[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(2,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->DS[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->DS[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(2,0);
	}
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			x_ind = _idx_x(1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((1 <= 1 && 1 <= 1)) {
			x_ind = _idx_x(1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2);
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(2,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("lotka_volterra",2,0,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	for(_d1 = 1; _d1<=2; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=2; _d1+=1) {
		modelOutput->nSO[_idx_x(_d1,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 2, 0);
	for(_d1 = 1; _d1<=2; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_1(_d1)].name, "x[%d]",_d1);
	}
	cleanVector(outputs, 0, 2);
	for(_d1 = 1; _d1<=2; _d1+=1) {
		modelOutput->OS[_idx_out_exp_1(_d1)][outputs[_idx_out_exp_1(_d1)]++] = _idx_x(_d1,0);
	}
	cleanVector(states, 0, 2);
	for(_d1 = 1; _d1<=2; _d1+=1) {
		modelOutput->SO[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_out_exp_1(_d1);
	}
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

