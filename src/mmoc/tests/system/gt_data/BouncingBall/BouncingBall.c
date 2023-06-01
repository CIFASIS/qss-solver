#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "BouncingBall.h"
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
	settings->method = 7;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	switch(idx) {
		case _eval_ball1_y(0): {
			_der_ball1_y(0) = _ball1_v(0);
			_der_ball1_y(1) = (0)/2;
			_der_ball1_y(2) = (0)/6;
	
			return;
		}
		case _eval_ball1_v(0): {
			_fixed1_flange1_v(0) = 0;
			_fixed1_flange1_v(1) = 0;
			_fixed1_flange1_v(2) = 0;
			_ball1_flange1_v(0) = _ball1_v(0);
			_ball1_flange1_v(1) = 0;
			_ball1_flange1_v(2) = 0;
			_spring1_flange2_v(0) = _fixed1_flange1_v(0);
			_spring1_flange2_v(1) = 0;
			_spring1_flange2_v(2) = 0;
			_spring1_flange1_v(0) = _ball1_flange1_v(0);
			_spring1_flange1_v(1) = 0;
			_spring1_flange1_v(2) = 0;
			_fixed1_flange1_s(0) = _fixed1_s0;
			_fixed1_flange1_s(1) = 0;
			_fixed1_flange1_s(2) = 0;
			_ball1_flange1_s(0) = _ball1_y(0);
			_ball1_flange1_s(1) = 0;
			_ball1_flange1_s(2) = 0;
			_spring1_flange2_s(0) = _fixed1_flange1_s(0);
			_spring1_flange2_s(1) = 0;
			_spring1_flange2_s(2) = 0;
			_spring1_flange1_s(0) = _ball1_flange1_s(0);
			_spring1_flange1_s(1) = 0;
			_spring1_flange1_s(2) = 0;
			_spring1_dv(0) = _spring1_flange1_v(0)-_spring1_flange2_v(0);
			_spring1_dv(1) = 0;
			_spring1_dv(2) = 0;
			_spring1_dy(0) = _spring1_flange1_s(0)-_spring1_flange2_s(0);
			_spring1_dy(1) = 0;
			_spring1_dy(2) = 0;
			_spring1_flange1_f(0) = _d0*(_spring1_b*_spring1_dv(0)+_spring1_k*_spring1_dy(0))+(1-_d0)*(0);
			_spring1_flange1_f(1) = 0;
			_spring1_flange1_f(2) = 0;
			_ball1_flange1_f(0) = (-_spring1_flange1_f(0));
			_ball1_flange1_f(1) = 0;
			_ball1_flange1_f(2) = 0;
			_der_ball1_v(0) = (_ball1_flange1_f(0)-_ball1_m*_ball1_g)*(1/(_ball1_m));
			_der_ball1_v(1) = (0)/2;
			_der_ball1_v(2) = (0)/6;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_fixed1_flange1_s(0) = _fixed1_s0;
			_fixed1_flange1_s(1) = 0;
			_fixed1_flange1_s(2) = 0;
			_ball1_flange1_s(0) = _ball1_y(0);
			_ball1_flange1_s(1) = 0;
			_ball1_flange1_s(2) = 0;
			_spring1_flange2_s(0) = _fixed1_flange1_s(0);
			_spring1_flange2_s(1) = 0;
			_spring1_flange2_s(2) = 0;
			_spring1_flange1_s(0) = _ball1_flange1_s(0);
			_spring1_flange1_s(1) = 0;
			_spring1_flange1_s(2) = 0;
			_spring1_dy(0) = _spring1_flange1_s(0)-_spring1_flange2_s(0);
			_spring1_dy(1) = 0;
			_spring1_dy(2) = 0;
			_zc(0) = _spring1_dy(0)-(0);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = _d0-(0);
			_zc(1) = (0)/1;
			_zc(2) = (0)/2;
	
			return;
		}
		case _eval_event_3: {
			_zc(0) = _d0-(1);
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
			_d0 = 0;
					return;
		}
		case _eval_event_2: {
			_d1 = 1;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_d0 = 1;
					return;
		}
		case _eval_event_3: {
			_d1 = 0;
					return;
		}
	}
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _ball1_y(0);
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
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->dg_dx[7]->index[c_row], dvdx->dg_dx[7]->size[c_row], x_ind);
		c_row_g = _idx_ball1_flange1_v(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->dg_dx[7]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->dg_dx[8]->index[c_row], dvdx->dg_dx[8]->size[c_row], x_ind);
		c_row_g = _idx_spring1_flange1_v(0) - 7;
		col_g = pos(dvdx->dg_dx[7]->index[c_row_g], dvdx->dg_dx[7]->size[c_row_g], x_ind);
		dvdx->dg_dx[8]->value[c_row][col] += aux * dvdx->dg_dx[7]->value[c_row_g][col_g];
		aux = 0;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->dg_dx[3]->index[c_row], dvdx->dg_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[3]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->dg_dx[6]->index[c_row], dvdx->dg_dx[6]->size[c_row], x_ind);
		c_row_g = _idx_ball1_flange1_s(0) - 3;
		col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
		dvdx->dg_dx[6]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->dg_dx[9]->index[c_row], dvdx->dg_dx[9]->size[c_row], x_ind);
		c_row_g = _idx_spring1_flange1_s(0) - 6;
		col_g = pos(dvdx->dg_dx[6]->index[c_row_g], dvdx->dg_dx[6]->size[c_row_g], x_ind);
		dvdx->dg_dx[9]->value[c_row][col] += aux * dvdx->dg_dx[6]->value[c_row_g][col_g];
		aux = 0;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->dg_dx[10]->index[c_row], dvdx->dg_dx[10]->size[c_row], x_ind);
		c_row_g = _idx_spring1_dv(0) - 8;
		col_g = pos(dvdx->dg_dx[8]->index[c_row_g], dvdx->dg_dx[8]->size[c_row_g], x_ind);
		dvdx->dg_dx[10]->value[c_row][col] += aux * dvdx->dg_dx[8]->value[c_row_g][col_g];
		aux = 0;
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->dg_dx[10]->index[c_row], dvdx->dg_dx[10]->size[c_row], x_ind);
		c_row_g = _idx_spring1_dy(0) - 9;
		col_g = pos(dvdx->dg_dx[9]->index[c_row_g], dvdx->dg_dx[9]->size[c_row_g], x_ind);
		dvdx->dg_dx[10]->value[c_row][col] += aux * dvdx->dg_dx[9]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->dg_dx[12]->index[c_row], dvdx->dg_dx[12]->size[c_row], x_ind);
		c_row_g = _idx_spring1_flange1_f(0) - 10;
		col_g = pos(dvdx->dg_dx[10]->index[c_row_g], dvdx->dg_dx[10]->size[c_row_g], x_ind);
		dvdx->dg_dx[12]->value[c_row][col] += aux * dvdx->dg_dx[10]->value[c_row_g][col_g];
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->dg_dx[12]->index[c_row], dvdx->dg_dx[12]->size[c_row], x_ind);
		c_row_g = _idx_spring1_flange1_f(0) - 10;
		col_g = pos(dvdx->dg_dx[10]->index[c_row_g], dvdx->dg_dx[10]->size[c_row_g], x_ind);
		dvdx->dg_dx[12]->value[c_row][col] += aux * dvdx->dg_dx[10]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_ball1_v(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_ball1_flange1_f(0) - 12;
		col_g = pos(dvdx->dg_dx[12]->index[c_row_g], dvdx->dg_dx[12]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[12]->value[c_row_g][col_g];
		x_ind = _idx_ball1_y(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_ball1_flange1_f(0) - 12;
		col_g = pos(dvdx->dg_dx[12]->index[c_row_g], dvdx->dg_dx[12]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[12]->value[c_row_g][col_g];
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
		case _eval_ball1_v(0): {
			_fixed1_flange1_v(0) = 0;
			_fixed1_flange1_v(1) = 0;
			_fixed1_flange1_v(2) = 0;
			_ball1_flange1_v(0) = _ball1_v(0);
			_ball1_flange1_v(1) = 0;
			_ball1_flange1_v(2) = 0;
			_spring1_flange2_v(0) = _fixed1_flange1_v(0);
			_spring1_flange2_v(1) = 0;
			_spring1_flange2_v(2) = 0;
			_spring1_flange1_v(0) = _ball1_flange1_v(0);
			_spring1_flange1_v(1) = 0;
			_spring1_flange1_v(2) = 0;
			_fixed1_flange1_s(0) = _fixed1_s0;
			_fixed1_flange1_s(1) = 0;
			_fixed1_flange1_s(2) = 0;
			_ball1_flange1_s(0) = _ball1_y(0);
			_ball1_flange1_s(1) = 0;
			_ball1_flange1_s(2) = 0;
			_spring1_flange2_s(0) = _fixed1_flange1_s(0);
			_spring1_flange2_s(1) = 0;
			_spring1_flange2_s(2) = 0;
			_spring1_flange1_s(0) = _ball1_flange1_s(0);
			_spring1_flange1_s(1) = 0;
			_spring1_flange1_s(2) = 0;
			_spring1_dv(0) = _spring1_flange1_v(0)-_spring1_flange2_v(0);
			_spring1_dv(1) = 0;
			_spring1_dv(2) = 0;
			_spring1_dy(0) = _spring1_flange1_s(0)-_spring1_flange2_s(0);
			_spring1_dy(1) = 0;
			_spring1_dy(2) = 0;
			_spring1_flange1_f(0) = _d0*(_spring1_b*_spring1_dv(0)+_spring1_k*_spring1_dy(0))+(1-_d0)*(0);
			_spring1_flange1_f(1) = 0;
			_spring1_flange1_f(2) = 0;
			_ball1_flange1_f(0) = (-_spring1_flange1_f(0));
			_ball1_flange1_f(1) = 0;
			_ball1_flange1_f(2) = 0;
			_eval_dep_ball1_y(1) = _ball1_v(0);
			_eval_dep_ball1_y(2) = (0)/2;
			_eval_dep_ball1_y(3) = (0)/6;
			_eval_dep_ball1_v(1) = (_ball1_flange1_f(0)-_ball1_m*_ball1_g)*(1/(_ball1_m));
			_eval_dep_ball1_v(2) = (0)/2;
			_eval_dep_ball1_v(3) = (0)/6;
			break;
		}
		case _eval_ball1_y(0): {
			_fixed1_flange1_v(0) = 0;
			_fixed1_flange1_v(1) = 0;
			_fixed1_flange1_v(2) = 0;
			_ball1_flange1_v(0) = _ball1_v(0);
			_ball1_flange1_v(1) = 0;
			_ball1_flange1_v(2) = 0;
			_spring1_flange2_v(0) = _fixed1_flange1_v(0);
			_spring1_flange2_v(1) = 0;
			_spring1_flange2_v(2) = 0;
			_spring1_flange1_v(0) = _ball1_flange1_v(0);
			_spring1_flange1_v(1) = 0;
			_spring1_flange1_v(2) = 0;
			_fixed1_flange1_s(0) = _fixed1_s0;
			_fixed1_flange1_s(1) = 0;
			_fixed1_flange1_s(2) = 0;
			_ball1_flange1_s(0) = _ball1_y(0);
			_ball1_flange1_s(1) = 0;
			_ball1_flange1_s(2) = 0;
			_spring1_flange2_s(0) = _fixed1_flange1_s(0);
			_spring1_flange2_s(1) = 0;
			_spring1_flange2_s(2) = 0;
			_spring1_flange1_s(0) = _ball1_flange1_s(0);
			_spring1_flange1_s(1) = 0;
			_spring1_flange1_s(2) = 0;
			_spring1_dv(0) = _spring1_flange1_v(0)-_spring1_flange2_v(0);
			_spring1_dv(1) = 0;
			_spring1_dv(2) = 0;
			_spring1_dy(0) = _spring1_flange1_s(0)-_spring1_flange2_s(0);
			_spring1_dy(1) = 0;
			_spring1_dy(2) = 0;
			_spring1_flange1_f(0) = _d0*(_spring1_b*_spring1_dv(0)+_spring1_k*_spring1_dy(0))+(1-_d0)*(0);
			_spring1_flange1_f(1) = 0;
			_spring1_flange1_f(2) = 0;
			_ball1_flange1_f(0) = (-_spring1_flange1_f(0));
			_ball1_flange1_f(1) = 0;
			_ball1_flange1_f(2) = 0;
			_eval_dep_ball1_v(1) = (_ball1_flange1_f(0)-_ball1_m*_ball1_g)*(1/(_ball1_m));
			_eval_dep_ball1_v(2) = (0)/2;
			_eval_dep_ball1_v(3) = (0)/6;
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
		case _eval_ball1_y(0): {
			_eval_dep_ball1_y(1) = _ball1_v(0);
	
	
			continue;
		}
		case _eval_ball1_v(0): {
			_fixed1_flange1_v(0) = 0;
	
			_ball1_flange1_v(0) = _ball1_v(0);
	
			_spring1_flange2_v(0) = _fixed1_flange1_v(0);
	
			_spring1_flange1_v(0) = _ball1_flange1_v(0);
	
			_fixed1_flange1_s(0) = _fixed1_s0;
	
			_ball1_flange1_s(0) = _ball1_y(0);
	
			_spring1_flange2_s(0) = _fixed1_flange1_s(0);
	
			_spring1_flange1_s(0) = _ball1_flange1_s(0);
	
			_spring1_dv(0) = _spring1_flange1_v(0)-_spring1_flange2_v(0);
	
			_spring1_dy(0) = _spring1_flange1_s(0)-_spring1_flange2_s(0);
	
			_spring1_flange1_f(0) = _d0*(_spring1_b*_spring1_dv(0)+_spring1_k*_spring1_dy(0))+(1-_d0)*(0);
	
			_ball1_flange1_f(0) = (-_spring1_flange1_f(0));
	
			_eval_dep_ball1_v(1) = (_ball1_flange1_f(0)-_ball1_m*_ball1_g)*(1/(_ball1_m));
	
	
			continue;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(2,2,3,0,14,2,14,"BouncingBall");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(14*sizeof(int));
	int* states = (int*) malloc(2*sizeof(int));
	int* discretes = (int*) malloc(2*sizeof(int));
	int* events = (int*) malloc(3*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_ball1_g = 9.800000e+00;
	_ball1_m = 1;
	_init_ball1_y(0) = 10;
	_fixed1_s0 = 0;
	_spring1_b = 10;
	_spring1_k = 10000;
	modelData->nSD[_idx_ball1_v(0)]++;
	modelData->nSD[_idx_ball1_v(0)]++;
	modelData->nSD[_idx_ball1_y(0)]++;
	modelData->nDS[_idx_ball1_y(0)]++;
	modelData->nDS[_idx_ball1_v(0)]++;
	modelData->nDS[_idx_ball1_v(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[7]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[8]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[3]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[6]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[9]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[10]->size[c_row]++;
			modelData->jac_matrices->dg_dx[10]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[12]->size[c_row]++;
			modelData->jac_matrices->dg_dx[12]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	modelData->nSZ[_idx_ball1_y(0)]++;
	modelData->nZS[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHD[_idx_event_1]++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 2);
	modelData->SD[_idx_ball1_v(0)][states[_idx_ball1_v(0)]++] = _idx_ball1_y(0);
	modelData->SD[_idx_ball1_v(0)][states[_idx_ball1_v(0)]++] = _idx_ball1_v(0);
	modelData->SD[_idx_ball1_y(0)][states[_idx_ball1_y(0)]++] = _idx_ball1_v(0);
	cleanVector(states, 0, 2);
	modelData->DS[_idx_ball1_y(0)][states[_idx_ball1_y(0)]++] = _idx_ball1_v(0);
	modelData->DS[_idx_ball1_v(0)][states[_idx_ball1_v(0)]++] = _idx_ball1_v(0);
	modelData->DS[_idx_ball1_v(0)][states[_idx_ball1_v(0)]++] = _idx_ball1_y(0);
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->dg_dx[7]->index[c_row],modelData->jac_matrices->dg_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[7]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->dg_dx[8]->index[c_row],modelData->jac_matrices->dg_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[8]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->dg_dx[3]->index[c_row],modelData->jac_matrices->dg_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[3]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->dg_dx[6]->index[c_row],modelData->jac_matrices->dg_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[6]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->dg_dx[9]->index[c_row],modelData->jac_matrices->dg_dx[9]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[9]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[9]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->dg_dx[10]->index[c_row],modelData->jac_matrices->dg_dx[10]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[10]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[10]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->dg_dx[10]->index[c_row],modelData->jac_matrices->dg_dx[10]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[10]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[10]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 14);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->dg_dx[12]->index[c_row],modelData->jac_matrices->dg_dx[12]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[12]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[12]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->dg_dx[12]->index[c_row],modelData->jac_matrices->dg_dx[12]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[12]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[12]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 2);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_ball1_v(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_ball1_y(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 2);
	modelData->SZ[_idx_ball1_y(0)][states[_idx_ball1_y(0)]++] = _idx_event_1;
	cleanVector(events, 0, 3);
	modelData->ZS[_idx_event_1][events[_idx_event_1]++] = _idx_ball1_y(0);
	cleanVector(events, 0, 3);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	cleanVector(events, 0, 3);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_ball1_v(0);
	cleanVector(events, 0, 3);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_d0;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_d1;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_d1;
	modelData->event[_idx_event_1].direction = 0;
	modelData->event[_idx_event_1].relation = 0;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = -1;
	modelData->event[_idx_event_3].relation = 0;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(2,3,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.01;
		simulator->output = SD_Output("BouncingBall",1,2,2,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_ball1_y(0)]++;
	SD_allocOutputMatrix(modelOutput, 2, 2);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "ball1_y");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_ball1_y(0);
	cleanVector(states, 0, 2);
	modelOutput->SO[_idx_ball1_y(0)][states[_idx_ball1_y(0)]++] = _idx_out_exp_1;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(discretes);
	free(events);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

