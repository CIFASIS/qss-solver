#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "advectionFlux.h"
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
	settings->method = 4;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_F(1,0) = _v*_inVal;
			_F(1,1) = 0;
			_F(2,0) = _v*_u(1,0);
			_F(2,1) = 0;
			_der_u(1,0) = -(_F(2,0)-_F(1,0))*100/_L;
			_der_u(1,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_alg_eq_2(_d1+1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
		_F(j,1) = 0;
	}
		_apply_usage_alg_eq_2(_d1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
		_F(j,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 100)) {
			_der_u(j,0) = -(_F(j+1,0)-_F(j,0))*100/_L;
			_der_u(j,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _u(100,0);
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
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_F(1,0) = _v*_inVal;
			_F(1,1) = 0;
			_F(2,0) = _v*_u(1,0);
			_F(2,1) = 0;
			_eval_dep_u(1,1) = -(_F(2,0)-_F(1,0))*100/_L;
			_eval_dep_u(1,2) = (0)/2;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_alg_eq_2(_d1+1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
		_F(j,1) = 0;
	}
		_apply_usage_alg_eq_2(_d1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
		_F(j,1) = 0;
	}
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 100)) {
			_eval_dep_u(j,1) = -(_F(j+1,0)-_F(j,0))*100/_L;
			_eval_dep_u(j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_alg_eq_2(_d1+2);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
		_F(j,1) = 0;
	}
		_apply_usage_eq_2(_d1+1);
		if ((j >= 2 && j <= 100)) {
			_eval_dep_u(j,1) = -(_F(j+1,0)-_F(j,0))*100/_L;
			_eval_dep_u(j,2) = (0)/2;
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
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_F(1,0) = _v*_inVal;
	
			_F(2,0) = _v*_u(1,0);
	
			_eval_dep_u(1,1) = -(_F(2,0)-_F(1,0))*100/_L;
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_alg_eq_2(_d1+1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
	
	}
		_apply_usage_alg_eq_2(_d1);
		if ((j >= 2 && j <= 101)) {
			_F(j,0) = _v*_u(j-1,0);
	
	}
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 100)) {
			_eval_dep_u(j,1) = -(_F(j+1,0)-_F(j,0))*100/_L;
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(100,0,0,0,101,2,2,"advectionFlux");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(101*sizeof(int));
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int _d1;
	int i;
	_L = 1.000000e+00;
	_beta = 1.000000e+02;
	_diff = 0.000000e+00;
	_gam = 0.000000e+00;
	_inVal = 0;
	_outVal = 1;
	_v = 1.000000e+00;
	_x0 = 5.000000e-01;
	for(i = 1; i<=50; i+=1) {
		_x(i) = 1.000000e+00*i/(double)100;
		_init_u(i,0) = 1;
	}
	modelData->nSD[_idx_u(1,0)]++;
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,0)]++;
	}
	modelData->nDS[_idx_u(1,0)]++;
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 100);
	modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,0)][states[_idx_u(_d1-1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 100);
	modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=100; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1-1,0);
	}
	simulator->time = QSS_Time(100,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("advectionFlux",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(100,0)]++;
	SD_allocOutputMatrix(modelOutput, 100, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[100]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(100,0);
	cleanVector(states, 0, 100);
	modelOutput->SO[_idx_u(100,0)][states[_idx_u(100,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

