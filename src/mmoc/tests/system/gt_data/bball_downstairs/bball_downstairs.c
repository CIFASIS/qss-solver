#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "bball_downstairs.h"
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
	switch(idx) {
		case _eval_y(0): {
			_der_y(0) = _vy(0);
			_der_y(1) = (0)/2;
			_der_y(2) = (0)/6;
	
			return;
		}
		case _eval_vy(0): {
			_der_vy(0) = -9.800000e+00-1.000000e-01*_vy(0)-_contact*((_y(0)-_stair)*1.000000e+06+_vy(0)*30);
			_der_vy(1) = (0)/2;
			_der_vy(2) = (0)/6;
	
			return;
		}
		case _eval_x(0): {
			_der_x(0) = _vx(0);
			_der_x(1) = (0)/2;
			_der_x(2) = (0)/6;
	
			return;
		}
		case _eval_vx(0): {
			_der_vx(0) = -1.000000e-01*_vx(0);
			_der_vx(1) = (0)/2;
			_der_vx(2) = (0)/6;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _y(0)-(_stair);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _x(0)-11+_stair-(0);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_contact = 0;
					return;
		}
		case _eval_event_2: {
			_stair = _stair-1;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_contact = 1;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _y(0);
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
		x_ind = _idx_vy(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_vy(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_y(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_vx(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_vx(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
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
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 3
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[3]->size[row]; col++) {
	    row_t = dvdx->df_dx[3]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[3]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_vx(0): {
			_eval_dep_x(1) = _vx(0);
			_eval_dep_x(2) = (0)/2;
			_eval_dep_x(3) = (0)/6;
			_eval_dep_vx(1) = -1.000000e-01*_vx(0);
			_eval_dep_vx(2) = (0)/2;
			_eval_dep_vx(3) = (0)/6;
			break;
		}
		case _eval_vy(0): {
			_eval_dep_y(1) = _vy(0);
			_eval_dep_y(2) = (0)/2;
			_eval_dep_y(3) = (0)/6;
			_eval_dep_vy(1) = -9.800000e+00-1.000000e-01*_vy(0)-_contact*((_y(0)-_stair)*1.000000e+06+_vy(0)*30);
			_eval_dep_vy(2) = (0)/2;
			_eval_dep_vy(3) = (0)/6;
			break;
		}
		case _eval_y(0): {
			_eval_dep_vy(1) = -9.800000e+00-1.000000e-01*_vy(0)-_contact*((_y(0)-_stair)*1.000000e+06+_vy(0)*30);
			_eval_dep_vy(2) = (0)/2;
			_eval_dep_vy(3) = (0)/6;
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
		case _eval_y(0): {
			_eval_dep_y(1) = _vy(0);
	
	
			continue;
		}
		case _eval_vy(0): {
			_eval_dep_vy(1) = -9.800000e+00-1.000000e-01*_vy(0)-_contact*((_y(0)-_stair)*1.000000e+06+_vy(0)*30);
	
	
			continue;
		}
		case _eval_x(0): {
			_eval_dep_x(1) = _vx(0);
	
	
			continue;
		}
		case _eval_vx(0): {
			_eval_dep_vx(1) = -1.000000e-01*_vx(0);
	
	
			continue;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(4,2,2,0,0,4,0,"bball_downstairs");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(4*sizeof(int));
	int* discretes = (int*) malloc(2*sizeof(int));
	int* events = (int*) malloc(2*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_stair = 10;
	_init_vx(0) = 5.000000e-01;
	_init_x(0) = 5.750000e-01;
	_init_y(0) = 1.050000e+01;
	modelData->nSD[_idx_vx(0)]++;
	modelData->nSD[_idx_vx(0)]++;
	modelData->nSD[_idx_vy(0)]++;
	modelData->nSD[_idx_vy(0)]++;
	modelData->nSD[_idx_y(0)]++;
	modelData->nDS[_idx_x(0)]++;
	modelData->nDS[_idx_vx(0)]++;
	modelData->nDS[_idx_y(0)]++;
	modelData->nDS[_idx_vy(0)]++;
	modelData->nDS[_idx_vy(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
	}
	modelData->nSZ[_idx_x(0)]++;
	modelData->nSZ[_idx_y(0)]++;
	modelData->nZS[_idx_event_2]++;
	modelData->nZS[_idx_event_1]++;
	modelData->nHZ[_idx_event_2]++;
	modelData->nHZ[_idx_event_2]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 4);
	modelData->SD[_idx_vx(0)][states[_idx_vx(0)]++] = _idx_x(0);
	modelData->SD[_idx_vx(0)][states[_idx_vx(0)]++] = _idx_vx(0);
	modelData->SD[_idx_vy(0)][states[_idx_vy(0)]++] = _idx_y(0);
	modelData->SD[_idx_vy(0)][states[_idx_vy(0)]++] = _idx_vy(0);
	modelData->SD[_idx_y(0)][states[_idx_y(0)]++] = _idx_vy(0);
	cleanVector(states, 0, 4);
	modelData->DS[_idx_x(0)][states[_idx_x(0)]++] = _idx_vx(0);
	modelData->DS[_idx_vx(0)][states[_idx_vx(0)]++] = _idx_vx(0);
	modelData->DS[_idx_y(0)][states[_idx_y(0)]++] = _idx_vy(0);
	modelData->DS[_idx_vy(0)][states[_idx_vy(0)]++] = _idx_vy(0);
	modelData->DS[_idx_vy(0)][states[_idx_vy(0)]++] = _idx_y(0);
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_vy(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_vy(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_y(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_vx(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_vx(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	cleanVector(states, 0, 4);
	modelData->SZ[_idx_x(0)][states[_idx_x(0)]++] = _idx_event_2;
	modelData->SZ[_idx_y(0)][states[_idx_y(0)]++] = _idx_event_1;
	cleanVector(events, 0, 2);
	modelData->ZS[_idx_event_2][events[_idx_event_2]++] = _idx_x(0);
	modelData->ZS[_idx_event_1][events[_idx_event_1]++] = _idx_y(0);
	cleanVector(events, 0, 2);
	modelData->HZ[_idx_event_2][events[_idx_event_2]++] = _idx_event_1;
	modelData->HZ[_idx_event_2][events[_idx_event_2]++] = _idx_event_2;
	cleanVector(events, 0, 2);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_vy(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_vy(0);
	cleanVector(events, 0, 2);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_contact;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_stair;
	modelData->event[_idx_event_1].direction = 0;
	modelData->event[_idx_event_1].relation = 0;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(4,2,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("bball_downstairs",1,2,4,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_y(0)]++;
	SD_allocOutputMatrix(modelOutput, 4, 2);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "y");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_y(0);
	cleanVector(states, 0, 4);
	modelOutput->SO[_idx_y(0)][states[_idx_y(0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

