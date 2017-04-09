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

double __PAR_CAP[200];
double __PAR_RES[200];
double __PAR_POT[200];
double __PAR_THA = 0;
double __PAR_pmax = 0;
double __PAR_Kp = 0;
double __PAR_Ki = 0;
double __PAR_tref = 0;

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
	int j = 0;
	switch(i)
	{
		case 200:
			dx[1] = d[(203)]-d[(1)]/__PAR_pmax;
			dx[2] = (0.0)/2;
			dx[3] = (0.0)/6;
			return;
		default:
			j = i;
			if(j >=0 && j <= 199)
			{
				dx[1] = (__PAR_THA/__PAR_RES[(j)]-__PAR_POT[(j)]*d[(j+2)]-x[(j) * 4]/__PAR_RES[(j)]+d[(205)]/__PAR_RES[(j)])/__PAR_CAP[(j)];
				dx[2] = (-(1.0/(__PAR_CAP[(j)]))*x[(j) * 4 + 1]*(1.0/(__PAR_RES[(j)])))/2;
				dx[3] = (-(1.0/(__PAR_CAP[(j)]))*(1.0/(__PAR_RES[(j)]))*x[(j) * 4 + 2]*2)/6;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 199)
	{
		der[(j) * 4 + 1] = (__PAR_THA/__PAR_RES[(j)]-__PAR_POT[(j)]*d[(j+2)]-x[(j) * 4]/__PAR_RES[(j)]+d[(205)]/__PAR_RES[(j)])/__PAR_CAP[(j)];
		der[(j) * 4 + 2] = (-(1.0/(__PAR_CAP[(j)]))*x[(j) * 4 + 1]*(1.0/(__PAR_RES[(j)])))/2;
		der[(j) * 4 + 3] = (-(1.0/(__PAR_CAP[(j)]))*(1.0/(__PAR_RES[(j)]))*x[(j) * 4 + 2]*2)/6;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	switch(i)
	{
		case 200:
			zc[0] = t-(1000.0);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
			return;
		case 201:
			zc[0] = t-(2000.0);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
			return;
		case 202:
			zc[0] = t-(d[(204)]);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
			return;
		default:
			if(i >= 0 && i <= 199)
			{
				zc[0] = x[(i) * 4]-__PAR_tref-d[(202)]+d[(i+2)]-5.000000000000000000000000e-01-(0.0);
				zc[1] = x[(i) * 4 + 1];
				zc[2] = (x[(i) * 4 + 2]*2)/2;
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	switch(i)
	{
		case 200:
			d[(203)] = 4.000000000000000222044605e-01;
			return;
		case 201:
			d[(203)] = 5.000000000000000000000000e-01;
			return;
		case 202:
			d[(204)] = d[(204)]+1.0;
			d[(205)] = __math__rand(2.0)-1.0;
			d[(1)] = d[(0)];
			d[(202)] = __PAR_Kp*(d[(1)]/__PAR_pmax-d[(203)])-__PAR_Ki*x[800];
			return;
		default:
			if(i >= 0 && i <= 199)
			{
				d[(i+2)] = 1.0;
				d[(0)] = d[(0)]+__PAR_POT[(i)];
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 199)
	{
		d[(i+2)] = 0.0;
		d[(0)] = d[(0)]-__PAR_POT[(i)];
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = d[(0)];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(206*sizeof(int));
	int *events = (int*)malloc(203*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(201*sizeof(int));
	int i2;
	int i;
	int j = 0;
	simulator->data = QSS_Data(201,206,203,0,0,"aircont");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_THA = 32.0;
	__PAR_pmax = 0.0;
	__PAR_Kp = 1.0;
	__PAR_Ki = 1.0;
	__PAR_tref = 20.0;
	modelData->d[(204)] = 1.0;
	modelData->d[(205)] = __math__rand(2.0)-1.0;
	modelData->d[(203)] = 5.000000000000000000000000e-01;
	// Initialize model code.
	for(i2 = 0; i2 <= 199; i2++)
	{
		modelData->x[(i2) * 4] = __math__rand(4.0)+18.0;
		__PAR_CAP[(i2)] = __math__rand(100.0)+550.0;
		__PAR_RES[(i2)] = __math__rand(4.000000000000000222044605e-01)+1.800000000000000044408921e+00;
		__PAR_POT[(i2)] = __math__rand(2.000000000000000111022302e-01)+13.0;
		__PAR_pmax = __PAR_pmax+__PAR_POT[(i2)];
	if(modelData->x[(i2) * 4]-__PAR_tref-modelData->d[(202)]+modelData->d[(i2+2)]-5.000000000000000000000000e-01>0.0)
	{
		modelData->d[(i2+2)] = 1.0;
		modelData->d[(0)] = modelData->d[(0)]+__PAR_POT[(i2)];
	}
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->nDS[i] = 1;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->nSD[i]++;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->nZS[i] = 1;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->nSZ[i]++;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->nHZ[i]++;
	}
	modelData->nHZ[202] += 201;
	for(i = 0; i <= 199; i++)
	{
		modelData->nHD[i]++;
	}
	modelData->nHD[200] = 1;
	modelData->nHD[201] = 1;
	modelData->nHD[202] = 201;
	modelData->event[202].nRHSSt = 1;
	for(i = 0; i <= 199; i++)
	{
		modelData->event[i].nLHSDsc = 2;
	}
	modelData->event[200].nLHSDsc = 1;
	modelData->event[201].nLHSDsc = 1;
	modelData->event[202].nLHSDsc = 4;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,201);

	for(i = 0; i <= 199; i++)
	{
		modelData->DS[i][states[i]++] = i;
	}
		cleanVector(states,0,201);

	for(i = 0; i <= 199; i++)
	{
		modelData->SD[i][states[i]++] = i;
	}
		cleanVector(events,0,203);

	for(i = 0; i <= 199; i++)
	{
		modelData->ZS[i][events[i]++] = i;
	}
		cleanVector(states,0,201);

	for(i = 0; i <= 199; i++)
	{
		modelData->SZ[i][states[i]++] = i;
	}
		cleanVector(events,0,203);

	for(i = 0; i <= 199; i++)
	{
		modelData->HZ[i][events[i]++] = i;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->HZ[202][events[202]++] = i;
	}
	modelData->HZ[202][events[202]++] = 202;
		cleanVector(events,0,203);

	for(i = 0; i <= 199; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->HD[202][events[202]++] = i;
	}
	modelData->HD[202][events[202]++] = 200;
	modelData->HD[200][events[200]++] = 200;
	modelData->HD[201][events[201]++] = 200;
		cleanVector(events,0,203);

	modelData->event[202].RHSSt[events[202]++] = 200;
		cleanVector(events,0,203);

	for(i = 0; i <= 199; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = 0;
		modelData->event[i].LHSDsc[events[i]++] = i+2;
	}
	modelData->event[200].LHSDsc[events[200]++] = 203;
	modelData->event[201].LHSDsc[events[201]++] = 203;
	modelData->event[202].LHSDsc[events[202]++] = 1;
	modelData->event[202].LHSDsc[events[202]++] = 202;
	modelData->event[202].LHSDsc[events[202]++] = 204;
	modelData->event[202].LHSDsc[events[202]++] = 205;
		cleanVector(events,0,203);

	for(i = 0; i <= 199; i++)
	{
		modelData->event[i].direction = 0;
		modelData->event[i].relation = 2;
	}
	modelData->event[200].direction = 1;
	modelData->event[200].relation = 2;
	modelData->event[201].direction = 1;
	modelData->event[201].relation = 2;
	modelData->event[202].direction = 1;
	modelData->event[202].relation = 2;
	simulator->time = QSS_Time(201,203,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("aircont",1,206,201,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOD[0] = 1;
		modelOutput->nDO[0]++;
	SD_allocOutputMatrix(modelOutput,201,206);
		cleanVector(discretes,0,206);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"ptotal");
		cleanVector(outputs,0,1);

		modelOutput->DO[0][discretes[0]++] = 0;
		modelOutput->OD[0][outputs[0]++] = 0;
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
