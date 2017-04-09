#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_L = 0;
double __PAR_v = 0;
double __PAR_beta = 0;
double __PAR_x0 = 0;
double __PAR_diff = 0;
double __PAR_gam = 0;
double __PAR_x[100];
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
	int j = 0;
	int j0 = 0;
	int j1 = 0;
	switch(i)
	{
		case 0:
			alg[0] = __PAR_v*__PAR_inVal;
			alg[0 + 1] = 0.0;
			alg[3] = __PAR_v*x[0];
			alg[3 + 1] = __PAR_v*x[1];
			dx[1] = -(alg[3]-alg[0])*100/__PAR_L;
			dx[2] = ((alg[1]-alg[4])*(1.0/(__PAR_L))*100)/2;
			return;
		default:
			j = i;
			if(j >=1 && j <= 99)
			{
				j0 = j;
	if (j0 >= 1 && j0 <= 100)
	{
				alg[(j) * 3] = __PAR_v*x[(j0-1) * 3];
				alg[(j) * 3 + 1] = __PAR_v*x[(j0-1) * 3 + 1];
				}
				j0 = j;
	if (j0 >= 1 && j0 <= 100)
	{
				j1 = j0+1;
	if (j0 >= 1 && j0 <= 99)
	{
				alg[(j0+1) * 3] = __PAR_v*x[(j1-1) * 3];
				alg[(j0+1) * 3 + 1] = __PAR_v*x[(j1-1) * 3 + 1];
				}
				}
				dx[1] = -(alg[(j+1) * 3]-alg[(j) * 3])*100/__PAR_L;
				dx[2] = (-(1.0/(__PAR_L))*(alg[(j+1) * 3 + 1]-alg[(j) * 3 + 1])*100)/2;
			}
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	int j = 0;
	int j0 = 0;
	int j1 = 0;
	int j2 = 0;
	int j3 = 0;
	int j4 = 0;
	int j5 = 0;
	switch(i)
	{
		case 0:
			alg[0] = __PAR_v*__PAR_inVal;
			alg[0 + 1] = 0.0;
			alg[3] = __PAR_v*x[0];
			alg[3 + 1] = __PAR_v*x[1];
			der[0 + 1] = -(alg[3]-alg[0])*100/__PAR_L;
			der[0 + 2] = ((alg[1]-alg[4])*(1.0/(__PAR_L))*100)/2;
			break;
	}
	j = i+1;
	if(j >=1 && j <= 100)
	{
		j2 = j;
	if (j2 >= 1 && j2 <= 100)
	{
		alg[(j) * 3] = __PAR_v*x[(j2-1) * 3];
		alg[(j) * 3 + 1] = __PAR_v*x[(j2-1) * 3 + 1];
		}
		j2 = j;
	if (j2 >= 1 && j2 <= 100)
	{
		j3 = j2+1;
	if (j2 >= 1 && j2 <= 99)
	{
		alg[(j+1) * 3] = __PAR_v*x[(j3-1) * 3];
		alg[(j+1) * 3 + 1] = __PAR_v*x[(j3-1) * 3 + 1];
		}
		}
		der[(j) * 3 + 1] = -(alg[(j+1) * 3]-alg[(j) * 3])*100/__PAR_L;
		der[(j) * 3 + 2] = (-(1.0/(__PAR_L))*(alg[(j+1) * 3 + 1]-alg[(j) * 3 + 1])*100)/2;
	}
	j = i;
	if(j >=1 && j <= 100)
	{
		j4 = j;
	if (j4 >= 1 && j4 <= 100)
	{
		alg[(j) * 3] = __PAR_v*x[(j4-1) * 3];
		alg[(j) * 3 + 1] = __PAR_v*x[(j4-1) * 3 + 1];
		}
		j4 = j;
	if (j4 >= 1 && j4 <= 100)
	{
		j5 = j4+1;
	if (j4 >= 1 && j4 <= 99)
	{
		alg[(j+1) * 3] = __PAR_v*x[(j5-1) * 3];
		alg[(j+1) * 3 + 1] = __PAR_v*x[(j5-1) * 3 + 1];
		}
		}
		der[(j) * 3 + 1] = -(alg[(j+1) * 3]-alg[(j) * 3])*100/__PAR_L;
		der[(j) * 3 + 2] = (-(1.0/(__PAR_L))*(alg[(j+1) * 3 + 1]-alg[(j) * 3 + 1])*100)/2;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[297];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(100*sizeof(int));
	int i6;
	int i;
	int j = 0;
	simulator->data = QSS_Data(100,0,0,0,101,"advectionFlux");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_L = 1.000000000000000000000000e+00;
	__PAR_v = 1.000000000000000000000000e+00;
	__PAR_beta = 1.000000000000000000000000e+02;
	__PAR_x0 = 5.000000000000000000000000e-01;
	__PAR_diff = 0.000000000000000000000000e+00;
	__PAR_gam = 0.000000000000000000000000e+00;
	__PAR_inVal = 0.0;
	__PAR_outVal = 1.0;
	// Initialize model code.
	for(i6 = 0; i6 <= 49; i6++)
	{
		__PAR_x[(i6)] = 1.000000000000000000000000e+00*(i6+1)/100;
		modelData->x[(i6) * 3] = 1.0;
	}
	modelData->nDS[0]++;
	for ( i = 1; i <= 99; i++) 
	{
	modelData->nDS[i]++;
	}
	for ( i = 1; i <= 99; i++) 
	{
	modelData->nDS[i]++;
	}
	modelData->nSD[0]++;
	for ( i = 1; i <= 99; i++) 
	{
	modelData->nSD[i-1]++;
	}
	for ( i = 1; i <= 99; i++) 
	{
	modelData->nSD[i]++;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,100);

	modelData->DS[0][states[0]++] = 0;
	for ( i = 1; i <= 99; i++) 
	{
	modelData->DS[i][states[i]++] = i-1;
	}
	for ( i = 1; i <= 99; i++) 
	{
	modelData->DS[i][states[i]++] = i;
	}
		cleanVector(states,0,100);

	modelData->SD[0][states[0]++] = 0;
	for ( i = 1; i <= 99; i++) 
	{
	modelData->SD[i-1][states[i-1]++] = i;
	}
	for ( i = 1; i <= 99; i++) 
	{
	modelData->SD[i][states[i]++] = i;
	}
	simulator->time = QSS_Time(100,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("advectionFlux",1,0,100,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[99]++;
	SD_allocOutputMatrix(modelOutput,100,0);
		cleanVector(states,0,100);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"u[100]");
		cleanVector(outputs,0,1);

		modelOutput->SO[99][states[99]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 99;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
