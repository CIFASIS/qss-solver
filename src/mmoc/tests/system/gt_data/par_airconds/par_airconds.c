#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "par_airconds.h"
#include "pkg_math.h"
#include <common/utils.h>
#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

void MOD_settings(SD_simulationSettings settings)
{
	settings->debug = 0;
	settings->parallel = TRUE;
	settings->hybrid = TRUE;
	settings->method = 3;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4000)) {
			_der_th(i,0) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
			_der_th(i,1) = (0)/2;
	
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
		if ((i >= 1 && i <= 4000)) {
			_zc(0) = _th(i,0)-_tref(i)+_on(i)-0.5-(0);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4000)) {
			_zc(0) = _time-(_nextTref(i));
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4000)) {
			_zc(0) = _time-(_nextSample(i));
			_zc(1) = (0)/1;
	
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
		if ((i >= 1 && i <= 4000)) {
			_on(i) = 1;
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4000)) {
			if((_nextTref(i)==1000)) {
		_tref(i) = 20.5;
	}else {
		_tref(i) = 20;
	}
			_nextTref(i) = 2000;
			}
			return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 4000)) {
			_nextSample(i) = _nextSample(i)+1;
			_noise(i) = 2*abs(sin(i*_time))-1;
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
		if ((i >= 1 && i <= 4000)) {
			_on(i) = 0;
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
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
	for(row = 1; row <= 4000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4000)) {
			x_ind = _idx_th(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 4000; row++) {
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
		if ((i >= 1 && i <= 4000)) {
			if (map[_idx_th(i,0)] > NOT_ASSIGNED) {
				_eval_dep_th(i,1) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
				_eval_dep_th(i,2) = (0)/2;
			}
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
		if ((i >= 1 && i <= 4000)) {
			_eval_dep_th(i,1) = (_THA/_RES(i)-_POT(i)*_on(i)-_th(i,0)/_RES(i)+_noise(i)/_RES(i))/_CAP(i);
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(4000,20000,12000,0,0,1,0,"par_airconds");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(4000*sizeof(int));
	int* discretes = (int*) malloc(20000*sizeof(int));
	int* events = (int*) malloc(12000*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
	int i;
	_THA = 32;
	_pmax = 0;
	for(i = 1; i<=4000; i+=1) {
		_init_th(i,0) = __math__rand(4)+18;
		_CAP(i) = __math__rand(100)+550;
		_RES(i) = __math__rand(0.4)+1.8;
		_POT(i) = __math__rand(2)+13;
		_pmax = _pmax+_POT(i);
		_nextSample(i) = 1;
		_noise(i) = __math__rand(2)-1;
		_tref(i) = 20;
		_nextTref(i) = 1000;
	}
	for(i = 1; i<=4000; i+=1) {
		if(_init_th(i,0)-_tref(i)-0.5>0) {
			_on(i) = 1;
		}
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nSD[_idx_th(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDS[_idx_th(_d1,0)]++;
	}
	for(row = 1; row <= 4000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4000)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nSZ[_idx_th(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nZS[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHD[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHD[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_nextSample(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_nextTref(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_noise(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_on(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_tref(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHH[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHH[_idx_event_3(_d1)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 4000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->SD[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 4000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DS[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 4000);
	for(row = 1; row <= 4000; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_th_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 4000)) {
			x_ind = _idx_th(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 4000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->SZ[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_event_1(_d1);
	}
	cleanVector(events, 0, 12000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->ZS[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 12000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_event_3(_d1);
	}
	cleanVector(events, 0, 12000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HD[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_th(_d1,0);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HD[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 12000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_nextSample(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_nextTref(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_noise(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_on(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_tref(_d1);
	}
	cleanVector(discretes, 0, 20000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_nextSample(_d1)][discretes[_idx_nextSample(_d1)]++] = _idx_event_3(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_nextTref(_d1)][discretes[_idx_nextTref(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_noise(_d1)][discretes[_idx_noise(_d1)]++] = _idx_event_3(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_on(_d1)][discretes[_idx_on(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_tref(_d1)][discretes[_idx_tref(_d1)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 12000);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HH[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HH[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_event_3(_d1);
	}
	for(i = 1; i<=4000; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 0;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	for(i = 1; i<=4000; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	for(i = 1; i<=4000; i+=1) {
		modelData->event[_idx_event_3(i)].direction = 1;
		modelData->event[_idx_event_3(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(4000,12000,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.6;
		simulator->output = SD_Output("par_airconds",1,20000,4000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_th(1,0)]++;
	SD_allocOutputMatrix(modelOutput, 4000, 20000);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "th[1]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_th(1,0);
	cleanVector(states, 0, 4000);
	modelOutput->SO[_idx_th(1,0)][states[_idx_th(1,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

