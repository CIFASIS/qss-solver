#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_retQSS.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_Bx = 0;
double __PAR_By = 0;
double __PAR_Bz = 0;
double __PAR_c = 0;
double __PAR_m = 0;
double __PAR_q = 0;
double __PAR_gamma2 = 0;
double __PAR_v0 = 0;
double __PAR_invMGamma2 = 0;
double __PAR_qc2 = 0;
double __PAR_coeff = 0;
double __PAR_status = 0;

/*
* Disabled for testing purposes.
void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 9;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 3;
}
*/

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			dx[1] = x[9];
			return;
		case 1:
			dx[1] = x[12];
			return;
		case 2:
			dx[1] = x[15];
			return;
		case 3:
			dx[1] = __PAR_coeff*(__PAR_Bz*x[12]-__PAR_By*x[15]);
			return;
		case 4:
			dx[1] = __PAR_coeff*(__PAR_Bx*x[15]-__PAR_Bz*x[9]);
			return;
		case 5:
			dx[1] = __PAR_coeff*(__PAR_By*x[9]-__PAR_Bx*x[12]);
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 3:
			der[0 + 1] = x[9];
			der[12 + 1] = __PAR_coeff*(__PAR_Bx*x[15]-__PAR_Bz*x[9]);
			der[15 + 1] = __PAR_coeff*(__PAR_By*x[9]-__PAR_Bx*x[12]);
			return;
		case 4:
			der[3 + 1] = x[12];
			der[9 + 1] = __PAR_coeff*(__PAR_Bz*x[12]-__PAR_By*x[15]);
			der[15 + 1] = __PAR_coeff*(__PAR_By*x[9]-__PAR_Bx*x[12]);
			return;
		case 5:
			der[6 + 1] = x[15];
			der[9 + 1] = __PAR_coeff*(__PAR_Bz*x[12]-__PAR_By*x[15]);
			der[12 + 1] = __PAR_coeff*(__PAR_Bx*x[15]-__PAR_Bz*x[9]);
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
			zc[0] = t-(__retQSS__particle_nextCrossingTime(1,x[0],x[3],x[6],x[9],x[12],x[15]));
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
			d[(0)] = __retQSS__particle_currentVolumeID(1);
			d[(1)] = __retQSS__volume_getProperty(d[(0)],"test_prop");
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = d[(0)];
			return;
		case 1:
			out[0] = d[(1)];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(2*sizeof(int));
	int *events = (int*)malloc(1*sizeof(int));
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(6*sizeof(int));
	int i0;
	simulator->data = QSS_Data(6,2,1,0,0,"helix");
	simulator->time = QSS_Time(6,1,0,0,ST_Linear,NULL);
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_Bx = 0.0;
	__PAR_By = 0.0;
	__PAR_Bz = 1.000000000000000020816682e-03;
	__PAR_c = 2.997924580000000105428626e+02;
	__PAR_m = 5.109989099999999728751732e-01;
	__PAR_q = 1.000000000000000000000000e+00;
	__PAR_gamma2 = 2.236627204212999942001261e+01;
	__PAR_v0 = 9.989999999999999991118216e-01*__PAR_c;
	__PAR_invMGamma2 = 1.0/(__PAR_m*__PAR_gamma2);
	__PAR_qc2 = __PAR_q*8.987551787369999510701746e+04;
	__PAR_coeff = __PAR_qc2*__PAR_invMGamma2;
	// Initialize model code.
		modelData->x[0] = 0.0;
		modelData->x[3] = 0.0;
		modelData->x[6] = 0.0;
		modelData->x[9] = 9e-01*__PAR_v0;
		modelData->x[12] = 0.0;
		modelData->x[15] = 4.358898943540672754615173e-01*__PAR_v0;
		__PAR_status = __retQSS__retQSS_setUp(1.0,"../../data/helix/geom/helix","helix");
	if(__PAR_status==0.0)
	{
		__PAR_status = __retQSS__retQSS_fail();
	}
		modelData->d[(0)] = __retQSS__particle_currentVolumeID(1);
	for(i0 = -1; i0 <= 63; i0++)
	{
		modelData->d[(1)] = __retQSS__volume_setProperty((i0+1),"test_prop",2.0*(i0+1));
	}
		modelData->d[(1)] = __retQSS__volume_getProperty(modelData->d[(0)],"test_prop");
	modelData->nDS[0] = 1;
	modelData->nDS[1] = 1;
	modelData->nDS[2] = 1;
	modelData->nDS[3] = 2;
	modelData->nDS[4] = 2;
	modelData->nDS[5] = 2;
	modelData->nSD[3]++;
	modelData->nSD[4]++;
	modelData->nSD[5]++;
	modelData->nSD[4]++;
	modelData->nSD[5]++;
	modelData->nSD[3]++;
	modelData->nSD[5]++;
	modelData->nSD[3]++;
	modelData->nSD[4]++;
	modelData->nZS[0] = 6;
	modelData->nSZ[0]++;
	modelData->nSZ[1]++;
	modelData->nSZ[2]++;
	modelData->nSZ[3]++;
	modelData->nSZ[4]++;
	modelData->nSZ[5]++;
	modelData->event[0].nLHSDsc = 2;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,6);

	modelData->DS[0][states[0]++] = 3;
	modelData->DS[1][states[1]++] = 4;
	modelData->DS[2][states[2]++] = 5;
	modelData->DS[3][states[3]++] = 4;
	modelData->DS[3][states[3]++] = 5;
	modelData->DS[4][states[4]++] = 3;
	modelData->DS[4][states[4]++] = 5;
	modelData->DS[5][states[5]++] = 3;
	modelData->DS[5][states[5]++] = 4;
		cleanVector(states,0,6);

	modelData->SD[3][states[3]++] = 0;
	modelData->SD[4][states[4]++] = 1;
	modelData->SD[5][states[5]++] = 2;
	modelData->SD[4][states[4]++] = 3;
	modelData->SD[5][states[5]++] = 3;
	modelData->SD[3][states[3]++] = 4;
	modelData->SD[5][states[5]++] = 4;
	modelData->SD[3][states[3]++] = 5;
	modelData->SD[4][states[4]++] = 5;
		cleanVector(events,0,1);

	modelData->ZS[0][events[0]++] = 0;
	modelData->ZS[0][events[0]++] = 1;
	modelData->ZS[0][events[0]++] = 2;
	modelData->ZS[0][events[0]++] = 3;
	modelData->ZS[0][events[0]++] = 4;
	modelData->ZS[0][events[0]++] = 5;
		cleanVector(states,0,6);

	modelData->SZ[0][states[0]++] = 0;
	modelData->SZ[1][states[1]++] = 0;
	modelData->SZ[2][states[2]++] = 0;
	modelData->SZ[3][states[3]++] = 0;
	modelData->SZ[4][states[4]++] = 0;
	modelData->SZ[5][states[5]++] = 0;
		cleanVector(events,0,1);

	modelData->event[0].LHSDsc[events[0]++] = 0;
	modelData->event[0].LHSDsc[events[0]++] = 1;
		cleanVector(events,0,1);

	modelData->event[0].direction = 1;
	modelData->event[0].relation = 2;

	simulator->output = SD_Output("helix",2,2,6,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOD[0] = 1;
		modelOutput->nDO[0]++;
		modelOutput->nOD[1] = 1;
		modelOutput->nDO[1]++;
	SD_allocOutputMatrix(modelOutput,6,2);
		cleanVector(discretes,0,2);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"volumeID");
		sprintf(modelOutput->variable[1].name,"prop");
		cleanVector(outputs,0,2);

		modelOutput->DO[0][discretes[0]++] = 0;
		modelOutput->OD[0][outputs[0]++] = 0;
		modelOutput->DO[1][discretes[1]++] = 1;
		modelOutput->OD[1][outputs[1]++] = 1;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,MOD_zeroCrossing,MOD_handlerPos,NULL);
	free(discretes);
	free(events);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
