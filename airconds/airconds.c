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
	int j = 0;
	j = i;
	if(j >=0 && j <= 1999)
	{
		dx[1] = (__PAR_THA/__PAR_RES[(j)]-__PAR_POT[(j)]*d[(j)]-x[(j) * 3]/__PAR_RES[(j)]+d[(j+6000)]/__PAR_RES[(j)])/__PAR_CAP[(j)];
		dx[2] = (-(1.0/(__PAR_CAP[(j)]))*x[(j) * 3 + 1]*(1.0/(__PAR_RES[(j)])))/2;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 1999)
	{
	if (map[j] > NOT_ASSIGNED)
{
		der[(j) * 3 + 1] = (__PAR_THA/__PAR_RES[(j)]-__PAR_POT[(j)]*d[(j)]-x[(j) * 3]/__PAR_RES[(j)]+d[(j+6000)]/__PAR_RES[(j)])/__PAR_CAP[(j)];
		der[(j) * 3 + 2] = (-(1.0/(__PAR_CAP[(j)]))*x[(j) * 3 + 1]*(1.0/(__PAR_RES[(j)])))/2;
	}
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 1999)
	{
		zc[0] = x[(i) * 3]-d[(i+2000)]+d[(i)]-5.000000000000000000000000e-01-(0.0);
		zc[1] = x[(i) * 3 + 1];
	}
	if(i >= 2000 && i <= 3999)
	{
		zc[0] = t-(d[(i+6000)]);
		zc[1] = 1.0;
	}
	if(i >= 4000 && i <= 5999)
	{
		zc[0] = t-(d[(i)]);
		zc[1] = 1.0;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 1999)
	{
		d[(i)] = 1.0;
	}
	if(i >= 2000 && i <= 3999)
	{
	if((d[(i+6000)]==1000.0))
	{
		d[(i)] = 2.050000000000000000000000e+01;
	}
	else
	{
		d[(i)] = 20.0;
	}
		d[(i+6000)] = 2000.0;
	}
	if(i >= 4000 && i <= 5999)
	{
		d[(i)] = d[(i)]+1.0;
		d[(i+2000)] = 2.0*fabs(sin((i-3999)*t))-1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 1999)
	{
		d[(i)] = 0.0;
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
	int i10;
	int i4;
	int i;
	int j = 0;
	simulator->data = QSS_Data(2000,10000,6000,0,0,"airconds");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_THA = 32.0;
	__PAR_pmax = 0.0;
	// Initialize model code.
	for(i4 = 0; i4 <= 1999; i4++)
	{
		modelData->x[(i4) * 3] = __math__rand(4.0)+18.0;
		__PAR_CAP[(i4)] = __math__rand(100.0)+550.0;
		__PAR_RES[(i4)] = __math__rand(4.000000000000000222044605e-01)+1.800000000000000044408921e+00;
		__PAR_POT[(i4)] = __math__rand(2.0)+13.0;
		__PAR_pmax = __PAR_pmax+__PAR_POT[(i4)];
		modelData->d[(i4+4000)] = 1.0;
		modelData->d[(i4+6000)] = __math__rand(2.0)-1.0;
		modelData->d[(i4+2000)] = 20.0;
		modelData->d[(i4+8000)] = 1000.0;
	}
	for(i10 = 0; i10 <= 1999; i10++)
	{
	if(modelData->x[(i10) * 3]-modelData->d[(i10+2000)]-5.000000000000000000000000e-01>0.0)
	{
		modelData->d[(i10)] = 1.0;
	}
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nDS[i] = 1;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nSD[i]++;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nZS[i] = 1;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nSZ[i]++;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 2000; i <= 3999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 2000; i <= 3999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 4000; i <= 5999; i++)
	{
		modelData->nHZ[i]++;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 4000; i <= 5999; i++)
	{
		modelData->nHD[i]++;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->nDH[i]++;
		modelData->nDH[i+2000]++;
		modelData->nDH[i+8000]++;
		modelData->nDH[i+4000]++;
		modelData->nDH[i+6000]++;
	}
	for(i = 0; i <= 1999; i++)
	{
		modelData->event[i].nLHSDsc = 1;
		modelData->event[i+2000].nLHSDsc = 2;
		modelData->event[i+4000].nLHSDsc = 2;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->DS[i][states[i]++] = i;
	}
		cleanVector(states,0,2000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->SD[i][states[i]++] = i;
	}
		cleanVector(events,0,6000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->ZS[i][events[i]++] = i;
	}
		cleanVector(states,0,2000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->SZ[i][states[i]++] = i;
	}
		cleanVector(events,0,6000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 2000; i <= 3999; i++)
	{
		modelData->HZ[i][events[i]++] = i-2000;
	}
	for(i = 2000; i <= 3999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 4000; i <= 5999; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
		cleanVector(events,0,6000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 4000; i <= 5999; i++)
	{
		modelData->HD[i][events[i]++] = i-4000;
	}
		cleanVector(discretes,0,10000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->DH[i][discretes[i]++] = i;
		modelData->DH[i+2000][discretes[i+2000]++] = i+2000;
		modelData->DH[i+8000][discretes[i+8000]++] = i+2000;
		modelData->DH[i+4000][discretes[i+4000]++] = i+4000;
		modelData->DH[i+6000][discretes[i+6000]++] = i+4000;
	}
		cleanVector(events,0,6000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = i;
		modelData->event[i+2000].LHSDsc[events[i+2000]++] = i+2000;
		modelData->event[i+2000].LHSDsc[events[i+2000]++] = i+8000;
		modelData->event[i+4000].LHSDsc[events[i+4000]++] = i+4000;
		modelData->event[i+4000].LHSDsc[events[i+4000]++] = i+6000;
	}
		cleanVector(events,0,6000);

	for(i = 0; i <= 1999; i++)
	{
		modelData->event[i].direction = 0;
		modelData->event[i].relation = 2;
		modelData->event[i+2000].direction = 1;
		modelData->event[i+2000].relation = 2;
		modelData->event[i+4000].direction = 1;
		modelData->event[i+4000].relation = 2;
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
