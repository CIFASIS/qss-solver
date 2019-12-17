#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_spring1_b = 0;
double __PAR_spring1_k = 0;
double __PAR_fixed1_s0 = 0;
double __PAR_ball1_m = 0;
double __PAR_ball1_g = 0;

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = TRUE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	switch(i)
	{
		case 0:
			dx[1] = x[4];
			dx[2] = (x[5])/2;
			dx[3] = (x[6]*2)/6;
			return;
		case 1:
			alg[0] = __PAR_fixed1_s0;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 0.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[8] = x[4];
			alg[8 + 1] = x[5];
			alg[8 + 2] = x[6]*2;
			alg[12] = x[0];
			alg[12 + 1] = x[1];
			alg[12 + 2] = x[2]*2;
			alg[16] = alg[0];
			alg[16 + 1] = alg[1];
			alg[16 + 2] = alg[2];
			alg[20] = alg[4];
			alg[20 + 1] = alg[5];
			alg[20 + 2] = alg[6];
			alg[24] = alg[12];
			alg[24 + 1] = alg[13];
			alg[24 + 2] = alg[14];
			alg[28] = alg[8];
			alg[28 + 1] = alg[9];
			alg[28 + 2] = alg[10];
			alg[32] = alg[28]-alg[20];
			alg[32 + 1] = -alg[21]+alg[29];
			alg[32 + 2] = alg[30]-alg[22];
			alg[36] = alg[24]-alg[16];
			alg[36 + 1] = -alg[17]+alg[25];
			alg[36 + 2] = alg[26]-alg[18];
			alg[40] = d[(0)]*(__PAR_spring1_b*alg[32]+__PAR_spring1_k*alg[36])+(1.0-d[(0)])*(0.0);
			alg[40 + 1] = (__PAR_spring1_k*alg[37]+__PAR_spring1_b*alg[33])*d[(0)];
			alg[40 + 2] = d[(0)]*(alg[38]*__PAR_spring1_k+alg[34]*__PAR_spring1_b);
			alg[48] = (-alg[40]);
			alg[48 + 1] = -alg[41];
			alg[48 + 2] = -alg[42];
			dx[1] = (alg[48]-__PAR_ball1_m*__PAR_ball1_g)*(1.0/(__PAR_ball1_m));
			dx[2] = ((1.0/(__PAR_ball1_m))*alg[49])/2;
			dx[3] = (alg[50]*(1.0/(__PAR_ball1_m)))/6;
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			alg[0] = __PAR_fixed1_s0;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 0.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[8] = x[4];
			alg[8 + 1] = x[5];
			alg[8 + 2] = x[6]*2;
			alg[12] = x[0];
			alg[12 + 1] = x[1];
			alg[12 + 2] = x[2]*2;
			alg[16] = alg[0];
			alg[16 + 1] = alg[1];
			alg[16 + 2] = alg[2];
			alg[20] = alg[4];
			alg[20 + 1] = alg[5];
			alg[20 + 2] = alg[6];
			alg[24] = alg[12];
			alg[24 + 1] = alg[13];
			alg[24 + 2] = alg[14];
			alg[28] = alg[8];
			alg[28 + 1] = alg[9];
			alg[28 + 2] = alg[10];
			alg[32] = alg[28]-alg[20];
			alg[32 + 1] = -alg[21]+alg[29];
			alg[32 + 2] = alg[30]-alg[22];
			alg[36] = alg[24]-alg[16];
			alg[36 + 1] = -alg[17]+alg[25];
			alg[36 + 2] = alg[26]-alg[18];
			alg[40] = d[(0)]*(__PAR_spring1_b*alg[32]+__PAR_spring1_k*alg[36])+(1.0-d[(0)])*(0.0);
			alg[40 + 1] = (__PAR_spring1_k*alg[37]+__PAR_spring1_b*alg[33])*d[(0)];
			alg[40 + 2] = d[(0)]*(alg[38]*__PAR_spring1_k+alg[34]*__PAR_spring1_b);
			alg[48] = (-alg[40]);
			alg[48 + 1] = -alg[41];
			alg[48 + 2] = -alg[42];
			der[4 + 1] = (alg[48]-__PAR_ball1_m*__PAR_ball1_g)*(1.0/(__PAR_ball1_m));
			der[4 + 2] = ((1.0/(__PAR_ball1_m))*alg[49])/2;
			der[4 + 3] = (alg[50]*(1.0/(__PAR_ball1_m)))/6;
			return;
		case 1:
			alg[0] = __PAR_fixed1_s0;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[4] = 0.0;
			alg[4 + 1] = 0.0;
			alg[4 + 2] = 0.0;
			alg[8] = x[4];
			alg[8 + 1] = x[5];
			alg[8 + 2] = x[6]*2;
			alg[12] = x[0];
			alg[12 + 1] = x[1];
			alg[12 + 2] = x[2]*2;
			alg[16] = alg[0];
			alg[16 + 1] = alg[1];
			alg[16 + 2] = alg[2];
			alg[20] = alg[4];
			alg[20 + 1] = alg[5];
			alg[20 + 2] = alg[6];
			alg[24] = alg[12];
			alg[24 + 1] = alg[13];
			alg[24 + 2] = alg[14];
			alg[28] = alg[8];
			alg[28 + 1] = alg[9];
			alg[28 + 2] = alg[10];
			alg[32] = alg[28]-alg[20];
			alg[32 + 1] = -alg[21]+alg[29];
			alg[32 + 2] = alg[30]-alg[22];
			alg[36] = alg[24]-alg[16];
			alg[36 + 1] = -alg[17]+alg[25];
			alg[36 + 2] = alg[26]-alg[18];
			alg[40] = d[(0)]*(__PAR_spring1_b*alg[32]+__PAR_spring1_k*alg[36])+(1.0-d[(0)])*(0.0);
			alg[40 + 1] = (__PAR_spring1_k*alg[37]+__PAR_spring1_b*alg[33])*d[(0)];
			alg[40 + 2] = d[(0)]*(alg[38]*__PAR_spring1_k+alg[34]*__PAR_spring1_b);
			alg[48] = (-alg[40]);
			alg[48 + 1] = -alg[41];
			alg[48 + 2] = -alg[42];
			der[0 + 1] = x[4];
			der[0 + 2] = (x[5])/2;
			der[0 + 3] = (x[6]*2)/6;
			der[4 + 1] = (alg[48]-__PAR_ball1_m*__PAR_ball1_g)*(1.0/(__PAR_ball1_m));
			der[4 + 2] = ((1.0/(__PAR_ball1_m))*alg[49])/2;
			der[4 + 3] = (alg[50]*(1.0/(__PAR_ball1_m)))/6;
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
			alg[0] = __PAR_fixed1_s0;
			alg[0 + 1] = 0.0;
			alg[0 + 2] = 0.0;
			alg[12] = x[0];
			alg[12 + 1] = x[1];
			alg[12 + 2] = x[2]*2;
			alg[16] = alg[0];
			alg[16 + 1] = alg[1];
			alg[16 + 2] = alg[2];
			alg[24] = alg[12];
			alg[24 + 1] = alg[13];
			alg[24 + 2] = alg[14];
			alg[36] = alg[24]-alg[16];
			alg[36 + 1] = -alg[17]+alg[25];
			alg[36 + 2] = alg[26]-alg[18];
			zc[0] = alg[36]-(0.0);
			zc[1] = alg[37];
			zc[2] = (alg[38])/2;
			return;
		case 1:
			zc[0] = d[(0)]-(0.0);
			zc[1] = 0.0;
			zc[2] = (0.0)/2;
			return;
		case 2:
			zc[0] = d[(0)]-(1.0);
			zc[1] = 0.0;
			zc[2] = (0.0)/2;
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
			d[(0)] = 0.0;
			return;
		case 1:
			d[(1)] = 1.0;
			return;
	}
}

void
MOD_handlerNeg(int i, double *x, double *d, double *alg, double t)
{
	int i0 = i;
	switch(i)
	{
		case 0:
			d[(0)] = 1.0;
			return;
		case 2:
			d[(1)] = 0.0;
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *discretes = (int*)malloc(2*sizeof(int));
	int *events = (int*)malloc(3*sizeof(int));
	int *outputs = (int*)malloc(1*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0;
	simulator->data = QSS_Data(2,2,3,0,14,"BouncingBall");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_spring1_b = 10.0;
	__PAR_spring1_k = 10000.0;
	__PAR_fixed1_s0 = 0.0;
	__PAR_ball1_m = 1.0;
	__PAR_ball1_g = 9.800000000000000710542736e+00;
	modelData->x[0] = 10.0;
	// Initialize model code.
	modelData->nDS[0] = 1;
	modelData->nDS[1]++;
	modelData->nDS[1]++;
	modelData->nSD[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nZS[0]++;
	modelData->nSZ[0]++;
	modelData->nHZ[0] += 2;
	modelData->nHD[0] = 1;
	modelData->event[0].nLHSDsc = 1;
	modelData->event[1].nLHSDsc = 1;
	modelData->event[2].nLHSDsc = 1;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2);

	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
		cleanVector(states,0,2);

	modelData->SD[1][states[1]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
		cleanVector(events,0,3);

	modelData->ZS[0][events[0]++] = 0;
		cleanVector(states,0,2);

	modelData->SZ[0][states[0]++] = 0;
		cleanVector(events,0,3);

	modelData->HZ[0][events[0]++] = 1;
	modelData->HZ[0][events[0]++] = 2;
		cleanVector(events,0,3);

	modelData->HD[0][events[0]++] = 1;
		cleanVector(events,0,3);

	modelData->event[0].LHSDsc[events[0]++] = 0;
	modelData->event[1].LHSDsc[events[1]++] = 1;
	modelData->event[2].LHSDsc[events[2]++] = 1;
		cleanVector(events,0,3);

	modelData->event[0].direction = 0;
	modelData->event[0].relation = 0;
	modelData->event[1].direction = 1;
	modelData->event[1].relation = 2;
	modelData->event[2].direction = -1;
	modelData->event[2].relation = 0;
	simulator->time = QSS_Time(2,3,0,0,ST_Binary,NULL);

		double period[1];
	period[0] = 0.01;
	simulator->output = SD_Output("BouncingBall",1,2,2,period,1,0,CI_Sampled,SD_Memory,MOD_output);
	SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,2,2);
		cleanVector(states,0,2);

		cleanVector(outputs,0,1);

		sprintf(modelOutput->variable[0].name,"ball1_y");
		cleanVector(outputs,0,1);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
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
