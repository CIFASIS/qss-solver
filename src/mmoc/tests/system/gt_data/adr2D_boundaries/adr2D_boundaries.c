#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "adr2D_boundaries.h"
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
	settings->method = 20;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int _d2;
	int i;
	int j;
	switch(idx) {
		case _eval_u(1,1,0): {
			_der_u(1,1,0) = -_ax*(_u(1,1,0)-_u0)/_dx-_ay*(_u(1,1,0)-_u0)/_dy+_d*(_u(2,1,0)-2*_u(1,1,0)+_u0)/pow(_dx,2)+_d*(_u(1,2,0)-2*_u(1,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(1,1,0),2)-pow(_u(1,1,0),3));
			_der_u(1,1,1) = (0)/2;
	
			return;
		}
		case _eval_u(1,4,0): {
			_der_u(1,4,0) = -_ax*(_u(1,4,0)-_u0)/_dx-_ay*(_u(1,4,0)-_u(1,3,0))/_dy+_d*(_u(2,4,0)-2*_u(1,4,0)+_u0)/pow(_dx,2)+_d*(2*_u(1,3,0)-2*_u(1,4,0))/pow(_dy,2)+_r*(pow(_u(1,4,0),2)-pow(_u(1,4,0),3));
			_der_u(1,4,1) = (0)/2;
	
			return;
		}
		case _eval_u(4,1,0): {
			_der_u(4,1,0) = -_ax*(_u(4,1,0)-_u(3,1,0))/_dx-_ay*(_u(4,1,0)-_u0)/_dy+_d*(2*_u(3,1,0)-2*_u(4,1,0))/pow(_dx,2)+_d*(_u(4,2,0)-2*_u(4,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(4,1,0),2)-pow(_u(4,1,0),3));
			_der_u(4,1,1) = (0)/2;
	
			return;
		}
		case _eval_u(4,4,0): {
			_der_u(4,4,0) = -_ax*(_u(4,4,0)-_u(3,4,0))/_dx-_ay*(_u(4,4,0)-_u(4,3,0))/_dy+_d*(2*_u(3,4,0)-2*_u(4,4,0))/pow(_dx,2)+_d*(2*_u(4,3,0)-2*_u(4,4,0))/pow(_dy,2)+_r*(pow(_u(4,4,0),2)-pow(_u(4,4,0),3));
			_der_u(4,4,1) = (0)/2;
	
			return;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1,_d2);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_der_u(i,j,0) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_der_u(i,j,1) = (0)/2;
				return;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1,_d2);
		if (_d1 == 1 && (j >= 2 && j <= 3)) {
			_der_u(1,j,0) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
			_der_u(1,j,1) = (0)/2;
			return;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1,_d2);
		if (_d1 == 4 && (j >= 2 && j <= 3)) {
			_der_u(4,j,0) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
			_der_u(4,j,1) = (0)/2;
			return;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1,_d2);
		if (_d2 == 1 && (i >= 2 && i <= 3)) {
			_der_u(i,1,0) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
			_der_u(i,1,1) = (0)/2;
			return;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1,_d2);
		if (_d2 == 4 && (i >= 2 && i <= 3)) {
			_der_u(i,4,0) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
			_der_u(i,4,1) = (0)/2;
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
			_out = _u(2,2,0);
			return;
		}
		case _eval_out_exp_2: {
			_out = _u(1,2,0);
			return;
		}
		case _eval_out_exp_3: {
			_out = _u(4,3,0);
			return;
		}
		case _eval_out_exp_4: {
			_out = _u(2,1,0);
			return;
		}
		case _eval_out_exp_5: {
			_out = _u(3,4,0);
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
	int _d1;
	int _d2;
	int i;
	int j;
	switch(idx) {
		case _eval_u(1,1,0): {
			_eval_dep_u(1,1,1) = -_ax*(_u(1,1,0)-_u0)/_dx-_ay*(_u(1,1,0)-_u0)/_dy+_d*(_u(2,1,0)-2*_u(1,1,0)+_u0)/pow(_dx,2)+_d*(_u(1,2,0)-2*_u(1,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(1,1,0),2)-pow(_u(1,1,0),3));
			_eval_dep_u(1,1,2) = (0)/2;
			break;
		}
		case _eval_u(1,2,0): {
			_eval_dep_u(1,1,1) = -_ax*(_u(1,1,0)-_u0)/_dx-_ay*(_u(1,1,0)-_u0)/_dy+_d*(_u(2,1,0)-2*_u(1,1,0)+_u0)/pow(_dx,2)+_d*(_u(1,2,0)-2*_u(1,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(1,1,0),2)-pow(_u(1,1,0),3));
			_eval_dep_u(1,1,2) = (0)/2;
			break;
		}
		case _eval_u(1,3,0): {
			_eval_dep_u(1,4,1) = -_ax*(_u(1,4,0)-_u0)/_dx-_ay*(_u(1,4,0)-_u(1,3,0))/_dy+_d*(_u(2,4,0)-2*_u(1,4,0)+_u0)/pow(_dx,2)+_d*(2*_u(1,3,0)-2*_u(1,4,0))/pow(_dy,2)+_r*(pow(_u(1,4,0),2)-pow(_u(1,4,0),3));
			_eval_dep_u(1,4,2) = (0)/2;
			break;
		}
		case _eval_u(1,4,0): {
			_eval_dep_u(1,4,1) = -_ax*(_u(1,4,0)-_u0)/_dx-_ay*(_u(1,4,0)-_u(1,3,0))/_dy+_d*(_u(2,4,0)-2*_u(1,4,0)+_u0)/pow(_dx,2)+_d*(2*_u(1,3,0)-2*_u(1,4,0))/pow(_dy,2)+_r*(pow(_u(1,4,0),2)-pow(_u(1,4,0),3));
			_eval_dep_u(1,4,2) = (0)/2;
			break;
		}
		case _eval_u(2,1,0): {
			_eval_dep_u(1,1,1) = -_ax*(_u(1,1,0)-_u0)/_dx-_ay*(_u(1,1,0)-_u0)/_dy+_d*(_u(2,1,0)-2*_u(1,1,0)+_u0)/pow(_dx,2)+_d*(_u(1,2,0)-2*_u(1,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(1,1,0),2)-pow(_u(1,1,0),3));
			_eval_dep_u(1,1,2) = (0)/2;
			break;
		}
		case _eval_u(2,4,0): {
			_eval_dep_u(1,4,1) = -_ax*(_u(1,4,0)-_u0)/_dx-_ay*(_u(1,4,0)-_u(1,3,0))/_dy+_d*(_u(2,4,0)-2*_u(1,4,0)+_u0)/pow(_dx,2)+_d*(2*_u(1,3,0)-2*_u(1,4,0))/pow(_dy,2)+_r*(pow(_u(1,4,0),2)-pow(_u(1,4,0),3));
			_eval_dep_u(1,4,2) = (0)/2;
			break;
		}
		case _eval_u(3,1,0): {
			_eval_dep_u(4,1,1) = -_ax*(_u(4,1,0)-_u(3,1,0))/_dx-_ay*(_u(4,1,0)-_u0)/_dy+_d*(2*_u(3,1,0)-2*_u(4,1,0))/pow(_dx,2)+_d*(_u(4,2,0)-2*_u(4,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(4,1,0),2)-pow(_u(4,1,0),3));
			_eval_dep_u(4,1,2) = (0)/2;
			break;
		}
		case _eval_u(3,4,0): {
			_eval_dep_u(4,4,1) = -_ax*(_u(4,4,0)-_u(3,4,0))/_dx-_ay*(_u(4,4,0)-_u(4,3,0))/_dy+_d*(2*_u(3,4,0)-2*_u(4,4,0))/pow(_dx,2)+_d*(2*_u(4,3,0)-2*_u(4,4,0))/pow(_dy,2)+_r*(pow(_u(4,4,0),2)-pow(_u(4,4,0),3));
			_eval_dep_u(4,4,2) = (0)/2;
			break;
		}
		case _eval_u(4,1,0): {
			_eval_dep_u(4,1,1) = -_ax*(_u(4,1,0)-_u(3,1,0))/_dx-_ay*(_u(4,1,0)-_u0)/_dy+_d*(2*_u(3,1,0)-2*_u(4,1,0))/pow(_dx,2)+_d*(_u(4,2,0)-2*_u(4,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(4,1,0),2)-pow(_u(4,1,0),3));
			_eval_dep_u(4,1,2) = (0)/2;
			break;
		}
		case _eval_u(4,2,0): {
			_eval_dep_u(4,1,1) = -_ax*(_u(4,1,0)-_u(3,1,0))/_dx-_ay*(_u(4,1,0)-_u0)/_dy+_d*(2*_u(3,1,0)-2*_u(4,1,0))/pow(_dx,2)+_d*(_u(4,2,0)-2*_u(4,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(4,1,0),2)-pow(_u(4,1,0),3));
			_eval_dep_u(4,1,2) = (0)/2;
			break;
		}
		case _eval_u(4,3,0): {
			_eval_dep_u(4,4,1) = -_ax*(_u(4,4,0)-_u(3,4,0))/_dx-_ay*(_u(4,4,0)-_u(4,3,0))/_dy+_d*(2*_u(3,4,0)-2*_u(4,4,0))/pow(_dx,2)+_d*(2*_u(4,3,0)-2*_u(4,4,0))/pow(_dy,2)+_r*(pow(_u(4,4,0),2)-pow(_u(4,4,0),3));
			_eval_dep_u(4,4,2) = (0)/2;
			break;
		}
		case _eval_u(4,4,0): {
			_eval_dep_u(4,4,1) = -_ax*(_u(4,4,0)-_u(3,4,0))/_dx-_ay*(_u(4,4,0)-_u(4,3,0))/_dy+_d*(2*_u(3,4,0)-2*_u(4,4,0))/pow(_dx,2)+_d*(2*_u(4,3,0)-2*_u(4,4,0))/pow(_dy,2)+_r*(pow(_u(4,4,0),2)-pow(_u(4,4,0),3));
			_eval_dep_u(4,4,2) = (0)/2;
			break;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1-1,_d2);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_eval_dep_u(i,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1,_d2-1);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_eval_dep_u(i,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1,_d2);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_eval_dep_u(i,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1,_d2+1);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_eval_dep_u(i,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1+1,_d2);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
				_eval_dep_u(i,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(1,_d2-1);
		if (_d1 == 1 && (j >= 2 && j <= 3)) {
			_eval_dep_u(1,j,1) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
			_eval_dep_u(1,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(1,_d2);
		if (_d1 == 1 && (j >= 2 && j <= 3)) {
			_eval_dep_u(1,j,1) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
			_eval_dep_u(1,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(1,_d2+1);
		if (_d1 == 1 && (j >= 2 && j <= 3)) {
			_eval_dep_u(1,j,1) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
			_eval_dep_u(1,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(2,_d2);
		if (_d1 == 2 && (j >= 2 && j <= 3)) {
			_eval_dep_u(1,j,1) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
			_eval_dep_u(1,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(3,_d2);
		if (_d1 == 3 && (j >= 2 && j <= 3)) {
			_eval_dep_u(4,j,1) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
			_eval_dep_u(4,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(4,_d2-1);
		if (_d1 == 4 && (j >= 2 && j <= 3)) {
			_eval_dep_u(4,j,1) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
			_eval_dep_u(4,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(4,_d2);
		if (_d1 == 4 && (j >= 2 && j <= 3)) {
			_eval_dep_u(4,j,1) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
			_eval_dep_u(4,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(4,_d2+1);
		if (_d1 == 4 && (j >= 2 && j <= 3)) {
			_eval_dep_u(4,j,1) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
			_eval_dep_u(4,j,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1-1,1);
		if (_d2 == 1 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,1,1) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
			_eval_dep_u(i,1,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1,1);
		if (_d2 == 1 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,1,1) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
			_eval_dep_u(i,1,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1,2);
		if (_d2 == 2 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,1,1) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
			_eval_dep_u(i,1,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1+1,1);
		if (_d2 == 1 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,1,1) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
			_eval_dep_u(i,1,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1-1,4);
		if (_d2 == 4 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,4,1) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
			_eval_dep_u(i,4,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1,3);
		if (_d2 == 3 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,4,1) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
			_eval_dep_u(i,4,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1,4);
		if (_d2 == 4 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,4,1) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
			_eval_dep_u(i,4,2) = (0)/2;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1+1,4);
		if (_d2 == 4 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,4,1) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
			_eval_dep_u(i,4,2) = (0)/2;
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
	int _d2;
	int i;
	int j;
	switch(idx) {
		case _eval_u(1,1,0): {
			_eval_dep_u(1,1,1) = -_ax*(_u(1,1,0)-_u0)/_dx-_ay*(_u(1,1,0)-_u0)/_dy+_d*(_u(2,1,0)-2*_u(1,1,0)+_u0)/pow(_dx,2)+_d*(_u(1,2,0)-2*_u(1,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(1,1,0),2)-pow(_u(1,1,0),3));
	
	
			continue;
		}
		case _eval_u(1,4,0): {
			_eval_dep_u(1,4,1) = -_ax*(_u(1,4,0)-_u0)/_dx-_ay*(_u(1,4,0)-_u(1,3,0))/_dy+_d*(_u(2,4,0)-2*_u(1,4,0)+_u0)/pow(_dx,2)+_d*(2*_u(1,3,0)-2*_u(1,4,0))/pow(_dy,2)+_r*(pow(_u(1,4,0),2)-pow(_u(1,4,0),3));
	
	
			continue;
		}
		case _eval_u(4,1,0): {
			_eval_dep_u(4,1,1) = -_ax*(_u(4,1,0)-_u(3,1,0))/_dx-_ay*(_u(4,1,0)-_u0)/_dy+_d*(2*_u(3,1,0)-2*_u(4,1,0))/pow(_dx,2)+_d*(_u(4,2,0)-2*_u(4,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(4,1,0),2)-pow(_u(4,1,0),3));
	
	
			continue;
		}
		case _eval_u(4,4,0): {
			_eval_dep_u(4,4,1) = -_ax*(_u(4,4,0)-_u(3,4,0))/_dx-_ay*(_u(4,4,0)-_u(4,3,0))/_dy+_d*(2*_u(3,4,0)-2*_u(4,4,0))/pow(_dx,2)+_d*(2*_u(4,3,0)-2*_u(4,4,0))/pow(_dy,2)+_r*(pow(_u(4,4,0),2)-pow(_u(4,4,0),3));
	
	
			continue;
		}
	}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1,_d2);
		if ((i >= 2 && i <= 3) && (j >= 2 && j <= 3)) {
				_eval_dep_u(i,j,1) = -_ax*(_u(i,j,0)-_u(i-1,j,0))/_dx-_ay*(_u(i,j,0)-_u(i,j-1,0))/_dy+_d*(_u(i+1,j,0)-2*_u(i,j,0)+_u(i-1,j,0))/pow(_dx,2)+_d*(_u(i,j+1,0)-2*_u(i,j,0)+_u(i,j-1,0))/pow(_dy,2)+_r*(pow(_u(i,j,0),2)-pow(_u(i,j,0),3));
	
				continue;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_2(_d1,_d2);
		if (_d1 == 1 && (j >= 2 && j <= 3)) {
			_eval_dep_u(1,j,1) = -_ax*(_u(1,j,0)-_u0)/_dx-_ay*(_u(1,j,0)-_u(1,j-1,0))/_dy+_d*(_u(2,j,0)-2*_u(1,j,0)+_u0)/pow(_dx,2)+_d*(_u(1,j+1,0)-2*_u(1,j,0)+_u(1,j-1,0))/pow(_dy,2)+_r*(pow(_u(1,j,0),2)-pow(_u(1,j,0),3));
	
			continue;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_3(_d1,_d2);
		if (_d1 == 4 && (j >= 2 && j <= 3)) {
			_eval_dep_u(4,j,1) = -_ax*(_u(4,j,0)-_u(3,j,0))/_dx-_ay*(_u(4,j,0)-_u(4,j-1,0))/_dy+_d*(2*_u(3,j,0)-2*_u(4,j,0))/pow(_dx,2)+_d*(_u(4,j+1,0)-2*_u(4,j,0)+_u(4,j-1,0))/pow(_dy,2)+_r*(pow(_u(4,j,0),2)-pow(_u(4,j,0),3));
	
			continue;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_4(_d1,_d2);
		if (_d2 == 1 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,1,1) = -_ax*(_u(i,1,0)-_u(i-1,1,0))/_dx-_ay*(_u(i,1,0)-_u0)/_dy+_d*(_u(i+1,1,0)-2*_u(i,1,0)+_u(i-1,1,0))/pow(_dx,2)+_d*(_u(i,2,0)-2*_u(i,1,0)+_u0)/pow(_dy,2)+_r*(pow(_u(i,1,0),2)-pow(_u(i,1,0),3));
	
			continue;
	
		}
		}
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_5(_d1,_d2);
		if (_d2 == 4 && (i >= 2 && i <= 3)) {
			_eval_dep_u(i,4,1) = -_ax*(_u(i,4,0)-_u(i-1,4,0))/_dx-_ay*(_u(i,4,0)-_u(i,3,0))/_dy+_d*(_u(i+1,4,0)-2*_u(i,4,0)+_u(i-1,4,0))/pow(_dx,2)+_d*(2*_u(i,3,0)-2*_u(i,4,0))/pow(_dy,2)+_r*(pow(_u(i,4,0),2)-pow(_u(i,4,0),3));
	
			continue;
	
		}
		}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(16,0,0,0,0,9,0,"adr2D_boundaries");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(16*sizeof(int));
	int* outputs = (int*) malloc(5*sizeof(int));
	int _d1;
	int _d2;
	_ax = 1;
	_ay = 1;
	_d = 1.000000e-02;
	_dx = 10/(double)(4-1);
	_dy = _dx;
	_r = 1;
	_u0 = 1;
	modelData->nSD[_idx_u(1,1,0)]++;
	modelData->nSD[_idx_u(1,2,0)]++;
	modelData->nSD[_idx_u(1,3,0)]++;
	modelData->nSD[_idx_u(1,4,0)]++;
	modelData->nSD[_idx_u(2,1,0)]++;
	modelData->nSD[_idx_u(2,4,0)]++;
	modelData->nSD[_idx_u(3,1,0)]++;
	modelData->nSD[_idx_u(3,4,0)]++;
	modelData->nSD[_idx_u(4,1,0)]++;
	modelData->nSD[_idx_u(4,2,0)]++;
	modelData->nSD[_idx_u(4,3,0)]++;
	modelData->nSD[_idx_u(4,4,0)]++;
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nSD[_idx_u(_d1+1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2+1,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nSD[_idx_u(_d1,_d2-1,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nSD[_idx_u(_d1-1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(1,_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(2,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(3,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(4,_d1+1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(4,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(4,_d1-1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1,2,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1+1,4,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1,3,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1,4,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nSD[_idx_u(_d1-1,4,0)]++;
	}
	modelData->nDS[_idx_u(1,1,0)]++;
	modelData->nDS[_idx_u(1,1,0)]++;
	modelData->nDS[_idx_u(1,4,0)]++;
	modelData->nDS[_idx_u(1,4,0)]++;
	modelData->nDS[_idx_u(1,1,0)]++;
	modelData->nDS[_idx_u(1,4,0)]++;
	modelData->nDS[_idx_u(4,1,0)]++;
	modelData->nDS[_idx_u(4,4,0)]++;
	modelData->nDS[_idx_u(4,1,0)]++;
	modelData->nDS[_idx_u(4,1,0)]++;
	modelData->nDS[_idx_u(4,4,0)]++;
	modelData->nDS[_idx_u(4,4,0)]++;
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->nDS[_idx_u(_d1,_d2,0)]++;
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(1,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(4,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(4,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(4,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(4,_d1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,1,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,4,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,4,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,4,0)]++;
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->nDS[_idx_u(_d1,4,0)]++;
	}
	QSS_allocDataMatrix(modelData);
	cleanVector(states, 0, 16);
	modelData->SD[_idx_u(1,1,0)][states[_idx_u(1,1,0)]++] = _idx_u(1,1,0);
	modelData->SD[_idx_u(1,2,0)][states[_idx_u(1,2,0)]++] = _idx_u(1,1,0);
	modelData->SD[_idx_u(1,3,0)][states[_idx_u(1,3,0)]++] = _idx_u(1,4,0);
	modelData->SD[_idx_u(1,4,0)][states[_idx_u(1,4,0)]++] = _idx_u(1,4,0);
	modelData->SD[_idx_u(2,1,0)][states[_idx_u(2,1,0)]++] = _idx_u(1,1,0);
	modelData->SD[_idx_u(2,4,0)][states[_idx_u(2,4,0)]++] = _idx_u(1,4,0);
	modelData->SD[_idx_u(3,1,0)][states[_idx_u(3,1,0)]++] = _idx_u(4,1,0);
	modelData->SD[_idx_u(3,4,0)][states[_idx_u(3,4,0)]++] = _idx_u(4,4,0);
	modelData->SD[_idx_u(4,1,0)][states[_idx_u(4,1,0)]++] = _idx_u(4,1,0);
	modelData->SD[_idx_u(4,2,0)][states[_idx_u(4,2,0)]++] = _idx_u(4,1,0);
	modelData->SD[_idx_u(4,3,0)][states[_idx_u(4,3,0)]++] = _idx_u(4,4,0);
	modelData->SD[_idx_u(4,4,0)][states[_idx_u(4,4,0)]++] = _idx_u(4,4,0);
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->SD[_idx_u(_d1+1,_d2,0)][states[_idx_u(_d1+1,_d2,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2+1,0)][states[_idx_u(_d1,_d2+1,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->SD[_idx_u(_d1,_d2-1,0)][states[_idx_u(_d1,_d2-1,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->SD[_idx_u(_d1-1,_d2,0)][states[_idx_u(_d1-1,_d2,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(1,_d1+1,0)][states[_idx_u(1,_d1+1,0)]++] = _idx_u(1,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(1,_d1,0)][states[_idx_u(1,_d1,0)]++] = _idx_u(1,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(1,_d1-1,0)][states[_idx_u(1,_d1-1,0)]++] = _idx_u(1,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(2,_d1,0)][states[_idx_u(2,_d1,0)]++] = _idx_u(1,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(3,_d1,0)][states[_idx_u(3,_d1,0)]++] = _idx_u(4,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(4,_d1+1,0)][states[_idx_u(4,_d1+1,0)]++] = _idx_u(4,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(4,_d1,0)][states[_idx_u(4,_d1,0)]++] = _idx_u(4,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(4,_d1-1,0)][states[_idx_u(4,_d1-1,0)]++] = _idx_u(4,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,1,0)][states[_idx_u(_d1+1,1,0)]++] = _idx_u(_d1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1,1,0)][states[_idx_u(_d1,1,0)]++] = _idx_u(_d1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1,2,0)][states[_idx_u(_d1,2,0)]++] = _idx_u(_d1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,1,0)][states[_idx_u(_d1-1,1,0)]++] = _idx_u(_d1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1+1,4,0)][states[_idx_u(_d1+1,4,0)]++] = _idx_u(_d1,4,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1,3,0)][states[_idx_u(_d1,3,0)]++] = _idx_u(_d1,4,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1,4,0)][states[_idx_u(_d1,4,0)]++] = _idx_u(_d1,4,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->SD[_idx_u(_d1-1,4,0)][states[_idx_u(_d1-1,4,0)]++] = _idx_u(_d1,4,0);
	}
	cleanVector(states, 0, 16);
	modelData->DS[_idx_u(1,1,0)][states[_idx_u(1,1,0)]++] = _idx_u(1,1,0);
	modelData->DS[_idx_u(1,1,0)][states[_idx_u(1,1,0)]++] = _idx_u(1,2,0);
	modelData->DS[_idx_u(1,4,0)][states[_idx_u(1,4,0)]++] = _idx_u(1,3,0);
	modelData->DS[_idx_u(1,4,0)][states[_idx_u(1,4,0)]++] = _idx_u(1,4,0);
	modelData->DS[_idx_u(1,1,0)][states[_idx_u(1,1,0)]++] = _idx_u(2,1,0);
	modelData->DS[_idx_u(1,4,0)][states[_idx_u(1,4,0)]++] = _idx_u(2,4,0);
	modelData->DS[_idx_u(4,1,0)][states[_idx_u(4,1,0)]++] = _idx_u(3,1,0);
	modelData->DS[_idx_u(4,4,0)][states[_idx_u(4,4,0)]++] = _idx_u(3,4,0);
	modelData->DS[_idx_u(4,1,0)][states[_idx_u(4,1,0)]++] = _idx_u(4,1,0);
	modelData->DS[_idx_u(4,1,0)][states[_idx_u(4,1,0)]++] = _idx_u(4,2,0);
	modelData->DS[_idx_u(4,4,0)][states[_idx_u(4,4,0)]++] = _idx_u(4,3,0);
	modelData->DS[_idx_u(4,4,0)][states[_idx_u(4,4,0)]++] = _idx_u(4,4,0);
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1+1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1,_d2+1,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1,_d2-1,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		for(_d2 = 2; _d2<=3; _d2+=1) {
			modelData->DS[_idx_u(_d1,_d2,0)][states[_idx_u(_d1,_d2,0)]++] = _idx_u(_d1-1,_d2,0);
		}
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(1,_d1,0)][states[_idx_u(1,_d1,0)]++] = _idx_u(1,_d1+1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(1,_d1,0)][states[_idx_u(1,_d1,0)]++] = _idx_u(1,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(1,_d1,0)][states[_idx_u(1,_d1,0)]++] = _idx_u(1,_d1-1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(1,_d1,0)][states[_idx_u(1,_d1,0)]++] = _idx_u(2,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(4,_d1,0)][states[_idx_u(4,_d1,0)]++] = _idx_u(3,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(4,_d1,0)][states[_idx_u(4,_d1,0)]++] = _idx_u(4,_d1+1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(4,_d1,0)][states[_idx_u(4,_d1,0)]++] = _idx_u(4,_d1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(4,_d1,0)][states[_idx_u(4,_d1,0)]++] = _idx_u(4,_d1-1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,1,0)][states[_idx_u(_d1,1,0)]++] = _idx_u(_d1+1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,1,0)][states[_idx_u(_d1,1,0)]++] = _idx_u(_d1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,1,0)][states[_idx_u(_d1,1,0)]++] = _idx_u(_d1,2,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,1,0)][states[_idx_u(_d1,1,0)]++] = _idx_u(_d1-1,1,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,4,0)][states[_idx_u(_d1,4,0)]++] = _idx_u(_d1+1,4,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,4,0)][states[_idx_u(_d1,4,0)]++] = _idx_u(_d1,3,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,4,0)][states[_idx_u(_d1,4,0)]++] = _idx_u(_d1,4,0);
	}
	for(_d1 = 2; _d1<=3; _d1+=1) {
		modelData->DS[_idx_u(_d1,4,0)][states[_idx_u(_d1,4,0)]++] = _idx_u(_d1-1,4,0);
	}
	simulator->time = QSS_Time(16,0,0,0,ST_Binary, NULL);
		double period[1];
		period[0] = 0.01;
		simulator->output = SD_Output("adr2D_boundaries",5,0,16,period,1,0,CI_Sampled,SD_Memory,MOD_output);
		SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_2]++;
	modelOutput->nOS[_idx_out_exp_4]++;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nOS[_idx_out_exp_5]++;
	modelOutput->nOS[_idx_out_exp_3]++;
	modelOutput->nSO[_idx_u(1,2,0)]++;
	modelOutput->nSO[_idx_u(2,1,0)]++;
	modelOutput->nSO[_idx_u(2,2,0)]++;
	modelOutput->nSO[_idx_u(3,4,0)]++;
	modelOutput->nSO[_idx_u(4,3,0)]++;
	SD_allocOutputMatrix(modelOutput, 16, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "u[2,2]");
	sprintf(modelOutput->variable[_idx_out_exp_2].name, "u[1,2]");
	sprintf(modelOutput->variable[_idx_out_exp_3].name, "u[4,3]");
	sprintf(modelOutput->variable[_idx_out_exp_4].name, "u[2,1]");
	sprintf(modelOutput->variable[_idx_out_exp_5].name, "u[3,4]");
	cleanVector(outputs, 0, 5);
	modelOutput->OS[_idx_out_exp_2][outputs[_idx_out_exp_2]++] = _idx_u(1,2,0);
	modelOutput->OS[_idx_out_exp_4][outputs[_idx_out_exp_4]++] = _idx_u(2,1,0);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_u(2,2,0);
	modelOutput->OS[_idx_out_exp_5][outputs[_idx_out_exp_5]++] = _idx_u(3,4,0);
	modelOutput->OS[_idx_out_exp_3][outputs[_idx_out_exp_3]++] = _idx_u(4,3,0);
	cleanVector(states, 0, 16);
	modelOutput->SO[_idx_u(1,2,0)][states[_idx_u(1,2,0)]++] = _idx_out_exp_2;
	modelOutput->SO[_idx_u(2,1,0)][states[_idx_u(2,1,0)]++] = _idx_out_exp_4;
	modelOutput->SO[_idx_u(2,2,0)][states[_idx_u(2,2,0)]++] = _idx_out_exp_1;
	modelOutput->SO[_idx_u(3,4,0)][states[_idx_u(3,4,0)]++] = _idx_out_exp_5;
	modelOutput->SO[_idx_u(4,3,0)][states[_idx_u(4,3,0)]++] = _idx_out_exp_3;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(outputs);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

