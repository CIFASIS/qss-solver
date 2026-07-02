#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "virus_replication.h"
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
	settings->method = 0;
	settings->random_seed = 1;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_I(0): {
			_der_I(0) = -_omega*(_I(0)-_mu)+_sigma*_dW/_h;
	
	
			return;
		}
		case _eval_X(1,0): {
			_der_X(1,0) = _I(0)-_k*_X(1,0);
	
	
			return;
		}
	}
	if (_is_var_X(idx)) {
		_get_X_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 10)) {
			_der_X(i,0) = _k*_X(i-1,0)-_k*_X(i,0);
	
	
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
		if ((i >= 1 && i <= 10000)) {
			_zc(0) = _time-(_TEvent(i)-_h);
	
	
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
		if ((i >= 1 && i <= 10000)) {
			_dW = _dWval(i);
			}
			return;
	}
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
			_out = _X(_d1,0);
		}
		return;
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_I(0): {
			_eval_dep_I(1) = -_omega*(_I(0)-_mu)+_sigma*_dW/_h;
			_eval_dep_X(1,1) = _I(0)-_k*_X(1,0);
			break;
		}
		case _eval_X(1,0): {
			_eval_dep_X(1,1) = _I(0)-_k*_X(1,0);
			break;
		}
	}
	if (_is_var_X(idx)) {
		_get_X_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 10)) {
			_eval_dep_X(i,1) = _k*_X(i-1,0)-_k*_X(i,0);
	
		}
	}
	if (_is_var_X(idx)) {
		_get_X_idxs(idx);
		_apply_usage_eq_3(_d1+1);
		if ((i >= 2 && i <= 10)) {
			_eval_dep_X(i,1) = _k*_X(i-1,0)-_k*_X(i,0);
	
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
		case _eval_I(0): {
			_eval_dep_I(1) = -_omega*(_I(0)-_mu)+_sigma*_dW/_h;
	
	
			continue;
		}
		case _eval_X(1,0): {
			_eval_dep_X(1,1) = _I(0)-_k*_X(1,0);
	
	
			continue;
		}
	}
	if (_is_var_X(idx)) {
		_get_X_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 10)) {
			_eval_dep_X(i,1) = _k*_X(i-1,0)-_k*_X(i,0);
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(11,1,10000,0,0,3,0,"virus_replication");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(11*sizeof(int));
	int* discretes = (int*) malloc(1*sizeof(int));
	int* events = (int*) malloc(10000*sizeof(int));
	int* outputs = (int*) malloc(10*sizeof(int));
	int _d1;
	int i;
	_init_I(0) = 0;
	_di = 10000001/(double)10000;
	_k = 1;
	_mu = 1;
	_omega = 1;
	_sigma = 5.000000e-01;
	_tf = 100;
	_h = _tf/(double)10000;
	_hmin = _tf/(double)10000001;
	_W(1) = __math__normal(1);
	for(i = 2; i<=10000001; i+=1) {
		_W(i) = _W(i-1)+__math__normal(1)*sqrt(_hmin);
	}
	for(i = 1; i<=10000; i+=1) {
		_dWval(i) = _W(i*_di+1)-_W((i-1)*_di+1);
		_TEvent(i) = i*_h;
	}
	_dW = _dWval(1);
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->nSD[_idx_X(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->nSD[_idx_X(_d1-1,0)]++;
	}
	modelData->nSD[_idx_I(0)]++;
	modelData->nSD[_idx_I(0)]++;
	modelData->nSD[_idx_X(1,0)]++;
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->nDS[_idx_X(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->nDS[_idx_X(_d1,0)]++;
	}
	modelData->nDS[_idx_I(0)]++;
	modelData->nDS[_idx_X(1,0)]++;
	modelData->nDS[_idx_X(1,0)]++;
	for(_d1 = 1; _d1<=10000; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=10000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 11);
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->SD[_idx_X(_d1,0)][states[_idx_X(_d1,0)]++] = _idx_X(_d1,0);
	}
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->SD[_idx_X(_d1-1,0)][states[_idx_X(_d1-1,0)]++] = _idx_X(_d1,0);
	}
	modelData->SD[_idx_I(0)][states[_idx_I(0)]++] = _idx_I(0);
	modelData->SD[_idx_I(0)][states[_idx_I(0)]++] = _idx_X(1,0);
	modelData->SD[_idx_X(1,0)][states[_idx_X(1,0)]++] = _idx_X(1,0);
	cleanVector(states, 0, 11);
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->DS[_idx_X(_d1,0)][states[_idx_X(_d1,0)]++] = _idx_X(_d1,0);
	}
	for(_d1 = 2; _d1<=10; _d1+=1) {
		modelData->DS[_idx_X(_d1,0)][states[_idx_X(_d1,0)]++] = _idx_X(_d1-1,0);
	}
	modelData->DS[_idx_I(0)][states[_idx_I(0)]++] = _idx_I(0);
	modelData->DS[_idx_X(1,0)][states[_idx_X(1,0)]++] = _idx_I(0);
	modelData->DS[_idx_X(1,0)][states[_idx_X(1,0)]++] = _idx_X(1,0);
	cleanVector(events, 0, 10000);
	for(_d1 = 1; _d1<=10000; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_I(0);
	}
	cleanVector(events, 0, 10000);
	for(_d1 = 1; _d1<=10000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_dW;
	}
	for(i = 1; i<=10000; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 1;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	simulator->time = QSS_Time(11,10000,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.1;
		simulator->output = SD_Output("virus_replication",10,1,11,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nSO[_idx_X(_d1,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 11, 1);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_1(_d1)].name, "X[%d]",_d1);
	}
	cleanVector(outputs, 0, 10);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->OS[_idx_out_exp_1(_d1)][outputs[_idx_out_exp_1(_d1)]++] = _idx_X(_d1,0);
	}
	cleanVector(states, 0, 11);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->SO[_idx_X(_d1,0)][states[_idx_X(_d1,0)]++] = _idx_out_exp_1(_d1);
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

