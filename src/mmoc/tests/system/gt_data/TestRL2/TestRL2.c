#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TestRL2.h"
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
	settings->method = 9;
}

void MOD_definition(double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
		_iR(100) = ((_I*(-1))+_iL(100));
	
	for(i = 99; i<=99; i+=1) {
		_iR((-i)+198) = (_iL((-i)+198)+_iR((-i)+199));
	
	}
	for(i = 1; i<=98; i+=1) {
		_iR((-i)+99) = (_iL((-i)+99)+_iR((-i)+100));
	
	}
		_uL(1) = (((_iR(1)*_R)*(-1))+((_iR(1)*_R0)*(-1)));
	
	for(i = 1; i<=99; i+=1) {
		_uL(i+1) = (((_R*_iR(i+1))*(-1))+_uL(i));
	
	}
	for(i = 1; i<=1; i+=1) {
		_der_iL((-i)+2) = (((pow(_L,(-1))))*_uL((-i)+2));
	}
	for(i = 2; i<=100; i+=1) {
		_der_iL((-i)+102) = (((pow(_L,(-1))))*_uL((-i)+102));
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
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _iL(100);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(100,0,0,0,200,2,5,"TestRL2");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(200*sizeof(int));
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	_I = 1;
	_L = 1;
	_L1 = 1;
	_R = 1;
	_R0 = 1;
	CLC_allocDataMatrix(modelData);
	simulator->output = SD_Output("TestRL2",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_iL(100)]++;
	SD_allocOutputMatrix(modelOutput, 100, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "iL[100]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_iL(100);
	cleanVector(states, 0, 100);
	modelOutput->SO[_idx_iL(100)][states[_idx_iL(100)]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(algebraics);
	free(states);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

