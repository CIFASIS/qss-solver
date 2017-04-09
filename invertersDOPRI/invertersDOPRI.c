#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;

double __PAR_UOP = 0;
double __PAR_G = 0;
double __PAR_UTH = 0;

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
		dx[0] = d[(1)];
	for(i = 1; i <= 501; i++)
	{
		dx[i] = __PAR_UOP-x[(i) * 1]-__PAR_G*(d[(i+1)]*pow((x[(i-1) * 1]-__PAR_UTH),2.0)-d[(i+502)]*pow((x[(i-1) * 1]-x[(i) * 1]-__PAR_UTH),2.0));
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	modelData->zeroCrossings++;
	switch(i)
	{
		case 1002:
			zc[0] = t-(d[(0)]);
			return;
		default:
			if(i >= 0 && i <= 500)
			{
				zc[0] = x[(i) * 1]-(__PAR_UTH);
			}
			if(i >= 501 && i <= 1001)
			{
				zc[0] = x[(i-501) * 1]-x[(i-500) * 1]-(__PAR_UTH);
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	switch(i)
	{
		case 1002:
	if(d[(0)]==5.0)
	{
			d[(1)] = 1.0;
			d[(0)] = 10.0;
	}
	else if(d[(0)]==10.0)
	{
			d[(1)] = 0.0;
			d[(0)] = 15.0;
	}
	else if(d[(0)]==15.0)
	{
			d[(1)] = -2.500000000000000000000000e+00;
			d[(0)] = 17.0;
	}
	else
	{
			d[(1)] = 0.0;
			d[(0)] = 1.000000000000000000000000e+10;
	}
			return;
		default:
			if(i >= 0 && i <= 500)
			{
				d[(i+2)] = 1.0;
			}
			if(i >= 501 && i <= 1001)
			{
				d[(i+2)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 500)
	{
		d[(i+2)] = 0.0;
	}
	if(i >= 501 && i <= 1001)
	{
		d[(i+2)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[500];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int discretes[1004];
	int i = 0;
	int i0;
	int outputs[2];
	int states[502];
	simulator->data = CLC_Data(502,1004,1003,0,0,"invertersDOPRI");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_UOP = 5.0;
	__PAR_G = 100.0;
	__PAR_UTH = 1.0;
	modelData->d[(0)] = 5.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 249; i0++)
	{
		modelData->x[(2*i0+1) * 1] = 5.0;
		modelData->x[(2*i0+2) * 1] = 6.246999999999999948763207e-03;
	}
	for(i = 0; i <= 500; i++)
	{
		modelData->event[i].direction = 0;
		modelData->event[i+501].direction = 0;
	}
		modelData->event[1002].direction = 1;
		double period[1];
	period[0] = 0.04;
	simulator->output = SD_Output("invertersDOPRI",2,1004,502,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[500]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,502,1004);
		cleanVector(states,0,502);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"x[501]");
		sprintf(modelOutput->variable[1].name,"x[1]");
		cleanVector(outputs,0,2);

		modelOutput->SO[500][states[500]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 500;
		modelOutput->SO[0][states[0]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 0;
	simulator->model = CLC_Model(MOD_definition,MOD_zeroCrossing,MOD_handlerPos,MOD_handlerNeg);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
