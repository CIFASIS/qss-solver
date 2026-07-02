#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "rltest_LI.h"
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
	int i;
	switch(idx) {
		case _eval_x1(10,0): {
			_a1(10,0) = (pow((_x1(9,0)-_x1(10,0)),3))/_R;
			_a1(10,1) = 0;
			_der_x1(10,0) = (_a1(10,0)-_x2(1,0))/_C;
			_der_x1(10,1) = (0)/2;
	
			return;
		}
		case _eval_x2(1,0): {
			_der_x2(1,0) = _x1(10,0)/_L;
			_der_x2(1,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_x1(idx)) {
		_get_x1_idxs(idx);
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
			_a1(1,1) = 0;
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_alg_eq_2(_d1-1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 9)) {
			_der_x1(i,0) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
			_der_x1(i,1) = (0)/2;
	
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
	int _d1;
	switch(idx) {
		case _eval_out_exp_2: {
			_out = _x2(1,0);
			return;
		}
	}
	if (_is_var_out_exp_1(idx)) {
		_get_out_exp_1_idxs(idx);
		_apply_usage_out_exp_1(_d1);
		if ((_d1 >= 1 && _d1 <= 10)) {
			_out = _x1(_d1,0);
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
		case _eval_x1(1,0): {
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
			_a1(1,1) = 0;
			_a1(2,0) = (pow((_x1(1,0)-_x1(2,0)),3))/_R;
			_a1(2,1) = 0;
			_eval_dep_x1(1,1) = (pow((_a1(1,0)-_a1(2,0)),3))/_C;
			_eval_dep_x1(1,2) = (0)/2;
			break;
		}
		case _eval_x1(10,0): {
			_a1(10,0) = (pow((_x1(9,0)-_x1(10,0)),3))/_R;
			_a1(10,1) = 0;
			_eval_dep_x1(10,1) = (_a1(10,0)-_x2(1,0))/_C;
			_eval_dep_x1(10,2) = (0)/2;
			_eval_dep_x2(1,1) = _x1(10,0)/_L;
			_eval_dep_x2(1,2) = (0)/2;
			break;
		}
		case _eval_x1(9,0): {
			_a1(10,0) = (pow((_x1(9,0)-_x1(10,0)),3))/_R;
			_a1(10,1) = 0;
			_eval_dep_x1(10,1) = (_a1(10,0)-_x2(1,0))/_C;
			_eval_dep_x1(10,2) = (0)/2;
			break;
		}
		case _eval_x2(1,0): {
			_a1(10,0) = (pow((_x1(9,0)-_x1(10,0)),3))/_R;
			_a1(10,1) = 0;
			_eval_dep_x1(10,1) = (_a1(10,0)-_x2(1,0))/_C;
			_eval_dep_x1(10,2) = (0)/2;
			break;
		}
	}
	if (_is_var_x1(idx)) {
		_get_x1_idxs(idx);
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
			_a1(1,1) = 0;
		_apply_usage_alg_eq_2(_d1-1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_alg_eq_2(_d1-2);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_eq_1(_d1-1);
		if ((i >= 1 && i <= 9)) {
			_eval_dep_x1(i,1) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
			_eval_dep_x1(i,2) = (0)/2;
		}
	}
	if (_is_var_x1(idx)) {
		_get_x1_idxs(idx);
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
			_a1(1,1) = 0;
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 9)) {
			_eval_dep_x1(i,1) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
			_eval_dep_x1(i,2) = (0)/2;
		}
	}
	if (_is_var_x1(idx)) {
		_get_x1_idxs(idx);
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
			_a1(1,1) = 0;
		_apply_usage_alg_eq_2(_d1+1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
		_a1(i+1,1) = 0;
	}
		_apply_usage_eq_1(_d1+1);
		if ((i >= 1 && i <= 9)) {
			_eval_dep_x1(i,1) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
			_eval_dep_x1(i,2) = (0)/2;
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
		case _eval_x1(10,0): {
			_a1(10,0) = (pow((_x1(9,0)-_x1(10,0)),3))/_R;
	
			_eval_dep_x1(10,1) = (_a1(10,0)-_x2(1,0))/_C;
	
	
			continue;
		}
		case _eval_x2(1,0): {
			_eval_dep_x2(1,1) = _x1(10,0)/_L;
	
	
			continue;
		}
	}
	if (_is_var_x1(idx)) {
		_get_x1_idxs(idx);
			_a1(1,0) = (pow((10-_x1(1,0)),3))/_R;
	
		_apply_usage_alg_eq_2(_d1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
	
	}
		_apply_usage_alg_eq_2(_d1-1);
		if ((i >= 1 && i <= 9)) {
			_a1(i+1,0) = (pow((_x1(i,0)-_x1(i+1,0)),3))/_R;
	
	}
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 9)) {
			_eval_dep_x1(i,1) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(11,0,0,0,10,3,2,"rltest_LI");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(10*sizeof(int));
	int* states = (int*) malloc(11*sizeof(int));
	int* outputs = (int*) malloc(11*sizeof(int));
	int _d1;
	_C = 1;
	_L = 1;
	_R = 1;
	modelData->nSD[_idx_x1(1,0)]++;
	modelData->nSD[_idx_x1(10,0)]++;
	modelData->nSD[_idx_x1(10,0)]++;
	modelData->nSD[_idx_x1(9,0)]++;
	modelData->nSD[_idx_x2(1,0)]++;
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_x1(_d1+1,0)]++;
	}
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_x1(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_x1(_d1-1,0)]++;
	}
	modelData->nDS[_idx_x1(1,0)]++;
	modelData->nDS[_idx_x1(10,0)]++;
	modelData->nDS[_idx_x2(1,0)]++;
	modelData->nDS[_idx_x1(10,0)]++;
	modelData->nDS[_idx_x1(10,0)]++;
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_x1(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_x1(_d1,0)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_x1(_d1,0)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 11);
	modelData->SD[_idx_x1(1,0)][states[_idx_x1(1,0)]++] = _idx_x1(1,0);
	modelData->SD[_idx_x1(10,0)][states[_idx_x1(10,0)]++] = _idx_x1(10,0);
	modelData->SD[_idx_x1(10,0)][states[_idx_x1(10,0)]++] = _idx_x2(1,0);
	modelData->SD[_idx_x1(9,0)][states[_idx_x1(9,0)]++] = _idx_x1(10,0);
	modelData->SD[_idx_x2(1,0)][states[_idx_x2(1,0)]++] = _idx_x1(10,0);
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->SD[_idx_x1(_d1+1,0)][states[_idx_x1(_d1+1,0)]++] = _idx_x1(_d1,0);
	}
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->SD[_idx_x1(_d1,0)][states[_idx_x1(_d1,0)]++] = _idx_x1(_d1,0);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_x1(_d1-1,0)][states[_idx_x1(_d1-1,0)]++] = _idx_x1(_d1,0);
	}
	cleanVector(states, 0, 11);
	modelData->DS[_idx_x1(1,0)][states[_idx_x1(1,0)]++] = _idx_x1(1,0);
	modelData->DS[_idx_x1(10,0)][states[_idx_x1(10,0)]++] = _idx_x1(10,0);
	modelData->DS[_idx_x2(1,0)][states[_idx_x2(1,0)]++] = _idx_x1(10,0);
	modelData->DS[_idx_x1(10,0)][states[_idx_x1(10,0)]++] = _idx_x1(9,0);
	modelData->DS[_idx_x1(10,0)][states[_idx_x1(10,0)]++] = _idx_x2(1,0);
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->DS[_idx_x1(_d1,0)][states[_idx_x1(_d1,0)]++] = _idx_x1(_d1+1,0);
	}
	for(_d1 = 1; _d1<=9; _d1+=1) {
		modelData->DS[_idx_x1(_d1,0)][states[_idx_x1(_d1,0)]++] = _idx_x1(_d1,0);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_x1(_d1,0)][states[_idx_x1(_d1,0)]++] = _idx_x1(_d1-1,0);
	}
	simulator->time = QSS_Time(11,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("rltest_LI",11,0,11,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nOS[_idx_out_exp_1(_d1)]++;
	}
	modelOutput->nSO[_idx_x2(1,0)]++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->nSO[_idx_x1(_d1,0)]++;
	}
	SD_allocOutputMatrix(modelOutput, 11, 0);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		sprintf(modelOutput->variable[_idx_out_exp_1(_d1)].name, "x1[%d]",_d1);
	}
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "x2[1]");
	cleanVector(outputs, 0, 11);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_x2(1,0);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->OS[_idx_out_exp_1(_d1)][outputs[_idx_out_exp_1(_d1)]++] = _idx_x1(_d1,0);
	}
	cleanVector(states, 0, 11);
	modelOutput->SO[_idx_x2(1,0)][states[_idx_x2(1,0)]++] = _idx_out_exp_2;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelOutput->SO[_idx_x1(_d1,0)][states[_idx_x1(_d1,0)]++] = _idx_out_exp_1(_d1);
	}
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

