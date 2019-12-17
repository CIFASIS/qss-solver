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
	 settings->hybrid = FALSE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			dx[1] = 1.000000000000000055511151e-01*x[0]-1.000000000000000055511151e-01*x[0]*x[4];
			dx[2] = (-(1.000000000000000055511151e-01)*x[4]*x[1]-(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1])/2;
			dx[3] = ((1.000000000000000055511151e-01)*x[2]*2-(1.000000000000000055511151e-01)*x[0]*x[6]*2-(2.000000000000000111022302e-01)*x[1]*x[5]-(1.000000000000000055511151e-01)*x[2]*2*x[4])/6;
			return;
		case 1:
			dx[1] = 1.000000000000000055511151e-01*x[0]*x[4]-1.000000000000000055511151e-01*x[4];
			dx[2] = (-(1.000000000000000055511151e-01)*x[5]+(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1]*x[4])/2;
			dx[3] = ((1.000000000000000055511151e-01)*x[6]*2*x[0]+(2.000000000000000111022302e-01)*x[1]*x[5]+(1.000000000000000055511151e-01)*x[4]*x[2]*2-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			der[0 + 1] = 1.000000000000000055511151e-01*x[0]-1.000000000000000055511151e-01*x[0]*x[4];
			der[0 + 2] = (-(1.000000000000000055511151e-01)*x[4]*x[1]-(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1])/2;
			der[0 + 3] = ((1.000000000000000055511151e-01)*x[2]*2-(1.000000000000000055511151e-01)*x[0]*x[6]*2-(2.000000000000000111022302e-01)*x[1]*x[5]-(1.000000000000000055511151e-01)*x[2]*2*x[4])/6;
			der[4 + 1] = 1.000000000000000055511151e-01*x[0]*x[4]-1.000000000000000055511151e-01*x[4];
			der[4 + 2] = (-(1.000000000000000055511151e-01)*x[5]+(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1]*x[4])/2;
			der[4 + 3] = ((1.000000000000000055511151e-01)*x[6]*2*x[0]+(2.000000000000000111022302e-01)*x[1]*x[5]+(1.000000000000000055511151e-01)*x[4]*x[2]*2-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
		case 1:
			der[0 + 1] = 1.000000000000000055511151e-01*x[0]-1.000000000000000055511151e-01*x[0]*x[4];
			der[0 + 2] = (-(1.000000000000000055511151e-01)*x[4]*x[1]-(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1])/2;
			der[0 + 3] = ((1.000000000000000055511151e-01)*x[2]*2-(1.000000000000000055511151e-01)*x[0]*x[6]*2-(2.000000000000000111022302e-01)*x[1]*x[5]-(1.000000000000000055511151e-01)*x[2]*2*x[4])/6;
			der[4 + 1] = 1.000000000000000055511151e-01*x[0]*x[4]-1.000000000000000055511151e-01*x[4];
			der[4 + 2] = (-(1.000000000000000055511151e-01)*x[5]+(1.000000000000000055511151e-01)*x[5]*x[0]+(1.000000000000000055511151e-01)*x[1]*x[4])/2;
			der[4 + 3] = ((1.000000000000000055511151e-01)*x[6]*2*x[0]+(2.000000000000000111022302e-01)*x[1]*x[5]+(1.000000000000000055511151e-01)*x[4]*x[2]*2-(1.000000000000000055511151e-01)*x[6]*2)/6;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		out[0] = x[(j0) * 4];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0;
	simulator->data = QSS_Data(2,0,0,0,0,"lotka_volterra");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	// Initialize model code.
		modelData->x[0] = 5.000000000000000000000000e-01;
		modelData->x[4] = 5.000000000000000000000000e-01;
	modelData->nDS[0] = 2;
	modelData->nDS[1] = 2;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
		cleanVector(states,0,2);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
	simulator->time = QSS_Time(2,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("lotka_volterra",2,0,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[i0]++;
	}
	SD_allocOutputMatrix(modelOutput,2,0);
		cleanVector(states,0,2);

		cleanVector(outputs,0,2);

	for(i0 = 0; i0 <= 1; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"x[%d]",i0+1);
	}
		cleanVector(outputs,0,2);

	for(i0 = 0; i0 <= 1; i0++)
	{
		modelOutput->SO[i0][states[i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = i0;
	}
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
