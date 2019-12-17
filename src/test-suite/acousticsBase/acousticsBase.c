#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_K0 = 0;
double __PAR_rho0 = 0;
double __PAR_c0 = 0;
double __PAR_Z0 = 0;
double __PAR_u0 = 0;
double __PAR_beta = 0;
double __PAR_x0 = 0;
double __PAR_gam = 0;
double __PAR_x[200];
double __PAR_inVal = 0;
double __PAR_outVal = 0;

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
			dx[1] = __PAR_K0*(-x[600]+x[1197])*200;
			dx[2] = ((x[1198]-x[601])*200*__PAR_K0)/2;
			return;
		case 200:
			dx[1] = 1.0/__PAR_rho0*(-x[3]+x[0])*200;
			dx[2] = ((1.0/(__PAR_rho0))*(x[1]-x[4])*200)/2;
			return;
		case 199:
			dx[1] = __PAR_K0*(-x[1197]+x[1194])*200;
			dx[2] = (__PAR_K0*(x[1195]-x[1198])*200)/2;
			return;
		case 399:
			dx[1] = 1.0/__PAR_rho0*(-x[0]+x[597])*200;
			dx[2] = (-(x[1]-x[598])*200*(1.0/(__PAR_rho0)))/2;
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = __PAR_K0*(-x[(j0+200) * 3]+x[(j0+199) * 3])*200;
				dx[2] = (-(x[(j0+200) * 3 + 1]-x[(j0+199) * 3 + 1])*200*__PAR_K0)/2;
			}
			j0 = i0-200;
			if(j0 >=1 && j0 <= 198)
			{
				dx[1] = 1.0/__PAR_rho0*(-x[(j0+1) * 3]+x[(j0) * 3])*200;
				dx[2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 3 + 1]-x[(j0+1) * 3 + 1]))/2;
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
			der[600 + 1] = 1.0/__PAR_rho0*(-x[3]+x[0])*200;
			der[600 + 2] = ((1.0/(__PAR_rho0))*(x[1]-x[4])*200)/2;
			der[1197 + 1] = 1.0/__PAR_rho0*(-x[0]+x[597])*200;
			der[1197 + 2] = (-(x[1]-x[598])*200*(1.0/(__PAR_rho0)))/2;
			return;
		case 1:
			der[600 + 1] = 1.0/__PAR_rho0*(-x[3]+x[0])*200;
			der[600 + 2] = ((1.0/(__PAR_rho0))*(x[1]-x[4])*200)/2;
			break;
		case 199:
			der[1197 + 1] = 1.0/__PAR_rho0*(-x[0]+x[597])*200;
			der[1197 + 2] = (-(x[1]-x[598])*200*(1.0/(__PAR_rho0)))/2;
			break;
		case 200:
			der[0 + 1] = __PAR_K0*(-x[600]+x[1197])*200;
			der[0 + 2] = ((x[1198]-x[601])*200*__PAR_K0)/2;
			break;
		case 398:
			der[597 + 1] = __PAR_K0*(-x[1197]+x[1194])*200;
			der[597 + 2] = (__PAR_K0*(x[1195]-x[1198])*200)/2;
			break;
		case 399:
			der[0 + 1] = __PAR_K0*(-x[600]+x[1197])*200;
			der[0 + 2] = ((x[1198]-x[601])*200*__PAR_K0)/2;
			der[597 + 1] = __PAR_K0*(-x[1197]+x[1194])*200;
			der[597 + 2] = (__PAR_K0*(x[1195]-x[1198])*200)/2;
			return;
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+200) * 3 + 1] = 1.0/__PAR_rho0*(-x[(j0+1) * 3]+x[(j0) * 3])*200;
		der[(j0+200) * 3 + 2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 3 + 1]-x[(j0+1) * 3 + 1]))/2;
	}
	j0 = i0-1;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0+200) * 3 + 1] = 1.0/__PAR_rho0*(-x[(j0+1) * 3]+x[(j0) * 3])*200;
		der[(j0+200) * 3 + 2] = (200*(1.0/(__PAR_rho0))*(x[(j0) * 3 + 1]-x[(j0+1) * 3 + 1]))/2;
	}
	j0 = i0-199;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 3 + 1] = __PAR_K0*(-x[(j0+200) * 3]+x[(j0+199) * 3])*200;
		der[(j0) * 3 + 2] = (-(x[(j0+200) * 3 + 1]-x[(j0+199) * 3 + 1])*200*__PAR_K0)/2;
	}
	j0 = i0-200;
	if(j0 >=1 && j0 <= 198)
	{
		der[(j0) * 3 + 1] = __PAR_K0*(-x[(j0+200) * 3]+x[(j0+199) * 3])*200;
		der[(j0) * 3 + 2] = (-(x[(j0+200) * 3 + 1]-x[(j0+199) * 3 + 1])*200*__PAR_K0)/2;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
	case 5:
			out[0] = x[897];
			return;
		default:
			j0 = i0;
			if(j0 >=0 && j0 <= 4)
			{
				out[0] = x[(40*j0) * 3];
			}
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(6*sizeof(int));
	int *states = (int*)malloc(400*sizeof(int));
	int i0;
	int j0 = 0;
	simulator->data = QSS_Data(400,0,0,0,0,"acousticsBase");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_K0 = 1.000000000000000000000000e+00;
	__PAR_rho0 = 1.000000000000000000000000e+00;
	__PAR_u0 = 0.0;
	__PAR_beta = 1000.0;
	__PAR_x0 = 2.500000000000000000000000e-01;
	__PAR_gam = 0.000000000000000000000000e+00;
	__PAR_inVal = 0.0;
	__PAR_outVal = 1.0;
	for(i0 = 0; i0 <= 199;i0++)
	{
		modelData->x[(i0+200) * 3] = 0.000000000000000000000000e+00;
	}
	// Initialize model code.
	for(i0 = 0; i0 <= 199; i0++)
	{
		__PAR_x[(i0)] = 1.000000000000000000000000e+00*(i0+1)/200;
		modelData->x[(i0) * 3] = pow(2.717999999999999971578291e+00,(-__PAR_beta*pow((__PAR_x[(i0)]-__PAR_x0),2.0)));
	}
		__PAR_c0 = sqrt(__PAR_K0/__PAR_rho0);
		__PAR_Z0 = __PAR_rho0*__PAR_c0;
	modelData->nDS[0] = 2;
	modelData->nDS[200] = 2;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nDS[i0] = 2;
		modelData->nDS[i0+200] = 2;
	}
	modelData->nDS[199] = 2;
	modelData->nDS[399] = 2;
	modelData->nSD[200]++;
	modelData->nSD[399]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->nSD[i0+199]++;
		modelData->nSD[i0+200]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+1]++;
	}
	modelData->nSD[398]++;
	modelData->nSD[399]++;
	modelData->nSD[0]++;
	modelData->nSD[199]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,400);

	modelData->DS[0][states[0]++] = 200;
	modelData->DS[0][states[0]++] = 399;
	modelData->DS[200][states[200]++] = 0;
	modelData->DS[200][states[200]++] = 1;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0+199;
		modelData->DS[i0][states[i0]++] = i0+200;
		modelData->DS[i0+200][states[i0+200]++] = i0;
		modelData->DS[i0+200][states[i0+200]++] = i0+1;
	}
	modelData->DS[199][states[199]++] = 398;
	modelData->DS[199][states[199]++] = 399;
	modelData->DS[399][states[399]++] = 0;
	modelData->DS[399][states[399]++] = 199;
		cleanVector(states,0,400);

	modelData->SD[200][states[200]++] = 0;
	modelData->SD[399][states[399]++] = 0;
	modelData->SD[0][states[0]++] = 200;
	modelData->SD[1][states[1]++] = 200;
	for(i0 = 1; i0 <= 198; i0++)
	{
		modelData->SD[i0+199][states[i0+199]++] = i0;
		modelData->SD[i0+200][states[i0+200]++] = i0;
		modelData->SD[i0][states[i0]++] = i0+200;
		modelData->SD[i0+1][states[i0+1]++] = i0+200;
	}
	modelData->SD[398][states[398]++] = 199;
	modelData->SD[399][states[399]++] = 199;
	modelData->SD[0][states[0]++] = 399;
	modelData->SD[199][states[199]++] = 399;
	simulator->time = QSS_Time(400,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("acousticsBase",6,0,400,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[40*i0]++;
	}
	modelOutput->nOS[5] = 1;
	modelOutput->nSO[299]++;
	SD_allocOutputMatrix(modelOutput,400,0);
		cleanVector(states,0,400);

		cleanVector(outputs,0,6);

	for(i0 = 0; i0 <= 4; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"p[%d]",40*i0+1);
	}
	sprintf(modelOutput->variable[5].name,"u[100]");
		cleanVector(outputs,0,6);

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->SO[40*i0][states[40*i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = 40*i0;
	}
	modelOutput->SO[299][states[299]++] = 5;
	modelOutput->OS[5][outputs[5]++] = 299;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
