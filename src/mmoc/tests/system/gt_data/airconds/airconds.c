#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "airconds.h"
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
	settings->method = 6;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 20000)) {
			_der_th(i,0) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
			_der_th(i,1) = (0)/2;
			_der_th(i,2) = (0)/6;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 20000)) {
			_zc(0) = _th(i,0)-_tref(i)+_on(i)-5.000000e-01-(0);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 20000)) {
			_zc(0) = _time-(1000);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 20000)) {
			_zc(0) = _time-(2000);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_4(idx)) {
		_get_event_4_idxs(idx);
		_apply_usage_event_4(_d1);
		if ((i >= 1 && i <= 20000)) {
			_zc(0) = _time-(_nextSample(i));
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 20000)) {
			_on(i) = 1;
			_ptotal = _ptotal+_POT(i);
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 20000)) {
			_tref(i) = 2.050000e+01;
			}
			return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 20000)) {
			_tref(i) = 20;
			}
			return;
	}
	if (_is_var_event_4(idx)) {
		_get_event_4_idxs(idx);
		_apply_usage_event_4(_d1);
		if ((i >= 1 && i <= 20000)) {
			_nextSample(i) = _nextSample(i)+1;
			_noise(i) = 2-1;
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 20000)) {
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
		case _eval_out_exp_2: {
			_out = _th(1,0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
	int row, row_t, eq_var, c_row, c_row_g;
	int col, col_g, col_t;
	int x_ind;
	double aux;
	int _d1;
	int _rg_d1;
	int i;
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 20000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 20000)) {
			x_ind = _idx_th(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 20000; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int i;
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 20000)) {
			_eval_dep_th(i,1) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
			_eval_dep_th(i,2) = (0)/2;
			_eval_dep_th(i,3) = (0)/6;
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
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 20000)) {
			_eval_dep_th(i,1) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(20000,80001,80000,0,0,1,0,"airconds");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(20000*sizeof(int));
	int* discretes = (int*) malloc(80001*sizeof(int));
	int* events = (int*) malloc(80000*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_THA = 32;
	_pmax = 0;
	for(i = 1; i<=20000; i+=1) {
		_init_th(i,0) = 4+18;
		_CAP(i) = 100+550;
		_RES(i) = 4.000000e-01+1.800000e+00;
		_POT(i) = 2+13;
		_pmax = _pmax+_POT(i);
		_nextSample(i) = 1;
		_noise(i) = 2-1;
		_tref(i) = 20;
	}
	for(i = 1; i<=20000; i+=1) {
		if(_init_th(i,0)-_tref(i)-5.000000e-01>0) {
			_on(i) = 1;
			_ptotal = _ptotal+_POT(i);
		}	else if(_init_th(i,0)-_tref(i)<0) {
			_on(i) = 0;
		}
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nSD[_idx_th(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nDS[_idx_th(_d1,0)]++;
	}
	for(row = 1; row <= 20000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 20000)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nSZ[_idx_th(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nZS[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHZ[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHZ[_idx_event_4(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->nHD[_idx_event_4(_d1)]++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 20000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->SD[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 20000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->DS[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 20000);
	for(row = 1; row <= 20000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 20000)) {
			x_ind = _idx_th(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 20000);
	cleanVector(states, 0, 20000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->SZ[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_event_1(_d1);
	}
	cleanVector(events, 0, 80000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->ZS[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 80000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HZ[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HZ[_idx_event_4(_d1)][events[_idx_event_4(_d1)]++] = _idx_event_4(_d1);
	}
	cleanVector(events, 0, 80000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_th(_d1,0);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->HD[_idx_event_4(_d1)][events[_idx_event_4(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 80000);
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_ptotal;
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].LHSDsc[events[_idx_event_4(_d1)]++] = _idx_nextSample(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_4(_d1)].LHSDsc[events[_idx_event_4(_d1)]++] = _idx_noise(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_on(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_tref(_d1);
	}
	for(_d1 = 1; _d1<=20000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_tref(_d1);
	}
	for(i = 1; i<=20000; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 0;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	for(i = 1; i<=20000; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	for(i = 1; i<=20000; i+=1) {
		modelData->event[_idx_event_3(i)].direction = 1;
		modelData->event[_idx_event_3(i)].relation = 2;
	}
	for(i = 1; i<=20000; i+=1) {
		modelData->event[_idx_event_4(i)].direction = 1;
		modelData->event[_idx_event_4(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(20000,80000,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("airconds",2,80001,20000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOD[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_th(1,0)]++;
	modelOutput->nDO[_idx_ptotal]++;
	SD_allocOutputMatrix(modelOutput, 20000, 80001);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "ptotal");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "th[1]");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_th(1,0);
	cleanVector(outputs, 0, 2);
	modelOutput->OD[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_ptotal;
	cleanVector(states, 0, 20000);
	modelOutput->SO[_idx_th(1,0)][states[_idx_th(1,0)]++] = _idx_out_exp_2;
	cleanVector(discretes, 0, 80001);
	modelOutput->DO[_idx_ptotal][discretes[_idx_ptotal]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

