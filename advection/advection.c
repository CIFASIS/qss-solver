#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_alpha = 0;
double __PAR_mu = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = FALSE;
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
			dx[1] = (-x[0]+1.0)*1000-__PAR_mu*x[0]*(x[0]-__PAR_alpha)*(x[0]-1.0);
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 999)
			{
				dx[1] = (-x[(j0) * 3]+x[(j0-1) * 3])*1000-__PAR_mu*x[(j0) * 3]*(x[(j0) * 3]-__PAR_alpha)*(x[(j0) * 3]-1.0);
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
		case 0:
			der[0 + 1] = (-x[0]+1.0)*1000-__PAR_mu*x[0]*(x[0]-__PAR_alpha)*(x[0]-1.0);
			break;
	}
	j0 = i0+1;
	if(j0 >=1 && j0 <= 999)
	{
		der[(j0) * 3 + 1] = (-x[(j0) * 3]+x[(j0-1) * 3])*1000-__PAR_mu*x[(j0) * 3]*(x[(j0) * 3]-__PAR_alpha)*(x[(j0) * 3]-1.0);
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 999)
	{
		der[(j0) * 3 + 1] = (-x[(j0) * 3]+x[(j0-1) * 3])*1000-__PAR_mu*x[(j0) * 3]*(x[(j0) * 3]-__PAR_alpha)*(x[(j0) * 3]-1.0);
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[2997];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(1000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(1000,0,0,0,0,"advection");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_alpha = 5.000000000000000000000000e-01;
	__PAR_mu = 1000.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 332; i0++)
	{
		modelData->x[(i0) * 3] = 1.0;
	}
	modelData->nDS[0] = 1;
	for(i0 = 1; i0 <= 999; i0++)
	{
		modelData->nDS[i0] = 2;
	}
	modelData->nSD[0]++;
	for(i0 = 1; i0 <= 999; i0++)
	{
		modelData->nSD[i0-1]++;
		modelData->nSD[i0]++;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,1000);

	modelData->DS[0][states[0]++] = 0;
	for(i0 = 1; i0 <= 999; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0-1;
		modelData->DS[i0][states[i0]++] = i0;
	}
		cleanVector(states,0,1000);

	modelData->SD[0][states[0]++] = 0;
	for(i0 = 1; i0 <= 999; i0++)
	{
		modelData->SD[i0-1][states[i0-1]++] = i0;
		modelData->SD[i0][states[i0]++] = i0;
	}
	simulator->time = QSS_Time(1000,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("advection",1,0,1000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[999]++;
	SD_allocOutputMatrix(modelOutput,1000,0);
		cleanVector(states,0,1000);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"u[1000]");
		cleanVector(outputs,0,1);

		modelOutput->SO[999][states[999]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 999;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
