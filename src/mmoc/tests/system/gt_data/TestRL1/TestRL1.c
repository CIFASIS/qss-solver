
#include <common/solve_loop.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "TestRL1.h"
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
	SVL_SolveLoop(x, d, a, &t, dx, 100, MOD_EvalTearingVars);
	_der_iL(1) = (((_Ua(1)+(_U0*(-1)))*((pow(_L,(-1)))))*(-1));
	for(i = 2; i<=2; i+=1) {
		_der_iL(i) = (((pow(_L,(-1))))*(_Uc(i-1)+(_Ua(i)*(-1))));
	}
	for(i = 3; i<=99; i+=1) {
		_der_iL(i) = ((((pow(_L,(-1))))*(_Ua(i)+(_Uc(i-1)*(-1))))*(-1));
	}
	for(i = 100; i<=100; i+=1) {
		_der_iL((-i)+200) = ((((pow(_L,(-1))))*(_Ua((-i)+200)+(_Uc((-i)+199)*(-1))))*(-1));
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
	int _d1;
	int i;
	int tearing_idx;
	tearing_idx = 0;
	for(_d1 = 1; _d1<=99; _d1+=1) {
		_tearing_var_guess_Ua(_d1) = guess[tearing_idx++];
	}
	_tearing_var_guess_Uc_N = guess[tearing_idx++];
	MOD_AssignLoop(x, d, a, t, dx);
	for(i = 1; i<=1; i+=1) {
		_tearing_var_res_Ua(i) = (((((_Rb*_Ra)*_iL(i))+((_tearing_var_guess_Ua(i)*_Ra)*(-1)))+((_Rb*_tearing_var_guess_Ua(i))*(-1)))*((pow(_Ra,(-1)))));
	
	}for(i = 2; i<=99; i+=1) {
		_tearing_var_res_Ua(i) = ((((pow(_Ra,(-1))))*(((_Rb*_tearing_var_guess_Ua(i))+(_tearing_var_guess_Ua(i)*_Ra))+(((_Rb*_iL(i))*_Ra)*(-1))))*(-1));
	
	}	_tearing_var_res_Uc_N = ((((pow(_Rc,(-1))))*(((_Uc(100)*_Ra)+(((_iL(100)*_Rc)*_Ra)*(-1)))+(_Ua(100)*_Rc)))*((pow(_Ra,(-1)))));
	
	tearing_idx = 0;
	for(_d1 = 1; _d1<=99; _d1+=1) {
		ret[tearing_idx++] = _tearing_var_res_Ua(_d1);
	}
	ret[tearing_idx++] = _tearing_var_res_Uc_N;
}

void MOD_AssignLoop(double *x, double *d, double *a, double *t, double *dx)
{
	int _d1;
	int i;
	for(i = 1; i<=1; i+=1) {
		_Ua(i) = _tearing_var_guess_Ua(i);
	
	}
	for(i = 1; i<=1; i+=1) {
		_Uc(i) = (((((_Ra*_iL(i))+(_tearing_var_guess_Ua(i)*(-1)))+((_iL(i+1)*_Ra)*(-1)))*_Rc)*((pow(_Ra,(-1)))));
	
	}
	for(i = 2; i<=99; i+=1) {
		_Ua(i) = _tearing_var_guess_Ua(i);
	
	}
	for(i = 2; i<=99; i+=1) {
		_Uc(i) = ((_Rc*(((_iL(i)*_Ra)+(_tearing_var_guess_Ua(i)*(-1)))+((_iL(i+1)*_Ra)*(-1))))*((pow(_Ra,(-1)))));
	
	}
		_Uc(100) = _tearing_var_guess_Uc_N;
	
		_Ua(100) = ((_Rb*_Uc(100))*((pow(_Rc,(-1)))));
	
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(100,0,0,0,300,4,12,"TestRL1");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(300*sizeof(int));
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int _d1;
	_L = 1;
	_Ra = 1;
	_Rb = 1;
	_Rc = 1;
	_U0 = 1;
	for(_d1 = 1; _d1<=99; _d1+=1) {
			_tearing_var_res_Ua(_d1) = 1;
	}
	_tearing_var_res_Uc_N = 1;
	CLC_allocDataMatrix(modelData);
	simulator->output = SD_Output("TestRL1",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
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

