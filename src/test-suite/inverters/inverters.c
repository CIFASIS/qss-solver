#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_UOP = 0;
double __PAR_G = 0;
double __PAR_UTH = 0;

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
	switch(i)
	{
		case 0:
			dx[1] = d[(1)];
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 501)
			{
				dx[1] = __PAR_UOP-x[(j0) * 3]-__PAR_G*(d[(j0+1)]*pow((x[(j0-1) * 3]-__PAR_UTH),2.0)-d[(j0+502)]*pow((x[(j0-1) * 3]-x[(j0) * 3]-__PAR_UTH),2.0));
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0+1;
	if(j0 >=1 && j0 <= 501)
	{
		der[(j0) * 3 + 1] = __PAR_UOP-x[(j0) * 3]-__PAR_G*(d[(j0+1)]*pow((x[(j0-1) * 3]-__PAR_UTH),2.0)-d[(j0+502)]*pow((x[(j0-1) * 3]-x[(j0) * 3]-__PAR_UTH),2.0));
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 501)
	{
		der[(j0) * 3 + 1] = __PAR_UOP-x[(j0) * 3]-__PAR_G*(d[(j0+1)]*pow((x[(j0-1) * 3]-__PAR_UTH),2.0)-d[(j0+502)]*pow((x[(j0-1) * 3]-x[(j0) * 3]-__PAR_UTH),2.0));
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 1002:
			zc[0] = t-(d[(0)]);
			return;
		default:
			if(i0 >= 0 && i0 <= 500)
			{
				zc[0] = x[(i0) * 3]-(__PAR_UTH);
			}
			if(i0 >= 501 && i0 <= 1001)
			{
				zc[0] = x[(i0-501) * 3]-x[(i0-500) * 3]-(__PAR_UTH);
			}
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
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
			if(i0 >= 0 && i0 <= 500)
			{
				d[(i0+2)] = 1.0;
			}
			if(i0 >= 501 && i0 <= 1001)
			{
				d[(i0+2)] = 1.0;
			}
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 500)
	{
		d[(i0+2)] = 0.0;
	}
	if(i0 >= 501 && i0 <= 1001)
	{
		d[(i0+2)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[1500];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1004*sizeof(int));
	int *events = (int*)malloc(1003*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(502*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(502,1004,1003,0,0,"inverters");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_UOP = 5.0;
	__PAR_G = 100.0;
	__PAR_UTH = 1.0;
	modelData->d[(0)] = 5.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 249; i0++)
	{
		modelData->x[(2*i0+1) * 3] = 5.0;
		modelData->x[(2*i0+2) * 3] = 6.246999999999999948763207e-03;
	}
	for(i0 = 0; i0 <= 500; i0++)
	{
	if(modelData->x[(i0) * 3]>__PAR_UTH)
	{
		modelData->d[(i0+2)] = 1.0;
	}
	else
	{
		modelData->d[(i0+2)] = 0.0;
	}
	if(modelData->x[(i0) * 3]-modelData->x[(i0+1) * 3]>__PAR_UTH)
	{
		modelData->d[(i0+503)] = 1.0;
	}
	else
	{
		modelData->d[(i0+503)] = 0.0;
	}
	}
	for(i0 = 1; i0 <= 501; i0++)
	{
		modelData->nDS[i0] = 2;
	}
	for(i0 = 1; i0 <= 501; i0++)
	{
		modelData->nSD[i0-1]++;
		modelData->nSD[i0]++;
	}
	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->nZS[i0] = 1;
		modelData->nZS[i0+501] = 2;
	}
	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->nSZ[i0]++;
		modelData->nSZ[i0]++;
		modelData->nSZ[i0+1]++;
	}
	modelData->nHZ[1002] += 1;
	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->nHD[i0]++;
	}
	for(i0 = 501; i0 <= 1001; i0++)
	{
		modelData->nHD[i0]++;
	}
	modelData->nHD[1002] = 1;
	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->event[i0].nLHSDsc = 1;
		modelData->event[i0+501].nLHSDsc = 1;
	}
	modelData->event[1002].nLHSDsc = 2;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,502);

	for(i0 = 1; i0 <= 501; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0-1;
		modelData->DS[i0][states[i0]++] = i0;
	}
		cleanVector(states,0,502);

	for(i0 = 1; i0 <= 501; i0++)
	{
		modelData->SD[i0-1][states[i0-1]++] = i0;
		modelData->SD[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,1003);

	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
		modelData->ZS[i0+501][events[i0+501]++] = i0;
		modelData->ZS[i0+501][events[i0+501]++] = i0+1;
	}
		cleanVector(states,0,502);

	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
		modelData->SZ[i0][states[i0]++] = i0+501;
		modelData->SZ[i0+1][states[i0+1]++] = i0+501;
	}
		cleanVector(events,0,1003);

	modelData->HZ[1002][events[1002]++] = 1002;
		cleanVector(events,0,1003);

	modelData->HD[1002][events[1002]++] = 0;
	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0+1;
	}
	for(i0 = 501; i0 <= 1001; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0-500;
	}
		cleanVector(events,0,1003);

	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = i0+2;
		modelData->event[i0+501].LHSDsc[events[i0+501]++] = i0+503;
	}
	modelData->event[1002].LHSDsc[events[1002]++] = 0;
	modelData->event[1002].LHSDsc[events[1002]++] = 1;
		cleanVector(events,0,1003);

	for(i0 = 0; i0 <= 500; i0++)
	{
		modelData->event[i0].direction = 0;
		modelData->event[i0].relation = 2;
		modelData->event[i0+501].direction = 0;
		modelData->event[i0+501].relation = 2;
	}
	modelData->event[1002].direction = 1;
	modelData->event[1002].relation = 2;
	simulator->time = QSS_Time(502,1003,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("inverters",2,1004,502,NULL,0,0,CI_Step,SD_Memory,MOD_output);
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
