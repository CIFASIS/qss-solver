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
double __PAR_u = 0;
double __PAR_qBC = 0;
double __PAR_x0 = 0;
double __PAR_x[10000];
double __PAR_beta = 0;

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
			dx[1] = __PAR_u*(-x[0]+x[29997])*10000+__PAR_beta*(x[3]-2.0*x[0]+x[29997])*pow(10000,2.0);
			return;
		case 9999:
			dx[1] = __PAR_u*(-x[29997]+x[29994])*10000+__PAR_beta*(x[0]-2.0*x[29997]+x[29994])*pow(10000,2.0);
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 9998)
			{
				dx[1] = __PAR_u*(-x[(j0) * 3]+x[(j0-1) * 3])*10000+__PAR_beta*(x[(j0+1) * 3]-2.0*x[(j0) * 3]+x[(j0-1) * 3])*pow(10000,2.0);
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
			der[0 + 1] = __PAR_u*(-x[0]+x[29997])*10000+__PAR_beta*(x[3]-2.0*x[0]+x[29997])*pow(10000,2.0);
			der[29997 + 1] = __PAR_u*(-x[29997]+x[29994])*10000+__PAR_beta*(x[0]-2.0*x[29997]+x[29994])*pow(10000,2.0);
			break;
		case 1:
			der[0 + 1] = __PAR_u*(-x[0]+x[29997])*10000+__PAR_beta*(x[3]-2.0*x[0]+x[29997])*pow(10000,2.0);
			break;
		case 9998:
			der[29997 + 1] = __PAR_u*(-x[29997]+x[29994])*10000+__PAR_beta*(x[0]-2.0*x[29997]+x[29994])*pow(10000,2.0);
			break;
		case 9999:
			der[0 + 1] = __PAR_u*(-x[0]+x[29997])*10000+__PAR_beta*(x[3]-2.0*x[0]+x[29997])*pow(10000,2.0);
			der[29997 + 1] = __PAR_u*(-x[29997]+x[29994])*10000+__PAR_beta*(x[0]-2.0*x[29997]+x[29994])*pow(10000,2.0);
			break;
	}
	j0 = i0+1;
	if(j0 >=1 && j0 <= 9998)
	{
		der[(j0) * 3 + 1] = __PAR_u*(-x[(j0) * 3]+x[(j0-1) * 3])*10000+__PAR_beta*(x[(j0+1) * 3]-2.0*x[(j0) * 3]+x[(j0-1) * 3])*pow(10000,2.0);
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 9998)
	{
		der[(j0) * 3 + 1] = __PAR_u*(-x[(j0) * 3]+x[(j0-1) * 3])*10000+__PAR_beta*(x[(j0+1) * 3]-2.0*x[(j0) * 3]+x[(j0-1) * 3])*pow(10000,2.0);
	}
	j0 = i0-1;
	if(j0 >=1 && j0 <= 9998)
	{
		der[(j0) * 3 + 1] = __PAR_u*(-x[(j0) * 3]+x[(j0-1) * 3])*10000+__PAR_beta*(x[(j0+1) * 3]-2.0*x[(j0) * 3]+x[(j0-1) * 3])*pow(10000,2.0);
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[14997];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(10000*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(10000,0,0,0,0,"advectionUpwind");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_alpha = 100.0;
	__PAR_u = 1.000000000000000000000000e+00;
	__PAR_qBC = 1.000000000000000000000000e+00;
	__PAR_x0 = 5.000000000000000000000000e-01;
	__PAR_beta = 0.000000000000000000000000e+00;
	// Initialize model code.
	for(i0 = 0; i0 <= 9999; i0++)
	{
		__PAR_x[(i0)] = 1.000000000000000000000000e+00*(i0+1)/10000;
		modelData->x[(i0) * 3] = pow(2.717999999999999971578291e+00,(-__PAR_alpha*pow((__PAR_x[(i0)]-__PAR_x0),2.0)));
	}
	modelData->nDS[0] = 3;
	modelData->nDS[9999] = 3;
	for(i0 = 1; i0 <= 9998; i0++)
	{
		modelData->nDS[i0] = 3;
	}
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[9999]++;
	modelData->nSD[0]++;
	modelData->nSD[9998]++;
	modelData->nSD[9999]++;
	for(i0 = 1; i0 <= 9998; i0++)
	{
		modelData->nSD[i0-1]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+1]++;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,10000);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[0][states[0]++] = 9999;
	modelData->DS[9999][states[9999]++] = 0;
	modelData->DS[9999][states[9999]++] = 9998;
	modelData->DS[9999][states[9999]++] = 9999;
	for(i0 = 1; i0 <= 9998; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0-1;
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+1;
	}
		cleanVector(states,0,10000);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[9999][states[9999]++] = 0;
	modelData->SD[0][states[0]++] = 9999;
	modelData->SD[9998][states[9998]++] = 9999;
	modelData->SD[9999][states[9999]++] = 9999;
	for(i0 = 1; i0 <= 9998; i0++)
	{
		modelData->SD[i0-1][states[i0-1]++] = i0;
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+1][states[i0+1]++] = i0;
	}
	simulator->time = QSS_Time(10000,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("advectionUpwind",1,0,10000,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[4999]++;
	SD_allocOutputMatrix(modelOutput,10000,0);
		cleanVector(states,0,10000);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"q[5000]");
		cleanVector(outputs,0,1);

		modelOutput->SO[4999][states[4999]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 4999;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
