#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lc_line.h"
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
	settings->method = 6;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_x(1,0): {
			_der_x(1,0) = _d-_x(2,0);
			_der_x(1,1) = (0)/2;
			_der_x(1,2) = (0)/6;
	
			return;
		}
		case _eval_x(200,0): {
			_der_x(200,0) = _x(199,0);
			_der_x(200,1) = (0)/2;
			_der_x(200,2) = (0)/6;
	
			return;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 2 && i <= 199)) {
			_der_x(i,0) = _x(i-1,0)-_x(i+1,0);
			_der_x(i,1) = (0)/2;
			_der_x(i,2) = (0)/6;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(1);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_d = 0;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_d = 1;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _x(1,0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _x(200,0);
			return;
		}
		case _eval_out_exp_3: {
			_out = _x(101,0);
			return;
		}
		case _eval_out_exp_4: {
			_out = _x(100,0);
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
	for(row = 1; row <= 198; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((1 <= _d1-1 && _d1-1 <= 198)) {
			x_ind = _idx_x(_d1-1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((3 <= _d1 + 1 && _d1 + 1 <= 200)) {
			x_ind = _idx_x(_d1+1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((199 <= 199 && 199 <= 199)) {
			x_ind = _idx_x(199,0);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 198; row++) {
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
	int i;
	switch(idx) {
		case _eval_x(2,0): {
			_eval_dep_x(1,1) = _d-_x(2,0);
			_eval_dep_x(1,2) = (0)/2;
			_eval_dep_x(1,3) = (0)/6;	
			break;
		}
		case _eval_x(199,0): {
			_eval_dep_x(200,1) = _x(199,0);
			_eval_dep_x(200,2) = (0)/2;
			_eval_dep_x(200,3) = (0)/6;	
			break;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_1(_d1+1);
		if ((i >= 2 && i <= 199)) {
			_eval_dep_x(i,1) = _x(i-1,0)-_x(i+1,0);
			_eval_dep_x(i,2) = (0)/2;
			_eval_dep_x(i,3) = (0)/6;	}
	
		_apply_usage_eq_1(_d1-1);
		if ((i >= 2 && i <= 199)) {
			_eval_dep_x(i,1) = _x(i-1,0)-_x(i+1,0);
			_eval_dep_x(i,2) = (0)/2;
			_eval_dep_x(i,3) = (0)/6;	}
	
		}
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int _d1;
	int i;
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	switch(idx) {
		case _eval_x(1,0): {
			_der_x(1,0) = _d-_x(2,0);
	
	
			return;
		}
		case _eval_x(200,0): {
			_der_x(200,0) = _x(199,0);
	
	
			return;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 2 && i <= 199)) {
			_der_x(i,0) = _x(i-1,0)-_x(i+1,0);
	
	
		}
		return;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(200,1,1,0,0,3,0,"lc_line");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(200*sizeof(int));
	int* discretes = (int*) malloc(1*sizeof(int));
	int* events = (int*) malloc(1*sizeof(int));
	int* outputs = (int*) malloc(4*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	_d = 1;
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nSD[_idx_x(_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nSD[_idx_x(_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nSD[_idx_x(2,0)]++;
	}
	for(_d1 = 199; _d1<=199; _d1+=1) {
		modelData->nSD[_idx_x(199,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->nDS[_idx_x(1,0)]++;
	}
	for(_d1 = 199; _d1<=199; _d1+=1) {
		modelData->nDS[_idx_x(200,0)]++;
	}
	for(row = 1; row <= 198; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((1 <= _d1-1 && _d1-1 <= 198)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((3 <= _d1 + 1 && _d1 + 1 <= 200)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((2 <= 2 && 2 <= 2)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((199 <= 199 && 199 <= 199)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
	}
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 200);
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->SD[_idx_x(_d1-1,0)][states[_idx_x(_d1-1,0)]++] = _idx_x(_d1,0);
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->SD[_idx_x(_d1+1,0)][states[_idx_x(_d1+1,0)]++] = _idx_x(_d1,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->SD[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 199; _d1<=199; _d1+=1) {
		modelData->SD[_idx_x(199,0)][states[_idx_x(199,0)]++] = _idx_x(200,0);
	}
	cleanVector(states, 0, 200);
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1-1,0);
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1+1,0);
	}
	for(_d1 = 2; _d1<=2; _d1+=1) {
		modelData->DS[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(2,0);
	}
	for(_d1 = 199; _d1<=199; _d1+=1) {
		modelData->DS[_idx_x(200,0)][states[_idx_x(200,0)]++] = _idx_x(199,0);
	}
	cleanVector(states, 0, 200);
	for(row = 1; row <= 198; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((1 <= _d1-1 && _d1-1 <= 198)) {
			x_ind = _idx_x(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((3 <= _d1 + 1 && _d1 + 1 <= 200)) {
			x_ind = _idx_x(_d1+1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 200);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((2 <= 2 && 2 <= 2)) {
			x_ind = _idx_x(2,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 200);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if((199 <= 199 && 199 <= 199)) {
			x_ind = _idx_x(199,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 200);
	cleanVector(events, 0, 1);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_d;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_d;
	modelData->event[_idx_event_1].direction = 0;
	modelData->event[_idx_event_1].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(200,1,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("lc_line",4,1,200,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_3]++;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nSO[_idx_x(1,0)]++;
	modelOutput->nSO[_idx_x(200,0)]++;
	modelOutput->nSO[_idx_x(101,0)]++;
	modelOutput->nSO[_idx_x(100,0)]++;
	SD_allocOutputMatrix(modelOutput, 200, 1);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "x[1]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "x[200]");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "x[101]");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "x[100]");
	cleanVector(outputs, 0, 4);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_x(1,0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_x(200,0);
	modelOutput->OS[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_x(101,0);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_x(100,0);
	cleanVector(states, 0, 200);
	modelOutput->SO[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_x(200,0)][states[_idx_x(200,0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_x(101,0)][states[_idx_x(101,0)]++] = _idx_out_exp_3;
	modelOutput->SO[_idx_x(100,0)][states[_idx_x(100,0)]++] = _idx_out_exp_4;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

