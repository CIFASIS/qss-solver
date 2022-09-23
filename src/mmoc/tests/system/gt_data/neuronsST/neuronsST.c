#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "neuronsST.h"
#include "pkg_math.h"
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
	int _d1;
	int i;
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 10)) {
			_der_V(i,0) = (-(_V(i,0))+_Is(i,0))/_taum*_active(i);
			_der_V(i,1) = (0)/2;
	
		}
		return;
	}
	if (_is_var_Is(idx)) {
		_get_Is_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 10)) {
			_der_Is(i,0) = (-_Is(i,0)+_mu+_sigma*sqrt(_taum)*_xi(i)/_dt)/_taus+1e-80*_tnext;
			_der_Is(i,1) = (0)/2;
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	int _d1;
	int i;
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_tnext);
			_zc(1) = (0)/1;
	
			return;
		}
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 10)) {
			_zc(0) = _V(i,0)-(_theta);
			_zc(1) = (0)/1;
	
		}
		return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 10)) {
			_zc(0) = _time-(_tnextr(i));
			_zc(1) = (0)/1;
	
		}
		return;
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	int _d1;
	int i;
	int j;
	switch(idx) {
		case _eval_event_1: {
			for(i = 1; i<=10; i+=1) {
		_xi(i) = (__math__rand(1)-0.5)*sqrt(_dt);
	}
			_tnext = _time+_dt;
					return;
		}
	}
	if (_is_var_event_2(idx)) {
		_get_event_2_idxs(idx);
		_apply_usage_event_2(_d1);
		if ((i >= 1 && i <= 10)) {
			_init_V(i,0) = _Vr;
			_tnextr(i) = _time+_taur;
			_active(i) = 0;
			for(j = 1; j<=_Cn(i); j+=1) {
		_init_Is(_C(i,j),0) = _init_Is(_C(i,j),0)+_J(i)*_taum/_taus;
	}
			}
			return;
	}
	if (_is_var_event_3(idx)) {
		_get_event_3_idxs(idx);
		_apply_usage_event_3(_d1);
		if ((i >= 1 && i <= 10)) {
			_active(i) = 1;
			}
			return;
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _V(1,0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _Is(1,0);
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
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_Is(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_V(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			_apply_usage_eq_1(_d1);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_Is_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_Is(_d1,0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 10; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 1
	for (row = 0; row < 10; row++) {
	  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
	    row_t = dvdx->df_dx[1]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int i;
	if (_is_var_Is(idx)) {
		_get_Is_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 10)) {
			_eval_dep_V(i,1) = (-(_V(i,0))+_Is(i,0))/_taum*_active(i);
			_eval_dep_V(i,2) = (0)/2;
		}
	}
	if (_is_var_Is(idx)) {
		_get_Is_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 10)) {
			_eval_dep_Is(i,1) = (-_Is(i,0)+_mu+_sigma*sqrt(_taum)*_xi(i)/_dt)/_taus+1e-80*_tnext;
			_eval_dep_Is(i,2) = (0)/2;
		}
	}
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 10)) {
			_eval_dep_V(i,1) = (-(_V(i,0))+_Is(i,0))/_taum*_active(i);
			_eval_dep_V(i,2) = (0)/2;
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
	if (_is_var_V(idx)) {
		_get_V_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 10)) {
			_der_V(i,0) = (-(_V(i,0))+_Is(i,0))/_taum*_active(i);
	
	
		}
		return;
	}
	if (_is_var_Is(idx)) {
		_get_Is_idxs(idx);
		_apply_usage_eq_2(_d1);
		if ((i >= 1 && i <= 10)) {
			_der_Is(i,0) = (-_Is(i,0)+_mu+_sigma*sqrt(_taum)*_xi(i)/_dt)/_taus+1e-80*_tnext;
	
	
		}
		return;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(20,31,21,0,0,2,0,"neuronsST");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(20*sizeof(int));
	int* discretes = (int*) malloc(31*sizeof(int));
	int* events = (int*) malloc(21*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _d2;
	int _rg_d1;
	int i;
	int j;
	_Cm = 2.5e-10;
	_EL = -0.065;
	_Jmean = 8.78e-11;
	_Vr = -0.065;
	for(_d1 = 1; _d1<=10; _d1+=1) {
			_active(_d1) = 1;
	}
	_dt = 0.0001;
	_kext = 10000;
	_mu = _Jmean*_taum*_vbg*_kext;
	_sigma = sqrt(_mu*_Jmean);
	_taum = 0.01;
	_taur = 0.002;
	_taus = 0.0005;
	_theta = -0.05;
	_tnext = 1e-20;
	for(_d1 = 1; _d1<=10; _d1+=1) {
			_tnextr(_d1) = 1;
	}
	_vbg = 10;
	_vext = _vbg*_kext;
	for(i = 1; i<=10; i+=1) {
		_xi(i) = (__math__rand(1)-0.5)*sqrt(_dt);
		_init_V(i,0) = __math__rand(1)*0.01-0.065;
		_Cn(i) = __math__integer(__math__rand(5+1));
		_J(i) = (__math__rand(1)-0.5)*2e-11+8.78e-11;
	}
	for(i = 1; i<=10; i+=1) {
	for(j = 1; j<=_Cn(i); j+=1) {
		_C(i,j) = __math__rand_int(10,i)+1;
	}
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nSD[_idx_Is(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nSD[_idx_Is(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nSD[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nDS[_idx_Is(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nDS[_idx_V(_d1,0)]++;
	}
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_Is_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nSZ[_idx_V(_d1,0)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nZS[_idx_event_2(_d1)]++;
	}
	modelData->nHZ[_idx_event_1]++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nHZ[_idx_event_2(_d1)]++;
	}
	modelData->nHD[_idx_event_1]++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nHD[_idx_event_2(_d1)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->nHD[_idx_event_3(_d1)]++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		for(_d2 = 1; _d2<=_Cn(_d1); _d2+=1) {
			modelData->nHD[_idx_event_2(_d1)]++;
		}
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].nLHSDsc++;
	}
	modelData->event[_idx_event_1].nLHSDsc++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSDsc++;
	}
	modelData->event[_idx_event_1].nLHSDsc++;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].nLHSSt++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 20);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->SD[_idx_Is(_d1,0)][states[_idx_Is(_d1,0)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->SD[_idx_Is(_d1,0)][states[_idx_Is(_d1,0)]++] = _idx_Is(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->SD[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_V(_d1,0);
	}
	cleanVector(states, 0, 20);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_Is(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->DS[_idx_Is(_d1,0)][states[_idx_Is(_d1,0)]++] = _idx_Is(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->DS[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_V(_d1,0);
	}
	cleanVector(states, 0, 20);
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_V_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_Is(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_V(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 20);
	for(row = 1; row <= 10; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_Is_idxs(eq_var);
		if((1 <= _d1 && _d1 <= 10) && (1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_Is(_d1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 20);
	cleanVector(states, 0, 20);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->SZ[_idx_V(_d1,0)][states[_idx_V(_d1,0)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 21);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->ZS[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_V(_d1,0);
	}
	cleanVector(events, 0, 21);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_3(_d1);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->HZ[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_event_2(_d1);
	}
	cleanVector(events, 0, 21);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_Is(_d1,0);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->HD[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->HD[_idx_event_3(_d1)][events[_idx_event_3(_d1)]++] = _idx_V(_d1,0);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		for(_d2 = 1; _d2<=_Cn(_d1); _d2+=1) {
			modelData->HD[_idx_event_2(_d1)][events[_idx_event_2(_d1)]++] = _idx_Is(_C(_d1,_d2),0);
		}
	}
	cleanVector(events, 0, 21);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_active(_d1);
	}
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_3(_d1)].LHSDsc[events[_idx_event_3(_d1)]++] = _idx_active(_d1);
	}
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_tnext;
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSDsc[events[_idx_event_2(_d1)]++] = _idx_tnextr(_d1);
	}
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_xi(_d1);
	cleanVector(events, 0, 21);
	for(_d1 = 1; _d1<=10; _d1+=1) {
		modelData->event[_idx_event_2(_d1)].LHSSt[events[_idx_event_2(_d1)]++] = _idx_V(_d1,0);
	}
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	for(i = 1; i<=10; i+=1) {
		modelData->event[_idx_event_2(i)].direction = 1;
		modelData->event[_idx_event_2(i)].relation = 2;
	}
	for(i = 1; i<=10; i+=1) {
		modelData->event[_idx_event_3(i)].direction = 1;
		modelData->event[_idx_event_3(i)].relation = 2;
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(20,21,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("neuronsST",2,31,20,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_Is(1,0)]++;
	modelOutput->nSO[_idx_V(1,0)]++;
	SD_allocOutputMatrix(modelOutput, 20, 31);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "V[1]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "Is[1]");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_Is(1,0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_V(1,0);
	cleanVector(states, 0, 20);
	modelOutput->SO[_idx_Is(1,0)][states[_idx_Is(1,0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_V(1,0)][states[_idx_V(1,0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

