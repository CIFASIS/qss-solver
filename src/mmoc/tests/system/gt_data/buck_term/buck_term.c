#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "buck_term.h"
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
		case _eval_iL(0): {
			_iD(0) = (_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_der_iL(0) = (-_iD(0)*_Rd-_uC(0))/_L;
			_der_iL(1) = (0)/2;
	
			return;
		}
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R0)/_C;
			_der_uC(1) = (0)/2;
	
			return;
		}
		case _eval_u(1,0): {
			_pS(0) = (_on+_off);
			_pS(1) = 0;
			_der_u(1,0) = _sigma*(_pS(0)*_c+298-2*_u(1,0)+_u(2,0))/(double)(_l*_l/100/100);
			_der_u(1,1) = (0)/2;
	
			return;
		}
		case _eval_u(100,0): {
			_der_u(100,0) = _sigma*(298-2*_u(100,0)+_u(99,0))/(_l*_l/100/100);
			_der_u(100,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_der_u(i,0) = _sigma*(_u(i-1,0)-2*_u(i,0)+_u(i+1,0))/(_l*_l/100/100);
			_der_u(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_nextT);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _time-_lastT-1.300000e-02*_Ts-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_3: {
			_zc(0) = _time-_lastT-_DC*_Ts-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_4: {
			_zc(0) = _time-_lastT-_DC*_Ts-5.500000e-02*_Ts-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_5: {
			_iD(0) = (_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_s(0) = _diodeon*_iD(0)+(1-_diodeon)*_iD(0)*_Rd;
			_s(1) = 0;
			_zc(0) = _s(0)-(0);
			_zc(1) = (0)/1;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_lastT = _nextT;
			_nextT = _nextT+_Ts;
			_Rs = _ROn;
			_on = 1.000000e+02;
					return;
		}
		case _eval_event_2: {
			_on = 0;
					return;
		}
		case _eval_event_3: {
			_Rs = _ROff;
			_off = 1.000000e+02;
					return;
		}
		case _eval_event_4: {
			_off = 0;
					return;
		}
		case _eval_event_5: {
			_Rd = _ROn;
			_diodeon = 1;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_5: {
			_Rd = _ROff;
			_diodeon = 0;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _u(50,0);
			return;
		}
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
		case _eval_iL(0): {
			_iD(0) = (_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_eval_dep_iL(1) = (-_iD(0)*_Rd-_uC(0))/_L;
			_eval_dep_iL(2) = (0)/2;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R0)/_C;
			_eval_dep_uC(2) = (0)/2;
			break;
		}
		case _eval_u(1,0): {
			_pS(0) = (_on+_off);
			_pS(1) = 0;
			_eval_dep_u(1,1) = _sigma*(_pS(0)*_c+298-2*_u(1,0)+_u(2,0))/(double)(_l*_l/100/100);
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(100,0): {
			_eval_dep_u(100,1) = _sigma*(298-2*_u(100,0)+_u(99,0))/(_l*_l/100/100);
			_eval_dep_u(100,2) = (0)/2;
			break;
		}
		case _eval_u(2,0): {
			_pS(0) = (_on+_off);
			_pS(1) = 0;
			_eval_dep_u(1,1) = _sigma*(_pS(0)*_c+298-2*_u(1,0)+_u(2,0))/(double)(_l*_l/100/100);
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
		case _eval_u(99,0): {
			_eval_dep_u(100,1) = _sigma*(298-2*_u(100,0)+_u(99,0))/(_l*_l/100/100);
			_eval_dep_u(100,2) = (0)/2;
			break;
		}
		case _eval_uC(0): {
			_iD(0) = (_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_eval_dep_iL(1) = (-_iD(0)*_Rd-_uC(0))/_L;
			_eval_dep_iL(2) = (0)/2;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R0)/_C;
			_eval_dep_uC(2) = (0)/2;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1-1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = _sigma*(_u(i-1,0)-2*_u(i,0)+_u(i+1,0))/(_l*_l/100/100);
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = _sigma*(_u(i-1,0)-2*_u(i,0)+_u(i+1,0))/(_l*_l/100/100);
			_eval_dep_u(i,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1+1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = _sigma*(_u(i-1,0)-2*_u(i,0)+_u(i+1,0))/(_l*_l/100/100);
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
		case _eval_iL(0): {
			_iD(0) = (_iL(0)*_Rs-_U)/(_Rs+_Rd);
	
			_eval_dep_iL(1) = (-_iD(0)*_Rd-_uC(0))/_L;
	
	
			continue;
		}
		case _eval_uC(0): {
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R0)/_C;
	
	
			continue;
		}
		case _eval_u(1,0): {
			_pS(0) = (_on+_off);
	
			_eval_dep_u(1,1) = _sigma*(_pS(0)*_c+298-2*_u(1,0)+_u(2,0))/(double)(_l*_l/100/100);
	
	
			continue;
		}
		case _eval_u(100,0): {
			_eval_dep_u(100,1) = _sigma*(298-2*_u(100,0)+_u(99,0))/(_l*_l/100/100);
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1);
		if ((i >= 2 && i <= 99)) {
			_eval_dep_u(i,1) = _sigma*(_u(i-1,0)-2*_u(i,0)+_u(i+1,0))/(_l*_l/100/100);
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(102,7,5,0,4,5,4,"buck_term");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(4*sizeof(int));
	int* states = (int*) malloc(102*sizeof(int));
	int* discretes = (int*) malloc(7*sizeof(int));
	int* events = (int*) malloc(5*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int _d1;
	_C = 1.000000e-04;
	_DC = 5.000000e-01;
	_L = 1.000000e-04;
	_R0 = 10;
	_ROff = 1.000000e+04;
	_ROn = 1.000000e-04;
	_Rd = 1.000000e+05;
	_Rs = 1.000000e-05;
	_Ts = 1.000000e-04;
	_U = 24;
	_c = 7.500000e+00;
	_l = 2.500000e-02;
	_nextT = _Ts;
	_off = 0;
	_on = 1;
	_sigma = 8.610000e-06;
	_toff = 5.500000e-06;
	_ton = 1.300000e-06;
	for(_d1 = 1; _d1<=100; _d1+=1) {
			_init_u(_d1,0) = 298;
	}
	_vc0 = 2.000000e-01;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_u(1,0)]++;
	modelData->nSD[_idx_u(100,0)]++;
	modelData->nSD[_idx_u(2,0)]++;
	modelData->nSD[_idx_u(99,0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,0)]++;
	}
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(100,0)]++;
	modelData->nDS[_idx_u(1,0)]++;
	modelData->nDS[_idx_u(100,0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nZS[_idx_event_5]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_5]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_5]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_4]++;
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_4].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 102);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(100,0);
	modelData->SD[_idx_u(2,0)][states[_idx_u(2,0)]++] = _idx_u(1,0);
	modelData->SD[_idx_u(99,0)][states[_idx_u(99,0)]++] = _idx_u(100,0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,0)][states[_idx_u(_d1+1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,0)][states[_idx_u(_d1-1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 102);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	modelData->DS[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(100,0);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(2,0);
	modelData->DS[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_u(99,0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1+1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=99; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1-1,0);
	}
	cleanVector(states, 0, 102);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_5;
	cleanVector(events, 0, 5);
	modelData->ZS[_idx_event_5][events[_idx_event_5]++] = _idx_iL(0);
	cleanVector(events, 0, 5);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_4;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_5;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_5;
	modelData->HZ[_idx_event_5][events[_idx_event_5]++] = _idx_event_5;
	cleanVector(events, 0, 5);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_5][events[_idx_event_5]++] = _idx_iL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_u(1,0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_u(1,0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_u(1,0);
	modelData->HD[_idx_event_4][events[_idx_event_4]++] = _idx_u(1,0);
	cleanVector(events, 0, 5);
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_Rd;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rs;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rs;
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_diodeon;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_off;
	modelData->event[_idx_event_4].LHSDsc[events[_idx_event_4]++] = _idx_off;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_on;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_on;
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = 1;
	modelData->event[_idx_event_3].relation = 2;
	modelData->event[_idx_event_4].direction = 1;
	modelData->event[_idx_event_4].relation = 2;
	modelData->event[_idx_event_5].direction = 0;
	modelData->event[_idx_event_5].relation = 2;
	simulator->time = QSS_Time(102,5,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.02;
		simulator->output = SD_Output("buck_term",1,7,102,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(50,0)]++;
	SD_allocOutputMatrix(modelOutput, 102, 7);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[50]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(50,0);
	cleanVector(states, 0, 102);
	modelOutput->SO[_idx_u(50,0)][states[_idx_u(50,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

