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
			_der_x(1,1) = (-_x(2,1))/2;
			_der_x(1,2) = (-_x(2,2))/6;
			return;
		}
		case _eval_x(200,0): {
			_der_x(200,0) = _x(200-1,0);
			_der_x(200,1) = (_x(200-1,1))/2;
			_der_x(200,2) = (_x(200-1,2))/6;
			return;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 2 && i <= 199)) {
			_der_x(i,0) = _x(i-1,0)-_x(i+1,0);
			_der_x(i,1) = (-_x(i+1,1)+_x(i-1,1))/2;
			_der_x(i,2) = (_x(i-1,2)-_x(i+1,2))/6;
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(1);
			_zc(1) = (1)/1;
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

void MOD_jacobian(double *x, double *d, double *a, double t, double *jac)
{
	int _d1;
	int i;
	int idx;
	int jit;
	for (idx = 1; idx <=200; idx++) {
	switch(idx) {
		case _eval_x(2,0): {
			_jac(jit) = -1;
		
		break;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
			_jac(jit) = 0;
		
		_apply_usage_eq_1(_d1-1);
		if ((i >= 2 && i <= 199)) {
			_jac(jit) = -1;
		}
	
		_apply_usage_eq_1(_d1+1);
		if ((i >= 2 && i <= 199)) {
			_jac(jit) = 1;
		}
	
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
			_eval_dep_x(1,2) = (-_x(2,1))/2;
			_eval_dep_x(1,3) = (-_x(2,2))/6;	
		break;
		}
	}
	if (_is_var_x(idx)) {
		_get_x_idxs(idx);
			_eval_dep_x(200,1) = _x(200-1,0);
			_eval_dep_x(200,2) = (_x(200-1,1))/2;
			_eval_dep_x(200,3) = (_x(200-1,2))/6;	
		_apply_usage_eq_1(_d1-1);
		if ((i >= 2 && i <= 199)) {
			_eval_dep_x(i,1) = _x(i-1,0)-_x(i+1,0);
			_eval_dep_x(i,2) = (_x(i-1,1)-_x(i+1,1))/2;
			_eval_dep_x(i,3) = (_x(i-1,2)-_x(i+1,2))/6;	}
	
		_apply_usage_eq_1(_d1+1);
		if ((i >= 2 && i <= 199)) {
			_eval_dep_x(i,1) = _x(i-1,0)-_x(i+1,0);
			_eval_dep_x(i,2) = (_x(i-1,1)-_x(i+1,1))/2;
			_eval_dep_x(i,3) = (_x(i-1,2)-_x(i+1,2))/6;	}
	
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
			_der_x(200,0) = _x(200-1,0);
	
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
	simulator->data = QSS_Data(200,1,1,0,0,"lc_line");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(200*sizeof(int));
	int* discretes = (int*) malloc(1*sizeof(int));
	int* events = (int*) malloc(1*sizeof(int));
	int* outputs = (int*) malloc(4*sizeof(int));
	int _d1;
	_d = 1;
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nSD[_idx_x(_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nSD[_idx_x(_d1+1,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nSD[_idx_x(2,0)]++;
	}
	for(_d1 = 200; _d1<=200; _d1+=1) {
		modelData->nSD[_idx_x(199,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->nDS[_idx_x(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nDS[_idx_x(1,0)]++;
	}
	for(_d1 = 200; _d1<=200; _d1+=1) {
		modelData->nDS[_idx_x(200,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nHD[_idx_event_1]++;
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
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->SD[_idx_x(2,0)][states[_idx_x(2,0)]++] = _idx_x(1,0);
	}
	for(_d1 = 200; _d1<=200; _d1+=1) {
		modelData->SD[_idx_x(199,0)][states[_idx_x(199,0)]++] = _idx_x(200,0);
	}
	cleanVector(states, 0, 200);
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1-1,0);
	}
	for(_d1 = 2; _d1<=199; _d1+=1) {
		modelData->DS[_idx_x(_d1,0)][states[_idx_x(_d1,0)]++] = _idx_x(_d1+1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->DS[_idx_x(1,0)][states[_idx_x(1,0)]++] = _idx_x(2,0);
	}
	for(_d1 = 200; _d1<=200; _d1+=1) {
		modelData->DS[_idx_x(200,0)][states[_idx_x(200,0)]++] = _idx_x(199,0);
	}
	cleanVector(events, 0, 1);
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_x(1,0);
	}
	cleanVector(events, 0, 1);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_d;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_d;
	modelData->event[_idx_event_1].direction = 0;
	modelData->event[_idx_event_1].relation = 2;
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

