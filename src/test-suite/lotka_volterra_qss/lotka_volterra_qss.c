#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <common/utils.h>


#include <common/model.h>
#include <common/commands.h>
#include <qss/qss_model.h>
#include <classic/classic_model.h>

double __PAR_QSSIntegrator_1_p[4];
double __PAR_QSSIntegrator_1_x0 = 0;
double __PAR_QSSIntegrator_2_p[4];
double __PAR_QSSIntegrator_2_x0 = 0;
double __PAR_WSum_3_p[9];
double __PAR_WSum_3_w[2];
double __PAR_WSum_4_p[9];
double __PAR_WSum_4_w[2];

void
MOD_settings(SD_simulationSettings settings)
{
	 settings->debug = 0;
	 settings->parallel = FALSE;
	 settings->hybrid = FALSE;
	 settings->method = 5;
}

void
MOD_definition(int i, double *x, double *d, double *alg, double t, double *dx)
{
	double tmp10[4];
	double tmp20[4];
	int j30;
	int j40;
	switch(i)
	{
		case 0:
			alg[0] = x[4];
			alg[4] = x[0];
			alg[8] = alg[4]*alg[0];
					alg[20] = alg[8];
					alg[24] = x[0];
	for(j30 = 0;j30 < 4; j30++)
	{
		tmp10[j30] = 0;
	}
	for(j30 = 0; j30 <= 1; j30++)
	{
		tmp10[0] += alg[(j30+5) * 4 + 0] * __PAR_WSum_4_w[j30];
		tmp10[1] += alg[(j30+5) * 4 + 1] * __PAR_WSum_4_w[j30];
		tmp10[2] += alg[(j30+5) * 4 + 2] * __PAR_WSum_4_w[j30];
		tmp10[3] += alg[(j30+5) * 4 + 3] * __PAR_WSum_4_w[j30];
	}
			alg[28] = tmp10[0];
			alg[36] = alg[28];
			dx[1] = alg[36];
			return;
		case 1:
			alg[0] = x[4];
			alg[4] = x[0];
			alg[8] = alg[4]*alg[0];
					alg[12] = x[4];
					alg[16] = alg[8];
	for(j40 = 0;j40 < 4; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+3) * 4 + 0] * __PAR_WSum_3_w[j40];
		tmp20[1] += alg[(j40+3) * 4 + 1] * __PAR_WSum_3_w[j40];
		tmp20[2] += alg[(j40+3) * 4 + 2] * __PAR_WSum_3_w[j40];
		tmp20[3] += alg[(j40+3) * 4 + 3] * __PAR_WSum_3_w[j40];
	}
			alg[32] = tmp20[0];
			alg[40] = alg[32];
			dx[1] = alg[40];
			return;
	}
}

void
MOD_dependencies(int i, double *x, double *d, double *alg, double t, double *der, int *map)
{
	double tmp10[4];
	double tmp20[4];
	int j30;
	int j40;
	switch(i)
	{
		case 0:
			alg[0] = x[4];
			alg[4] = x[0];
			alg[8] = alg[4]*alg[0];
			alg[12] = x[4];
			alg[16] = alg[8];
			alg[20] = alg[8];
			alg[24] = x[0];
	for(j30 = 0;j30 < 4; j30++)
	{
		tmp10[j30] = 0;
	}
	for(j30 = 0; j30 <= 1; j30++)
	{
		tmp10[0] += alg[(j30+5) * 4 + 0] * __PAR_WSum_4_w[j30];
		tmp10[1] += alg[(j30+5) * 4 + 1] * __PAR_WSum_4_w[j30];
		tmp10[2] += alg[(j30+5) * 4 + 2] * __PAR_WSum_4_w[j30];
		tmp10[3] += alg[(j30+5) * 4 + 3] * __PAR_WSum_4_w[j30];
	}
			alg[28] = tmp10[0];
	for(j40 = 0;j40 < 4; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+3) * 4 + 0] * __PAR_WSum_3_w[j40];
		tmp20[1] += alg[(j40+3) * 4 + 1] * __PAR_WSum_3_w[j40];
		tmp20[2] += alg[(j40+3) * 4 + 2] * __PAR_WSum_3_w[j40];
		tmp20[3] += alg[(j40+3) * 4 + 3] * __PAR_WSum_3_w[j40];
	}
			alg[32] = tmp20[0];
			alg[36] = alg[28];
			alg[40] = alg[32];
			der[0 + 1] = alg[36];
			der[4 + 1] = alg[40];
			return;
		case 1:
			alg[0] = x[4];
			alg[4] = x[0];
			alg[8] = alg[4]*alg[0];
			alg[12] = x[4];
			alg[16] = alg[8];
			alg[20] = alg[8];
			alg[24] = x[0];
	for(j30 = 0;j30 < 4; j30++)
	{
		tmp10[j30] = 0;
	}
	for(j30 = 0; j30 <= 1; j30++)
	{
		tmp10[0] += alg[(j30+5) * 4 + 0] * __PAR_WSum_4_w[j30];
		tmp10[1] += alg[(j30+5) * 4 + 1] * __PAR_WSum_4_w[j30];
		tmp10[2] += alg[(j30+5) * 4 + 2] * __PAR_WSum_4_w[j30];
		tmp10[3] += alg[(j30+5) * 4 + 3] * __PAR_WSum_4_w[j30];
	}
			alg[28] = tmp10[0];
	for(j40 = 0;j40 < 4; j40++)
	{
		tmp20[j40] = 0;
	}
	for(j40 = 0; j40 <= 1; j40++)
	{
		tmp20[0] += alg[(j40+3) * 4 + 0] * __PAR_WSum_3_w[j40];
		tmp20[1] += alg[(j40+3) * 4 + 1] * __PAR_WSum_3_w[j40];
		tmp20[2] += alg[(j40+3) * 4 + 2] * __PAR_WSum_3_w[j40];
		tmp20[3] += alg[(j40+3) * 4 + 3] * __PAR_WSum_3_w[j40];
	}
			alg[32] = tmp20[0];
			alg[36] = alg[28];
			alg[40] = alg[32];
			der[0 + 1] = alg[36];
			der[4 + 1] = alg[40];
			return;
	}
}

void
MOD_output(int i, double *x, double *d, double *alg, double t, double *out)
{
	switch(i)
	{
		case 0:
			out[0] = x[4];
			return;
		case 1:
			out[0] = x[0];
			return;
	}
}

void
QSS_initializeDataStructs(QSS_simulator simulator)
{
	int *outputs = (int*)malloc(2*sizeof(int));
	int *states = (int*)malloc(2*sizeof(int));
	int i0;
	simulator->data = QSS_Data(2,0,0,0,11,"lotka_volterra_qss");
  QSS_data modelData = simulator->data;
  const double t = 0;

	// Allocate main data structures.
	__PAR_QSSIntegrator_1_x0 = 5.000000000000000000000000e-01;
	__PAR_QSSIntegrator_2_x0 = 5.000000000000000000000000e-01;
	for(i0 = 0; i0 <= 0;i0++)
	{
		modelData->x[(i0+1) * 4] = __PAR_QSSIntegrator_1_x0;
	}
	for(i0 = 0; i0 <= 0;i0++)
	{
		modelData->x[i0 * 4] = __PAR_QSSIntegrator_2_x0;
	}
	// Initialize model code.
		__PAR_QSSIntegrator_1_p[(0)] = 0.0;
		__PAR_QSSIntegrator_1_p[(1)] = 9.999999999999999547481118e-07;
		__PAR_QSSIntegrator_1_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_QSSIntegrator_1_p[(3)] = 5.000000000000000000000000e-01;
		__PAR_QSSIntegrator_2_p[(0)] = 0.0;
		__PAR_QSSIntegrator_2_p[(1)] = 9.999999999999999547481118e-07;
		__PAR_QSSIntegrator_2_p[(2)] = 1.000000000000000020816682e-03;
		__PAR_QSSIntegrator_2_p[(3)] = 5.000000000000000000000000e-01;
		__PAR_WSum_3_p[(0)] = 1.000000000000000055511151e-01;
		__PAR_WSum_3_p[(1)] = (-1.000000000000000055511151e-01);
		__PAR_WSum_3_p[(2)] = 0.0;
		__PAR_WSum_3_p[(3)] = 0.0;
		__PAR_WSum_3_p[(4)] = 0.0;
		__PAR_WSum_3_p[(5)] = 0.0;
		__PAR_WSum_3_p[(6)] = 0.0;
		__PAR_WSum_3_p[(7)] = 0.0;
		__PAR_WSum_3_p[(8)] = 2.0;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_3_w[(i0)] = __PAR_WSum_3_p[(i0)];
	}
		__PAR_WSum_4_p[(0)] = 1.000000000000000055511151e-01;
		__PAR_WSum_4_p[(1)] = (-1.000000000000000055511151e-01);
		__PAR_WSum_4_p[(2)] = 0.0;
		__PAR_WSum_4_p[(3)] = 0.0;
		__PAR_WSum_4_p[(4)] = 0.0;
		__PAR_WSum_4_p[(5)] = 0.0;
		__PAR_WSum_4_p[(6)] = 0.0;
		__PAR_WSum_4_p[(7)] = 0.0;
		__PAR_WSum_4_p[(8)] = 2.0;
	for(i0 = 0; i0 <= 1; i0++)
	{
		__PAR_WSum_4_w[(i0)] = __PAR_WSum_4_p[(i0)];
	}
	modelData->nDS[0]++;
	modelData->nDS[0]++;
	modelData->nDS[1]++;
	modelData->nDS[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	modelData->nSD[0]++;
	modelData->nSD[1]++;
	QSS_allocDataMatrix(modelData);
	// Initialize model data.
	// Initialize model time.
		cleanVector(states,0,2);

	modelData->DS[0][states[0]++] = 0;
	modelData->DS[0][states[0]++] = 1;
	modelData->DS[1][states[1]++] = 0;
	modelData->DS[1][states[1]++] = 1;
		cleanVector(states,0,2);

	modelData->SD[0][states[0]++] = 0;
	modelData->SD[1][states[1]++] = 0;
	modelData->SD[0][states[0]++] = 1;
	modelData->SD[1][states[1]++] = 1;
	simulator->time = QSS_Time(2,0,0,0,ST_Binary,NULL);

	simulator->output = SD_Output("lotka_volterra_qss",2,0,2,NULL,0,0,CI_Step,SD_Memory,MOD_output);
SD_output modelOutput = simulator->output;

		modelOutput->nOS[0] = 1;
		modelOutput->nSO[1]++;
		modelOutput->nOS[1] = 1;
		modelOutput->nSO[0]++;
	SD_allocOutputMatrix(modelOutput,2,0);
		cleanVector(states,0,2);

		cleanVector(outputs,0,2);

		sprintf(modelOutput->variable[0].name,"QSSIntegrator_1_y[1]");
		sprintf(modelOutput->variable[1].name,"QSSIntegrator_2_y[1]");
		cleanVector(outputs,0,2);

		modelOutput->SO[1][states[1]++] = 0;
		modelOutput->OS[0][outputs[0]++] = 1;
		modelOutput->SO[0][states[0]++] = 1;
		modelOutput->OS[1][outputs[1]++] = 0;
	simulator->model = QSS_Model(MOD_definition,MOD_dependencies,NULL,NULL,NULL);
	free(outputs);
	free(states);
}

void
CLC_initializeDataStructs (CLC_simulator simulator)
{
}
