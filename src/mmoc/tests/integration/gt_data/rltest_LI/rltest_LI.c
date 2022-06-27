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

void MOD_handlerPos(int idx, double *x, double *d, double *a, double t)
{
}

void MOD_handlerNeg(int idx, double *x, double *d, double *a, double t)
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
	int row, row_t, eq_var, c_row, c_row_g;
	int col, col_g, col_t;
	int x_ind;
	double aux;
	int _d1;
	int _rg_d1;
	int i;
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_x1(1,0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_a1_idxs(eq_var);
		if((2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1+1,0);
			col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
			_apply_usage_alg_eq_2(_d1+1);
			aux = 0;
			dvdx->dg_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1,0);
			col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
			_apply_usage_alg_eq_2(_d1+1);
			aux = 0;
			dvdx->dg_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x1_idxs(eq_var);
		_apply_usage_eq_1(_d1);
		aux = 0;
		if((2 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1+1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_a1(_rg_d1+1,0) - 1;
			col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		}
		if((1 <= _d1 && _d1 <= 9) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_a1(_rg_d1+1,0) - 1;
			col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		}
		_apply_usage_eq_1(_d1);
		aux = 0;
		 if((1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_x1(1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_a1(_rg_d1,0) - 0;
			col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_a1(_rg_d1,0) - 1;
			col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1-1,0);
			col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
			c_row_g = _idx_a1(_rg_d1,0) - 1;
			col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
			dvdx->df_dx[0]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_x1(10,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_a1(10,0) - 1;
		col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		x_ind = _idx_x1(9,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		c_row_g = _idx_a1(9,0) - 1;
		col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
		dvdx->df_dx[1]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		x_ind = _idx_x2(1,0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_x1(10,0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 9; row++) {
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
	
			_der_x1(10,0) = (_a1(10,0)-_x2(1,0))/_C;
	
	
			return;
		}
		case _eval_x2(1,0): {
			_der_x2(1,0) = _x1(10,0)/_L;
	
	
			return;
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
			_der_x1(i,0) = (pow((_a1(i,0)-_a1(i+1,0)),3))/_C;
	
	
		}
		return;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(11,0,0,0,10,3,2,"rltest_LI");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(11*sizeof(int));
	int* outputs = (int*) malloc(11*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _rg_d1;
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
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_a1_idxs(eq_var);
		if((2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 9)) {
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x1_idxs(eq_var);
		if((2 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1 && _d1 <= 9) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		 if((1 <= _d1 && _d1 <= 1)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d1 && _d1 <= 9)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d1 && _d1 <= 9)) {
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
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
	cleanVector(states, 0, 11);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_x1(1,0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 11);
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_alg_eq_2_var_idxs(row, eq_var);
		_get_a1_idxs(eq_var);
		if((2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1+1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1,0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 11);
	for(row = 1; row <= 9; row++) {
		c_row = _c_index(row);
		_get_eq_1_var_idxs(row, eq_var);
		_get_x1_idxs(eq_var);
		if((2 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1+1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1 && _d1 <= 9) && (2 <= _d1 + 1 && _d1 + 1 <= 10)) {
			x_ind = _idx_x1(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		 if((1 <= _d1 && _d1 <= 1)) {
			x_ind = _idx_x1(1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d1 && _d1 <= 9)) {
			x_ind = _idx_x1(_d1-1,0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 11);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_x1(10,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_x1(9,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_x2(1,0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 11);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_x1(10,0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 11);
	SD_setupJacMatrices(modelData->jac_matrices);
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
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

