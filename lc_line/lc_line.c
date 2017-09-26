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
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			dx[1] = d[(0)]-x[4];
			dx[2] = (-x[5])/2;
			dx[3] = (-x[6]*2)/6;
			return;
		case 199:
			dx[1] = x[792];
			dx[2] = (x[793])/2;
			dx[3] = (x[794]*2)/6;
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = x[(j0-1) * 4]-x[(j0+1) * 4];
				dx[2] = (-x[(j0+1) * 4 + 1]+x[(j0-1) * 4 + 1])/2;
				dx[3] = (x[(j0-1) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)/6;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 1:
			der[0 + 1] = d[(0)]-x[4];
			der[0 + 2] = (-x[5])/2;
			der[0 + 3] = (-x[6]*2)/6;
			break;
		case 198:
			der[796 + 1] = x[792];
			der[796 + 2] = (x[793])/2;
			der[796 + 3] = (x[794]*2)/6;
			break;
	}
	j0 = i0+1;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 4 + 1] = x[(j0-1) * 4]-x[(j0+1) * 4];
		der[(j0) * 4 + 2] = (-x[(j0+1) * 4 + 1]+x[(j0-1) * 4 + 1])/2;
		der[(j0) * 4 + 3] = (x[(j0-1) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)/6;
	}
	j0 = i0-1;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 4 + 1] = x[(j0-1) * 4]-x[(j0+1) * 4];
		der[(j0) * 4 + 2] = (-x[(j0+1) * 4 + 1]+x[(j0-1) * 4 + 1])/2;
		der[(j0) * 4 + 3] = (x[(j0-1) * 4 + 2]*2-x[(j0+1) * 4 + 2]*2)/6;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			zc[0] = t-(1.0);
			zc[1] = 1.0;
			zc[2] = (0.0)/2;
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
			d[(0)] = 0.0;
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
			d[(0)] = 1.0;
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
		case 1:
			out[0] = x[796];
			return;
		case 2:
			out[0] = x[400];
			return;
		case 3:
			out[0] = x[396];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1*sizeof(int));
	int *events = (int*)malloc(1*sizeof(int));
	int *outputs = (int*)malloc(4*sizeof(int));
	int *states = (int*)malloc(200*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(200,1,1,0,0,"lc_line");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	modelData->d[(0)] = 1.0;
	// Initialize model code.
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nDS[i0] = 2;
	}
	modelData->nDS[0] = 1;
	modelData->nDS[199] = 1;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nSD[i0-1]++;
		modelData->nSD[i0+1]++;
	}
	modelData->nSD[1]++;
	modelData->nSD[198]++;
	modelData->nHD[0] = 1;
	modelData->event[0].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,200);

	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0-1;
		modelData->DS[i0][states[i0]++] = i0+1;
	}
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[199][states[199]++] = 198;
		cleanVector(states,0,200);

	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->SD[i0-1][states[i0-1]++] = i0;
		modelData->SD[i0+1][states[i0+1]++] = i0;
	}
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[198][states[198]++] = 199;
		cleanVector(events,0,1);

	modelData->HD[0][events[0]++] = 0;
		cleanVector(events,0,1);

	modelData->event[0].LHSDsc[events[0]++] = 0;
		cleanVector(events,0,1);

	modelData->event[0].direction = 0;
	modelData->event[0].relation = 2;
	simulator->time = QSS_Time(200,1,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("lc_line",4,1,200,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[199]++;
		modelOutput->nOS[2] = 1;
		modelOutput->nSO[100]++;
		modelOutput->nOS[3] = 1;
		modelOutput->nSO[99]++;
	SD_allocOutputMatrix(modelOutput,200,1);
		cleanVector(states,0,200);

		cleanVector(outputs,0,4);

		sprintf(modelOutput->variable[0].name,"x[1]");
		sprintf(modelOutput->variable[1].name,"x[200]");
		sprintf(modelOutput->variable[2].name,"x[101]");
		sprintf(modelOutput->variable[3].name,"x[100]");
		cleanVector(outputs,0,4);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
		modelOutput->SO[199][states[199]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 199;
		modelOutput->SO[100][states[100]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 100;
		modelOutput->SO[99][states[99]++] = 3;
		modelOutput->OS[3][outputs[3]++] = 99;
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
