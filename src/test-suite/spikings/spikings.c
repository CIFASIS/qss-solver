#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_math.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_a = 0;
double __PAR_b = 0;
double __PAR_c[1000];
double __PAR_d[1000];
double __PAR_I[1000];
double __PAR_r[1000];

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
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 999)
	{
		dx[1] = 4.000000000000000083266727e-02*x[(j0) * 3]*x[(j0) * 3]+5.0*x[(j0) * 3]+140.0-x[(j0+1000) * 3]+__PAR_I[(j0)];
		dx[2] = (-x[(j0+1000) * 3 + 1]+5.0*x[(j0) * 3 + 1]+(8.000000000000000166533454e-02)*x[(j0) * 3]*x[(j0) * 3 + 1])/2;
	}
	j0 = i0-1000;
	if(j0 >=0 && j0 <= 999)
	{
		dx[1] = __PAR_a*(__PAR_b*x[(j0) * 3]-x[(j0+1000) * 3]);
		dx[2] = (-(x[(j0+1000) * 3 + 1]-x[(j0) * 3 + 1]*__PAR_b)*__PAR_a)/2;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 999)
	{
		der[(j0) * 3 + 1] = 4.000000000000000083266727e-02*x[(j0) * 3]*x[(j0) * 3]+5.0*x[(j0) * 3]+140.0-x[(j0+1000) * 3]+__PAR_I[(j0)];
		der[(j0) * 3 + 2] = (-x[(j0+1000) * 3 + 1]+5.0*x[(j0) * 3 + 1]+(8.000000000000000166533454e-02)*x[(j0) * 3]*x[(j0) * 3 + 1])/2;
		der[(j0+1000) * 3 + 1] = __PAR_a*(__PAR_b*x[(j0) * 3]-x[(j0+1000) * 3]);
		der[(j0+1000) * 3 + 2] = (-(x[(j0+1000) * 3 + 1]-x[(j0) * 3 + 1]*__PAR_b)*__PAR_a)/2;
	}
	j0 = i0-1000;
	if(j0 >=0 && j0 <= 999)
	{
		der[(j0) * 3 + 1] = 4.000000000000000083266727e-02*x[(j0) * 3]*x[(j0) * 3]+5.0*x[(j0) * 3]+140.0-x[(j0+1000) * 3]+__PAR_I[(j0)];
		der[(j0) * 3 + 2] = (-x[(j0+1000) * 3 + 1]+5.0*x[(j0) * 3 + 1]+(8.000000000000000166533454e-02)*x[(j0) * 3]*x[(j0) * 3 + 1])/2;
		der[(j0+1000) * 3 + 1] = __PAR_a*(__PAR_b*x[(j0) * 3]-x[(j0+1000) * 3]);
		der[(j0+1000) * 3 + 2] = (-(x[(j0+1000) * 3 + 1]-x[(j0) * 3 + 1]*__PAR_b)*__PAR_a)/2;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 999)
	{
		zc[0] = x[(i0) * 3]-(30.0);
		zc[1] = x[(i0) * 3 + 1];
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 999)
	{
		x[(i0) * 3] = __PAR_c[(i0)];
		x[(i0+1000) * 3] = x[(i0+1000) * 3]+__PAR_d[(i0)];
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
		case 1:
			out[0] = x[3];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *events = (int*)malloc(1000*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(2000,0,1000,0,0,"spikings");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_a = 2.000000000000000041633363e-02;
	__PAR_b = 2.000000000000000111022302e-01;
	// Initialize model code.
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->x[(i0) * 3] = -60.0;
		modelData->x[(i0+1000) * 3] = 0.0;
		__PAR_r[(i0)] = __math__rand(1.0);
		__PAR_c[(i0)] = -65.0+15.0*pow(__PAR_r[(i0)],2.0);
		__PAR_d[(i0)] = 8.0-6.0*pow(__PAR_r[(i0)],2.0);
		__PAR_I[(i0)] = __math__rand(10.0);
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nDS[i0] = 2;
		modelData->nDS[i0+1000] = 2;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nSD[i0]++;
		modelData->nSD[i0+1000]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+1000]++;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nZS[i0] = 1;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nSZ[i0]++;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].nLHSSt = 2;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].nRHSSt = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+1000;
		modelData->DS[i0+1000][states[i0+1000]++] = i0;
		modelData->DS[i0+1000][states[i0+1000]++] = i0+1000;
	}
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+1000][states[i0+1000]++] = i0;
		modelData->SD[i0][states[i0]++] = i0+1000;
		modelData->SD[i0+1000][states[i0+1000]++] = i0+1000;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
	}
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].LHSSt[events[i0]++] = i0;
		modelData->event[i0].LHSSt[events[i0]++] = i0+1000;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].RHSSt[events[i0]++] = i0+1000;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].direction = 1;
		modelData->event[i0].relation = 2;
	}
	simulator->time = QSS_Time(2000,1000,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("spikings",2,0,2000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[1]++;
	SD_allocOutputMatrix(modelOutput,2000,0);
		cleanVector(states,0,2000);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"v[1]");
		sprintf(modelOutput->variable[1].name,"v[2]");
		cleanVector(outputs,0,2);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
		modelOutput->SO[1][states[1]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 1;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,NULL);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
