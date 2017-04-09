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
double __PAR_x[100];
double __PAR_inVal = 0;
double __PAR_outVal = 0;
double __PAR_p0[100];

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
	int j = 0;
	switch(i)
	{
		case 0:
			dx[1] = (__PAR_u0-__PAR_c0)*(-x[3]+x[0])*100;
			return;
		case 100:
			dx[1] = (__PAR_u0+__PAR_c0)*(-x[300]+x[597])*100;
			return;
		case 99:
			dx[1] = (__PAR_u0-__PAR_c0)*(-x[0]+x[297])*100;
			return;
		case 199:
			dx[1] = (__PAR_u0+__PAR_c0)*(-x[597]+x[594])*100;
			return;
		default:
			j = i;
			if(j >=1 && j <= 98)
			{
				dx[1] = (__PAR_u0-__PAR_c0)*(-x[(j+1) * 3]+x[(j) * 3])*100;
			}
			j = i-100;
			if(j >=1 && j <= 98)
			{
				dx[1] = (__PAR_u0+__PAR_c0)*(-x[(j+100) * 3]+x[(j+99) * 3])*100;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	switch(i)
	{
		case 0:
			der[0 + 1] = (__PAR_u0-__PAR_c0)*(-x[3]+x[0])*100;
			der[297 + 1] = (__PAR_u0-__PAR_c0)*(-x[0]+x[297])*100;
			return;
		case 1:
			der[0 + 1] = (__PAR_u0-__PAR_c0)*(-x[3]+x[0])*100;
			break;
		case 99:
			der[297 + 1] = (__PAR_u0-__PAR_c0)*(-x[0]+x[297])*100;
			break;
		case 100:
			der[300 + 1] = (__PAR_u0+__PAR_c0)*(-x[300]+x[597])*100;
			break;
		case 198:
			der[597 + 1] = (__PAR_u0+__PAR_c0)*(-x[597]+x[594])*100;
			break;
		case 199:
			der[300 + 1] = (__PAR_u0+__PAR_c0)*(-x[300]+x[597])*100;
			der[597 + 1] = (__PAR_u0+__PAR_c0)*(-x[597]+x[594])*100;
			return;
	}
	j = i;
	if(j >=1 && j <= 98)
	{
		der[(j) * 3 + 1] = (__PAR_u0-__PAR_c0)*(-x[(j+1) * 3]+x[(j) * 3])*100;
	}
	j = i-1;
	if(j >=1 && j <= 98)
	{
		der[(j) * 3 + 1] = (__PAR_u0-__PAR_c0)*(-x[(j+1) * 3]+x[(j) * 3])*100;
	}
	j = i-99;
	if(j >=1 && j <= 98)
	{
		der[(j+100) * 3 + 1] = (__PAR_u0+__PAR_c0)*(-x[(j+100) * 3]+x[(j+99) * 3])*100;
	}
	j = i-100;
	if(j >=1 && j <= 98)
	{
		der[(j+100) * 3 + 1] = (__PAR_u0+__PAR_c0)*(-x[(j+100) * 3]+x[(j+99) * 3])*100;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 1)
	{
		out[0] = x[(50*j) * 3];
	}
	j = i-2;
	if(j >=0 && j <= 1)
	{
		out[0] = x[(50*j+100) * 3];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(4*sizeof(int));
	int *states = (int*)malloc(200*sizeof(int));
	int i1;
	int i;
	int j = 0;
	simulator->data = QSS_Data(200,0,0,0,0,"acousticsRiemann");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_K0 = 1.000000000000000000000000e+00;
	__PAR_rho0 = 1.000000000000000000000000e+00;
	__PAR_u0 = 0.0;
	__PAR_beta = 100.0;
	__PAR_x0 = 5.000000000000000000000000e-01;
	__PAR_gam = 0.000000000000000000000000e+00;
	__PAR_inVal = 0.0;
	__PAR_outVal = 1.0;
	// Initialize model code.
		__PAR_c0 = sqrt(__PAR_K0/__PAR_rho0);
		__PAR_Z0 = __PAR_rho0*__PAR_c0;
	for(i1 = 0; i1 <= 99; i1++)
	{
		__PAR_x[(i1)] = 1.000000000000000000000000e+00*(i1+1)/100;
		__PAR_p0[(i1)] = pow(2.717999999999999971578291e+00,(-__PAR_beta*pow((__PAR_x[(i1)]-__PAR_x0),2.0)))*cos(__PAR_gam*(__PAR_x[(i1)]-__PAR_x0));
		modelData->x[(i1) * 3] = 1.0/2.0/__PAR_Z0*(-__PAR_p0[(i1)]);
		modelData->x[(i1+100) * 3] = 1.0/2.0/__PAR_Z0*__PAR_p0[(i1)];
	}
	modelData->nDS[0] = 2;
	modelData->nDS[100] = 2;
	for(i = 1; i <= 98; i++)
	{
		modelData->nDS[i] = 2;
		modelData->nDS[i+100] = 2;
	}
	modelData->nDS[99] = 2;
	modelData->nDS[199] = 2;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[100]++;
	modelData->nSD[199]++;
	for(i = 1; i <= 98; i++)
	{
		modelData->nSD[i]++;
		modelData->nSD[i+1]++;
		modelData->nSD[i+99]++;
		modelData->nSD[i+100]++;
	}
	modelData->nSD[0]++;
	modelData->nSD[99]++;
	modelData->nSD[198]++;
	modelData->nSD[199]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,200);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[100][states[100]++] = 100;
	modelData->DS[100][states[100]++] = 199;
	for(i = 1; i <= 98; i++)
	{
		modelData->DS[i][states[i]++] = i;
		modelData->DS[i][states[i]++] = i+1;
		modelData->DS[i+100][states[i+100]++] = i+99;
		modelData->DS[i+100][states[i+100]++] = i+100;
	}
	modelData->DS[99][states[99]++] = 0;
	modelData->DS[99][states[99]++] = 99;
	modelData->DS[199][states[199]++] = 198;
	modelData->DS[199][states[199]++] = 199;
		cleanVector(states,0,200);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[100][states[100]++] = 100;
	modelData->SD[199][states[199]++] = 100;
	for(i = 1; i <= 98; i++)
	{
		modelData->SD[i][states[i]++] = i;
		modelData->SD[i+1][states[i+1]++] = i;
		modelData->SD[i+99][states[i+99]++] = i+100;
		modelData->SD[i+100][states[i+100]++] = i+100;
	}
	modelData->SD[0][states[0]++] = 99;
	modelData->SD[99][states[99]++] = 99;
	modelData->SD[198][states[198]++] = 199;
	modelData->SD[199][states[199]++] = 199;
	simulator->time = QSS_Time(200,0,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.1;
	simulator->output = SD_Output("acousticsRiemann",4,0,200,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

	for(i = 0; i <= 1; i++)
	{
		modelOutput->nOS[i] = 1;
		modelOutput->nSO[50*i]++;
		modelOutput->nOS[i+2] = 1;
		modelOutput->nSO[50*i+100]++;
	}
	SD_allocOutputMatrix(modelOutput,200,0);
		cleanVector(states,0,200);

		cleanVector(outputs,0,4);

	for(i = 0; i <= 1; i++)
	{
		sprintf(modelOutput->variable[i].name,"w1[%d]",50*i+1);
		sprintf(modelOutput->variable[i+2].name,"w2[%d]",50*i+1);
	}
		cleanVector(outputs,0,4);

	for(i = 0; i <= 1; i++)
	{
		modelOutput->SO[50*i][states[50*i]++] = i;
		modelOutput->OS[i][outputs[i]++] = 50*i;
		modelOutput->SO[50*i+100][states[50*i+100]++] = i+2;
		modelOutput->OS[i+2][outputs[i+2]++] = 50*i+100;
	}
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
