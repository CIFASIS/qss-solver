
#include <common/solve_loop.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TestRL3.h"
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
	SVL_SolveLoop(x, d, a, &t, dx, 1, MOD_EvalTearingVars);
	for(i = 1; i<=1; i+=1) {
		_der_iL((-i)+2) = (((pow(_L,(-1))))*_uL((-i)+2));
	}
	for(i = 2; i<=100; i+=1) {
		_der_iL(i) = (((pow(_L,(-1))))*_uL(i));
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
			_out = _iL(1);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void MOD_EvalTearingVars(double *x, double *d, double *a, double *t, double *dx, const double *guess, double *ret)
{
	int tearing_idx;
	tearing_idx = 0;
	_tearing_var_guess_iR_N = guess[tearing_idx++];
	MOD_AssignLoop(x, d, a, t, dx);
		_tearing_var_res_iR_N = (((_uL(100)*(-1))+(_R0*_iR(100)))+((_R0*_iL(100))*(-1)));
	
	tearing_idx = 0;
	ret[tearing_idx++] = _tearing_var_res_iR_N;
}

void MOD_AssignLoop(double *x, double *d, double *a, double *t, double *dx)
{
	int _d1;
	int i;
		_iR(100) = _tearing_var_guess_iR_N;
	
	for(i = 99; i<=99; i+=1) {
		_iR((-i)+198) = (_iL((-i)+198)+_iR((-i)+199));
	
	}
	for(i = 1; i<=98; i+=1) {
		_iR((-i)+99) = (_iL((-i)+99)+_iR((-i)+100));
	
	}
		_uL(1) = (_U+((_iR(1)*_R)*(-1)));
	
	for(i = 1; i<=1; i+=1) {
		_uL((-i)+3) = (((_R*_iR((-i)+3))*(-1))+_uL((-i)+2));
	
	}
	for(i = 2; i<=99; i+=1) {
		_uL(i+1) = (((_iR(i+1)*_R)*(-1))+_uL(i));
	
	}
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(100,0,0,0,201,2,8,"TestRL3");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(201*sizeof(int));
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	_L = 1;
	_L1 = 1;
	_R = 1;
	_R0 = 1;
	_U = 1;
	_tearing_var_res_iR_N = 1;
	CLC_allocDataMatrix(modelData);
	simulator->output = SD_Output("TestRL3",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_iL(1)]++;
	SD_allocOutputMatrix(modelOutput, 100, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "iL[1]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_iL(1);
	cleanVector(states, 0, 100);
	modelOutput->SO[_idx_iL(1)][states[_idx_iL(1)]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(algebraics);
	free(states);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

