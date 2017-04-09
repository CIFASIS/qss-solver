#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_beta = 0;
double __PAR_x0 = 0;
double __PAR_pi = 0;
double __PAR_x[1000];
double __PAR_inVal = 0;
double __PAR_outVal = 0;

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
	int j = 0;
	switch(i)
	{
		case 0:
			dx[1] = d[0]*(x[0]+x[2997])/2.0*(-x[0]+x[2997])*1000+(1.0-d[0])*(x[3]+x[0])/2.0*(-x[3]+x[0])*1000;
			return;
		case 999:
			dx[1] = d[999]*(x[2997]+x[2994])/2.0*(-x[2997]+x[2994])*1000+(1.0-d[999])*(x[0]+x[2997])*(-x[0]+x[2997])*1000;
			return;
		default:
			j = i;
			if(j >=1 && j <= 998)
			{
				dx[1] = d[(j)]*(x[(j) * 3]+x[(j-1) * 3])/2.0*(-x[(j) * 3]+x[(j-1) * 3])*1000+(1.0-d[(j)])*(x[(j+1) * 3]+x[(j) * 3])*(-x[(j+1) * 3]+x[(j) * 3])*1000;
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
			der[0 + 1] = d[0]*(x[0]+x[2997])/2.0*(-x[0]+x[2997])*1000+(1.0-d[0])*(x[3]+x[0])/2.0*(-x[3]+x[0])*1000;
			der[2997 + 1] = d[999]*(x[2997]+x[2994])/2.0*(-x[2997]+x[2994])*1000+(1.0-d[999])*(x[0]+x[2997])*(-x[0]+x[2997])*1000;
			break;
		case 1:
			der[0 + 1] = d[0]*(x[0]+x[2997])/2.0*(-x[0]+x[2997])*1000+(1.0-d[0])*(x[3]+x[0])/2.0*(-x[3]+x[0])*1000;
			break;
		case 998:
			der[2997 + 1] = d[999]*(x[2997]+x[2994])/2.0*(-x[2997]+x[2994])*1000+(1.0-d[999])*(x[0]+x[2997])*(-x[0]+x[2997])*1000;
			break;
		case 999:
			der[0 + 1] = d[0]*(x[0]+x[2997])/2.0*(-x[0]+x[2997])*1000+(1.0-d[0])*(x[3]+x[0])/2.0*(-x[3]+x[0])*1000;
			der[2997 + 1] = d[999]*(x[2997]+x[2994])/2.0*(-x[2997]+x[2994])*1000+(1.0-d[999])*(x[0]+x[2997])*(-x[0]+x[2997])*1000;
			break;
	}
	j = i+1;
	if(j >=1 && j <= 998)
	{
		der[(j) * 3 + 1] = d[(j)]*(x[(j) * 3]+x[(j-1) * 3])/2.0*(-x[(j) * 3]+x[(j-1) * 3])*1000+(1.0-d[(j)])*(x[(j+1) * 3]+x[(j) * 3])*(-x[(j+1) * 3]+x[(j) * 3])*1000;
	}
	j = i;
	if(j >=1 && j <= 998)
	{
		der[(j) * 3 + 1] = d[(j)]*(x[(j) * 3]+x[(j-1) * 3])/2.0*(-x[(j) * 3]+x[(j-1) * 3])*1000+(1.0-d[(j)])*(x[(j+1) * 3]+x[(j) * 3])*(-x[(j+1) * 3]+x[(j) * 3])*1000;
	}
	j = i-1;
	if(j >=1 && j <= 998)
	{
		der[(j) * 3 + 1] = d[(j)]*(x[(j) * 3]+x[(j-1) * 3])/2.0*(-x[(j) * 3]+x[(j-1) * 3])*1000+(1.0-d[(j)])*(x[(j+1) * 3]+x[(j) * 3])*(-x[(j+1) * 3]+x[(j) * 3])*1000;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	if(i >= 0 && i <= 999)
	{
		zc[0] = x[(i) * 3]-(0.0);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 999)
	{
		d[(i)] = 1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	if(i >= 0 && i <= 999)
	{
		d[(i)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int j = 0;
	j = i;
	if(j >=0 && j <= 4)
	{
		out[0] = x[(200*j) * 3];
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(1000*sizeof(int));
	int *events = (int*)malloc(1000*sizeof(int));
	int *outputs = (int*)malloc(5*sizeof(int));
	int *states = (int*)malloc(1000*sizeof(int));
	int i0;
	int i;
	int j = 0;
	simulator->data = QSS_Data(1000,1000,1000,0,0,"burgers");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_beta = 100.0;
	__PAR_x0 = 5.000000000000000000000000e-01;
	__PAR_pi = 3.141589999999999882618340e+00;
	__PAR_inVal = 1.0;
	__PAR_outVal = 1.0;
	modelData->d[(0)] = 0.0;
	// Initialize model code.
	for(i0 = 0; i0 <= 999; i0++)
	{
		__PAR_x[(i0)] = 1.000000000000000000000000e+00*(i0+1)/1000;
		modelData->x[(i0) * 3] = sin(__PAR_pi*2.0*__PAR_x[(i0)]);
	if(modelData->x[(i0) * 3]>0.0)
	{
		modelData->d[(i0)] = 1.0;
	}
	else
	{
		modelData->d[(i0)] = 0.0;
	}
	}
	modelData->nDS[0] = 3;
	for(i = 1; i <= 998; i++)
	{
		modelData->nDS[i] = 3;
	}
	modelData->nDS[999] = 3;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[999]++;
	for(i = 1; i <= 998; i++)
	{
		modelData->nSD[i-1]++;
		modelData->nSD[i]++;
		modelData->nSD[i+1]++;
	}
	modelData->nSD[0]++;
	modelData->nSD[998]++;
	modelData->nSD[999]++;
	for(i = 0; i <= 999; i++)
	{
		modelData->nZS[i] = 1;
	}
	for(i = 0; i <= 999; i++)
	{
		modelData->nSZ[i]++;
	}
	modelData->nHD[0]++;
	for(i = 1; i <= 998; i++)
	{
		modelData->nHD[i]++;
	}
	modelData->nHD[999]++;
	for(i = 0; i <= 999; i++)
	{
		modelData->event[i].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,1000);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[0][states[0]++] = 999;
	for(i = 1; i <= 998; i++)
	{
		modelData->DS[i][states[i]++] = i-1;
		modelData->DS[i][states[i]++] = i;
		modelData->DS[i][states[i]++] = i+1;
	}
	modelData->DS[999][states[999]++] = 0;
	modelData->DS[999][states[999]++] = 998;
	modelData->DS[999][states[999]++] = 999;
		cleanVector(states,0,1000);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[999][states[999]++] = 0;
	for(i = 1; i <= 998; i++)
	{
		modelData->SD[i-1][states[i-1]++] = i;
		modelData->SD[i][states[i]++] = i;
		modelData->SD[i+1][states[i+1]++] = i;
	}
	modelData->SD[0][states[0]++] = 999;
	modelData->SD[998][states[998]++] = 999;
	modelData->SD[999][states[999]++] = 999;
		cleanVector(events,0,1000);

	for(i = 0; i <= 999; i++)
	{
		modelData->ZS[i][events[i]++] = i;
	}
		cleanVector(states,0,1000);

	for(i = 0; i <= 999; i++)
	{
		modelData->SZ[i][states[i]++] = i;
	}
		cleanVector(events,0,1000);

	modelData->HD[0][events[0]++] = 0;
	for(i = 1; i <= 998; i++)
	{
		modelData->HD[i][events[i]++] = i;
	}
	modelData->HD[999][events[999]++] = 999;
		cleanVector(events,0,1000);

	for(i = 0; i <= 999; i++)
	{
		modelData->event[i].LHSDsc[events[i]++] = i;
	}
		cleanVector(events,0,1000);

	for(i = 0; i <= 999; i++)
	{
		modelData->event[i].direction = 0;
		modelData->event[i].relation = 2;
	}
	simulator->time = QSS_Time(1000,1000,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.01;
	simulator->output = SD_Output("burgers",5,1000,1000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

	for(i = 0; i <= 4; i++)
	{
		modelOutput->nOS[i] = 1;
		modelOutput->nSO[200*i]++;
	}
	SD_allocOutputMatrix(modelOutput,1000,1000);
		cleanVector(states,0,1000);

		cleanVector(outputs,0,5);

	for(i = 0; i <= 4; i++)
	{
		sprintf(modelOutput->variable[i].name,"u[%d]",200*i+1);
	}
		cleanVector(outputs,0,5);

	for(i = 0; i <= 4; i++)
	{
		modelOutput->SO[200*i][states[200*i]++] = i;
		modelOutput->OS[i][outputs[i]++] = 200*i;
	}
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
