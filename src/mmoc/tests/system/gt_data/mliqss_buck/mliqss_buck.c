#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "mliqss_buck.h"
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
	settings->method = 15;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	switch(idx) {
		case _eval_dc(0): {
			_der_dc(0) = _ptedc;
			_der_dc(1) = (0)/2;
	
			return;
		}
		case _eval_uC(0): {
			_der_uC(0) = (_iL(0)-_uC(0)/_R)/_C;
			_der_uC(1) = (0)/2;
	
			return;
		}
		case _eval_iL(0): {
			_K(0) = (1-_dc(0))/_dc(0);
			_K(1) = 0;
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
			_Vd(1) = 0;
			_der_iL(0) = (-_Vd(0)-_uC(0))/_L;
			_der_iL(1) = (0)/2;
	
			return;
		}
		case _eval_meanIL(0): {
			_der_meanIL(0) = _modSEL*(_iL(0)-_lastIL)/_T;
			_der_meanIL(1) = (0)/2;
	
			return;
		}
		case _eval_meanUC(0): {
			_der_meanUC(0) = _modSEL*(_uC(0)-_lastUC)/_T;
			_der_meanUC(1) = (0)/2;
	
			return;
		}
		case _eval_avSwVoltage(0): {
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vsw(0) = _modSEL*(_iL(0)-_iD(0))*_Rs;
			_Vsw(1) = 0;
			_der_avSwVoltage(0) = _modSEL*_Vsw(0)/_T;
			_der_avSwVoltage(1) = (0)/2;
	
			return;
		}
		case _eval_avDiVoltage(0): {
			_K(0) = (1-_dc(0))/_dc(0);
			_K(1) = 0;
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
			_Vd(1) = 0;
			_der_avDiVoltage(0) = _modSEL*_Vd(0)/_T;
			_der_avDiVoltage(1) = (0)/2;
	
			return;
		}
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time*_modSEL-(_nextT);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_2: {
			_zc(0) = (_time-_lastT-_DC*_T)*_modSEL-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_3: {
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_s(0) = _diodeon*_iD(0)+(1-_diodeon)*_iD(0)*_Rd;
			_s(1) = 0;
			_zc(0) = _s(0)-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_4: {
			_zc(0) = (_uC(0)-_meanUCdisc)*(1-_modSEL)-(_sigma*_rUc);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_5: {
			_zc(0) = (_meanUCdisc-_uC(0))*(1-_modSEL)-(_sigma*_rUc);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_6: {
			_zc(0) = (_iL(0)-_meanILdisc)*(1-_modSEL)-(_sigma*_riL);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_7: {
			_zc(0) = (_meanILdisc-_iL(0))*(1-_modSEL)-(_sigma*_riL);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_8: {
			_zc(0) = (_DC-_lastDC)*(1-_modSEL)-(_deltaDC);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_9: {
			_zc(0) = (_lastDC-_DC)*(1-_modSEL)-(_deltaDC);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_10: {
			_zc(0) = _time-(_toSwNextT);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_11: {
			_zc(0) = _time-(0);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_12: {
			_zc(0) = _time-(1.100000e-01);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_13: {
			_zc(0) = _time-(1.500000e-01);
			_zc(1) = (0)/1;
	
			return;
		}
		case _eval_event_14: {
			_zc(0) = _time-(6.000000e-01);
			_zc(1) = (0)/1;
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_DC = _q_dc(0);
			if(_DC>9.900000e-01) {
		_DC = 9.900000e-01;
	}else {
		if(_DC<1.000000e-02) {
			_DC = 1.000000e-02;
		}
	}
			_lastT = _nextT;
			_nextT = _nextT+_T;
			_Rs = _ROn;
			_lastMeanILdisc = _meanILdisc;
			_meanILdisc = _q_meanIL(0)+_lastIL;
			_meanILdiff = __math__fabs(_meanILdisc-_lastMeanILdisc);
			_init_meanIL(0) = 0;
			_lastIL = _q_iL(0);
			_lastMeanUCdisc = _meanUCdisc;
			_meanUCdisc = _q_meanUC(0)+_lastUC;
			_meanUCdiff = __math__fabs(_meanUCdisc-_lastMeanUCdisc);
			_init_meanUC(0) = 0;
			_lastUC = _q_uC(0);
			_avDiVoltageDisc = _q_avDiVoltage(0);
			_init_avDiVoltage(0) = 0;
			_avSwVoltageDisc = _q_avSwVoltage(0);
			_init_avSwVoltage(0) = 0;
			if((_time-_lastTaverage)>1.500000e+00*_T) {
		_Kadj = (-_avSwVoltageDisc/_avDiVoltageDisc)/_K(0);
	}
			_toAvEnableI = 0;
			_toAvEnableU = 0;
			_toAverageEnabled = 0;
			if(_meanILdiff<_alpha*_riL) {
		_toAvEnableI = 1;
	}
			if(_meanUCdiff<_alpha*_rUc) {
		_toAvEnableU = 1;
	}
			if(_toAvEnableU&&_toAvEnableI) {
		_toAverageEnabled = 1;
	}
			if(_toAverageEnabled) {
		_modSEL = 0;
		_Rs = _ROff;
		_init_iL(0) = _meanILdisc;
		_init_uC(0) = _meanUCdisc;
		_lastDC = _DC;
	}
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
		case _eval_event_4: {
			if(_time<16) {
		_toSwEnabled = 1;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_5: {
			if(_time<16) {
		_toSwEnabled = 2;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_6: {
			if(_time<16) {
		_toSwEnabled = 3;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_7: {
			if(_time<16) {
		_toSwEnabled = 4;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_8: {
			if(_time<16) {
		_toSwEnabled = 5;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_9: {
			if(_time<16) {
		_toSwEnabled = 6;
		_toSwNextT = _T*(1+floor(_time/_T));
	}
					return;
		}
		case _eval_event_10: {
			_modSEL = 1;
			_nextT = _time;
			_init_iL(0) = (_lastIL-_meanILdisc)+_init_iL(0);
			_init_uC(0) = _lastUC-_meanUCdisc+_init_uC(0);
			_lastTaverage = _time;
			_toSwEnabled = 0;
					return;
		}
		case _eval_event_11: {
			_ptedc = 5.000000e-01;
			_rUc = 6.000000e-01;
			_riL = 3.500000e+00;
					return;
		}
		case _eval_event_12: {
			_ptedc = 0;
					return;
		}
		case _eval_event_13: {
			_init_dc(0) = 4.000000e-01;
			_rUc = 6.000000e-01;
			_riL = 3.500000e+00;
					return;
		}
		case _eval_event_14: {
			_R = 2;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
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
			_out = _modSEL;
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
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
		x_ind = _idx_uC(0);
		col = pos(dvdx->df_dx[1]->index[c_row], dvdx->df_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_dc(0);
		col = pos(dvdx->dg_dx[0]->index[c_row], dvdx->dg_dx[0]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[0]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_iL(0);
		col = pos(dvdx->dg_dx[1]->index[c_row], dvdx->dg_dx[1]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[1]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_dc(0);
		col = pos(dvdx->dg_dx[3]->index[c_row], dvdx->dg_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_K(0) - 0;
		col_g = pos(dvdx->dg_dx[0]->index[c_row_g], dvdx->dg_dx[0]->size[c_row_g], x_ind);
		dvdx->dg_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[0]->value[c_row_g][col_g];
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->dg_dx[3]->index[c_row], dvdx->dg_dx[3]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 1;
		col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
		dvdx->dg_dx[3]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_dc(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_Vd(0) - 3;
		col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		c_row_g = _idx_Vd(0) - 3;
		col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
		dvdx->df_dx[2]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
		x_ind = _idx_uC(0);
		col = pos(dvdx->df_dx[2]->index[c_row], dvdx->df_dx[2]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[2]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[3]->index[c_row], dvdx->df_dx[3]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[3]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		x_ind = _idx_uC(0);
		col = pos(dvdx->df_dx[4]->index[c_row], dvdx->df_dx[4]->size[c_row], x_ind);
		aux = 0;
		dvdx->df_dx[4]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->dg_dx[4]->index[c_row], dvdx->dg_dx[4]->size[c_row], x_ind);
		c_row_g = _idx_iD(0) - 1;
		col_g = pos(dvdx->dg_dx[1]->index[c_row_g], dvdx->dg_dx[1]->size[c_row_g], x_ind);
		dvdx->dg_dx[4]->value[c_row][col] += aux * dvdx->dg_dx[1]->value[c_row_g][col_g];
		x_ind = _idx_iL(0);
		col = pos(dvdx->dg_dx[4]->index[c_row], dvdx->dg_dx[4]->size[c_row], x_ind);
		aux = 0;
		dvdx->dg_dx[4]->value[c_row][col] +=  aux;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
		c_row_g = _idx_Vsw(0) - 4;
		col_g = pos(dvdx->dg_dx[4]->index[c_row_g], dvdx->dg_dx[4]->size[c_row_g], x_ind);
		dvdx->df_dx[5]->value[c_row][col] += aux * dvdx->dg_dx[4]->value[c_row_g][col_g];
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[5]->index[c_row], dvdx->df_dx[5]->size[c_row], x_ind);
		c_row_g = _idx_Vsw(0) - 4;
		col_g = pos(dvdx->dg_dx[4]->index[c_row_g], dvdx->dg_dx[4]->size[c_row_g], x_ind);
		dvdx->df_dx[5]->value[c_row][col] += aux * dvdx->dg_dx[4]->value[c_row_g][col_g];
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
		aux = 0;
		x_ind = _idx_dc(0);
		col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
		c_row_g = _idx_Vd(0) - 3;
		col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
		dvdx->df_dx[6]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
		x_ind = _idx_iL(0);
		col = pos(dvdx->df_dx[6]->index[c_row], dvdx->df_dx[6]->size[c_row], x_ind);
		c_row_g = _idx_Vd(0) - 3;
		col_g = pos(dvdx->dg_dx[3]->index[c_row_g], dvdx->dg_dx[3]->size[c_row_g], x_ind);
		dvdx->df_dx[6]->value[c_row][col] += aux * dvdx->dg_dx[3]->value[c_row_g][col_g];
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
	// Assign Jacobian Matrix values for equation: 6
	for (row = 0; row < 1; row++) {
	  for (col = 0; col < dvdx->df_dx[6]->size[row]; col++) {
	    row_t = dvdx->df_dx[6]->index[row][col];
	    _assign_jac(row_t, dvdx->df_dx[6]->value[row][col]);
	  }
	}
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	switch(idx) {
		case _eval_dc(0): {
			_K(0) = (1-_dc(0))/_dc(0);
			_K(1) = 0;
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
			_Vd(1) = 0;
			_eval_dep_iL(1) = (-_Vd(0)-_uC(0))/_L;
			_eval_dep_iL(2) = (0)/2;
			_eval_dep_avDiVoltage(1) = _modSEL*_Vd(0)/_T;
			_eval_dep_avDiVoltage(2) = (0)/2;
			break;
		}
		case _eval_iL(0): {
			_K(0) = (1-_dc(0))/_dc(0);
			_K(1) = 0;
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
			_Vd(1) = 0;
			_Vsw(0) = _modSEL*(_iL(0)-_iD(0))*_Rs;
			_Vsw(1) = 0;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
			_eval_dep_iL(1) = (-_Vd(0)-_uC(0))/_L;
			_eval_dep_iL(2) = (0)/2;
			_eval_dep_meanIL(1) = _modSEL*(_iL(0)-_lastIL)/_T;
			_eval_dep_meanIL(2) = (0)/2;
			_eval_dep_avSwVoltage(1) = _modSEL*_Vsw(0)/_T;
			_eval_dep_avSwVoltage(2) = (0)/2;
			_eval_dep_avDiVoltage(1) = _modSEL*_Vd(0)/_T;
			_eval_dep_avDiVoltage(2) = (0)/2;
			break;
		}
		case _eval_uC(0): {
			_K(0) = (1-_dc(0))/_dc(0);
			_K(1) = 0;
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
			_iD(1) = 0;
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
			_Vd(1) = 0;
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
			_eval_dep_uC(2) = (0)/2;
			_eval_dep_iL(1) = (-_Vd(0)-_uC(0))/_L;
			_eval_dep_iL(2) = (0)/2;
			_eval_dep_meanUC(1) = _modSEL*(_uC(0)-_lastUC)/_T;
			_eval_dep_meanUC(2) = (0)/2;
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
		case _eval_dc(0): {
			_eval_dep_dc(1) = _ptedc;
	
	
			continue;
		}
		case _eval_uC(0): {
			_eval_dep_uC(1) = (_iL(0)-_uC(0)/_R)/_C;
	
	
			continue;
		}
		case _eval_iL(0): {
			_K(0) = (1-_dc(0))/_dc(0);
	
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
	
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
	
			_eval_dep_iL(1) = (-_Vd(0)-_uC(0))/_L;
	
	
			continue;
		}
		case _eval_meanIL(0): {
			_eval_dep_meanIL(1) = _modSEL*(_iL(0)-_lastIL)/_T;
	
	
			continue;
		}
		case _eval_meanUC(0): {
			_eval_dep_meanUC(1) = _modSEL*(_uC(0)-_lastUC)/_T;
	
	
			continue;
		}
		case _eval_avSwVoltage(0): {
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
	
			_Vsw(0) = _modSEL*(_iL(0)-_iD(0))*_Rs;
	
			_eval_dep_avSwVoltage(1) = _modSEL*_Vsw(0)/_T;
	
	
			continue;
		}
		case _eval_avDiVoltage(0): {
			_K(0) = (1-_dc(0))/_dc(0);
	
			_iD(0) = _modSEL*(_iL(0)*_Rs-_U)/(_Rs+_Rd);
	
			_Vd(0) = _iD(0)*_Rd*_modSEL-_U/(1+_K(0)*_Kadj)*(1-_modSEL);
	
			_eval_dep_avDiVoltage(1) = _modSEL*_Vd(0)/_T;
	
	
			continue;
		}
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(7,29,14,0,5,7,5,"mliqss_buck");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(5*sizeof(int));
	int* states = (int*) malloc(7*sizeof(int));
	int* discretes = (int*) malloc(29*sizeof(int));
	int* events = (int*) malloc(14*sizeof(int));
	int* outputs = (int*) malloc(3*sizeof(int));
	int row, eq_var, c_row;
	int x_ind;
	_C = 1.000000e-04;
	_DC = 5.000000e-01;
	_Kadj = 1;
	_L = 1.000000e-04;
	_R = 10;
	_ROff = 1.000000e+05;
	_ROn = 1.000000e-05;
	_Rd = 1.000000e+05;
	_Rs = 1.000000e-05;
	_T = 1.000000e-04;
	_U = 24;
	_alpha = 1.000000e-02;
	_init_avDiVoltage(0) = 0;
	_avDiVoltageDisc = 0;
	_init_avSwVoltage(0) = 0;
	_avSwVoltageDisc = 0;
	_init_dc(0) = 6.500000e-01;
	_deltaDC = 1.000000e-02;
	_dutyCycle = 5.000000e-01;
	_lastMeanILdisc = 1.000000e+09;
	_lastMeanUCdisc = 1.000000e+09;
	_lastTaverage = 0;
	_init_meanIL(0) = 0;
	_meanILdisc = 0;
	_init_meanUC(0) = 0;
	_meanUCdisc = 0;
	_modSEL = 1;
	_nextT = _T;
	_ptedc = 5.000000e-01;
	_rUc = 6.000000e-01;
	_riL = 3.500000e+00;
	_sigma = 1.000000e-01;
	_toAvEnableI = 0;
	_toAvEnableU = 0;
	_toAverageEnabled = 0;
	_toSwEnabled = 0;
	_toSwNextT = 1.000000e+09;
	modelData->nSD[_idx_dc(0)]++;
	modelData->nSD[_idx_dc(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_iL(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nSD[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_avDiVoltage(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_meanIL(0)]++;
	modelData->nDS[_idx_avSwVoltage(0)]++;
	modelData->nDS[_idx_avDiVoltage(0)]++;
	modelData->nDS[_idx_uC(0)]++;
	modelData->nDS[_idx_iL(0)]++;
	modelData->nDS[_idx_meanUC(0)]++;
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
			modelData->jac_matrices->df_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[0]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[1]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[3]->size[c_row]++;
			modelData->jac_matrices->dg_dx[3]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
			modelData->jac_matrices->df_dx[2]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[3]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[4]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->dg_dx[4]->size[c_row]++;
			modelData->jac_matrices->dg_dx[4]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
			modelData->jac_matrices->df_dx[5]->size[c_row]++;
	}
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
			modelData->jac_matrices->df_dx[6]->size[c_row]++;
	}
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_iL(0)]++;
	modelData->nSZ[_idx_uC(0)]++;
	modelData->nSZ[_idx_uC(0)]++;
	modelData->nZS[_idx_event_3]++;
	modelData->nZS[_idx_event_6]++;
	modelData->nZS[_idx_event_7]++;
	modelData->nZS[_idx_event_4]++;
	modelData->nZS[_idx_event_5]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_4]++;
	modelData->nHZ[_idx_event_5]++;
	modelData->nHZ[_idx_event_6]++;
	modelData->nHZ[_idx_event_7]++;
	modelData->nHZ[_idx_event_8]++;
	modelData->nHZ[_idx_event_9]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_2]++;
	modelData->nHZ[_idx_event_3]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_11]++;
	modelData->nHZ[_idx_event_13]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_11]++;
	modelData->nHZ[_idx_event_13]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_11]++;
	modelData->nHZ[_idx_event_13]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_11]++;
	modelData->nHZ[_idx_event_13]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHZ[_idx_event_1]++;
	modelData->nHZ[_idx_event_10]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_10]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_10]++;
	modelData->nHD[_idx_event_11]++;
	modelData->nHD[_idx_event_12]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_2]++;
	modelData->nHD[_idx_event_3]++;
	modelData->nHD[_idx_event_10]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_10]++;
	modelData->nHD[_idx_event_1]++;
	modelData->nHD[_idx_event_10]++;
	modelData->nHD[_idx_event_14]++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_14].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_2].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_3].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_10].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_10].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_10].nLHSDsc++;
	modelData->event[_idx_event_11].nLHSDsc++;
	modelData->event[_idx_event_12].nLHSDsc++;
	modelData->event[_idx_event_11].nLHSDsc++;
	modelData->event[_idx_event_13].nLHSDsc++;
	modelData->event[_idx_event_11].nLHSDsc++;
	modelData->event[_idx_event_13].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSDsc++;
	modelData->event[_idx_event_4].nLHSDsc++;
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_6].nLHSDsc++;
	modelData->event[_idx_event_7].nLHSDsc++;
	modelData->event[_idx_event_8].nLHSDsc++;
	modelData->event[_idx_event_9].nLHSDsc++;
	modelData->event[_idx_event_10].nLHSDsc++;
	modelData->event[_idx_event_4].nLHSDsc++;
	modelData->event[_idx_event_5].nLHSDsc++;
	modelData->event[_idx_event_6].nLHSDsc++;
	modelData->event[_idx_event_7].nLHSDsc++;
	modelData->event[_idx_event_8].nLHSDsc++;
	modelData->event[_idx_event_9].nLHSDsc++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_13].nLHSSt++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_10].nLHSSt++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_1].nLHSSt++;
	modelData->event[_idx_event_10].nLHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_10].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_1].nRHSSt++;
	modelData->event[_idx_event_10].nRHSSt++;
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 7);
	modelData->SD[_idx_dc(0)][states[_idx_dc(0)]++] = _idx_iL(0);
	modelData->SD[_idx_dc(0)][states[_idx_dc(0)]++] = _idx_avDiVoltage(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_meanIL(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_avSwVoltage(0);
	modelData->SD[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_avDiVoltage(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->SD[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_meanUC(0);
	cleanVector(states, 0, 7);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_dc(0);
	modelData->DS[_idx_avDiVoltage(0)][states[_idx_avDiVoltage(0)]++] = _idx_dc(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_iL(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_meanIL(0)][states[_idx_meanIL(0)]++] = _idx_iL(0);
	modelData->DS[_idx_avSwVoltage(0)][states[_idx_avSwVoltage(0)]++] = _idx_iL(0);
	modelData->DS[_idx_avDiVoltage(0)][states[_idx_avDiVoltage(0)]++] = _idx_iL(0);
	modelData->DS[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_uC(0);
	modelData->DS[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_uC(0);
	modelData->DS[_idx_meanUC(0)][states[_idx_meanUC(0)]++] = _idx_uC(0);
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
	}
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[1]->index[c_row],modelData->jac_matrices->df_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[1]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_dc(0);
			if(in(modelData->jac_matrices->dg_dx[0]->index[c_row],modelData->jac_matrices->dg_dx[0]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[0]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[0]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->dg_dx[1]->index[c_row],modelData->jac_matrices->dg_dx[1]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[1]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[1]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_dc(0);
			if(in(modelData->jac_matrices->dg_dx[3]->index[c_row],modelData->jac_matrices->dg_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[3]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->dg_dx[3]->index[c_row],modelData->jac_matrices->dg_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[3]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_dc(0);
			if(in(modelData->jac_matrices->df_dx[2]->index[c_row],modelData->jac_matrices->df_dx[2]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[2]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[2]->index[c_row][states[c_row]++] = x_ind;
			}
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
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[3]->index[c_row],modelData->jac_matrices->df_dx[3]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[3]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[3]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_uC(0);
			if(in(modelData->jac_matrices->df_dx[4]->index[c_row],modelData->jac_matrices->df_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[4]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(algebraics, 0, 5);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->dg_dx[4]->index[c_row],modelData->jac_matrices->dg_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[4]->index[c_row][algebraics[c_row]++] = x_ind;
			}
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->dg_dx[4]->index[c_row],modelData->jac_matrices->dg_dx[4]->size[c_row], x_ind)){
				modelData->jac_matrices->dg_dx[4]->size[c_row]--;
			} else {
				modelData->jac_matrices->dg_dx[4]->index[c_row][algebraics[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[5]->index[c_row],modelData->jac_matrices->df_dx[5]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[5]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[5]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 7);
	for(row = 1; row <= 1; row++) {
		c_row = _c_index(row);
			x_ind = _idx_dc(0);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
			x_ind = _idx_iL(0);
			if(in(modelData->jac_matrices->df_dx[6]->index[c_row],modelData->jac_matrices->df_dx[6]->size[c_row], x_ind)){
				modelData->jac_matrices->df_dx[6]->size[c_row]--;
			} else {
				modelData->jac_matrices->df_dx[6]->index[c_row][states[c_row]++] = x_ind;
			}
	}
	cleanVector(states, 0, 7);
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_3;
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_6;
	modelData->SZ[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_event_7;
	modelData->SZ[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_event_4;
	modelData->SZ[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_event_5;
	cleanVector(events, 0, 14);
	modelData->ZS[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->ZS[_idx_event_6][events[_idx_event_6]++] = _idx_iL(0);
	modelData->ZS[_idx_event_7][events[_idx_event_7]++] = _idx_iL(0);
	modelData->ZS[_idx_event_4][events[_idx_event_4]++] = _idx_uC(0);
	modelData->ZS[_idx_event_5][events[_idx_event_5]++] = _idx_uC(0);
	cleanVector(events, 0, 14);
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_1;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_1;
	modelData->HZ[_idx_event_4][events[_idx_event_4]++] = _idx_event_10;
	modelData->HZ[_idx_event_5][events[_idx_event_5]++] = _idx_event_10;
	modelData->HZ[_idx_event_6][events[_idx_event_6]++] = _idx_event_10;
	modelData->HZ[_idx_event_7][events[_idx_event_7]++] = _idx_event_10;
	modelData->HZ[_idx_event_8][events[_idx_event_8]++] = _idx_event_10;
	modelData->HZ[_idx_event_9][events[_idx_event_9]++] = _idx_event_10;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_2;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_2;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	modelData->HZ[_idx_event_2][events[_idx_event_2]++] = _idx_event_3;
	modelData->HZ[_idx_event_3][events[_idx_event_3]++] = _idx_event_3;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_3;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_4;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_4;
	modelData->HZ[_idx_event_11][events[_idx_event_11]++] = _idx_event_4;
	modelData->HZ[_idx_event_13][events[_idx_event_13]++] = _idx_event_4;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_5;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_5;
	modelData->HZ[_idx_event_11][events[_idx_event_11]++] = _idx_event_5;
	modelData->HZ[_idx_event_13][events[_idx_event_13]++] = _idx_event_5;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_6;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_6;
	modelData->HZ[_idx_event_11][events[_idx_event_11]++] = _idx_event_6;
	modelData->HZ[_idx_event_13][events[_idx_event_13]++] = _idx_event_6;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_7;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_7;
	modelData->HZ[_idx_event_11][events[_idx_event_11]++] = _idx_event_7;
	modelData->HZ[_idx_event_13][events[_idx_event_13]++] = _idx_event_7;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_8;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_8;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_9;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_9;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_3;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_3;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_4;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_4;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_5;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_5;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_6;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_6;
	modelData->HZ[_idx_event_1][events[_idx_event_1]++] = _idx_event_7;
	modelData->HZ[_idx_event_10][events[_idx_event_10]++] = _idx_event_7;
	cleanVector(events, 0, 14);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_avDiVoltage(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_avDiVoltage(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_avDiVoltage(0);
	modelData->HD[_idx_event_10][events[_idx_event_10]++] = _idx_avDiVoltage(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_avSwVoltage(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_avSwVoltage(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_avSwVoltage(0);
	modelData->HD[_idx_event_10][events[_idx_event_10]++] = _idx_avSwVoltage(0);
	modelData->HD[_idx_event_11][events[_idx_event_11]++] = _idx_dc(0);
	modelData->HD[_idx_event_12][events[_idx_event_12]++] = _idx_dc(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_iL(0);
	modelData->HD[_idx_event_2][events[_idx_event_2]++] = _idx_iL(0);
	modelData->HD[_idx_event_3][events[_idx_event_3]++] = _idx_iL(0);
	modelData->HD[_idx_event_10][events[_idx_event_10]++] = _idx_iL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_meanIL(0);
	modelData->HD[_idx_event_10][events[_idx_event_10]++] = _idx_meanIL(0);
	modelData->HD[_idx_event_1][events[_idx_event_1]++] = _idx_meanUC(0);
	modelData->HD[_idx_event_10][events[_idx_event_10]++] = _idx_meanUC(0);
	modelData->HD[_idx_event_14][events[_idx_event_14]++] = _idx_uC(0);
	cleanVector(events, 0, 14);
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_DC;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Kadj;
	modelData->event[_idx_event_14].LHSDsc[events[_idx_event_14]++] = _idx_R;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_Rd;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_Rs;
	modelData->event[_idx_event_2].LHSDsc[events[_idx_event_2]++] = _idx_Rs;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_avDiVoltageDisc;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_avSwVoltageDisc;
	modelData->event[_idx_event_3].LHSDsc[events[_idx_event_3]++] = _idx_diodeon;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastDC;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastIL;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastMeanILdisc;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastMeanUCdisc;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastT;
	modelData->event[_idx_event_10].LHSDsc[events[_idx_event_10]++] = _idx_lastTaverage;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_lastUC;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_meanILdiff;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_meanILdisc;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_meanUCdiff;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_meanUCdisc;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_modSEL;
	modelData->event[_idx_event_10].LHSDsc[events[_idx_event_10]++] = _idx_modSEL;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_nextT;
	modelData->event[_idx_event_10].LHSDsc[events[_idx_event_10]++] = _idx_nextT;
	modelData->event[_idx_event_11].LHSDsc[events[_idx_event_11]++] = _idx_ptedc;
	modelData->event[_idx_event_12].LHSDsc[events[_idx_event_12]++] = _idx_ptedc;
	modelData->event[_idx_event_11].LHSDsc[events[_idx_event_11]++] = _idx_rUc;
	modelData->event[_idx_event_13].LHSDsc[events[_idx_event_13]++] = _idx_rUc;
	modelData->event[_idx_event_11].LHSDsc[events[_idx_event_11]++] = _idx_riL;
	modelData->event[_idx_event_13].LHSDsc[events[_idx_event_13]++] = _idx_riL;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_toAvEnableI;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_toAvEnableU;
	modelData->event[_idx_event_1].LHSDsc[events[_idx_event_1]++] = _idx_toAverageEnabled;
	modelData->event[_idx_event_4].LHSDsc[events[_idx_event_4]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_6].LHSDsc[events[_idx_event_6]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_7].LHSDsc[events[_idx_event_7]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_8].LHSDsc[events[_idx_event_8]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_9].LHSDsc[events[_idx_event_9]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_10].LHSDsc[events[_idx_event_10]++] = _idx_toSwEnabled;
	modelData->event[_idx_event_4].LHSDsc[events[_idx_event_4]++] = _idx_toSwNextT;
	modelData->event[_idx_event_5].LHSDsc[events[_idx_event_5]++] = _idx_toSwNextT;
	modelData->event[_idx_event_6].LHSDsc[events[_idx_event_6]++] = _idx_toSwNextT;
	modelData->event[_idx_event_7].LHSDsc[events[_idx_event_7]++] = _idx_toSwNextT;
	modelData->event[_idx_event_8].LHSDsc[events[_idx_event_8]++] = _idx_toSwNextT;
	modelData->event[_idx_event_9].LHSDsc[events[_idx_event_9]++] = _idx_toSwNextT;
	cleanVector(events, 0, 14);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_avDiVoltage(0);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_avSwVoltage(0);
	modelData->event[_idx_event_13].LHSSt[events[_idx_event_13]++] = _idx_dc(0);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_iL(0);
	modelData->event[_idx_event_10].LHSSt[events[_idx_event_10]++] = _idx_iL(0);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_meanIL(0);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_meanUC(0);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_uC(0);
	modelData->event[_idx_event_10].LHSSt[events[_idx_event_10]++] = _idx_uC(0);
	cleanVector(events, 0, 14);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_avDiVoltage(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_avSwVoltage(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_dc(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_dc(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_iL(0);
	modelData->event[_idx_event_10].RHSSt[events[_idx_event_10]++] = _idx_iL(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_meanIL(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_meanUC(0);
	modelData->event[_idx_event_1].RHSSt[events[_idx_event_1]++] = _idx_uC(0);
	modelData->event[_idx_event_10].RHSSt[events[_idx_event_10]++] = _idx_uC(0);
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	modelData->event[_idx_event_2].direction = 1;
	modelData->event[_idx_event_2].relation = 2;
	modelData->event[_idx_event_3].direction = 0;
	modelData->event[_idx_event_3].relation = 2;
	modelData->event[_idx_event_4].direction = 1;
	modelData->event[_idx_event_4].relation = 2;
	modelData->event[_idx_event_5].direction = 1;
	modelData->event[_idx_event_5].relation = 2;
	modelData->event[_idx_event_6].direction = 1;
	modelData->event[_idx_event_6].relation = 2;
	modelData->event[_idx_event_7].direction = 1;
	modelData->event[_idx_event_7].relation = 2;
	modelData->event[_idx_event_8].direction = 1;
	modelData->event[_idx_event_8].relation = 2;
	modelData->event[_idx_event_9].direction = 1;
	modelData->event[_idx_event_9].relation = 2;
	modelData->event[_idx_event_10].direction = 1;
	modelData->event[_idx_event_10].relation = 2;
	modelData->event[_idx_event_11].direction = 1;
	modelData->event[_idx_event_11].relation = 2;
	modelData->event[_idx_event_12].direction = 1;
	modelData->event[_idx_event_12].relation = 2;
	modelData->event[_idx_event_13].direction = 1;
	modelData->event[_idx_event_13].relation = 2;
	modelData->event[_idx_event_14].direction = 1;
	modelData->event[_idx_event_14].relation = 2;
	SD_setupJacMatrices(modelData->jac_matrices);
	simulator->time = QSS_Time(7,14,0,0,ST_Binary, NULL);
	simulator->output = SD_Output("mliqss_buck",3,29,7,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOD[_idx_out_exp_3]++;
	modelOutput->nSO[_idx_iL(0)]++;
	modelOutput->nSO[_idx_uC(0)]++;
	modelOutput->nDO[_idx_modSEL]++;
	SD_allocOutputMatrix(modelOutput, 7, 29);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "uC");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "iL");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "modSEL");
	cleanVector(outputs, 0, 3);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_iL(0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_uC(0);
	cleanVector(outputs, 0, 3);
	modelOutput->OD[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_modSEL;
	cleanVector(states, 0, 7);
	modelOutput->SO[_idx_iL(0)][states[_idx_iL(0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_uC(0)][states[_idx_uC(0)]++] = _idx_out_exp_1;
	cleanVector(discretes, 0, 29);
	modelOutput->DO[_idx_modSEL][discretes[_idx_modSEL]++] = _idx_out_exp_3;
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

