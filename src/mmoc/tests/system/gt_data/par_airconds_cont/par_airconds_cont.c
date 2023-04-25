#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "par_airconds_cont.h"
#include "par_airconds_cont_functions.h"
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
	settings->method = 6;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_ierr(0): {
			_der_ierr(0) = _pref-_ptotals/_pmax;
			_der_ierr(1) = (0)/2;
			_der_ierr(2) = (0)/6;
	
			return;
		}
		case _eval_ptotal(0): {
			_der_ptotal(0) = 0;
			_der_ptotal(1) = (0)/2;
			_der_ptotal(2) = (0)/6;
	
			return;
		}
	}
	if (_is_var_th(idx)) {
		_get_th_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 4000)) {
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
	switch(idx) {
		case _eval_event_3: {
			_zc(0) = _time-(1000);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
		case _eval_event_4: {
			_zc(0) = _time-(2000);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
		case _eval_event_5: {
			_zc(0) = _time-(_nextSample);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _time-(_partSample(i));
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_zc(0) = _update(i)-(5.000000e-01);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_6(idx)) {
		_get_event_6_idxs(idx);
		_apply_usage_event_6(_d1);
		if ((i >= 1 && i <= 4000)) {
			_zc(0) = _th(i,0)-_tref-_dtref+_on(i)-5.000000e-01-(0);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
		}
		return;
	}
	if (_is_var_event_7(idx)) {
		_get_event_7_idxs(idx);
		_apply_usage_event_7(_d1);
		if ((i >= 1 && i <= 4000)) {
			_zc(0) = _time-(_sampleNoise(i));
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
	switch(idx) {
		case _eval_event_3: {
			_pref = 4.000000e-01;
					return;
		}
		case _eval_event_4: {
			_pref = 5.000000e-01;
					return;
		}
		case _eval_event_5: {
			_nextSample = _nextSample+1;
			_ptotals = _q_ptotal(0);
			_dtref = _Kp*(_ptotals/_pmax-_pref)-_Ki*_q_ierr(0);
					return;
		}
	}
	if (_is_var_event_1(idx)) {
		_get_event_1_idxs(idx);
		_apply_usage_event_1(_d1);
		if ((i >= 1 && i <= 4)) {
			_partSample(i) = _partSample(i)+1;
			_update(i) = 1;
			_sendPartTotal(i) = _partTotal(i);
			_init_ptotal(0) = _init_ptotal(0)+_sendPartTotal(i);
			_partTotal(i) = 0;
			}
			return;
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 4)) {
			_init_ptotal(0) = _init_ptotal(0)+_sendPartTotal(i);
			_update(i) = 0;
			}
			return;
	}
	if (_is_var_event_6(idx)) {
		_get_event_6_idxs(idx);
		_apply_usage_event_6(_d1);
		if ((i >= 1 && i <= 4000)) {
			_on(i) = 1;
			_partTotal(_sections(i)) = _partTotal(_sections(i))+_POT(i);
			}
			return;
	}
	if (_is_var_event_7(idx)) {
		_get_event_7_idxs(idx);
		_apply_usage_event_7(_d1);
		if ((i >= 1 && i <= 4000)) {
			_sampleNoise(i) = _sampleNoise(i)+1;
			_inputs(i) = _inputs(i)+1;
			_noise(i) = 2*pow(sin(i*_inputs(i)),2);
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	int _d1;
	int i;
	if (_is_var_event_6(idx)) {
		_get_event_6_idxs(idx);
		_apply_usage_event_6(_d1);
		if ((i >= 1 && i <= 4000)) {
			_on(i) = 0;
			_partTotal(_sections(i)) = _partTotal(_sections(i))-_POT(i);
			}
			return;
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _ptotals;
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
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 4000; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 1
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
	    row_t = dvdx->df_dx[1]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
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
				_eval_dep_th(i,3) = (0)/6;
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
	switch(idx) {
		case _eval_ierr(0): {
			_eval_dep_ierr(1) = _pref-_ptotals/_pmax;
	
	
			continue;
		}
		case _eval_ptotal(0): {
			_eval_dep_ptotal(1) = 0;
	
	
			continue;
		}
	}
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
	simulator->data = QSS_Data(4002,16020,8011,0,0,3,0,"par_airconds_cont");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(4002*sizeof(int));
	int* discretes = (int*) malloc(16020*sizeof(int));
	int* events = (int*) malloc(8011*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _d2;
	int _rg_d1;
	int i;
	_Ki = 1;
	_Kp = 1;
	_THA = 32;
	_nextSample = 1;
	for(_d1 = 1; _d1<=4; _d1+=1) {
			_partSample(_d1) = 1;
	}
	_pmax = 0;
	_pref = 5.000000e-01;
	_tref = 20;
	for(i = 1; i<=4000; i+=1) {
		_init_th(i,0) = __math__random(4)+18;
		_CAP(i) = __math__random(100)+550;
		_RES(i) = __math__random(4.000000e-01)+1.800000e+00;
		_POT(i) = __math__random(2.000000e-01)+13;
		_pmax = _pmax+_POT(i);
		_noise(i) = 2*pow(sin(i),2);
		_sampleNoise(i) = __math__random(2);
		_sections(i) = __getSection(i);
	}
	for(i = 1; i<=4; i+=1) {
		_sections_init(i) = (i-1)*4000/(double)4;
		_sections_end(i) = i*4000/(double)4;
	}
	for(i = 1; i<=4000; i+=1) {
		if(_init_th(i,0)-_tref-5.000000e-01>0) {
			_on(i) = 1;
			_init_ptotal(0) = _init_ptotal(0)+_POT(i);
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
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nSZ[_idx_th(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nZS[_idx_event_6(_d1)]++;
	}
	modelData->nHZ[_idx_event_5]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_5]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_6(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHZ[_idx_event_7(_d1)]++;
	}
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_4]++;
	modelData->nHD[_idx_event_5]++;
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHD[_idx_event_6(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHD[_idx_event_7(_d1)]++;
	}
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_4].nLHSDsc++;
	modelData->event[_idx_event_5].nLHSDsc++;
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_6(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_6(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nLHSSt++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSSt++;
	}
	modelData->event[_idx_event_5].nRHSSt++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].nRHSSt++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nRHSSt++;
	}
	modelData->event[_idx_event_5].nRHSSt++;
	modelData->nDH[_idx_dtref]++;
	modelData->nDH[_idx_nextSample]++;
	modelData->nDH[_idx_pref]++;
	modelData->nDH[_idx_pref]++;
	modelData->nDH[_idx_ptotals]++;
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_inputs(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_noise(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_on(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDH[_idx_partSample(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDH[_idx_partTotal(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_sampleNoise(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDH[_idx_sendPartTotal(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDH[_idx_update(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nDH[_idx_update(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nDH[_idx_event_6(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSH[_idx_ptotal(0)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nSH[_idx_ptotal(0)]++;
	}
	modelData->nHH[_idx_event_3]++;
	modelData->nHH[_idx_event_4]++;
	modelData->nHH[_idx_event_5]++;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHH[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->nHH[_idx_event_1(_d1)]++;
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHH[_idx_event_7(_d1)]++;
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		for(_d2 = _sections_init(_d1); _d2<=_sections_end(_d1); _d2+=1) {
			modelData->nHH[_idx_event_1(_d1)]++;
		}
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->nHH[_idx_event_6(_d1)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 4002);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->SD[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 4002);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DS[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_th(_d1,0);
	}
	cleanVector(states, 0, 4002);
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
	cleanVector(states, 0, 4002);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(states, 0, 4002);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(states, 0, 4002);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->SZ[_idx_th(_d1,0)][states[_idx_th(_d1,0)]++] = _idx_event_6(_d1);
	}
	cleanVector(events, 0, 8011);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->ZS[_idx_event_6(_d1)][events[_idx_event_6(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 8011);
	modelData->HZ[_idx_event_5][events[_idx_event_5]++] = _idx_event_5;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_5][events[_idx_event_5]++] = _idx_event_6(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_6(_d1)][events[_idx_event_6(_d1)]++] = _idx_event_6(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HZ[_idx_event_7(_d1)][events[_idx_event_7(_d1)]++] = _idx_event_7(_d1);
	}
	cleanVector(events, 0, 8011);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_ierr(0);
	modelData->HD[_idx_event_4][events[_idx_event_4]++] = _idx_ierr(0);
	modelData->HD[_idx_event_5][events[_idx_event_5]++] = _idx_ierr(0);
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HD[_idx_event_6(_d1)][events[_idx_event_6(_d1)]++] = _idx_th(_d1,0);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HD[_idx_event_7(_d1)][events[_idx_event_7(_d1)]++] = _idx_th(_d1,0);
	}
	cleanVector(events, 0, 8011);
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_dtref;
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_nextSample;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_pref;
	modelData->event[_idx_event_4].LHSDsc[events[_idx_event_4]++] = _idx_pref;
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_ptotals;
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].LHSDsc[events[_idx_event_7(_d1)]++] = _idx_inputs(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].LHSDsc[events[_idx_event_7(_d1)]++] = _idx_noise(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_6(_d1)].LHSDsc[events[_idx_event_6(_d1)]++] = _idx_on(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_partSample(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_partTotal(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_7(_d1)].LHSDsc[events[_idx_event_7(_d1)]++] = _idx_sampleNoise(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_sendPartTotal(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSDsc[events[_idx_event_1(_d1)]++] = _idx_update(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_update(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->event[_idx_event_6(_d1)].LHSDsc[events[_idx_event_6(_d1)]++] = _idx_partTotal(_sections(_d1));
	}
	cleanVector(events, 0, 8011);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].LHSSt[events[_idx_event_1(_d1)]++] = _idx_ptotal(0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSSt[events[_idx_event_2(_d1)]++] = _idx_ptotal(0);
	}
	cleanVector(events, 0, 8011);
	modelData->event[_idx_event_5].RHSSt[events[_idx_event_5]++] = _idx_ierr(0);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_1(_d1)].RHSSt[events[_idx_event_1(_d1)]++] = _idx_ptotal(0);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].RHSSt[events[_idx_event_2(_d1)]++] = _idx_ptotal(0);
	}
	modelData->event[_idx_event_5].RHSSt[events[_idx_event_5]++] = _idx_ptotal(0);
	cleanVector(discretes, 0, 16020);
	modelData->DH[_idx_dtref][discretes[_idx_dtref]++] = _idx_event_5;
	modelData->DH[_idx_nextSample][discretes[_idx_nextSample]++] = _idx_event_5;
	modelData->DH[_idx_pref][discretes[_idx_pref]++] = _idx_event_3;
	modelData->DH[_idx_pref][discretes[_idx_pref]++] = _idx_event_4;
	modelData->DH[_idx_ptotals][discretes[_idx_ptotals]++] = _idx_event_5;
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_inputs(_d1)][discretes[_idx_inputs(_d1)]++] = _idx_event_7(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_noise(_d1)][discretes[_idx_noise(_d1)]++] = _idx_event_7(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_on(_d1)][discretes[_idx_on(_d1)]++] = _idx_event_6(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DH[_idx_partSample(_d1)][discretes[_idx_partSample(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DH[_idx_partTotal(_d1)][discretes[_idx_partTotal(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_sampleNoise(_d1)][discretes[_idx_sampleNoise(_d1)]++] = _idx_event_7(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DH[_idx_sendPartTotal(_d1)][discretes[_idx_sendPartTotal(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DH[_idx_update(_d1)][discretes[_idx_update(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->DH[_idx_update(_d1)][discretes[_idx_update(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->DH[_idx_event_6(_d1)][discretes[_idx_event_6(_d1)]++] = _idx_partTotal(_sections(_d1));
	}
	cleanVector(states, 0, 4002);
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SH[_idx_ptotal(0)][states[_idx_ptotal(0)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->SH[_idx_ptotal(0)][states[_idx_ptotal(0)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 8011);
	modelData->HH[_idx_event_3][events[_idx_event_3]++] = _idx_event_5;
	modelData->HH[_idx_event_4][events[_idx_event_4]++] = _idx_event_5;
	modelData->HH[_idx_event_5][events[_idx_event_5]++] = _idx_event_5;
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HH[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_1(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		modelData->HH[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_2(_d1);
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HH[_idx_event_7(_d1)][events[_idx_event_7(_d1)]++] = _idx_event_7(_d1);
	}
	for(_d1 = 1; _d1<=4; _d1+=1) {
		for(_d2 = _sections_init(_d1); _d2<=_sections_end(_d1); _d2+=1) {
			modelData->HH[_idx_event_1(_d1)][events[_idx_event_1(_d1)]++] = _idx_event_6(_d2);
		}
	}
	for(_d1 = 1; _d1<=4000; _d1+=1) {
		modelData->HH[_idx_event_6(_d1)][events[_idx_event_6(_d1)]++] = _idx_event_1(_sections(_d1));
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_1(i)].direction = 1;
		modelData->event[_idx_event_1(i)].relation = 2;
	}
	for(i = 1; i<=4; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	modelData->event[_idx_event_3].direction = 1;
	modelData->event[_idx_event_3].relation = 2;
	modelData->event[_idx_event_4].direction = 1;
	modelData->event[_idx_event_4].relation = 2;
	modelData->event[_idx_event_5].direction = 1;
	modelData->event[_idx_event_5].relation = 2;
	for(i = 1; i<=4000; i+=1) {
		modelData->event[_idx_event_6(i)].direction = 0;
		modelData->event[_idx_event_6(i)].relation = 2;
	}
	for(i = 1; i<=4000; i+=1) {
		modelData->event[_idx_event_7(i)].direction = 1;
		modelData->event[_idx_event_7(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(4002,8011,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("par_airconds_cont",1,16020,4002,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOD[_idx_out_exp_1]++;
	modelOutput->nDO[_idx_ptotals]++;
	SD_allocOutputMatrix(modelOutput, 4002, 16020);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "ptotals");
	cleanVector(outputs, 0, 1);
	modelOutput->OD[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_ptotals;
	cleanVector(discretes, 0, 16020);
	modelOutput->DO[_idx_ptotals][discretes[_idx_ptotals]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

