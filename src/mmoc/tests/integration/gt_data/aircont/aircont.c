#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "aircont.h"
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
	settings->method = 10;
}

void MOD_definition(double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	for(i = 1; i<=200; i+=1) {
		_der_th(i) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i)/_RES(i)+_noise/_RES(i))/_CAP(i);
	}
	_der_ierr = _pref-_ptotals/_pmax;
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_2: {
			_zc = _time-(1000);
	
			return;
		}
		case _eval_event_3: {
			_zc = _time-(2000);
	
			return;
		}
		case _eval_event_4: {
			_zc = _time-(_nextSample);
	
			return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 200)) {
			_zc = _th(i)-_tref-_dtref+_on(i)-0.5-(0);
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_2: {
			_pref = 0.4;
					return;
		}
		case _eval_event_3: {
			_pref = 0.5;
					return;
		}
		case _eval_event_4: {
			_nextSample = _nextSample+1;
			_noise = __math__rand(2)-1;
			_ptotals = _ptotal;
			_dtref = _Kp*(_ptotals/_pmax-_pref)-_Ki*_ierr;
					return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 200)) {
			_on(i) = 1;
			_ptotal = _ptotal+_POT(i);
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 200)) {
			_on(i) = 0;
			_ptotal = _ptotal-_POT(i);
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _ptotal;
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
	for (idx = 1; idx <=201; idx++) {
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 200)) {
			_jac(jit) = -(1/(_CAP(i)))*(1/(_RES(i)));
		}
	
		}
	}
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(201,206,203,0,0,"aircont");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(201*sizeof(int));
	int* discretes = (int*) malloc(206*sizeof(int));
	int* events = (int*) malloc(203*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int _d1;
	int i;
	_Ki = 1;
	_Kp = 1;
	_THA = 32;
	_nextSample = 1;
	_noise = __math__rand(2)-1;
	_pmax = 0;
	_pref = 0.5;
	_tref = 20;
	for(i = 1; i<=200; i+=1) {
		_init_th(i) = __math__rand(4)+18;
		_CAP(i) = __math__rand(100)+550;
		_RES(i) = __math__rand(0.4)+1.8;
		_POT(i) = __math__rand(0.2)+13;
		_pmax = _pmax+_POT(i);
	}
	for(i = 1; i<=200; i+=1) {
		if(_th(i)-_tref-0.5>0) {
			_on(i) = 1;
			_ptotal = _ptotal+_POT(i);
		}
	}
	for(_d1 = 1; _d1<=200; _d1+=1) {
		modelData->nSD[_idx_th(_d1)]++;
	}
	for(_d1 = 1; _d1<=200; _d1+=1) {
		modelData->nDS[_idx_th(_d1)]++;
	}
	CLC_allocDataMatrix(modelData);
	cleanVector(states, 0, 201);
	for(_d1 = 1; _d1<=200; _d1+=1) {
		modelData->SD[_idx_th(_d1)][states[_idx_th(_d1)]++] = _idx_th(_d1);
	}
	cleanVector(states, 0, 201);
	for(_d1 = 1; _d1<=200; _d1+=1) {
		modelData->DS[_idx_th(_d1)][states[_idx_th(_d1)]++] = _idx_th(_d1);
	}
	for(i = 1; i<=200; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 0;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = 1;
	modelData->event[_idx_event_3].relation = 2;
	modelData->event[_idx_event_4].direction = 1;
	modelData->event[_idx_event_4].relation = 2;
	simulator->output = SD_Output("aircont",1,206,201,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOD[_idx_out_exp_1]++;
	modelOutput->nDO[_idx_ptotal]++;
	SD_allocOutputMatrix(modelOutput, 201, 206);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "ptotal");
	cleanVector(outputs, 0, 1);
	modelOutput->OD[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_ptotal;
	cleanVector(discretes, 0, 206);
	modelOutput->DO[_idx_ptotal][discretes[_idx_ptotal]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

