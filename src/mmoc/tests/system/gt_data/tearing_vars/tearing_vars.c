
#include <common/solve_loop.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "tearing_vars.h"
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
	settings->method = 9;
}

void MOD_definition(double *x, double *d, double *a, double t, double *dx)
{
	SVL_SolveLoop(x, d, a, &t, dx, 1, MOD_EvalTearingVars);
	_der_x = 1+_a;
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
			_out = _x;
			return;
		}
	}
}

void MOD_jacobian(double *x, double *d, double *a, double t, SD_jacMatrices dvdx, double *jac)
{
}

void MOD_EvalTearingVars(double *x, double *d, double *a, double *t, double *dx, const double *guess, double *ret)
{
	_a = guess[0];
	MOD_AssignLoop(x, d, a, t, dx);
	ret[0] = _x+_a;
}

void MOD_AssignLoop(double *x, double *d, double *a, double *t, double *dx)
{
}

void CLC_initializeDataStructs(CLC_simulator simulator)
{
	simulator->data = CLC_Data(1,0,0,0,2,1,3,"tearing_vars");
	CLC_data modelData = simulator->data;
	MODEL_DATA_ACCESS(modelData)
	int* algebraics = (int*) malloc(2*sizeof(int));
	int* states = (int*) malloc(1*sizeof(int));
	int* outputs = (int*) malloc(1*sizeof(int));
	CLC_allocDataMatrix(modelData);
	simulator->output = SD_Output("tearing_vars",1,0,1,NULL,0,0,CI_Step,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;
	modelOutput->nOS[_idx_out_exp_1]++;
	modelOutput->nSO[_idx_x]++;
	SD_allocOutputMatrix(modelOutput, 1, 0);
	sprintf(modelOutput->variable[_idx_out_exp_1].name, "x");
	cleanVector(outputs, 0, 1);
	modelOutput->OS[_idx_out_exp_1][outputs[_idx_out_exp_1]++] = _idx_x;
	cleanVector(states, 0, 1);
	modelOutput->SO[_idx_x][states[_idx_x]++] = _idx_out_exp_1;
	simulator->model = CLC_Model(MOD_definition, MOD_zeroCrossing, MOD_handlerPos, MOD_handlerNeg, MOD_jacobian);
	free(algebraics);
	free(states);
	free(outputs);
}

void QSS_initializeDataStructs(QSS_simulator simulator)
{
}

