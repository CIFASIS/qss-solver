#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "advection2D.h"
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
	settings->method = 11;
}

void MOD_definition(double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int _d2;
	int i;
	int j;
	_der_u(1,1) = -_ax*_u(1,1)/_dx-_ay*_u(1,1)/_dy+_r*(pow(_u(1,1),2)-pow(_u(1,1),3));
	for(i = 2; i<=20; i+=1) {
		_der_u(i,1) = -_ax*_u(i,1)/_dx-_ay*(_u(i,1)-_u(i-1,1))/_dy+_r*(pow(_u(i,1),2)-pow(_u(i,1),3));
	}
	for(j = 2; j<=20; j+=1) {
		_der_u(1,j) = -_ax*(_u(1,j)-_u(1,j-1))/_dx-_ay*_u(1,j)/_dy+_r*(pow(_u(1,j),2)-pow(_u(1,j),3));
	}
	for(i = 2; i<=20; i+=1) {
		for(j = 2; j<=20; j+=1) {
			_der_u(i,j) = -_ax*(_u(i,j)-_u(i,j-1))/_dx-_ay*(_u(i,j)-_u(i-1,j))/_dy+_r*(pow(_u(i,j),2)-pow(_u(i,j),3));
		}
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
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _u(20,20);
			return;
		}
		case _eval_out_exp_2: {
			_out = _u(1,1);
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
	int _d2;
	int _rg_d1;
	int _rg_d2;
	int i;
	int j;
	SD_cleanJacMatrices(dvdx);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1,1);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1,1);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1,1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1-1,1);
			col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
			_apply_usage_eq_2(_d1,1);
			aux = 0;
			dvdx->df_dx[1]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(1,_d2);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(1,_d2);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 19)) {
			x_ind = _idx_u(1,_d2-1);
			col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
			_apply_usage_eq_3(1,_d2);
			aux = 0;
			dvdx->df_dx[2]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 361; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(_d1,_d2);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			_apply_usage_eq_4(_d1,_d2);
			aux = 0;
			dvdx->df_dx[3]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 20) && (1 <= _d2-1 && _d2-1 <= 19)) {
			x_ind = _idx_u(_d1,_d2-1);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			_apply_usage_eq_4(_d1,_d2);
			aux = 0;
			dvdx->df_dx[3]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(_d1-1,_d2);
			col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
			_apply_usage_eq_4(_d1,_d2);
			aux = 0;
			dvdx->df_dx[3]->value[c_row][col] +=  aux;
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
	for (row = 0; row < 19; row++) {
	  for (col = 0; col < dvdx->df_dx[1]->size[row]; col++) {
	    row_t = dvdx->df_dx[1]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[1]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 19; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 3
	for (row = 0; row < 361; row++) {
	  for (col = 0; col < dvdx->df_dx[3]->size[row]; col++) {
	    row_t = dvdx->df_dx[3]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[3]->value[row][col]);
	  }
	}
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(400,0,0,0,0,4,0,"advection2D");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(400*sizeof(int));
	int* outputs = (int*) malloc(2*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _d2;
	int _rg_d1;
	int _rg_d2;
	int i;
	int j;
	_ax = 1;
	_ay = 1;
	_r = 1;
	_dx = 1;
	_dy = 1;
	_init_u(1,1) = 1;
	modelData->nSD[_idx_u(1,1)]++;
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nSD[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1-1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2-1)]++;
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nSD[_idx_u(_d1-1,_d2)]++;
		}
	}
	modelData->nDS[_idx_u(1,1)]++;
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
		}
	}
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 20)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 19)) {
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
		}
	}
	for(row = 1; row <= 361; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (2 <= _d2 && _d2 <= 20)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 20) && (1 <= _d2-1 && _d2-1 <= 19)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (2 <= _d2 && _d2 <= 20)) {
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
		}
	}
	CLC_allocDataMatrix(modelData);
	cleanVector(states, 0, 400);
	modelData->SD[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(1,1);
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->SD[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,1)][states[_idx_u(_d1-1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->SD[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->SD[_idx_u(1,_d1-1)][states[_idx_u(1,_d1-1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2-1)][states[_idx_u(_d1,_d2-1)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->SD[_idx_u(_d1-1,_d2)][states[_idx_u(_d1-1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	cleanVector(states, 0, 400);
	modelData->DS[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(1,1);
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1-1,1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1-1);
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2-1);
		}
	}
	for(_d1 = 2; _d1<=20; _d1+=1) {
		for(_d2 = 2; _d2<=20; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1-1,_d2);
		}
	}
	cleanVector(states, 0, 400);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1,1);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 400);
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_2_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1,1);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1-1,1);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 400);
	for(row = 1; row <= 19; row++) {
		c_row = _c_index(row);
		_get_eq_3_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(1,_d2);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 19)) {
			x_ind = _idx_u(1,_d2-1);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 400);
	for(row = 1; row <= 361; row++) {
		c_row = _c_index(row);
		_get_eq_4_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((2 <= _d1 && _d1 <= 20) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(_d1,_d2);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 20) && (1 <= _d2-1 && _d2-1 <= 19)) {
			x_ind = _idx_u(_d1,_d2-1);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 19) && (2 <= _d2 && _d2 <= 20)) {
			x_ind = _idx_u(_d1-1,_d2);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 400);
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->output = SD_Output("advection2D",2,0,400,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(1,1)]++;
	modelOutput->nSO[_idx_u(20,20)]++;
	SD_allocOutputMatrix(modelOutput, 400, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[20,20]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "u[1,1]");
	cleanVector(outputs, 0, 2);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_u(1,1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(20,20);
	cleanVector(states, 0, 400);
	modelOutput->SO[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_u(20,20)][states[_idx_u(20,20)]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(states);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

