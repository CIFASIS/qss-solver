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

double __PAR_CAP[2000];
double __PAR_RES[2000];
double __PAR_POT[2000];
double __PAR_THA = 0;
double __PAR_pmax = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = TRUE;
	 settings->hybrid = TRUE;
	 settings->method = 3;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 1999)
	{
		dx[1] = (__PAR_THA/__PAR_RES[(j0)]-__PAR_POT[(j0)]*d[(j0)]-x[(j0) * 3]/__PAR_RES[(j0)]+d[(j0+6000)]/__PAR_RES[(j0)])/__PAR_CAP[(j0)];
		dx[2] = (-(1.0/(__PAR_CAP[(j0)]))*x[(j0) * 3 + 1]*(1.0/(__PAR_RES[(j0)])))/2;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 1999)
	{
	if (map[j0] > NOT_ASSIGNED)
{
		der[(j0) * 3 + 1] = (__PAR_THA/__PAR_RES[(j0)]-__PAR_POT[(j0)]*d[(j0)]-x[(j0) * 3]/__PAR_RES[(j0)]+d[(j0+6000)]/__PAR_RES[(j0)])/__PAR_CAP[(j0)];
		der[(j0) * 3 + 2] = (-(1.0/(__PAR_CAP[(j0)]))*x[(j0) * 3 + 1]*(1.0/(__PAR_RES[(j0)])))/2;
	}
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1999)
	{
		zc[0] = x[(i0) * 3]-d[(i0+2000)]+d[(i0)]-5.000000000000000000000000e-01-(0.0);
		zc[1] = x[(i0) * 3 + 1];
	}
	if(i0 >= 2000 && i0 <= 3999)
	{
		zc[0] = t-(d[(i0+6000)]);
		zc[1] = 1.0;
	}
	if(i0 >= 4000 && i0 <= 5999)
	{
		zc[0] = t-(d[(i0)]);
		zc[1] = 1.0;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1999)
	{
		d[(i0)] = 1.0;
	}
	if(i0 >= 2000 && i0 <= 3999)
	{
	if((d[(i0+6000)]==1000.0))
	{
		d[(i0)] = 2.050000000000000000000000e+01;
	}
	else
	{
		d[(i0)] = 20.0;
	}
		d[(i0+6000)] = 2000.0;
	}
	if(i0 >= 4000 && i0 <= 5999)
	{
		d[(i0)] = d[(i0)]+1.0;
		d[(i0+2000)] = 2.0*fabs(sin((i0-3999)*t))-1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 1999)
	{
		d[(i0)] = 0.0;
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
	int *discretes = (int*)malloc(10000*sizeof(int));
	int *events = (int*)malloc(6000*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(2000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(2000,10000,6000,0,0,"airconds");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_THA = 32.0;
	__PAR_pmax = 0.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->x[(i0) * 3] = __math__rand(4.0)+18.0;
		__PAR_CAP[(i0)] = __math__rand(100.0)+550.0;
		__PAR_RES[(i0)] = __math__rand(4.000000000000000222044605e-01)+1.800000000000000044408921e+00;
		__PAR_POT[(i0)] = __math__rand(2.0)+13.0;
		__PAR_pmax = __PAR_pmax+__PAR_POT[(i0)];
		modelData->d[(i0+4000)] = 1.0;
		modelData->d[(i0+6000)] = __math__rand(2.0)-1.0;
		modelData->d[(i0+2000)] = 20.0;
		modelData->d[(i0+8000)] = 1000.0;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
	if(modelData->x[(i0) * 3]-modelData->d[(i0+2000)]-5.000000000000000000000000e-01>0.0)
	{
		modelData->d[(i0)] = 1.0;
	}
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nDS[i0] = 1;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nSD[i0]++;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nZS[i0] = 1;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nSZ[i0]++;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 2000; i0 <= 3999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 2000; i0 <= 3999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 4000; i0 <= 5999; i0++)
	{
		modelData->nHZ[i0]++;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 4000; i0 <= 5999; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->nDH[i0]++;
		modelData->nDH[i0+2000]++;
		modelData->nDH[i0+8000]++;
		modelData->nDH[i0+4000]++;
		modelData->nDH[i0+6000]++;
	}
	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->event[i0].nLHSDsc = 1;
		modelData->event[i0+2000].nLHSDsc = 2;
		modelData->event[i0+4000].nLHSDsc = 2;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0;
	}
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->SD[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,6000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
	}
		cleanVector(states,0,2000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,6000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 2000; i0 <= 3999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0-2000;
	}
	for(i0 = 2000; i0 <= 3999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
	for(i0 = 4000; i0 <= 5999; i0++)
	{
		modelData->HZ[i0][events[i0]++] = i0;
	}
		cleanVector(events,0,6000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	for(i0 = 4000; i0 <= 5999; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-4000;
	}
		cleanVector(discretes,0,10000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->DH[i0][discretes[i0]++] = i0;
		modelData->DH[i0+2000][discretes[i0+2000]++] = i0+2000;
		modelData->DH[i0+8000][discretes[i0+8000]++] = i0+2000;
		modelData->DH[i0+4000][discretes[i0+4000]++] = i0+4000;
		modelData->DH[i0+6000][discretes[i0+6000]++] = i0+4000;
	}
		cleanVector(events,0,6000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = i0;
		modelData->event[i0+2000].LHSDsc[events[i0+2000]++] = i0+2000;
		modelData->event[i0+2000].LHSDsc[events[i0+2000]++] = i0+8000;
		modelData->event[i0+4000].LHSDsc[events[i0+4000]++] = i0+4000;
		modelData->event[i0+4000].LHSDsc[events[i0+4000]++] = i0+6000;
	}
		cleanVector(events,0,6000);

	for(i0 = 0; i0 <= 1999; i0++)
	{
		modelData->event[i0].direction = 0;
		modelData->event[i0].relation = 2;
		modelData->event[i0+2000].direction = 1;
		modelData->event[i0+2000].relation = 2;
		modelData->event[i0+4000].direction = 1;
		modelData->event[i0+4000].relation = 2;
	}
	simulator->time = QSS_Time(2000,6000,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("airconds",1,10000,2000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,2000,10000);
		cleanVector(states,0,2000);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"th[1]");
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
