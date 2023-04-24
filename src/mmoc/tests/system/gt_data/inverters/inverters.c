#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "inverters.h"
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
	int i;
	switch(idx) {
		case _eval_x(1,0): {
			_der_x(1,0) = _uslope;
			_der_x(1,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 2 && i <= 502)) {
			_der_x(i,0) = _UOP-_x(i,0)-_G*(_satx(i-1)*pow((_x(i-1,0)-_UTH),2)-_satdx(i-1)*pow((_x(i-1,0)-_x(i,0)-_UTH),2));
			_der_x(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_3: {
			_zc(0) = _time-(_nextchange);
			_zc(1) = (0)/1;
	
			return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 501)) {
			_zc(0) = _x(i,0)-(_UTH);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 501)) {
			_zc(0) = _x(i,0)-_x(i+1,0)-(_UTH);
			_zc(1) = (0)/1;
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_3: {
			if(_nextchange==5) {
		_uslope = 1;
		_nextchange = 10;
	}else if(_nextchange==10) {
		_uslope = 0;
		_nextchange = 15;
	}else if(_nextchange==15) {
		_uslope = -2.5;
		_nextchange = 17;
	}else {
		_uslope = 0;
		_nextchange = 1e+10;
	}
					return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 501)) {
			_satx(i) = 1;
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 501)) {
			_satdx(i) = 1;
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
		if ((i >= 1 && i <= 501)) {
			_satx(i) = 0;
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 501)) {
			_satdx(i) = 0;
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _x(501,0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _x(1,0);
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
	}
	for(row = 1; row <= 501; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 502)) {
			x_ind = _idx_x(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 501)) {
			x_ind = _idx_x(_d1-1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
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
	for (row = 0; row < 501; row++) {
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
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 2 && i <= 502)) {
			_eval_dep_x(i,1) = _UOP-_x(i,0)-_G*(_satx(i-1)*pow((_x(i-1,0)-_UTH),2)-_satdx(i-1)*pow((_x(i-1,0)-_x(i,0)-_UTH),2));
			_eval_dep_x(i,2) = (0)/2;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_2(_d1+1);
		if ((i >= 2 && i <= 502)) {
			_eval_dep_x(i,1) = _UOP-_x(i,0)-_G*(_satx(i-1)*pow((_x(i-1,0)-_UTH),2)-_satdx(i-1)*pow((_x(i-1,0)-_x(i,0)-_UTH),2));
			_eval_dep_x(i,2) = (0)/2;
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
		case _eval_x(1,0): {
			_eval_dep_x(1,1) = _uslope;
	
	
			continue;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 2 && i <= 502)) {
			_eval_dep_x(i,1) = _UOP-_x(i,0)-_G*(_satx(i-1)*pow((_x(i-1,0)-_UTH),2)-_satdx(i-1)*pow((_x(i-1,0)-_x(i,0)-_UTH),2));
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(502,1004,1003,0,0,2,0,"inverters");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(502*sizeof(int));
	int* discretes = (int*) malloc(1004*sizeof(int));
	int* events = (int*) malloc(1003*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_G = 100;
	_UOP = 5;
	_UTH = 1;
	_nextchange = 5;
	for(i = 1; i<=250; i+=1) {
		_init_x(2*i,0) = 5;
		_init_x(2*i+1,0) = 0.006247;
	}
	for(i = 1; i<=501; i+=1) {
		if(_init_x(i,0)>_UTH) {
			_satx(i) = 1;
		}	else {
			_satx(i) = 0;
		}
		if(_init_x(i,0)-_init_x(i+1,0)>_UTH) {
			_satdx(i) = 1;
		}	else {
			_satdx(i) = 0;
		}
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->nSD[_idx_x(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->nSD[_idx_x(_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 501; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 502)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 501)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nSZ[_idx_x(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nSZ[_idx_x(_d1+1,0)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nSZ[_idx_x(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nZS[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nZS[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nZS[_idx_event_2(_d1)]++;
	}
	modelData->nHZ[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->nHD[_idx_event_2(_d1)]++;
	}
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 502);
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->SD[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1,0);
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->SD[_idx_x(_d1-1,0)][states[_idx_x(_d1-1,0)]++] = _idx_x(_d1,0);
	}
	cleanVector(states, 0, 502);
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1,0);
	}
	for(_d1 = 2; _d1<=502; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1-1,0);
	}
	cleanVector(states, 0, 502);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(states, 0, 502);
	for(row = 1; row <= 501; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_x_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 502)) {
			x_ind = _idx_x(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 501)) {
			x_ind = _idx_x(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 502);
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->SZ[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->SZ[_idx_x(_d1+1,0)][states[_idx_x(_d1+1,0)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->SZ[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 1003);
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->ZS[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_x(_d1,0);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->ZS[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_x(_d1+1,0);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->ZS[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_x(_d1,0);
	}
	cleanVector(events, 0, 1003);
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	cleanVector(events, 0, 1003);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_x(1,0);
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_x(_d1+1,0);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->HD[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_x(_d1+1,0);
	}
	cleanVector(events, 0, 1003);
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_nextchange;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_uslope;
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_satdx(_d1);
	}
	for(_d1 = 1; _d1<=501; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_satx(_d1);
	}
	for(i = 1; i<=501; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 0;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	for(i = 1; i<=501; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 0;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	modelData->event[_idx_event_3].direction = 1;
	modelData->event[_idx_event_3].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(502,1003,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("inverters",2,1004,502,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_x(1,0)]++;
	modelOutput->nSO[_idx_x(501,0)]++;
	SD_allocOutputMatrix(modelOutput, 502, 1004);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "x[501]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "x[1]");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_x(1,0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_x(501,0);
	cleanVector(states, 0, 502);
	modelOutput->SO[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_x(501,0)][states[_idx_x(501,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

