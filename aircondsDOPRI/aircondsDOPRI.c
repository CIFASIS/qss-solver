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
	 settings->method = 8;
}

void
MOD_definition(double *x, double *d, double *alg, double t, double *dx)
{
	int i = 0;
	modelData->scalarEvaluations++;
	for(i = 0; i <= 199; i++)
	{
		dx[i] = (__PAR_THA/__PAR_RES[(i)]-__PAR_POT[(i)]*d[(i+1)]-x[(i) * 1]/__PAR_RES[(i)]+d[(i+601)]/__PAR_RES[(i)])/__PAR_CAP[(i)];
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	modelData->zeroCrossings++;
	if(i >= 0 && i <= 199)
	{
		zc[0] = x[(i) * 1]-d[(i+201)]+d[(i+1)]-5.000000000000000000000000e-01-(0.0);
	}
	if(i >= 200 && i <= 399)
	{
		zc[0] = t-(1000.0);
	}
	if(i >= 400 && i <= 599)
	{
		zc[0] = t-(2000.0);
	}
	if(i >= 600 && i <= 799)
	{
		zc[0] = t-(d[(i-199)]);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 199)
	{
		d[(i+1)] = 1.0;
		d[(0)] = d[(0)]+__PAR_POT[(i)];
	}
	if(i >= 200 && i <= 399)
	{
		d[(i+1)] = 2.050000000000000000000000e+01;
	}
	if(i >= 400 && i <= 599)
	{
		d[(i-199)] = 20.0;
	}
	if(i >= 600 && i <= 799)
	{
		d[(i-199)] = d[(i-199)]+1.0;
		d[(i+1)] = __math__rand(2.0)-1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 199)
	{
	if(t>0.0)
	{
		d[(i+1)] = 0.0;
		d[(0)] = d[(0)]-__PAR_POT[(i)];
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
	int discretes[801];
	int i = 0;
	int i2;
	int outputs[1];
	simulator->data = CLC_Data(200,801,800,0,0,"aircondsDOPRI");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_THA = 32.0;
	__PAR_pmax = 0.0;
	// Initialize model code.
	for(i2 = 0; i2 <= 199; i2++)
	{
		modelData->x[(i2) * 1] = __math__rand(4.0)+18.0;
		__PAR_CAP[(i2)] = __math__rand(100.0)+550.0;
		__PAR_RES[(i2)] = __math__rand(4.000000000000000222044605e-01)+1.800000000000000044408921e+00;
		__PAR_POT[(i2)] = __math__rand(2.0)+13.0;
		__PAR_pmax = __PAR_pmax+__PAR_POT[(i2)];
		modelData->d[(i2+401)] = 1.0;
		modelData->d[(i2+601)] = __math__rand(2.0)-1.0;
		modelData->d[(i2+201)] = 20.0;
	}
	for(i = 0; i <= 199; i++)
	{
		modelData->event[i].direction = 0;
		modelData->event[i+200].direction = 1;
		modelData->event[i+400].direction = 1;
		modelData->event[i+600].direction = 1;
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
	simulator->model = CLC_Model(MOD_definition,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
