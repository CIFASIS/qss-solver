#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

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
	 settings->method = 4;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			dx[1] = 1000.0*(__PAR_U*sin(__PAR_w*t)-x[0]);
			dx[2] = (-1000.0*x[1]+1000.0*cos(t*__PAR_w)*__PAR_U*__PAR_w)/2;
			return;
		case 1:
			dx[1] = (x[0]-x[3]*(__PAR_R+d[(0)]))/__PAR_L;
			dx[2] = (-(1.0/(__PAR_L))*(x[4]*(d[(0)]+__PAR_R)-x[1]))/2;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			der[0 + 1] = 1000.0*(__PAR_U*sin(__PAR_w*t)-x[0]);
			der[0 + 2] = (-1000.0*x[1]+1000.0*cos(t*__PAR_w)*__PAR_U*__PAR_w)/2;
			der[3 + 1] = (x[0]-x[3]*(__PAR_R+d[(0)]))/__PAR_L;
			der[3 + 2] = (-(1.0/(__PAR_L))*(x[4]*(d[(0)]+__PAR_R)-x[1]))/2;
			return;
		case 1:
			der[3 + 1] = (x[0]-x[3]*(__PAR_R+d[(0)]))/__PAR_L;
			der[3 + 2] = (-(1.0/(__PAR_L))*(x[4]*(d[(0)]+__PAR_R)-x[1]))/2;
			return;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			zc[0] = x[3]-(0.0);
			zc[1] = x[4];
			return;
		case 1:
			zc[0] = x[0]-(0.0);
			zc[1] = x[1];
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
			out[0] = x[3];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(2*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0;
	int td = 0;
	simulator->data = QSS_Data(2,1,2,1,0,"rectifier");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Ron = 1.000000000000000081803054e-05;
	__PAR_Roff = 1.000000000000000000000000e+05;
	__PAR_U = 311.0;
	__PAR_L = 1.000000000000000020816682e-03;
	__PAR_R = 10.0;
	__PAR_w = 3.141600000000000250111043e+02;
	modelData->d[(0)] = 1.000000000000000081803054e-05;
	// Initialize model code.
	modelData->nDS[0] = 1;
	modelData->nDS[1] = 2;
	modelData->nSD[0]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nZS[0] = 1;
	modelData->nZS[1] = 1;
	modelData->nSZ[1]++;
	modelData->nSZ[0]++;
	modelData->nHD[0] = 1;
	modelData->nHD[1] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
	modelData->TD[td++] = 0;
		cleanVector(states,0,2);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
		cleanVector(states,0,2);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
		cleanVector(events,0,2);

	modelData->ZS[0][events[0]++] = 1;
	modelData->ZS[1][events[1]++] = 0;
		cleanVector(states,0,2);

	modelData->SZ[1][states[1]++] = 0;
	modelData->SZ[0][states[0]++] = 1;
		cleanVector(events,0,2);

	modelData->HD[0][events[0]++] = 1;
	modelData->HD[1][events[1]++] = 1;
		cleanVector(events,0,2);

	modelData->event[0].LHSDsc[events[0]++] = 0;
	modelData->event[1].LHSDsc[events[1]++] = 0;
		cleanVector(events,0,2);

	modelData->event[0].direction = -1;
	modelData->event[0].relation = 0;
	modelData->event[1].direction = 1;
	modelData->event[1].relation = 2;
	simulator->time = QSS_Time(2,2,1,0,ST_Binary,NULL);

	simulator->output = SD_Output("rectifier",2,1,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
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
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
