#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "allow_discrete_integer.h"
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
	settings->method = 0;
}

void MOD_definition(int idx, double *x, double *d, double *a, double t, double *dx)
{
	int _d1;
	int i;
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_der_u(i,0) = _time;
	
	
		}
		return;
	}
}

void MOD_zeroCrossing(int idx, double *x, double *d, double *a, double t, double *zc)
{
	switch(idx) {
		case _eval_event_1: {
			_zc(0) = _time-(0);
	
	
			return;
		}
	}
}

void MOD_handlerPos(int idx, double *x, double* q, double *d, double *a, double t)
{
	switch(idx) {
		case _eval_event_1: {
			_init_u((int)_d,0) = 0;
					return;
		}
	}
}

void MOD_handlerNeg(int idx, double *x, double* q, double *d, double *a, double t)
{
}

void MOD_output(int idx, double *x, double *d, double *a, double t, double *out)
{
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void MOD_dependencies(int idx, double *x, double *d, double *a, double t, double *dx, int *map)
{
}

void MOD_BDF_definition(double *x, double *d, double *a, double t, double *dx, int *BDFMap, int nBDF)
{
	int idx;
	int __bdf_it;
	for(__bdf_it = 0; __bdf_it < nBDF; __bdf_it++) {
	idx = BDFMap[__bdf_it];
	int _d1;
	int i;
	if (_is_var_u(idx)) {
		_get_u_idxs(idx);
		_apply_usage_eq_1(_d1);
		if ((i >= 1 && i <= 1000)) {
			_eval_dep_u(i,1) = _time;
	
	
		}
		continue;
	}
	}
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
	simulator->data = QSS_Data(1000,1,1,1000,0,1,0,"allow_discrete_integer");
	QSS_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* states = (int*) malloc(1000*sizeof(int));
	int* discretes = (int*) malloc(1*sizeof(int));
	int* events = (int*) malloc(1*sizeof(int));
	int _d1;
	int i;
	modelData->event[_idx_event_1].nLHSSt++;
	QSS_allocDataMatrix(modelData);
	cleanVector(events, 0, 1);
	modelData->event[_idx_event_1].LHSSt[events[_idx_event_1]++] = _idx_u(0,0);
	modelData->event[_idx_event_1].direction = 1;
	modelData->event[_idx_event_1].relation = 2;
	simulator->time = QSS_Time(1000,1,1000,0,ST_Binary, NULL);
	for(i = 1; i<=1000; i+=1) {
		modelData->IT[_input_1(i)] = _idx_u(i,0);
	}
	simulator->output = SD_Output("allow_discrete_integer",0,1,1000,NULL,0,0,CI_Step,SD_Memory,NULL);
	SD_output modelOutput = simulator->output;
	simulator->model = QSS_Model(MOD_definition, MOD_dependencies, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian, MOD_BDF_definition);
	free(states);
	free(discretes);
	free(events);
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
}

