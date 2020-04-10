#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cuk2.h"
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
	settings->method = 2;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	switch(idx) {
		case _eval_uC1(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_der_uC1(0) = (_iD(0)-_iL(0))/_C1;
	
			return;
		}
		case _eval_phi(0): {
			_der_phi(0) = (_U+_uC(0)-_uC1(0))/_L;
	
			return;
		}
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
	
			return;
		}
		case _eval_iL(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_der_iL(0) = (-_uC(0)-_iD(0)*_Rd)/_L;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_nextT);
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _time-_lastT-_DC*_T-(0);
	
			return;
		}
		case _eval_event_3: {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_s(0) = _diodeon*_iD(0)+(1-_diodeon)*_iD(0)*_Rd;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_zc(0) = _s(0)-(0);
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_lastT = _nextT;
			_nextT = _nextT+_T;
			_Rs = _ROn;
					return;
		}
		case _eval_event_2: {
			_Rs = _ROff;
					return;
		}
		case _eval_event_3: {
			_Rd = _ROn;
			_diodeon = 1;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_3: {
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
			_out = _iL(0);
			return;
		}
		case _eval_out_exp_2: {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_out = _iL1(0);
			return;
		}
		case _eval_out_exp_3: {
			_out = _uC(0);
			return;
		}
		case _eval_out_exp_4: {
			_out = _uC1(0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, double *jac)
{
	int idx;
	int jit;
	for (idx = 1; idx <=4; idx++) {
	switch(idx) {
		case _eval_iL(0): {
			_jac(jit) = (1/(_C1));
		
			_jac(jit) = (1/(_C));
		
			_jac(jit) = _L*(1/(_L1));
				_jac(jit) = (1/(_Rd+_Rs))*_Rs;
				_jac(jit) = (1/(_C1));
		
			_jac(jit) = _L*(1/(_L1));
				_jac(jit) = (1/(_Rd+_Rs))*_Rs;
				_jac(jit) = 0;
		
			_jac(jit) = (1/(_Rs+_Rd))*_Rs;
				_jac(jit) = (1/(_C1));
		
			_jac(jit) = _Rs*(1/(_Rs+_Rd));
				_jac(jit) = 0;
		
		break;
		}
		case _eval_phi(0): {
			_jac(jit) = _L*(1/(_L1));
				_jac(jit) = 0;
				_jac(jit) = 0;
		
			_jac(jit) = _L*(1/(_L1));
				_jac(jit) = 0;
				_jac(jit) = 0;
		
		break;
		}
		case _eval_uC(0): {
			_jac(jit) = (1/(_L));
		
			_jac(jit) = -(1/(_R))*(1/(_C));
		
			_jac(jit) = -(1/(_L));
		
		break;
		}
		case _eval_uC1(0): {
			_jac(jit) = -(1/(_L));
		
			_jac(jit) = -(1/(_Rd+_Rs));
				_jac(jit) = 0;
		
			_jac(jit) = -(1/(_Rs+_Rd));
				_jac(jit) = 0;
		
		break;
		}
	}
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_iL(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
		
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
		
		break;
		}
		case _eval_phi(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
		
		break;
		}
		case _eval_uC(0): {
			_eval_dep_phi(1) = (_U+_uC(0)-_uC1(0))/_L;
		
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
		
		break;
		}
		case _eval_uC1(0): {
			_eval_dep_phi(1) = (_U+_uC(0)-_uC1(0))/_L;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
		
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
		
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
		case _eval_uC1(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_der_uC1(0) = (_iD(0)-_iL(0))/_C1;
	
			return;
		}
		case _eval_phi(0): {
			_der_phi(0) = (_U+_uC(0)-_uC1(0))/_L;
	
			return;
		}
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
	
			return;
		}
		case _eval_iL(0): {
			_iL1(0) = (_L*_phi(0)+_L*_iL(0))/_L1;
	
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_der_iL(0) = (-_uC(0)-_iD(0)*_Rd)/_L;
	
			return;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(4,5,3,0,3,"cuk2");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(4*sizeof(int));
	int* discretes = (int*) malloc(5*sizeof(int));
	int* events = (int*) malloc(3*sizeof(int));
	int* outputs = (int*) malloc(4*sizeof(int));
	_C = 0.0001;
	_C1 = 0.0001;
	_DC = 0.25;
	_L = 0.0001;
	_L1 = 0.0001;
	_R = 10;
	_ROff = 100000;
	_ROn = 1e-05;
	_Rd = 100000;
	_Rs = 1e-05;
	_T = 0.0001;
	_U = 24;
	_nextT = _T;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_phi(0)]++;
	modelData->nSD[_idx_phi(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_phi(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_phi(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_phi(0)]++;
	modelData->nSZ[_idx_uC1(0)]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_2]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_2]++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 4);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_phi(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_phi(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	cleanVector(states, 0, 4);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_phi(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_phi(0);
	modelData->DS[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_uC(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->DS[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_uC1(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_uC1(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	cleanVector(states, 0, 4);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_3;
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_3;
	modelData->SZ[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_event_3;
	modelData->SZ[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_event_3;
	cleanVector(events, 0, 3);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_phi(0);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_uC1(0);
	cleanVector(events, 0, 3);
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	modelData->HZ[_idx_event_2][events[_idx_event_2]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	cleanVector(events, 0, 3);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_uC1(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_uC1(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_uC1(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_uC1(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	cleanVector(events, 0, 3);
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rd;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rd;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rs;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rs;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_diodeon;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_diodeon;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = 0;
	modelData->event[_idx_event_3].relation = 2;
	simulator->time = QSS_Time(4,3,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("cuk2",4,5,4,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_3]++;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_phi(0)]++;
	modelOutput->nSO[_idx_uC(0)]++;
	modelOutput->nSO[_idx_uC1(0)]++;
	SD_allocOutputMatrix(modelOutput, 4, 5);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "iL");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "iL1");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "uC");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "uC1");
	cleanVector(outputs, 0, 4);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_phi(0);
	modelOutput->OS[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_uC(0);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_uC1(0);
	cleanVector(states, 0, 4);
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_phi(0)][states[_idx_phi(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_out_exp_3;
	modelOutput->SO[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_out_exp_4;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

