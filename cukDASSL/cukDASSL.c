#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

double __PAR_C = 0;
double __PAR_L = 0;
double __PAR_R = 0;
double __PAR_U = 0;
double __PAR_T = 0;
double __PAR_DC = 0;
double __PAR_ROn = 0;
double __PAR_ROff = 0;
double __PAR_C1 = 0;
double __PAR_L1 = 0;

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
		alg[0] = (d[(1)]*(x[3]+x[1])-x[0])/(d[(0)]+d[(1)]);
		dx[0] = (alg[0]-x[3])/__PAR_C1;
		dx[1] = (__PAR_U-x[0]-alg[0]*d[(0)])/__PAR_L1;
		dx[2] = (x[3]-x[2]/__PAR_R)/__PAR_C;
		dx[3] = (-x[2]-alg[0]*d[(0)])/__PAR_L;
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
			zc[0] = t-(d[(2)]);
			return;
		case 1:
			zc[0] = t-d[(3)]-__PAR_DC*__PAR_T-(0.0);
			return;
		case 2:
			alg[0] = (d[(1)]*(x[3]+x[1])-x[0])/(d[(0)]+d[(1)]);
			zc[0] = alg[0]-(0.0);
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(3)] = d[(2)];
			d[(2)] = d[(2)]+__PAR_T;
			d[(1)] = __PAR_ROn;
			return;
		case 1:
			d[(1)] = __PAR_ROff;
			d[(0)] = __PAR_ROn;
			return;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 2:
			d[(0)] = __PAR_ROff;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[2];
			return;
		case 1:
			out[0] = x[3];
			return;
		case 2:
			out[0] = x[0];
			return;
		case 3:
			out[0] = x[1];
			return;
	}
}

void
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int *discretes = (int*)malloc(4*sizeof(int));
	int *events = (int*)malloc(3*sizeof(int));
	int *outputs = (int*)malloc(4*sizeof(int));
	int *states = (int*)malloc(4*sizeof(int));
	int i0 = 0;
	simulator->data = CLC_Data(4,4,3,0,1,"cukDASSL");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_C = 1.000000000000000047921736e-04;
	__PAR_L = 1.000000000000000047921736e-04;
	__PAR_R = 10.0;
	__PAR_U = 24.0;
	__PAR_T = 1.000000000000000047921736e-04;
	__PAR_DC = 2.500000000000000000000000e-01;
	__PAR_ROn = 1.000000000000000081803054e-05;
	__PAR_ROff = 1.000000000000000000000000e+05;
	__PAR_C1 = 1.000000000000000047921736e-04;
	__PAR_L1 = 1.000000000000000047921736e-04;
	// Initialize model code.
		modelData->d[(2)] = __PAR_T;
		modelData->d[(1)] = 1.000000000000000000000000e+05;
		modelData->d[(0)] = 1.000000000000000000000000e+05;
	CLC_allocDataMatrix(modelData);
		modelData->event[0].direction = 1;
		modelData->event[1].direction = 1;
		modelData->event[2].direction = -1;
		double period[1];
	period[0] = 2e-06;
	simulator->output = SD_Output("cukDASSL",4,4,4,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[2]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[3]++;
		modelOutput->nOS[2] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[3] = 1;
		modelOutput->nSO[1]++;
	SD_allocOutputMatrix(modelOutput,4,4);
		cleanVector(states,0,4);

		cleanVector(outputs,0,4);

		sprintf(modelOutput->variable[0].name,"uC");
		sprintf(modelOutput->variable[1].name,"iL");
		sprintf(modelOutput->variable[2].name,"uC1");
		sprintf(modelOutput->variable[3].name,"iL1");
		cleanVector(outputs,0,4);

		modelOutput->SO[2][states[2]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 2;
		modelOutput->SO[3][states[3]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 3;
		modelOutput->SO[0][states[0]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 0;
		modelOutput->SO[1][states[1]++] = 3;
		modelOutput->OS[3][outputs[3]++] = 1;
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
