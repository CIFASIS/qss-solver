#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

double __PAR_Ron = 0;
double __PAR_Roff = 0;
double __PAR_U = 0;
double __PAR_L = 0;
double __PAR_R = 0;
double __PAR_w = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 8;
}

void
MOD_definition(double *x, double *d, double *alg, double t, double *dx)
{
	modelData->scalarEvaluations++;
		dx[0] = 1000.0*(__PAR_U*sin(__PAR_w*t)-x[0]);
		dx[1] = (x[0]-x[1]*(__PAR_R+d[(0)]))/__PAR_L;
}

void
MOD_jacobian(double *x, double *d, double *alg, double t, double *jac)
{
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	modelData->zeroCrossings++;
	switch(i)
	{
		case 0:
			zc[0] = x[1]-(0.0);
			return;
		case 1:
			zc[0] = x[0]-(0.0);
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 1:
			d[(0)] = __PAR_Ron;
			return;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(0)] = __PAR_Roff;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[1];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(2*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0 = 0;
	simulator->data = CLC_Data(2,1,2,1,0,"rectifierDASSL");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Ron = 1.000000000000000081803054e-05;
	__PAR_Roff = 1.000000000000000000000000e+05;
	__PAR_U = 311.0;
	__PAR_L = 1.000000000000000020816682e-03;
	__PAR_R = 10.0;
	__PAR_w = 3.141600000000000250111043e+02;
	modelData->d[(0)] = 1.000000000000000000000000e+05;
	// Initialize model code.
	CLC_allocDataMatrix(modelData);
		modelData->event[0].direction = -1;
		modelData->event[1].direction = 1;
		double period[1];
	period[0] = 0.0002;
	simulator->output = SD_Output("rectifierDASSL",2,1,2,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[1]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,2,1);
		cleanVector(states,0,2);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"iL");
		sprintf(modelOutput->variable[1].name,"u");
		cleanVector(outputs,0,2);

		modelOutput->SO[1][states[1]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 1;
		modelOutput->SO[0][states[0]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 0;
	simulator->model = CLC_Model(MOD_definition,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg, MOD_jacobian);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
