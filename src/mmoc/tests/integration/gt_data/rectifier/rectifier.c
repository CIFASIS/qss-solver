#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "rectifier.h"
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
	switch(idx) {
		case _eval_u(0): {
			_der_u(0) = 1000*(_U*sin(_w*_time)-_u(0));
			_der_u(1) = (0)/2;
	
			return;
		}
		case _eval_iL(0): {
			_der_iL(0) = (_u(0)-_iL(0)*(_R+_Rd))/_L;
			_der_iL(1) = (0)/2;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _iL(0)-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _u(0)-(0);
			_zc(1) = (0)/1;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_2: {
			_Rd = _Ron;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_Rd = _Roff;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _iL(0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _u(0);
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
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			x_ind = _idx_u(0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[0]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			x_ind = _idx_iL(0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if(1) {
			x_ind = _idx_u(0);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 1; row++) {
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
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_u(0): {
			_eval_dep_u(1) = 1000*(_U*sin(_w*_time)-_u(0));
			_eval_dep_u(2) = (0)/2;	
			_eval_dep_iL(1) = (_u(0)-_iL(0)*(_R+_Rd))/_L;
			_eval_dep_iL(2) = (0)/2;	
			break;
		}
		case _eval_iL(0): {
			_eval_dep_iL(1) = (_u(0)-_iL(0)*(_R+_Rd))/_L;
			_eval_dep_iL(2) = (0)/2;	
			break;
		}
	}
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	switch(idx) {
		case _eval_u(0): {
			_der_u(0) = 1000*(_U*sin(_w*_time)-_u(0));
	
	
			return;
		}
		case _eval_iL(0): {
			_der_iL(0) = (_u(0)-_iL(0)*(_R+_Rd))/_L;
	
	
			return;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(2,1,2,0,0,2,0,"rectifier");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(2*sizeof(int));
	int* discretes = (int*) malloc(1*sizeof(int));
	int* events = (int*) malloc(2*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_L = 0.001;
	_R = 10;
	_Rd = 100000;
	_Roff = 100000;
	_Ron = 1e-05;
	_U = 311;
	_w = 314.16;
	modelData->nSD[_idx_u(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_u(0)]++;
	modelData->nDS[_idx_u(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if(1) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_u(0)]++;
	modelData->nZS[_idx_event_1]++;
	modelData->nZS[_idx_event_2]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 2);
	modelData->SD[_idx_u(0)][states[_idx_u(0)]++] = _idx_u(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_u(0)][states[_idx_u(0)]++] = _idx_iL(0);
	cleanVector(states, 0, 2);
	modelData->DS[_idx_u(0)][states[_idx_u(0)]++] = _idx_u(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_u(0);
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			x_ind = _idx_u(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		if(1) {
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if(1) {
			x_ind = _idx_u(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 2);
	cleanVector(states, 0, 2);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_1;
	modelData->SZ[_idx_u(0)][states[_idx_u(0)]++] = _idx_event_2;
	cleanVector(events, 0, 2);
	modelData->ZS[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->ZS[_idx_event_2][events[_idx_event_2]++] = _idx_u(0);
	cleanVector(events, 0, 2);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	cleanVector(events, 0, 2);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rd;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rd;
	modelData->event[_idx_event_1].direction = -1;
	modelData->event[_idx_event_1].relation = 0;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(2,2,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("rectifier",2,1,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_u(0)]++;
	SD_allocOutputMatrix(modelOutput, 2, 1);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "iL");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "u");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_u(0);
	cleanVector(states, 0, 2);
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_u(0)][states[_idx_u(0)]++] = _idx_out_exp_2;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

