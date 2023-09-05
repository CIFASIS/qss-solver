#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mliqss_TYSON.h"
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
	settings->hybrid = FALSE;
	settings->method = 15;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	switch(idx) {
		case _eval_C2(0): {
			_der_C2(0) = _k6*_M(0)-_k8*_C2(0)+_k9*_CP(0);
	
	
			return;
		}
		case _eval_CP(0): {
			_der_CP(0) = -_k3*_CP(0)*_Y(0)+_k8*_C2(0)-_k9*_CP(0);
	
	
			return;
		}
		case _eval_pM(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_der_pM(0) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
	
	
			return;
		}
		case _eval_M(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_der_M(0) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
	
	
			return;
		}
		case _eval_Y(0): {
			_der_Y(0) = _k1-_k2*_Y(0)-_k3*_CP(0)*_Y(0);
	
	
			return;
		}
		case _eval_yP(0): {
			_der_yP(0) = _k6*_M(0)-_k7*_yP(0);
	
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
	switch(idx) {
		case _eval_out_exp_1: {
			_out = _C2(0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _CP(0);
			return;
		}
		case _eval_out_exp_3: {
			_out = _pM(0);
			return;
		}
		case _eval_out_exp_4: {
			_out = _M(0);
			return;
		}
		case _eval_out_exp_5: {
			_out = _Y(0);
			return;
		}
		case _eval_out_exp_6: {
			_out = _yP(0);
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
		x_ind = _idx_C2(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[0]->index[c_row], dvdx->df_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_C2(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_Y(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_C2(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_CP(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_M(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
		x_ind = _idx_pM(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_C2(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_CT(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->dg_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_CP(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_CT(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->dg_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_CT(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->dg_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_pM(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_CT(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->dg_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->dg_dx[2]->index[c_row], dvdx->dg_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_C2(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_pM(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_Y(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
		x_ind = _idx_pM(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_C2(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_pM(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_F(0) - 2;
		col_g = pos(dvdx->dg_dx[2]->index[c_row_g], dvdx->dg_dx[2]->size[c_row_g], x_ind);
		dvdx->df_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[2]->value[c_row_g][col_g];
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
		x_ind = _idx_pM(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_CP(0);
		col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[4]->value[c_row][col] +=  aux;
		x_ind = _idx_Y(0);
		col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[4]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_M(0);
		col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[5]->value[c_row][col] +=  aux;
		x_ind = _idx_yP(0);
		col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[5]->value[c_row][col] +=  aux;
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
	// Assign Jacobian Matrix values for equation: 4
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[4]->size[row]; col++) {
	    row_t = dvdx->df_dx[4]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[4]->value[row][col]);
	  }
	}
	// Assign Jacobian Matrix values for equation: 5
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[5]->size[row]; col++) {
	    row_t = dvdx->df_dx[5]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[5]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_C2(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_C2(1) = _k6*_M(0)-_k8*_C2(0)+_k9*_CP(0);
			_eval_dep_CP(1) = -_k3*_CP(0)*_Y(0)+_k8*_C2(0)-_k9*_CP(0);
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
			_eval_dep_M(1) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
			break;
		}
		case _eval_CP(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_C2(1) = _k6*_M(0)-_k8*_C2(0)+_k9*_CP(0);
			_eval_dep_CP(1) = -_k3*_CP(0)*_Y(0)+_k8*_C2(0)-_k9*_CP(0);
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
			_eval_dep_M(1) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
			_eval_dep_Y(1) = _k1-_k2*_Y(0)-_k3*_CP(0)*_Y(0);
			break;
		}
		case _eval_M(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_C2(1) = _k6*_M(0)-_k8*_C2(0)+_k9*_CP(0);
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
			_eval_dep_M(1) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
			_eval_dep_yP(1) = _k6*_M(0)-_k7*_yP(0);
			break;
		}
		case _eval_Y(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_CP(1) = -_k3*_CP(0)*_Y(0)+_k8*_C2(0)-_k9*_CP(0);
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
			_eval_dep_Y(1) = _k1-_k2*_Y(0)-_k3*_CP(0)*_Y(0);
			break;
		}
		case _eval_pM(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
			_eval_dep_M(1) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
			break;
		}
		case _eval_yP(0): {
			_eval_dep_yP(1) = _k6*_M(0)-_k7*_yP(0);
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
		case _eval_C2(0): {
			_eval_dep_C2(1) = _k6*_M(0)-_k8*_C2(0)+_k9*_CP(0);
	
	
			continue;
		}
		case _eval_CP(0): {
			_eval_dep_CP(1) = -_k3*_CP(0)*_Y(0)+_k8*_C2(0)-_k9*_CP(0);
	
	
			continue;
		}
		case _eval_pM(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_pM(1) = _k3*_CP(0)*_Y(0)+-_pM(0)*_F(0)+_k5*_M(0);
	
	
			continue;
		}
		case _eval_M(0): {
			_CT(0) = _C2(0)+_CP(0)+_pM(0)+_M(0);
	
			_F(0) = _k4prim+_k4*__math__pow((_M(0)*1.000000e+00/_CT(0)), 2);
	
			_eval_dep_M(1) = _pM(0)*_F(0)-_k5*_M(0)-_k6*_M(0);
	
	
			continue;
		}
		case _eval_Y(0): {
			_eval_dep_Y(1) = _k1-_k2*_Y(0)-_k3*_CP(0)*_Y(0);
	
	
			continue;
		}
		case _eval_yP(0): {
			_eval_dep_yP(1) = _k6*_M(0)-_k7*_yP(0);
	
	
			continue;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(6,0,0,0,4,6,4,"mliqss_TYSON");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(4*sizeof(int));
	int* states = (int*) malloc(6*sizeof(int));
	int* outputs = (int*) malloc(6*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_init_C2(0) = 0.000000e+00;
	_init_CP(0) = 7.500000e-01;
	_init_M(0) = 0.000000e+00;
	_init_Y(0) = 0.000000e+00;
	_k1 = 1.500000e-02;
	_k2 = 0.000000e+00;
	_k3 = 200;
	_k4 = 180;
	_k4prim = 1.800000e-02;
	_k5 = 0.000000e+00;
	_k6 = 1.000000e+00;
	_k7 = 6.000000e-01;
	_k8 = 1.000000e+06;
	_k9 = 1.000000e+03;
	_init_pM(0) = 2.500000e-01;
	_init_yP(0) = 0.000000e+00;
	modelData->nSD[_idx_C2(0)]++;
	modelData->nSD[_idx_C2(0)]++;
	modelData->nSD[_idx_C2(0)]++;
	modelData->nSD[_idx_C2(0)]++;
	modelData->nSD[_idx_CP(0)]++;
	modelData->nSD[_idx_CP(0)]++;
	modelData->nSD[_idx_CP(0)]++;
	modelData->nSD[_idx_CP(0)]++;
	modelData->nSD[_idx_CP(0)]++;
	modelData->nSD[_idx_M(0)]++;
	modelData->nSD[_idx_M(0)]++;
	modelData->nSD[_idx_M(0)]++;
	modelData->nSD[_idx_M(0)]++;
	modelData->nSD[_idx_Y(0)]++;
	modelData->nSD[_idx_Y(0)]++;
	modelData->nSD[_idx_Y(0)]++;
	modelData->nSD[_idx_pM(0)]++;
	modelData->nSD[_idx_pM(0)]++;
	modelData->nSD[_idx_yP(0)]++;
	modelData->nDS[_idx_C2(0)]++;
	modelData->nDS[_idx_CP(0)]++;
	modelData->nDS[_idx_pM(0)]++;
	modelData->nDS[_idx_M(0)]++;
	modelData->nDS[_idx_C2(0)]++;
	modelData->nDS[_idx_CP(0)]++;
	modelData->nDS[_idx_pM(0)]++;
	modelData->nDS[_idx_M(0)]++;
	modelData->nDS[_idx_Y(0)]++;
	modelData->nDS[_idx_C2(0)]++;
	modelData->nDS[_idx_pM(0)]++;
	modelData->nDS[_idx_M(0)]++;
	modelData->nDS[_idx_yP(0)]++;
	modelData->nDS[_idx_CP(0)]++;
	modelData->nDS[_idx_pM(0)]++;
	modelData->nDS[_idx_Y(0)]++;
	modelData->nDS[_idx_pM(0)]++;
	modelData->nDS[_idx_M(0)]++;
	modelData->nDS[_idx_yP(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
			modelData->jac_matrices->df_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
			modelData->jac_matrices->dg_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 6);
	modelData->SD[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_C2(0);
	modelData->SD[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_CP(0);
	modelData->SD[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_pM(0);
	modelData->SD[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_M(0);
	modelData->SD[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_C2(0);
	modelData->SD[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_CP(0);
	modelData->SD[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_pM(0);
	modelData->SD[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_M(0);
	modelData->SD[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_Y(0);
	modelData->SD[_idx_M(0)][states[_idx_M(0)]++] = _idx_C2(0);
	modelData->SD[_idx_M(0)][states[_idx_M(0)]++] = _idx_pM(0);
	modelData->SD[_idx_M(0)][states[_idx_M(0)]++] = _idx_M(0);
	modelData->SD[_idx_M(0)][states[_idx_M(0)]++] = _idx_yP(0);
	modelData->SD[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_CP(0);
	modelData->SD[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_pM(0);
	modelData->SD[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_Y(0);
	modelData->SD[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_pM(0);
	modelData->SD[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_M(0);
	modelData->SD[_idx_yP(0)][states[_idx_yP(0)]++] = _idx_yP(0);
	cleanVector(states, 0, 6);
	modelData->DS[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_C2(0);
	modelData->DS[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_C2(0);
	modelData->DS[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_C2(0);
	modelData->DS[_idx_M(0)][states[_idx_M(0)]++] = _idx_C2(0);
	modelData->DS[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_CP(0);
	modelData->DS[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_CP(0);
	modelData->DS[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_CP(0);
	modelData->DS[_idx_M(0)][states[_idx_M(0)]++] = _idx_CP(0);
	modelData->DS[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_CP(0);
	modelData->DS[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_M(0);
	modelData->DS[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_M(0);
	modelData->DS[_idx_M(0)][states[_idx_M(0)]++] = _idx_M(0);
	modelData->DS[_idx_yP(0)][states[_idx_yP(0)]++] = _idx_M(0);
	modelData->DS[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_Y(0);
	modelData->DS[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_Y(0);
	modelData->DS[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_Y(0);
	modelData->DS[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_pM(0);
	modelData->DS[_idx_M(0)][states[_idx_M(0)]++] = _idx_pM(0);
	modelData->DS[_idx_yP(0)][states[_idx_yP(0)]++] = _idx_yP(0);
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[0]->index[c_row],modelData->jac_matrices->df_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[0]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_Y(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 4);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->dg_dx[2]->index[c_row],modelData->jac_matrices->dg_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[2]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_Y(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_C2(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_pM(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_CP(0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_Y(0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 6);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_M(0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_yP(0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(6,0,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("mliqss_TYSON",6,0,6,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nOS[_idx_out_exp_5]++;
	modelOutput->nOS[_idx_out_exp_3]++;
	modelOutput->nOS[_idx_out_exp_6]++;
	modelOutput->nSO[_idx_C2(0)]++;
	modelOutput->nSO[_idx_CP(0)]++;
	modelOutput->nSO[_idx_M(0)]++;
	modelOutput->nSO[_idx_Y(0)]++;
	modelOutput->nSO[_idx_pM(0)]++;
	modelOutput->nSO[_idx_yP(0)]++;
	SD_allocOutputMatrix(modelOutput, 6, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "C2");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "CP");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "pM");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "M");
	sprintf(modelOutput->variable[_idx_out_exp_5].name, "Y");
	sprintf(modelOutput->variable[_idx_out_exp_6].name, "yP");
	cleanVector(outputs, 0, 6);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_C2(0);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_CP(0);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_M(0);
	modelOutput->OS[_idx_out_exp_5][outputs[_idx_out_exp_5]++] = _idx_Y(0);
	modelOutput->OS[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_pM(0);
	modelOutput->OS[_idx_out_exp_6][outputs[_idx_out_exp_6]++] = _idx_yP(0);
	cleanVector(states, 0, 6);
	modelOutput->SO[_idx_C2(0)][states[_idx_C2(0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_CP(0)][states[_idx_CP(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_M(0)][states[_idx_M(0)]++] = _idx_out_exp_4;
	modelOutput->SO[_idx_Y(0)][states[_idx_Y(0)]++] = _idx_out_exp_5;
	modelOutput->SO[_idx_pM(0)][states[_idx_pM(0)]++] = _idx_out_exp_3;
	modelOutput->SO[_idx_yP(0)][states[_idx_yP(0)]++] = _idx_out_exp_6;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(algebraics);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

