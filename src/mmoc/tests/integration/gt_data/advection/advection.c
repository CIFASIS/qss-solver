#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "advection.h"
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
			_der_u(1,0) = (-_u(1,0)+1)*20000-_mu*_u(1,0)*(_u(1,0)-_alpha)*(_u(1,0)-1);
			_der_u(1,1) = (-_u(1,1)*_mu*(_u(1,0)-_alpha)*(-1+_u(1,0))-20000*_u(1,1)-_u(1,1)*_mu*_u(1,0)*(-1+_u(1,0))-_u(1,1)*_mu*(_u(1,0)-_alpha)*_u(1,0))/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 20000)) {
			_der_u(j,0) = (-_u(j,0)+_u(j-1,0))*20000-_mu*_u(j,0)*(_u(j,0)-_alpha)*(_u(j,0)-1);
			_der_u(j,1) = (-_u(j,0)*_u(j,1)*(_u(j,0)-_alpha)*_mu-_u(j,0)*(-1+_u(j,0))*_u(j,1)*_mu-(-1+_u(j,0))*_u(j,1)*(_u(j,0)-_alpha)*_mu-20000*_u(j,1)+20000*_u(j-1,1))/2;
	
		}
		return;
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
			_out = _u(20000,0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, double *jac)
{
	double __chain_rule = 0;
	double __jac_exp = 0;
	int _d1;
	int idx;
	int j;
	int jit = 0;
	for (idx = 0; idx <20000; idx++) {
	switch(idx) {
		case _eval_u(1,0): {
			__jac_exp = 0;
				__jac_exp += -20000+(_alpha-_u(1,0))*_mu*_u(1,0)+(_alpha-_u(1,0))*(-1+_u(1,0))*_mu-(-1+_u(1,0))*_mu*_u(1,0);
				_jac(jit) = __jac_exp;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
			__jac_exp = 0;
				__jac_exp += 20000;
			_apply_usage_eq_2(_d1+1);
		if ((j >= 2 && j <= 20000)) {
			_jac(jit) = __jac_exp;
		}
			__jac_exp = 0;
			_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 20000)) {
			__jac_exp += -20000-_mu*(_u(j,0)-_alpha)*_u(j,0)-_mu*_u(j,0)*(-1+_u(j,0))-_mu*(_u(j,0)-_alpha)*(-1+_u(j,0));
		}
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 20000)) {
			_jac(jit) = __jac_exp;
		}
		}
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int j;
	switch(idx) {
		case _eval_u(1,0): {
			_eval_dep_u(1,1) = (-_u(1,0)+1)*20000-_mu*_u(1,0)*(_u(1,0)-_alpha)*(_u(1,0)-1);
			_eval_dep_u(1,2) = (-(-1+_u(1,0))*_u(1,1)*_mu*_u(1,0)-20000*_u(1,1)+(_alpha-_u(1,0))*(-1+_u(1,0))*_u(1,1)*_mu+(_alpha-_u(1,0))*_u(1,1)*_mu*_u(1,0))/2;	
		break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1+1);
		if ((j >= 2 && j <= 20000)) {
			_eval_dep_u(j,1) = (-_u(j,0)+_u(j-1,0))*20000-_mu*_u(j,0)*(_u(j,0)-_alpha)*(_u(j,0)-1);
			_eval_dep_u(j,2) = (-_mu*_u(j,1)*(_u(j,0)-_alpha)*_u(j,0)+20000*_u(j-1,1)-20000*_u(j,1)-_mu*_u(j,1)*(-1+_u(j,0))*_u(j,0)-_mu*_u(j,1)*(-1+_u(j,0))*(_u(j,0)-_alpha))/2;	}
	
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 20000)) {
			_eval_dep_u(j,1) = (-_u(j,0)+_u(j-1,0))*20000-_mu*_u(j,0)*(_u(j,0)-_alpha)*(_u(j,0)-1);
			_eval_dep_u(j,2) = (-20000*_u(j,1)-_u(j,1)*_u(j,0)*_mu*(_u(j,0)-_alpha)-_u(j,1)*_u(j,0)*(-1+_u(j,0))*_mu-_u(j,1)*(-1+_u(j,0))*_mu*(_u(j,0)-_alpha)+20000*_u(j-1,1))/2;	}
	
		}
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int _d1;
	int j;
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	switch(idx) {
		case _eval_u(1,0): {
			_der_u(1,0) = (-_u(1,0)+1)*20000-_mu*_u(1,0)*(_u(1,0)-_alpha)*(_u(1,0)-1);
	
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((j >= 2 && j <= 20000)) {
			_der_u(j,0) = (-_u(j,0)+_u(j-1,0))*20000-_mu*_u(j,0)*(_u(j,0)-_alpha)*(_u(j,0)-1);
	
	
		}
		return;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(20000,0,0,0,0,"advection");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(20000*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int _d1;
	int i;
	_alpha = 0.5;
	_mu = 1000;
	for(i = 1; i<=6666; i+=1) {
		_init_u(i,0) = 1;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nSD[_idx_u(1,0)]++;
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->nSD[_idx_u(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->nDS[_idx_u(1,0)]++;
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->nDS[_idx_u(_d1,0)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 20000);
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->SD[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,0)][states[_idx_u(_d1-1,0)]++] = _idx_u(_d1,0);
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->SD[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	cleanVector(states, 0, 20000);
	for(_d1 = 1; _d1<=1; _d1+=1) {
		modelData->DS[_idx_u(1,0)][states[_idx_u(1,0)]++] = _idx_u(1,0);
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1-1,0);
	}
	for(_d1 = 2; _d1<=20000; _d1+=1) {
		modelData->DS[_idx_u(_d1,0)][states[_idx_u(_d1,0)]++] = _idx_u(_d1,0);
	}
	simulator->time = QSS_Time(20000,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("advection",1,0,20000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(20000,0)]++;
	SD_allocOutputMatrix(modelOutput, 20000, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[20000]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(20000,0);
	cleanVector(states, 0, 20000);
	modelOutput->SO[_idx_u(20000,0)][states[_idx_u(20000,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

