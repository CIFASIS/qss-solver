#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_math.h"
#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

double __PAR_CAP[200];
double __PAR_RES[200];
double __PAR_POT[200];
double __PAR_THA = 0;
double __PAR_pmax = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 9;
}

void
MOD_definition(double *x, double *d, double *alg, double t, double *dx)
{
	int i0 = 0;
	modelData->scalarEvaluations++;
	for(i0 = 0; i0 <= 199; i0++)
	{
		dx[i0] = (__PAR_THA/__PAR_RES[(i0)]-__PAR_POT[(i0)]*d[(i0+1)]-x[(i0) * 1]/__PAR_RES[(i0)]+d[(i0+601)]/__PAR_RES[(i0)])/__PAR_CAP[(i0)];
	}
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
	if(i0 >= 0 && i0 <= 199)
	{
		zc[0] = x[(i0) * 1]-d[(i0+201)]+d[(i0+1)]-5.000000000000000000000000e-01-(0.0);
	}
	if(i0 >= 200 && i0 <= 399)
	{
		zc[0] = t-(1000.0);
	}
	if(i0 >= 400 && i0 <= 599)
	{
		zc[0] = t-(2000.0);
	}
	if(i0 >= 600 && i0 <= 799)
	{
		zc[0] = t-(d[(i0-199)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 199)
	{
		d[(i0+1)] = 1.0;
		d[(0)] = d[(0)]+__PAR_POT[(i0)];
	}
	if(i0 >= 200 && i0 <= 399)
	{
		d[(i0+1)] = 2.050000000000000000000000e+01;
	}
	if(i0 >= 400 && i0 <= 599)
	{
		d[(i0-199)] = 20.0;
	}
	if(i0 >= 600 && i0 <= 799)
	{
		d[(i0-199)] = d[(i0-199)]+1.0;
		d[(i0+1)] = __math__rand(2.0)-1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 199)
	{
	if(t>0.0)
	{
		d[(i0+1)] = 0.0;
		d[(0)] = d[(0)]-__PAR_POT[(i0)];
	}
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
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int *discretes = (int*)malloc(801*sizeof(int));
	int *events = (int*)malloc(800*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(200*sizeof(int));
	int i0 = 0;
	int j0 = 0;
	simulator->data = CLC_Data(200,801,800,0,0,"aircondsDOPRI");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_THA = 32.0;
	__PAR_pmax = 0.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 199; i0++)
	{
		modelData->x[(i0) * 1] = __math__rand(4.0)+18.0;
		__PAR_CAP[(i0)] = __math__rand(100.0)+550.0;
		__PAR_RES[(i0)] = __math__rand(4.000000000000000222044605e-01)+1.800000000000000044408921e+00;
		__PAR_POT[(i0)] = __math__rand(2.0)+13.0;
		__PAR_pmax = __PAR_pmax+__PAR_POT[(i0)];
		modelData->d[(i0+401)] = 1.0;
		modelData->d[(i0+601)] = __math__rand(2.0)-1.0;
		modelData->d[(i0+201)] = 20.0;
	}
	CLC_allocDataMatrix(modelData);
	for(i0 = 0; i0 <= 199; i0++)
	{
		modelData->event[i0].direction = 0;
		modelData->event[i0+200].direction = 1;
		modelData->event[i0+400].direction = 1;
		modelData->event[i0+600].direction = 1;
	}
		double period[1];
	period[0] = 0.6;
	simulator->output = SD_Output("aircondsDOPRI",1,801,200,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOD[0] = 1;
		modelOutput->nDO[0]++;
	SD_allocOutputMatrix(modelOutput,200,801);
		cleanVector(discretes,0,801);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"ptotal");
		cleanVector(outputs,0,1);

		modelOutput->DO[0][discretes[0]++] = 0;
		modelOutput->OD[0][outputs[0]++] = 0;
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
