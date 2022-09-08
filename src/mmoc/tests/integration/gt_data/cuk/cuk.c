#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "cuk.h"
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
		case _eval_uC1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_der_uC1(0) = (_iD(0)-_iL(0))/_C1;
			_der_uC1(1) = (0)/2;
	
			return;
		}
		case _eval_iL1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_der_iL1(0) = (_U-_uC1(0)-_iD(0)*_Rd)/_L1;
			_der_iL1(1) = (0)/2;
	
			return;
		}
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
			_der_uC(1) = (0)/2;
	
			return;
		}
		case _eval_iL(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_der_iL(0) = (-_uC(0)-_iD(0)*_Rd)/_L;
			_der_iL(1) = (0)/2;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(_nextT);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _time-_lastT-_DC*_T-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_3: {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_s(0) = _diodeon*_iD(0)+(1-_diodeon)*_iD(0)*_Rd;
			_s(1) = 0;
			_zc(0) = _s(0)-(0);
			_zc(1) = (0)/1;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_lastT = _nextT;
			_nextT = _nextT+_T;
			_Rs = _ROn;
					return;
		}
		case _eval_event_2: {
			_Rs = _ROff;
					return;
		}
		case _eval_event_3: {
			_Rd = _ROn;
			_diodeon = 1;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_3: {
			_Rd = _ROff;
			_diodeon = 0;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _uC(0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _iL(0);
			return;
		}
		case _eval_out_exp_3: {
			_out = _uC1(0);
			return;
		}
		case _eval_out_exp_4: {
			_out = _iL1(0);
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
		x_ind = _idx_iL(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_iL1(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_uC1(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_iL1(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_uC1(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_iL1(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_uC1(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_uC1(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_uC(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_iL1(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_uC1(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_uC(0);
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
		case _eval_iL(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
			_eval_dep_uC1(2) = (0)/2;
			_eval_dep_iL1(1) = (_U-_uC1(0)-_iD(0)*_Rd)/_L1;
			_eval_dep_iL1(2) = (0)/2;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
			_eval_dep_iL(2) = (0)/2;
			break;
		}
		case _eval_iL1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
			_eval_dep_uC1(2) = (0)/2;
			_eval_dep_iL1(1) = (_U-_uC1(0)-_iD(0)*_Rd)/_L1;
			_eval_dep_iL1(2) = (0)/2;
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
			_eval_dep_iL(2) = (0)/2;
			break;
		}
		case _eval_uC(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
			_eval_dep_iL(2) = (0)/2;
			break;
		}
		case _eval_uC1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
			_iD(1) = 0;
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
			_eval_dep_uC1(2) = (0)/2;
			_eval_dep_iL1(1) = (_U-_uC1(0)-_iD(0)*_Rd)/_L1;
			_eval_dep_iL1(2) = (0)/2;
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
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
		case _eval_uC1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_uC1(1) = (_iD(0)-_iL(0))/_C1;
	
	
			continue;
		}
		case _eval_iL1(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL1(1) = (_U-_uC1(0)-_iD(0)*_Rd)/_L1;
	
	
			continue;
		}
		case _eval_uC(0): {
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
	
	
			continue;
		}
		case _eval_iL(0): {
			_iD(0) = (_Rs*(_iL(0)+_iL1(0))-_uC1(0))/(_Rd+_Rs);
	
			_eval_dep_iL(1) = (-_uC(0)-_iD(0)*_Rd)/_L;
	
	
			continue;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(4,5,3,0,2,4,2,"cuk");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(4*sizeof(int));
	int* discretes = (int*) malloc(5*sizeof(int));
	int* events = (int*) malloc(3*sizeof(int));
	int* outputs = (int*) malloc(4*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_C = 0.0001;
	_C1 = 0.0001;
	_DC = 0.25;
	_L = 0.0001;
	_L1 = 0.0001;
	_R = 10;
	_ROff = 100000;
	_ROn = 1e-05;
	_Rd = 100000;
	_Rs = 1e-05;
	_T = 0.0001;
	_U = 24;
	_nextT = _T;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL1(0)]++;
	modelData->nSD[_idx_iL1(0)]++;
	modelData->nSD[_idx_iL1(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nSD[_idx_uC1(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL1(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_uC1(0)]++;
	modelData->nDS[_idx_iL1(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
	}
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL1(0)]++;
	modelData->nSZ[_idx_uC1(0)]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_2]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 4);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL1(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_iL1(0);
	modelData->SD[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_uC1(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL1(0);
	modelData->SD[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	cleanVector(states, 0, 4);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_iL1(0);
	modelData->DS[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_iL1(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL1(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->DS[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_uC1(0);
	modelData->DS[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_uC1(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC1(0);
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL1(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC1(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL1(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC1(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL1(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC1(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC1(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL1(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC1(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 4);
	cleanVector(states, 0, 4);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_3;
	modelData->SZ[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_event_3;
	modelData->SZ[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_event_3;
	cleanVector(events, 0, 3);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL1(0);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_uC1(0);
	cleanVector(events, 0, 3);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	modelData->HZ[_idx_event_2][events[_idx_event_2]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	cleanVector(events, 0, 3);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL1(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL1(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL1(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_uC1(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_uC1(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_uC1(0);
	cleanVector(events, 0, 3);
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rd;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rs;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rs;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_diodeon;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = 0;
	modelData->event[_idx_event_3].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(4,3,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("cuk",4,5,4,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_3]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_iL1(0)]++;
	modelOutput->nSO[_idx_uC(0)]++;
	modelOutput->nSO[_idx_uC1(0)]++;
	SD_allocOutputMatrix(modelOutput, 4, 5);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "uC");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "iL");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "uC1");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "iL1");
	cleanVector(outputs, 0, 4);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_iL1(0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_uC(0);
	modelOutput->OS[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_uC1(0);
	cleanVector(states, 0, 4);
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_iL1(0)][states[_idx_iL1(0)]++] = _idx_out_exp_4;
	modelOutput->SO[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_uC1(0)][states[_idx_uC1(0)]++] = _idx_out_exp_3;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

