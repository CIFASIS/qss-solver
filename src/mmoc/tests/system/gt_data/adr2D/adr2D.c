#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "adr2D.h"
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
	_der_u(1,1) = -_a*(_u(1,1)-1)/_dx-_a*(_u(1,1)-1)/_dx+_d*(_u(2,1)-2*_u(1,1)+1)/pow(_dx,2)+_d*(_u(1,2)-2*_u(1,1)+1)/pow(_dx,2)+_r*(pow(_u(1,1),2)-pow(_u(1,1),3));
	_der_u(10,10) = -_a*(_u(10,10)-_u(9,10))/_dx-_a*(_u(10,10)-_u(10,9))/_dx+_d*(2*_u(9,10)-2*_u(10,10))/pow(_dx,2)+_d*(2*_u(10,9)-2*_u(10,10))/pow(_dx,2)+_r*(pow(_u(10,10),2)-pow(_u(10,10),3));
	_der_u(1,10) = -_a*(_u(1,10)-1)/_dx-_a*(_u(1,10)-_u(1,9))/_dx+_d*(_u(2,10)-2*_u(1,10)+1)/pow(_dx,2)+_d*(2*_u(1,9)-2*_u(1,10))/pow(_dx,2)+_r*(pow(_u(1,10),2)-pow(_u(1,10),3));
	_der_u(10,1) = -_a*(_u(10,1)-_u(9,1))/_dx-_a*(_u(10,1)-1)/_dx+_d*(2*_u(9,1)-2*_u(10,1))/pow(_dx,2)+_d*(_u(10,2)-2*_u(10,1)+1)/pow(_dx,2)+_r*(pow(_u(10,1),2)-pow(_u(10,1),3));
	for(i = 2; i<=9; i+=1) {
		for(j = 2; j<=9; j+=1) {
			_der_u(i,j) = -_a*(_u(i,j)-_u(i-1,j))/_dx-_a*(_u(i,j)-_u(i,j-1))/_dx+_d*(_u(i+1,j)-2*_u(i,j)+_u(i-1,j))/pow(_dx,2)+_d*(_u(i,j+1)-2*_u(i,j)+_u(i,j-1))/pow(_dx,2)+_r*(pow(_u(i,j),2)-pow(_u(i,j),3));
		}
	}
	for(j = 2; j<=9; j+=1) {
		_der_u(1,j) = -_a*(_u(1,j)-1)/_dx-_a*(_u(1,j)-_u(1,j-1))/_dx+_d*(_u(2,j)-2*_u(1,j)+1)/pow(_dx,2)+_d*(_u(1,j+1)-2*_u(1,j)+_u(1,j-1))/pow(_dx,2)+_r*(pow(_u(1,j),2)-pow(_u(1,j),3));
	}
	for(j = 2; j<=9; j+=1) {
		_der_u(10,j) = -_a*(_u(10,j)-_u(9,j))/_dx-_a*(_u(10,j)-_u(10,j-1))/_dx+_d*(2*_u(9,j)-2*_u(1,j))/pow(_dx,2)+_d*(_u(10,j+1)-2*_u(10,j)+_u(10,j-1))/pow(_dx,2)+_r*(pow(_u(10,j),2)-pow(_u(10,j),3));
	}
	for(i = 2; i<=9; i+=1) {
		_der_u(i,1) = -_a*(_u(i,1)-_u(i-1,1))/_dx-_a*(_u(i,1)-1)/_dx+_d*(_u(i+1,1)-2*_u(i,1)+_u(i-1,1))/pow(_dx,2)+_d*(_u(i,2)-2*_u(i,1)+1)/pow(_dx,2)+_r*(pow(_u(i,1),2)-pow(_u(i,1),3));
	}
	for(i = 2; i<=9; i+=1) {
		_der_u(i,10) = -_a*(_u(i,10)-_u(i-1,10))/_dx-_a*(_u(i,10)-_u(i,9))/_dx+_d*(_u(i+1,10)-2*_u(i,10)+_u(i-1,10))/pow(_dx,2)+_d*(2*_u(i,9)-2*_u(i,10))/pow(_dx,2)+_r*(pow(_u(i,10),2)-pow(_u(i,10),3));
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
			_out = _u(2,2);
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
		x_ind = _idx_u(1,2);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_u(2,1);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1+1,1);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1,1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1,1);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1,1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= 2 && 2 <= 2)) {
			x_ind = _idx_u(_d1,2);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1,1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1-1,1);
			col = pos(dvdx->df_dx[7]->index[c_row], dvdx->df_dx[7]->size[c_row], x_ind);
			_apply_usage_eq_8(_d1,1);
			aux = 0;
			dvdx->df_dx[7]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(1,_d2);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(1,_d2);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
		if((1 <= 1 && 1 <= 1) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(1,_d2+1);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(1,_d2);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(1,_d2-1);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(1,_d2);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
		if((2 <= 2 && 2 <= 2) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(2,_d2);
			col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
			_apply_usage_eq_6(1,_d2);
			aux = 0;
			dvdx->df_dx[5]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 64; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1+1,_d2);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1,_d2);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1,_d2);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1,_d2);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(_d1,_d2+1);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1,_d2);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(_d1,_d2-1);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1,_d2);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1-1,_d2);
			col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
			_apply_usage_eq_5(_d1,_d2);
			aux = 0;
			dvdx->df_dx[4]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(10,1);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
		x_ind = _idx_u(10,2);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
		x_ind = _idx_u(9,1);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(1,_d2);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(10,_d2);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((10 <= 10 && 10 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(10,_d2);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(10,_d2);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((10 <= 10 && 10 <= 10) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(10,_d2+1);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(10,_d2);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((10 <= 10 && 10 <= 10) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(10,_d2-1);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(10,_d2);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
		if((9 <= 9 && 9 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(9,_d2);
			col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
			_apply_usage_eq_7(10,_d2);
			aux = 0;
			dvdx->df_dx[6]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(1,10);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_u(1,9);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_u(2,10);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1+1,10);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1,10);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1,10);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1,10);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((2 <= _d1 && _d1 <= 9) && (9 <= 9 && 9 <= 9)) {
			x_ind = _idx_u(_d1,9);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1,10);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1-1,10);
			col = pos(dvdx->df_dx[8]->index[c_row], dvdx->df_dx[8]->size[c_row], x_ind);
			_apply_usage_eq_9(_d1,10);
			aux = 0;
			dvdx->df_dx[8]->value[c_row][col] +=  aux;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_u(10,10);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_u(10,9);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_u(9,10);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	// Assign Jacobian Matrix values for equation: 0
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[0]->size[row]; col++) {
	    row_t = dvdx->df_dx[0]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[0]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 7
	for (row = 0; row < 8; row++) {
	  for (col = 0; col < dvdx->df_dx[7]->size[row]; col++) {
	    row_t = dvdx->df_dx[7]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[7]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 5
	for (row = 0; row < 8; row++) {
	  for (col = 0; col < dvdx->df_dx[5]->size[row]; col++) {
	    row_t = dvdx->df_dx[5]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[5]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 4
	for (row = 0; row < 64; row++) {
	  for (col = 0; col < dvdx->df_dx[4]->size[row]; col++) {
	    row_t = dvdx->df_dx[4]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[4]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 3
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[3]->size[row]; col++) {
	    row_t = dvdx->df_dx[3]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[3]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 6
	for (row = 0; row < 8; row++) {
	  for (col = 0; col < dvdx->df_dx[6]->size[row]; col++) {
	    row_t = dvdx->df_dx[6]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[6]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 2
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[2]->size[row]; col++) {
	    row_t = dvdx->df_dx[2]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[2]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 8
	for (row = 0; row < 8; row++) {
	  for (col = 0; col < dvdx->df_dx[8]->size[row]; col++) {
	    row_t = dvdx->df_dx[8]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[8]->value[row][col]);
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

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(100,0,0,0,0,9,0,"adr2D");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(100*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	int _d1;
	int _d2;
	int _rg_d1;
	int _rg_d2;
	int i;
	int j;
	_a = 1;
	_d = 1.000000e-02;
	_dx = 10/(double)10;
	_r = 1000;
	modelData->nSD[_idx_u(1,1)]++;
	modelData->nSD[_idx_u(1,10)]++;
	modelData->nSD[_idx_u(1,2)]++;
	modelData->nSD[_idx_u(1,9)]++;
	modelData->nSD[_idx_u(10,1)]++;
	modelData->nSD[_idx_u(10,10)]++;
	modelData->nSD[_idx_u(10,2)]++;
	modelData->nSD[_idx_u(10,9)]++;
	modelData->nSD[_idx_u(2,1)]++;
	modelData->nSD[_idx_u(2,10)]++;
	modelData->nSD[_idx_u(9,1)]++;
	modelData->nSD[_idx_u(9,10)]++;
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nSD[_idx_u(_d1+1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2+1)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2-1)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nSD[_idx_u(_d1-1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1+1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1-1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(2,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(10,_d1+1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(10,_d1-1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(9,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1,2)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,10)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1,10)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1,9)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,10)]++;
	}
	modelData->nDS[_idx_u(1,1)]++;
	modelData->nDS[_idx_u(1,10)]++;
	modelData->nDS[_idx_u(1,1)]++;
	modelData->nDS[_idx_u(1,10)]++;
	modelData->nDS[_idx_u(10,1)]++;
	modelData->nDS[_idx_u(10,10)]++;
	modelData->nDS[_idx_u(10,1)]++;
	modelData->nDS[_idx_u(10,10)]++;
	modelData->nDS[_idx_u(1,1)]++;
	modelData->nDS[_idx_u(1,10)]++;
	modelData->nDS[_idx_u(10,1)]++;
	modelData->nDS[_idx_u(10,10)]++;
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2)]++;
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(10,_d1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,10)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,10)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,10)]++;
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->nDS[_idx_u(_d1,10)]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= 2 && 2 <= 2)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (1 <= 1 && 1 <= 1)) {
			modelData->jac_matrices->df_dx[7]->size[c_row]++;
		}
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
		if((1 <= 1 && 1 <= 1) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 8)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
		if((2 <= 2 && 2 <= 2) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
		}
	}
	for(row = 1; row <= 64; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= _d2-1 && _d2-1 <= 8)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((10 <= 10 && 10 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((10 <= 10 && 10 <= 10) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((10 <= 10 && 10 <= 10) && (1 <= _d2-1 && _d2-1 <= 8)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
		if((9 <= 9 && 9 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (10 <= 10 && 10 <= 10)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (10 <= 10 && 10 <= 10)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((2 <= _d1 && _d1 <= 9) && (9 <= 9 && 9 <= 9)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (10 <= 10 && 10 <= 10)) {
			modelData->jac_matrices->df_dx[8]->size[c_row]++;
		}
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	CLC_allocDataMatrix(modelData);
	cleanVector(states, 0, 100);
	modelData->SD[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(1,1);
	modelData->SD[_idx_u(1,10)][states[_idx_u(1,10)]++] = _idx_u(1,10);
	modelData->SD[_idx_u(1,2)][states[_idx_u(1,2)]++] = _idx_u(1,1);
	modelData->SD[_idx_u(1,9)][states[_idx_u(1,9)]++] = _idx_u(1,10);
	modelData->SD[_idx_u(10,1)][states[_idx_u(10,1)]++] = _idx_u(10,1);
	modelData->SD[_idx_u(10,10)][states[_idx_u(10,10)]++] = _idx_u(10,10);
	modelData->SD[_idx_u(10,2)][states[_idx_u(10,2)]++] = _idx_u(10,1);
	modelData->SD[_idx_u(10,9)][states[_idx_u(10,9)]++] = _idx_u(10,10);
	modelData->SD[_idx_u(2,1)][states[_idx_u(2,1)]++] = _idx_u(1,1);
	modelData->SD[_idx_u(2,10)][states[_idx_u(2,10)]++] = _idx_u(1,10);
	modelData->SD[_idx_u(9,1)][states[_idx_u(9,1)]++] = _idx_u(10,1);
	modelData->SD[_idx_u(9,10)][states[_idx_u(9,10)]++] = _idx_u(10,10);
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->SD[_idx_u(_d1+1,_d2)][states[_idx_u(_d1+1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2+1)][states[_idx_u(_d1,_d2+1)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2-1)][states[_idx_u(_d1,_d2-1)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->SD[_idx_u(_d1-1,_d2)][states[_idx_u(_d1-1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(1,_d1+1)][states[_idx_u(1,_d1+1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(1,_d1-1)][states[_idx_u(1,_d1-1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(2,_d1)][states[_idx_u(2,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(10,_d1+1)][states[_idx_u(10,_d1+1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(10,_d1-1)][states[_idx_u(10,_d1-1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(9,_d1)][states[_idx_u(9,_d1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,1)][states[_idx_u(_d1+1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1,2)][states[_idx_u(_d1,2)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,1)][states[_idx_u(_d1-1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,10)][states[_idx_u(_d1+1,10)]++] = _idx_u(_d1,10);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1,10)][states[_idx_u(_d1,10)]++] = _idx_u(_d1,10);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1,9)][states[_idx_u(_d1,9)]++] = _idx_u(_d1,10);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,10)][states[_idx_u(_d1-1,10)]++] = _idx_u(_d1,10);
	}
	cleanVector(states, 0, 100);
	modelData->DS[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(1,1);
	modelData->DS[_idx_u(1,10)][states[_idx_u(1,10)]++] = _idx_u(1,10);
	modelData->DS[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(1,2);
	modelData->DS[_idx_u(1,10)][states[_idx_u(1,10)]++] = _idx_u(1,9);
	modelData->DS[_idx_u(10,1)][states[_idx_u(10,1)]++] = _idx_u(10,1);
	modelData->DS[_idx_u(10,10)][states[_idx_u(10,10)]++] = _idx_u(10,10);
	modelData->DS[_idx_u(10,1)][states[_idx_u(10,1)]++] = _idx_u(10,2);
	modelData->DS[_idx_u(10,10)][states[_idx_u(10,10)]++] = _idx_u(10,9);
	modelData->DS[_idx_u(1,1)][states[_idx_u(1,1)]++] = _idx_u(2,1);
	modelData->DS[_idx_u(1,10)][states[_idx_u(1,10)]++] = _idx_u(2,10);
	modelData->DS[_idx_u(10,1)][states[_idx_u(10,1)]++] = _idx_u(9,1);
	modelData->DS[_idx_u(10,10)][states[_idx_u(10,10)]++] = _idx_u(9,10);
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1+1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2+1);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1,_d2-1);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		for(_d2 = 2; _d2<=9; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2)][states[_idx_u(_d1,_d2)]++] = _idx_u(_d1-1,_d2);
		}
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1+1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(1,_d1-1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(1,_d1)][states[_idx_u(1,_d1)]++] = _idx_u(2,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(1,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(10,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(10,_d1+1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(10,_d1-1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(10,_d1)][states[_idx_u(10,_d1)]++] = _idx_u(9,_d1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1+1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1,2);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,1)][states[_idx_u(_d1,1)]++] = _idx_u(_d1-1,1);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,10)][states[_idx_u(_d1,10)]++] = _idx_u(_d1+1,10);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,10)][states[_idx_u(_d1,10)]++] = _idx_u(_d1,10);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,10)][states[_idx_u(_d1,10)]++] = _idx_u(_d1,9);
	}
	for(_d1 = 2; _d1<=9; _d1+=1) {
		modelData->DS[_idx_u(_d1,10)][states[_idx_u(_d1,10)]++] = _idx_u(_d1-1,10);
	}
	cleanVector(states, 0, 100);
	cleanVector(states, 0, 100);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1,1);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(1,2);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(2,1);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_8_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1+1,1);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1,1);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= 2 && 2 <= 2)) {
			x_ind = _idx_u(_d1,2);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (1 <= 1 && 1 <= 1)) {
			x_ind = _idx_u(_d1-1,1);
			if(in(modelData->jac_matrices->df_dx[7]->index[c_row],modelData->jac_matrices->df_dx[7]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[7]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[7]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_6_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(1,_d2);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= 1 && 1 <= 1) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(1,_d2+1);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= 1 && 1 <= 1) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(1,_d2-1);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= 2 && 2 <= 2) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(2,_d2);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 64; row++) {
		c_row = _c_index(row);
		_get_eq_5_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1+1,_d2);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1,_d2);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(_d1,_d2+1);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(_d1,_d2-1);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(_d1-1,_d2);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(10,1);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(10,2);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(9,1);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_7_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((1 <= 1 && 1 <= 1) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(1,_d2);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((10 <= 10 && 10 <= 10) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(10,_d2);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((10 <= 10 && 10 <= 10) && (3 <= _d2 + 1 && _d2 + 1 <= 10)) {
			x_ind = _idx_u(10,_d2+1);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((10 <= 10 && 10 <= 10) && (1 <= _d2-1 && _d2-1 <= 8)) {
			x_ind = _idx_u(10,_d2-1);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((9 <= 9 && 9 <= 9) && (2 <= _d2 && _d2 <= 9)) {
			x_ind = _idx_u(9,_d2);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(1,10);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(1,9);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(2,10);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 8; row++) {
		c_row = _c_index(row);
		_get_eq_9_var_idxs(row, eq_var);
		_get_u_idxs(eq_var);
		if((3 <= _d1 + 1 && _d1 + 1 <= 10) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1+1,10);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1,10);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((2 <= _d1 && _d1 <= 9) && (9 <= 9 && 9 <= 9)) {
			x_ind = _idx_u(_d1,9);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
		if((1 <= _d1-1 && _d1-1 <= 8) && (10 <= 10 && 10 <= 10)) {
			x_ind = _idx_u(_d1-1,10);
			if(in(modelData->jac_matrices->df_dx[8]->index[c_row],modelData->jac_matrices->df_dx[8]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[8]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[8]->index[c_row][states[c_row]++] = x_ind;
			}
		}
	}
	cleanVector(states, 0, 100);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_u(10,10);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(10,9);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_u(9,10);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->output = SD_Output("adr2D",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_u(2,2)]++;
	SD_allocOutputMatrix(modelOutput, 100, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[2,2]");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(2,2);
	cleanVector(states, 0, 100);
	modelOutput->SO[_idx_u(2,2)][states[_idx_u(2,2)]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(states);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

