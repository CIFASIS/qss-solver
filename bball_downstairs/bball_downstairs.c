#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>


void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			dx[1] = x[4];
			dx[2] = (x[5])/2;
			dx[3] = (x[6]*2)/6;
			return;
		case 1:
			dx[1] = -9.800000000000000710542736e+00-1.000000000000000055511151e-01*x[4]-d[(1)]*((x[0]-d[(0)])*1.000000000000000000000000e+06+x[4]*30.0);
			dx[2] = (-d[(1)]*((1.000000000000000000000000e+06)*x[1]+30.0*x[5])-(1.000000000000000055511151e-01)*x[5])/2;
			dx[3] = (-d[(1)]*((1.000000000000000000000000e+06)*x[2]*2+30.0*x[6]*2)-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
		case 2:
			dx[1] = x[12];
			dx[2] = (x[13])/2;
			dx[3] = (x[14]*2)/6;
			return;
		case 3:
			dx[1] = -1.000000000000000055511151e-01*x[12];
			dx[2] = (-(1.000000000000000055511151e-01)*x[13])/2;
			dx[3] = (-(1.000000000000000055511151e-01)*x[14]*2)/6;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			der[4 + 1] = -9.800000000000000710542736e+00-1.000000000000000055511151e-01*x[4]-d[(1)]*((x[0]-d[(0)])*1.000000000000000000000000e+06+x[4]*30.0);
			der[4 + 2] = (-d[(1)]*((1.000000000000000000000000e+06)*x[1]+30.0*x[5])-(1.000000000000000055511151e-01)*x[5])/2;
			der[4 + 3] = (-d[(1)]*((1.000000000000000000000000e+06)*x[2]*2+30.0*x[6]*2)-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
		case 1:
			der[0 + 1] = x[4];
			der[0 + 2] = (x[5])/2;
			der[0 + 3] = (x[6]*2)/6;
			der[4 + 1] = -9.800000000000000710542736e+00-1.000000000000000055511151e-01*x[4]-d[(1)]*((x[0]-d[(0)])*1.000000000000000000000000e+06+x[4]*30.0);
			der[4 + 2] = (-d[(1)]*((1.000000000000000000000000e+06)*x[1]+30.0*x[5])-(1.000000000000000055511151e-01)*x[5])/2;
			der[4 + 3] = (-d[(1)]*((1.000000000000000000000000e+06)*x[2]*2+30.0*x[6]*2)-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
		case 3:
			der[8 + 1] = x[12];
			der[8 + 2] = (x[13])/2;
			der[8 + 3] = (x[14]*2)/6;
			der[12 + 1] = -1.000000000000000055511151e-01*x[12];
			der[12 + 2] = (-(1.000000000000000055511151e-01)*x[13])/2;
			der[12 + 3] = (-(1.000000000000000055511151e-01)*x[14]*2)/6;
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
			zc[0] = x[0]-(d[(0)]);
			zc[1] = x[1];
			zc[2] = (x[2]*2)/2;
			return;
		case 1:
			zc[0] = x[8]-11.0+d[(0)]-(0.0);
			zc[1] = x[9];
			zc[2] = (x[10]*2)/2;
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
			d[(1)] = 0.0;
			return;
		case 1:
			d[(0)] = d[(0)]-1.0;
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
			d[(1)] = 1.0;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(2*sizeof(int));
	int *events = (int*)malloc(2*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(4*sizeof(int));
	int i0;
	simulator->data = QSS_Data(4,2,2,0,0,"bball_downstairs");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	modelData->d[(0)] = 10.0;
	modelData->x[12] = 5.000000000000000000000000e-01;
	modelData->x[8] = 5.749999999999999555910790e-01;
	modelData->x[0] = 1.050000000000000000000000e+01;
	// Initialize model code.
	modelData->nDS[0] = 1;
	modelData->nDS[1] = 2;
	modelData->nDS[2] = 1;
	modelData->nDS[3] = 1;
	modelData->nSD[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[3]++;
	modelData->nSD[3]++;
	modelData->nZS[0] = 1;
	modelData->nZS[1] = 1;
	modelData->nSZ[0]++;
	modelData->nSZ[2]++;
	modelData->nHZ[1] += 2;
	modelData->nHD[0] = 1;
	modelData->nHD[1] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,4);

	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
	modelData->DS[2][states[2]++] = 3;
	modelData->DS[3][states[3]++] = 3;
		cleanVector(states,0,4);

	modelData->SD[1][states[1]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
	modelData->SD[3][states[3]++] = 2;
	modelData->SD[3][states[3]++] = 3;
		cleanVector(events,0,2);

	modelData->ZS[0][events[0]++] = 0;
	modelData->ZS[1][events[1]++] = 2;
		cleanVector(states,0,4);

	modelData->SZ[0][states[0]++] = 0;
	modelData->SZ[2][states[2]++] = 1;
		cleanVector(events,0,2);

	modelData->HZ[1][events[1]++] = 0;
	modelData->HZ[1][events[1]++] = 1;
		cleanVector(events,0,2);

	modelData->HD[1][events[1]++] = 1;
	modelData->HD[0][events[0]++] = 1;
		cleanVector(events,0,2);

	modelData->event[0].LHSDsc[events[0]++] = 1;
	modelData->event[1].LHSDsc[events[1]++] = 0;
		cleanVector(events,0,2);

	modelData->event[0].direction = 0;
	modelData->event[0].relation = 0;
	modelData->event[1].direction = 1;
	modelData->event[1].relation = 2;
	simulator->time = QSS_Time(4,2,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("bball_downstairs",1,2,4,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,4,2);
		cleanVector(states,0,4);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"y");
		cleanVector(outputs,0,1);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
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
