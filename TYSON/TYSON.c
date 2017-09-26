#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pkg_math.h"
#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_k1 = 0;
double __PAR_k2 = 0;
double __PAR_k3 = 0;
double __PAR_k4 = 0;
double __PAR_k4prim = 0;
double __PAR_k5 = 0;
double __PAR_k6 = 0;
double __PAR_k7 = 0;
double __PAR_k8 = 0;
double __PAR_k9 = 0;

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
	switch(i)
	{
		case 0:
			dx[1] = __PAR_k6*x[9]-__PAR_k8*x[0]+__PAR_k9*x[3];
			return;
		case 1:
			dx[1] = -__PAR_k3*x[3]*x[12]+__PAR_k8*x[0]-__PAR_k9*x[3];
			return;
		case 2:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			dx[1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			return;
		case 3:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			dx[1] = x[6]*alg[6]-__PAR_k5*x[9]-__PAR_k6*x[9];
			return;
		case 4:
			dx[1] = __PAR_k1-__PAR_k2*x[12]-__PAR_k3*x[3]*x[12];
			return;
		case 5:
			dx[1] = __PAR_k6*x[9]-__PAR_k7*x[15];
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			der[0 + 1] = __PAR_k6*x[9]-__PAR_k8*x[0]+__PAR_k9*x[3];
			der[3 + 1] = -__PAR_k3*x[3]*x[12]+__PAR_k8*x[0]-__PAR_k9*x[3];
			der[6 + 1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			der[9 + 1] = x[6]*alg[6]-__PAR_k5*x[9]-__PAR_k6*x[9];
			return;
		case 1:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			der[0 + 1] = __PAR_k6*x[9]-__PAR_k8*x[0]+__PAR_k9*x[3];
			der[3 + 1] = -__PAR_k3*x[3]*x[12]+__PAR_k8*x[0]-__PAR_k9*x[3];
			der[6 + 1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			der[9 + 1] = x[6]*alg[6]-__PAR_k5*x[9]-__PAR_k6*x[9];
			der[12 + 1] = __PAR_k1-__PAR_k2*x[12]-__PAR_k3*x[3]*x[12];
			return;
		case 2:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			der[6 + 1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			der[9 + 1] = x[6]*alg[6]-__PAR_k5*x[9]-__PAR_k6*x[9];
			return;
		case 3:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			der[0 + 1] = __PAR_k6*x[9]-__PAR_k8*x[0]+__PAR_k9*x[3];
			der[6 + 1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			der[9 + 1] = x[6]*alg[6]-__PAR_k5*x[9]-__PAR_k6*x[9];
			der[15 + 1] = __PAR_k6*x[9]-__PAR_k7*x[15];
			return;
		case 4:
			alg[0] = x[0]+x[3]+x[6]+x[9];
			alg[6] = __PAR_k4prim+__PAR_k4*__math__pow((x[9]*1.000000000000000000000000e+00/alg[0]),2.0);
			der[3 + 1] = -__PAR_k3*x[3]*x[12]+__PAR_k8*x[0]-__PAR_k9*x[3];
			der[6 + 1] = __PAR_k3*x[3]*x[12]+-x[6]*alg[6]+__PAR_k5*x[9];
			der[12 + 1] = __PAR_k1-__PAR_k2*x[12]-__PAR_k3*x[3]*x[12];
			return;
		case 5:
			der[15 + 1] = __PAR_k6*x[9]-__PAR_k7*x[15];
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
		case 1:
			out[0] = x[3];
			return;
		case 2:
			out[0] = x[6];
			return;
		case 3:
			out[0] = x[9];
			return;
		case 4:
			out[0] = x[12];
			return;
		case 5:
			out[0] = x[15];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(6*sizeof(int));
	int *states = (int*)malloc(6*sizeof(int));
	int i0;
	simulator->data = QSS_Data(6,0,0,0,4,"TYSON");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_k1 = 1.499999999999999944488849e-02;
	__PAR_k2 = 0.000000000000000000000000e+00;
	__PAR_k3 = 200.0;
	__PAR_k4 = 180.0;
	__PAR_k4prim = 1.799999999999999863997679e-02;
	__PAR_k5 = 0.000000000000000000000000e+00;
	__PAR_k6 = 1.000000000000000000000000e+00;
	__PAR_k7 = 5.999999999999999777955395e-01;
	__PAR_k8 = 1.000000000000000000000000e+06;
	__PAR_k9 = 1.000000000000000000000000e+03;
	modelData->x[0] = 0.000000000000000000000000e+00;
	modelData->x[3] = 7.500000000000000000000000e-01;
	modelData->x[9] = 0.000000000000000000000000e+00;
	modelData->x[12] = 0.000000000000000000000000e+00;
	modelData->x[6] = 2.500000000000000000000000e-01;
	modelData->x[15] = 0.000000000000000000000000e+00;
	// Initialize model code.
	modelData->nDS[0] = 3;
	modelData->nDS[1] = 3;
	modelData->nDS[2] = 4;
	modelData->nDS[3] = 2;
	modelData->nDS[4] = 2;
	modelData->nDS[5] = 2;
	modelData->nDS[2]++;
	modelData->nDS[3]++;
	modelData->nDS[3]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[3]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[4]++;
	modelData->nSD[1]++;
	modelData->nSD[2]++;
	modelData->nSD[3]++;
	modelData->nSD[4]++;
	modelData->nSD[2]++;
	modelData->nSD[3]++;
	modelData->nSD[1]++;
	modelData->nSD[4]++;
	modelData->nSD[3]++;
	modelData->nSD[5]++;
	modelData->nSD[0]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,6);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[0][states[0]++] = 3;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
	modelData->DS[1][states[1]++] = 4;
	modelData->DS[2][states[2]++] = 1;
	modelData->DS[2][states[2]++] = 2;
	modelData->DS[2][states[2]++] = 3;
	modelData->DS[2][states[2]++] = 4;
	modelData->DS[3][states[3]++] = 2;
	modelData->DS[3][states[3]++] = 3;
	modelData->DS[4][states[4]++] = 1;
	modelData->DS[4][states[4]++] = 4;
	modelData->DS[5][states[5]++] = 3;
	modelData->DS[5][states[5]++] = 5;
	modelData->DS[2][states[2]++] = 0;
	modelData->DS[3][states[3]++] = 0;
	modelData->DS[3][states[3]++] = 1;
		cleanVector(states,0,6);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[3][states[3]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
	modelData->SD[4][states[4]++] = 1;
	modelData->SD[1][states[1]++] = 2;
	modelData->SD[2][states[2]++] = 2;
	modelData->SD[3][states[3]++] = 2;
	modelData->SD[4][states[4]++] = 2;
	modelData->SD[2][states[2]++] = 3;
	modelData->SD[3][states[3]++] = 3;
	modelData->SD[1][states[1]++] = 4;
	modelData->SD[4][states[4]++] = 4;
	modelData->SD[3][states[3]++] = 5;
	modelData->SD[5][states[5]++] = 5;
	modelData->SD[0][states[0]++] = 2;
	modelData->SD[0][states[0]++] = 3;
	modelData->SD[1][states[1]++] = 3;
	simulator->time = QSS_Time(6,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("TYSON",6,0,6,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[1]++;
		modelOutput->nOS[2] = 1;
		modelOutput->nSO[2]++;
		modelOutput->nOS[3] = 1;
		modelOutput->nSO[3]++;
		modelOutput->nOS[4] = 1;
		modelOutput->nSO[4]++;
		modelOutput->nOS[5] = 1;
		modelOutput->nSO[5]++;
	SD_allocOutputMatrix(modelOutput,6,0);
		cleanVector(states,0,6);

		cleanVector(outputs,0,6);

		sprintf(modelOutput->variable[0].name,"C2");
		sprintf(modelOutput->variable[1].name,"CP");
		sprintf(modelOutput->variable[2].name,"pM");
		sprintf(modelOutput->variable[3].name,"M");
		sprintf(modelOutput->variable[4].name,"Y");
		sprintf(modelOutput->variable[5].name,"yP");
		cleanVector(outputs,0,6);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
		modelOutput->SO[1][states[1]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 1;
		modelOutput->SO[2][states[2]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 2;
		modelOutput->SO[3][states[3]++] = 3;
		modelOutput->OS[3][outputs[3]++] = 3;
		modelOutput->SO[4][states[4]++] = 4;
		modelOutput->OS[4][outputs[4]++] = 4;
		modelOutput->SO[5][states[5]++] = 5;
		modelOutput->OS[5][outputs[5]++] = 5;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
