#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>
static CLC_data modelData = NULL;


void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = FALSE;
	 settings->method = 8;
}

void
MOD_definition(double *x, double *d, double *alg, double t, double *dx)
{
	modelData->scalarEvaluations++;
		dx[0] = 1.000000000000000055511151e-01*x[0]-1.000000000000000055511151e-01*x[0]*x[1];
		dx[1] = 1.000000000000000055511151e-01*x[0]*x[1]-1.000000000000000055511151e-01*x[1];
}

void
MOD_jacobian(double *x, double *d, double *alg, double t, double *jac)
{
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 1)
	{
		out[0] = x[(j0) * 1];
	}
}

void
CLC_initializeDataStructs(CLC_simulator simulator)
{
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0 = 0;
	simulator->data = CLC_Data(2,0,0,0,0,"lotka_volterraDASSL");
  modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	// Initialize model code.
		modelData->x[0] = 5.000000000000000000000000e-01;
		modelData->x[1] = 5.000000000000000000000000e-01;
	CLC_allocDataMatrix(modelData);
		double period[1];
	period[0] = 0.06;
	simulator->output = SD_Output("lotka_volterraDASSL",2,0,2,period,1,0,CI_Sampled,SD_Memory,MOD_output);
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
	simulator->model = CLC_Model(MOD_definition,NULL,NULL,NULL, MOD_jacobian);
	free(outputs);
	free(states);
}

void
QSS_initializeDataStructs (QSS_simulator simulator)
{
}
