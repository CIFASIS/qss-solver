#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_vm1 = 0;
double __PAR_vm3 = 0;
double __PAR_vi = 0;
double __PAR_vd = 0;
double __PAR_Kd = 0;
double __PAR_kd = 0;
double __PAR_k1 = 0;
double __PAR_k2 = 0;
double __PAR_k3 = 0;
double __PAR_k4 = 0;
double __PAR_kc = 0;
double __PAR_v2 = 0;
double __PAR_v4 = 0;

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
			dx[1] = __PAR_vi-__PAR_vd*x[6]*(x[0]*1.000000000000000000000000e+00/(__PAR_Kd+x[0]))-__PAR_kd*x[0];
			return;
		case 1:
			dx[1] = (x[0]*1.000000000000000000000000e+00/(__PAR_kc+x[0]))*__PAR_vm1*(1.0-x[3])*1.000000000000000000000000e+00/(__PAR_k1+(1.0-x[3]))-__PAR_v2*x[3]*1.000000000000000000000000e+00/(__PAR_k2+x[3]);
			return;
		case 2:
			dx[1] = x[3]*__PAR_vm3*(1.0-x[6])*1.000000000000000000000000e+00/(__PAR_k3+(1.0-x[6]))-__PAR_v4*x[6]*1.000000000000000000000000e+00/(__PAR_k4+x[6]);
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	switch(i)
	{
		case 0:
			der[0 + 1] = __PAR_vi-__PAR_vd*x[6]*(x[0]*1.000000000000000000000000e+00/(__PAR_Kd+x[0]))-__PAR_kd*x[0];
			der[3 + 1] = (x[0]*1.000000000000000000000000e+00/(__PAR_kc+x[0]))*__PAR_vm1*(1.0-x[3])*1.000000000000000000000000e+00/(__PAR_k1+(1.0-x[3]))-__PAR_v2*x[3]*1.000000000000000000000000e+00/(__PAR_k2+x[3]);
			return;
		case 1:
			der[3 + 1] = (x[0]*1.000000000000000000000000e+00/(__PAR_kc+x[0]))*__PAR_vm1*(1.0-x[3])*1.000000000000000000000000e+00/(__PAR_k1+(1.0-x[3]))-__PAR_v2*x[3]*1.000000000000000000000000e+00/(__PAR_k2+x[3]);
			der[6 + 1] = x[3]*__PAR_vm3*(1.0-x[6])*1.000000000000000000000000e+00/(__PAR_k3+(1.0-x[6]))-__PAR_v4*x[6]*1.000000000000000000000000e+00/(__PAR_k4+x[6]);
			return;
		case 2:
			der[0 + 1] = __PAR_vi-__PAR_vd*x[6]*(x[0]*1.000000000000000000000000e+00/(__PAR_Kd+x[0]))-__PAR_kd*x[0];
			der[6 + 1] = x[3]*__PAR_vm3*(1.0-x[6])*1.000000000000000000000000e+00/(__PAR_k3+(1.0-x[6]))-__PAR_v4*x[6]*1.000000000000000000000000e+00/(__PAR_k4+x[6]);
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
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(3*sizeof(int));
	int *states = (int*)malloc(3*sizeof(int));
	int i0;
	simulator->data = QSS_Data(3,0,0,0,0,"GOLDBETER");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_vm1 = 3.000000000000000000000000e+00;
	__PAR_vm3 = 1.000000000000000000000000e+00;
	__PAR_vi = 2.500000000000000138777878e-02;
	__PAR_vd = 2.500000000000000000000000e-01;
	__PAR_Kd = 2.000000000000000041633363e-02;
	__PAR_kd = 1.000000000000000020816682e-02;
	__PAR_k1 = 5.000000000000000104083409e-03;
	__PAR_k2 = 5.000000000000000104083409e-03;
	__PAR_k3 = 5.000000000000000104083409e-03;
	__PAR_k4 = 5.000000000000000104083409e-03;
	__PAR_kc = 5.000000000000000000000000e-01;
	__PAR_v2 = 1.500000000000000000000000e+00;
	__PAR_v4 = 5.000000000000000000000000e-01;
	modelData->x[0] = 1.000000000000000020816682e-02;
	modelData->x[3] = 1.000000000000000020816682e-02;
	modelData->x[6] = 1.000000000000000020816682e-02;
	// Initialize model code.
	modelData->nDS[0] = 2;
	modelData->nDS[1] = 2;
	modelData->nDS[2] = 2;
	modelData->nSD[0]++;
	modelData->nSD[2]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[1]++;
	modelData->nSD[2]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,3);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 2;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
	modelData->DS[2][states[2]++] = 1;
	modelData->DS[2][states[2]++] = 2;
		cleanVector(states,0,3);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[2][states[2]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
	modelData->SD[1][states[1]++] = 2;
	modelData->SD[2][states[2]++] = 2;
	simulator->time = QSS_Time(3,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("GOLDBETER",3,0,3,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[0]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[1]++;
		modelOutput->nOS[2] = 1;
		modelOutput->nSO[2]++;
	SD_allocOutputMatrix(modelOutput,3,0);
		cleanVector(states,0,3);

		cleanVector(outputs,0,3);

		sprintf(modelOutput->variable[0].name,"c");
		sprintf(modelOutput->variable[1].name,"m");
		sprintf(modelOutput->variable[2].name,"x");
		cleanVector(outputs,0,3);

		modelOutput->SO[0][states[0]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 0;
		modelOutput->SO[1][states[1]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 1;
		modelOutput->SO[2][states[2]++] = 2;
		modelOutput->OS[2][outputs[2]++] = 2;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
