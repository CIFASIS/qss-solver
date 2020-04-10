#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "buck_circuit.h"
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
	switch(idx) {
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
			_der_uC(1) = (-(1/(_C))*((1/(_R))*_uC(1)-_iL(1)))/2;
			return;
		}
		case _eval_iL(0): {
			_der_iL(0) = (((_U/_Rs)-_iL(0))*(_Rs*_Rd/(_Rs+_Rd))-_uC(0))/_L;
			_der_iL(1) = (-(_uC(1)+_Rs*(1/(_Rs+_Rd))*_iL(1)*_Rd)*(1/(_L)))/2;
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_nextT);
			_zc(1) = (1)/1;
			return;
		}
		case _eval_event_2: {
			_zc(0) = _time-_lastT-_DC*_T-(0);
			_zc(1) = (1)/1;
			return;
		}
		case _eval_event_3: {
			_zc(0) = _iL(0)-(0);
			_zc(1) = (_iL(1))/1;
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
			_Rd = _ROff;
					return;
		}
		case _eval_event_2: {
			_Rs = _ROff;
			_Rd = _ROn;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_3: {
			_Rd = _ROff;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _uC(0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _iL(0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, double *jac)
{
	int idx;
	int jit;
	for (idx = 1; idx <=2; idx++) {
	switch(idx) {
		case _eval_iL(0): {
			_jac(jit) = (1/(_C));
		
			_jac(jit) = -(1/(_L))*_Rs*(1/(_Rs+_Rd))*_Rd;
		
		break;
		}
		case _eval_uC(0): {
			_jac(jit) = -(1/(_C))*(1/(_R));
		
			_jac(jit) = -(1/(_L));
		
		break;
		}
	}
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_iL(0): {
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (-(1/(_C))*((1/(_R))*_uC(1)-_iL(1)))/2;	
			_eval_dep_iL(1) = (((_U/_Rs)-_iL(0))*(_Rs*_Rd/(_Rs+_Rd))-_uC(0))/_L;
			_eval_dep_iL(2) = (-(_iL(1)*_Rd*_Rs*(1/(_Rd+_Rs))+_uC(1))*(1/(_L)))/2;	
		break;
		}
		case _eval_uC(0): {
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = ((_iL(1)-(1/(_R))*_uC(1))*(1/(_C)))/2;	
			_eval_dep_iL(1) = (((_U/_Rs)-_iL(0))*(_Rs*_Rd/(_Rs+_Rd))-_uC(0))/_L;
			_eval_dep_iL(2) = (-(_uC(1)+_Rs*(1/(_Rs+_Rd))*_iL(1)*_Rd)*(1/(_L)))/2;	
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
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
	
			return;
		}
		case _eval_iL(0): {
			_der_iL(0) = (((_U/_Rs)-_iL(0))*(_Rs*_Rd/(_Rs+_Rd))-_uC(0))/_L;
	
			return;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(2,4,3,0,0,"buck_circuit");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(2*sizeof(int));
	int* discretes = (int*) malloc(4*sizeof(int));
	int* events = (int*) malloc(3*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	_C = 0.0001;
	_DC = 0.5;
	_L = 0.0001;
	_R = 10;
	_ROff = 100000;
	_ROn = 1e-05;
	_T = 0.0001;
	_U = 24;
	_nextT = _T;
	_lastT = 0;
	_init_uC(0) = 0;
	_Rs = 100000;
	_Rd = 100000;
	_init_iL(0) = 0;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 2);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	cleanVector(states, 0, 2);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	cleanVector(states, 0, 2);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_3;
	cleanVector(events, 0, 3);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	cleanVector(events, 0, 3);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	cleanVector(events, 0, 3);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	cleanVector(events, 0, 3);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rd;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rd;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rd;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rs;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rs;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = -1;
	modelData->event[_idx_event_3].relation = 0;
	simulator->time = QSS_Time(2,3,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("buck_circuit",2,4,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_uC(0)]++;
	SD_allocOutputMatrix(modelOutput, 2, 4);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "uC");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "iL");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_uC(0);
	cleanVector(states, 0, 2);
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

