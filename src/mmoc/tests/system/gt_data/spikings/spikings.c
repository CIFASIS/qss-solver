#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "spikings.h"
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
	if (_is_var_v(idx)) {
		_get_v_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_der_v(i,0) = 4.000000e-02*_v(i,0)*_v(i,0)+5*_v(i,0)+140-_u(i,0)+_I(i);
			_der_v(i,1) = (0)/2;
			_der_v(i,2) = (0)/6;
	
		}
		return;
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 1000)) {
			_der_u(i,0) = _a*(_b*_v(i,0)-_u(i,0));
			_der_u(i,1) = (0)/2;
			_der_u(i,2) = (0)/6;
	
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
			_zc(0) = _v(i,0)-(30);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
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
			_init_v(i,0) = _c(i);
			_init_u(i,0) = _init_u(i,0)+_d(i);
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
			_out = _v(1,0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _v(2,0);
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
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_v_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_u(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_v(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_u(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_v(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 1000; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 1
	for (row = 0; row < 1000; row++) {
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
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_v(i,1) = 4.000000e-02*_v(i,0)*_v(i,0)+5*_v(i,0)+140-_u(i,0)+_I(i);
			_eval_dep_v(i,2) = (0)/2;
			_eval_dep_v(i,3) = (0)/6;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_u(i,1) = _a*(_b*_v(i,0)-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
			_eval_dep_u(i,3) = (0)/6;
		}
	}
	if (_is_var_v(idx)) {
		_get_v_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_v(i,1) = 4.000000e-02*_v(i,0)*_v(i,0)+5*_v(i,0)+140-_u(i,0)+_I(i);
			_eval_dep_v(i,2) = (0)/2;
			_eval_dep_v(i,3) = (0)/6;
		}
	}
	if (_is_var_v(idx)) {
		_get_v_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_u(i,1) = _a*(_b*_v(i,0)-_u(i,0));
			_eval_dep_u(i,2) = (0)/2;
			_eval_dep_u(i,3) = (0)/6;
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
	if (_is_var_v(idx)) {
		_get_v_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_v(i,1) = 4.000000e-02*_v(i,0)*_v(i,0)+5*_v(i,0)+140-_u(i,0)+_I(i);
	
	
		}
		continue;
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_u(i,1) = _a*(_b*_v(i,0)-_u(i,0));
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(2000,0,1000,0,0,2,0,"spikings");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(2000*sizeof(int));
	int* events = (int*) malloc(1000*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_a = 2.000000e-02;
	_b = 2.000000e-01;
	for(i = 1; i<=1000; i+=1) {
		_init_v(i,0) = -60;
		_init_u(i,0) = 0;
		_r(i) = 1;
		_c(i) = -65+15*pow(_r(i),2);
		_d(i) = 8-6*pow(_r(i),2);
		_I(i) = 10;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSD[_idx_v(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSD[_idx_v(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nDS[_idx_v(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nDS[_idx_v(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_v_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nSZ[_idx_v(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nZS[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSSt++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSSt++;
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nRHSSt++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 2000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_v(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SD[_idx_v(_d1,0)][states[_idx_v(_d1,0)]++] = _idx_v(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SD[_idx_v(_d1,0)][states[_idx_v(_d1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 2000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->DS[_idx_v(_d1,0)][states[_idx_v(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->DS[_idx_v(_d1,0)][states[_idx_v(_d1,0)]++] = _idx_v(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_v(_d1,0);
	}
	cleanVector(states, 0, 2000);
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_v_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_u(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_v(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2000);
	for(row = 1; row <= 1000; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_u(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 1000)) {
			x_ind = _idx_v(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2000);
	cleanVector(states, 0, 2000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->SZ[_idx_v(_d1,0)][states[_idx_v(_d1,0)]++] = _idx_event_1(_d1);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->ZS[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_v(_d1,0);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSSt[events[_idx_event_1(_d1)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSSt[events[_idx_event_1(_d1)]++] = _idx_v(_d1,0);
	}
	cleanVector(events, 0, 1000);
	for(_d1 = 1; _d1<=1000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].RHSSt[events[_idx_event_1(_d1)]++] = _idx_u(_d1,0);
	}
	for(i = 1; i<=1000; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 1;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(2000,1000,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("spikings",2,0,2000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nSO[_idx_v(1,0)]++;
	modelOutput->nSO[_idx_v(2,0)]++;
	SD_allocOutputMatrix(modelOutput, 2000, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "v[1]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "v[2]");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_v(1,0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_v(2,0);
	cleanVector(states, 0, 2000);
	modelOutput->SO[_idx_v(1,0)][states[_idx_v(1,0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_v(2,0)][states[_idx_v(2,0)]++] = _idx_out_exp_2;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

