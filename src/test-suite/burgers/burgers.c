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
	int i0 = i;
	int j0 = 0;
	switch(i)
	{
		case 0:
			dx[1] = d[0]*(x[0]+x[2997])/2.0*(-x[0]+x[2997])*1000+(1.0-d[0])*(x[3]+x[0])/2.0*(-x[3]+x[0])*1000;
			return;
		case 999:
			dx[1] = d[999]*(x[2997]+x[2994])/2.0*(-x[2997]+x[2994])*1000+(1.0-d[999])*(x[0]+x[2997])*(-x[0]+x[2997])*1000;
			return;
		default:
			j0 = i0;
			if(j0 >=1 && j0 <= 998)
			{
				dx[1] = d[(j0)]*(x[(j0) * 3]+x[(j0-1) * 3])/2.0*(-x[(j0) * 3]+x[(j0-1) * 3])*1000+(1.0-d[(j0)])*(x[(j0+1) * 3]+x[(j0) * 3])*(-x[(j0+1) * 3]+x[(j0) * 3])*1000;
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
	j0 = i0+1;
	if(j0 >=1 && j0 <= 998)
	{
		der[(j0) * 3 + 1] = d[(j0)]*(x[(j0) * 3]+x[(j0-1) * 3])/2.0*(-x[(j0) * 3]+x[(j0-1) * 3])*1000+(1.0-d[(j0)])*(x[(j0+1) * 3]+x[(j0) * 3])*(-x[(j0+1) * 3]+x[(j0) * 3])*1000;
	}
	j0 = i0;
	if(j0 >=1 && j0 <= 998)
	{
		der[(j0) * 3 + 1] = d[(j0)]*(x[(j0) * 3]+x[(j0-1) * 3])/2.0*(-x[(j0) * 3]+x[(j0-1) * 3])*1000+(1.0-d[(j0)])*(x[(j0+1) * 3]+x[(j0) * 3])*(-x[(j0+1) * 3]+x[(j0) * 3])*1000;
	}
	j0 = i0-1;
	if(j0 >=1 && j0 <= 998)
	{
		der[(j0) * 3 + 1] = d[(j0)]*(x[(j0) * 3]+x[(j0-1) * 3])/2.0*(-x[(j0) * 3]+x[(j0-1) * 3])*1000+(1.0-d[(j0)])*(x[(j0+1) * 3]+x[(j0) * 3])*(-x[(j0+1) * 3]+x[(j0) * 3])*1000;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 999)
	{
		zc[0] = x[(i0) * 3]-(0.0);
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 999)
	{
		d[(i0)] = 1.0;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	if(i0 >= 0 && i0 <= 999)
	{
		d[(i0)] = 0.0;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	int i0 = i;
	int j0 = 0;
	j0 = i0;
	if(j0 >=0 && j0 <= 4)
	{
		out[0] = x[(200*j0) * 3];
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
	int j0 = 0;
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
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->nDS[i0] = 3;
	}
	modelData->nDS[999] = 3;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[999]++;
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->nSD[i0-1]++;
		modelData->nSD[i0]++;
		modelData->nSD[i0+1]++;
	}
	modelData->nSD[0]++;
	modelData->nSD[998]++;
	modelData->nSD[999]++;
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nZS[i0] = 1;
	}
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->nSZ[i0]++;
	}
	modelData->nHD[0]++;
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->nHD[i0]++;
	}
	modelData->nHD[999]++;
	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].nLHSDsc = 1;
	}
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,1000);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[0][states[0]++] = 999;
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->DS[i0][states[i0]++] = i0-1;
		modelData->DS[i0][states[i0]++] = i0;
		modelData->DS[i0][states[i0]++] = i0+1;
	}
	modelData->DS[999][states[999]++] = 0;
	modelData->DS[999][states[999]++] = 998;
	modelData->DS[999][states[999]++] = 999;
		cleanVector(states,0,1000);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[999][states[999]++] = 0;
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->SD[i0-1][states[i0-1]++] = i0;
		modelData->SD[i0][states[i0]++] = i0;
		modelData->SD[i0+1][states[i0+1]++] = i0;
	}
	modelData->SD[0][states[0]++] = 999;
	modelData->SD[998][states[998]++] = 999;
	modelData->SD[999][states[999]++] = 999;
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->ZS[i0][events[i0]++] = i0;
	}
		cleanVector(states,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->SZ[i0][states[i0]++] = i0;
	}
		cleanVector(events,0,1000);

	modelData->HD[0][events[0]++] = 0;
	for(i0 = 1; i0 <= 998; i0++)
	{
		modelData->HD[i0][events[i0]++] = i0;
	}
	modelData->HD[999][events[999]++] = 999;
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].LHSDsc[events[i0]++] = i0;
	}
		cleanVector(events,0,1000);

	for(i0 = 0; i0 <= 999; i0++)
	{
		modelData->event[i0].direction = 0;
		modelData->event[i0].relation = 2;
	}
	simulator->time = QSS_Time(1000,1000,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.01;
	simulator->output = SD_Output("burgers",5,1000,1000,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->nOS[i0] = 1;
		modelOutput->nSO[200*i0]++;
	}
	SD_allocOutputMatrix(modelOutput,1000,1000);
		cleanVector(states,0,1000);

		cleanVector(outputs,0,5);

	for(i0 = 0; i0 <= 4; i0++)
	{
		sprintf(modelOutput->variable[i0].name,"u[%d]",200*i0+1);
	}
		cleanVector(outputs,0,5);

	for(i0 = 0; i0 <= 4; i0++)
	{
		modelOutput->SO[200*i0][states[200*i0]++] = i0;
		modelOutput->OS[i0][outputs[i0]++] = 200*i0;
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
