#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_C = 0;
double __PAR_L = 0;
double __PAR_R = 0;
double __PAR_U = 0;
double __PAR_T = 0;
double __PAR_DC = 0;
double __PAR_ROn = 0;
double __PAR_ROff = 0;
double __PAR_C1 = 0;
double __PAR_L1 = 0;

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
	switch(i)
	{
		case 0:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			dx[1] = (alg[0]-x[9])/__PAR_C1;
			dx[2] = (-(x[10]-alg[1])*(1.0/(__PAR_C1)))/2;
			return;
		case 1:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			dx[1] = (__PAR_U-x[0]-alg[0]*d[(0)])/__PAR_L1;
			dx[2] = (-(1.0/(__PAR_L1))*(alg[1]*d[(0)]+x[1]))/2;
			return;
		case 2:
			dx[1] = (x[9]-x[6]/__PAR_R)/__PAR_C;
			dx[2] = (-(1.0/(__PAR_C))*(x[7]*(1.0/(__PAR_R))-x[10]))/2;
			return;
		case 3:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			dx[1] = (-x[6]-alg[0]*d[(0)])/__PAR_L;
			dx[2] = (-(1.0/(__PAR_L))*(x[7]+d[(0)]*alg[1]))/2;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			der[0 + 1] = (alg[0]-x[9])/__PAR_C1;
			der[0 + 2] = (-(x[10]-alg[1])*(1.0/(__PAR_C1)))/2;
			der[3 + 1] = (__PAR_U-x[0]-alg[0]*d[(0)])/__PAR_L1;
			der[3 + 2] = (-(1.0/(__PAR_L1))*(alg[1]*d[(0)]+x[1]))/2;
			der[9 + 1] = (-x[6]-alg[0]*d[(0)])/__PAR_L;
			der[9 + 2] = (-(1.0/(__PAR_L))*(x[7]+d[(0)]*alg[1]))/2;
			return;
		case 1:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			der[0 + 1] = (alg[0]-x[9])/__PAR_C1;
			der[0 + 2] = (-(x[10]-alg[1])*(1.0/(__PAR_C1)))/2;
			der[3 + 1] = (__PAR_U-x[0]-alg[0]*d[(0)])/__PAR_L1;
			der[3 + 2] = (-(1.0/(__PAR_L1))*(alg[1]*d[(0)]+x[1]))/2;
			der[9 + 1] = (-x[6]-alg[0]*d[(0)])/__PAR_L;
			der[9 + 2] = (-(1.0/(__PAR_L))*(x[7]+d[(0)]*alg[1]))/2;
			return;
		case 2:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			der[6 + 1] = (x[9]-x[6]/__PAR_R)/__PAR_C;
			der[6 + 2] = (-(1.0/(__PAR_C))*(x[7]*(1.0/(__PAR_R))-x[10]))/2;
			der[9 + 1] = (-x[6]-alg[0]*d[(0)])/__PAR_L;
			der[9 + 2] = (-(1.0/(__PAR_L))*(x[7]+d[(0)]*alg[1]))/2;
			return;
		case 3:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			der[0 + 1] = (alg[0]-x[9])/__PAR_C1;
			der[0 + 2] = (-(x[10]-alg[1])*(1.0/(__PAR_C1)))/2;
			der[3 + 1] = (__PAR_U-x[0]-alg[0]*d[(0)])/__PAR_L1;
			der[3 + 2] = (-(1.0/(__PAR_L1))*(alg[1]*d[(0)]+x[1]))/2;
			der[6 + 1] = (x[9]-x[6]/__PAR_R)/__PAR_C;
			der[6 + 2] = (-(1.0/(__PAR_C))*(x[7]*(1.0/(__PAR_R))-x[10]))/2;
			der[9 + 1] = (-x[6]-alg[0]*d[(0)])/__PAR_L;
			der[9 + 2] = (-(1.0/(__PAR_L))*(x[7]+d[(0)]*alg[1]))/2;
			return;
	}
}

void
MOD_zeroCrossing(int i, double *x, double *d, double *alg, double t, double *zc)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			zc[0] = t-(d[(2)]);
			zc[1] = 1.0;
			return;
		case 1:
			zc[0] = t-d[(3)]-__PAR_DC*__PAR_T-(0.0);
			zc[1] = 1.0;
			return;
		case 2:
			alg[0] = (d[(1)]*(x[9]+x[3])-x[0])/(d[(0)]+d[(1)]);
			alg[0 + 1] = (1.0/(d[(1)]+d[(0)]))*(d[(1)]*(x[10]+x[4])-x[1]);
			zc[0] = alg[0]-(0.0);
			zc[1] = alg[1];
			return;
	}
}

void
MOD_handlerPos(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(3)] = d[(2)];
			d[(2)] = d[(2)]+__PAR_T;
			d[(1)] = __PAR_ROn;
			return;
		case 1:
			d[(1)] = __PAR_ROff;
			d[(0)] = __PAR_ROn;
			return;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 2:
			d[(0)] = __PAR_ROff;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[6];
			return;
		case 1:
			out[0] = x[9];
			return;
		case 2:
			out[0] = x[0];
			return;
		case 3:
			out[0] = x[3];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(4*sizeof(int));
	int *events = (int*)malloc(3*sizeof(int));
	int *outputs = (int*)malloc(4*sizeof(int));
	int *states = (int*)malloc(4*sizeof(int));
	int i0;
	simulator->data = QSS_Data(4,4,3,0,1,"cuk");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_C = 1.000000000000000047921736e-04;
	__PAR_L = 1.000000000000000047921736e-04;
	__PAR_R = 10.0;
	__PAR_U = 24.0;
	__PAR_T = 1.000000000000000047921736e-04;
	__PAR_DC = 2.500000000000000000000000e-01;
	__PAR_ROn = 1.000000000000000081803054e-05;
	__PAR_ROff = 1.000000000000000000000000e+05;
	__PAR_C1 = 1.000000000000000047921736e-04;
	__PAR_L1 = 1.000000000000000047921736e-04;
	// Initialize model code.
		modelData->d[(2)] = __PAR_T;
		modelData->d[(1)] = 1.000000000000000000000000e+05;
		modelData->d[(0)] = 1.000000000000000000000000e+05;
	modelData->nDS[0] = 1;
	modelData->nDS[1] = 1;
	modelData->nDS[2] = 2;
	modelData->nDS[3] = 1;
	modelData->nDS[0]++;
	modelData->nDS[0]++;
	modelData->nDS[1]++;
	modelData->nDS[1]++;
	modelData->nDS[3]++;
	modelData->nDS[3]++;
	modelData->nDS[3]++;
	modelData->nSD[3]++;
	modelData->nSD[0]++;
	modelData->nSD[2]++;
	modelData->nSD[3]++;
	modelData->nSD[2]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[1]++;
	modelData->nSD[3]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[3]++;
	modelData->nZS[2]++;
	modelData->nZS[2]++;
	modelData->nZS[2]++;
	modelData->nSZ[0]++;
	modelData->nSZ[1]++;
	modelData->nSZ[3]++;
	modelData->nHZ[0] += 2;
	modelData->nHZ[0] += 1;
	modelData->nHZ[1] += 2;
	modelData->nHZ[2] += 1;
	modelData->nHD[0] = 3;
	modelData->nHD[1] = 3;
	modelData->nHD[2] = 3;
	modelData->event[0].nLHSDsc = 3;
	modelData->event[1].nLHSDsc = 2;
	modelData->event[2].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,4);

	modelData->DS[0][states[0]++] = 3;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[2][states[2]++] = 2;
	modelData->DS[2][states[2]++] = 3;
	modelData->DS[3][states[3]++] = 2;
	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 1;
	modelData->DS[1][states[1]++] = 3;
	modelData->DS[3][states[3]++] = 0;
	modelData->DS[3][states[3]++] = 1;
	modelData->DS[3][states[3]++] = 3;
		cleanVector(states,0,4);

	modelData->SD[3][states[3]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[2][states[2]++] = 2;
	modelData->SD[3][states[3]++] = 2;
	modelData->SD[2][states[2]++] = 3;
	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[1][states[1]++] = 1;
	modelData->SD[3][states[3]++] = 1;
	modelData->SD[0][states[0]++] = 3;
	modelData->SD[1][states[1]++] = 3;
	modelData->SD[3][states[3]++] = 3;
		cleanVector(events,0,3);

	modelData->ZS[2][events[2]++] = 0;
	modelData->ZS[2][events[2]++] = 1;
	modelData->ZS[2][events[2]++] = 3;
		cleanVector(states,0,4);

	modelData->SZ[0][states[0]++] = 2;
	modelData->SZ[1][states[1]++] = 2;
	modelData->SZ[3][states[3]++] = 2;
		cleanVector(events,0,3);

	modelData->HZ[0][events[0]++] = 0;
	modelData->HZ[0][events[0]++] = 1;
	modelData->HZ[0][events[0]++] = 2;
	modelData->HZ[1][events[1]++] = 2;
	modelData->HZ[1][events[1]++] = 2;
	modelData->HZ[2][events[2]++] = 2;
		cleanVector(events,0,3);

	modelData->HD[1][events[1]++] = 0;
	modelData->HD[2][events[2]++] = 0;
	modelData->HD[0][events[0]++] = 0;
	modelData->HD[1][events[1]++] = 1;
	modelData->HD[2][events[2]++] = 1;
	modelData->HD[0][events[0]++] = 1;
	modelData->HD[1][events[1]++] = 3;
	modelData->HD[2][events[2]++] = 3;
	modelData->HD[0][events[0]++] = 3;
		cleanVector(events,0,3);

	modelData->event[0].LHSDsc[events[0]++] = 1;
	modelData->event[0].LHSDsc[events[0]++] = 2;
	modelData->event[0].LHSDsc[events[0]++] = 3;
	modelData->event[1].LHSDsc[events[1]++] = 0;
	modelData->event[1].LHSDsc[events[1]++] = 1;
	modelData->event[2].LHSDsc[events[2]++] = 0;
		cleanVector(events,0,3);

	modelData->event[0].direction = 1;
	modelData->event[0].relation = 2;
	modelData->event[1].direction = 1;
	modelData->event[1].relation = 2;
	modelData->event[2].direction = -1;
	modelData->event[2].relation = 0;
	simulator->time = QSS_Time(4,3,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("cuk",4,4,4,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[2]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[3]++;
		modelOutput->nOS[2] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[3] = 1;
		modelOutput->nSO[1]++;
	SD_allocOutputMatrix(modelOutput,4,4);
		cleanVector(states,0,4);

		cleanVector(outputs,0,4);

		sprintf(modelOutput->variable[0].name,"uC");
		sprintf(modelOutput->variable[1].name,"iL");
		sprintf(modelOutput->variable[2].name,"uC1");
		sprintf(modelOutput->variable[3].name,"iL1");
		cleanVector(outputs,0,4);

		modelOutput->SO[2][states[2]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 2;
		modelOutput->SO[3][states[3]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 3;
		modelOutput->SO[0][states[0]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 0;
		modelOutput->SO[1][states[1]++] = 3;
		modelOutput->OS[3][outputs[3]++] = 1;
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
